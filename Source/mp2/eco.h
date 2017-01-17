#ifndef ECO_H
#define ECO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "thpool.h"

#define MAX_LINHA 1000

typedef enum {VAZIO = 0, PEDRA, COELHO, RAPOSA} tipo_objeto;

//estrutura alinhada
typedef struct coord_s
{
    int x;
    int y;
} coord;

//estrutura alinhada
typedef struct objeto_s
{
    tipo_objeto tipo;
    int x;
    int y;
    int gen;
    int proc;
    int comida;
    int id;
} objeto;

typedef struct posicao_s
{
    objeto *pos_atual;
    objeto *pos_prox;
} posicao;

//estrutura alinhada
typedef struct ecosistema_s
{
    int gen_proc_coelhos;
    int gen_proc_raposas;
    int gen_comida_raposas;
    int n_gen;
    int linhas;
    int colunas;
    int num_objetos;
    int n_gen_atual;
    posicao ***matriz;
    coord *lista_limpeza_pos;
    int qtd_limpeza_pos;
    int tam_limpeza_pos;
    objeto **lista_limpeza_mem;
    int qtd_limpeza_mem;
    int tam_limpeza_mem;
    int qtd_threads;
    threadpool thpool;
    pthread_mutex_t **lista_mutex;
    pthread_mutex_t mutex_eco;
} ecosistema;

int carregaDadosIniciais(char *nomeArq);
void preparaThreads();
void imprimeEcosistema();
void imprimeEcosistemaId();
void imprimeSaida();
int executaCiclo();
void processaObjeto(objeto *obj);
int verificaPossibilidades(objeto *obj, tipo_objeto objetivo, coord *pos);
tipo_objeto tipoPos(int x, int y);
void moveObjeto(objeto *obj, int p, coord *pos);
objeto* resolveConflito(objeto *obj1, objeto *obj2);
void adicionaObjetoLimpezaPos(int x, int y);
void limpaListaObjetosPos();
void adicionaObjetoLimpezaMem(objeto *obj);
void limpaListaObjetosMem();
void limpezaGeral();
int obtemId();

ecosistema eco;

#endif
