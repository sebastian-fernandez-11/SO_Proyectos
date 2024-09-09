#include "linkedList.h"

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

    FILE *compressed = fopen("compressed.txt", "w");
    if (compressed == NULL)
    {
        printf("Error al crear el archivo de compresión\n");
        exit(1);
    }

    char c;
    while ((c = fgetc(file)) != EOF)
    {
        fprintf(compressed, searchCode(list->head, c));
    }

    fclose(file);
    fclose(compressed);
}

void decompress(LinkedList *list)
{

    FILE *compressed = fopen("compressed.txt", "rb"); // "rb" para lectura binaria
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
    char buffer[256];
    int cont = 0;
    char character;
    while ((c = fgetc(compressed)) != EOF){
        buffer[cont] = c;
        buffer[cont + 1] = '\0';

        character = getCharacter(list->head, buffer);
        
        if (character != '\0')
        {
            //printf("Código: %s\n", buffer);
            //printf("Caracter: %c\n", character);
            fprintf(decompressed, "%c", character);
            memset(buffer, 0, sizeof(buffer));
            cont = -1;
        }

        cont++;
    }

    fclose(compressed);
    fclose(decompressed);
}

int main()
{
    LinkedList list;
    list.head = NULL;

    const char* filename = "libros_gutenberg/2_Moby_Dick;_Or,_The_Whale_by_Herman_Melville_(72669).txt";
    //const char* filename = "prueba.txt";
    processFile(filename, &list);

    printf("Lista desordenada: \n");
    printList(&list);

    sortList(&list);

    printf("Lista ordenada: \n");
    printList(&list);

    printf("Creación del arbol: \n");
    createTree(&list);
    // printTree(list.head, 0);

    asignCodes(list.head, "", 0);
    // printTree(list.head, 0);

    compress(filename, &list);
    decompress(&list);

    freeList(&list);

    return 0;
}