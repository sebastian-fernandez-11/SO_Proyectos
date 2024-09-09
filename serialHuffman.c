#include "linkedList.h"

#include <unistd.h>

void processFile(const char *filename, LinkedList *list)
{
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

void compress(const char *filename, LinkedList *list)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error al abrir el archivo\n");
        exit(1);
    }

    FILE *compressed = fopen("compressed.bin", "wb");
    if (compressed == NULL)
    {
        printf("Error al crear el archivo de compresión\n");
        exit(1);
    }

    char jostin[] = "Jos";
    size_t length = strlen(jostin);
    for (size_t i = 0; i < length; ++i) {
        unsigned char ch = jostin[i];
        for (int bit = 7; bit >= 0; --bit) {
            unsigned char bitValue = (ch >> bit) & 1;
            fwrite(&bitValue, sizeof(unsigned char), 1, compressed);
        }
    }

    return;

    char c;
    while ((c = fgetc(file)) != EOF)
    {
        fprintf(compressed, "%s", searchCode(list->head, c));
    }

    fclose(file);
    fclose(compressed);
}

void decompress(LinkedList *list)
{
    FILE *compressed = fopen("compressed.bin", "rb");
    if (compressed == NULL)
    {
        printf("Error al abrir el archivo de compresión\n");
        exit(1);
    }

    FILE *decompressed = fopen("decompressed.txt", "w");
    if (decompressed == NULL)
    {
        printf("Error al crear el archivo de descompresión\n");
        fclose(compressed);
        exit(1);
    }

    char c;
    char code[100];
    int cont = 0;
    while ((c = fgetc(compressed)) != EOF){
        code[cont] = c;
        code[cont + 1] = '\0';
        cont++;

        char character = getCharacter(list->head, code);
        if (character != '\0')
        {
            fprintf(decompressed, "%c", character);
            memset(code, 0, sizeof(code));
            cont = 0;
        }
    }

    fclose(compressed);
    fclose(decompressed);
}

int main()
{
    LinkedList list;
    list.head = NULL;

    //const char* filename = "libros_gutenberg/2_Moby_Dick;_Or,_The_Whale_by_Herman_Melville_(72669).txt";
    const char* filename = "prueba.txt";
    processFile(filename, &list);

    printf("Lista desordenada: \n");
    //printList(&list);

    sortList(&list);

    printf("Lista ordenada: \n");
    //printList(&list);

    printf("Creación del arbol: \n");
    createTree(&list);
    // printTree(list.head, 0);

    asignCodes(list.head, "", 0);
    // printTree(list.head, 0);

    compress(filename, &list);
  //  decompress(&list);

    freeList(&list);

    return 0;
}