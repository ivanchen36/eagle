# Process this file with automake to produce Makefile.in
AUTOMAKE_OPTIONS = foreign

TEST_INCLUDE = $(top_srcdir)/test
TEST_SRC = $(top_srcdir)/test
LIB_PATH = $(top_srcdir)/libsrc/eagle/.libs
TEST_LD = -L$(LIB_PATH) -leagle -ltcmalloc
LIB_INCLUDE = $(top_srcdir)/libsrc/include

noinst_PROGRAMS = test autoptrtest timestatstest singletontest \
				  logtest

INCLUDES = -I$(TEST_INCLUDE) -I$(LIB_INCLUDE)
test_SOURCES = Test.cpp
test_LDFLAGS = $(TEST_LD)

autoptrtest_SOURCES = AutoPtrTest.cpp
autoptrtest_LDFLAGS = $(TEST_LD)

timestatstest_SOURCES = TimeStatsTest.cpp
timestatstest_LDFLAGS = $(TEST_LD)

singletontest_SOURCES = SingletonTest.cpp
singletontest_LDFLAGS = $(TEST_LD)

logtest_SOURCES = LogTest.cpp
logtest_LDFLAGS = $(TEST_LD)
