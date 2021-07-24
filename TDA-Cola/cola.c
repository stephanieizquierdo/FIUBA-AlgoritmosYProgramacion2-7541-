#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "cola.h"

#define EXITO 0
#define ERROR -1
#define NO_EXISTE_COLA 0


cola_t* cola_crear(){
	cola_t* cola = calloc(1, sizeof(cola_t));
	return cola; // si calloc falla, cola es NULL.
}

int cola_encolar(cola_t* cola, void* elemento){
	if(!cola){
		return ERROR;
	}
	nodo_t* nodo_nuevo = malloc(sizeof(nodo_t));
	if(!nodo_nuevo){
		return ERROR;
	}
	nodo_nuevo->elemento = elemento;
	nodo_nuevo->siguiente = NULL;

	if (!cola->nodo_inicio) {
		cola->nodo_inicio = nodo_nuevo;
		cola->nodo_fin = nodo_nuevo;
	} else {
		cola->nodo_fin->siguiente = nodo_nuevo;
		cola->nodo_fin = nodo_nuevo;
	}

	(cola->cantidad)++;
	return EXITO;
}

int cola_desencolar(cola_t* cola){
	if (!cola || cola_vacia(cola)) {
		return ERROR;
	}
	nodo_t* aux = cola->nodo_inicio;
	(cola->nodo_inicio) = ((cola->nodo_inicio)->siguiente);
	free(aux);

	(cola->cantidad)--;
    if (cola->cantidad == 0) cola->nodo_fin = NULL;

	return EXITO;
}

bool cola_vacia(cola_t* cola){
	if (!cola) {
		return true;
	}
	return ((cola->nodo_inicio == NULL) && (cola->cantidad == 0));
}

int cola_cantidad(cola_t* cola){
	if (!cola) {
		return NO_EXISTE_COLA;
	}
	return(cola->cantidad);
}

void* cola_primero(cola_t* cola){
	if (!cola || cola_vacia(cola)) {
		return NULL;
	}
	return((cola->nodo_inicio)->elemento);
}

void cola_destruir(cola_t* cola){
	if (!cola) {
		return;
	}
	while (!cola_vacia(cola)) {
		cola_desencolar(cola);
	}
	free(cola);
}
