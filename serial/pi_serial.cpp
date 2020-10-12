#include <iostream>
#include <chrono>
#include <iomanip>
#include <random>

double generate_number()
{
      std::random_device seed;

      std::mt19937 gen( seed() );

      std::uniform_real_distribution<double> distr(0.0, 1.0);

      return distr( gen );
}


double monteCarlo(int n) 
{

    int correct_hits = 0;

    for( int i; i < n; i++ ) 
    {
    
        double x = generate_number();
        double y = generate_number();

        if( ( ( x * x ) + ( y * y ) ) < 1 ) 
        {
        
            correct_hits++;
        }
    }

    return (double) ( 4.0 * correct_hits / n );
}


int main( int argc, char *argv[] ) 
{
    double arithmetic_mean = 0.0;

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
    
    short int testCount = 0;

    while( testCount < 5 )
    {
        // - START TIMER -
        std::chrono::steady_clock::time_point START = std::chrono::steady_clock::now();
        double pi = monteCarlo(atoi(argv[1]));
        std::chrono::steady_clock::time_point STOP = std::chrono::steady_clock::now();
        // - STOP TIMER -

        auto timer = (STOP - START);

        // Calculating progressive mean.
        arithmetic_mean = arithmetic_mean + ( ( std::chrono::duration< double > (timer).count() - arithmetic_mean )/(testCount+1) );

        // Current execution time
        double exec_time = std::chrono::duration< double > (timer).count(); 
        
        std::cout << "N = " 
                  << argv[1] 
                  << " | PI = " 
                  << pi 
                  << " | Exec time(" 
                  << testCount+1 
                  << ")= " 
                  << exec_time 
                  << "s |" << std::endl;
        
        testCount++;

    }

    std::cout << "\n\nArithmetic mean of the time = " << arithmetic_mean << "s" << std::endl;

    return EXIT_SUCCESS;

}


