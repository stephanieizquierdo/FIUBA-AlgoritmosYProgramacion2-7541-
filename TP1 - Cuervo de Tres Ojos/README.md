# Trabajo Practico 1 - Cuervo de Tres Ojos

El Cuervo de Tres Ojos quiere transmitirle a su aprendiz, Bran, la mayor cantidad de visiones que pueda. Sin embargo, como es demasiada información para que el joven adquiera, decide comenzar inculcándole un conjunto de visiones del pasado y del futuro que compartan determinadas características.
Las visiones se encontraran en el archivo visiones.txt. El mismo contiene 4 datos por linea, separados entre si por un punto y coma.

## :pushpin: Compilación
Abrir terminal en la carpeta donde recide los archivos correspondientes e ingresar lo siguiente:
```
gcc *.c -Wall -Werror -Wconversion -std=c99 -o cuervo_de_tres_ojos
```
## :pushpin: Ejecución con valgrind
Abrir terminal donde se encuentra el ejecutable "cuervo_de_tres_ojos" e ingresar lo siguiente:

```
valgrind --leak-check=full --track -origins=yes --show-reachable=yes ./cuervo_de_tres_ojos
```


Detalles del enunciado: [TP1-Enunciado.pdf](https://github.com/stephanieizquierdo/FIUBA-AlgoritmosYProgramacion1-7540-/blob/master/TP1%20-%20Competencia%20Caliz%20de%20Fuego/TP1-Enunciado.pdf)
