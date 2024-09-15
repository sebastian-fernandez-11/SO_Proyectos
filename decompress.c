#include "linkedList.h"
#include "fileLinkedList.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

typedef struct ThreadArgs {
    FILE* file;
    Node* root;
    char* folder;
    FileNode* current;
} ThreadArgs;

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

//Función para leer el encabezado del archivo comprimido
void readPositions(FILE* compressed, FileLinkedList *positions){
    //Obtiene los caracteres extra del encabezado
    char c;
    char extraChar[64] = {0};
    while((c = fgetc(compressed)) != '@'){
        strncat(extraChar, &c, 1);
    }

    //Obtiene la cantidad de archivos del encabezado
    char cantFiles[64] = {0};
    while((c = fgetc(compressed)) != '@'){
        strncat(cantFiles, &c, 1);
    }

    char position[512] = {0};
    int files = atoi(cantFiles);
    int cantExtra = atoi(extraChar);

    //Lee y guarda las posiciones de cada libro
    int cont = 0;    
    while(cont < files){
        while((c = fgetc(compressed)) != '@'){
            strncat(position, &c, 1);
        }
        if(c == '@'){
            addFileNode(positions, " ", atoi(position) + cantExtra);
            memset(position, 0, sizeof(position));
        }
        else{
            strncat(position, &c, 1);
        }
        cont++;
    }
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
   // printf("Descomprimiendo %s\n", url);

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

void serial_decompress(char *filename, char *folder)
{
    FILE* compressed = readBinFile(filename);
    FileLinkedList positions;
    positions.head = NULL;

    readPositions(compressed, &positions);
    Node* root = readTree(compressed);
    consultFolder(folder);
    
    //Descomprimir todos los archivos
    FileNode* current = positions.head;
    while(current->next != NULL){
        decompressFile(compressed, root, folder, current->position);
        current = current->next;
    }
 
    fclose(compressed);
    return;
}

void fork_decompress(char* filename, char* folder){
    FileLinkedList positions;
    positions.head = NULL;
    
    // Abrir el archivo comprimido y leer las posiciones y el árbol
    FILE* compressed = readBinFile(filename);
    readPositions(compressed, &positions);
    Node* root = readTree(compressed);
    fclose(compressed); 

    consultFolder(folder);

    FileNode* current = positions.head;
    while(current != NULL){
        pid_t pid = fork();
        if (pid == 0) {
            FILE* compressed_child = readBinFile(filename);
            decompressFile(compressed_child, root, folder, current->position);
            fclose(compressed_child);
            exit(0);
        } 
        current = current->next;
    }
    
    return;
}

void* concurrent_decompress_file(void* args){
    ThreadArgs* arguments = (ThreadArgs*) args;
    decompressFile(arguments->file, arguments->root, arguments->folder, arguments->current->position);
    fclose(arguments->file);
    return NULL;
}

void concurrent_decompress(char* filename, char* folder){
    FileLinkedList positions;
    positions.head = NULL;
    //printf("Descomprimiendo en concurrencia...\n");
    // Abrir el archivo comprimido y leer las posiciones y el árbol
    FILE* compressed = readBinFile(filename);
    readPositions(compressed, &positions);
    Node* root = readTree(compressed);
    fclose(compressed); 
    
    consultFolder(folder);

    int i = 0;
    int threadsSize = sizeFileList(&positions);
    pthread_t threads[threadsSize];
    ThreadArgs args[threadsSize];


    FileNode* current = positions.head;
    while(current != NULL && i < threadsSize){
        args[i].current = current;
        args[i].file = readBinFile(filename);
        args[i].root = root;
        args[i].folder = folder;

        pthread_create(&threads[i], NULL, concurrent_decompress_file, &args[i]);

        current = current->next;
        i++;
    }

    for(i = 0; i < threadsSize; i++){
        pthread_join(threads[i], NULL);
    }
    
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

    struct timespec start, end;
    long spend_time;

    if (strcmp(exec, "0") == 0)
    {
        clock_gettime(CLOCK_MONOTONIC, &start);
        serial_decompress(filename, folder);
        clock_gettime(CLOCK_MONOTONIC, &end);
        spend_time = (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);
        printf("Descompresion serial finalizada. Tiempo de ejecución: %ld ns\n", spend_time);
        spend_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
        printf("Tiempo de ejecución: %ld s\n", spend_time);
    }
    else if(strcmp(exec, "1") == 0){
        clock_gettime(CLOCK_MONOTONIC, &start);
        fork_decompress(filename, folder);
        clock_gettime(CLOCK_MONOTONIC, &end);
        spend_time = (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);
        printf("Descompresion en paralelo finalizada. Tiempo de ejecución: %ld ns\n", spend_time);
        spend_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
        printf("Tiempo de ejecución: %ld s\n", spend_time);
    } else if(strcmp(exec, "2") == 0){
        clock_gettime(CLOCK_MONOTONIC, &start);
        concurrent_decompress(filename, folder);
        clock_gettime(CLOCK_MONOTONIC, &end);
        spend_time = (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);
        printf("Descompresion concurrente finalizada. Tiempo de ejecución: %ld ns\n", spend_time);
        spend_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
        printf("Tiempo de ejecución: %ld s\n", spend_time);
    } else
    {
        printf("Tipo de ejecución no válido.\n");
        return 1;
    }

    return 0;
}