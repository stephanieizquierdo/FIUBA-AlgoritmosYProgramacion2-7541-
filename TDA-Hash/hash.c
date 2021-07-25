#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lista.h"
#include "cola.h"

#define EXITO 0
#define ERROR -1
#define VACIO 0
#define POS_INICIAL 0

#define PROPORCION 2

// Necesario porque strdup es POSIX pero no C99
extern char* strdup(const char*);

/*===============================ESTRUCTURAS===================================*/
typedef struct item_hash {
	void* elemento;
	const char* clave;
} item_hash_t;

typedef void (*hash_destruir_dato_t)(void*);

typedef struct hash {
	lista_t** tabla;
	size_t tamanio;
	hash_destruir_dato_t destructor;
	size_t cant_elementos;
} hash_t;

/*=============================FUNCIONES LISTA================================*/

hash_t* hash_crear(hash_destruir_dato_t destruir_elemento, size_t capacidad){
	if (!capacidad) {
		return NULL;
	}
	hash_t* hash = (hash_t*)calloc(1, sizeof(hash_t));
	if (!hash) {
		return NULL;
	}
	size_t nueva_capacidad = (capacidad + capacidad/2);
	lista_t** tabla = (lista_t**)calloc(nueva_capacidad, sizeof(lista_t*));
	if (!tabla) {
		free(hash);
		return NULL;
	}

	hash->tabla = tabla;
	hash->tamanio = nueva_capacidad;
	hash->destructor = destruir_elemento;

	return hash;
}


bool hash_vacio(hash_t* hash){
	if (!hash) {
		return true;
	} else {
		return(hash->cant_elementos == VACIO);
	}
}

/*
 *Inicializa un nuevo item con su correspondiente clave valida y elemento .
 */
item_hash_t* nuevo_elemento(const char* clave, void* elemento){
	item_hash_t* nuevo = (item_hash_t*)malloc(sizeof(item_hash_t));
	if(!nuevo) return NULL;

	nuevo->clave = strdup(clave);
	nuevo->elemento = elemento;

	return nuevo;
}

/*
 *Funcion de Hash, dada una clave que es valida, genera una clave del hash.
 */
size_t generar_clave_hash(size_t capacidad, const char* clave){
	size_t clave_de_hash = 0;

	for (int i=0; i<strlen(clave); i++) {
		clave_de_hash+=((size_t)clave[i]);
	}
	clave_de_hash = (clave_de_hash % capacidad);

	return clave_de_hash;
}

/*
 * Devuelve la cantidad de elementos almacenados en el hash.
 */
size_t hash_cantidad(hash_t* hash){
	if(!hash)
		return VACIO;
	else
		return(hash->cant_elementos);
}

/*
 *Dada una lista, clave valida y posicion inicial, recorre la lista en busca del elemento cuya clave
 *sea igual a la pasada y actualizará la posicion en la que se encuentra.
 *Devuelve True si la encontro o False en caso contrario
 */
bool encontrar_posicion(lista_t* lista, const char* clave, size_t* posicion){
	lista_iterador_t* iter = lista_iterador_crear(lista);
	if(!iter){
		return false;
	}
	bool esta = false;

	while (lista_iterador_tiene_siguiente(iter) && !esta) {
		item_hash_t* elemento = (item_hash_t*)lista_iterador_siguiente(iter);

		if(strcmp(elemento->clave, clave) == 0){
			esta = true;
		} else {
			(*posicion)++;
        }
	}
	lista_iterador_destruir(iter);
	return esta;
}

void* hash_obtener(hash_t* hash, const char* clave){
	if (!hash || !clave || hash_vacio(hash)) {
		return NULL;
	}

	size_t clave_hash = generar_clave_hash(hash->tamanio,clave);

	size_t posicion= POS_INICIAL;

	if (!encontrar_posicion(hash->tabla[clave_hash], clave, &posicion)) {
		return NULL;
	}
	item_hash_t* elemento_del_hash = lista_elemento_en_posicion(hash->tabla[clave_hash], posicion);

	return (elemento_del_hash->elemento);
}

bool hash_contiene(hash_t* hash, const char* clave){
	if(!hash || !clave || hash_vacio(hash)){
		return false;
	}
	size_t clave_hash = generar_clave_hash(hash->tamanio,clave);

	size_t posicion = POS_INICIAL;
	bool contiene = encontrar_posicion(hash->tabla[clave_hash], clave, &posicion);

	return(contiene);
}

int hash_quitar(hash_t* hash, const char* clave){
	if (!hash || !clave || hash_vacio(hash)) {
		return ERROR;
	}
	int estado_borrar = ERROR;
	size_t clave_de_hash = generar_clave_hash(hash->tamanio, clave);
	size_t posicion = POS_INICIAL;

	if (encontrar_posicion(hash->tabla[clave_de_hash], clave, &posicion)) {
		item_hash_t* elem_a_borrar= (item_hash_t*)lista_elemento_en_posicion(hash->tabla[clave_de_hash], posicion);
		if (!elem_a_borrar) {
			return ERROR;
		}
		estado_borrar = lista_borrar_de_posicion(hash->tabla[clave_de_hash], posicion);

		if (hash->destructor) {
			hash->destructor(elem_a_borrar->elemento);
		}
		free((void*)elem_a_borrar->clave);
		free(elem_a_borrar);

		if (lista_vacia(hash->tabla[clave_de_hash])) {
			lista_destruir(hash->tabla[clave_de_hash]);
			hash->tabla[clave_de_hash]= NULL;
		}
		(hash->cant_elementos)--;
	}
	return estado_borrar;
}


