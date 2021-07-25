#include "lista.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct item_hash{
	void* elemento;
	const char* clave;
}item_hash_t;

typedef void (*hash_destruir_dato_t)(void*);

typedef struct hash {
	lista_t** tabla;
	size_t tamanio;
	hash_destruir_dato_t destructor;
	size_t cant_elementos;
} hash_t;

typedef struct hash_iterador {
	lista_iterador_t* corriente;
	lista_t* lista;
	lista_t** tabla;
	hash_t* hash;
	size_t posicion_tabla_actual;
} hash_iterador_t;

/*
 *Busca la proxima lista valida desde la posicion pasada a la funcion.
 *Si existe deuvelve false y actualiza la posicion. Sino devuelve false.
 *El iterador, tamaño y posicion son validos.
 */
bool proxima_lista_valida(size_t tamanio, hash_iterador_t* iterador, size_t* posicion){
	while (!(iterador->tabla[(*posicion)]) && (*posicion) < tamanio) {
		(*posicion)++;
	}
	if (!iterador->tabla[*posicion]) {
		return false;
	} else {
        return true;
    }
}

hash_iterador_t* hash_iterador_crear(hash_t* hash){
	if (!hash) {
		return NULL;
	}
	hash_iterador_t* iterador = (hash_iterador_t*)malloc(sizeof(hash_iterador_t));
	if (!iterador) {
		return NULL;
	}
	iterador->hash = hash;

	iterador->tabla = hash->tabla;
	size_t posicion = 0;

	proxima_lista_valida(hash->tamanio, iterador, &posicion);

	iterador->posicion_tabla_actual = posicion;
	iterador->lista = iterador->tabla[posicion];

	lista_iterador_t* iter_lista = lista_iterador_crear(iterador->lista);
	if (!iter_lista) {
		return NULL;
	}
	iterador->corriente = iter_lista;
	return iterador;
}

void* hash_iterador_siguiente(hash_iterador_t* iterador){
	if (!iterador || !iterador->corriente) {
		return NULL;
	}
	item_hash_t* item = (item_hash_t*)lista_iterador_siguiente(iterador->corriente);
	return (void*)item->clave;
}

/*
 * Devuelve true si quedan claves por recorrer o false en caso
 * contrario.
 */
bool hash_iterador_tiene_siguiente(hash_iterador_t* iterador){
	if (!iterador|| !iterador->corriente) {
		return false;
	}
	if (iterador->corriente && lista_iterador_tiene_siguiente(iterador->corriente)) {
		return lista_iterador_tiene_siguiente(iterador->corriente);
	}
	if(iterador->posicion_tabla_actual < iterador->hash->tamanio -1 ){
		(iterador->posicion_tabla_actual)++;

		proxima_lista_valida(iterador->hash->tamanio, iterador, &(iterador->posicion_tabla_actual));

		lista_iterador_destruir(iterador->corriente);
		iterador->corriente = NULL;
		iterador->lista = iterador->tabla[iterador->posicion_tabla_actual];

		lista_iterador_t* iter_lista = lista_iterador_crear(iterador->lista);
		if (!iter_lista) {
			return false;
		}
		iterador->corriente = iter_lista;

		return lista_iterador_tiene_siguiente(iterador->corriente);
	}
	return false;
}

void hash_iterador_destruir(hash_iterador_t* iterador){
	free(iterador->corriente);
	free(iterador);
}
