#!/bin/bash

BIN_DIR_SP="/home/luizpaulo/build-cptrab2-Desktop-Release-sp"
BIN_DIR_MP1="/home/luizpaulo/build-cptrab2-Desktop-Release-mp1"
BIN_DIR_MP2="/home/luizpaulo/build-cptrab2-Desktop-Release-mp2"
IN_DIR="/home/luizpaulo/trab2-input"
OUT_DIR="/home/luizpaulo/cptrab2/relatorio"

cd ${IN_DIR}

echo "Processando sp..."

/usr/bin/valgrind --tool=callgrind --callgrind-out-file=${OUT_DIR}/callgrind_sp_20x20.out ${BIN_DIR_SP}/cptrab_bin ${IN_DIR}/input20x20 > /dev/null
/usr/bin/gprof2dot --format=callgrind --output=${OUT_DIR}/out_sp_20x20.dot ${OUT_DIR}/callgrind_sp_20x20.out
/usr/bin/dot -Tpng ${OUT_DIR}/out_sp_20x20.dot -o ${OUT_DIR}/grafico_sp_20x20.png

/usr/bin/valgrind --tool=callgrind --callgrind-out-file=${OUT_DIR}/callgrind_sp_200x200.out ${BIN_DIR_SP}/cptrab_bin ${IN_DIR}/input200x200 > /dev/null
/usr/bin/gprof2dot --format=callgrind --output=${OUT_DIR}/out_sp_200x200.dot ${OUT_DIR}/callgrind_sp_200x200.out
/usr/bin/dot -Tpng ${OUT_DIR}/out_sp_200x200.dot -o ${OUT_DIR}/grafico_sp_200x200.png

CPUPROFILE=${OUT_DIR}/prof_sp_20x20.out ${BIN_DIR_SP}/cptrab_bin ${IN_DIR}/input20x20 > /dev/null
/usr/bin/pprof --dot ${BIN_DIR_SP}/cptrab_bin ${OUT_DIR}/prof_sp_${threads}_20x20.out > ${OUT_DIR}/prof_sp_20x20.dot
/usr/bin/dot -Tpng ${OUT_DIR}/prof_sp_20x20.dot -o ${OUT_DIR}/grafico_prof_sp_20x20.png

CPUPROFILE=${OUT_DIR}/prof_sp_200x200.out ${BIN_DIR_SP}/cptrab_bin ${IN_DIR}/input200x200 > /dev/null
/usr/bin/pprof --dot ${BIN_DIR_SP}/cptrab_bin ${OUT_DIR}/prof_sp_200x200.out > ${OUT_DIR}/prof_sp_200x200.dot
/usr/bin/dot -Tpng ${OUT_DIR}/prof_sp_200x200.dot -o ${OUT_DIR}/grafico_prof_sp_200x200.png

for threads in 1 2 4 6 8
do
	echo "Processando  MP1 ${threads}..."
	#Valgrind
    /usr/bin/valgrind --tool=callgrind --callgrind-out-file=${OUT_DIR}/callgrind_mp1_${threads}_20x20.out ${BIN_DIR_MP1}/cptrab_bin ${IN_DIR}/input20x20 ${threads} > /dev/null
    /usr/bin/gprof2dot --format=callgrind --output=${OUT_DIR}/out_mp1_${threads}_20x20.dot ${OUT_DIR}/callgrind_mp1_${threads}_20x20.out
    /usr/bin/dot -Tpng ${OUT_DIR}/out_mp1_${threads}_20x20.dot -o ${OUT_DIR}/grafico_mp1_${threads}_20x20.png

    /usr/bin/valgrind --tool=callgrind --callgrind-out-file=${OUT_DIR}/callgrind_mp1_${threads}_200x200.out ${BIN_DIR_MP1}/cptrab_bin ${IN_DIR}/input200x200 ${threads} > /dev/null
    /usr/bin/gprof2dot --format=callgrind --output=${OUT_DIR}/out_mp1_${threads}_200x200.dot ${OUT_DIR}/callgrind_mp1_${threads}_200x200.out
    /usr/bin/dot -Tpng ${OUT_DIR}/out_mp1_${threads}_200x200.dot -o ${OUT_DIR}/grafico_mp1_${threads}_200x200.png    
	#pprof
    CPUPROFILE=${OUT_DIR}/prof_mp1_${threads}_20x20.out ${BIN_DIR_MP1}/cptrab_bin ${IN_DIR}/input20x20 ${threads} > /dev/null
    /usr/bin/pprof --dot ${BIN_DIR_MP1}/cptrab_bin ${OUT_DIR}/prof_mp1_${threads}_20x20.out > ${OUT_DIR}/prof_mp1_${threads}_20x20.dot
    /usr/bin/dot -Tpng ${OUT_DIR}/prof_mp1_${threads}_20x20.dot -o ${OUT_DIR}/grafico_prof_mp1_${threads}_20x20.png

    CPUPROFILE=${OUT_DIR}/prof_mp1_${threads}_200x200.out ${BIN_DIR_MP1}/cptrab_bin ${IN_DIR}/input200x200 ${threads} > /dev/null
    /usr/bin/pprof --dot ${BIN_DIR_MP1}/cptrab_bin ${OUT_DIR}/prof_mp1_${threads}_200x200.out > ${OUT_DIR}/prof_mp1_${threads}_200x200.dot
    /usr/bin/dot -Tpng ${OUT_DIR}/prof_mp1_${threads}_200x200.dot -o ${OUT_DIR}/grafico_prof_mp1_${threads}_200x200.png
