#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"
#include "abb.h"
#include "cola.h"
extern char* strdup(const char*);

#define MAX_NOMBRE 100

#define EXITO 0
#define ERROR -1

#define ERROR_APERTURA_ARCHIVO -2
#define LECTURA "r"
#define FORMATO_CASAS "C;%[^;];%i;%i"
#define FORMATO_PERSONAS "I;%[^;];%i"

#define ARCHIVO_CON_NOMBRES "bebes.dat"
#define FORMATO_NOMBRES "%[^\n]\n"

#define CASA 'C'
#define CANT_CORRECTA_CASA 3
#define CANT_CORRECTA_PERSONA 2
#define PERSONA 'I'
#define TAM_BUFFER 100

#define EDAD_DE_MUERTE 60

#define MAYOR 1
#define MENOR -1
#define IGUAL 0

typedef struct persona{
	char nombre[MAX_NOMBRE];
	int edad;
}persona_t;

typedef struct casa{
	char nombre[MAX_NOMBRE];
	int factor_envejecimiento;
	int factor_nacimiento;

	lista_t* comunidad;
	int cantidad_de_personas;
}casa_t;

typedef struct westeros{
	abb_t* reino;
	int cant_casas;

	int antiguedad;

	cola_t* reinos_extintos;
	int cant_casas_extintas;

	char* casa_ganadora;
	int casa_ganadora_integrantes;
}westeros_t;

int comparar_casas(void* casa1, void* casa2){
    if(!casa1 || !casa2)
	return 0;

    if (strcmp( ((casa_t*)casa1)->nombre, ((casa_t*)casa2)->nombre ) > 0)
    	return MAYOR;
    if (strcmp( ((casa_t*)casa1)->nombre,((casa_t*)casa2)->nombre ) < 0 )
		return MENOR;
    return IGUALES;
}

void destruir_casa(casa_t* casa){
    if(casa){
    	if(casa->comunidad){
    		for(size_t i= 0; i< casa->cantidad_de_personas; i++){
				free(lista_ultimo(casa->comunidad));
				lista_borrar(casa->comunidad);
			}
			lista_destruir(casa->comunidad);
    	}
		free(casa);
    }
}

void destructor_de_casas(void* elemento){
    if(!elemento){
    	return;
    }
    destruir_casa((casa_t*)elemento);
}

/************** AGREGAR CASAS *****************/

/*
 *Inicializa un casa guardando la memoria necesaria para ello
 */
casa_t* inicializar_casa(){
	casa_t* casa = (casa_t*)calloc(1, sizeof(casa_t));
	lista_t* comunidad = lista_crear();
	if(!comunidad){
		free(casa);
		return NULL;
	}
	casa->comunidad = comunidad;
	return casa;
}

/*
 *Inicializa una persona guardando la memoria necesaria para ello
 */
persona_t* inicializar_persona(){
	persona_t* persona = (persona_t*)calloc(1,sizeof(persona_t));
	if(!persona)
		return NULL;
	return persona;
}

/*
 *Inicializa la estructura del continente guardando la memoria necesaria para ello.
 */
westeros_t* inicializar_westeros(){

	westeros_t* westeros = (westeros_t*)calloc(1,sizeof(westeros_t));
	if(!westeros){
		return NULL;
	}

	abb_t* reino = arbol_crear(comparar_casas, NULL);
	if(!reino){
		free(westeros);
		return NULL;
	}

	cola_t* reinos_extintos = cola_crear();
	if(!reinos_extintos){
		free(reino);
		free(westeros);
		return NULL;
	}

	westeros->reino = reino;
	westeros->reinos_extintos = reinos_extintos;
	westeros->casa_ganadora= NULL;
	return westeros;
}

/*Inserta la casa en el reino (arbol de casas)
 *La casa y westeros son validos.
 */
int agregar_casa_a_westeros(westeros_t* westeros, casa_t* casa){
	return arbol_insertar(westeros->reino, (void*)casa);
}


/*
 *Inserta una persona a la lista de miembros de su casa correspondiente.
 */
