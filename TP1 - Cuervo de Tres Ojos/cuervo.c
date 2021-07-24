#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "cuervo_tres_ojos.h"

#define ERROR -1
#define EXITO 0

#define NOMBRE_CUERVO "Brynden"
#define NOMBRE_APRENDIZ "Bran Stark"
#define CANTIDAD_CORRECTA 4

#define ARCHIVO_VISIONES "visiones.txt"
#define LECTURA "r"
#define FORMATO "%[^;];%[^;];%[^;];%c\n"

#define CASO_TRANSMITIR 1
#define CASO_LISTAR 2

#define PASADO 'P'
#define FUTURO 'F'

#define SALIR 0

#define ID_PASADO 1
#define ID_FUTURO 2
#define JON_SNOW 3
#define CASA_STARK 4
#define TARGARYEN 5
#define CASA_LANNISTER 6
#define CORTO 7
#define LARGO 8

#define MAX_MIN 23;

/*
 *Libera las estucturas correspondientes
 */
void liberar_memoria(cuervo_tres_ojos_t* cuervo, cuervo_aprendiz_t* aprendiz){
	if (!cuervo) {
		destruir_cuervo(cuervo);
	}
	if (!aprendiz) {
		destruir_aprendiz(aprendiz);
	}
}

/*
 *Lee un registro vision_t en un archivo de texto, y lo inicializa con los valores leidos.
 *Devuelve la cantidad que leyó.
 */
int leer_vision_txt(FILE* archivo, vision_t *vision){
	int leido = fscanf(archivo, FORMATO, vision->protagonista, vision->casa_protagonista, vision->descripcion, &(vision->epoca));
	return leido;
}

/*==============================TIPOS=DE=VISIONES===================================*/
/*
 *Devuelve verdadero si la vision recibida es pasada, en caso contrario devolvera falso.
 */
bool es_pasada(vision_t vision){
	return(vision.epoca == PASADO);
}

/*
 *Devuelve verdadero si la vision recibida es futura, en caso contrario devolvera falso.
 */
bool es_futura(vision_t vision){
	return(vision.epoca == FUTURO);
}

/*
 *Devuelve verdadero si la vision recibida es de Jon Snow, en caso contrario devolvera falso.
 */
bool es_jon_snow(vision_t vision){
	return strcmp(vision.protagonista,"Jon Snow") == 0;
}

/*
 *Devuelve verdadero si la vision recibida pertenece a la casa stark, en caso contrario devolvera falso.
 */
bool es_casa_stark(vision_t vision){
	return(strcmp(vision.casa_protagonista,"Stark") == 0);
}

/*
 *Devuelve verdadero si la vision recibida es de Daenerys Targaryen, en caso contrario devolvera falso.
 */
bool es_de_targaryen(vision_t vision){
	return(strcmp(vision.protagonista,"Daenerys Targaryen") == 0);
}


/*
 *Devuelve verdadero si la vision recibida pertenece a la casa Lannister, en caso contrario devolvera falso.
 */

bool es_casa_lannister(vision_t vision){
	return strcmp(vision.casa_protagonista,"Lannister")==0;
}
/*
 *Devuelve verdadero si la cantidad de caracteres de la descripcion de la vision recibida es corta,
 *en caso contrario devolvera falso.
 */
bool descripcion_corta(vision_t vision){
	return(strlen(vision.descripcion) < MAX_MIN);
}

/*
 *Devuelve verdadero si la cantidad de caracteres de la descripcion de la vision recibida es larga.
 */
bool es_larga(vision_t vision){
	return strlen(vision.descripcion) >= MAX_MIN;
}
/*===========================================================================*/
/*
 * Informa al usuario sobre las opciones transmisibles/listables y le pregunta cual quiere hacer
 */
void preguntar_opcion(int* respuesta, int caso){
	char* situacion = (caso == CASO_LISTAR)? "Listar" : "Transmitir";
	printf("\n\nLas opciones son las siguientes (Ingrese el numero de la que quiere):\n");
	printf("\t 0 - Salir \n\t 1 - %s las del pasado \n\t 2 - %s las del futuro \n\t 3 - %s las de Jon Snow\n", situacion, situacion, situacion);
	printf("\n\t 4 - %s las de la casa Stark \n\t 5 - %s las de la casa Targaryen \n\t 6 - %s las de la casa Lannister", situacion, situacion, situacion);
	printf("\n\t 7 - %s las mas cortas \n\t 8 - %s las mas largas ",situacion, situacion);
	scanf("%i",respuesta);
}

