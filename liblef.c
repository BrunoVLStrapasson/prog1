#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "liblef.h"

lef_t* cria_lef(){
    lef_t *l;
    if((l = malloc(sizeof(lef_t)))){
        l->Primeiro = NULL;
        return l;
    }else
        return NULL;
}


lef_t* destroi_lef(lef_t *l){
    nodo_lef_t *no;
    if(l->Primeiro == NULL){
        free (l);
        return NULL;
    }
    while(l->Primeiro != NULL){
        no = l->Primeiro;
        l->Primeiro = l->Primeiro->prox;
        /*destroidados(void* evento->dados)*/
        free (no->evento->dados);
        free (no->evento);
        free (no);
    }
    free (l);
    return NULL;
}

/* cria uma copia do evento e se estiver vazia altera a sequencia dos ponteiros*/ 
int adiciona_inicio_lef(lef_t *l, evento_t *evento){
    nodo_lef_t *no;
    evento_t *copia;
    if((copia = malloc(sizeof(evento_t)))){
        copia->tempo = evento->tempo;
        copia->tipo = evento->tipo;
        copia->tamanho = evento->tamanho;
        copia->dados = malloc(evento->tamanho);
        memcpy(copia->dados, evento->dados, evento->tamanho);
    }else{
        return 0;
    }
    if((no = malloc(sizeof(nodo_lef_t)))){
        no->evento = copia; 
        no->prox = NULL;
    }else{
        return 0;
    }
    if(l->Primeiro == NULL){ 
        l->Primeiro = no;
    }else{
        no->prox = l->Primeiro;
        l->Primeiro = no;
    }   
    return 1;
}

/* se a lista estiver vazia ou se posição encontrada for a primeira, insere no inicio e tbm faz copia do evento*/
int adiciona_ordem_lef(lef_t *l, evento_t *evento){
    nodo_lef_t *no, *t_ant, *t_atual;
    evento_t *copia;
    if((copia = malloc(sizeof(evento_t)))){
        copia->tempo = evento->tempo;
        copia->tipo = evento->tipo;
        copia->tamanho = evento->tamanho;
        copia->dados = malloc(evento->tamanho);
        memcpy (copia->dados, evento->dados, evento->tamanho);
    }else 
        return 0;
    if(!(no = malloc (sizeof (nodo_lef_t)))){
        return 0;
    }
    t_atual = l->Primeiro;
    if(t_atual == NULL){
        free (copia->dados);
        free (copia);
        free (no);
        adiciona_inicio_lef (l, evento);
        return 1;
    }
    while (t_atual->prox != NULL && t_atual->evento->tempo < evento->tempo) {
        t_ant = t_atual;
        t_atual = t_atual->prox;
    }
    if (t_atual == l->Primeiro && t_atual->evento->tempo >= evento->tempo) { 
        adiciona_inicio_lef(l, evento);
        free (copia->dados); 
        free (copia); 
        free (no); 
        return 1;
    } else if(t_atual->prox == NULL){
        no->evento = copia;
        t_atual->prox = no;
        no->prox = NULL;
        return 1;
    } else { 
        no->evento = copia; 
        t_ant->prox = no; 
        no->prox = t_atual;
        return 1;
    }
}

/* o ponteiro do primeiro sera apontado para o prox da lista */
evento_t * obtem_primeiro_lef (lef_t *l){
    nodo_lef_t *no;
    evento_t *evento;    
    if (!l->Primeiro)
        return NULL;
    evento = l->Primeiro->evento; 
    no = l->Primeiro;
    l->Primeiro = l->Primeiro->prox;
    free(no);
    return evento;
}
