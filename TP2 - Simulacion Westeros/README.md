# TP2 - TRONO DE HIERRO:

### Las convenciones tomadas para este programa fueron:

- El reino ganador será cual tenga mayo cantidad de integrantes.

- En caso de que se hayan extinto todas los reinos, No habrá reino ganador

- Un reino se considera extinto cuando no posee integrantes. Cuando se extingue se conserva su nombre y si desea verlo puede seleccionar la opcion 'E'

- Al simular primero se actualizan las edades, los integrantes mueren a la edad de los 60. Si sigue teniendo integrantes, se calcularan cuantas personas deben nacer y se las incorporara.

- La variacion de edad entre cada año  se calcula de la siguiente manera: edad = edad + factor de envejecimiento

- la cantidad de nacimientos por año se determina: cantidad a nacer = cantidad de integrantes/factor de nacimiento

#### Para que la lectura de su archivo sea valida, su archivo debe tener el siguiente formato:
- Si es un reino "C;Nombre_del_reino;factor_de_envejecimiento;factor_de_nacimiento"
- Si es una persona "P;nombre;edad"

(Reemplazar factor_de_envejecimiento, factor_de_nacimientoy edad por numeros)
Si no le queda claro, usted puede tomar como ejemplo el archivo "datos.dat".

Si desea ver las casas extintas, una vez mostradas, se tomará como si nunca estuvieron (es decir, no estan dentro de Westeros y por ende si pide volver a mostrar las casas extintas, estas no apareceran).
Los reinos extintos se muestran en el orden que fueron desapareciendo.

Listar lista los reinos (incluido el ganador) de forma descendente. No se mostraran los reinos extintos, para ello elija la opción "E".


## :pushpin: Compilación
```
gcc *. c -o tronodehierro -g -std=c99 -Wall -Wconversion -Wtype -limits -pedantic -Werror -
```

## :pushpin: Ejecución con valgrind
```
valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./tronodehierro
```

(Se entregan todos los archivos correspondientes para la ejecucion del programa. Deben estar todos en la misma carpeta para su correcto funcionamiento)
