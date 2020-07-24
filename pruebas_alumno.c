#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // For ssize_t in Linux.
#include "heap.h"
#include "testing.h"

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

bool comparar_arreglos(void* arr1[], void* arr2[], size_t n){
    bool son_iguales = true;
    for (size_t i=0; i < n; i++){
        if ( *(int*) arr1[i] != *(int*) arr2[i]){
            son_iguales = false;
        }
    }
    return son_iguales;
}

/* ******************************************************************
 *                        PRUEBAS UNITARIAS
 * *****************************************************************/

static void prueba_crear_heap_vacio()
{
    printf("\n **** PRUEBA HEAP VACIO ****\n");
    heap_t* heap = heap_crear(cmp_int);

    print_test("Prueba heap crear heap vacio", heap);
    print_test("Prueba heap esta vacio", heap_esta_vacio(heap));
    print_test("Prueba heap ver maximo es NULL", heap_ver_max(heap) == NULL);
    print_test("Prueba heap desencolar es NULL", heap_desencolar(heap) == NULL);

    heap_destruir(heap, NULL);
}

static void prueba_heap_encolar()
{
    printf("\n **** PRUEBA HEAP ENCOLAR ****\n");
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
    printf("\n ** PRUEBA HEAP CON DESTRUIR **\n");
    heap_t* heap = heap_crear(cmp_char);

    /* Pide memoria para 4 valores */
    char* valor1 = "Nazarena";
    char* valor2 = "Ernesto";
    char* valor3 = "Algo2";
    char* copia_1 = strdup(valor1);
    char* copia_2 = strdup(valor2);
    char* copia_3 = strdup(valor3);

    /* Inserta 2 valores y luego se liberan en heap_destruir */
    print_test("Prueba heap encolar valor1", heap_encolar(heap, copia_1));
    print_test("Prueba heap insertar valor2", heap_encolar(heap, copia_2));
    print_test("Prueba heap insertar valor3", heap_encolar(heap, copia_3));
    // print_test("Prueba heap ver max es valor1", heap_ver_max(heap) == valor1);
    print_test("Prueba heap no esta vacio", !heap_esta_vacio(heap));
    print_test("Prueba heap la cantidad de elementos es 3", heap_cantidad(heap) == 3);

    heap_destruir(heap, free);
}

static void prueba_heap_arr()
{
    printf("\n ** PRUEBA HEAP CREAR DESDE ARREGLO **\n");
    /* Creo arreglo [3, 6, 4, 7, 5] */
    int valor1 = 3;
    int valor2 = 6;
    int valor3 = 4;
    int valor4 = 7;
    int valor5 = 5;
    void* arr[] = {&valor1, &valor2, &valor3, &valor4, &valor5};

    heap_t* heap = heap_crear_arr(arr, 5, cmp_int);

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
    for(size_t i = 0; i < 5; i++){
        heap_desencolar(heap);
    }

    print_test("Prueba heap esta vacio", heap_esta_vacio(heap));
    print_test("Prueba heap ver max es NULL", heap_ver_max(heap) == NULL);
    print_test("Prueba heap desencolar es NULL", heap_desencolar(heap) == NULL);

    heap_destruir(heap, NULL);
}

static void prueba_heap_volumen(size_t largo, bool debug)
{
    printf("\n **** PRUEBA HEAP VOLUMEN ****\n");
    heap_t* heap = heap_crear(cmp_char);

    const size_t largo_clave = 10;
    char (*claves)[largo_clave] = malloc(largo * largo_clave);

    /* Inserta 'largo' claves en el heap */
    bool ok = true;
    for (unsigned i = 0; i < largo; i++) {
        sprintf(claves[i], "%08d", i);
        ok = heap_encolar(heap, claves[i]);
        if (!ok) break;
    }

    if (debug) print_test("Prueba heap almacenar muchos elementos", ok);
    if (debug) print_test("Prueba heap la cantidad de elementos es correcta", heap_cantidad(heap) == largo);

    /* Desencolo todos los elementos del heap */
    for (size_t i = 0; i < largo; i++) {
        ok = heap_desencolar(heap) != NULL;
        if (!ok) break;
    }

    if (debug) print_test("Prueba heap borrar muchos elementos", ok);
    if (debug) print_test("Prueba heap la cantidad de elementos es 0", heap_cantidad(heap) == 0);

    /* Destruye el heap*/
    heap_destruir(heap, NULL);

    free(claves);
}

void pruebas_heap_sort(){
    printf("\n ** PRUEBA HEAPSORT **\n");
    /* Creo arreglo desordenado [3, 6, 4, 7, 5] */
    int valor1 = 3;
    int valor2 = 6;
    int valor3 = 4;
    int valor4 = 7;
    int valor5 = 5;
    void* arr[] = {&valor1, &valor2, &valor3, &valor4, &valor5};

    /* Creo arreglo ordenado [3, 4, 5, 6, 7] */
    void* arreglo_ordenado[] = {&valor1, &valor3, &valor5, &valor2, &valor4};
    heap_sort(arr, 5, cmp_int);
    
    bool iguales = comparar_arreglos(arr, arreglo_ordenado, 5);

    print_test("El arreglo se encuentra ordenado luego de aplicar heapsort", iguales == true);
}


void pruebas_heap_alumno(void){
    prueba_crear_heap_vacio();
    prueba_heap_encolar();
    prueba_heap_con_destruir();
    prueba_heap_arr();
    prueba_heap_volumen(100000, true);
    pruebas_heap_sort();
}