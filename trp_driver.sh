#!/bin/bash

#Compilação de Código.
mpicxx -g -Wall parallel/trp_parallel.cpp -o trpprl

#OBRIGATÓRIO: Laço de iteração para resgate dos tempos de acordo com "cores" e "size"

#Loop principal de execuções. São 10 tentativas
	tentativas=5 #Quantas vezes o código será executado dado um par (cores,size)

	for cores in 2 4 8 #números de cores utilizados
	do
			for size in 2200000000 2900000000 3300000000 4400000000 #tamanho do problema
			do   	
				echo -e "\n$cores\t$size\t\t\c" >> "trp_cores_samples.txt" 

				for tentativa in $(seq $tentativas) #Cria uma vetor de 1 a "tentativas"
				do
					#Executar o código.
					mpiexec -np  $cores --oversubscribe ./trpprl 0 60 $size >> "trp_parallel_time.txt" 
				done
			done

	done
	
	exit 

rm -rf trpprl

exit
