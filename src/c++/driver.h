#ifndef __MCDRIVER_HPP__
#define __MCDRIVER_HPP__ 1

#include <string>
#include <cstdint>
#include "scanner.h"
#include "parser.hh"

namespace Rp
{

class Driver
{
  public:
   Driver() = default;

   virtual ~Driver();

   void parse( const char *filename );

   void add_upper();
   void add_lower();
   void add_word( const std::string &word );
   void add_newline();
   void add_char();

   std::ostream& print(std::ostream &stream);

private:
   int chars      = 0;
   int words      = 0;
   int lines      = 0;
   int uppercase  = 0;
   int lowercase  = 0;
   Parser  *parser  = nullptr;
   Scanner *scanner = nullptr;
};

}

#endif
