# Process this file with automake to produce Makefile.in

LIB_INCLUDE = $(top_srcdir)/libsrc/include
OS_PATH = $(top_srcdir)/libsrc/os
UTIL_PATH = $(top_srcdir)/libsrc/util

INCLUDES = -I$(LIB_INCLUDE)

lib_LTLIBRARIES = libeagle.la
libeagle_la_SOURCES = Eagle.cpp
libeagle_la_LIBADD = $(OS_PATH)/libos.la $(UTIL_PATH)/libutil.la