int agregar_persona(lista_t* comunidad, persona_t* persona){
	return(lista_insertar(comunidad, (void*)persona));
}

/*
 *Verifica si existe la casa dentro del reino o no. Si existe la devuelve, sino devuelve Null.
 */
casa_t* existe_casa(westeros_t* westeros, casa_t* casa){
	return(arbol_buscar(westeros->reino, (void*)casa));
}

/*
 *Dado un archivo, lee las casas y personas correspondientes para insertarlas dentro de la estructura de Westeros.
 *Retorna un valor de estado. EXITO (0) si se concluyó satisfactoriamente la operacion, ERROR(-1) si ocurrio un problema al leer o insertar
 * o ERROR_APERTURA_ARCHIVO(-2) si no se pudo abrir el archivo.
 */
int agregar_casas(westeros_t* westeros, char* nombre_archivo){
	FILE* westeros_file;
	westeros_file = fopen(nombre_archivo,LECTURA);
	if(!westeros_file){
		return ERROR_APERTURA_ARCHIVO;
	}
	casa_t* casa_actual = NULL;
	int agregue = EXITO;

	char buf[TAM_BUFFER];
	void* lei= fgets(buf,TAM_BUFFER,westeros_file);
	char identificador = buf[0];

	while(lei && agregue == EXITO){
		identificador = buf[0];

		if(identificador == CASA){
			casa_actual = inicializar_casa();
			if(!casa_actual){
				return ERROR;
			}
			int leido_casa = sscanf(buf, FORMATO_CASAS, (casa_actual->nombre), &(casa_actual->factor_envejecimiento), &(casa_actual->factor_nacimiento));
			casa_t* existente = existe_casa(westeros, casa_actual);

			if(!existente && leido_casa == CANT_CORRECTA_CASA){
				agregue = agregar_casa_a_westeros(westeros, casa_actual);

				(westeros->cant_casas)++;
			}else{
				destruir_casa(casa_actual);
				casa_actual = existente;
			}
		}
		else if(identificador == PERSONA){
			persona_t* persona = inicializar_persona();
			if(!persona){
				return ERROR;
			}
			int leido_persona = sscanf(buf, FORMATO_PERSONAS, persona->nombre, &persona->edad);
			if(leido_persona == CANT_CORRECTA_PERSONA && casa_actual){
				agregue = agregar_persona(casa_actual->comunidad, persona);
				(casa_actual->cantidad_de_personas)++;
			}
			else agregue = ERROR;
		}
		lei = fgets(buf, TAM_BUFFER, westeros_file);
	}
	fclose(westeros_file);
	return agregue;
}

/**************** SIMULACION ****************/

/*
 *Compara la cantidad de personas de la casa ganadora actual con la cantidad de personas de la casa que recibe.
 *Si la casa que recibe la funcion tiene mayor cantidad de personas, pasara a ser la casa ganadora.
 */
void elegir_casa_ganadora(westeros_t* westeros, casa_t* casa_nueva){
	if(casa_nueva->cantidad_de_personas > 0){
		if(!westeros->casa_ganadora){
			westeros->casa_ganadora = casa_nueva->nombre;
			westeros->casa_ganadora_integrantes= casa_nueva->cantidad_de_personas;
		}
		if(casa_nueva->cantidad_de_personas > westeros->casa_ganadora_integrantes){
			westeros->casa_ganadora = casa_nueva->nombre;
			westeros->casa_ganadora_integrantes = casa_nueva->cantidad_de_personas;
		}
	}
}

/*
 *Dada un reino, se calcula la cantidad de personas a nacer y se les otorga un nombre proveniende te un archivo externo
 */
