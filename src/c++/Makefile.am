lib_LTLIBRARIES            += librpcorenew.la

librpcorenew_la_SOURCES     =                                   \
                              src/c++/parser.yy                 \
                              src/c++/lexer.lpp                 \
                              src/c++/driver.cpp

                              
librpcorenewinclude_HEADERS =                                   \
                              src/c++/driver.h

librpcorenewincludedir      = $(includedir)/rp

librpcorenew_la_CPPFLAGS    =                                   \
                              -I$(top_builddir)                 \
                              -I$(top_builddir)/src/c++         \
                              -I$(top_srcdir)                   \
                              -I$(top_srcdir)/src/c++           \
                              @LIBLOG_CFLAGS@

# lexer.lpp does an "#include "parser.hh" (built from parser.yy)
#src/c++/lexer.cpp: src/c++/parser.hh

.lpp.cpp:
	$(AM_V_LEX)$(am__skiplex) $(SHELL) $(YLWRAP) $< $(LEX_OUTPUT_ROOT).c $@ -- $(LEXCOMPILE) -o $(notdir $@)
	$(am__mv) $(notdir $@) $(top_builddir)/$@

#clean-local:
#	-rm -f src/c++/* test stack.h

CLEAN                      += stack.hh                          \
                              src/c++/parser.cc                 \
                              src/c++/parser.hh                 \
                              src/c++/parser.output             \
                              src/c++/lexer.cpp