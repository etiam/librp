EXTRA_PROGRAMS             += plugindump

plugindump_SOURCES          = src/utils/plugindump.cpp
plugindump_LDFLAGS          = -Wl,-E

plugindump_CPPFLAGS         =                                               \
                              -I$(top_builddir)                             \
                              -I$(top_srcdir)                               \
                              -I$(top_srcdir)/src                           \
                              -I$(top_srcdir)/lib                           \
                              @ILMBASE_CFLAGS@                              \
                              @BOOST_CPPFLAGS@

plugindump_LDADD            =                                               \
                              $(top_builddir)/src/core/libsgcore.la         \
                              @BOOST_SYSTEM_LDFLAGS@                        \
                              @BOOST_SYSTEM_LIBS@                           \
                              @BOOST_THREAD_LDFLAGS@                        \
                              @BOOST_THREAD_LIBS@                           \
                              @BOOST_LOG_LDFLAGS@                           \
                              @BOOST_LOG_LIBS@                              \
                              @ILMBASE_LIBS@                                \
                              -ldl
