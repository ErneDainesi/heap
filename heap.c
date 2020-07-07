#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "heap.h"

/* *****************************************************************
 *                        ESTRUCTURAS
 * *****************************************************************/

struct heap{
    void** datos;
    size_t cant;
    size_t tam;
    cmp_func_t cmp;
};

/* *****************************************************************
 *                    FUNCIONES AUXILIARES
 * *****************************************************************/
// upheap, downheap

/* *****************************************************************
 *                    PRIMITIVAS DEL HEAP
 * *****************************************************************/

heap_t *heap_crear(cmp_func_t cmp);

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp);

void heap_destruir(heap_t *heap, void (*destruir_elemento)(void *e));

size_t heap_cantidad(const heap_t *heap){
    return heap->cant;
}

bool heap_esta_vacio(const heap_t *heap){
    return heap->cant == 0;
}

bool heap_encolar(heap_t *heap, void *elem);

void *heap_ver_max(const heap_t *heap){
    if(heap_esta_vacio(heap)){
        return NULL;
    }
    return heap->datos[0]; // esto nose si es asi, pero relleno para las pruebas.
}

void *heap_desencolar(heap_t *heap);

/* *****************************************************************
 *                    HEAP SORT
 * *****************************************************************/

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp);