#include "linkedList.h"

void processFile(const char* filename, LinkedList* list) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error al abrir el archivo\n");
        exit(1);
    }

    char ch;
    while ((ch = fgetc(file)) != EOF) {
        incrementFrequency(list, ch);
    }

    fclose(file);
}

int main() {
    LinkedList list;
    list.head = NULL;

    const char* filename = "libros_gutenberg/2_Moby_Dick;_Or,_The_Whale_by_Herman_Melville_(72669).txt";
    processFile(filename, &list);

    printf("Lista desordenada: \n");
    printList(&list);

    sortList(&list);

    printf("Lista ordenada: \n");
    printList(&list);

    freeList(&list);

    return 0;
}