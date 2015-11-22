%skeleton "lalr1.cc"
%require  "3.0"
%debug 
%defines 
%define api.namespace {Rp}
%define parser_class_name {Parser}

%code requires
{
   namespace Rp 
   {
      class Driver;
      class Scanner;
   }
}

%parse-param { Scanner  &scanner  }
%parse-param { Driver  &driver  }

%code{
   #include <iostream>
   #include <cstdlib>
   #include <fstream>
   
   /* include for all driver functions */
   #include "driver.h"

#undef yylex
#define yylex scanner.yylex
}

/* token types */
%union {
   std::string *sval;
}

%token            END    0     "end of file"
%token            UPPER
%token            LOWER
%token   <sval>   WORD
%token            NEWLINE
%token            CHAR


/* destructor rule for <sval> objects */
%destructor { if ($$)  { delete ($$); ($$) = nullptr; } } <sval>


%%

list_option : END | list END;

list
  : item
  | list item
  ;

item
  : UPPER   { driver.add_upper(); }
  | LOWER   { driver.add_lower(); }
  | WORD    { driver.add_word( *$1 ); }
  | NEWLINE { driver.add_newline(); }
  | CHAR    { driver.add_char(); }
  ;

%%


void 
Rp::Parser::error(const std::string &err_message)
{
   std::cerr << "Error: " << err_message << "\n"; 
}
