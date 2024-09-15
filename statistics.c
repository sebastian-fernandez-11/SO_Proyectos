#include "compress.c"
#include "decompress.c"
#include <time.h>


// haz una funcion que para cada algoritmo de compresion y descompresion se ejecute 50 veces y se guarde su tiempo promedio de ejecucion en un archivo
// falta agregar la compresion

void testCompress(int exec){
    FileLinkedList fileList;
    fileList.head = NULL;
    LinkedList list;
    list.head = NULL;
    const char* dirpath = "./libros_gutenberg";

    struct timespec start, end;
    long spend_time;
    long total_time = 0;
    for(int i = 0; i < 50; i++){
        if (exec == 0)
        {
            clock_gettime(CLOCK_MONOTONIC, &start);
            serial_compress(dirpath, &list, &fileList);
            clock_gettime(CLOCK_MONOTONIC, &end);
            spend_time = (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);
            total_time += spend_time;
        }
        else if(exec == 1){
            clock_gettime(CLOCK_MONOTONIC, &start);
            fork_compress(dirpath, &list, &fileList);
            clock_gettime(CLOCK_MONOTONIC, &end);
            spend_time = (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);
            total_time += spend_time;
        } else if(exec == 2){
            clock_gettime(CLOCK_MONOTONIC, &start);
            concurrent_compress(dirpath, &list, &fileList);
            clock_gettime(CLOCK_MONOTONIC, &end);
            spend_time = (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);
            total_time += spend_time;
        } else
        {
            printf("Tipo de ejecución no válido.\n");
            return;
        }
    }
    total_time /= 50;
    FILE* file = fopen("results.txt", "a");
    switch (exec)
    {
    case 0:
        fprintf(file, "Serial\n");
        break;
    
    case 1:
        fprintf(file, "Fork\n");
        break;
    
    case 3:
        fprintf(file, "Concurrent\n");
        break;

    default:
        break;
    }
    fprintf(file, "%ld\n", total_time);
    fclose(file);
}

void testDecompress(char* filename, char* folder, int exec){
    struct timespec start, end;
    long spend_time;
    long total_time = 0;
    for(int i = 0; i < 50; i++){
        if (exec == 0)
        {
            clock_gettime(CLOCK_MONOTONIC, &start);
            serial_decompress(filename, folder);
            clock_gettime(CLOCK_MONOTONIC, &end);
            spend_time = (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);
            total_time += spend_time;
        }
        else if(exec == 1){
            clock_gettime(CLOCK_MONOTONIC, &start);
            fork_decompress(filename, folder);
            clock_gettime(CLOCK_MONOTONIC, &end);
            spend_time = (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);
            total_time += spend_time;
        } else if(exec == 2){
            clock_gettime(CLOCK_MONOTONIC, &start);
            concurrent_decompress(filename, folder);
            clock_gettime(CLOCK_MONOTONIC, &end);
            spend_time = (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);
            total_time += spend_time;
        } else
        {
            printf("Tipo de ejecución no válido.\n");
            return;
        }
    }
    total_time /= 50;
    FILE* file = fopen("results.txt", "a");
    switch (exec)
    {
    case 0:
        fprintf(file, "Serial\n");
        break;
    
    case 1:
        fprintf(file, "Fork\n");
        break;
    
    case 3:
        fprintf(file, "Concurrent\n");
        break;

    default:
        break;
    }
    fprintf(file, "%ld\n", total_time);
    fclose(file);
}

int main()
{
    testCompress("1");

    return 0;
}
