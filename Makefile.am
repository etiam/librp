ACLOCAL_AMFLAGS           = -I m4 -I /usr/local/share/aclocal

lib_LTLIBRARIES           =
check_PROGRAMS            =
noinst_PROGRAMS           =
TESTS					  =
EXTRA_DIST                =
EXTRA_PROGRAMS            =
CLEAN                     =
EXTRACLEAN                =

include src/core/Makefile.am
include src/check/core/Makefile.am
include src/utils/Makefile.am

# PKGCONFIG SUPPORT
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = librp.pc

# DOXYGEN SUPPORT
include $(top_srcdir)/doxygen.am
MOSTLYCLEANFILES          = $(DX_CLEANFILES)
EXTRA_DIST               += $(DX_CONFIG)

include $(top_srcdir)/extra.am

clean-local:
	-rm -f $(CLEAN)
