EXTRA_PROGRAMS             += ribparse

ribparse_SOURCES            = src/utils/ribparse.cpp
ribparse_LDFLAGS            = -Wl,-E

ribparse_CPPFLAGS           =                                               \
                              -I$(top_builddir)                             \
                              -I$(top_builddir)/install/include

ribparse_LDADD              =                                               \
                              $(top_builddir)/install/lib/librpcore.la      \
                              -lfl
