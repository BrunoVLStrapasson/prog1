#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include "libconjunto.h"
#include "libfila.h"
#include "liblef.h"

typedef struct Pessoa{
    int id; /* id da pessoa */
    int extroversao; /* valor de 0 a 100 que indica o grau de extroversao da pessoa */
    int paciencia; /* valor de 0 a 100 que indica o grau de paciencia da pessoa */
    int idade; /* valor de 18 a 100 que indica a idade da pessoa */
    conjunto_t *rumores; /* conjunto de rumores que a pessoa possui */
}pessoa;

typedef struct Coordenada{
    int x; 
    int y; 
}coordenada;

typedef struct Local{
    int id; /* id do local */
    int lot_max; /* numero max de pessoas para o local */
    conjunto_t *pessoas; /* pessoas que estao no lugar */
    fila_t *fila; /* fila de pessoas que querem entrar no lugar */
    coordenada localizacao; /* coordenadas do lugar */
}local;

typedef struct Mundo {
    int tempo_atual; /* tempo atual do mundo */
    pessoa pessoas[100]; /* vetor com todas as pessaos pertencentes ao mundo */
    local locais[8]; /* vetor que contem todos os locais */
    conjunto_t *rumores; /* vetor que armazena todos os rumores do mundo */
    int n_pessoas; /* variavel com o numero de pessoas do mundo */
    int n_locais; /* variavel com o numero de locais do mundo */
    coordenada tam_mundo; /* coordenadas maximas do mundo */
}mundo;

typedef struct dados_chegada{
    int id_pessoa;
    int id_local;
}dados_chegada_m; 

/* struct com os dados do evento de saida*/
typedef struct dados_saida {
    int id_pessoa;
    int id_local;
} dados_saida_m;

/* struct com os dados do evento de disceminacao */
typedef struct dados_disseminacao {
    conjunto_t *cj_rumores;
    int id_pessoa_origem;
    int id_local;
} dados_disseminacao_m;

/*header da func cria_rumor_mundo*/
void cria_rumores_mundo(conjunto_t *rumores);

/*header da func cria_locais_mundo*/
void cria_locais_mundo (local locais[]);

/*header da func cria_pessoa_mundo*/
void cria_pessoas_mundo (pessoa pessoas[], local locais[], conjunto_t rumores[], lef_t *eventos);

/*header func cria_evento*/
void cria_evento_de_fim (lef_t *eventos);

mundo cria_mundo(lef_t *eventos){
    mundo mundo;
    mundo.tempo_atual = 0; 
    mundo.n_pessoas = 100; 
    mundo.n_locais = 8; 
    mundo.tam_mundo.x = 20000; 
    mundo.tam_mundo.y = 20000; 
    mundo.rumores = cria_conjunto (30);
    cria_rumores_mundo (mundo.rumores); 
    cria_locais_mundo (mundo.locais); 
    cria_pessoas_mundo (mundo.pessoas, mundo.locais, mundo.rumores, eventos); 
    cria_evento_de_fim (eventos);
    return mundo;
}

/* funcao para criar eventos de chegada */
void cria_evento_de_chegada(pessoa pessoa, local local, lef_t *eventos, int tempo){
    evento_t evento; 
    dados_chegada_m chegada_pessoa; 

    chegada_pessoa.id_pessoa = pessoa.id; 
    chegada_pessoa.id_local = local.id;   
    evento.tempo = tempo;
    evento.tipo = 1;
    evento.tamanho = sizeof(dados_chegada_m);
    evento.dados = &chegada_pessoa;   
    adiciona_ordem_lef (eventos, &evento); /*adiciona o evento na lef*/
}

/* funcao para criar eventos de saida */
void cria_evento_de_saida(pessoa pessoa, local local, lef_t *eventos, int tempo){
    evento_t evento; 
    dados_saida_m saida_pessoa; 
    saida_pessoa.id_pessoa = pessoa.id; 
    saida_pessoa.id_local = local.id;     
    evento.tempo = tempo;
    evento.tipo = 2;
    evento.tamanho = sizeof(dados_saida_m);
    evento.dados = &saida_pessoa;  
    adiciona_ordem_lef (eventos, &evento); /*adiciona o evento na lef*/
}

