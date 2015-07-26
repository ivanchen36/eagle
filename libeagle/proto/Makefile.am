# Process this file with automake to produce Makefile.in

CXXFLAGS = -g -O0 -pedantic -Wall -Werror -Wfloat-equal

CLEANFILES = $(proto_outputs)

LIB_INCLUDE = $(top_srcdir)/libeagle/include

INCLUDES = -I$(LIB_INCLUDE)

proto_inputs = Propose.proto
proto_outputs = Propose.pb.cc Propose.pb.h
$(proto_outputs) : $(proto_inputs)
	protoc -I. --cpp_out=. $(proto_inputs)

noinst_LTLIBRARIES = libproto.la
libproto_la_SOURCES = $(proto_outputs)
libproto_la_LDFLAGS = -lprotobuf
