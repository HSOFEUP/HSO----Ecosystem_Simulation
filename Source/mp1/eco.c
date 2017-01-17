#include "eco.h"

int carregaDadosIniciais(char *nomeArq, ecosistema *eco)
{
    FILE *arq = fopen(nomeArq, "rt");
    int x, y, pos = 0;
    char buffer[MAX_LINHA];
    char *linha = buffer;
    char *aux;
    int num;
    tipo_objeto tipo;

    if (arq == NULL) {printf("Arquivo %s nao existe.\n", nomeArq); exit(0);}

    //le os dados iniciais do ecosistema
    fgets(linha, MAX_LINHA, arq);
    linha[strcspn(linha, "\n")] = 0;

    while ((aux = strsep(&linha, " ")))
    {
        num = atoi(aux);
        switch (pos++)
        {
            case 0: eco->gen_proc_coelhos = num; break;
            case 1: eco->gen_proc_raposas = num; break;
            case 2: eco->gen_comida_raposas = num; break;
            case 3: eco->n_gen = num; break;
            case 4: eco->linhas = num; break;
            case 5: eco->colunas = num; break;
            case 6: eco->num_objetos = num; break;
        }
    }

    eco->n_gen_atual = 0;

    //aloca a matriz
    eco->matriz = (posicao ***) malloc(eco->linhas * sizeof(posicao **));

    if (eco->matriz == 0) {return 0;}

    for (x = 0; x < eco->linhas; x++)
    {
        eco->matriz[x] = (posicao **) calloc(eco->colunas, sizeof(posicao *));
        if (eco->matriz[x] == 0) {return 0;}
        for (y = 0; y < eco->colunas; y++)
        {
            eco->matriz[x][y] = calloc(1, sizeof(posicao));
            if (eco->matriz[x][y] == 0) {return 0;}
        }
    }

    //le os dados dos objetos
    while (fgets(buffer, MAX_LINHA, arq) != NULL)
    {
        linha = buffer;
        linha[strcspn(linha, "\n")] = 0;
        pos = 0;

        while ((aux = strsep(&linha, " ")))
        {
            if (pos == 0)
            {
                if (strcmp(aux, "ROCK") == 0) {tipo = PEDRA;}
                else if (strcmp(aux, "RABBIT") == 0) {tipo = COELHO;}
                else if (strcmp(aux, "FOX") == 0) {tipo = RAPOSA;}
            }
            else if (pos == 1) { x = atoi(aux);}
            else if (pos == 2) { y = atoi(aux);}

            pos++;
        }

        eco->matriz[x][y]->pos_atual = calloc(1, sizeof(objeto));
        eco->matriz[x][y]->pos_atual->tipo = tipo;
        eco->matriz[x][y]->pos_atual->x = x;
        eco->matriz[x][y]->pos_atual->y = y;
    }

    preparaThreads(eco);

    return 1;
}

void imprimeEcosistema(ecosistema *eco)
{
    int x, y, col_espaco;

    if (eco == 0) {return;}

    //col_espaco = eco->colunas * 2 + 1;
    col_espaco = eco->colunas + 2;

    printf("Generation %d\n", eco->n_gen_atual);

    for (y = 0; y < col_espaco; y++) {printf("-");} printf("\n");

    for (x = 0; x < eco->linhas; x++)
    {
        printf("|");
        for (y = 0; y < eco->colunas; y++)
        {
            if (eco->matriz[x][y]->pos_atual)
            {
                switch (eco->matriz[x][y]->pos_atual->tipo)
                {
                    case PEDRA: printf("*"); break;
                    case COELHO: printf("R"); break;
                    case RAPOSA: printf("F"); break;
                }
            }
            else //VAZIO
            {
                printf(" ");
            }
            //if (y < eco->colunas-1) {printf(" ");}
        }
        printf("|\n");
    }

    for (y = 0; y < col_espaco; y++) {printf("-");} printf("\n");

    if (eco->n_gen_atual != eco->n_gen) {printf("\n");}
}

