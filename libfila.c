#include <stdlib.h>
#include <stdio.h>
#include "libfila.h"

/* cria fila vazia e retorna a propria, em caso de erro retorna NULL*/
fila_t* cria_fila(){
    fila_t *f;
    if((f = malloc(sizeof(fila_t)))){
        f->fim = NULL;
        f->ini = NULL;
        f->tamanho = 0;
        return f;
    }
    return NULL;
}

/* remove os elementos da fila e libera 0 espaco e retorna NULL*/
fila_t* destroi_fila(fila_t* f){
    int elemento;
    while(fila_vazia(f) != 1){
        retira_fila(f, &elemento);
    }
    free(f);
    return NULL;
}

/* retorna o tamanho da fila*/
int tamanho_fila(fila_t* f){
    return f->tamanho;
}

/* retorna 1 se fila for vazia e 0 se nao estiver*/
int fila_vazia(fila_t* f){
    if(tamanho_fila(f) == 0){
        return 1;
    }
    return 0;
}

/* insere no final da fila e retorna 1 se der certo e 0 caso contrario*/
int insere_fila(fila_t* f, int elemento){
    nodo_f_t *nodo;
    nodo = malloc(sizeof(nodo_f_t));
    if((nodo != NULL)){
        nodo->chave = elemento;
        if(fila_vazia(f) == 1){
            f->ini = nodo;
            nodo->prox = NULL;
            nodo->prev = NULL;
        }else{
            f->fim->prox = nodo;
            nodo->prox = NULL;
            nodo->prev = f->fim;
        }
        f->fim = nodo;
        f->tamanho++;
        return 1;
    }
    return 0;
}
    
/* Remove elemento do inicio e o retorna, se der certo retorna 1, e 0 caso contrario*/
int retira_fila(fila_t* f, int *elemento){    
    if(fila_vazia(f) == 1)
        return 0;
    nodo_f_t *inicio;
    *elemento = f->ini->chave;
    inicio = f->ini;
    f->ini = f->ini->prox;
    f->tamanho--;
    free(inicio);
    return 1;   
}