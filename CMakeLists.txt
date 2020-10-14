cmake_minimum_required(VERSION 3.5)
project( MPI1 VERSION 0.1 LANGUAGES CXX)


#=== FIND PACKAGES===#


# set the compiling flags
set ( CMAKE_CXX_FLAGS "-g -Wall")
#CMAKE_CXX_FLAGS = "-Wall";

# define the sources for the project
add_executable ( pi_serial
				 serial/pi_serial.cpp ) #this is the application for pi

add_executable( trp_serial
                serial/trp_serial.cpp ) #this is the application trp rule

# define C++11 standard
set_property (TARGET pi_serial PROPERTY CXX_STANDARD 11)
set_property (TARGET trp_serial PROPERTY CXX_STANDARD 11)

# the end