/* funcao para criar eventos de disceminacao */
void cria_evento_de_disseminacao(pessoa pessoa, local local, lef_t *eventos, int tempo){
    evento_t evento; 
    dados_disseminacao_m disseminacao_pessoa; 
    int nrd = pessoa.extroversao/10;
    disseminacao_pessoa.id_pessoa_origem = pessoa.id; 
    disseminacao_pessoa.id_local = local.id; 
    disseminacao_pessoa.cj_rumores = cria_subconjunto (pessoa.rumores, nrd);    
    evento.tempo = tempo;
    evento.tipo = 3;
    evento.tamanho = sizeof(dados_disseminacao_m);
    evento.dados = &disseminacao_pessoa;  
    adiciona_ordem_lef (eventos, &evento); /*adiciona o evento na lef*/
} 

/* funcao para criar eventos de fim */
void cria_evento_de_fim(lef_t *eventos){
    evento_t evento; 
    evento.tempo = 34944;
    evento.tipo = 4;
    evento.tamanho = 0;
    evento.dados = NULL;
    adiciona_ordem_lef (eventos, &evento); /*adiciona o evento na lef*/
}

/* funcao que gera um numero aleatorio entre dois limites */
int ALEAT(int m, int M){
    return m + (rand() % M); 
}

int MAX(int min, int max){
    if (max > min)
        return max;
    return 1; 
}
/* cria vetor de rumores e o retorna em rumores e uma seguencia ordenada de rumores com id 1 a 30*/
void cria_rumores_mundo(conjunto_t *rumores){
    int aux;
    for (aux = 1; aux <= 30; aux++) 
        insere_conjunto (rumores, aux);
}

/* cria os locais que vão ser usados na simulacao e gera os locais do mundo */
void cria_locais_mundo(local locais[]){
    local loc;
    int aux;
    for (aux = 1; aux <= 8; aux++) {
        loc.id = aux;
        loc.lot_max = ALEAT(5, 30); 
        loc.localizacao.x = ALEAT(0, 20000-1); 
        loc.localizacao.y = ALEAT(0, 20000-1); 
        loc.pessoas = cria_conjunto(loc.lot_max); 
        loc.fila = cria_fila();        
        locais[aux-1] = loc;
    }
}

/*cria as pessoas que participaram do mundo e os inicia com os eventos de chegada no mundo*/
void cria_pessoas_mundo (pessoa pessoas[], local locais[], conjunto_t rumores[], lef_t *eventos) {
    pessoa pessoa;
    int aux;
    for (aux = 1; aux <= 100; aux++) {
        pessoa.id = aux;
        pessoa.extroversao = ALEAT(0, 100);
        pessoa.paciencia = ALEAT(0, 100);
        pessoa.idade = ALEAT(18, 100);
        pessoa.rumores = cria_subconjunto (rumores, ALEAT(1, 5));        
        pessoas[aux-1] = pessoa;
        cria_evento_de_chegada (pessoa, locais[ALEAT(1, 8)-1], eventos, ALEAT(0, 96*7));
    }
}

/* tratamento para a saida do evento de chegada */
void trata_saida_evento_chegada (mundo mundo, pessoa pessoa, local local, int caso){
    switch (caso) {
        case 1: { /* entrada */
            printf ("%6d:CHEGA Pessoa %4d Local %2d (%2d/%2d), ENTRA\n", mundo.tempo_atual, pessoa.id, local.id, cardinalidade (local.pessoas), local.lot_max);
            break;
        }
        case 2: { /*fica na fila */
            printf ("%6d:CHEGA Pessoa %4d Local %2d (%2d/%2d), FILA %2d\n", mundo.tempo_atual, pessoa.id, local.id, cardinalidade (local.pessoas), local.lot_max, tamanho_fila(local.fila));
            break;
        }
        case 3: { /* desistiu */
            printf ("%6d:CHEGA Pessoa %4d Local %2d (%2d/%2d), DESISTE\n", mundo.tempo_atual, pessoa.id, local.id, cardinalidade (local.pessoas), local.lot_max);
            break;
        }
    }
}

