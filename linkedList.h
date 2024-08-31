// implement a linked list in c
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    char data;
    int frequency;
    struct Node* next;
} Node;

typedef struct LinkedList {
    Node* head;
} LinkedList;

// Crear un nuevo nodo
Node* createNode(char data, int frequency) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("Error al asignar memoria\n");
        exit(1);
    }
    newNode->data = data;
    newNode->frequency = frequency;
    newNode->next = NULL;
    return newNode;
}

// Añadir un nodo al final de la lista
void append(LinkedList* list, char data, int frequency) {
    Node* newNode = createNode(data, frequency);
    if (list->head == NULL) {
        list->head = newNode;
        return;
    }
    Node* temp = list->head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
}

// Imprimir la lista
void printList(LinkedList* list) {
    Node* temp = list->head;
    while (temp != NULL) {
        printf("(\'%c\', %d) -> ", temp->data, temp->frequency);
        temp = temp->next;
    }
    printf("NULL\n");
}

// Liberar la memoria de la lista
void freeList(LinkedList* list) {
    Node* temp;
    while (list->head != NULL) {
        temp = list->head;
        list->head = list->head->next;
        free(temp);
    }
}

// Incrementar la frecuencia de un carácter en la lista
void incrementFrequency(LinkedList* list, char data) {
    Node* temp = list->head;
    while (temp != NULL) {
        if (temp->data == data) {
            temp->frequency++;
            return;
        }
        temp = temp->next;
    }
    append(list, data, 1);
}

// Ordenar la lista enlazada por frecuencia usando ordenación por inserción
void sortList(LinkedList* list) {
    if (list->head == NULL || list->head->next == NULL) {
        return;
    }

    Node* sorted = NULL;
    Node* current = list->head;

    while (current != NULL) {
        Node* next = current->next;
        if (sorted == NULL || sorted->frequency >= current->frequency) {
            current->next = sorted;
            sorted = current;
        } else {
            Node* temp = sorted;
            while (temp->next != NULL && temp->next->frequency < current->frequency) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
        current = next;
    }

    list->head = sorted;
}