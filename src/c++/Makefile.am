EXTRA_PROGRAMS             += test 

test_SOURCES                = src/c++/main.cpp

test_CPPFLAGS               =                                   \
                              -I$(top_builddir)                 \
                              -I$(top_builddir)/src/c++

test_LDADD                  = librpcorenew.a

noinst_LIBRARIES            = librpcorenew.a

librpcorenew_a_SOURCES      =                                   \
                              src/c++/mc_parser.yy              \
                              src/c++/mc_lexer.lpp              \
                              src/c++/mc_driver.cpp

                              
#librpcorenewinclude_HEADERS =                                   \
#                              src/c++/parse.h

#librpcorenew_a_LDFLAGS      = -version-info @LIBRARY_VERSION@ -no-undefined

librpcorenewincludedir      = $(includedir)/rp

librpcorenew_a_CPPFLAGS     =                                   \
                              -I$(top_builddir)                 \
                              -I$(top_builddir)/src/c++         \
                              -I$(top_srcdir)                   \
                              -I$(top_srcdir)/src/c++           \
                              @LIBLOG_CFLAGS@

# lexer.lpp does an "#include "mc_parser.hh" (built from mc_parser.yy)
src/c++/mc_lexer.cpp: src/c++/mc_parser.hh

.lpp.cpp:
	$(AM_V_LEX)$(am__skiplex) $(SHELL) $(YLWRAP) $< $(LEX_OUTPUT_ROOT).c $@ -- $(LEXCOMPILE) -o $(notdir $@)
	$(am__mv) $(notdir $@) $(top_builddir)/$@

clean-local:
	-rm -f src/c++/* test stack.hh