/* tratamento para a saida do evento de saida */
void trata_saida_evento_saida (mundo mundo, pessoa pessoa_saida, int pessoa_fila, local local, int caso) {
    switch (caso) {
        case 1: { /* sai sem liberar espaço na fila */
            printf ("%6d:SAIDA Pessoa %4d Local %2d (%2d/%2d)\n", mundo.tempo_atual, pessoa_saida.id, local.id, cardinalidade (local.pessoas), local.lot_max);
            break;
        }
        case 2: { /* libera lugar na fila */
            printf ("%6d:SAIDA Pessoa %4d Local %2d (%2d/%2d), REMOVE FILA %2d\n", mundo.tempo_atual, pessoa_saida.id, local.id, cardinalidade (local.pessoas), local.lot_max, pessoa_fila);
            break;
        }
    }
}

void evento_chegada (mundo mundo, lef_t *eventos, int id_pessoa, int id_local){
    local local = mundo.locais[id_local-1]; /* variavel para o local */
    pessoa pessoa = mundo.pessoas[id_pessoa-1]; /* variavel para a pessoa */
    int tpl; 
    /*avalia se deve ir pra fila ou nao*/
    if(cardinalidade (local.pessoas) == local.lot_max){
        if((pessoa.paciencia / 4 - tamanho_fila (local.fila)) > 0){
            insere_fila (local.fila, id_pessoa);
            trata_saida_evento_chegada (mundo, pessoa, local, 2); 
        }
        else{ 
            cria_evento_de_saida (pessoa, local, eventos, mundo.tempo_atual);
            trata_saida_evento_chegada (mundo, pessoa, local, 3);
        }
    } else{ 
        insere_conjunto (local.pessoas, id_pessoa); 
        trata_saida_evento_chegada (mundo, pessoa, local, 1);
        tpl = MAX(1,pessoa.paciencia/10+ALEAT(-2, 6)); 
        cria_evento_de_disseminacao (pessoa, local, eventos, mundo.tempo_atual+ALEAT(0, tpl));
        cria_evento_de_saida (pessoa, local, eventos, mundo.tempo_atual+tpl);
    }
}

void evento_saida (mundo mundo, lef_t *eventos, int id_pessoa, int id_local) {
    local loc = mundo.locais[id_local-1]; /*variavel para o local*/
    pessoa pessoa = mundo.pessoas[id_pessoa-1]; /*variavel para a pessoa*/
    int pessoa_fila; /*possivel pessoa que a sair da fila*/
    local local_dest; /*vai receber o local de destino da pessoa*/
    int tdl, dist_cart, velocidade; 

    local_dest = mundo.locais[ALEAT(1, 8)-1];
    dist_cart = sqrt(pow (local_dest.localizacao.x - loc.localizacao.x, 2) + pow (local_dest.localizacao.y - loc.localizacao.y, 2));
    velocidade = 100 - MAX(1,pessoa.idade-40);
    tdl = dist_cart/velocidade;
    retira_conjunto (loc.pessoas, pessoa.id);
    cria_evento_de_chegada (pessoa, local_dest, eventos, mundo.tempo_atual+(tdl/15));
    if (!fila_vazia (loc.fila)){
        retira_fila (loc.fila, &pessoa_fila);
        cria_evento_de_chegada (mundo.pessoas[pessoa_fila-1], loc, eventos, mundo.tempo_atual);
        trata_saida_evento_saida (mundo, pessoa, pessoa_fila, loc, 2);
        return;
    }
    trata_saida_evento_saida(mundo, pessoa, -1, loc, 1);
}