void imprimeSaida(ecosistema *eco)
{
    int x, y;

    if (eco == 0) {return;}

    printf("%d %d %d %d %d %d %d\n", eco->gen_proc_coelhos, eco->gen_proc_raposas, eco->gen_comida_raposas, eco->n_gen_atual - eco->n_gen, eco->linhas, eco->colunas, eco->num_objetos);

    for (x = 0; x < eco->linhas; x++)
    {
        for (y = 0; y < eco->colunas; y++)
        {
            if (eco->matriz[x][y]->pos_atual)
            {
                switch (eco->matriz[x][y]->pos_atual->tipo)
                {
                    case PEDRA: printf("ROCK %d %d\n", x, y); break;
                    case COELHO: printf("RABBIT %d %d\n", x, y); break;
                    case RAPOSA: printf("FOX %d %d\n", x, y); break;
                }
            }
        }
    }
}

//prepara a estrutura de dados utilizada por cada thread a ser instanciada
void preparaThreads(ecosistema *eco)
{
    int x, y;
    int trab = eco->linhas / eco->qtd_threads;
    int mod_trab = eco->linhas % eco->qtd_threads;

    eco->thpool = thpool_init(eco->qtd_threads);

    eco->lista_eco_thread = (eco_thread *)malloc(sizeof(eco_thread) * eco->qtd_threads);

    //TODO: deixar a distribuicao da matriz entre as threads mais inteligente (verificando o melhor caso de acordo com o formato da matriz)
    //distribui um bloco de linhas da matriz principal para cada thread
    for (x = 0; x < eco->qtd_threads; x++)
    {
        eco->lista_eco_thread[x].ini.x = x?eco->lista_eco_thread[x-1].fim.x:0; //inicio eh o ponto depois do fim da anterior
        eco->lista_eco_thread[x].ini.y = 0;
        eco->lista_eco_thread[x].fim.x = eco->lista_eco_thread[x].ini.x + trab + (mod_trab?1:0); //distribui o resto da divisao entre as threads iniciais
        eco->lista_eco_thread[x].fim.y = eco->colunas;
        if (mod_trab) {mod_trab--;}

        eco->lista_eco_thread[x].tam_limpeza_pos = eco->num_objetos / eco->qtd_threads * 2;
        eco->lista_eco_thread[x].qtd_limpeza_pos = 0;
        eco->lista_eco_thread[x].lista_limpeza_pos = (coord *) malloc(sizeof(coord) * eco->lista_eco_thread[x].tam_limpeza_pos);

        eco->lista_eco_thread[x].tam_limpeza_mem = eco->lista_eco_thread[x].tam_limpeza_pos;
        eco->lista_eco_thread[x].qtd_limpeza_mem = 0;
        eco->lista_eco_thread[x].lista_limpeza_mem = (objeto **) malloc(sizeof(objeto *) * eco->lista_eco_thread[x].tam_limpeza_mem);

        eco->lista_eco_thread[x].novos = 0;

        eco->lista_eco_thread[x].base = eco;
    }

    pthread_mutex_init(&(eco->mutex_eco), NULL);

    eco->lista_mutex = (pthread_mutex_t **) malloc(eco->linhas * sizeof(pthread_mutex_t *));

    for (x = 0; x < eco->linhas; x++)
    {
        eco->lista_mutex[x] = (pthread_mutex_t *) malloc(eco->colunas * sizeof(pthread_mutex_t));
        for (y = 0; y < eco->linhas; y++)
        {
            pthread_mutex_init(&(eco->lista_mutex[x][y]), NULL);
        }
    }
}

