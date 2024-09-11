#include "linkedList.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

// Función para crear un nodo del árbol de Huffman
Node* createNodeTree(char data){
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Función recursiva para leer el árbol de Huffman desde el archivo
Node* readTree(FILE *file) {
    int bit = fgetc(file);
    if (bit == EOF) {
        return NULL;
    }

    if (bit == '1') {
        char data = fgetc(file);
        Node* node = createNodeTree(data);
        return node;
    } else if (bit == '0') {
        Node* node = createNodeTree('\0');
        node->left = readTree(file);
        node->right = readTree(file);
        return node;
    }

    return NULL;
}

void decompressFiles(FILE *compressed, Node* root, char* folder) {
    while (!feof(compressed)) {
        // Leer encabezado
        char c;
        char name[512] = {0};
        while ((c = fgetc(compressed)) != '@' && c != EOF) {
            strncat(name, &c, 1); 
        }
        if (c == EOF) {
            break; 
        }

        // Obtener solo el nombre del archivo a partir de la ruta completa
        char *filename = strrchr(name, '/');
        if (filename != NULL) {
            filename++; 
        } else {
            filename = name; 
        }

        char url[512];
        strcpy(url, folder);
        strcat(url, "/");
        strcat(url, filename);

        FILE *decompressed = fopen(url, "w");
        if (decompressed == NULL) {
            printf("Error al crear el archivo de descompresión\n");
            fclose(compressed);
            exit(1);
        }

        // Leer y descomprimir el contenido
        char code[100] = {0};
        int cont = 0;
        while ((c = fgetc(compressed)) != EOF) {
            if (c == '@') {
                break; 
            }
            code[cont] = c;
            code[cont + 1] = '\0';
            cont++;

            char character = getCharacter(root, code);
            if (character != '\0') {
                fprintf(decompressed, "%c", character);
                memset(code, 0, sizeof(code));
                cont = 0;
            }
        }

        fclose(decompressed);
    }
}

void decompress(char* filename, char* folder)
{
    FILE *compressed = fopen(filename, "rb");
    if (compressed == NULL)
    {
        printf("Error al abrir el archivo de compresión\n");
        exit(1);
    }

    //Leer árbol
    Node* root  = readTree(compressed);

    if(mkdir(folder, 0755) == -1){
        printf("Error al crear la carpeta de descompresión\n");
        fclose(compressed);
        exit(1);
    }

    decompressFiles(compressed, root, folder);

    fclose(compressed);
    return;
}

int main(int argc, char *argv[])
{
    if(argc < 3){
        printf("Ingrese el nombre del archivo comprimido y el nombre de la carpeta para descompresión\n");
        return 1;
    }

    char* filename = argv[1];
    char* folder = argv[2];


    decompress(filename, folder);

    printf("Descompresión exitosa\n");
    return 0;
}