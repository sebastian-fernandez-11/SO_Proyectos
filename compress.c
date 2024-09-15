#include "linkedList.h"
#include "fileLinkedList.h"
#include "dirent.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/file.h> 
#include <libgen.h>
#include <sys/stat.h>

pthread_mutex_t file_mutex;

FILE* openWriteFile(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    return file;
}

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
            addFileNode(fileList, filepath, strlen(filepath));
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

    writeTree(file, root->left);
    writeTree(file, root->right);
}

void compress_file(LinkedList *list, char *filename, FILE *compressed, FILE *temp) {
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error al abrir el archivo\n");
        exit(1);
    }

    if(temp != NULL) {
        fprintf(temp, "%ld", ftell(compressed));
        fprintf(temp, "@");
    }
    
    // Insertar nombre del archivo
    int nameLen = strlen(filename) + 2; // +2 para '@' y '\0'
    char *name = (char *)malloc(nameLen);
    if (name == NULL)
    {
        perror("malloc");
        exit(1);
    }
    strcpy(name, filename);
    strcat(name, "@");

    fwrite(name, sizeof(char), nameLen - 1, compressed); // -1 para no escribir el '\0'

    free(name);

    char c;
    while ((c = fgetc(file)) != EOF)
    {   
        fprintf(compressed, "%s", searchCode(list->head, c));
    }

    fclose(file);
    fprintf(compressed, "@");
}

void compress_files(const FileLinkedList* fileList, LinkedList *list) {
    FILE *compressed = fopen("compressed.txt", "w");
    if (compressed == NULL)
    {
        printf("Error al crear el archivo de compresión\n");
        exit(1);
    }

    writeTree(compressed, list->head);

    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL)
    {
        printf("Error al crear el archivo temporal\n");
        exit(1);
    }

    fprintf(temp, "%d", sizeFileList(fileList));
    fprintf(temp, "@");

    FileNode *current = fileList->head;
    while(current != NULL){
        compress_file(list, current->filename, compressed, temp);
        current = current->next;
    }

    fclose(temp);
    fclose(compressed);
}

void generate_bin(){
    // Insertar contenido de temp al inicio de compressed
    FILE *originalCompressed = fopen("compressed.txt", "r");
    if (originalCompressed == NULL) {
        printf("Error al abrir el archivo de compresión original\n");
        exit(1);
    }
    
    FILE *tempFile = fopen("temp.txt", "r");
    if (tempFile == NULL) {
        printf("Error al abrir el archivo temporal\n");
        exit(1);
    }
    
    // Crear un nuevo archivo para almacenar el contenido combinado
    FILE *newCompressed = fopen("new_compressed.bin", "wb");
    if (newCompressed == NULL) {
        printf("Error al crear el nuevo archivo de compresión\n");
        exit(1);
    }
    
    // Calcular la cantidad de caracteres en el archivo temporal
    fseek(tempFile, 0, SEEK_END);
    long tempFileSize = ftell(tempFile);
    fseek(tempFile, 0, SEEK_SET);
    
    // Calcular la longitud de la representación en cadena de tempFileSize
    char tempFileSizeStr[20];
    sprintf(tempFileSizeStr, "%ld", tempFileSize);
    size_t tempFileSizeStrLen = strlen(tempFileSizeStr);
    
    // Sumar la longitud de tempFileSizeStr y el carácter '@' a tempFileSize
    long totalSize = tempFileSize + tempFileSizeStrLen + 1; // +1 para '@'
    
    // Escribir la cantidad total de caracteres y un '@' al inicio del nuevo archivo
    fprintf(newCompressed, "%ld@", totalSize);
    
    // Copiar contenido de temp al nuevo archivo de compresión
    char buffer[1024];
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), tempFile)) > 0) {
        fwrite(buffer, 1, bytesRead, newCompressed);
    }
    
    // Copiar contenido original de compressed al nuevo archivo de compresión
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), originalCompressed)) > 0) {
        fwrite(buffer, 1, bytesRead, newCompressed);
    }
    
    fclose(originalCompressed);
    fclose(tempFile);
    fclose(newCompressed);
    
 
    if (rename("new_compressed.bin", "compressed.bin") != 0) {
        perror("Error al renombrar el nuevo archivo de compresión");
        exit(1);
    }
    remove("compressed.txt");
    remove("temp.txt");
}