/*faz o tratamento de retorno na propia funcao, devido a forma que este evento funciona */
void evento_disseminacao (mundo mundo, int id_pessoa, int id_local, conjunto_t *cj_rumores) {
    local loc = mundo.locais[id_local-1]; /* variavel para o local */
    pessoa pessoa_origem = mundo.pessoas[id_pessoa-1]; /* variavel para a pessoa de origem dos rumores */
    int id_pessoa_destino;
    int iterador_rumor, iterador_pessoa, rumor;
    printf ("%6d:RUMOR Pessoa %4d Local %2d ", mundo.tempo_atual, pessoa_origem.id, loc.id);
    /*nrd = mundo.pessoas[id_pessoa].extroversao/10;*/
    iniciar_iterador (cj_rumores);
    for(iterador_rumor = 0; iterador_rumor < cardinalidade(cj_rumores); iterador_rumor++){
        incrementar_iterador (cj_rumores, &rumor); 
        iniciar_iterador (loc.pessoas); 
        for(iterador_pessoa = 0; iterador_pessoa < cardinalidade (loc.pessoas); iterador_pessoa++){
            incrementar_iterador (loc.pessoas, &id_pessoa_destino);
            if(ALEAT(0, 100) < mundo.pessoas[id_pessoa_destino-1].extroversao)  
                if(insere_conjunto (mundo.pessoas[id_pessoa_destino-1].rumores, rumor))
                    printf("(P%d:R%d)  ", mundo.pessoas[id_pessoa_destino-1].id, rumor);
        }
    }
    printf ("\n");
}

/* evento do fim do mundo */
void evento_fim (mundo mundo) {
    int aux;
    for (aux = 0; aux < mundo.n_pessoas; aux++) 
        destroi_conjunto (mundo.pessoas[aux].rumores);
    for (aux = 0; aux < mundo.n_locais; aux++) {
        destroi_conjunto (mundo.locais[aux].pessoas);
        destroi_fila (mundo.locais[aux].fila);
    }
    destroi_conjunto (mundo.rumores);
}

/* programa principal */
int main(){   
    mundo mundo; /* var mundo criado para a simulacao */
    lef_t *eventos; /* lef de eventos */
    evento_t *evento_atual; /* o evento atual que esta ocorrendo*/
    dados_chegada_m *evento_dados_chegada; /* ptr que recebe os dados de chegada */
    dados_saida_m *evento_dados_saida; /* ptr que recebe os dados de saida */
    dados_disseminacao_m *evento_dados_disseminacao; /* ptr que recebe os dados de disseminacao */
    int fim_mundo = 0; /* flag para o fim */

    /*unicia a lista de eventos futuros e inicializa o mundo da simulacao */
    eventos = cria_lef ();
    mundo = cria_mundo (eventos);
    /*escolhe dados correto do evento, enquanto o fim do mundo ja nao foi chamado */
    while ((evento_atual = obtem_primeiro_lef(eventos)) != NULL){
        /* atualiza o tempo do mundo */
        mundo.tempo_atual = (evento_atual->tempo) ;
        switch (evento_atual->tipo) {
            case 1: /*CHEGADA*/ {
                evento_dados_chegada =(dados_chegada_m*)evento_atual->dados;/*carrega dados de chegada*/
                if (fim_mundo == 0){
                    evento_chegada (mundo, eventos, evento_dados_chegada->id_pessoa,evento_dados_chegada->id_local);
                }
                free (evento_dados_chegada);
                free (evento_atual);
                break;
            }
            case 2: /*SAIDA*/{ 
                evento_dados_saida = (dados_saida_m*)evento_atual->dados;/*carrega dados de saida*/
                if (fim_mundo == 0){
                    evento_saida (mundo, eventos, evento_dados_saida->id_pessoa,evento_dados_saida->id_local);
                }
                free (evento_dados_saida);
                free (evento_atual);
                break;
            }
            case 3: /*DISSEMINACAO*/{
                evento_dados_disseminacao = (dados_disseminacao_m*)evento_atual->dados;/*carrega os dados de disseminacao*/
                if (fim_mundo == 0){
                    evento_disseminacao(mundo, evento_dados_disseminacao->id_pessoa_origem, evento_dados_disseminacao->id_local, evento_dados_disseminacao->cj_rumores);
                }
                destroi_conjunto(evento_dados_disseminacao->cj_rumores);
                free (evento_dados_disseminacao);
                free (evento_atual);
                break;
            }
            case 4: /*FIM*/{
                free (evento_atual->dados);
                free (evento_atual);
                fim_mundo = 1;
                evento_fim(mundo);
                break;
            }
        }
    }
    destroi_lef(eventos); /*libera a LEF*/
    return 1;
}