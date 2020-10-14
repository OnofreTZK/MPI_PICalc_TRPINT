#include <iostream>
#include <math.h>
#include <random>
#include <chrono>
#include <iomanip>

// Algorithm 2
double trapezoidal_rule(double a, double b, long int n)
{

    double x_i; //x1
	double h = (b - a) / n;
	double total = (sin(a) + sin(b)) / 2;

    // Loop for the sum of y1 to yn
	for ( long int i = 1; i < n; i++)
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
    
    short int testCount = 0;

    double arithmetic_mean = 0.0;
    
    while( testCount < 5 )
    {
        // - START TIMER -
        std::chrono::steady_clock::time_point START = std::chrono::steady_clock::now();
        double final_area = trapezoidal_rule( atoi(argv[1]), atoi(argv[2]), atol(argv[3]) );
        std::chrono::steady_clock::time_point STOP = std::chrono::steady_clock::now();
        // - STOP TIMER -

        auto timer = (STOP - START);

        // Calculating progressive mean.
        arithmetic_mean = arithmetic_mean + ( ( std::chrono::duration< double > (timer).count() - arithmetic_mean )/(testCount+1) );

        // Current execution time
        double exec_time = std::chrono::duration< double > (timer).count(); 
        
        std::cout << std::fixed
                  << std::setprecision(3)
                  << "Number of trapezes = " 
                  << argv[3]
                  << " | AREA = " 
                  << final_area 
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