void multi_generate_bin(const char* temp_dir) {
    FILE *compressed = fopen("compressed.txt", "a");
    if (compressed == NULL) {
        printf("Error al abrir el archivo compressed.txt\n");
        exit(1);
    }

    FILE *temp = fopen("temp.txt", "a");
    if (temp == NULL) {
        printf("Error al abrir el archivo temp.txt\n");
        exit(1);
    }

    DIR *dir = opendir(temp_dir);
    if (dir == NULL) {
        printf("Error al abrir el directorio temporales\n");
        exit(1);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) { // Solo archivos regulares
            char filepath[512];
            snprintf(filepath, sizeof(filepath), "%s/%s", temp_dir, entry->d_name);

            FILE *input = fopen(filepath, "rb");
            if (input == NULL) {
                printf("Error al abrir el archivo temporal %s\n", filepath);
                exit(1);
            }

            fprintf(temp, "%ld", ftell(compressed));
            fprintf(temp, "@");

            // Leer el contenido del archivo y escribirlo en compressed.bin
            char buffer[1024];
            size_t bytes;
            while ((bytes = fread(buffer, 1, sizeof(buffer), input)) > 0) {
                fwrite(buffer, 1, bytes, compressed);
            }

            fclose(input);

            // Eliminar el archivo temporal
            if (remove(filepath) != 0) {
                printf("Error al eliminar el archivo temporal %s\n", filepath);
            }
        }
    }

    //eliminar directorio temporales
    if (rmdir(temp_dir) != 0) {
        printf("Error al eliminar el directorio temporales\n");
    }

    closedir(dir);
    fclose(compressed);
    fclose(temp);
}

void serial_compress(const char* dirpath, LinkedList *list, FileLinkedList *fileList) {
    loadFiles(dirpath, fileList);
    processFiles(fileList, list);
    sortList(list);
    createTree(list);
    asignCodes(list->head, "", 0);
    compress_files(fileList, list);
    generate_bin();
}

void fork_compress(const char* dirpath, LinkedList *list, FileLinkedList *fileList) {

    loadFiles(dirpath, fileList);

    processFiles(fileList, list);
    sortList(list);
    createTree(list);
    asignCodes(list->head, "", 0);

    FILE *compressed = fopen("compressed.txt", "w");
    if (compressed == NULL) {
        printf("Error al crear el archivo de compresión\n");
        exit(1);
    }

    writeTree(compressed, list->head);
    fclose(compressed); 

    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        printf("Error al crear el archivo temporal\n");
        exit(1);
    }

    fprintf(temp, "%d", sizeFileList(fileList));
    fprintf(temp, "@");
    fclose(temp); 

    struct stat st = {0};
    if (stat("temporales", &st) == -1) {
        if (mkdir("temporales", 0755) == -1) {
            perror("Error al crear la carpeta de descompresión");
            exit(1);
        }
    }
    
    FileNode* current = fileList->head;
    while (current != NULL) {
        pid_t pid = fork();
        if (pid == 0) {

            char url[512];
            strcpy(url, "temporales");
            strcat(url, "/");
            strcat(url, basename(current->filename));

            FILE *compressed_child = fopen(url, "a");
            if (compressed_child == NULL) {
                printf("Error al abrir el archivo de compresión en el proceso hijo\n");
                exit(1);
            }
            
            compress_file(list, current->filename, compressed_child, NULL);

            fclose(compressed_child);

            exit(0); 
        } 
        current = current->next;
    }

    // Proceso padre espera a que todos los hijos terminen
    int status;
    while (wait(&status) > 0);

    multi_generate_bin("temporales");
    generate_bin();
}

void execute_time(struct timespec start, struct timespec end) {
    long spend_time = (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);
    printf("Compresion finalizada. Tiempo de ejecución: %ld ns\n", spend_time);
    spend_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Tiempo de ejecución: %ld s\n", spend_time);
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

    struct timespec start, end;

    const char* dirpath = "./libros_gutenberg";
    if(argv[1][0] == '0') {
        clock_gettime(CLOCK_MONOTONIC, &start);
        serial_compress(dirpath, &list, &fileList);
        clock_gettime(CLOCK_MONOTONIC, &end);
        execute_time(start, end);
        

    }
    else if(argv[1][0] == '1') {
        clock_gettime(CLOCK_MONOTONIC, &start);
        fork_compress(dirpath, &list, &fileList);
        clock_gettime(CLOCK_MONOTONIC, &end);
        execute_time(start, end);
    } 
    else {
        printf("Algoritmo no válido.\n");
        exit(1);
    }

    freeList(&list);

    return 0;
}