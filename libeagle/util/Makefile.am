# Process this file with automake to produce Makefile.in

CXXFLAGS = -g -O0 -pedantic -Wall -Werror -Wfloat-equal -Wshadow 

libeagle_PATH = $(top_srcdir)/libeagle
LIB_INCLUDE = $(top_srcdir)/libeagle/include

INCLUDES = -I$(LIB_INCLUDE) -I$(UTIL_INCLUDE)

noinst_LTLIBRARIES = libutil.la
libutil_la_SOURCES = TimeStats.cpp Singleton.cpp Log.cpp ClassFactory.cpp 