void hash_destruir(hash_t* hash){
	if (!hash) {
		return;
	}
	if (!hash_vacio(hash)) {

		for(int i = 0; i < hash->tamanio; i++){
			lista_t* lista = hash->tabla[i];
			if (lista) {
				while(!lista_vacia(lista)){
					item_hash_t* item = (item_hash_t*)lista_ultimo(lista);
					if(hash->destructor){
						hash->destructor(item->elemento);
					}
					lista_borrar(item);
					free((void*)item->clave);
					free(item);
				}
				lista_destruir(lista);
			}
		}
	}
	free(hash->tabla);
	free(hash);
}

/*
 *Dada una tabla de hash, la clave de hash y un elemento de hash, inserta el elemento en
 *su posicion (lista) correspondiente en la tabla
 */
int insertar_en_tabla(lista_t** tabla, item_hash_t* nuevo, size_t clave_de_hash){
	int estado_insertar= ERROR;

	if (!tabla[clave_de_hash]) {
		lista_t* lista = lista_crear();
		if (!lista) {
			return ERROR;
		}

		estado_insertar = lista_insertar(lista, nuevo);
		tabla[clave_de_hash] = lista;
	} else {
		estado_insertar = lista_insertar(tabla[clave_de_hash], nuevo);
	}
    return estado_insertar;
}

/*
 *En caso de que se inserte un elemento con una clave ya insertada, se destruirá el elemento anterior y se reemplazara
 *Por el nuevo elemento pasado a insertar
 */
int actualizar_elemento(lista_t* lista,void* elemento, size_t posicion, hash_destruir_dato_t destructor){
	item_hash_t* item_a_actualizar = lista_elemento_en_posicion(lista, posicion);

	if (destructor) {
		destructor(item_a_actualizar->elemento);
	}

	item_a_actualizar->elemento = elemento;

	return EXITO;
}

/*
 *Verifica que en la lista donde se ha insertado un elemento, no supere más de tres elementos
 */
bool factor_rehasheo_mayor_a_tres(size_t capacidad, lista_t* lista){
	return (lista_elementos(lista)>3);
}

int rehashear(hash_t* hash);

int hash_insertar(hash_t* hash, const char* clave, void* elemento){
	if (!hash || !clave) {
		return ERROR;
	}
	int estado = ERROR;
    size_t posicion = POS_INICIAL;

	size_t clave_de_hash = generar_clave_hash(hash->tamanio, clave);

	if (!encontrar_posicion(hash->tabla[clave_de_hash],clave,&posicion)) {
		item_hash_t* nuevo = nuevo_elemento(clave, elemento);
		if (!nuevo) {
			return ERROR;
		}
		estado = insertar_en_tabla(hash->tabla, nuevo, clave_de_hash);

		(hash->cant_elementos)++;
	} else {
		estado = actualizar_elemento(hash->tabla[clave_de_hash], elemento,posicion, hash->destructor);
	}
	if (factor_rehasheo_mayor_a_tres(hash->tamanio, hash->tabla[clave_de_hash])) {
		estado = rehashear(hash);
	}
	return estado;
}

/*
 *En caso que alguna lista tenga tres elementos, se actualizara el tamaño de la tabla y
 *se volveran a distribuir los elementos previamentes insertados.
 */
int rehashear(hash_t* hash){
	cola_t* cola = cola_crear();
	if(!cola){
		return ERROR;
	}
	lista_t** tabla= hash->tabla;

	for (size_t i= 0; i< hash->tamanio; i++) {
		lista_t* lista = tabla[i];
		if (lista) {
			lista_iterador_t* iter = lista_iterador_crear(lista);
			if (!iter) {
				return ERROR;
			}
			int pude_guardar = EXITO;

			while (lista_iterador_tiene_siguiente(iter) && pude_guardar == EXITO) {
				item_hash_t* item = (item_hash_t*)lista_iterador_siguiente(iter);
				pude_guardar = cola_encolar(cola, item);
			}
			if (pude_guardar != EXITO) {
				return ERROR;
			}
			lista_iterador_destruir(iter);
			lista_destruir(lista);
		}
	}
	hash->cant_elementos = VACIO;

	hash->tamanio = (hash->tamanio)*PROPORCION;

	lista_t** aux = (lista_t**)realloc((hash->tabla), (hash->tamanio)*sizeof(void*));
	if (!aux) {
		return ERROR;
	}
	hash->tabla= aux;

	for (size_t i= 0; i < hash->tamanio; i++) {
		hash->tabla[i]=NULL;
	}

	int pude_insertar = EXITO;

	while (!cola_vacia(cola) && pude_insertar == EXITO) {
		item_hash_t* item = (item_hash_t*)cola_primero(cola);

		pude_insertar = hash_insertar(hash, item->clave, item->elemento);
		free((void*)item->clave);
		free(item);

		cola_desencolar(cola);
	}
	cola_destruir(cola);

    return pude_insertar;
}
