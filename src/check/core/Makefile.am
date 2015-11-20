check_PROGRAMS           +=                                               \
                            nametest                                      \
                            attrtest                                      \
                            relationshiptest                              \
                            factorytest                                   \
                            eventtest

TESTS                    += $(check_PROGRAMS)

CHECK_PROGRAMS_CPPFLAGS   =                                               \
                            -I$(top_srcdir)/src                           \
                            @LIBUT_CFLAGS@                                \
                            @LIBLOG_CFLAGS@                               \
                            @ILMBASE_CFLAGS@                              \
                            @BOOST_CPPFLAGS@

CHECK_PROGRAMS_LDADD      =                                               \
                            $(top_builddir)/libsgcore.la                  \
                            @LIBUT_LIBS@                                  \
                            @ILMBASE_LIBS@                                \
                            @BOOST_REGEX_LDFLAGS@                         \
                            @BOOST_REGEX_LIBS@                            \
                            @BOOST_SYSTEM_LDFLAGS@                        \
                            @BOOST_SYSTEM_LIBS@                           \
                            @BOOST_THREAD_LDFLAGS@                        \
                            @BOOST_THREAD_LIBS@                           \
                            @BOOST_LOG_LDFLAGS@                           \
                            @BOOST_LOG_LIBS@                              \
                            @BOOST_IOSTREAMS_LDFLAGS@                     \
                            @BOOST_IOSTREAMS_LIBS@                        \
                            @BOOST_UNIT_TEST_FRAMEWORK_LDFLAGS@           \
                            @BOOST_UNIT_TEST_FRAMEWORK_LIBS@

nametest_SOURCES          = src/check/core/nametest.cpp
nametest_CPPFLAGS         = $(CHECK_PROGRAMS_CPPFLAGS)
nametest_LDADD            = $(CHECK_PROGRAMS_LDADD)
nametest_LDFLAGS          = $(CHECK_PROGRAMS_LDFLAGS)

attrtest_SOURCES          = src/check/core/attrtest.cpp
attrtest_CPPFLAGS         = $(CHECK_PROGRAMS_CPPFLAGS)
attrtest_LDADD            = $(CHECK_PROGRAMS_LDADD)
attrtest_LDFLAGS          = $(CHECK_PROGRAMS_LDFLAGS)

relationshiptest_SOURCES  = src/check/core/relationshiptest.cpp
relationshiptest_CPPFLAGS = $(CHECK_PROGRAMS_CPPFLAGS)
relationshiptest_LDADD    = $(CHECK_PROGRAMS_LDADD)
relationshiptest_LDFLAGS  = $(CHECK_PROGRAMS_LDFLAGS)
 
factorytest_SOURCES       = src/check/core/factorytest.cpp
factorytest_CPPFLAGS      = $(CHECK_PROGRAMS_CPPFLAGS)
factorytest_LDADD         = $(CHECK_PROGRAMS_LDADD)
factorytest_LDFLAGS       = $(CHECK_PROGRAMS_LDFLAGS)

eventtest_SOURCES         = src/check/core/eventtest.cpp
eventtest_CPPFLAGS        = $(CHECK_PROGRAMS_CPPFLAGS)
eventtest_LDADD           = $(CHECK_PROGRAMS_LDADD)
eventtest_LDFLAGS         = $(CHECK_PROGRAMS_LDFLAGS)
