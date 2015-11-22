#include <iostream>
#include <cstdlib>

#include "driver.h"

int 
main( const int argc, const char **argv )
{
   if(argc != 2 ) 
      return ( EXIT_FAILURE );
   Rp::Driver driver;
   
   driver.parse( argv[1] );
   
   std::cout << "Results\n";
   
   driver.print(std::cout) << "\n";

   return( EXIT_SUCCESS );
}
