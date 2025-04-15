# TP2 assembly x86_64 

## Ej1


En el directorio de este ejercicio encontrarán los siguientes archivos:

- Makefile
- ej1.asm
- ej1.c
- ej1.h
- main.c
- runMain.sh
- runTester.sh
- tester.c
- salida.catedra.ej1.txt

Los archivos sobre los que deben trabajar son **ej1.asm** y **ej1.c**, aquí deberán implementar las funciones solicitadas en el enunciado. El archivo **ej1.h** contiene las declaraciones de las estructuras y funciones. En la línea 8 de este archivo se encuentra definida la directiva:
    
    #define USE_ASM_IMPL 1 

la cual está inicializada en 1 para probar la implementación en ensamblador. Si se desea evaluar la implementación en C, es necesario cambiar su valor a 0:

    #define USE_ASM_IMPL 0
    
El archivo **main.c** también lo pueden modificar y es para que ustedes realicen sus propias pruebas. Siéntanse a gusto de manejarlo como crean conveniente. Para compilar el código y poder correr las pruebas cortas implementadas en main.

Compila:

    make main

Para correr sus pruebas:

    ./runMain.sh

**runMain.sh** verifica que no se pierde memoria ni se realizan accesos incorrectos a la misma.

Para compilar el código y correr las pruebas intensivas, deberá ejecutar:


    ./runTester.sh

En este punto se usa el archivo **salida.catedra.ej1.txt** para comparar la salida que produce su código con el de la cátedra. 

## EJ2