//executa o ciclo global - usa um pool de threads para dividir as atividades
int executaCiclo(ecosistema *eco)
{
    int x;

    eco->n_gen_atual++;

    //usa um pool de threads para reduzir o custo de instanciar/remover thread
    for (x = 0; x < eco->qtd_threads; x++)
    {
        thpool_add_work(eco->thpool, (void *)executeCicloThreadCoelhos, (void *)&(eco->lista_eco_thread[x]));
    }

    thpool_wait(eco->thpool);

    //como a limpeza de posicao pode inteferir na decisao de movimentacao tem que ser feito depois da movimentacao
    for (x = 0; x < eco->qtd_threads; x++)
    {
        thpool_add_work(eco->thpool, (void *)limpaListaObjetosPos, (void *)&(eco->lista_eco_thread[x]));
    }

    thpool_wait(eco->thpool);

    for (x = 0; x < eco->qtd_threads; x++)
    {
        thpool_add_work(eco->thpool, (void *)executeCicloThreadRaposas, (void *)&(eco->lista_eco_thread[x]));
    }

    thpool_wait(eco->thpool);

    //como a limpeza de posicao pode inteferir na decisao de movimentacao tem que ser feito depois da movimentacao
    //a limpeza de memoria jah nao interfere em nada
    for (x = 0; x < eco->qtd_threads; x++)
    {
        thpool_add_work(eco->thpool, (void *)limpaListaObjetosPos, (void *)&(eco->lista_eco_thread[x]));
        thpool_add_work(eco->thpool, (void *)limpaListaObjetosMem, (void *)&(eco->lista_eco_thread[x]));
    }

    thpool_wait(eco->thpool);

    return 0;
}

void executeCicloThreadCoelhos(eco_thread *eco)
{
    int x, y;

    for (x = eco->ini.x; x < eco->fim.x; x++)
    {
        for (y = eco->ini.y; y < eco->fim.y; y++)
        {
            //so move se existir um objeto naquela posicao
            if (eco->base->matriz[x][y]->pos_atual != 0 && eco->base->matriz[x][y]->pos_atual->tipo == COELHO) {processaObjeto(eco, x, y);}
        }
    }
}

void executeCicloThreadRaposas(eco_thread *eco)
{
    int x, y;

    for (x = eco->ini.x; x < eco->fim.x; x++)
    {
        for (y = eco->ini.y; y < eco->fim.y; y++)
        {
            //so move se exitir um objeto naquela posicao
            if (eco->base->matriz[x][y]->pos_atual != 0 && eco->base->matriz[x][y]->pos_atual->tipo == RAPOSA) {processaObjeto(eco, x, y);}
        }
    }
}

void processaObjeto(eco_thread *eco, int x, int y)
{
    int p;
    coord pos[4];
    objeto *obj;

    memset(&pos, 0, sizeof(coord) * 4);

    obj = eco->base->matriz[x][y]->pos_atual;

    if (obj->tipo == COELHO)
    {
        p = verificaPossibilidades(eco, obj, VAZIO, pos);
        moveObjeto(eco, obj, p, pos);
    }
    else //RAPOSA
    {
        p = verificaPossibilidades(eco, obj, COELHO, pos);
        if (p != -1)
        {
            moveObjeto(eco, obj, p, pos);
        }
        else
        {
            obj->comida++;
            p = verificaPossibilidades(eco, obj, VAZIO, pos);
            moveObjeto(eco, obj, p, pos);
        }
    }
}

//retorna a qtd de possibilidades encontradas
int verificaPossibilidades(eco_thread *eco, objeto *obj, tipo_objeto objetivo, coord *pos)
{
    int resul = 0;

    if (obj->x != 0 && tipoPos(eco, obj->x-1, obj->y) == objetivo) //verifica se pode ir para o norte
    {
        pos[resul].x = obj->x - 1; pos[resul].y = obj->y; ; resul++;
    }
    if (obj->y != eco->base->colunas-1 && tipoPos(eco, obj->x, obj->y+1) == objetivo) //verifica se pode ir para o leste
    {
        pos[resul].x = obj->x; pos[resul].y = obj->y + 1; ; resul++;
    }
    if (obj->x != eco->base->linhas-1 && tipoPos(eco, obj->x+1, obj->y) == objetivo) //verifica se pode ir para o sul
    {
        pos[resul].x = obj->x + 1; pos[resul].y = obj->y; ; resul++;
    }
    if (obj->y != 0 && tipoPos(eco, obj->x, obj->y-1) == objetivo) //verifica se pode ir para o oeste
    {
        pos[resul].x = obj->x; pos[resul].y = obj->y - 1; ; resul++;
    }

    return resul-1;
}

