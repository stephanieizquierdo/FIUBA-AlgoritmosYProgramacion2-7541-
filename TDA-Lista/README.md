# TDA - LISTA

TO DO: AGREGAR LAS PRUEBAS PROPIAS.

Se entregan los archivos TDA LISTA (lista.h y lista.c) con sus respectivas pruebas(pruebaslista.h pruebaslista.c MisPruebasLista.c)
Las pruebas verifican que el TDA funcione correctamente y cumpla con el contrato preestablecido.

Para ejecutar el tda con sus respectivas pruebas, debe ingresar las siguientes lineas, desde la carpeta donde se encuentra el TDA, en la terminal:

## :pushpin: Compilación
```
gcc *. c -o lista -g - std = c99 - Wall - Wconversion - Wtype - limits - pedantic - Werror - O0
```
## :pushpin: Ejecución con valgrind
```
valgrind -- leak - check = full -- track - origins = yes -- show - reachable = yes ./lista
```
