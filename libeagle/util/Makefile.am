# Process this file with automake to produce Makefile.in

CXXFLAGS = -g -O0 -pedantic -Wall -Werror -Wfloat-equal -Wshadow 

LIB_INCLUDE = $(top_srcdir)/libeagle/include

INCLUDES = -I$(LIB_INCLUDE)

noinst_LTLIBRARIES = libutil.la
libutil_la_SOURCES = TimeStats.cpp Log.cpp ClassFactory.cpp \
					 StrUtil.cpp ServerReport.cpp Shell.cpp 
