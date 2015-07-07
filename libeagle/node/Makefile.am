# Process this file with automake to produce Makefile.in

CXXFLAGS = -g -O0 -pedantic -Wall -Werror -Wfloat-equal -Wshadow 

LIB_INCLUDE = $(top_srcdir)/libeagle/include
EAGLE_PATH = $(top_srcdir)/libeagle/eagle
EVENT_PATH = $(top_srcdir)/libeagle/event

INCLUDES = -I$(LIB_INCLUDE) -I$(EAGLE_PATH) -I$(EVENT_PATH)

noinst_LTLIBRARIES = libnode.la
libnode_la_SOURCES = Follower.cpp Leader.cpp PropertiesParser.cpp \
					 EagleNode.cpp NodeHandler.cpp NodeClient.cpp \
					 NodeServer.cpp
