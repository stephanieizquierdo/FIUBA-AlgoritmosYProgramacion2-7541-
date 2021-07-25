#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define EXITO 0
#define ERROR -1
#define VACIO 0


#define ABB_RECORRER_INORDEN   0
#define ABB_RECORRER_PREORDEN  1
#define ABB_RECORRER_POSTORDEN 2

typedef int (*abb_comparador)(void*, void*);

typedef void (*abb_liberar_elemento)(void*);

typedef struct nodo_abb {
  void* elemento;
  struct nodo_abb* izquierda;
  struct nodo_abb* derecha;
} nodo_abb_t;

typedef struct abb{
  nodo_abb_t* nodo_raiz;
  abb_comparador comparador;
  abb_liberar_elemento destructor;
} abb_t;


/***CREACION***/


/*
 *Crea un nodo reservando la memoria necesaria para el mismo y almacenando el elemento pasado
 *Si no se pudo crear devuelve NULL
 */
nodo_abb_t* abb_crear_nodo(void* elemento){

	nodo_abb_t* nodo = (nodo_abb_t*)calloc(1,sizeof(nodo_abb_t));
	if(!nodo){
		return NULL;
	}
	nodo->elemento = elemento;

	return nodo;
}

abb_t* arbol_crear(abb_comparador comparador, abb_liberar_elemento destructor){

	if(!comparador){
		return NULL;
	}

	abb_t* arbol= (abb_t*)malloc(sizeof(abb_t));
	if(!arbol){
		return NULL;
	}

	arbol->nodo_raiz=NULL;
	arbol->comparador = comparador;
	arbol->destructor = destructor;

	return arbol;
}


/***ARBOL VACIO***/

bool arbol_vacio(abb_t* arbol){
	if(!arbol){
		return true;
	}
	return(arbol->nodo_raiz == NULL );
}

/***ARBOL RAIZ***/

void* arbol_raiz(abb_t* arbol){
	if(!arbol || !arbol->nodo_raiz){
		return NULL;
	}
	else{
		return(arbol->nodo_raiz->elemento);
	}
}



/***INSERCION***/


/*
 *Recibe un nodo del arbol y comparará el elemento recibido con los elementos existentes dentro del arbol.
 *Si el elemento es menor al nodo ya existente dentro del arbol, se insertara del lado izquierdo del arbol.
 *Si es mayor o igual se insertará del lado derecho.
 */
nodo_abb_t* arbol_insertar_rec(nodo_abb_t* nodo, void* elemento, abb_comparador comparador, int* estado_funcion){
	
	if(!nodo){

		nodo_abb_t* nodo_nuevo = abb_crear_nodo(elemento);

		if(!nodo_nuevo){
			return NULL;
		}

		*estado_funcion = EXITO;

		return nodo_nuevo;
	}
	
	int comparacion = comparador(elemento, nodo->elemento);

	if(comparacion < 0){
		nodo->izquierda = arbol_insertar_rec(nodo->izquierda, elemento,comparador, estado_funcion);
		return nodo;
	}	
	else{ //si es mayor o igual va a la derecha
		nodo->derecha = arbol_insertar_rec(nodo->derecha, elemento, comparador, estado_funcion);
		return nodo;
	}
}

int arbol_insertar(abb_t* arbol, void* elemento){
	if(!arbol){
		return ERROR;
	}

	int estado_funcion= ERROR;

	arbol->nodo_raiz= arbol_insertar_rec(arbol->nodo_raiz, elemento, arbol->comparador, &estado_funcion);

	return estado_funcion;
}


/***BUSQUEDA***/

/*
 *recibe un nodo y se compara su valor con el elemento recibido, si es igual se devuelve el elemento
 *Si es menor se comparará con su hijo izquierdo y es mayor con su hijo derecho, hasta encontrarlo.
 *Si no lo encuentradevuelve NULL.
 */
void* arbol_buscar_rec(nodo_abb_t* nodo, abb_comparador comparador, void* elemento){

	if(!nodo || !elemento){
		return NULL;
	}
	int comparacion = comparador(elemento, nodo->elemento);

	if(comparacion<0){
		return arbol_buscar_rec(nodo->izquierda, comparador, elemento);
	}

	else if(comparacion>0){
		return arbol_buscar_rec(nodo->derecha, comparador, elemento);
	}
	else{
		return(nodo->elemento);
	}
}

void* arbol_buscar(abb_t* arbol, void* elemento){
	if(!arbol || arbol_vacio(arbol)){
		return NULL;
	}
	return(arbol_buscar_rec(arbol->nodo_raiz, arbol->comparador, elemento));
}


