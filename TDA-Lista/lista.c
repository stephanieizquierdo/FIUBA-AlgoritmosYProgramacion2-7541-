#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define EXITO 0
#define ERROR -1

#define NO_ELEMENTOS 0

/*===============================ESTRUCTURAS===================================*/
typedef struct nodo{
	void* elemento;
	struct nodo* siguiente;
} nodo_t;

typedef struct lista{
	nodo_t* nodo_inicio;
	nodo_t* nodo_fin;
	int cantidad;
}lista_t;

typedef struct lista_iterador{
	nodo_t* corriente;
	lista_t* lista;
}lista_iterador_t;

/*=============================FUNCIONES LISTA================================*/

nodo_t* crear_nodo(void* elemento){
    nodo_t* nodo_nuevo = (nodo_t*)calloc(1, sizeof(nodo_t));
	if (!nodo_nuevo) {
		return NULL;
	}
	nodo_nuevo->elemento = elemento;

    return nodo_nuevo;
}

lista_t* lista_crear(){
	lista_t* lista = (lista_t*)calloc(1, sizeof(lista_t));
	return lista; //si calloc falla, lista = NULL entonces retorna null
}

bool lista_vacia(lista_t* lista){
	if (!lista) {
		return true;
	}
	return((lista->cantidad) == 0);
}

int lista_insertar(lista_t* lista, void* elemento){
	if (!lista) {
		return ERROR;
	}
	nodo_t* nodo_nuevo = crear_nodo(elemento);
	if (!nodo_nuevo) {
		return ERROR;
	}
	if (lista->cantidad == 0){
		lista->nodo_inicio = nodo_nuevo;
		lista->nodo_fin = nodo_nuevo;
	} else {
		lista->nodo_fin->siguiente = nodo_nuevo;
		lista->nodo_fin = nodo_nuevo;
	}

	(lista->cantidad)++;
	return EXITO;
}

/*
 *Recibe una lista valida. Inserta el elemento recibido al inicio de la lista recibida
 */
int lista_insertar_inicio(lista_t* lista, void* elemento){
    nodo_t* nodo_nuevo = crear_nodo(elemento);
	if (!nodo_nuevo) {
		return ERROR;
	}
	nodo_nuevo->siguiente = lista->nodo_inicio;

	lista->nodo_inicio = nodo_nuevo;
	(lista->cantidad)++;

	return EXITO;
}

int lista_insertar_en_posicion(lista_t* lista, void* elemento, size_t posicion){
	int estado = ERROR;

	if (!lista) {
		return estado;
	}
	if (posicion >= (lista->cantidad)) {
		estado = lista_insertar(lista, elemento);
	} else if(posicion == 0) {
		estado = lista_insertar_inicio(lista, elemento);
	} else {
        nodo_t* nodo_nuevo = crear_nodo(elemento);
    	if (!nodo_nuevo) {
    		return estado;
    	}

		nodo_t* nodo_actual = lista->nodo_inicio;
		int i = 0;
		while (i < posicion - 1) {
			nodo_actual = nodo_actual->siguiente;
			i ++;
		}

		nodo_nuevo->siguiente = nodo_actual->siguiente;
		nodo_actual->siguiente = nodo_nuevo;
		(lista->cantidad)++;

		estado = EXITO;
	}
	return estado;
}

int lista_borrar(lista_t* lista){
	if(!lista || lista_vacia(lista) || !lista->nodo_primero){
		return ERROR;
	}

	nodo_t* nodo_borrar= lista->nodo_fin;

    if (lista->cantidad == 1) {
		lista->nodo_fin=NULL;
		lista->nodo_inicio=NULL;
		free(nodo_borrar);
	} else {
		nodo_t* nodo_actual = lista->nodo_inicio;
		int i = 0;

		while ((nodo_actual->siguiente) != nodo_borrar) {
			nodo_actual = nodo_actual->siguiente;
			i++;
		}
		lista->nodo_fin=nodo_actual;
		free(nodo_borrar);
	}
	(lista->cantidad)--;
	return EXITO;
}

/*
 *Recibe una lista valida. Borra el primer nodo de la lista.
 */
void lista_borrar_inicio(lista_t* lista){
	nodo_t* aux = lista->nodo_inicio;

	lista->nodo_inicio = lista->nodo_inicio->siguiente;
	free(aux);

	(lista->cantidad)--;
}

int lista_borrar_de_posicion(lista_t* lista, size_t posicion){
	if(!lista || lista_vacia(lista)){
		return ERROR;
	}

	if (posicion>= lista->cantidad) {
		lista_borrar(lista);
	} else if (posicion == 0) {
		lista_borrar_inicio(lista);
	} else {
		nodo_t* nodo_actual = lista->nodo_inicio;
		int i = 0;
		while (i < posicion - 1) {
			nodo_actual = nodo_actual->siguiente;
			i++;
		}
		nodo_t* nodo_borrar = nodo_actual->siguiente;

		nodo_actual->siguiente=(nodo_actual->siguiente)->siguiente;

		free(nodo_borrar);
		(lista->cantidad)--;
	}
	return EXITO;
}

void* lista_elemento_en_posicion(lista_t* lista, size_t posicion){
	if (!lista || lista_vacia(lista) || posicion >= (lista->cantidad)) {
		return NULL;
	}

	nodo_t* nodo_actual = lista->nodo_inicio;

	int i=0;
	while (i < posicion) {
		nodo_actual = nodo_actual->siguiente;
		i++;
	}
	return(nodo_actual->elemento);
}

void* lista_ultimo(lista_t* lista){
	if (!lista || lista_vacia(lista)){
		return NULL;
	}
	return(lista->nodo_fin->elemento);
}

size_t lista_elementos(lista_t* lista){
	if(!lista){
		return NO_ELEMENTOS;
	}
	return((size_t)lista->cantidad);
}

void lista_destruir(lista_t* lista){
	if (!lista){
		return;
	}
	while(!lista_vacia(lista)){
		lista_borrar(lista);
	}
	free(lista);
}

/*================================ITERADOR====================================*/

lista_iterador_t* lista_iterador_crear(lista_t* lista){
	if (!lista) {
		return NULL;
	}
	lista_iterador_t* iterador = (lista_iterador_t*)malloc(sizeof(lista_iterador_t));
	if(!iterador){
		return NULL;
	}
	iterador->corriente = lista->nodo_inicio;
	iterador->lista = lista;

	return iterador;
}

/*
 *Si el iterador no es valido, devolverá false;
 */
bool lista_iterador_tiene_siguiente(lista_iterador_t* iterador){
	if(!iterador || !iterador->lista){
		return false;
	}
	return(iterador->corriente);
}

void* lista_iterador_siguiente(lista_iterador_t* iterador){
	if(!iterador|| !iterador->lista){
		return NULL;
	}
	void* elemento = (iterador->corriente)->elemento;

	if(iterador->corriente){
		iterador->corriente = iterador->corriente->siguiente;
	}
	return(elemento);
}

void lista_iterador_destruir(lista_iterador_t* iterador){
	free(iterador); //if iterador is null, nothing happens
}

void lista_con_cada_elemento(lista_t* lista, void (*funcion)(void*)){
	if(!lista || lista_vacia(lista)){
		return;
	}
	lista_iterador_t* iterador = lista_iterador_crear(lista);
	if(!iterador){
		return;
	}
	while(lista_iterador_tiene_siguiente(iterador)){
		funcion(lista_iterador_siguiente(iterador));
	}
	lista_iterador_destruir(iterador);
	return;
}
