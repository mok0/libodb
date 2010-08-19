## -*- mode: Makefile ; mode: font-lock -*-
## $Id: Makefile.am 17 2007-10-25 18:18:49Z mok $

AM_CPPFLAGS = -I$(top_srcdir)/src
AM_LDFLAGS = $(top_builddir)/src/libodb.la

#bin_PROGRAMS = odbdir odbcat

noinst_PROGRAMS = fix_residue_pointers testodb2 odbdir odbcat

fix_residue_pointers_SOURCES = fix_residue_pointers.cpp

odbcat_SOURCES =  odbcat.cpp

odbdir_SOURCES =  odbdir.cpp

testodb2_SOURCES =  testodb2.cpp



####
