#include "pila.h"
#include <stdio.h>

int main(){
	pila_t* pila = pila_crear();
	
	char elemento_1 = '2';
	char elemento_2 = 'o';
	char elemento_3 = 'g';
	char elemento_4 = 'l';
	char elemento_5 = 'A';
	
for (int i = 0; i < 3; i++) {
		pila_apilar(pila, &elemento_1);
		pila_apilar(pila, &elemento_2);
		pila_apilar(pila, &elemento_3);
		pila_apilar(pila, &elemento_4);
		pila_apilar(pila, &elemento_5);
		printf("Tamanio pila: %i\n", pila->tamanio);
	}
	
	for (int i = 0; i < 5; i++) {
		printf("%c\n", *(char*)pila_tope(pila));
		pila_desapilar(pila);
	}
	printf("Tamanio pila: %i\n", pila->tamanio);
	
	for (int i = 0; i < 5; i++) {
		pila_desapilar(pila);
	}
	printf("Tamanio pila: %i\n", pila->tamanio);
	
	for (int i = 0; i < 5; i++) {
		pila_desapilar(pila);
	}
	printf("Tamanio pila: %i\n", pila->tamanio);
	pila_destruir(pila);
	return 0;
}
