#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "pila.h"
#include "vector.h"

#define MAX_CANT_ARG_VALIDOS 2
#define LARGO_INICIAL 200

vector_t* copiar_texto(FILE* input){
	vector_t* vector = vector_crear(LARGO_INICIAL);
	if(vector == NULL){
		return NULL;
	}
	for(int i = 0;i < vector->tam;i ++){
		vector->datos[i] = 0;
	}
	char *linea = NULL;
	size_t tam = 0;
	ssize_t caracteres = 1;
	caracteres = getline(&linea,&tam,input);
	int i = 0;
	int j = 0;
	while(caracteres != -1){
		while(linea[j] != '\n' && linea[j] != '\0'){
			if(linea[j] == '+' || linea[j] == '-' || linea[j] == '.' || linea[j] == ',' || linea[j] == '>' || linea[j] == '<' || linea[j] == '[' || linea[j] == ']'){
				if(i == vector->tam){
					if(!vector_redimensionar(vector, vector->tam * 2)){
						vector_destruir(vector);
						free(linea);
						return NULL;
					}
				}
				vector->datos[i] = linea[j];
				i ++;
			}
			j ++;
		}
		if(input == stdin){
			free(linea);
			return vector;
		}
		caracteres = getline(&linea,&tam,input);
		j = 0;
	}
	free(linea);
	return vector;
}

int brainfuck(FILE* input){
	vector_t* codigo = copiar_texto(input);
	if(codigo == NULL){
		return -1;
	}
	int pos_vector = 0;
	int pos_linea = 0;
	pila_t* pila_jump = pila_crear();
	if(pila_jump == NULL){
		return -1;
	} 
	pila_t* pila_pos_linea = pila_crear();
	if(pila_pos_linea == NULL){
		return -1;
	}
	vector_t* vector = vector_crear(LARGO_INICIAL);
	if(vector == NULL){
		return -1;
	}
	for(int i = 0;i < vector->tam;i ++){
		vector->datos[i] = 0;
	}
	vector_t* vector_pos = vector_crear(LARGO_INICIAL);
	if(vector_pos == NULL){
		return -1;
	}
	for(int i = 0;i < vector_pos->tam;i ++){
		vector->datos[i] = 0;
	}
	int i = 0;
	while((char)codigo->datos[pos_linea] != '\0'){
		if(codigo->datos[pos_linea] == '+'){
			if(vector->datos[pos_vector] == 255){
				vector->datos[pos_vector] = 0;
			}
			else{
				vector->datos[pos_vector] ++;
			}
		}
		else if(codigo->datos[pos_linea] == '-'){
			if(vector->datos[pos_vector] == 0){
				vector->datos[pos_vector] = 255;
			}
			else{
				vector->datos[pos_vector] --;
			}
		}
		else if(codigo->datos[pos_linea] == '.'){
			fprintf(stdout,"%c", vector->datos[pos_vector]);
		}
		else if(codigo->datos[pos_linea] == ','){
			vector->datos[pos_vector] = getchar();
		}
		else if(codigo->datos[pos_linea] == '>'){
			if(pos_vector == vector->tam){
				if(!vector_redimensionar(vector, vector->tam * 2)){
					return -1;
				}
			}
			pos_vector ++;
		}
		else if(codigo->datos[pos_linea] == '<'){
			pos_vector --;
		}
		else if(codigo->datos[pos_linea] == '[' && vector->datos[pos_vector] == 0){
			char caracter = '[';
			pila_apilar(pila_jump, &caracter);
			while(!pila_esta_vacia(pila_jump)){
				pos_linea ++;
				if(codigo->datos[pos_linea] == '['){
					char nuevo_car = '[';
					pila_apilar(pila_jump, &nuevo_car);
				}
				else if(codigo->datos[pos_linea] == ']'){
					pila_desapilar(pila_jump);
				}
			}
		}
		else if(codigo->datos[pos_linea] == '[' && vector->datos[pos_vector] != 0){
			vector_pos->datos[i] = pos_linea;
			pila_apilar(pila_pos_linea, &vector_pos->datos[i]);
			i ++;
		}
		else if(codigo->datos[pos_linea] == ']' && vector->datos[pos_vector] != 0){
			pos_linea = *(int*)pila_ver_tope(pila_pos_linea);
		}
		else if(codigo->datos[pos_linea] == ']' && vector->datos[pos_vector] == 0){
			pila_desapilar(pila_pos_linea);
			i --;
		}
		pos_linea ++;
	}
	vector_destruir(vector);
	vector_destruir(vector_pos);
	vector_destruir(codigo);
	pila_destruir(pila_pos_linea);
	pila_destruir(pila_jump);
	return 0;
}

int main(int argc, char* argv[]){
	if(argc > MAX_CANT_ARG_VALIDOS){
		fprintf(stderr, "Error: Cantidad erronea de parametros\n");
		return -1;
	}
	FILE* input;
	if(argc == MAX_CANT_ARG_VALIDOS){
		const char* archivo = argv[1];
		FILE* codigo = fopen(archivo, "r");
		if(codigo == NULL){
			fprintf(stderr,"Error: archivo fuente inaccesible\n");
			return -1;
		}
		input = codigo;
		brainfuck(input);
		fclose(codigo);
	}
	else{
		input = stdin;
		brainfuck(input);
	}
	return 0;
}