/***RECORRIDOS***/


/*
 *Recibe un array valido.
 *Inserta el elemento recibido en la ultima posicion del array mientras no se supere el tamaño provisto para el mismo.
 */
void insertar_elemento_en_array(void* elemento, void** array, int tamanio_array, int* contador){
	if((*contador) < tamanio_array){
		array[*contador] = elemento;
		(*contador)++;
	}
}

/*
 *Recorre el arbol dado sus nodos, en el sentido de Preorden recursivamente
 *Recibe un array, con su correspondiente tamaño, valido.
 */
void arbol_recorrido_preorden_rec(nodo_abb_t* nodo, void** array, int tamanio_array, int* contador){
	if(!nodo || (*contador) == tamanio_array){
		return;
	}

	insertar_elemento_en_array(nodo->elemento, array, tamanio_array, contador);
	arbol_recorrido_preorden_rec(nodo->izquierda, array, tamanio_array, contador);
	arbol_recorrido_preorden_rec(nodo->derecha, array, tamanio_array, contador);	
}


int arbol_recorrido_preorden(abb_t* arbol, void** array, int tamanio_array){
	if(!array ||arbol_vacio(arbol) || tamanio_array == 0){
		return VACIO;
	}
	int contador= 0;

	arbol_recorrido_preorden_rec(arbol->nodo_raiz, array, tamanio_array, &contador);

	return contador;
}

/*
 *Recorre el arbol dado sus nodos, en el sentido de Inorden recursivamente
 *Recibe un array, con su correspondiente tamaño, valido.
 */
void arbol_recorrido_inorden_rec(nodo_abb_t* nodo, void** array, int tamanio_array, int* contador){
	if(!nodo || (*contador) == tamanio_array){
		return;
	}
	
	arbol_recorrido_inorden_rec(nodo->izquierda, array, tamanio_array, contador);

	insertar_elemento_en_array(nodo->elemento, array, tamanio_array, contador);

	arbol_recorrido_inorden_rec(nodo->derecha, array, tamanio_array, contador);
}

int arbol_recorrido_inorden(abb_t* arbol, void** array, int tamanio_array){
	if(!array ||arbol_vacio(arbol) || tamanio_array == 0){
		return VACIO;
	}
	int contador=0;

	arbol_recorrido_inorden_rec(arbol->nodo_raiz, array, tamanio_array, &contador);
	return contador;
}

/*
 *Recorre el arbol dado sus nodos, en el sentido de Inorden recursivamente
 *Recibe un array, con su correspondiente tamaño, valido.
 */
void arbol_recorrido_postorden_rec(nodo_abb_t* nodo, void** array, int tamanio_array, int* contador){
	if(!nodo || (*contador) == tamanio_array){
		return;
	}

	arbol_recorrido_postorden_rec(nodo->izquierda, array, tamanio_array, contador);
	arbol_recorrido_postorden_rec(nodo->derecha, array, tamanio_array, contador);

	insertar_elemento_en_array(nodo->elemento, array, tamanio_array, contador);
}

int arbol_recorrido_postorden(abb_t* arbol, void** array, int tamanio_array){
	if(!array || arbol_vacio(arbol) || tamanio_array == 0){
		return VACIO;
	}	
	int contador=0;

	arbol_recorrido_postorden_rec(arbol->nodo_raiz, array, tamanio_array, &contador);
	return contador;
}



/***BORRAR***/


/*
 *Dado un nodo, se devolvera el menor de sus hijos.
 *El elemento que este tenía será pasado al puntero recibido y se liberará el nodo.
 *Devuelve el nodo derecha del nodo liberado.
 */
nodo_abb_t* extraer_menor_de_los_mayores(nodo_abb_t* nodo, void** nuevo_elemento){

	if(!nodo->izquierda){

		(*nuevo_elemento) = nodo->elemento;

		nodo_abb_t* aux= nodo->derecha;

		free(nodo);

		return aux;
	}

	else{
		nodo->izquierda = extraer_menor_de_los_mayores(nodo->izquierda, nuevo_elemento);

		return nodo;
	}
}

/*
 *Recibe un comparador valido y un estado inicializado en ERROR.
 *Se evaluará si el nodo pasado es el que se tiene que eliminar, si no, se lo comparará para repetir el proceso con el hijo correspondiente.
 *En caso de que halla podido borrar el nodo, actualizara el estado a EXITO.
 */
