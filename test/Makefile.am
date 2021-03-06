# Process this file with automake to produce Makefile.in
AUTOMAKE_OPTIONS = foreign

CXXFLAGS = -g -O0

TEST_INCLUDE = $(top_srcdir)/test
TEST_SRC = $(top_srcdir)/test
LIB_PATH = $(top_srcdir)/libeagle/frame/.libs
TEST_LD = -L$(LIB_PATH) -leagle #-ltcmalloc
LIB_INCLUDE = $(top_srcdir)/libeagle/include
LIBEAGLE_INCLUDE = $(top_srcdir)/libeagle/frame
LIBEVENT_INCLUDE = $(top_srcdir)/libeagle/event
LIBNODE_INCLUDE = $(top_srcdir)/libeagle/node
LIBPROTO_INCLUDE = $(top_srcdir)/libeagle/proto

noinst_PROGRAMS = autoptrtest timestatstest singletontest \
				  logtest psemtest threadtest shmtest mmaptest \
				  eagletest rbtest locktest sockettest queuetest \
				  classtest eventtest proptest alloctest prototest \
				  chtest reporttest sigtest

INCLUDES = -I$(TEST_INCLUDE) -I$(LIB_INCLUDE) -I$(LIBEAGLE_INCLUDE) \
		   -I$(LIBEVENT_INCLUDE) -I$(LIBNODE_INCLUDE) -I$(LIBPROTO_INCLUDE)

autoptrtest_SOURCES = AutoPtrTest.cpp
autoptrtest_LDFLAGS = $(TEST_LD)

timestatstest_SOURCES = TimeStatsTest.cpp
timestatstest_LDFLAGS = $(TEST_LD)

singletontest_SOURCES = SingletonTest.cpp
singletontest_LDFLAGS = $(TEST_LD)

logtest_SOURCES = LogTest.cpp
logtest_LDFLAGS = $(TEST_LD)

psemtest_SOURCES = PSemTest.cpp
psemtest_LDFLAGS = $(TEST_LD)

threadtest_SOURCES = ThreadTest.cpp
threadtest_LDFLAGS = $(TEST_LD)

shmtest_SOURCES = ShmTest.cpp
shmtest_LDFLAGS = $(TEST_LD)

mmaptest_SOURCES = MmapTest.cpp
mmaptest_LDFLAGS = $(TEST_LD)

eagletest_SOURCES = EagleTest.cpp
eagletest_LDFLAGS = $(TEST_LD)

rbtest_SOURCES = RbTest.cpp
rbtest_LDFLAGS = $(TEST_LD)

locktest_SOURCES = LockTest.cpp
locktest_LDFLAGS = $(TEST_LD)

sockettest_SOURCES = SocketTest.cpp
sockettest_LDFLAGS = $(TEST_LD)

queuetest_SOURCES = CyQueueTest.cpp
queuetest_LDFLAGS = $(TEST_LD)

classtest_SOURCES = ClassTest.cpp
classtest_LDFLAGS = $(TEST_LD)

eventtest_SOURCES = EventTest.cpp
eventtest_LDFLAGS = $(TEST_LD)

proptest_SOURCES = PropertiesTest.cpp
proptest_LDFLAGS = $(TEST_LD)

alloctest_SOURCES = ShmAllocatorTest.cpp
alloctest_LDFLAGS = $(TEST_LD)

prototest_SOURCES = ProtoTest.cpp
prototest_LDFLAGS = $(TEST_LD)

chtest_SOURCES = CHTest.cpp
chtest_LDFLAGS = $(TEST_LD)

reporttest_SOURCES = ReportTest.cpp
reporttest_LDFLAGS = $(TEST_LD)

sigtest_SOURCES = SigTest.cpp
sigtest_LDFLAGS = $(TEST_LD)
