# Process this file with automake to produce Makefile.in

CXXFLAGS = -g -O0

LIBSRC_PATH = $(top_srcdir)/libsrc
LIB_INCLUDE = $(top_srcdir)/libsrc/include

INCLUDES = -I$(LIB_INCLUDE) -I$(UTIL_INCLUDE)

noinst_LTLIBRARIES = libutil.la
libutil_la_SOURCES = TimeStats.cpp Singleton.cpp Log.cpp
