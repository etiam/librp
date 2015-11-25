EXTRA_PROGRAMS             += catrib

catrib_SOURCES              = src/utils/catrib.cpp
catrib_LDFLAGS              = -Wl,-E

catrib_CPPFLAGS             =                                               \
                              -I$(top_builddir)                             \
                              -I$(top_builddir)/install/include             \
                              -I$(top_srcdir)/src/core

catrib_LDADD                =                                               \
                              $(top_builddir)/librpcore.la                  \
                              -lfl

extra:
	$(MAKE) catrib

CLEAN                      += catrib