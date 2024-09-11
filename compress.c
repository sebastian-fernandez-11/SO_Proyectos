#include "linkedList.h"
#include "fileLinkedList.h"
#include "dirent.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void loadFiles(const char* dirpath, FileLinkedList* fileList) {
    DIR *dir = opendir(dirpath);
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) { // Solo archivos regulares
            char filepath[1024];
            snprintf(filepath, sizeof(filepath), "%s/%s", dirpath, entry->d_name);
            addFileNode(fileList, filepath);
        }
    }

    closedir(dir);
}

void processFile(char* filename, LinkedList *list) {
    FILE *file = fopen(filename, "r");
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
}

void processFiles(const FileLinkedList *fileList, LinkedList *list) {
    FileNode *current = fileList->head;
    while (current != NULL)
    {
        processFile(current->filename, list);
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

void compress(const FileLinkedList* fileList, LinkedList *list) {
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
        if (file == NULL) {
            printf("Error al abrir el archivo\n");
            exit(1);
        }

        //Insertar encabezado
        char* name = (char*)malloc(strlen(current->filename) + 1);
        strcpy(name, current->filename);
        strcat(name, "@");
        int len = strlen(name);
        fwrite(name, sizeof(char), len, compressed);

        char c;
        while ((c = fgetc(file)) != EOF)
        {
            fprintf(compressed, "%s", searchCode(list->head, c));
        }

        fclose(file);
        fprintf(compressed, "@");

        current = current->next;
    }

    fclose(compressed);
}

void serial_compress(const char* dirpath, LinkedList *list, FileLinkedList *fileList) {
    loadFiles(dirpath, fileList);
    processFiles(fileList, list);
    sortList(list);
    createTree(list);
    asignCodes(list->head, "", 0);
    compress(fileList, list);
}

int main(int argc, char *argv[]) {
    if(argc < 2) {
        printf("Elija el algoritmo de compresión a utilizar.\n");
        exit(1);
    } else if(argc > 2) {
        printf("Demasiados argumentos.\n");
        exit(1);
    }

    LinkedList list;
    list.head = NULL;

    FileLinkedList fileList;
    fileList.head = NULL;

    const char* dirpath = "./libros_gutenberg";
    if(argv[1][0] == '0') {
        serial_compress(dirpath, &list, &fileList);
    } else {
        printf("Algoritmo no reconocido.\n");
        exit(1);
    }

    freeList(&list);

    return 0;
}