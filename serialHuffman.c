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

    char* text = "Jos";
    size_t length = strlen(text);
    for (size_t i = 0; i < length; ++i) {
        unsigned char ch = text[i];
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

void writeBitsToFile(const char *filename, const char *text) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error al abrir el archivo para escritura");
        exit(1);
    }

    size_t length = strlen(text);
    for (size_t i = 0; i < length; ++i) {
        unsigned char ch = text[i];
        for (int bit = 7; bit >= 0; --bit) {
            unsigned char bitValue = (ch >> bit) & 1;
            fwrite(&bitValue, sizeof(unsigned char), 1, file);
        }
    }

    fclose(file);
}

void readBitsFromFile(const char *filename, char *buffer, size_t bufferSize) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error al abrir el archivo para lectura");
        exit(1);
    }

    size_t index = 0;
    unsigned char bitValue;
    unsigned char ch = 0;
    int bitCount = 0;

    while (fread(&bitValue, sizeof(unsigned char), 1, file) == 1 && index < bufferSize - 1) {
        ch = (ch << 1) | bitValue;
        bitCount++;
        if (bitCount == 8) {
            buffer[index++] = ch;
            ch = 0;
            bitCount = 0;
        }
    }

    buffer[index] = '\0';
    fclose(file);
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

    size_t index = 0;
    unsigned char bitValue;
    unsigned char ch = 0;
    int bitCount = 0;
    char buffer[100];

    while (fread(&bitValue, sizeof(unsigned char), 1, compressed) == 1 && index < 100 - 1) {
        ch = (ch << 1) | bitValue;
        bitCount++;
        if (bitCount == 8) {
            buffer[index++] = ch;
            ch = 0;
            bitCount = 0;
        }
    }

    buffer[index] = '\0'; // Añadir el carácter nulo al final

    printf("Cadena leída: %s\n", buffer);

    //free(buffer);
    return;

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
    return;
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
    decompress(&list);

    freeList(&list);

    return 0;
}