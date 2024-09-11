all: linkedList.h serialHuffman.c
	gcc -o serialHuffman serialHuffman.c -lm

decompress: linkedList.h decompress.c
	gcc -o decompress decompress.c -lm