#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // For ssize_t in Linux.
#include "heap.h"
#include "testing.h"
#define TAM_ARREGLO 5

/* ******************************************************************
 *                        FUNCIONES AUXILIARES
 * *****************************************************************/

int cmp_int(const void* a, const void* b){
    if(*(int*)a == *(int*)b) return 0;
    return *(int*)a > *(int*)b ? 1 : -1;
}

int cmp_char(const void* a, const void* b){
    return strcmp(a, b);
}

/* ******************************************************************
 *                        PRUEBAS UNITARIAS
 * *****************************************************************/

static void prueba_crear_heap_vacio()
{
    heap_t* heap = heap_crear(cmp_int);

    print_test("Prueba heap crear heap vacio", heap);
    print_test("Prueba heap esta vacio", heap_esta_vacio(heap));
    print_test("Prueba heap ver maximo es NULL", heap_ver_max(heap) == NULL);
    print_test("Prueba heap desencolar es NULL", heap_desencolar(heap) == NULL);

    heap_destruir(heap, NULL);
}

static void prueba_heap_encolar()
{
    heap_t* heap = heap_crear(cmp_int);

    int valor1 = 2;
    int valor2 = 4;
    int valor3 = 6;

    /* Inserta 1 valor y luego lo desencola */
    print_test("Prueba heap encolar valor1", heap_encolar(heap, &valor1));
    print_test("Prueba heap no esta vacio", !heap_esta_vacio(heap));
    print_test("Prueba heap la cantidad de elementos es 1", heap_cantidad(heap) == 1);
    print_test("Prueba heap ver max es valor1", heap_ver_max(heap) == &valor1);
    print_test("Prueba heap desencolar es valor1", heap_desencolar(heap) == &valor1);
    print_test("Prueba heap esta vacio", heap_esta_vacio(heap));

    /* Inserta otros 2 valores y verifica que se mantenga la invariante de heap */
    print_test("Prueba heap encolar valor2", heap_encolar(heap, &valor2));
    print_test("Prueba heap no esta vacio", !heap_esta_vacio(heap));
    print_test("Prueba heap la cantidad de elementos es 1", heap_cantidad(heap) == 1);
    print_test("Prueba heap ver max es valor2", heap_ver_max(heap) == &valor2);

    print_test("Prueba heap encolar valor3", heap_encolar(heap, &valor3));
    print_test("Prueba heap la cantidad de elementos es 2", heap_cantidad(heap) == 2);
    print_test("Prueba heap ver max es valor3", heap_ver_max(heap) == &valor3);

    heap_destruir(heap, NULL);
}

static void prueba_heap_con_destruir()
{
    heap_t* heap = heap_crear(cmp_char);

    /* Pide memoria para 4 valores */
    char* valor1 = malloc(10 * sizeof(char));
    char* valor2 = malloc(8 * sizeof(char));
    char* valor3 = malloc(6 * sizeof(char));

    /* Inserta 2 valores y luego se liberan en heap_destruir */
    print_test("Prueba heap encolar valor1", heap_encolar(heap, valor1));
    print_test("Prueba heap insertar valor2", heap_encolar(heap, valor2));
    print_test("Prueba heap insertar valor3", heap_encolar(heap, valor3));
    print_test("Prueba heap ver max es valor1", heap_ver_max(heap) == valor1);
    print_test("Prueba heap no esta vacio", !heap_esta_vacio(heap));
    print_test("Prueba heap la cantidad de elementos es 3", heap_cantidad(heap) == 3);

    heap_destruir(heap, free);
}

static void prueba_heap_arr()
{
    /* Creo arreglo [3, 6, 4, 7, 5] */
    int valor1 = 3;
    int valor2 = 6;
    int valor3 = 4;
    int valor4 = 7;
    int valor5 = 5;
    void** arr = calloc(1, sizeof(void*) * TAM_ARREGLO);
    arr[0] = &valor1;
    arr[1] = &valor2;
    arr[2] = &valor3;
    arr[3] = &valor4;
    arr[4] = &valor5;

    heap_t* heap = heap_crear_arr(arr, TAM_ARREGLO, cmp_int);

    /* Pruebo que se haya creado bien el heap */
    print_test("Prueba heap crear desde arreglo", heap);
    print_test("Prueba heap ver max es valor4", heap_ver_max(heap) == &valor4);
    print_test("Prueba heap no esta vacio", !heap_esta_vacio(heap));
    print_test("Prueba heap la cantidad de elementos es 5", heap_cantidad(heap) == 5);

    /* Desencolo y encolo elementos y pruebo que se mantenga la invariante de heap */
    print_test("Prueba heap desencolar es valor4", heap_desencolar(heap) == &valor4);
    print_test("Prueba heap la cantidad de elementos es 4", heap_cantidad(heap) == 4);
    print_test("Prueba heap ver max es valor2", heap_ver_max(heap) == &valor2);
    print_test("Prueba heap encolar valor4", heap_encolar(heap, &valor4));
    print_test("Prueba heap ver max es valor4", heap_ver_max(heap) == &valor4);
    print_test("Prueba heap la cantidad de elementos es 5", heap_cantidad(heap) == 5);

    /* Desencolo todos los elementos y pruebo que haya quedado vacio */
    for(size_t i = 0; i < heap_cantidad(heap); i++){
        heap_desencolar(heap);
    }

    print_test("Prueba heap esta vacio", heap_esta_vacio(heap));
    print_test("Prueba heap ver max es NULL", heap_ver_max(heap) == NULL);
    print_test("Prueba heap desencolar es NULL", heap_desencolar(heap) == NULL);

    heap_destruir(heap, NULL);
}

void pruebas_heap_alumno(void){
    prueba_crear_heap_vacio();
    prueba_heap_encolar();
    prueba_heap_con_destruir();
    prueba_heap_arr();
}