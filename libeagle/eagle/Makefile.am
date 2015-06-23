# Process this file with automake to produce Makefile.in

CXXFLAGS = -g -O0 -pedantic -Wall -Werror -Wfloat-equal -Wshadow -Wno-invalid-offsetof -D_TEST_

LIB_INCLUDE = $(top_srcdir)/libeagle/include
OS_PATH = $(top_srcdir)/libeagle/os
UTIL_PATH = $(top_srcdir)/libeagle/util
EVENT_PATH = $(top_srcdir)/libeagle/event
NODE_PATH = $(top_srcdir)/libeagle/node
OPEN_PATH = $(top_srcdir)/libeagle/open

INCLUDES = -I$(LIB_INCLUDE) -I$(NODE_PATH)

lib_LTLIBRARIES = libeagle.la
libeagle_la_SOURCES = Eagle.cpp TaskThread.cpp SignalManager.cpp \
					  MasterSigManager.cpp ChildSigManager.cpp \
					  ProcessManager.cpp EagleTime.cpp Timer.cpp \
					  Proctitle.cpp
libeagle_la_LIBADD = $(OS_PATH)/libos.la $(UTIL_PATH)/libutil.la \
					 $(EVENT_PATH)/libevent.la $(NODE_PATH)/libnode.la \
					 $(OPEN_PATH)/libopen.la
