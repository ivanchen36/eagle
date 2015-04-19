# Process this file with automake to produce Makefile.in
AUTOMAKE_OPTIONS = foreign

TEST_INCLUDE = $(top_srcdir)/test
TEST_SRC = $(top_srcdir)/test
LIB_PATH = $(top_srcdir)/libsrc/.libs
TEST_LD = -L$(LIB_PATH) -leagle -ltcmalloc
LIB_INCLUDE = $(top_srcdir)/libsrc/include

noinst_PROGRAMS = autoptrtest timestatstest

INCLUDES = -I$(TEST_INCLUDE) -I$(LIB_INCLUDE)
autoptrtest_SOURCES = $(TEST_SRC)/AutoPtrTest.cpp
autoptrtest_LDFLAGS = $(TEST_LD)

timestatstest_SOURCES = $(TEST_SRC)/TimeStatsTest.cpp
timestatstest_LDFLAGS = $(TEST_LD)
