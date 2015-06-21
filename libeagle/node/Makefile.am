# Process this file with automake to produce Makefile.in

CXXFLAGS = -g -O0 -pedantic -Wall -Werror -Wfloat-equal -Wshadow 

LIB_INCLUDE = $(top_srcdir)/libeagle/include

INCLUDES = -I$(LIB_INCLUDE)

noinst_LTLIBRARIES = libnode.la
libnode_la_SOURCES = Follower.cpp Leader.cpp PropertiesParser.cpp \
					 EagleNode.cpp
