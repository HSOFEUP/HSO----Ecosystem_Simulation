#!/bin/bash

BIN_DIR="/home/luizpaulo/build-cptrab2-Desktop-Release"
IN_DIR="/home/luizpaulo/trab2-input"

cd ${IN_DIR}

for input in `ls input*`;
do
    output=${input/input/output}
    #echo ${input}
    #echo ${output}
    echo "Processando ${input}..."
    ${BIN_DIR}/cptrab_bin ${IN_DIR}/${input} > ${BIN_DIR}/${output}
    diff ${BIN_DIR}/${output} ${IN_DIR}/${output} > /dev/null
    if [ $? -eq 1 ]; then
        echo "Outputs diferentes..."
    else
        echo "Outputs iguais!"
    fi
done
