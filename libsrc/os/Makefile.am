# Process this file with automake to produce Makefile.in

LIBSRC_PATH = $(top_srcdir)/libsrc

LIB_INCLUDE = $(top_srcdir)/libsrc/include

INCLUDES = -I$(LIB_INCLUDE)

noinst_LTLIBRARIES = libos.la
libos_la_SOURCES = OsApi.cpp FileEx.cpp ProcessSem.cpp ThreadSem.cpp Thread.cpp MutexLock.cpp
libos_la_LDFLAGS  = -lrt
