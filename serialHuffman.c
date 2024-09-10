#include "linkedList.h"
#include "fileLinkedList.h"
#include "dirent.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void processFiles(const FileLinkedList *fileList, LinkedList *list)
{
    FileNode *current = fileList->head;
    while (current != NULL)
    {
        FILE *file = fopen(current->filename, "r");
        if (file == NULL)
        {
            printf("Error al abrir el archivo\n");
            exit(1);
        }

        char ch;
        while ((ch = fgetc(file)) != EOF)
        {
            incrementFrequency(list, ch);
        }

        fclose(file);
        current = current->next;
    }
}

void writeTreeAux(FILE *file, Node *root) {
    if (root == NULL) {
        return;
    }

    if (root->data != '\0') {
        fwrite("1", sizeof(char), 1, file);
        fwrite(&root->data, sizeof(char), 1, file);
    } else {
        fwrite("0", sizeof(char), 1, file);
    }

    writeTreeAux(file, root->left);
    writeTreeAux(file, root->right);
}

void writeTree(FILE* file, Node *root) {
    //writeTreeAux(file, root);
    if (root == NULL) {
        return;
    }

    if (root->data != '\0') {
        fwrite("1", sizeof(char), 1, file);
        fwrite(&root->data, sizeof(char), 1, file);
    } else {
        fwrite("0", sizeof(char), 1, file);
    }

    writeTreeAux(file, root->left);
    writeTreeAux(file, root->right);
}

void compress(const FileLinkedList* fileList, LinkedList *list)
{
    FILE *compressed = fopen("compressed.bin", "wb");
    if (compressed == NULL)
    {
        printf("Error al crear el archivo de compresión\n");
        exit(1);
    }

    writeTree(compressed, list->head);

    FileNode *current = fileList->head;
    while(current != NULL){
        FILE *file = fopen(current->filename, "r");
        if (file == NULL)
        {
            printf("Error al abrir el archivo\n");
            exit(1);
        }

        //Insertar encabezado
        char* name = (char*)malloc(strlen(current->filename) + 1);
        strcpy(name, current->filename);
        strcat(name, "@");
        int len = strlen(name);
        fwrite(name, sizeof(char), len, compressed);

        //Escribir archivo comprimido
        char c;
        //printf("Comprimiendo archivo: %s\n", current->filename);
        while ((c = fgetc(file)) != EOF)
        {
            fprintf(compressed, "%s", searchCode(list->head, c));
        }

        fclose(file);
        fprintf(compressed, "@");

        current = current->next;
    }

    /*current = fileList->head;
    while(current != NULL){
        FILE *file = fopen(current->filename, "r");
        if (file == NULL)
        {
            printf("Error al abrir el archivo\n");
            exit(1);
        }

        //Escribir archivo comprimido
        char c;
        //printf("Comprimiendo archivo: %s\n", current->filename);
        while ((c = fgetc(file)) != EOF)
        {
            fprintf(compressed, "%s", searchCode(list->head, c));
        }

        fclose(file);
        fprintf(compressed, "@");
        current = current->next;
    }*/

    fclose(compressed);
}

// Función para crear un nodo del árbol de Huffman
Node* createNodeTree(char data){
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Función recursiva para leer el árbol de Huffman desde el archivo
Node* readTreeAux(FILE *file) {
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
        node->left = readTreeAux(file);
        node->right = readTreeAux(file);
        return node;
    }

    return NULL;
}

// Función para leer el árbol de Huffman desde el archivo
Node* readTree(FILE* file) {
    Node* root = readTreeAux(file);
    return root;
}

// Función para imprimir el árbol de Huffman (para pruebas)
void printTreeMain(Node *root, int depth) {
    if (root == NULL) {
        return;
    }

    for (int i = 0; i < depth; i++) {
        printf("  ");
    }

    if (root->data == '\0') {
        printf("Node\n");
    } else {
        printf("Leaf: %c\n", root->data);
    }

    printTree(root->left, depth + 1);
    printTree(root->right, depth + 1);
}

void decompress()
{
    FILE *compressed = fopen("compressed.bin", "rb");
    if (compressed == NULL)
    {
        printf("Error al abrir el archivo de compresión\n");
        exit(1);
    }

    //Leer árbol
    Node* root  = readTree(compressed);
    //printTreeMain(root, 0);

    //Leer encabezado
    char c;
    char name[512];
    while((c = fgetc(compressed)) != '@'){
        strcat(name, &c);
    }
    strcat(name, "\0");

    char url[512];
    strcpy(url, "decompressed/");
    strcat(url, name);

    printf("URL: %s\n", url);

    FILE *decompressed = fopen("decompressed.txt", "w");
    if (decompressed == NULL)
    {
        printf("Error al crear el archivo de descompresión\n");
        fclose(compressed);
        exit(1);
    }

    //char c;
    char code[100];
    int cont = 0;
    while ((c = fgetc(compressed)) != EOF){
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

    fclose(compressed);
    fclose(decompressed);
    return;
}

int main()
{
    LinkedList list;
    list.head = NULL;
    
    // Lista para guardar nombres de los archivos
    FileLinkedList fileList;
    fileList.head = NULL;

    //const char* filename = "libros_gutenberg/2_Moby_Dick;_Or,_The_Whale_by_Herman_Melville_(72669).txt";
    //const char* filename = "prueba.txt";
    //processFile(filename, &list);
    const char* dirpath = "./libros_gutenberg";
    DIR *dir = opendir(dirpath);
    if(dir == NULL){
        perror("opendir");
        return 0;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) { // Solo archivos regulares
            char filepath[1024];
            snprintf(filepath, sizeof(filepath), "%s/%s", dirpath, entry->d_name);
            addFileNode(&fileList, filepath);
        }
    }

    //printFileList(&fileList);
    //int size = sizeFileList(&fileList);
    //printf("Size: %d\n", size);
    closedir(dir);

    processFiles(&fileList, &list);
    
    //printf("Lista desordenada: \n");
    //printList(&list);

    sortList(&list);

   // printf("Lista ordenada: \n");
    //printList(&list);


    //printf("Creación del arbol: \n");
    createTree(&list);
  //  printTree(list.head, 0);

    asignCodes(list.head, "", 0);
    //printTree(list.head, 0);

    compress(&fileList, &list);
    freeList(&list);
    
    //decompress();


    return 0;

    //printf("Lista desordenada: \n");
    //printList(&list);

    //sortList(&list);

    //printf("Lista ordenada: \n");
    //printList(&list);

   // printf("Creación del arbol: \n");
   // createTree(&list);
    // printTree(list.head, 0);

  //  asignCodes(list.head, "", 0);
    // printTree(list.head, 0);

    //compress(filename, &list);
  //  freeList(&list);
   // decompress();

    return 0;
}