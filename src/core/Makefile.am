lib_LTLIBRARIES            += librpcore.la

librpcore_la_SOURCES        =                                   \
                              src/core/parse.cpp                \
                              src/core/rr.cpp                   \
                              src/core/yacc.ypp                 \
                              src/core/lex.lpp
                              
librpcoreinclude_HEADERS    =                                   \
                              src/core/parse.h

librpcore_la_LDFLAGS        = -version-info @LIBRARY_VERSION@ -no-undefined

librpcoreincludedir         = $(includedir)/rp

librpcore_la_CPPFLAGS       =                                   \
                              -I$(top_builddir)                 \
                              -I$(top_srcdir)                   \
                              -I$(top_srcdir)/src/core          \
                              @LIBLOG_CFLAGS@
                              
CLEAN                      += stack.hh                          \
                              src/core/yacc.cpp                 \
                              src/core/yacc.hpp                 \
                              src/core/lex.cpp