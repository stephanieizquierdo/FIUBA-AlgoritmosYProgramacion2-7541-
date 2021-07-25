# TDA - ABB

TODO: SUBIR PRUEBAS Y README

Se entregan los archivos de TDA ABB (abb.h y abb.c) con sus respectivas pruebas(pruebas_abb.h pruebas_abb.c mispruebasabb.c).
Las pruebas verifican que el TDA funcione correctamente y cumpla con el contrato preestablecido.

Para ejecutar el TDA con sus respectivas pruebas, debe ingresar las siguientes lineas, desde la carpeta donde se encuentra el TDA, en la terminal:

## :pushpin: Compilación
```
gcc *. c -o abb -g - std = c99 - Wall - Wconversion - Wtype - limits - pedantic - Werror - O0
```
## :pushpin: Ejecución con valgrind
```
valgrind -- leak - check = full -- track - origins = yes -- show - reachable = yes ./ abb
```
