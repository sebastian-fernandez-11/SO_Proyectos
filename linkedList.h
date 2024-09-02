// implement a linked list in c
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    char data;
    int frequency;
    struct Node* next;
    struct Node* left;
    struct Node* right;
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
    newNode->left = NULL;
    newNode->right = NULL;
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

// Añadir un nodo al final de la lista
void appendForTree(LinkedList* list, char data, int frequency, Node* left, Node* right) {
    Node* newNode = createNode(data, frequency);
    newNode->left = left;
    newNode->right = right;
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

// imprimir estructura completa del árbol
void printTotal(LinkedList* list) {
    Node* temp = list->head;
    while (temp != NULL) {
        printf("(\'%c\', %d) -> ", temp->data, temp->frequency);
        if(temp->left != NULL) {
            printf("Left: (\'%c\', %d) -> ", temp->left->data, temp->left->frequency);
        }
        if(temp->right != NULL) {
            printf("Right: (\'%c\', %d) -> ", temp->right->data, temp->right->frequency);
        }
        
        temp = temp->next;
    }
    printf("NULL\n");
}

// Crear un árbol de Huffman a partir de la lista enlazada
void createTree(LinkedList* list) {
    while (list->head != NULL && list->head->next != NULL) {
        // Tomar los dos nodos con menor frecuencia
        Node* left = list->head;
        Node* right = list->head->next;

        // Actualizar la cabeza de la lista
        list->head = list->head->next->next;

        // Insertar el nuevo nodo padre en la lista
        appendForTree(list, '\0', left->frequency + right->frequency, left, right);
        

        printTotal(list);

        // Ordenar la lista
        sortList(list);
        printTotal(list);
        printf("***************************************\n");
    }

    // La cabeza de la lista ahora es la raíz del árbol de Huffman
    if (list->head != NULL) {
        printf("Árbol de Huffman creado. Raíz: %c\n", list->head->data);
    }
}

void printTree(Node* root) {
    if (root == NULL) {
        return;
    }
    printf("(\'%c\', %d) -> ", root->data, root->frequency);
    if(root->left != NULL) {
        printf("Left: ");
        printTree(root->left);
    }
    if(root->right != NULL) {
        printf("Right: ");
        printTree(root->right);
    }
}

