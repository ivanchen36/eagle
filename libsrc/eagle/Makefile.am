# Process this file with automake to produce Makefile.in

CXXFLAGS = -g -O0 -pedantic -Wall -Werror -Wfloat-equal -Wshadow -Wno-invalid-offsetof #-D_TEST_

LIB_INCLUDE = $(top_srcdir)/libsrc/include
OS_PATH = $(top_srcdir)/libsrc/os
UTIL_PATH = $(top_srcdir)/libsrc/util

INCLUDES = -I$(LIB_INCLUDE) -I.

lib_LTLIBRARIES = libeagle.la
libeagle_la_SOURCES = Eagle.cpp TaskThread.cpp SignalManager.cpp \
					  MasterSigManager.cpp ChildSigManager.cpp \
					  ProcessManager.cpp EagleTime.cpp Timer.cpp \
					  EpollManager.cpp EventHandler.cpp EventManager.cpp \
					  EventWorker.cpp SelectManager.cpp AcceptHandler.cpp \
					  ReceiveHandler.cpp MessageHandlerFactory.cpp
libeagle_la_LIBADD = $(OS_PATH)/libos.la $(UTIL_PATH)/libutil.la
