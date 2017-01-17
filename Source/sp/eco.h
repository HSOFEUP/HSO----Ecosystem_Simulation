#ifndef ECO_H
#define ECO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
} ecosistema;

int carregaDadosIniciais(char *nomeArq, ecosistema *eco);
void imprimeEcosistema(ecosistema *eco);
void imprimeEcosistemaId(ecosistema *eco);
void imprimeSaida(ecosistema *eco);
int executaCiclo(ecosistema *eco);
void processaObjeto(ecosistema *eco, int x, int y);
int verificaPossibilidades(ecosistema *eco, objeto *obj, tipo_objeto objetivo, coord *pos);
tipo_objeto tipoPos(ecosistema *eco, int x, int y);
void moveObjeto(ecosistema *eco, objeto *obj, int p, coord *pos);
objeto* resolveConflito(ecosistema *eco, objeto *obj1, objeto *obj2);
void adicionaObjetoLimpezaPos(ecosistema *eco, int x, int y);
void limpaListaObjetosPos(ecosistema *eco);
void adicionaObjetoLimpezaMem(ecosistema *eco, objeto *obj);
void limpaListaObjetosMem(ecosistema *eco);
void limpezaGeral(ecosistema *eco);
int obtemId();

#endif
