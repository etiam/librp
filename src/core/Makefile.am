lib_LTLIBRARIES          += librpcore.la

librpcore_la_SOURCES      =                                             \
                              src/core/parser.yy                        \
                              src/core/lexer.lpp                        \
                              src/core/driver.cpp

                              
librpcoreinclude_HEADERS  =                                             \
                              src/core/driver.h

librpcoreincludedir       = $(includedir)/rp

librpcore_la_CPPFLAGS     =                                             \
                              -I$(top_builddir)                         \
                              -I$(top_builddir)/src/core                \
                              -I$(top_srcdir)                           \
                              -I$(top_srcdir)/src/core                  \
                              @LIBUT_CFLAGS@

# lexer.lpp does an "#include "parser.hh" (built from parser.yy)
#src/core/lexer.cpp: src/core/parser.hh

.lpp.cpp:
	$(AM_V_LEX)$(am__skiplex) $(SHELL) $(YLWRAP) $< $(LEX_OUTPUT_ROOT).c $@ -- $(LEXCOMPILE) -o $(notdir $@)
	$(am__mv) $(notdir $@) $(top_builddir)/$@

CLEAN                      += stack.hh                                  \
                              src/core/parser.cc                        \
                              src/core/parser.hh                        \
                              src/core/parser.output                    \
                              src/core/lexer.cpp
