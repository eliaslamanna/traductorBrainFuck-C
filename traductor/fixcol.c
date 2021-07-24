#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

int fixcol(const char* archivo, int columnas){
	FILE* arch = fopen(archivo, "r");
	if(arch == NULL){
		fprintf(stderr,"Error: archivo fuente inaccesible\n");
		return -1;
	}
	char caracteres[columnas + 1];
	while(fgets(caracteres,columnas + 1,arch)){
		if(caracteres[0] != '\n'){
			fprintf(stdout,"%s",caracteres);
			if(caracteres[strlen(caracteres) - 1] != '\n'){
				fprintf(stdout,"\n");
			}
			memset(caracteres, 0, columnas + 1);
		}
	}
	fclose(arch);
	return 0;
}

int main(int argc, char *argv[]){
	if(argc != 3){
		fprintf(stderr,"Error: Cantidad erronea de parametros\n");
		return -1;
	}
	const char *archivo = argv[1];
	int columnas = atoi(argv[2]);
	fixcol(archivo, columnas);
	return 0;
}