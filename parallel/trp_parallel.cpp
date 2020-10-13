#include <mpi.h>
#include <iostream>
#include <math.h>
#include <random>
#include <chrono>
#include <iomanip>
#include <utility>

// Algorithm 2
double trapezoidal_rule(double a, double b, long int n )
{

    double x_i; //x1
	double h = (b - a) / n;
	double total = (sin(a) + sin(b)) / 2;

    // Loop for the sum of y1 to yn
	for (long int i = 1; i < n; i++)
    {
		x_i = a + i * h;
		total += sin(x_i);
	}
	
    total = h * total;
    
    return total; 
}


int main(int argc, char *argv[])
{
    if( argc > 4 )
    {
        std::cout << "Too many arguments! Only 3 are needed( point a, point b and number of trapezes )." 
                  << std::endl;
        return EXIT_FAILURE;
    }
    else if( argc < 4 )
    {
        std::cout << "Please give at least 3 arguments( point a, point b and number of trapezes )." 
                  << std::endl;
        return EXIT_FAILURE;
    }

    // else

    int my_rank, comm_sz;


    // Gettin a, b and n.
    double a = atoi(argv[1]);
    double b = atoi(argv[2]);
    long int n = atoi(argv[3]);

    // This var will be useful to calculate the point ranges of each proc
    double height = ( b - a ) / n;


    // Starting MPI ---------------------------------------------------------------------------------------
    MPI_Init(NULL, NULL);

    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);


    // Local vars( each procs )
    long int local_n = n/comm_sz; // Number of trapezes to each proc

    double local_a = a + my_rank * local_n * height; // Current point a of the proc number <my_rank>.

    double local_b = local_a + local_n * height; // Current point b of proc number <my_rank>.

    double local_integral; // local result
    

    // Time var -------------------------------------------------------------------------------------------
    double final_time = 0.0; // Final exec time


    // Final var ------------------------------------------------------------------------------------------
    double final_integral = 0.0; // Final result


    // ----------------------------------------------------------------------------------------------------
    // - START TIMER -
    std::chrono::steady_clock::time_point START = std::chrono::steady_clock::now();
    local_integral = trapezoidal_rule( local_a, local_b, local_n );
    std::chrono::steady_clock::time_point STOP = std::chrono::steady_clock::now();
    // - STOP TIMER -
    // ----------------------------------------------------------------------------------------------------
    

    // Aux local vars    
    auto timer = (STOP - START);

    double local_time = std::chrono::duration< double > (timer).count();

    auto local_pair = std::make_pair( local_integral, local_time );

    auto pair_sz = sizeof(local_pair);
    // ----------------------------------------------------------------------------------------------------


    // MPI COMMS
    if( my_rank != 0 )
    {
        // Send to root proc de result of local integration and exec time.
        MPI_Send( &local_pair, pair_sz, MPI_PACKED, 0, 0, MPI_COMM_WORLD );

    }
    else
    {
     
        final_integral = local_pair.first;

        final_time = local_pair.second;
            
        // Receiving all integrations and times.
        for( int proc = 1; proc < comm_sz; proc++ )
        {
            MPI_Recv( &local_pair, pair_sz, MPI_PACKED, proc, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
            final_integral += local_pair.first;

            // Getting the highest exec time
            if( final_time < local_pair.second )
            {
                final_time = local_pair.second;
            }
        }

        
        std::cout << std::fixed
                  << std::setprecision(3)
                  << "Number of trapezes = " 
                  << argv[3]
                  << " | AREA = " 
                  << final_integral 
                  << " | Exec time = "  
                  << final_time 
                  << "s |" << std::endl;
    
            

    }

    
    MPI_Finalize();

    return EXIT_SUCCESS;  

}
