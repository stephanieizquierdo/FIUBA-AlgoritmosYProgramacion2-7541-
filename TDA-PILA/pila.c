#include "pila.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define TAMANIO_MIN 3
#define EXITO 0
#define PILA_NO_EXISTE 0
#define ERROR -1

#define ACHICAR 0.6
#define COND_CORTE 0.5
#define AGRANDAR 2

void pila_destruir(pila_t* pila){
	if (!pila) {
		return;
	}
	if (pila->elementos) {
		free(pila->elementos);
	}
	free(pila);
}

pila_t* pila_crear(){
	pila_t* pila = malloc(sizeof(pila_t));
	if (!pila) {
		return NULL;
	}
	pila->tope = 0;
	pila->elementos = malloc((TAMANIO_MIN)*sizeof(void*));
	if (!pila->elementos) {
		pila_destruir(pila);
		return NULL;
	}
	pila->tamanio = TAMANIO_MIN;

	return pila;
}

bool pila_vacia(pila_t* pila){
	if (!pila) {
		return true;
	}
	return ((pila->tope) == 0);
}

int pila_cantidad(pila_t* pila){
	if (!pila) {
		return PILA_NO_EXISTE;
	}
	return (pila -> tope);
}

/*
 *Redimensiona la pila recibida al tamaño que recibe.
 *El tamaño a redimencionar la pila no puede ser menor al tamaño minimo de la pila ya preestablecido (3)
 *En caso de que haya podido redimensionar bien devuelve EXITO (1) sino ERROR (-1).
 */
int redimensionar(pila_t* pila, int nuevo_tam){
	if (nuevo_tam < TAMANIO_MIN) {
		nuevo_tam = TAMANIO_MIN;
	}
    void** aux = NULL;
	aux = realloc((pila->elementos), (unsigned int)nuevo_tam*sizeof(void*));
	if(!aux){
		return ERROR;
	}
	pila->elementos = aux;
	pila->tamanio = nuevo_tam;

	return EXITO;
}
/*
 * Devuelve true en caso de que quede espacio en la pila. Los parametros son positivos
 */
bool hay_espacio(int tope,int tamanio){
	return tope < tamanio;
}

int pila_apilar(pila_t* pila, void* elemento){
	if(!pila || !pila->elementos){
		return ERROR;
	}
	if (hay_espacio(pila->tope,pila->tamanio)){
		pila->elementos[(pila->tope)] = elemento;
	} else {
        int nuevo_tam = (pila->tamanio) * AGRANDAR;
		if (redimensionar(pila, nuevo_tam) == EXITO){
			pila->elementos[(pila->tope)] = elemento;
		} else {
			return ERROR;
		}
	}
	(pila->tope) ++;
	return EXITO;
}


int pila_desapilar(pila_t* pila){
	if (!pila || !pila->elementos || pila_vacia(pila)) {
		return ERROR;
	}
    int tam_de_corte = (pila->tamanio) * (int)COND_CORTE;

	if (pila->tope < tam_de_corte) {
        int nuevo_tam = (pila->tamanio) * (int)(ACHICAR);
		if (redimensionar(pila, nuevo_tam) != EXITO) {
			return ERROR;
		}
	}
	(pila->tope)--;
	return EXITO;
}

void* pila_tope(pila_t* pila){
	if(!pila || pila_vacia(pila) || pila->elementos==NULL){
		return NULL;
	}
	return(pila->elementos[(pila->tope)-1]);
}