inline tipo_objeto tipoPos(eco_thread *eco, int x, int y)
{
    //trata o caso do vazio que na verdade eh um ponteiro zerado
    return eco->base->matriz[x][y]->pos_atual == 0?VAZIO:eco->base->matriz[x][y]->pos_atual->tipo;
}

void moveObjeto(eco_thread *eco, objeto *obj, int p, coord *pos)
{
    if (p > 0) {p = ((eco->base->n_gen_atual-1) + obj->x + obj->y) % (p+1);}

    //verifica se este objeto esta nas extremidades das linhas entre threads e com isso requer mutex para suas operacoes
    int requer_mutex = requerMutex(eco, obj->x, obj->y);
    pthread_mutex_t *mutex = &(eco->base->lista_mutex[obj->x][obj->y]);

    adicionaObjetoLimpezaPos(eco, obj->x, obj->y);

    if (p != -1)
    {
        int requer_mutex_pos = requerMutex(eco, pos[p].x, pos[p].y);
        pthread_mutex_t *mutex_pos = &(eco->base->lista_mutex[pos[p].x][pos[p].y]);

        //a raposa morre se passar fome por muito tempo (o coelho tem sempre o valor 0 em comida portanto nunca entra neste if)
        if (obj->comida == eco->base->gen_comida_raposas)
        {
            //se nessa ultima rodada nao comer um coelho ela morre (nao requex mutex pois so le a estrutura atual - que nunca eh alterada durante o sub ciclo)
            if (!(eco->base->matriz[pos[p].x][pos[p].y]->pos_atual && eco->base->matriz[pos[p].x][pos[p].y]->pos_atual->tipo == COELHO))
            {
                adicionaObjetoLimpezaMem(eco, obj);
                return;
            }
        }

        //procria se for a hora
        if ((obj->tipo == COELHO && obj->proc >= eco->base->gen_proc_coelhos) || (obj->tipo == RAPOSA && obj->proc >= eco->base->gen_proc_raposas))
        {
            if (requer_mutex) {pthread_mutex_lock(mutex);}
            objeto *filho = malloc(sizeof(objeto));
            filho->tipo = obj->tipo;
            filho->x = obj->x;
            filho->y = obj->y;
            filho->gen = filho->proc = -1;
            filho->comida = 0;

            //pode haver um conflito se jah ouver algo no prox aqui. Sera a causa da diff entre o 100x100 e o 200x200? - Feito e nao resolveu nada.
            if (eco->base->matriz[obj->x][obj->y]->pos_prox == 0) {eco->base->matriz[obj->x][obj->y]->pos_prox = filho;}
            else {eco->base->matriz[obj->x][obj->y]->pos_prox = resolveConflito(eco, eco->base->matriz[obj->x][obj->y]->pos_prox, filho);}

            eco->novos++;
            obj->proc = -1;
            if (requer_mutex) {pthread_mutex_unlock(mutex);}
        }

        if (requer_mutex_pos) {pthread_mutex_lock(mutex_pos);}

        obj->x = pos[p].x;
        obj->y = pos[p].y;

        if (eco->base->matriz[obj->x][obj->y]->pos_prox == 0) {adicionaObjetoLimpezaPos(eco, obj->x, obj->y);}

        //nao ha nada neste local nem esta prevista para haver
        if (eco->base->matriz[obj->x][obj->y]->pos_atual == 0 && eco->base->matriz[obj->x][obj->y]->pos_prox == 0)
        {
            eco->base->matriz[obj->x][obj->y]->pos_prox = obj;
        }
        else
        {
            //tem algo no atual mas nada previsto para haver na proxima rodada
            if (eco->base->matriz[obj->x][obj->y]->pos_atual && eco->base->matriz[obj->x][obj->y]->pos_prox == 0)
            {
                //do mesmo tipo nao a conflito pois os objetos ainda estao movendo
                if (eco->base->matriz[obj->x][obj->y]->pos_atual->tipo == obj->tipo)
                {
                    eco->base->matriz[obj->x][obj->y]->pos_prox = obj;
                }
                //tipos diferentes - resolve o conflito
                else
                {
                    eco->base->matriz[obj->x][obj->y]->pos_prox = resolveConflito(eco, eco->base->matriz[obj->x][obj->y]->pos_atual, obj);
                }
            }
            //jah tem algo previsto para a proxima rodada
            else
            {
                eco->base->matriz[obj->x][obj->y]->pos_prox = resolveConflito(eco, eco->base->matriz[obj->x][obj->y]->pos_prox, obj);
            }

            //eco->base->num_objetos--;
        }

        if (requer_mutex_pos) {pthread_mutex_unlock(mutex_pos);}
    }
    else //quando o objeto permanece na mesma posicao
    {
        if (obj->comida == eco->base->gen_comida_raposas)
        {
            adicionaObjetoLimpezaMem(eco, obj);
        }
        else
        {
            if (requer_mutex) {pthread_mutex_lock(mutex);}
            //fica no mesmo lugar se for possivel, do contrario resolve o conflito
            if (eco->base->matriz[obj->x][obj->y]->pos_prox == 0)
            {
                eco->base->matriz[obj->x][obj->y]->pos_prox = obj;
            }
            else
            {
                eco->base->matriz[obj->x][obj->y]->pos_prox = resolveConflito(eco, eco->base->matriz[obj->x][obj->y]->pos_prox, obj);
            }
            if (requer_mutex) {pthread_mutex_unlock(mutex);}
        }
    }
}

