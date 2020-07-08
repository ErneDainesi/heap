#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // For ssize_t in Linux.
#include "heap.h"
#include "testing.h"

/* ******************************************************************
 *                        FUNCIONES AUXILIARES
 * *****************************************************************/

int cmp(const void* a, const void* b){
    if(a < b) return -1;
    if(a > b) return 1;
    return 0;
}

int cmp_char(const void* a, const void* b){
    return strcmp(a, b);
}

/* ******************************************************************
 *                        PRUEBAS UNITARIAS
 * *****************************************************************/

static void prueba_crear_heap_vacio()
{
    heap_t* heap = heap_crear(cmp);

    print_test("Prueba heap crear heap vacio", heap);
    print_test("Prueba heap esta vacio", heap_esta_vacio(heap));
    print_test("Prueba heap ver maximo es NULL", heap_ver_max(heap) == NULL);
    print_test("Prueba heap desencolar es NULL", heap_desencolar(heap) == NULL);

    heap_destruir(heap, NULL);
}

static void prueba_heap_encolar()
{
    heap_t* heap = heap_crear(cmp);

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

    /* Inserta otros 2 valores y no los desencola (se destruyen con el heap) */
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

    /* Inserta 2 valores y luego los reemplaza (debe liberar lo que reemplaza) */
    print_test("Prueba heap encolar valor1", heap_encolar(heap, valor1));
    print_test("Prueba heap insertar valor2", heap_encolar(heap, valor2));
    print_test("Prueba heap insertar valor3", heap_encolar(heap, valor3));
    print_test("Prueba heap ver max es valor1", heap_ver_max(heap) == valor1);
    print_test("Prueba heap no esta vacio", !heap_esta_vacio(heap));
    print_test("Prueba heap la cantidad de elementos es 3", heap_cantidad(heap) == 3);

    /* Se destruye el heap (se debe liberar lo que quedó dentro) */
    heap_destruir(heap, free);
}

void pruebas_heap_alumno(void){
    prueba_crear_heap_vacio();
    prueba_heap_encolar();
    prueba_heap_con_destruir();
}