/*
 * Transmitir/listar visiones del cuervo al aprendiz
 * Puede ser invocada varias veces con diferentes funciones
 */
void accionar_con_visiones(cuervo_tres_ojos_t* cuervo, cuervo_aprendiz_t* aprendiz, int* estado, int caso){
	int opcion =- 1;
	while (opcion != SALIR && (*estado) != ERROR) {
		preguntar_opcion(&opcion, caso);
		switch (opcion) {
			case SALIR:
				break;
			case ID_PASADO:
				(caso == CASO_LISTAR)? listar_visiones_transmitidas(*aprendiz, es_pasada) : (*estado) = transmitir_visiones(cuervo, aprendiz, es_pasada);
				break;
			case ID_FUTURO:
				(caso == CASO_LISTAR)? listar_visiones_transmitidas(*aprendiz, es_futura) : (*estado) = transmitir_visiones(cuervo, aprendiz, es_futura);
				break;
			case JON_SNOW:
				(caso == CASO_LISTAR)? listar_visiones_transmitidas(*aprendiz, es_jon_snow) : (*estado) = transmitir_visiones(cuervo, aprendiz, es_jon_snow);
				break;
			case TARGARYEN:
				(caso == CASO_LISTAR)? listar_visiones_transmitidas(*aprendiz, es_de_targaryen) : (*estado) = transmitir_visiones(cuervo, aprendiz, es_de_targaryen);
				break;
			case CASA_STARK:
				(caso == CASO_LISTAR)? listar_visiones_transmitidas(*aprendiz, es_casa_stark) : (*estado) = transmitir_visiones(cuervo, aprendiz, es_casa_stark);
				break;
			case CASA_LANNISTER:
				(caso == CASO_LISTAR)? listar_visiones_transmitidas(*aprendiz, es_casa_lannister) : (*estado) = transmitir_visiones(cuervo, aprendiz, es_casa_lannister);
				break;
			case CORTO:
				(caso == CASO_LISTAR)? listar_visiones_transmitidas(*aprendiz, descripcion_corta) : (*estado) = transmitir_visiones(cuervo, aprendiz, descripcion_corta);
			case LARGO:
				(caso == CASO_LISTAR)? listar_visiones_transmitidas(*aprendiz, es_larga) : (*estado) = transmitir_visiones(cuervo, aprendiz, es_larga);
				break;
			default:
				printf("No es una opcion.\n");
				break;
		}
	}
}

int main(){
	char nombre_cuervo[MAX_NOMBRE] = NOMBRE_CUERVO, nombre_aprendiz[MAX_NOMBRE] = NOMBRE_APRENDIZ;

	vision_t vision;
	int agregue = EXITO, ya_transmiti = EXITO;

	FILE* visiones_file = fopen(ARCHIVO_VISIONES, LECTURA);
	if (!visiones_file) {
		printf("Fallo al intentar abrir el archivo. \n");
		return 0;
	}

	cuervo_tres_ojos_t* cuervo = invocar_cuervo(nombre_cuervo);
	if (!cuervo) {
		printf("Fallo al crear el cuervo.\n");
		fclose(visiones_file);
		return 0;
	}

	cuervo_aprendiz_t* aprendiz = invocar_aprendiz(nombre_aprendiz);
	if (!aprendiz) {
		printf("Fallo al crear el cuervo aprendiz.\n");
		liberar_memoria(cuervo, aprendiz);
		fclose(visiones_file);
		return 0;
	}

	int lei = leer_vision_txt(visiones_file, &vision);
	while (lei == CANTIDAD_CORRECTA  &&  agregue == EXITO) {
		agregue = agregar_vision(cuervo,vision);
		lei = leer_vision_txt(visiones_file, &vision);
	}
	if (agregue != EXITO) {
		fclose(visiones_file);
		liberar_memoria(cuervo, aprendiz);
		printf("Hubo un error en la carga de las visiones\n");
		return 0;
	}
	fclose(visiones_file);

	accionar_con_visiones(cuervo, aprendiz, &ya_transmiti, CASO_TRANSMITIR);
	if (ya_transmiti != EXITO) {
		printf("Fallo la transmicion de las visiones\n");
		liberar_memoria(cuervo, aprendiz);
		return 0;

	system("clear");
	accionar_con_visiones(cuervo, aprendiz, &ya_transmiti, CASO_LISTAR);
	liberar_memoria(cuervo, aprendiz);

	return 0;
}
