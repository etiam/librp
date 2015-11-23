#ifndef __SCANNER_HPP__
#define __SCANNER_HPP__

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "parser.hh"

namespace Rp
{

class Scanner : public yyFlexLexer
{
  public:
    Scanner(std::istream *in) : yyFlexLexer(in) {};

    int yylex(Parser::semantic_type *const lval)
    {
        yylval = lval;
        return (yylex ());
    }

    void setParser(Parser *parser)
    {
        m_parser = parser;
    }

  private:
    int yylex ();

    Parser::semantic_type * yylval = nullptr;
    Parser *                m_parser = nullptr;
};

}

#endif