inline int requerMutex(eco_thread *eco, int x, int y)
{
    if (x <= eco->ini.x || x >= (eco->fim.x-1))
    {
        //casa a thread soh esteja processando uma linha tem que ter mutex em todas
        if (x == 0) {return eco->fim.x==1;}
        else if (x == eco->base->linhas-1) {return (eco->ini.x==x || eco->base->linhas/eco->base->qtd_threads==1);}
        else {return 1;}
    }

    return 0;
}

objeto* resolveConflito(eco_thread *eco, objeto *obj1, objeto *obj2)
{
    if (obj1->tipo == COELHO && obj2->tipo == RAPOSA)
    {
        obj2->comida = 0;
        adicionaObjetoLimpezaMem(eco, obj1);
        return obj2;
    }
    else if (obj1->tipo == RAPOSA && obj2->tipo == COELHO)
    {
        obj1->comida = 0;
        adicionaObjetoLimpezaMem(eco, obj2);
        return obj1;
    }
    else //RAPOSA e RAPOSA ou COELHO e COELHO - sobrevive quem tem o maior proc
    {
        if (obj1->proc > obj2->proc)
        {
            adicionaObjetoLimpezaMem(eco, obj2);
            return obj1;
        }
        else if (obj1->proc < obj2->proc)
        {
            adicionaObjetoLimpezaMem(eco, obj1);
            return obj2;
        }
        else //idade proc igual
        {
            if (obj1->comida > obj2->comida) //sobrevive a raposa menos faminta - para o coelho isso nao importa, o seu valor de comida eh sempre 0
            {
                adicionaObjetoLimpezaMem(eco, obj1);
                return obj2;
            }
            else //aplica para o caso do coelho tb
            {
                adicionaObjetoLimpezaMem(eco, obj2);
                return obj1;
            }
        }
    }
}

