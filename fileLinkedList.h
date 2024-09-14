#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct FileNode {
    char* filename;
    int position;
    struct FileNode* next;
} FileNode;

typedef struct FileLinkedList {
    FileNode* head;
} FileLinkedList;

// Función para crear un nuevo nodo
FileNode* createFileNode(const char* filename, int position) {
    FileNode* newNode = (FileNode*)malloc(sizeof(FileNode));
    if (newNode == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    newNode->filename = strdup(filename);
    if (newNode->filename == NULL) {
        perror("strdup");
        free(newNode);
        exit(EXIT_FAILURE);
    }
    newNode->position = position;
    newNode->next = NULL;
    return newNode;
}

// Función para agregar un nodo a la lista
void addFileNode(FileLinkedList* list, const char* filename, int position) {
    FileNode* newNode = createFileNode(filename, position);
    if (list->head == NULL) {
        list->head = newNode;
    } else {
        FileNode* current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

// Función para imprimir la lista
void printFileList(const FileLinkedList* list) {
    FileNode* current = list->head;
    while (current != NULL) {
        printf("Filname: %s, Position: %d\n", current->filename, current->position);
        current = current->next;
    }
}

// Función para liberar la memoria de la lista
void freeFileList(FileLinkedList* list) {
    FileNode* current = list->head;
    while (current != NULL) {
        FileNode* temp = current;
        current = current->next;
        free(temp->filename);
        free(temp);
    }
}

// Funcion para obtener el tamaño de la lista
int sizeFileList(const FileLinkedList* list) {
    int size = 0;
    FileNode* current = list->head;
    while (current != NULL) {
        size++;
        current = current->next;
    }
    return size;
}