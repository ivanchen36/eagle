# Process this file with automake to produce Makefile.in
AUTOMAKE_OPTIONS = foreign

TEST_INCLUDE = $(top_srcdir)/test
TEST_SRC = $(top_srcdir)/test
LIB_PATH = $(top_srcdir)/libsrc/eagle/.libs
TEST_LD = -L$(LIB_PATH) -leagle -ltcmalloc
LIB_INCLUDE = $(top_srcdir)/libsrc/include

noinst_PROGRAMS = autoptrtest timestatstest singletontest \
				  logtest psemtest threadtest

INCLUDES = -I$(TEST_INCLUDE) -I$(LIB_INCLUDE)

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
