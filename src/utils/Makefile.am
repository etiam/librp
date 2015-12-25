EXTRA_PROGRAMS           += catrib

catrib_SOURCES            = src/utils/catrib.cpp
catrib_LDFLAGS            = -Wl,-E

catrib_CPPFLAGS           =                                             \
                            -I$(top_builddir)                           \
                            -I$(top_builddir)/install/include           \
                            -I$(top_srcdir)/src/core                    \
                            @LIBUT_CFLAGS@                              \
                            @BOOST_CPPFLAGS@

catrib_LDADD              =                                             \
                            $(top_builddir)/librpcore.la                \
                            @LIBUT_LIBS@                                \
                            @ILMBASE_LIBS@                              \
                            @BOOST_LDFLAGS@                             \
                            @BOOST_IOSTREAMS_LIB@                       \
                            -lfl

extra:
	$(MAKE) catrib

CLEAN                      += catrib