int nacimientos(westeros_t* westeros, casa_t* casa, FILE* nombre_de_bebes){
	if(casa->factor_nacimiento == 0){
		return EXITO;
	}
	int nuevos_integrantes = (casa->cantidad_de_personas)/casa->factor_nacimiento;

	int agregue = EXITO;
	int i = 0;
	int resultado;

	while(i < nuevos_integrantes && agregue==EXITO){
		persona_t* persona = inicializar_persona();
		if(!persona){
			return ERROR;
		}
		resultado = fscanf(nombre_de_bebes, FORMATO_NOMBRES, persona->nombre);

		if(resultado == EOF){
			rewind(nombre_de_bebes);
			resultado = fscanf(nombre_de_bebes, FORMATO_NOMBRES, persona->nombre);
		}
		agregue = lista_insertar(casa->comunidad, (void*)persona);

		(casa->cantidad_de_personas)++;
		i++;
	}
	return agregue;
}

/*
 *Dada una casa, se actualizaran las edades de cada uno de sus integrantes.
 *Los integrantes no pueden superar la edad de los 59 años. Si tiene 60 o más se lo eliminará.
 */
int actualizar_edades(casa_t* casa){
	if(!casa){
		return ERROR;
	}
	int estado = EXITO;

	for(size_t pos=0; pos < casa->cantidad_de_personas; pos++){
		persona_t* persona = (persona_t*)lista_elemento_en_posicion(casa->comunidad, pos);
		(persona->edad) += (casa->factor_envejecimiento);

		if(persona->edad >= EDAD_DE_MUERTE){
			estado = lista_borrar_de_posicion(casa->comunidad, pos);
			free(persona);
			(casa->cantidad_de_personas)--;
			pos--;
		}
	}
	return estado;
}

/*
 *Recorre un vector de casas y verificara si no tiene integrantes. Si no los tiene, la casa se considera extinta y por ende
 *Se lo borrara del reino, se la liberará y se insertará su nombre en la cola de casas extintas.
 */
void borrar_y_encolar_casas_extintas(westeros_t* westeros, casa_t* array_de_casas[]){
	int cant_de_casas = westeros->cant_casas;

	for(int i=0; i<cant_de_casas; i++){
		if(array_de_casas[i]->cantidad_de_personas == 0){
			lista_destruir(array_de_casas[i]->comunidad);
			array_de_casas[i]->comunidad = NULL;

			char* nombre = strdup(array_de_casas[i]->nombre);

			cola_encolar(westeros->reinos_extintos, (void*)nombre);
			arbol_borrar(westeros->reino, array_de_casas[i]);
			free(array_de_casas[i]);

			westeros->cant_casas--;
			(westeros->cant_casas_extintas)++;
		}
	}
}

/*
 *Muestra por pantalla la casa ganadora y su cantidad de integrantes o en caso de que se hallan extinto las casas se informara sobre ello.
 */
void resultado(westeros_t* westeros){
	if(westeros->casa_ganadora_integrantes>0){
		printf("\n\nLa casa ganadora es %s. Cantidad de integrantes: %i\n", westeros->casa_ganadora, westeros->casa_ganadora_integrantes);
	}
	else{
		printf("\n\nSe extinguieron todas las casas. No hay ganador\n");
	}
}

/*
 *Dada una cantidad de años, se simulara en cada casa el paso de ellos actualizando las edades de los integrantes.
 *En el caso en que una casa se queda sin integrantes, esta se extinguira y se la eliminara del abb de los reinos y se
 *insertara su nombre en la cola de las casas extintas.
 *Si no se extinguió se calculará la cantifdad de nuevos integrantes a nacer y se los incroporara a la casa correspondeinte.
 *Cada año se calcula la casa ganadora. Si finalmente todas las casas extinguieron, no hay casa ganadora
 *Si se pudo simular correctamente se informará por pantalla la casa ganadora y la funcion devolvera EXITO(0) o ERROR(-1) si ocurriio algún error.
 */
