#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "heap.h"
#define TAM_INI 10

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
void swap(void *x, void *y) {
    void* aux = x;
    x = y;
    y = aux;
}

int calcular_pos_padre(int hijo){
    return (hijo - 1) / 2;
}

int calcular_pos_hijo(int padre, int n){
    return 2 * padre + n;
}

int calcular_max(heap_t* heap, cmp_func_t cmp, int padre, int hijo_izq, int hijo_der){
    if( (cmp(heap->datos[padre], heap->datos[hijo_izq]) >= 0) && (cmp(heap->datos[padre], heap->datos[hijo_der]) >= 0) )
        return padre;
    if( (cmp(heap->datos[hijo_izq], heap->datos[padre]) >= 0) && (cmp(heap->datos[hijo_izq], heap->datos[hijo_der]) >= 0) )
        return hijo_izq;
    if( (cmp(heap->datos[hijo_der], heap->datos[padre]) >= 0) && (cmp(heap->datos[hijo_der], heap->datos[hijo_izq]) >= 0) )
        return hijo_der;
}

void upheap(heap_t* heap, int hijo, cmp_func_t cmp){
    if(hijo == 0) return;
    int padre = calcular_pos_padre(hijo);
    if(cmp(heap->datos[padre], heap->datos[hijo]) < 0){
        swap(heap->datos[padre], heap->datos[hijo]);
        upheap(heap, padre, cmp);
    }
}

void downheap(heap_t* heap, size_t tam, int padre, cmp_func_t cmp){
    if(padre >= tam - 1) return;
    int hijo_izq = calcular_pos_hijo(padre, 1);
    int hijo_der = calcular_pos_hijo(padre, 2);
    int max = calcular_max(heap, cmp, padre, hijo_izq, hijo_der);
    if(cmp(heap->datos[max], heap->datos[padre]) != 0){
        swap(heap->datos[padre], heap->datos[max]);
        downheap(heap, tam, max, cmp);
    }
}

/* *****************************************************************
 *                    PRIMITIVAS DEL HEAP
 * *****************************************************************/

heap_t *heap_crear(cmp_func_t cmp){
    heap_t* heap = malloc(sizeof(heap_t));
    if(!heap) return NULL;
    heap->datos = malloc(sizeof(void*) * TAM_INI);
    if(!heap->datos){
        free(heap);
        return NULL;
    }
    heap->cant = 0;
    heap->tam = TAM_INI;
    heap->cmp = cmp;
    return heap;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp);

void heap_destruir(heap_t *heap, void (*destruir_elemento)(void *e)){
    if(destruir_elemento){
        for(int i = 0; i < heap->tam; i++){
            destruir_elemento(heap->datos[i]);
        }
    }
    free(heap->datos);
    free(heap);
}

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
    return heap->datos[0];
}

void *heap_desencolar(heap_t *heap);

/* *****************************************************************
 *                    HEAP SORT
 * *****************************************************************/

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp);