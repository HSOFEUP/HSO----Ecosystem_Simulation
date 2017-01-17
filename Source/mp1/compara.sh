#!/bin/bash

BIN_DIR_SP="/home/luizpaulo/build-cptrab2-Desktop-Release-sp"
BIN_DIR="/home/luizpaulo/build-cptrab2-Desktop-Release"
IN_DIR="/home/luizpaulo/trab2-input"

cd ${IN_DIR}

for input in `ls input*`;
do
    output=${input/input/output}
    #echo ${input}
    #echo ${output}
    echo "Processando ${input}..."
    if [ ! -f ${BIN_DIR_SP}/${output} ]; then
        ${BIN_DIR_SP}/cptrab_bin ${IN_DIR}/${input} > ${BIN_DIR_SP}/${output}
    fi
    diff ${BIN_DIR_SP}/${output} ${IN_DIR}/${output} > /dev/null
    if [ $? -eq 1 ]; then
        echo "Outputs diferentes..."
    else
        echo "Outputs iguais!"
    fi
done

for input in `ls input*`;
do
    output=${input/input/output}
    echo "Processando MP ${input}..."
    ${BIN_DIR}/cptrab_bin ${IN_DIR}/${input} > ${BIN_DIR}/${output}
    diff ${BIN_DIR}/${output} ${BIN_DIR_SP}/${output} > /dev/null
    if [ $? -eq 1 ]; then
        echo "Outputs diferentes..."
    else
        echo "Outputs iguais!"
    fi
done
