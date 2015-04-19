# Process this file with automake to produce Makefile.in

LIBSRC_PATH = $(top_srcdir)/libsrc

LIB_INCLUDE = $(top_srcdir)/libsrc/include

OS_SRC = $(top_srcdir)/libsrc/os/src
UTIL_SRC = $(top_srcdir)/libsrc/util/src

INCLUDES = -I$(LIB_INCLUDE)

lib_LTLIBRARIES = libeagle.la
libeagle_la_SOURCES = $(UTIL_SRC)/AutoPtr.cpp $(OS_SRC)/OsApi.cpp \
					  $(UTIL_SRC)/TimeStats.cpp
