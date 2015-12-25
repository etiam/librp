check_PROGRAMS           +=                                               \
                            parsetest

TESTS                    += $(check_PROGRAMS)

CHECK_PROGRAMS_CPPFLAGS   =                                               \
                            -I$(top_srcdir)/src                           \
                            @LIBUT_CFLAGS@                                \
                            @BOOST_CPPFLAGS@

CHECK_PROGRAMS_LDADD      =                                               \
                            $(top_builddir)/librpcore.la                  \
                            @BOOST_LDFLAGS@                               \
                            @BOOST_UNIT_TEST_FRAMEWORK_LIB@

parsetest_SOURCES         = src/check/core/parsetest.cpp
parsetest_CPPFLAGS        = $(CHECK_PROGRAMS_CPPFLAGS)
parsetest_LDADD           = $(CHECK_PROGRAMS_LDADD)
parsetest_LDFLAGS         = $(CHECK_PROGRAMS_LDFLAGS)
