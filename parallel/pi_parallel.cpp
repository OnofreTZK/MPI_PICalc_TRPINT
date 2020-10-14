#include <iostream>
#include <chrono>
#include <iomanip>
#include <random>
#include <mpi.h>

double generate_number()
{
      std::random_device seed;

      std::mt19937 gen( seed() );

      std::uniform_real_distribution<double> distr(0.0, 1.0);

      return distr( gen );
}


double monteCarlo(long int n) 
{

    int correct_hits = 0;

    for( int i = 0; i < n; i++ ) 
    {
    
        double x = generate_number();
        double y = generate_number();

        if( ( ( x * x ) + ( y * y ) ) < 1 ) 
        {
        
            correct_hits++;
        }
    }

    return correct_hits;

}


int main( int argc, char *argv[] ) 
{

    if( argc > 2 )
    {
        std::cout << "Too many arguments! Only one(1) necessary." << std::endl;
        return EXIT_FAILURE;
    }
    else if( argc == 1 )
    {
        std::cout << "Please give a integer value to N as an argument" << std::endl;
        return EXIT_FAILURE;
    }

    // else

    
    int my_rank, comm_sz;


    // Starting MPI ---------------------------------------------------------------------------------------
    MPI_Init(NULL, NULL);

    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    
    // Getting N
    long int N = atoi(argv[1]);

    // MAIN VARS ------------------------------------------------------------------------------------------
    
    // Local N
    long int local_N = N/comm_sz;

    // Total hits
    int total_hits;

    // Local hits
    double local_hits;

    // pi
    double pi;

    // Final time
    double final_time;
    

    // ----------------------------------------------------------------------------------------------------
    // - START TIMER -
    std::chrono::steady_clock::time_point START = std::chrono::steady_clock::now();
    local_hits = monteCarlo(local_N);
    std::chrono::steady_clock::time_point STOP = std::chrono::steady_clock::now();
    // - STOP TIMER -
    // ----------------------------------------------------------------------------------------------------


    // Aux local vars
    auto timer = (STOP - START);

    // Current execution time
    double local_time = std::chrono::duration< double > (timer).count(); 

    auto local_pair = std::make_pair( local_hits, local_time );

    auto pair_sz = sizeof(local_pair);


    // MPI COMMS
    if( my_rank != 0 )
    {
        // Send to root the local number of correct hits and exec time
        MPI_Send( &local_pair, pair_sz, MPI_PACKED, 0, 0, MPI_COMM_WORLD );
    }
    else
    {
        total_hits = local_pair.first;

        final_time = local_pair.second;

        // Receiving all correct hits and exec times.
        for( int proc = 1; proc < comm_sz; proc++ )
        {
            MPI_Recv( &local_pair, pair_sz, MPI_PACKED, proc, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
            total_hits += local_pair.first;

            // Getting the highest exec time
            if( final_time < local_pair.second )
            {
                final_time = local_pair.second;
            }

         }

        // Final result
        pi = 4.0 * total_hits / N;    
   
    

       /* to generate file with time samples. 
        std::cout << std::fixed
                  << std::setprecision(3)
                  << "N = " 
                  << N
                  << " | PI = " 
                  << pi 
                  << " | Exec time = "
                  << final_time 
                  << "s |" << std::endl;
                  */
        std::cout << std::fixed
                  << std::setprecision(3)
                  << final_time
                  << std::endl;
    }
        

    MPI_Finalize();    

    return EXIT_SUCCESS;

}
