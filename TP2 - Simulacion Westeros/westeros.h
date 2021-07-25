#ifndef __WESTEROS_H__
#define __WESTEROS_H__

typedef struct persona persona_t;

typedef struct casa casa_t;

typedef struct westeros westeros_t;
westeros_t* inicializar_westeros();

int agregar_casas(westeros_t* westeros, char* nombre_archivo);

void destruir_westeros(westeros_t* westeros);
int simulacion(westeros_t* westeros, int anios);
int extintos(westeros_t* westeros);
int listar(westeros_t* westeros);

#endif /* __ARBOL_BINARIO_DE_BUSQUEDA_H__ */
