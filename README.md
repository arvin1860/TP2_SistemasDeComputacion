# TP2 - Sistemas de Computación

Trabajo Práctico N.º 2 de la materia Sistemas de Computación.
## Integrantes

- Mario Lautaro Marin Martinez
- Florencia Tobares
- Marcos Viera Colombo

## Descripción

El programa obtiene el índice GINI de Argentina desde la API REST del Banco Mundial.

El dato obtenido es procesado en C y enviado a una rutina desarrollada en Assembly x86-64 utilizando la convención de llamadas System V AMD64.

La rutina Assembly recibe el índice GINI como un valor de punto flotante, lo convierte a entero, le suma 1 y devuelve el resultado al programa en C.

## Funcionamiento

El flujo general del programa es:

1-API REST del Banco Mundial

2-Programa en C

3-Índice GINI como float 

4-Paso del parámetro mediante el stack

5-Rutina Assembly x86-64

6-Conversión de float a entero

7-Suma de 1

8-Retorno del resultado a C

Para forzar el paso del índice GINI mediante el stack, se utiliza como noveno argumento de tipo float. Según la convención System V AMD64, los primeros ocho argumentos de punto flotante utilizan los registros XMM0-XMM7 y el argumento siguiente se almacena en el stack.

## Archivos principales

- `main.c`: consulta la API REST, obtiene el índice GINI y llama a la rutina Assembly.
- `conversion.s`: rutina Assembly encargada de convertir el índice de float a entero y sumar 1.
- `gini.c`: programa utilizado inicialmente para comprobar la comunicación con la API del Banco Mundial.
- `.gitignore`: evita almacenar en Git archivos generados durante la compilación.
-  `.gini_c.c`: Funcion que transforma numero decimal en entero y suma 1.
- `main_c.c`: Main que utiliza como datos de entrada la informacion del Banco Mundial y asigna un valor a cada año.

## Requisitos

El proyecto fue desarrollado para Linux x86-64.

Se requiere:

- GCC
- GNU Assembler
- GDB
- libcurl
- Git

## Instalación de dependencias

En Ubuntu, la biblioteca libcurl puede instalarse mediante:

sudo apt install libcurl4-openssl-dev


## Compilación

Primero se ensambla el archivo Assembly:

as --64 -g -o conversion.o conversion.s

Luego se compila el programa en C:

gcc -g -O0 -c main.c -o main.o

Finalmente se enlazan el código C, el código Assembly y la biblioteca libcurl:

gcc main.o conversion.o -lcurl -o programa

## Recorrido del codigo sin assembler 

Pide los datos con libcurl hace un GET a la URL de la API (país ARG, indicador SI.POV.GINI).

Los guarda en memoria, la función escribir va juntando los pedazos que llegan en un buffer que crece con realloc, hasta tener el JSON completo como un string.

Recorre el JSON busca con strstr cada "date":" para leer el año y luego "value": para leer el valor.

Filtra y convierte, si el valor es null, saltea ese año. Si no, strtod lo pasa a double y se lo entrega a gini_mas_uno_c, que lo trunca y suma 1.

Muestra y limpia, imprime año: Gini valor -> resultado y libera la memoria con free.

## Ejecución 

Para ejecutar el programa:

./programa

Ejemplo de salida:

Indice GINI de Argentina: 42.4
GINI convertido a entero + 1: 43

## Depuración con GDB

Para comprobar el funcionamiento de la rutina Assembly y analizar el uso del stack se utilizó GDB.

### Antes de ingresar a la rutina Assembly

Se detiene la ejecución al comienzo de `convertir_gini`. El noveno parámetro se encuentra en `$rsp+8`. Al interpretarlo como `float`, GDB muestra `42.4000015`, correspondiente al índice GINI `42.4`.

<img width="567" height="128" alt="image" src="https://github.com/user-attachments/assets/5d710233-30b4-4b97-9aa5-a8eca4f0d39f" />

### Al finalizar la rutina Assembly

Antes de ejecutar `ret`, el registro `%rax` contiene `0x2b`, equivalente a `43` en decimal. Esto demuestra que Assembly convirtió `42.4` a `42` y posteriormente sumó `1`.

<img width="501" height="183" alt="image" src="https://github.com/user-attachments/assets/bc3b5c97-4ce6-43a8-8522-1b8914727633" />


### Retorno al programa principal

Luego de ejecutar `ret`, la ejecución vuelve a `main.c`. El registro `%rax` conserva el valor `43`, correspondiente al resultado devuelto por la rutina Assembly.

<img width="456" height="196" alt="image" src="https://github.com/user-attachments/assets/9bfbb00c-3e5a-4ff7-9a3b-2955aec01c33" />


## Tecnologías utilizadas

- C
- Assembly x86-64
- System V AMD64 ABI
- GCC
- GNU Assembler
- GDB
- libcurl
- API REST del Banco Mundial
- Git
- GitHub
