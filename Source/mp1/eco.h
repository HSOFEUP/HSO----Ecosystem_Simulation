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
    //int id;
} objeto;

typedef struct posicao_s
{
    objeto *pos_atual;
    objeto *pos_prox;
} posicao;

typedef struct ecosistema_s ecosistema;

typedef struct eco_thread_s
{
    coord ini;
    coord fim;
    coord *lista_limpeza_pos;
    int qtd_limpeza_pos;
    int tam_limpeza_pos;
    objeto **lista_limpeza_mem;
    int qtd_limpeza_mem;
    int tam_limpeza_mem;
    int novos;
    ecosistema *base;
} eco_thread;

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
    int qtd_threads;
    threadpool thpool;
    eco_thread *lista_eco_thread;
    pthread_mutex_t **lista_mutex;
    pthread_mutex_t mutex_eco;
} ecosistema;

int carregaDadosIniciais(char *nomeArq, ecosistema *eco);
void preparaThreads(ecosistema *eco);
void imprimeEcosistema(ecosistema *eco);
//void imprimeEcosistemaId(ecosistema *eco);
void imprimeSaida(ecosistema *eco);
int executaCiclo(ecosistema *eco);
void executeCicloThreadCoelhos(eco_thread *eco);
void executeCicloThreadRaposas(eco_thread *eco);
void processaObjeto(eco_thread *eco, int x, int y);
int verificaPossibilidades(eco_thread *eco, objeto *obj, tipo_objeto objetivo, coord *pos);
tipo_objeto tipoPos(eco_thread *eco, int x, int y);
void moveObjeto(eco_thread *eco, objeto *obj, int p, coord *pos);
int requerMutex(eco_thread *eco, int x, int y);
objeto* resolveConflito(eco_thread *eco, objeto *obj1, objeto *obj2);
void adicionaObjetoLimpezaPos(eco_thread *eco, int x, int y);
void limpaListaObjetosPos(eco_thread *eco);
void adicionaObjetoLimpezaMem(eco_thread *eco, objeto *obj);
void limpaListaObjetosMem(eco_thread *eco);
void limpezaGeral(ecosistema *eco);

#endif
