#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "heap.h"
#define TAM_INI 10
#define FACTOR_REDIMENSION 2

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

size_t calcular_pos_padre(size_t hijo){
    return (hijo - 1) / 2;
}

size_t calcular_pos_hijo(size_t padre, size_t n){
    return 2 * padre + n;
}

size_t calcular_hijo_mayor(void** datos, cmp_func_t cmp, size_t hijo_izq, size_t hijo_der){
    if (cmp(datos[hijo_der], datos[hijo_izq]) >= 0) return hijo_der;
	return hijo_izq;
}

size_t calcular_max(void** datos, cmp_func_t cmp, size_t padre, size_t hijo_izq, size_t hijo_der){
    size_t hijo_mayor = calcular_hijo_mayor(datos, cmp, hijo_izq, hijo_der);
    if (cmp(datos[padre], datos[hijo_mayor]) >= 0) return padre;
    return hijo_mayor;
}

void upheap(heap_t* heap, size_t hijo, cmp_func_t cmp){
    if(hijo == 0) return;
    size_t padre = calcular_pos_padre(hijo);
    if(cmp(heap->datos[padre], heap->datos[hijo]) < 0){
        swap(heap->datos[padre], heap->datos[hijo]);
        upheap(heap, padre, cmp);
    }
}

void downheap(heap_t* heap, size_t tam, size_t padre, cmp_func_t cmp){
    if(padre >= tam - 1) return;
    size_t hijo_izq = calcular_pos_hijo(padre, 1);
    size_t hijo_der = calcular_pos_hijo(padre, 2);
    size_t max = calcular_max(heap->datos, cmp, padre, hijo_izq, hijo_der);
    if(cmp(heap->datos[max], heap->datos[padre]) != 0){
        swap(heap->datos[padre], heap->datos[max]);
        downheap(heap, tam, max, cmp);
    }
}

bool heap_redimensionar(heap_t* heap, size_t nuevo_tam){
    void* datos_nuevo = realloc(heap->datos, nuevo_tam * sizeof(void*));
    if (nuevo_tam > 0 && !datos_nuevo) {
        return false;
    }
	heap->tam = nuevo_tam;
    heap->datos	= datos_nuevo;
    return true;
}

bool heap_esta_lleno(heap_t* heap){
    return heap->cant == heap->tam;
}

bool heap_hay_que_achicar(heap_t* heap){
    return heap->cant * 4 <= heap->tam;
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

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){
    return NULL;
}

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

bool heap_encolar(heap_t *heap, void *elem){
    if (!elem) return false;
    if (heap_esta_lleno(heap)) heap_redimensionar(heap, heap->tam*FACTOR_REDIMENSION);
    heap->datos[heap->cant] = elem;
    heap->cant++;
    upheap(heap, heap->cant-1, heap->cmp);
    return true;
}

void *heap_ver_max(const heap_t *heap){
    if(heap_esta_vacio(heap)){
        return NULL;
    }
    return heap->datos[0];
}

void *heap_desencolar(heap_t *heap){
    size_t pos_ult = heap->cant - 1;
    void* ultimo = heap->datos[pos_ult];
    void* primero = heap_ver_max(heap);
    swap(ultimo, primero);
    heap->cant--;
    downheap(heap, heap->tam, 0, heap->cmp);
    if(heap_hay_que_achicar(heap)){
        heap_redimensionar(heap, heap->tam / 2);
    }
    return primero;
}

/* *****************************************************************
 *                    HEAP SORT
 * *****************************************************************/

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp);