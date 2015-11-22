#ifndef __MCSCANNER_HPP__
#define __MCSCANNER_HPP__ 1

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "parser.hh"

namespace Rp
{

class Scanner : public yyFlexLexer
{
  public:

    Scanner (std::istream *in) :
        yyFlexLexer (in), yylval (nullptr) {};

    int yylex (Parser::semantic_type * const lval)
    {
        yylval = lval;
        return (yylex ());
    }

  private:
    int yylex ();

    Parser::semantic_type *yylval;
};

}

#endif
