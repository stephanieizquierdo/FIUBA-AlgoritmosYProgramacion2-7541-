# TDA - PILA

Para el TDA Pila se utilizaran pruebas con las siguientes conveciones:

TO DO: AGREGAR LAS PRUEBAS PROPIAS.

Convencion de redimensionar:
Si la cantidad de elementos que tiene la pila es 3 veces menor que el tamañao de la pila, se redimenionara a un cuarenta porciento, dejando asi un nuevo tamaño de la pila.
El tamaño de la pila nunca puede ser menor que su tamaño minimo, tres.

## :pushpin: Compilación
Debe poner el archivo de pruebas en la misma carpeta donde se encuentran los archivos del tda pila y luego en la consola ejecutar la siguiente linea:
```
gcc *. c -o pila_vd -g - std = c99 - Wall - Wconversion - Wtype - limits - pedantic - Werror - O0
```
## :pushpin: Ejecución con valgrind
```
valgrind -- leak - check = full -- track - origins = yes -- show - reachable = yes ./ pila_vd
```
