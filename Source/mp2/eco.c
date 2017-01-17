#include "eco.h"

int carregaDadosIniciais(char *nomeArq)
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
            case 0: eco.gen_proc_coelhos = num; break;
            case 1: eco.gen_proc_raposas = num; break;
            case 2: eco.gen_comida_raposas = num; break;
            case 3: eco.n_gen = num; break;
            case 4: eco.linhas = num; break;
            case 5: eco.colunas = num; break;
            case 6: eco.num_objetos = num; break;
        }
    }

    eco.n_gen_atual = 0;

    //aloca a matriz
    eco.matriz = (posicao ***) malloc(eco.linhas * sizeof(posicao **));

    if (eco.matriz == 0) {return 0;}

    for (x = 0; x < eco.linhas; x++)
    {
        eco.matriz[x] = (posicao **) calloc(eco.colunas, sizeof(posicao *));
        if (eco.matriz[x] == 0) {return 0;}
        for (y = 0; y < eco.colunas; y++)
        {
            eco.matriz[x][y] = calloc(1, sizeof(posicao));
            if (eco.matriz[x][y] == 0) {return 0;}
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

        eco.matriz[x][y]->pos_atual = calloc(1, sizeof(objeto));
        eco.matriz[x][y]->pos_atual->tipo = tipo;
        eco.matriz[x][y]->pos_atual->x = x;
        eco.matriz[x][y]->pos_atual->y = y;
        //eco.matriz[x][y]->pos_atual->id = obtemId();
    }

    eco.tam_limpeza_pos = eco.num_objetos * 2;
    eco.qtd_limpeza_pos = 0;
    eco.lista_limpeza_pos = (coord *) malloc(sizeof(coord) * eco.tam_limpeza_pos);

    eco.tam_limpeza_mem = eco.tam_limpeza_pos;
    eco.qtd_limpeza_mem = 0;
    eco.lista_limpeza_mem = (objeto **) malloc(sizeof(objeto *) * eco.tam_limpeza_mem);

    preparaThreads();

    return 1;
}

