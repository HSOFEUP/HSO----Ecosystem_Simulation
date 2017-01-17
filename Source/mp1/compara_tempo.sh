#!/bin/bash

BIN_DIR_SP="/home/luizpaulo/build-cptrab2-Desktop-Release-sp"
BIN_DIR_MP1="/home/luizpaulo/build-cptrab2-Desktop-Release-mp1"
BIN_DIR_MP2="/home/luizpaulo/build-cptrab2-Desktop-Release-mp2"
IN_DIR="/home/luizpaulo/trab2-input"

cd ${IN_DIR}

for input in `ls input*`;
do
    output=${input/input/output}

    echo "Processando ${input}..."
    /usr/bin/time -f '%Uu %Sk %es %Mkb %w waits' ${BIN_DIR_SP}/cptrab_bin ${IN_DIR}/${input} > ${BIN_DIR_SP}/${output}

    for threads in 1 2 4 6 8
    do
        echo "Processando MP1 ${threads} ${input}..."
        /usr/bin/time -f '%Uu %Sk %es %Mkb %w waits' ${BIN_DIR_MP1}/cptrab_bin ${IN_DIR}/${input} ${threads} > ${BIN_DIR_MP1}/${output}
    done;

    for threads in 1 2 4 6 8
    do
        echo "Processando MP2 ${threads} ${input}..."
        /usr/bin/time -f '%Uu %Sk %es %Mkb %w waits' ${BIN_DIR_MP2}/cptrab_bin ${IN_DIR}/${input} ${threads} > ${BIN_DIR_MP2}/${output}
    done;
    
done
