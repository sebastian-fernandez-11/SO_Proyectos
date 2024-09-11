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

    fclose(compressed);
}

int main()
{
    /*LinkedList list;
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

    //compress(&fileList, &list);
    freeList(&list);*/
    
    decompress();


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