void imprimeEcosistema()
{
    int x, y, col_espaco;

    //col_espaco = eco.colunas * 2 + 1;
    col_espaco = eco.colunas + 2;

    printf("Generation %d\n", eco.n_gen_atual);

    for (y = 0; y < col_espaco; y++) {printf("-");} printf("\n");

    for (x = 0; x < eco.linhas; x++)
    {
        printf("|");
        for (y = 0; y < eco.colunas; y++)
        {
            if (eco.matriz[x][y]->pos_atual)
            {
                switch (eco.matriz[x][y]->pos_atual->tipo)
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
            //if (y < eco.colunas-1) {printf(" ");}
        }
        printf("|\n");
    }

    for (y = 0; y < col_espaco; y++) {printf("-");} printf("\n");

    if (eco.n_gen_atual != eco.n_gen) {printf("\n");}
}

//imprime com o id - para fins de depuracao
void imprimeEcosistemaId()
{
    int x, y, col_espaco;

    //col_espaco = eco.colunas * 2 + 1;
    col_espaco = eco.colunas*8 + 8 + 2;

    printf("Generation %d\n", eco.n_gen_atual);

    printf("          "); for (y = 0; y < eco.colunas; y++) {printf("[%6d]", y);} printf("\n");
    for (y = 0; y < col_espaco; y++) {printf("-");} printf("\n");

    for (x = 0; x < eco.linhas; x++)
    {
        printf("[%6d]|", x);
        for (y = 0; y < eco.colunas; y++)
        {
            if (eco.matriz[x][y]->pos_atual)
            {
                switch (eco.matriz[x][y]->pos_atual->tipo)
                {
                    case PEDRA: printf("*[     ]"); break;
                    case COELHO: printf("R[%5d]", eco.matriz[x][y]->pos_atual->id); break;
                    case RAPOSA: printf("F[%5d]", eco.matriz[x][y]->pos_atual->id);; break;
                }
            }
            else //VAZIO
            {
                printf("        ");
            }
            //if (y < eco.colunas-1) {printf(" ");}
        }
        printf("|\n");
    }

    for (y = 0; y < col_espaco; y++) {printf("-");} printf("\n");

    if (eco.n_gen_atual != eco.n_gen) {printf("\n");}
}

void imprimeSaida()
{
    int x, y;

    printf("%d %d %d %d %d %d %d\n", eco.gen_proc_coelhos, eco.gen_proc_raposas, eco.gen_comida_raposas, eco.n_gen_atual - eco.n_gen, eco.linhas, eco.colunas, eco.num_objetos);

    for (x = 0; x < eco.linhas; x++)
    {
        for (y = 0; y < eco.colunas; y++)
        {
            if (eco.matriz[x][y]->pos_atual)
            {
                switch (eco.matriz[x][y]->pos_atual->tipo)
                {
                    case PEDRA: printf("ROCK %d %d\n", x, y); break;
                    case COELHO: printf("RABBIT %d %d\n", x, y); break;
                    case RAPOSA: printf("FOX %d %d\n", x, y); break;
                }
            }
        }
    }
}

//prepara a estrutura de threads (thread pool, mutex, etc)
void preparaThreads()
{
    int x, y;

    eco.thpool = thpool_init(eco.qtd_threads);

    pthread_mutex_init(&(eco.mutex_eco), NULL);

    eco.lista_mutex = (pthread_mutex_t **) malloc(eco.linhas * sizeof(pthread_mutex_t *));

    for (x = 0; x < eco.linhas; x++)
    {
        eco.lista_mutex[x] = (pthread_mutex_t *) malloc(eco.colunas * sizeof(pthread_mutex_t));
        for (y = 0; y < eco.linhas; y++)
        {
            pthread_mutex_init(&(eco.lista_mutex[x][y]), NULL);
        }
    }
}

int executaCiclo()
{
    int x, y;

    eco.n_gen_atual++;

    for (x = 0; x < eco.linhas; x++)
    {
        for (y = 0; y < eco.colunas; y++)
        {
            //so move se existir um objeto naquela posicao
            if (eco.matriz[x][y]->pos_atual != 0 && eco.matriz[x][y]->pos_atual->tipo == COELHO) {thpool_add_work(eco.thpool, (void *)processaObjeto, (void *)eco.matriz[x][y]->pos_atual);}
        }
    }

    thpool_wait(eco.thpool);

    limpaListaObjetosPos();

    for (x = 0; x < eco.linhas; x++)
    {
        for (y = 0; y < eco.colunas; y++)
        {
            //so move se exitir um objeto naquela posicao
            if (eco.matriz[x][y]->pos_atual != 0 && eco.matriz[x][y]->pos_atual->tipo == RAPOSA) {thpool_add_work(eco.thpool, (void *)processaObjeto, (void *)eco.matriz[x][y]->pos_atual);}
        }
    }

    thpool_wait(eco.thpool);

    thpool_add_work(eco.thpool, (void *)limpaListaObjetosPos, NULL);
    thpool_add_work(eco.thpool, (void *)limpaListaObjetosMem, NULL);

    thpool_wait(eco.thpool);

    return 0;
}

void processaObjeto(objeto *obj)
{
    int p;
    coord pos[4];

    memset(&pos, 0, sizeof(coord) * 4);

    if (obj->tipo == COELHO)
    {
        p = verificaPossibilidades(obj, VAZIO, pos);
        moveObjeto(obj, p, pos);
    }
    else //RAPOSA
    {
        p = verificaPossibilidades(obj, COELHO, pos);
        if (p != -1)
        {
            moveObjeto(obj, p, pos);
        }
        else
        {
            obj->comida++;
            p = verificaPossibilidades(obj, VAZIO, pos);
            moveObjeto(obj, p, pos);
        }
    }
}

//retorna a qtd de possibilidades encontradas
int verificaPossibilidades(objeto *obj, tipo_objeto objetivo, coord *pos)
{
    int resul = 0;

    if (obj->x != 0 && tipoPos(obj->x-1, obj->y) == objetivo) //verifica se pode ir para o norte
    {
        pos[resul].x = obj->x - 1; pos[resul].y = obj->y; ; resul++;
    }
    if (obj->y != eco.colunas-1 && tipoPos(obj->x, obj->y+1) == objetivo) //verifica se pode ir para o leste
    {
        pos[resul].x = obj->x; pos[resul].y = obj->y + 1; ; resul++;
    }
    if (obj->x != eco.linhas-1 && tipoPos(obj->x+1, obj->y) == objetivo) //verifica se pode ir para o sul
    {
        pos[resul].x = obj->x + 1; pos[resul].y = obj->y; ; resul++;
    }
    if (obj->y != 0 && tipoPos(obj->x, obj->y-1) == objetivo) //verifica se pode ir para o oeste
    {
        pos[resul].x = obj->x; pos[resul].y = obj->y - 1; ; resul++;
    }

    return resul-1;
}

inline tipo_objeto tipoPos(int x, int y)
{
    //trata o caso do vazio que na verdade eh um ponteiro zerado
    return eco.matriz[x][y]->pos_atual == 0?VAZIO:eco.matriz[x][y]->pos_atual->tipo;
}

void moveObjeto(objeto *obj, int p, coord *pos)
{
    pthread_mutex_t *mutex = &(eco.lista_mutex[obj->x][obj->y]);

    if (p > 0) {p = ((eco.n_gen_atual-1) + obj->x + obj->y) % (p+1);}

    adicionaObjetoLimpezaPos(obj->x, obj->y);

    if (p != -1)
    {
        pthread_mutex_t *mutex_pos = &(eco.lista_mutex[pos[p].x][pos[p].y]);

        pthread_mutex_lock(mutex_pos);

        //a raposa morre se passar fome por muito tempo (o coelho tem sempre o valor 0 em comida portanto nunca entra neste if)
        if (obj->comida == eco.gen_comida_raposas)
        {
            //se nessa ultima rodada nao comer um coelho ela morre
            if (!(eco.matriz[pos[p].x][pos[p].y]->pos_atual && eco.matriz[pos[p].x][pos[p].y]->pos_atual->tipo == COELHO))
            {
                adicionaObjetoLimpezaMem(obj);
                pthread_mutex_lock(&(eco.mutex_eco));
                eco.num_objetos--;
                pthread_mutex_unlock(&(eco.mutex_eco));
                pthread_mutex_unlock(mutex_pos);
                return;
            }
        }

        if (eco.matriz[pos[p].x][pos[p].y]->pos_prox == 0) {adicionaObjetoLimpezaPos(pos[p].x, pos[p].y);}

        //procria se for a hora
        if ((obj->tipo == COELHO && obj->proc >= eco.gen_proc_coelhos) || (obj->tipo == RAPOSA && obj->proc >= eco.gen_proc_raposas))
        {
            pthread_mutex_lock(mutex);
            eco.matriz[obj->x][obj->y]->pos_prox = malloc(sizeof(objeto));
            eco.matriz[obj->x][obj->y]->pos_prox->tipo = obj->tipo;
            eco.matriz[obj->x][obj->y]->pos_prox->x = obj->x;
            eco.matriz[obj->x][obj->y]->pos_prox->y = obj->y;
            eco.matriz[obj->x][obj->y]->pos_prox->gen = eco.matriz[obj->x][obj->y]->pos_prox->proc = -1;
            eco.matriz[obj->x][obj->y]->pos_prox->comida =  0;
            //eco.matriz[obj->x][obj->y]->pos_prox->id = obtemId();
            pthread_mutex_lock(&(eco.mutex_eco));
            eco.num_objetos++;
            pthread_mutex_unlock(&(eco.mutex_eco));
            obj->proc = -1;
            pthread_mutex_unlock(mutex);
        }


        obj->x = pos[p].x;
        obj->y = pos[p].y;

        //nao ha nada neste local nem esta prevista para haver
        if (eco.matriz[obj->x][obj->y]->pos_atual == 0 && eco.matriz[obj->x][obj->y]->pos_prox == 0)
        {
            eco.matriz[obj->x][obj->y]->pos_prox = obj;
        }
        else
        {
            //tem algo no atual mas nada previsto para haver na proxima rodada
            if (eco.matriz[obj->x][obj->y]->pos_atual && eco.matriz[obj->x][obj->y]->pos_prox == 0)
            {
                //do mesmo tipo nao a conflito pois os objetos ainda estao movendo
                if (eco.matriz[obj->x][obj->y]->pos_atual->tipo == obj->tipo)
                {
                    eco.matriz[obj->x][obj->y]->pos_prox = obj;
                }
                //tipos diferentes - resolve o conflito
                else
                {
                    eco.matriz[obj->x][obj->y]->pos_prox = resolveConflito(eco.matriz[obj->x][obj->y]->pos_atual, obj);
                }
            }
            //jah tem algo previsto para a proxima rodada
            else
            {
                eco.matriz[obj->x][obj->y]->pos_prox = resolveConflito(eco.matriz[obj->x][obj->y]->pos_prox, obj);
            }

            pthread_mutex_lock(&(eco.mutex_eco));
            eco.num_objetos--;
            pthread_mutex_unlock(&(eco.mutex_eco));
        }

        pthread_mutex_unlock(mutex_pos);
    }
    else //quando o objeto permanece na mesma posicao
    {
        if (obj->comida == eco.gen_comida_raposas)
        {
            adicionaObjetoLimpezaMem(obj);
            pthread_mutex_lock(&(eco.mutex_eco));
            eco.num_objetos--;
            pthread_mutex_unlock(&(eco.mutex_eco));
        }
        else
        {
            //fica no mesmo lugar se for possivel, do contrario resolve o conflito
            pthread_mutex_lock(mutex);

            if (eco.matriz[obj->x][obj->y]->pos_prox == 0) {eco.matriz[obj->x][obj->y]->pos_prox = obj;}
            else {eco.matriz[obj->x][obj->y]->pos_prox = resolveConflito(eco.matriz[obj->x][obj->y]->pos_prox, obj);}

            pthread_mutex_unlock(mutex);
        }
    }
}

objeto* resolveConflito(objeto *obj1, objeto *obj2)
{
    if (obj1->tipo == COELHO && obj2->tipo == RAPOSA)
    {
        obj2->comida = 0;
        adicionaObjetoLimpezaMem(obj1);
        return obj2;
    }
    else if (obj1->tipo == RAPOSA && obj2->tipo == COELHO)
    {
        obj1->comida = 0;//check
        adicionaObjetoLimpezaMem(obj2);
        return obj1;
    }
    else //RAPOSA e RAPOSA ou COELHO e COELHO - sobrevive quem tem o maior proc
    {
        if (obj1->proc > obj2->proc)
        {
            adicionaObjetoLimpezaMem(obj2);
            return obj1;
        }
        else if (obj1->proc < obj2->proc)
        {
            adicionaObjetoLimpezaMem(obj1);
            return obj2;
        }
        else //idade proc igual
        {
            if (obj1->comida > obj2->comida) //sobrevive a raposa menos faminta - para o coelho isso nao importa, o seu valor de comida eh sempre 0
            {
                adicionaObjetoLimpezaMem(obj1);
                return obj2;
            }
            else //aplica para o caso do coelho tb
            {
                adicionaObjetoLimpezaMem(obj2);
                return obj1;
            }
        }
    }
}

//adiciona objeto a lista de limpeza (para limpar no final do ciclo)
void adicionaObjetoLimpezaPos(int x, int y)
{
    pthread_mutex_lock(&(eco.mutex_eco));
    //realoca a lista de limpeza se necessario
    if (eco.qtd_limpeza_pos == eco.tam_limpeza_pos)
    {
        eco.tam_limpeza_pos *= 2;
        coord *lista_tmp = realloc(eco.lista_limpeza_pos, sizeof(coord) * eco.tam_limpeza_pos);
        eco.lista_limpeza_pos = lista_tmp;
    }

    eco.lista_limpeza_pos[eco.qtd_limpeza_pos].x = x;
    eco.lista_limpeza_pos[eco.qtd_limpeza_pos].y = y;

    eco.qtd_limpeza_pos++;
    pthread_mutex_unlock(&(eco.mutex_eco));
}

//limpa os ponteiros da matriz atual
void limpaListaObjetosPos()
{
    int i;

    for (i = 0; i < eco.qtd_limpeza_pos; i++)
    {
        eco.matriz[eco.lista_limpeza_pos[i].x][eco.lista_limpeza_pos[i].y]->pos_atual = eco.matriz[eco.lista_limpeza_pos[i].x][eco.lista_limpeza_pos[i].y]->pos_prox;
        eco.matriz[eco.lista_limpeza_pos[i].x][eco.lista_limpeza_pos[i].y]->pos_prox = 0;

        if (eco.matriz[eco.lista_limpeza_pos[i].x][eco.lista_limpeza_pos[i].y]->pos_atual)
        {
            eco.matriz[eco.lista_limpeza_pos[i].x][eco.lista_limpeza_pos[i].y]->pos_atual->proc++;
            eco.matriz[eco.lista_limpeza_pos[i].x][eco.lista_limpeza_pos[i].y]->pos_atual->gen++;
        }
    }

    eco.qtd_limpeza_pos = 0;
}

void adicionaObjetoLimpezaMem(objeto *obj)
{
    pthread_mutex_lock(&(eco.mutex_eco));
    //realoca a lista de limpeza se necessario
    if (eco.qtd_limpeza_mem == eco.tam_limpeza_mem)
    {
        eco.tam_limpeza_mem *= 2;
        objeto **lista_tmp = realloc(eco.lista_limpeza_mem, sizeof(objeto *) * eco.tam_limpeza_mem);
        eco.lista_limpeza_mem = lista_tmp;
    }

    eco.lista_limpeza_mem[eco.qtd_limpeza_mem] = obj;

    eco.qtd_limpeza_mem++;
    pthread_mutex_unlock(&(eco.mutex_eco));
}

//limpa os objetos marcados para tal - funciona como um coletor de lixo
void limpaListaObjetosMem()
{
    int i;

    for (i = 0; i < eco.qtd_limpeza_mem; i++)
    {
        free(eco.lista_limpeza_mem[i]);
    }

    eco.qtd_limpeza_mem = 0;
}

//limpa toda a memoria alocada pelo aplicativo
void limpezaGeral()
{
    int x, y;

    for (x = 0; x < eco.linhas; x++)
    {
        for (y = 0; y < eco.colunas; y++)
        {
            if (eco.matriz[x][y]->pos_atual) {free(eco.matriz[x][y]->pos_atual);}
            if (eco.matriz[x][y]->pos_prox) {free(eco.matriz[x][y]->pos_prox);}

            pthread_mutex_destroy(&(eco.lista_mutex[x][y]));
            free(eco.matriz[x][y]);
        }
    }

    free(eco.lista_limpeza_pos);
    free(eco.lista_limpeza_mem);

    for (x = 0; x < eco.linhas; x++)
    {
        free(eco.matriz[x]);
        free(eco.lista_mutex[x]);
    }

    thpool_destroy(eco.thpool);

    free(eco.lista_mutex);
    free(eco.matriz);
}

//obtem id do objeto - usado apenas para fins de depuracao
int obtemId()
{
    static int id = 0;

    return id++;
}
