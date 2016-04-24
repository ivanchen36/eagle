# Process this file with automake to produce Makefile.in

CXXFLAGS = -g -O0 -pedantic -Wall -Werror -Wfloat-equal -Wshadow -Wno-invalid-offsetof

LIB_INCLUDE = $(top_srcdir)/libeagle/include
OS_PATH = $(top_srcdir)/libeagle/os
UTIL_PATH = $(top_srcdir)/libeagle/util
EVENT_PATH = $(top_srcdir)/libeagle/event
NODE_PATH = $(top_srcdir)/libeagle/node
OPEN_PATH = $(top_srcdir)/libeagle/open
PROTO_PATH = $(top_srcdir)/libeagle/proto

INCLUDES = -I$(LIB_INCLUDE) -I$(NODE_PATH) -I$(EVENT_PATH) -I$(NODE_PATH)

libeagledir = $(includedir)/
libeagle_HEADERS = $(top_srcdir)/libeagle/include/AutoPtr.h \
				   $(top_srcdir)/libeagle/include/CallBack.h \
				   $(top_srcdir)/libeagle/include/ClassFactory.h \
				   $(top_srcdir)/libeagle/include/CycleQueue.h \
				   $(top_srcdir)/libeagle/include/Define.h \
				   $(top_srcdir)/libeagle/include/Eagle.h \
				   $(top_srcdir)/libeagle/include/ServerTime.h \
				   $(top_srcdir)/libeagle/include/EventHandler.h \
				   $(top_srcdir)/libeagle/include/EventManager.h \
				   $(top_srcdir)/libeagle/include/EventWorker.h \
				   $(top_srcdir)/libeagle/include/FileEx.h \
				   $(top_srcdir)/libeagle/include/IoBuffer.h \
				   $(top_srcdir)/libeagle/include/Log.h \
				   $(top_srcdir)/libeagle/include/MmapFile.h \
				   $(top_srcdir)/libeagle/include/MutexLock.h \
				   $(top_srcdir)/libeagle/include/Pool.h \
				   $(top_srcdir)/libeagle/include/ProcessSem.h \
				   $(top_srcdir)/libeagle/include/Properties.h \
				   $(top_srcdir)/libeagle/include/RbTree.h \
				   $(top_srcdir)/libeagle/include/SemEx.h \
				   $(top_srcdir)/libeagle/include/ShareMem.h \
				   $(top_srcdir)/libeagle/include/ShmAllocator.h \
				   $(top_srcdir)/libeagle/include/Singleton.h \
				   $(top_srcdir)/libeagle/include/SocketEx.h \
				   $(top_srcdir)/libeagle/include/StrUtil.h \
				   $(top_srcdir)/libeagle/include/TaskThread.h \
				   $(top_srcdir)/libeagle/include/Thread.h \
				   $(top_srcdir)/libeagle/include/ThreadSem.h \
				   $(top_srcdir)/libeagle/include/Timer.h \
				   $(top_srcdir)/libeagle/include/TimeStats.h \
				   $(top_srcdir)/libeagle/include/tinyxml2.h

lib_LTLIBRARIES = libeagle.la

libeagle_la_SOURCES = Eagle.cpp TaskThread.cpp SignalManager.cpp \
					  MasterSigManager.cpp ChildSigManager.cpp \
					  ProcessManager.cpp Timer.cpp Proctitle.cpp \
					  ServerTime.cpp
libeagle_la_LIBADD = $(OS_PATH)/libos.la $(UTIL_PATH)/libutil.la \
					 $(EVENT_PATH)/libevent.la $(NODE_PATH)/libnode.la \
					 $(OPEN_PATH)/libopen.la $(PROTO_PATH)/libproto.la
libeagle_la_LDFLAGS = -version-info  1:0:1 --export-dynamic -no-undefined
