#include <stdio.h>
#include <string.h>

#include "eco.h"

int main(int argc, char **argv)
{
    int x;

    memset(&eco, 0, sizeof(eco));

    if (argc > 2) {eco.qtd_threads = atoi(argv[2]);}
    if (eco.qtd_threads == 0) {eco.qtd_threads = 4;}

    if (argc > 1) {carregaDadosIniciais(argv[1]);}
    else {carregaDadosIniciais("/home/luizpaulo/trab2-input/input20x20");}
//    else {carregaDadosIniciais("eco.txt", &eco);}

    //imprimeEcosistema(&eco);

    for (x = 0; x < eco.n_gen; x++)
    {
        executaCiclo();

        //imprimeEcosistema(&eco);
    }

    imprimeSaida();

    limpezaGeral();

    return 0;
}
