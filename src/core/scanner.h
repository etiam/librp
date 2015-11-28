#ifndef __SCANNER_HPP__
#define __SCANNER_HPP__

#include <istream>
#include <fstream>
#include <memory>

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "parser.hh"

namespace Rp
{

class Scanner : public yyFlexLexer
{
  public:
    Scanner(const std::string &filename) : m_filename(filename)
    {
        if(filename.empty())
        {
            m_input.reset(&std::cin, [](...){});
            m_filename = "stdin";
        }
        else
        {
            auto ifstream = new std::ifstream(filename);
            if (!ifstream->good())
                throw std::runtime_error(filename + ": No such file or directory");
            m_input.reset(ifstream);
        }

        yyrestart(m_input.get());
    };

    int yylex(Parser::semantic_type *const lval)
    {
        yylval = lval;
        return yylex();
    }

    void setParser(Parser &parser)
    {
        m_parser = &parser;
    }

    std::string filename() const
    {
        return m_filename;
    }

  private:
    int yylex();

    std::string                     m_filename;
    std::shared_ptr<std::istream>   m_input;

    Parser::semantic_type *         yylval = nullptr;
    Parser *                        m_parser = nullptr;
};

}

#endif
