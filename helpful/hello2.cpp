#include <iostream>
#include <mpi.h>
#include <string>

const int MAX_STRING = 100;

int main ( void ) {

    char greeting[MAX_STRING];
    int my_rank, comm_sz;

    // Abre a área paralela do código.
    MPI_Init(NULL, NULL); // --> setup inicial do MPI.
    
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);


    // Entendendo a comunicacão entre os processos
    //----------------------------------------------------------------------------------------------------------------------------
    if ( my_rank != 0 ) {
    
        sprintf( greeting, "Greetings from process %d of %d!", my_rank, comm_sz );
        MPI_Send( greeting, strlen(greeting)+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD ); // Envia mensagens para o processo 0

        /* MPI_Send
         * Mensagem
         * Tamanho da mensagem
         * Tipo da mensagem
         * Destino
         * Tag ( sempre usaremos 0 )
         * Comunicador
         */

    } else {
        
        std::cout << "Greeting from process " << my_rank << " of " << comm_sz << std::endl;

        for( int q = 1; q < comm_sz; q++ ) {
        
            MPI_Recv( greeting, MAX_STRING, MPI_CHAR, q, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
            /*MPI_Recv
             * Mensagem
             * Tamanho do buffer
             * Tipo da mensagem
             * Fonte ( qual processo enviou )
             * Tag ( sempre usaremos 0 )
             * Status de mensagem
             */
            std::cout << greeting << std::endl;
        }
    }


    /* MPI_Send e MPI_Recv
     * MPI_Send tem dois modos de funcionamento:
     *  * Com bloqueio -> O processo que enviou para a execucão enquanto 0 não confirmar que recebeu
     *  * Com buffer -> armazena a mensagem e continua sua execucão
     * 
     * MPI_Recv sempre funciona com bloqueio
     */

    // + Muitas implementaćões do MPI definem um limite no qual o sistema alterna do buffer para o bloqueio
    // + Mensagens relativamente pequenas serão armazenadas em buffer pelo MPI_Send
    // + Mensagens maiores causarão um bloqueio
    // + Um programa que depende somente do MPI_Send com buffer é considerado inseguro
    // + Esse programa pode ser executado sem problemas para vários conjuntos de entradas, mas pode travar com outros conjuntos.


    // MPI_Reduce
    // - Vetor de elementos ou variável que queremos reduzir
    // - Só importa para o root( processo que recebe os dados somados ). Vetor ou variável que recebe o resultado da redućão.
    //    + Tem tamanho de sizeof(datatype)*count
    // - Número de elementos que queremos reduzir
    // - Tipo de dado(datatype)
    // - Tipo de operaćão
    // - Número do processo que recebe a redućão( root )
    // - Comunicador
    // !!! Só funciona se for chamado por TODOS os processos


    // MPI_Bcast
    // - O dado ou mensagem
    // - ?
    // - Tipo do dado
    // - Processo de origem
    // - Comunicador
    
    
    // MPI_Scatter -> Distribui os elementos de um vetor entre os processos
    // - Vetor de dados que reside no root
    // - Quantos elementos do root serão enviado aos processos.
    //   + Normalmente é utilizado como n_de_elem_vetor/num_de_processos
    // - Tipo do dado( Tipo do vetor )
    // - Vetor de dados que receberá os dados
    // - Num de elementos que receberá( Deve ser igual a quantidade de elementos enviados )
    // - Tipo do vetor que receberá( Deve ser o mesmo do que envia )
    // - Num do processo root
    // - Comunicador
    

    // MPI_Gather -> Junta todos os dados de um vetor em um unico nó(root)


    // MPI_Barrier
    // + Cada processo que chama a barreira tem sua execućão parada.
    // + Os processos parados somente continuam sua exec se TODOS os processos do comunicador alcanćarem(executarem) a barreira.
    // + Pode ser usado como ponto de sincronizacao, uma quebra na execucao para que todos voltem a prosseguir.
    //
    //
    //----------------------------------------------------------------------------------------------------------------------------
    

    return 0;
} 
