# Process this file with automake to produce Makefile.in

CXXFLAGS = -g -O0 -pedantic -Wall -Werror -Wfloat-equal -Wshadow

LIBSRC_PATH = $(top_srcdir)/libsrc

LIB_INCLUDE = $(top_srcdir)/libsrc/include

INCLUDES = -I$(LIB_INCLUDE)

noinst_LTLIBRARIES = libos.la
libos_la_SOURCES = FileEx.cpp ProcessSem.cpp ThreadSem.cpp Thread.cpp \
				   MutexLock.cpp ShareMem.cpp MmapFile.cpp SocketEx.cpp
libos_la_LDFLAGS  = -lrt
