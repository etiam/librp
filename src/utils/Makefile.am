EXTRA_PROGRAMS           += catrib

catrib_SOURCES            = src/utils/catrib.cpp
catrib_LDFLAGS            = -Wl,-E

catrib_CPPFLAGS           =                                             \
                            -I$(top_builddir)                           \
                            -I$(top_builddir)/install/include           \
                            -I$(top_srcdir)/src/core                    \
                            @LIBUT_CFLAGS@

catrib_LDADD              =                                             \
                            $(top_builddir)/librpcore.la                \
                            @LIBUT_LIBS@                                \
                            @ILMBASE_LIBS@                              \
                            @BOOST_IOSTREAMS_LDFLAGS@                   \
                            @BOOST_IOSTREAMS_LIBS@                      \
                            -lfl

extra:
	$(MAKE) catrib

CLEAN                      += catrib