//adiciona objeto a lista de limpeza de posicao (para limpar no final do ciclo)
void adicionaObjetoLimpezaPos(eco_thread *eco, int x, int y)
{
    //realoca a lista de limpeza se necessario
    if (eco->qtd_limpeza_pos == eco->tam_limpeza_pos)
    {
        eco->tam_limpeza_pos *= 2;
        coord *lista_tmp = realloc(eco->lista_limpeza_pos, sizeof(coord) * eco->tam_limpeza_pos);
        eco->lista_limpeza_pos = lista_tmp;
    }

    eco->lista_limpeza_pos[eco->qtd_limpeza_pos].x = x;
    eco->lista_limpeza_pos[eco->qtd_limpeza_pos].y = y;

    eco->qtd_limpeza_pos++;
}

//limpa os ponteiros da matriz atual
void limpaListaObjetosPos(eco_thread *eco)
{
    int i;

    for (i = 0; i < eco->qtd_limpeza_pos; i++)
    {
        eco->base->matriz[eco->lista_limpeza_pos[i].x][eco->lista_limpeza_pos[i].y]->pos_atual = eco->base->matriz[eco->lista_limpeza_pos[i].x][eco->lista_limpeza_pos[i].y]->pos_prox;
        eco->base->matriz[eco->lista_limpeza_pos[i].x][eco->lista_limpeza_pos[i].y]->pos_prox = 0;

        if (eco->base->matriz[eco->lista_limpeza_pos[i].x][eco->lista_limpeza_pos[i].y]->pos_atual)
        {
            eco->base->matriz[eco->lista_limpeza_pos[i].x][eco->lista_limpeza_pos[i].y]->pos_atual->proc++;
            eco->base->matriz[eco->lista_limpeza_pos[i].x][eco->lista_limpeza_pos[i].y]->pos_atual->gen++;
        }
    }

    eco->qtd_limpeza_pos = 0;
}

//adiciona objeto a lista de limpeza de memoria (para limpar no final do ciclo)
void adicionaObjetoLimpezaMem(eco_thread *eco, objeto *obj)
{
    //realoca a lista de limpeza se necessario
    if (eco->qtd_limpeza_mem == eco->tam_limpeza_mem)
    {
        eco->tam_limpeza_mem *= 2;
        objeto **lista_tmp = realloc(eco->lista_limpeza_mem, sizeof(objeto *) * eco->tam_limpeza_mem);
        eco->lista_limpeza_mem = lista_tmp;
    }

    eco->lista_limpeza_mem[eco->qtd_limpeza_mem] = obj;

    eco->qtd_limpeza_mem++;
}

//limpa os objetos marcados para tal - funciona como um coletor de lixo
void limpaListaObjetosMem(eco_thread *eco)
{
    int i;

    for (i = 0; i < eco->qtd_limpeza_mem; i++)
    {
        free(eco->lista_limpeza_mem[i]);
    }

    //atualiza o num_objetos uma vez por ciclo
    pthread_mutex_lock(&(eco->base->mutex_eco));
    eco->base->num_objetos += eco->novos - eco->qtd_limpeza_mem;
    pthread_mutex_unlock(&(eco->base->mutex_eco));

    eco->novos = 0;

    eco->qtd_limpeza_mem = 0;
}

//limpa toda a memoria alocada pelo aplicativo
void limpezaGeral(ecosistema *eco)
{
    int x, y;

    for (x = 0; x < eco->linhas; x++)
    {
        for (y = 0; y < eco->colunas; y++)
        {
            if (eco->matriz[x][y]->pos_atual) {free(eco->matriz[x][y]->pos_atual);}
            if (eco->matriz[x][y]->pos_prox) {free(eco->matriz[x][y]->pos_prox);}

            pthread_mutex_destroy(&(eco->lista_mutex[x][y]));
            free(eco->matriz[x][y]);
        }
    }

    for (x = 0; x < eco->qtd_threads; x++)
    {
        free(eco->lista_eco_thread[x].lista_limpeza_pos);
        free(eco->lista_eco_thread[x].lista_limpeza_mem);
    }

    free(eco->lista_eco_thread);

    for (x = 0; x < eco->linhas; x++)
    {
        free(eco->matriz[x]);
        free(eco->lista_mutex[x]);
    }

    thpool_destroy(eco->thpool);

    free(eco->lista_mutex);
    free(eco->matriz);
}