done

for threads in 1 2 4 6 8
do
	echo "Processando MP2 ${threads}..."
	#Valgrind
	/usr/bin/valgrind --tool=callgrind --callgrind-out-file=${OUT_DIR}/callgrind_mp2_${threads}_20x20.out ${BIN_DIR_MP2}/cptrab_bin ${IN_DIR}/input20x20 ${threads} > /dev/null
    /usr/bin/gprof2dot --format=callgrind --output=${OUT_DIR}/out_mp2_${threads}_20x20.dot ${OUT_DIR}/callgrind_mp2_${threads}_20x20.out
    /usr/bin/dot -Tpng ${OUT_DIR}/out_mp2_${threads}_20x20.dot -o ${OUT_DIR}/grafico_mp2_${threads}_20x20.png

    /usr/bin/valgrind --tool=callgrind --callgrind-out-file=${OUT_DIR}/callgrind_mp2_${threads}_200x200.out ${BIN_DIR_MP2}/cptrab_bin ${IN_DIR}/input200x200 ${threads} > /dev/null
    /usr/bin/gprof2dot --format=callgrind --output=${OUT_DIR}/out_mp2_${threads}_200x200.dot ${OUT_DIR}/callgrind_mp2_${threads}_200x200.out
    /usr/bin/dot -Tpng ${OUT_DIR}/out_mp2_${threads}_200x200.dot -o ${OUT_DIR}/grafico_mp2_${threads}_200x200.png        
	#pprof
    CPUPROFILE=${OUT_DIR}/prof_mp2_${threads}_20x20.out ${BIN_DIR_MP2}/cptrab_bin ${IN_DIR}/input20x20 ${threads} > /dev/null
    /usr/bin/pprof --dot ${BIN_DIR_MP2}/cptrab_bin ${OUT_DIR}/prof_mp2_${threads}_20x20.out > ${OUT_DIR}/prof_mp2_${threads}_20x20.dot
    /usr/bin/dot -Tpng ${OUT_DIR}/prof_mp2_${threads}_20x20.dot -o ${OUT_DIR}/grafico_prof_mp2_${threads}_20x20.png

    CPUPROFILE=${OUT_DIR}/prof_mp2_${threads}_200x200.out ${BIN_DIR_MP2}/cptrab_bin ${IN_DIR}/input200x200 ${threads} > /dev/null
    /usr/bin/pprof --dot ${BIN_DIR_MP2}/cptrab_bin ${OUT_DIR}/prof_mp2_${threads}_200x200.out > ${OUT_DIR}/prof_mp2_${threads}_200x200.dot
    /usr/bin/dot -Tpng ${OUT_DIR}/prof_mp2_${threads}_200x200.dot -o ${OUT_DIR}/grafico_prof_mp2_${threads}_200x200.png

#     /usr/bin/pprof --callgrind ${BIN_DIR}/cptrab_bin ${OUT_DIR}/prof_${threads}.out > ${OUT_DIR}/prof_${threads}.call
#     /usr/bin/gprof2dot --format=callgrind --output=${OUT_DIR}/prof_${threads}.dot ${OUT_DIR}/prof_${threads}.call
#     /usr/bin/dot -Tpng ${OUT_DIR}/prof_${threads}.dot -o ${OUT_DIR}/grafico_prof_${threads}.png
done
