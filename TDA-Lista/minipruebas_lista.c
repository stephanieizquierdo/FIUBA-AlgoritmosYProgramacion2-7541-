#include "lista.h"
#include <stdio.h>

void mostrar_elemento(void* elemento){
  if(elemento)
    printf("%c ", *(char*)elemento);
}

int main(){

  lista_t* lista = lista_crear();
  
  char a='a', b='b', c='c', d='d', w='w';
  
  lista_insertar(lista, &a);
  lista_insertar(lista, &c);
  lista_insertar_en_posicion(lista, &d, 100);
  lista_insertar_en_posicion(lista, &b, 1);
  lista_insertar_en_posicion(lista, &w, 3);
  
  lista_borrar_de_posicion(lista, 3);

  printf("Elementos en la lista: ");
  for(size_t i=0;i<lista_elementos(lista);i++)
    printf("%c ", *(char*)lista_elemento_en_posicion(lista, i));

  printf("\n");

  lista_iterador_t* it = lista_iterador_crear(lista);
  while(lista_iterador_tiene_siguiente(it))
    printf("%c ", *(char*)lista_iterador_siguiente(it));
  printf("\n");

  lista_iterador_destruir(it);

  printf("Imprimo la lista usando el iterador interno: ");
  lista_con_cada_elemento(lista, mostrar_elemento);
  printf("\n");
  
  lista_destruir(lista);

  return 0;
}
