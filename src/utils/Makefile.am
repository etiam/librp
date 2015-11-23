EXTRA_PROGRAMS             += parserib_old

parserib_old_SOURCES        = src/utils/parserib_old.cpp
parserib_old_LDFLAGS        = -Wl,-E

parserib_old_CPPFLAGS       =                                               \
                              -I$(top_builddir)                             \
                              -I$(top_builddir)/install/include

parserib_old_LDADD          =                                               \
                              $(top_builddir)/librpcore.la                  \
                              -lfl

EXTRA_PROGRAMS             += parserib_new

parserib_new_SOURCES        = src/utils/parserib_new.cpp
parserib_new_LDFLAGS        = -Wl,-E

parserib_new_CPPFLAGS       =                                               \
                              -I$(top_builddir)                             \
                              -I$(top_builddir)/install/include

parserib_new_LDADD          =                                               \
                              $(top_builddir)/librpcorenew.la               \
                              -lfl
                              
EXTRA_PROGRAMS             += catrib

catrib_SOURCES              = src/utils/catrib.cpp
catrib_LDFLAGS              = -Wl,-E

catrib_CPPFLAGS             =                                               \
                              -I$(top_builddir)                             \
                              -I$(top_builddir)/install/include             \
                              -I$(top_srcdir)/src/c++

catrib_LDADD                =                                               \
                              $(top_builddir)/librpcorenew.la               \
                              -lfl

extra:
	$(MAKE) parserib_old parserib_new catrib

CLEAN                      += parserib_old parserib_new catrib