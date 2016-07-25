EXTRA_PROGRAMS           += catrib rib2obj

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
                            @BOOST_REGEX_LIB@                           \
                            @BOOST_FILESYSTEM_LIB@                      \
                            @BOOST_SYSTEM_LIB@                          \
                            @BOOST_PROGRAM_OPTIONS_LIB@                 \
                            -lfl

rib2obj_SOURCES            = src/utils/rib2obj.cpp
rib2obj_LDFLAGS            = -Wl,-E

rib2obj_CPPFLAGS           =                                            \
                            -I$(top_builddir)                           \
                            -I$(top_builddir)/install/include           \
                            -I$(top_srcdir)/src/core                    \
                            @LIBUT_CFLAGS@                              \
                            @BOOST_CPPFLAGS@

rib2obj_LDADD              =                                            \
                            $(top_builddir)/librpcore.la                \
                            @LIBUT_LIBS@                                \
                            @ILMBASE_LIBS@                              \
                            @BOOST_LDFLAGS@                             \
                            @BOOST_IOSTREAMS_LIB@                       \
                            @BOOST_REGEX_LIB@                           \
                            @BOOST_FILESYSTEM_LIB@                      \
                            @BOOST_SYSTEM_LIB@                          \
                            @BOOST_PROGRAM_OPTIONS_LIB@                 \
                            -lfl

extra:
	$(MAKE) catrib rib2obj

CLEAN                      += catrib rib2obj
