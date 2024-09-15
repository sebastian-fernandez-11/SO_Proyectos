// implement a linked list in c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char data;
    int frequency;

    struct Node* next;
    struct Node* left;
    struct Node* right;

    char* code;
    int nCode;
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
        if (sorted == NULL || (sorted->frequency > current->frequency) || 
            (sorted->frequency == current->frequency && sorted->data > current->data)) {
            current->next = sorted;
            sorted = current;
        } else {
            Node* temp = sorted;
            while (temp->next != NULL && 
                   (temp->next->frequency < current->frequency || 
                   (temp->next->frequency == current->frequency && temp->next->data < current->data))) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
        current = next;
    }

    list->head = sorted;
}


// Crear un árbol de Huffman a partir de la lista enlazada
void createTree(LinkedList* list) {
    while (list->head != NULL && list->head->next != NULL) {
        // Tomar los dos nodos con menor frecuencia
        Node* right = list->head;
        Node* left = list->head->next;

        // Actualizar la cabeza de la lista
        list->head = list->head->next->next;

        // Insertar el nuevo nodo padre en la lista
        appendForTree(list, '\0', left->frequency + right->frequency, left, right);

        // Ordenar la lista
        sortList(list);
    }

    // La cabeza de la lista ahora es la raíz del árbol de Huffman
    if (list->head != NULL) {
        //printf("Árbol de Huffman creado. Raíz: %c\n", list->head->data);
    }
}

// Función auxiliar para imprimir espacios
void printSpaces(int count) {
    for (int i = 0; i < count; i++) {
        printf("  ");
    }
}

// Función para imprimir el árbol de Huffman de manera jerárquica
void printTree(Node* root, int level) {
    if (root == NULL) {
        return;
    }

    printSpaces(level);
    if(root->code == NULL){
        printf("(\'%c\', %d)\n", root->data, root->frequency);
    }
    else{
        printf("(\'%c\', %d, %s, %d)\n", root->data, root->frequency, root->code, root->nCode);
    }

    if (root->left != NULL) {
        printSpaces(level);
        printf("Left:\n");
        printTree(root->left, level + 1);
    }

    if (root->right != NULL) {
        printSpaces(level);
        printf("Right:\n");
        printTree(root->right, level + 1);
    }
}

void asignCodes(Node* root, char* code, int level) {
    if (root == NULL) {
        return;
    }

    if (root->left == NULL && root->right == NULL) {
        root->code = (char*) malloc((strlen(code) + 1) * sizeof(char));
        strcpy(root->code, code);
        
        root->nCode = level;
        return;
    }

    if (root->left != NULL) {
        char leftCode[100];
        strcpy(leftCode, code);
        strcat(leftCode, "0");
        asignCodes(root->left, leftCode, level + 1);
    }

    if (root->right != NULL) {
        char rightCode[100];
        strcpy(rightCode, code);
        strcat(rightCode, "1");
        asignCodes(root->right, rightCode, level + 1);
    }
}

char* searchCode(Node* root, char c) {
    if (root == NULL) {
        return NULL;
    }

    // Si el nodo actual contiene el carácter buscado
    if (root->data == c) {
        return root->code;
    }

    // Buscar en el subárbol izquierdo
    char* leftCode = searchCode(root->left, c);
    if (leftCode != NULL) {
        return leftCode;
    }

    // Buscar en el subárbol derecho
    return searchCode(root->right, c);
}

char getCharacter(Node* current, char* code){
    int len = strlen(code);
    for(int i = 0; i < len; i++){
        if(code[i] == '0' && current->left != NULL){
            current = current->left;
        }
        else if(code[i] == '1' && current->right != NULL){
            current = current->right;
        }
    }

    return current->data;
}