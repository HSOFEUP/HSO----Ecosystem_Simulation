#include <stdio.h>
#include <string.h>

#include "eco.h"

int main(int argc, char **argv)
{
    ecosistema eco;
    unsigned int x;

    memset(&eco, 0, sizeof(eco));

    if (argc > 2) {eco.qtd_threads = atoi(argv[2]);}
    if (eco.qtd_threads == 0) {eco.qtd_threads = 4;}

    if (argc > 1) {carregaDadosIniciais(argv[1], &eco);}
    else {carregaDadosIniciais("/home/luizpaulo/trab2-input/input100x100", &eco);}
//    else {carregaDadosIniciais("eco.txt", &eco);}

    //imprimeEcosistema(&eco);

    for (x = 0; x < eco.n_gen; x++)
    {
        executaCiclo(&eco);

        //imprimeEcosistema(&eco);
    }

    imprimeSaida(&eco);

    limpezaGeral(&eco);

    return 0;
}
