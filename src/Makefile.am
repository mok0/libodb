# $Id: Makefile,v 1.3 2003/10/09 23:47:29 mok Exp $
# -*- mode: Makefile ; mode: font-lock -*-

INCLUDES = -I$(top_srcdir)/include/odb -I$(prefix)/include

libodb_h_sources = datablock.h odb.h ofile.h

libodb_cpp_sources = odb.cpp  ofile.cpp

library_includedir = $(includedir)/odb
library_include_HEADERS = $(libodb_h_sources)

lib_LTLIBRARIES = libodb.la
libodb_la_SOURCES = $(libodb_cpp_sources) $(libodb_h_sources)

#tst: odb.cpp ofile.o 
#	g++ -DTESTING -o $@  $(DEBUG) odb.cpp ofile.o 
#
#fix_residue_pointers: fix_residue_pointers.cpp
#	g++ -o $@ fix_residue_pointers.cpp odb.o ofile.o

####
