# Process this file with automake to produce Makefile.in

CXXFLAGS = -g -O0 -pedantic -Wall -Werror -Wfloat-equal -Wshadow -Wno-invalid-offsetof #-D_TEST_

LIB_INCLUDE = $(top_srcdir)/libeagle/include
OS_PATH = $(top_srcdir)/libeagle/os
UTIL_PATH = $(top_srcdir)/libeagle/util
EVENT_PATH = $(top_srcdir)/libeagle/event

INCLUDES = -I$(LIB_INCLUDE)

noinst_LTLIBRARIES = libevent.la
libevent_la_SOURCES = EpollManager.cpp EventHandler.cpp EventManager.cpp \
					  EventWorker.cpp SelectManager.cpp AcceptHandler.cpp \
					  ReceiveHandler.cpp MessageHandlerFactory.cpp
