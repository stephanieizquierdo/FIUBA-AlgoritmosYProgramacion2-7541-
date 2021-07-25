#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "westeros.h"

#define ERROR_APERTURA_ARCHIVO -2
#define MAX_NOMBRE 100
#define AGREGAR_CASA 'A'
#define SIMULACION 'S'
#define LISTAR 'L'
#define EXTINTOS 'E'
#define QUIT 'Q'

#define EXITO 0

void mensaje_de_error_apertura(){
	printf("Error al abrir el archivo.\n");
}

void mensaje_exitoso(){
	printf("\nLa operacion fue realizada exitosamente :) \n");
}

void mensaje_de_error(){
	printf("\nLa operación no se ha podido completar :( \n");
}
void mensaje_de_bienvenida(){
	printf("｡.｡.｡:+* ﾟ ゜ﾟ *+:｡.｡:+* ﾟ ゜ﾟ *SIMULACION DE JUEGOS DE TRONOS* ﾟ ゜ﾟ *+:｡.｡:+* ﾟ ゜ﾟ *+:｡.｡.｡\n\n");
	printf("Cada año que transcurre, en una casa pueden morir o nacer integrantes. Esto depende del factor de nacimiento y de envejecimiento de la misma. Para que muera un integrante debe llegar a la edad de 60 años\n");
	printf("Una vez acabada la simulación, la casa con la mayor cantidad de integrantes será la que ocupe el Trono de Hierro.\n\n");
}

void opciones(){
	printf("\nIngrese A para cargar reinos, S para comenzar la simulacion, L para listar las casas de forma desendente, E para ver los reinos extintos o Q para cerrar el programa: ");
}

void mensaje_de_ayuda(){
	printf("\nAyuda: \n");
	printf("\tIngrese A para cargar un archivo con los reinos y luego ingrese el nombre del archivo con su correspondiente extension\n");
	printf("\n\tIngrese S para simular con el archivo previamente cargado. Luego ingrese la cantidad de años que desea simular \n");
	printf("\n\tIngrese L para listar las casas por cantidad de integrantes en orden descendente.\n");
	printf("\n\tIngrese E para mostrar las casas extintas en el orden en el que fueron desapareciendo.\n");
	printf("\n\tIngrese Q para finalizar el programa.\n");
}

void obtener_anios(int* anios){
	printf("Indique cuantos años quiere simular: ");
   	scanf(" %i", anios);
}

int main(){

	westeros_t* westeros = inicializar_westeros();
	if(!westeros){
		return 0;
	}

	char opcion;
	mensaje_de_bienvenida();

	do{
		opciones();
		scanf(" %c", &opcion);
	
	    if(opcion == AGREGAR_CASA){
	
	    	char nombre_archivo[MAX_NOMBRE];
	    	printf("Ingrese el nombre del archivo: ");
	    	scanf(" %s", nombre_archivo);

			int agregar= agregar_casas(westeros, nombre_archivo);
	    	if( agregar == EXITO){
	    		mensaje_exitoso();
	    	}
	    	else if(agregar== ERROR_APERTURA_ARCHIVO){
	    		mensaje_de_error_apertura();
	    	}else{
	    		mensaje_de_error();
	    	}
	    }

	   else if(opcion == SIMULACION){
	   		int anios;
	    	obtener_anios(&anios);
			simulacion(westeros, anios) == EXITO? mensaje_exitoso(): mensaje_de_error();
	    }
	    
	    else if(opcion == LISTAR){
			listar(westeros) == EXITO? mensaje_exitoso(): mensaje_de_error();
	    }
	    else if(opcion == EXTINTOS){
	    	extintos(westeros) == EXITO? mensaje_exitoso(): mensaje_de_error();
	
	    }
	    else if(opcion == QUIT){
	    	destruir_westeros(westeros);
	    }
	    else
	    	mensaje_de_ayuda();
	}while(opcion != QUIT);
	
	return 0;
}