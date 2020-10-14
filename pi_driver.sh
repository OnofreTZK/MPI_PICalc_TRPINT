#!/bin/bash

#Compilação de Código. Modifique para o que mais se adequa a você.
mpicxx -g -Wall parallel/pi_parallel.cpp -o piprl

#OBRIGATÓRIO: Laço de iteração para resgate dos tempos de acordo com "cores" e "size"

#Loop principal de execuções. São 10 tentativas
	tentativas=5 #Quantas vezes o código será executado dado um par (cores,size)

	for cores in 2 4 8 #números de cores utilizados
	do
			for size in 1000000 1300000 1600000 1900000 #tamanho do problema
			do   	
				echo -e "\n$cores\t$size\t\t\c" >> "pi_cores_samples.txt" 

				for tentativa in $(seq $tentativas) #Cria uma vetor de 1 a "tentativas"
				do
					#Executar o código.
					mpiexec -np  $cores --oversubscribe ./piprl $size >> "pi_parallel_time.txt" 
				done
			done

	done
	
	exit 

rm -rf piprl

exit
