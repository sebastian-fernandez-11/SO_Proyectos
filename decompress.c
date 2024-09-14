#include "linkedList.h"
#include "fileLinkedList.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// Función para crear un nodo del árbol de Huffman
Node *createNodeTree(char data)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Función recursiva para leer el árbol de Huffman desde el archivo
Node *readTree(FILE *file)
{   
    int bit = fgetc(file);
    if (bit == EOF)
    {
        return NULL;
    }

    if (bit == '1')
    {
        char data = fgetc(file);
        Node *node = createNodeTree(data);
        return node;
    }
    else if (bit == '0')
    {
        Node *node = createNodeTree('\0');
        node->left = readTree(file);
        node->right = readTree(file);
        return node;
    }

    return NULL;
}

//Función para abrir un archivo binario
FILE* readBinFile(char* filename){
    FILE* compressed = fopen(filename, "rb");
    if(compressed == NULL){
        printf("Error al abrir el archivo de compresión\n");
        exit(1);
    }
    return compressed;
}

//Función para consultar y/o crear una carpeta
void consultFolder(char* folder){
    struct stat st = {0};
    if (stat(folder, &st) == -1) {
        if (mkdir(folder, 0755) == -1) {
            perror("Error al crear la carpeta de descompresión");
            exit(1);
        }
    }
}

void decompressFile(FILE *compressed, Node *root, char *folder, int fileNumber)
{
    /*int contAt = 0;
    char at; 
    while(contAt != fileNumber){
        at = fgetc(compressed);
        if(at == '@'){
            contAt++;
        }
    }*/

   printf("File number: %d\n", fileNumber);

    fseek(compressed, fileNumber, SEEK_SET);

    char c;
    char name[512] = {0};
    while ((c = fgetc(compressed)) != '@' && c != EOF)
    {
        strncat(name, &c, 1);
    }

    // Obtener solo el nombre del archivo a partir de la ruta completa
    char *filename = strrchr(name, '/');
    if (filename != NULL)
    {
        filename++;
    }
    else
    {
        filename = name;
    }

    char url[512];
    strcpy(url, folder);
    strcat(url, "/");
    strcat(url, filename);

    printf("Descomprimiendo archivo: %s\n", url);

    FILE *decompressed = fopen(url, "w");
    if (decompressed == NULL)
    {
        printf("Error al crear el archivo de descompresión\n");
        fclose(compressed);
        exit(1);
    }

    // Leer y descomprimir el contenido
    char code[100] = {0};
    int cont = 0;
    while ((c = fgetc(compressed)) != EOF)
    {
        if (c == '@')
        {
            break;
        }
        code[cont] = c;
        code[cont + 1] = '\0';
        cont++;

        char character = getCharacter(root, code);
        if (character != '\0')
        {
            fprintf(decompressed, "%c", character);
            memset(code, 0, sizeof(code));
            cont = 0;
        }
    }

    fclose(decompressed);
}

void decompressFiles(FILE *compressed, Node *root, char *folder)
{
    while (!feof(compressed))
    {
        // Leer encabezado
        char c;
        char name[512] = {0};
        while ((c = fgetc(compressed)) != '@' && c != EOF)
        {
            strncat(name, &c, 1);
        }
        if (c == EOF)
        {
            break;
        }

        // Obtener solo el nombre del archivo a partir de la ruta completa
        char *filename = strrchr(name, '/');
        if (filename != NULL)
        {
            filename++;
        }
        else
        {
            filename = name;
        }

        char url[512];
        strcpy(url, folder);
        strcat(url, "/");
        strcat(url, filename);

        FILE *decompressed = fopen(url, "w");
        if (decompressed == NULL)
        {
            printf("Error al crear el archivo de descompresión\n");
            fclose(compressed);
            exit(1);
        }

        // Leer y descomprimir el contenido
        char code[100] = {0};
        int cont = 0;
        while ((c = fgetc(compressed)) != EOF)
        {
            if (c == '@')
            {
                break;
            }
            code[cont] = c;
            code[cont + 1] = '\0';
            cont++;

            char character = getCharacter(root, code);
            if (character != '\0')
            {
                fprintf(decompressed, "%c", character);
                memset(code, 0, sizeof(code));
                cont = 0;
            }
        }

        fclose(decompressed);
    }
}

void serial_decompress(char *filename, char *folder)
{
    FILE* compressed = readBinFile(filename);
    FileLinkedList positions;
    positions.head = NULL;

    char c;
    char extraChar[64] = {0};
    while((c = fgetc(compressed)) != '@'){
        strncat(extraChar, &c, 1);
    }

    char cantFiles[64] = {0};
    while((c = fgetc(compressed)) != '@'){
        strncat(cantFiles, &c, 1);
    }

    char position[512] = {0};
    int files = atoi(cantFiles);

    int cantExtra = atoi(extraChar);

    printf("Extra char: %s\n", extraChar);
    printf("Cantidad de archivos: %d\n", files);
    int cont = 0;    
    while(cont < files){
        while((c = fgetc(compressed)) != '@'){
            strncat(position, &c, 1);
        }
        if(c == '@'){
            addFileNode(&positions, " ", atoi(position) + cantExtra);
            memset(position, 0, sizeof(position));
        }
        else{
            strncat(position, &c, 1);
        }
        cont++;
    }

    //print positions
    printf("Leyendo posiciones\n");
    FileNode* current = positions.head;
    while(current != NULL){
        printf("%d\n", current->position);
        current = current->next;
    }

    Node* root = readTree(compressed);
    consultFolder(folder);

    //decompressFiles(compressed, root, folder);
    decompressFile(compressed, root, folder, positions.head->next->position);
    //decompressFile(compressed, root, folder, 0);

    fclose(compressed);
    return;
}

void fork_decompress(char* filename, char* folder){
    FILE* compressed = readBinFile(filename);
    Node* root = readTree(compressed);
    consultFolder(folder);

    int fileNumber = 0;
    //while (fileNumber < 177) {
        pid_t pid = fork();
        if (pid == 0) {
            // Proceso hijo
            printf("Proceso hijo número: %d\n", fileNumber);
            decompressFile(compressed, root, folder, fileNumber);
            //fclose(compressed);
            exit(0);
        } else if (pid > 0) {
            // Proceso padre
            fileNumber += 2;
        } else {
            printf("Error al crear el proceso hijo");
            exit(1);
        }
    //}

    /*// Esperar a que todos los procesos hijos terminen
    while (fileNumber > 0) {
        wait(NULL);
        fileNumber -= 2;
    }*/

    fclose(compressed);
    return;
}

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        printf("Ingrese el nombre del archivo comprimido, el nombre de la carpeta para descompresión y el tipo de ejecución.\n");
        return 1;
    }
    if(argc > 4){
        printf("Demasiados argumentos.\n");
        return 1;
    }

    char *filename = argv[1];
    char *folder = argv[2];
    char *exec = argv[3];

    if (strcmp(exec, "0") == 0)
    {
        serial_decompress(filename, folder);
    }
    else if(strcmp(exec, "1") == 0){
        fork_decompress(filename, folder);
    }

    printf("Descompresión exitosa\n");
    return 0;
}