int simulacion(westeros_t* westeros, int anios_pedidos){
	int estado = EXITO;

	FILE* nombre_de_bebes;
	nombre_de_bebes = fopen(ARCHIVO_CON_NOMBRES,LECTURA);
	if(!nombre_de_bebes){
		return ERROR;
	}
	int anios_transcurridos=0;

	casa_t* array_de_casas[westeros->cant_casas];
	arbol_recorrido_inorden(westeros->reino, (void*)array_de_casas, westeros->cant_casas);

	while(anios_transcurridos < anios_pedidos && estado == EXITO){
		westeros->casa_ganadora = NULL;
		westeros->casa_ganadora_integrantes=0;

		for(int i = 0; i<westeros->cant_casas; i++){
			estado = actualizar_edades(array_de_casas[i]);
			if(array_de_casas[i]->cantidad_de_personas >0){
				estado = nacimientos(westeros, array_de_casas[i], nombre_de_bebes);
				elegir_casa_ganadora(westeros, array_de_casas[i]);
			}
		}
		anios_transcurridos++;
	}
	if(estado == ERROR){
		return ERROR;
	}
	borrar_y_encolar_casas_extintas(westeros, array_de_casas);

	resultado(westeros);
	fclose(nombre_de_bebes);
	return estado;
}

/*************** LISTAR *****************/

/*
 *Ordena un vector de casas descendentemente.
 */
void ordenar_casas_descendentemente(casa_t** vector, int tope){
	casa_t* aux=NULL;
	int i=1;

	bool esta_ordenado= false;
	while(i<tope && !esta_ordenado){
		esta_ordenado= true;
		for(int j = 0; j < tope-1; j++){
			if(vector[j+1]->cantidad_de_personas > vector[j]->cantidad_de_personas){
				aux = vector[j+1];
				vector[j+1] = vector[j];
				vector[j] = aux;
				esta_ordenado = false;
			}
		}
		i++;
	}
}

/*
 *Mostrará por pantalla las casas existentes y la cantidad de miembros de forma descendente.
 */
int listar(westeros_t* westeros){
	printf("\n\nLISTA:\n");
	if(westeros->cant_casas == 0){
		printf("---\nNo hay Reinos\n");
	}

	casa_t* array_de_casas[westeros->cant_casas];
	int cantidad= arbol_recorrido_inorden(westeros->reino, (void*)array_de_casas, westeros->cant_casas);

	ordenar_casas_descendentemente(array_de_casas, cantidad);

	for(int i=0; i< cantidad; i++){
		printf("\t %s cantidad de personas: %i\n", array_de_casas[i]->nombre, array_de_casas[i]->cantidad_de_personas);
	}

	return EXITO;

}


/*************** EXTINTOS **************/

/*
 *Se imprimirá los nombres de las casas extintas en el orden en el que fueron desapareciendo. Luego de ello la cola de casas extintas queda vacia.
 *Devuelve EXITO si
 */
int extintos(westeros_t* westeros){
	int estado= EXITO;
	printf("\n\nREINOS EXTINTOS\n");

	cola_t* casas_extintas = westeros->reinos_extintos;
	if(cola_vacia(casas_extintas)){
		printf("No hay reinos extintos.\n");
		return EXITO;
	}
	int i = 0;

	while( i < cola_cantidad(casas_extintas) && estado==EXITO){
		char* nombre = (char*)cola_primero(casas_extintas);
		printf("%s\n", nombre);
		estado = cola_desencolar(casas_extintas);
		free(nombre);

		i++;
	}
	if(estado == ERROR || !cola_vacia(casas_extintas)){
		return ERROR;
	}
	return EXITO;
}

/************ DESTRUIR ****************/

/*
 *Destruye la estructura del continente liberando la correspondiente memoria.
 */
void destruir_westeros(westeros_t* westeros){
	casa_t* array_de_casas[westeros->cant_casas];

	int cantidad = arbol_recorrido_inorden(westeros->reino, (void**)array_de_casas, westeros->cant_casas);

	for(int i= 0; i < cantidad; i++){
		arbol_borrar(westeros->reino, (void*)array_de_casas[i]);
		destructor_de_casas(array_de_casas[i]);
	}
	if(!cola_vacia(westeros->reinos_extintos)){

		int cant_extintos = cola_cantidad(westeros->reinos_extintos);
		for(int i = 0; i< cant_extintos;i++){
			free(cola_primero(westeros->reinos_extintos));
			cola_desencolar(westeros->reinos_extintos);
		}
	}
	cola_destruir(westeros->reinos_extintos);
	arbol_destruir(westeros->reino);
	free(westeros);
}