nodo_abb_t* borrar_rec(nodo_abb_t* nodo, void* elemento, abb_comparador comparador, abb_liberar_elemento destructor, int* estado){
	if(!nodo){
		return NULL;
	}

	int comparacion = comparador(elemento, nodo->elemento);

	
	if(comparacion < 0){
		nodo->izquierda = borrar_rec(nodo->izquierda, elemento, comparador, destructor, estado);
		return nodo;
	}
	else if(comparacion > 0){
		nodo->derecha = borrar_rec(nodo->derecha, elemento, comparador, destructor, estado);
		return nodo;
	}
	else{
		nodo_abb_t* nodo_aux=NULL;

		if(!nodo->izquierda && !nodo->derecha){

			if(destructor){
				destructor(nodo->elemento);
			}
			free(nodo);
		}

		else if(!nodo->izquierda || !nodo->derecha){

			nodo_aux = (!nodo->izquierda)?  nodo->derecha : nodo->izquierda;

			if(destructor){
				destructor(nodo->elemento);
			}
			free(nodo);
		}

		else{

			void* nuevo_elemento;

			nodo->derecha = extraer_menor_de_los_mayores(nodo->derecha, &nuevo_elemento);

			if(destructor){
				destructor(nodo->elemento);
			}

			nodo->elemento = nuevo_elemento;

			nodo_aux= nodo;
		}

		(*estado) = EXITO;

		return nodo_aux;
	}
}


/***DESTRUIR***/



/*
 *Recibe un arbol valido. Borra los nodos existentes en el arbol.
 */
int arbol_borrar(abb_t* arbol, void* elemento){
	if(!arbol){
		return ERROR;
	}

	int estado_funcion = ERROR;

	arbol->nodo_raiz = borrar_rec(arbol->nodo_raiz, elemento, arbol->comparador, arbol->destructor, &estado_funcion);

	return estado_funcion;
}

void arbol_destruir(abb_t* arbol){
	if(!arbol){
		return;
	}
	while(!arbol_vacio(arbol)){
		arbol_borrar(arbol, arbol->nodo_raiz->elemento);
	}

	free(arbol);

	return;
}


/***ITERADOR INTERNO***/


/*
 *Recibe una funcion válida y  actualiza el estado segun lo que devuelva esta funcion.
 *Se iterará inorden hasta que ya no haya más nodos o la funcion devuelva true
 */
void iterador_con_inorden(nodo_abb_t* nodo, bool (*funcion)(void*, void*),void* extra, bool* estado){
	if(!nodo || *estado == true){
		return;
	}

	iterador_con_inorden(nodo->izquierda,funcion,extra, estado);
	if(*estado != true){
		*estado= funcion(nodo->elemento, extra);
	}
	iterador_con_inorden(nodo->derecha,funcion,extra, estado);
}

/*
 *Recibe una funcion válida y  actualiza el estado segun lo que devuelva esta funcion.
 *Se iterará preorden hasta que ya no haya más nodos o la funcion devuelva true
 */
void iterador_con_preorden(nodo_abb_t* nodo, bool (*funcion)(void*, void*),void* extra, bool* estado){
	if(!nodo || *estado == true){
		return;
	}

	if(*estado != true){
		*estado= funcion(nodo->elemento, extra);
	}
	iterador_con_preorden(nodo->izquierda, funcion, extra, estado);
	iterador_con_preorden(nodo->derecha, funcion , extra, estado);
}

/*
 *Recibe una funcion válida y  actualiza el estado segun lo que devuelva esta funcion.
 *Se iterará postorden hasta que ya no haya más nodos o la funcion devuelva true
 */
void iterador_con_postorden(nodo_abb_t* nodo, bool (*funcion)(void*, void*),void* extra, bool* estado){
	if(!nodo || *estado == true){
		return;
	}
	iterador_con_postorden(nodo->izquierda, funcion, extra, estado);
	iterador_con_postorden(nodo->derecha, funcion, extra, estado);

	if(*estado != true){
		*estado= funcion(nodo->elemento, extra);
	}
}	

void abb_con_cada_elemento(abb_t* arbol, int recorrido, bool (*funcion)(void*, void*), void* extra){
	if(!arbol || !funcion){
		return;
	}

	bool estado = false;

	if(recorrido == ABB_RECORRER_INORDEN){

		iterador_con_inorden(arbol->nodo_raiz, funcion, extra, &estado);
	}
	else if(recorrido == ABB_RECORRER_PREORDEN){

		iterador_con_preorden(arbol->nodo_raiz, funcion, extra, &estado);
	}
	else if(recorrido == ABB_RECORRER_POSTORDEN){

		iterador_con_postorden(arbol->nodo_raiz, funcion, extra, &estado);
	}
	return;
}
