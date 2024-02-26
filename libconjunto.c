#include <stdio.h>
#include <stdlib.h>
#include "libconjunto.h"

void insere_ordenado(conjunto_t *c, int x);

int busca_binaria(conjunto_t *c, int x);

conjunto_t* cria_conjunto(int max){
    conjunto_t *c;
    if((c = malloc(sizeof(conjunto_t)))){
        c->max = max;
        c->card = 0;
        if ((c->v = malloc(sizeof(int)*(max+1))))
            return c;
        else{
            free(c);
            return NULL;
        }
    }else
        return NULL;
}

/*percorre todo o cojunto liberando seus elementos, depois libera o vetor e por fim o proprio c*/
conjunto_t* destroi_conjunto(conjunto_t *c){
    int aux;
    for(aux=0; aux < c->card; aux++){
        retira_conjunto(c, *(c->v+aux));
    }
    free (c->v);
    free (c);
    return NULL;
}

int conjunto_vazio(conjunto_t *c){
    return c->card == 0;
}

int cardinalidade(conjunto_t *c){
    return c->card;
}

/* insere o elemento no conjunto com as verificacoes necessaria*/
int insere_conjunto(conjunto_t *c, int elemento){
    if(!pertence(c,elemento)){
        /* retorna -1 no caso de falha por falta de espaco*/
        if(c->card + 1 > c->max)
            return -1;
        else{
           /*insere o elemento na posicao correta*/
           insere_ordenado(c, elemento);
           c->card++;
           return 1;
        }
    }else
        return 0;
}

/* retira elementos do conjunto mas mantem ordem*/
int retira_conjunto(conjunto_t *c, int elemento){
    int posicao, aux;
    if((posicao = busca_binaria (c, elemento)) != -1){
        posicao = posicao + 1;
        for(aux = posicao; aux <= c->card; aux++) 
            *(c->v+aux-1) = *(c->v+aux);
        c->card--;
        return 1;
    }else
        return 0;
}

/* retorna se o elemento pertence ao conjunto */
int pertence(conjunto_t *c, int elemento){
   /* retorna busca_binaria(elemento, c); */
    if(busca_binaria(c, elemento) == -1)
        return 0;
    return 1;
}

/* verifica se c1 esta contido no c2*/
int contido(conjunto_t *c1, conjunto_t *c2){
    int aux;
    if(c1->card > c2->card) 
        return 0;
    else{
        for(aux = 0; aux < c1->card; aux++){
            if(busca_binaria(c2,*(c1->v+aux)) == -1)
                return 0;
        }        
        return 1;
    }
}

/* verifica a igualdade de cada elemento da posição equivalente */
int sao_iguais(conjunto_t *c1, conjunto_t *c2){
    int aux;
    if(c1->card != c2->card)
        return 0;
    else{
        for(aux = 0; aux < c1->card; aux++){
            if(!(*(c1->v+aux) == *(c2->v+aux)))
                return 0;
        }        
        return 1;
    }
}

conjunto_t* cria_diferenca(conjunto_t *c1, conjunto_t *c2){
    conjunto_t *c3;
    int aux;
    if((c3 = cria_conjunto (c1->max)) != NULL){
        for(aux = 0; aux < c1->card; aux++) {
            if(busca_binaria(c2,*(c1->v+aux)) == -1)
             /* insere no conjunto de diferença se ele nao estiver c2*/
                insere_conjunto(c3, *(c1->v+aux));
        }
        return c3;
    }else
        return NULL;
}

conjunto_t* cria_interseccao(conjunto_t *c1, conjunto_t *c2){
    conjunto_t *minimo, *c3;
    int aux;
    if (c1->card < c2->card)
        minimo = c1;
    else
        minimo = c2;
    if((c3 = cria_conjunto(minimo->max)) != NULL){
        for(aux = 0; aux < c1->card; aux++) {
            if(busca_binaria (c2,*(c1->v+aux)) != -1)
             /* insere no conjunto interseccao se o elemento estiver no maior conjunto */ 
                insere_conjunto(c3, *(c1->v+aux));
            }
        return c3;
    }else
        return NULL;
}

conjunto_t* cria_uniao(conjunto_t *c1, conjunto_t *c2){
    conjunto_t *c3;
    int aux, max;
    /*max de c3 eh 2 vezes o max dos conjuntos */
    max = (c1->max)+(c2->max);
    if((c3 = cria_conjunto (max)) != NULL){
        /*cada for vai até o ultimo elemento de cada conjunto, o insere_conjunto faz verificacao necessaria */
        for(aux = 0; aux < c1->card; aux++) 
            insere_conjunto (c3, *(c1->v+aux));
        for(aux = 0; aux < c2->card; aux++) 
            insere_conjunto (c3, *(c2->v+aux));       
        return c3;
    }else 
        return NULL;
}

/* faz a copia do conjunto e o retorna*/
conjunto_t* cria_copia(conjunto_t *c){
    conjunto_t *c3;
    int aux;
    if((c3 = cria_conjunto(c->max)) != NULL){
        for(aux = 0; aux< c->card; aux++) {
            insere_conjunto(c3, *(c->v+aux));
        }
        return c3;
    }else 
        return 0;
}

/*se card do sub >= que de c retorna copia de c, senao cria sub aleatorio*/
conjunto_t* cria_subconjunto(conjunto_t *c, int n){
    conjunto_t *c3;
    if(n >= c->card) 
        return cria_copia(c);
    if((c3 = cria_conjunto(c->max)) != NULL){
        if(c->card == 0)
            return c3;
        while(c3->card < n)
            insere_conjunto(c3, *(c->v+(rand() % (c->card))));
        return c3;
    }else 
        return NULL;
}

void imprime(conjunto_t *c){
    int aux;
    for (aux = 0; aux < c->card; aux++){
        printf(" %d ", *(c->v+aux));
    }
    printf("\n");
}

int redimensiona(conjunto_t *c){
    int *aux;
    /* uso do realloc, pois vamos modificar o espaco de memoria deixado anteriormente*/
    if((aux = realloc(c->v, sizeof(int)*(c->max+1)*2))) {
        c->v = aux;
        c->max = c->max*2;
        return 1;
    }else
        return 0;
}

void iniciar_iterador(conjunto_t *c){
    c->ptr = 0;
}

int incrementar_iterador (conjunto_t *c, int *elemento) {
    /* ponteiro chegou no fim do conjunto */
    if(c->ptr == c->card) 
        return 0;
    c->ptr++;
    /* corrige o indice do ponteiro */
    *elemento = *(c->v+(c->ptr-1));
    return 1;
}

int retirar_um_elemento(conjunto_t *c){
    return retira_conjunto (c, *(c->v+(rand() % c->card)));
}

/* busca binaria necessaria para conseguir realizar varias operacoes nos conjuntos acima*/
int busca_binaria(conjunto_t *c, int x){
    int ini, fim, aux;
    ini = 0;
    fim = c->card;
    while(fim > ini){
        aux = (ini + fim)/2;
        if(*(c->v+aux) == x){
            return aux;
        } else if(*(c->v+aux) < x)
            ini = aux + 1;
        else
            fim = aux;
    }
    return -1;
}

void insere_ordenado(conjunto_t *c, int x){
    int aux;
    aux = c->card;
    if(c->card == 0) 
        *(c->v+c->card)=x;
    else{
        while(aux != 0 && x <= *(c->v+aux-1)){
            *(c->v+aux) = *(c->v+aux-1);
            aux--;
        }
        *(c->v+aux) = x;
    }
}

