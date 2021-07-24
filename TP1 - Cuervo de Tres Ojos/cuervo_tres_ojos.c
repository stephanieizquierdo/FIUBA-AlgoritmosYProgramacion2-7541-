#include "cuervo_tres_ojos.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define PASADO 'P'
#define FUTURO 'F'
#define FALLO -1
#define EXITO 0

cuervo_tres_ojos_t* invocar_cuervo(char* nombre){
	cuervo_tres_ojos_t* cuervo = (cuervo_tres_ojos_t*)calloc(1,sizeof(cuervo_tres_ojos_t));
	if (!cuervo) {
		return NULL;
	}
    strcpy(cuervo->nombre, nombre);
    return cuervo;
}

cuervo_aprendiz_t* invocar_aprendiz(char* nombre){
	cuervo_aprendiz_t* aprendiz = (cuervo_aprendiz_t*)calloc(1,sizeof(cuervo_tres_ojos_t));
	if (!aprendiz) {
		return NULL;
	}
	strcpy(aprendiz->nombre,nombre);
	return aprendiz;
}

/*
 *Agregará una vision en el vector de visiones recibido, en la posicion recibida,
 *Reservando la memoria necesaria para ella.
 *Devolverá 0 si pudo agregarla o -1 si no pudo.
 */
int cargar_vector(vision_t** visiones, vision_t* visionptr, int* tope){
	vision_t* visionptr_adquiridas = NULL;
	visionptr_adquiridas = (vision_t*)realloc(*visiones,((unsigned int)(*tope)+1)*sizeof(vision_t));
	if (!visionptr_adquiridas) {
		return FALLO;
	}
	*visiones = visionptr_adquiridas;
	(*visiones)[*tope] = *visionptr;
	(*tope)++;

	return EXITO;
}

int agregar_vision(cuervo_tres_ojos_t* cuervo, vision_t vision){
	int estado = EXITO;
	if(vision.epoca == PASADO){
		estado = cargar_vector(&(cuervo->visiones_del_pasado),&vision,&(cuervo->cantidad_visiones_pasado));
	}
	else if(vision.epoca==FUTURO){
		estado = cargar_vector(&(cuervo->visiones_del_futuro),&vision,&(cuervo->cantidad_visiones_futuro));
	}
	return estado;
}

/*
 *Verifica si la visión ya está insertada en el vector de visiones
 */
bool la_inserte(vision_t* visionesprt, vision_t vision, int tope){
	bool ya_inserte = false;
	int i = 0;
	while ( i < tope && !ya_inserte) {
		bool son_iguales = (strcmp(visionesprt[i].protagonista, vision.protagonista)==0 && strcmp(visionesprt[i].casa_protagonista, vision.casa_protagonista)==0 && strcmp(visionesprt[i].descripcion, vision.descripcion)==0 && visionesprt[i].epoca==vision.epoca);
		if(son_iguales){
			ya_inserte=true;
		}
		i++;
	}
	return ya_inserte;
}


int transmitir_visiones(cuervo_tres_ojos_t* cuervo, cuervo_aprendiz_t* aprendiz, bool (*es_transmisible)(vision_t)){
	int estado = EXITO;
	int i = 0;
	int j = 0;

	while (i < (cuervo->cantidad_visiones_pasado) && estado == EXITO) {
		if (es_transmisible(cuervo->visiones_del_pasado[i]) && !la_inserte(aprendiz->visiones_adquiridas, cuervo->visiones_del_pasado[i], aprendiz->cantidad_visiones)){
			estado = cargar_vector(&aprendiz->visiones_adquiridas,&cuervo->visiones_del_pasado[i],&(aprendiz->cantidad_visiones));
		}
		i++;
	}
	while (j < (cuervo->cantidad_visiones_futuro) && estado == EXITO){
		if (es_transmisible(cuervo->visiones_del_futuro[j]) && !la_inserte(aprendiz->visiones_adquiridas, cuervo->visiones_del_futuro[j], aprendiz->cantidad_visiones) ){
			estado = cargar_vector(&aprendiz->visiones_adquiridas, &cuervo->visiones_del_futuro[j], &(aprendiz->cantidad_visiones));
		}
		j++;
	}
	return estado;
}

void listar_visiones_transmitidas(cuervo_aprendiz_t aprendiz, bool (*es_listable)(vision_t)){
	int i = 0;
	while (i < aprendiz.cantidad_visiones) {
		if (es_listable(aprendiz.visiones_adquiridas[i]) ){
			printf("Vision sobre %s, %s: %s. Vision del %c.\n",(aprendiz.visiones_adquiridas[i]).protagonista, (aprendiz.visiones_adquiridas[i]).casa_protagonista, (aprendiz.visiones_adquiridas[i]).descripcion, (aprendiz.visiones_adquiridas[i]).epoca);
		}
		i++;
	}
}

int destruir_cuervo(cuervo_tres_ojos_t* cuervo){
	if(!cuervo){
		return FALLO;
	}
	if(cuervo->visiones_del_futuro){
		free(cuervo->visiones_del_futuro);
	}
	if(cuervo->visiones_del_pasado){
		free(cuervo->visiones_del_pasado);
	}
	free(cuervo);
	return EXITO;
}

int destruir_aprendiz(cuervo_aprendiz_t* aprendiz){
	if (!aprendiz) {
		return FALLO;
	}
	if (aprendiz->visiones_adquiridas) {
		free(aprendiz->visiones_adquiridas);
	}
	free(aprendiz);
	return EXITO;
}
