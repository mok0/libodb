# -*- mode: Autoconf; mode: font-lock -*-
#
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(libodb, 1.0.3, mok@bioxray.dk)
AC_CONFIG_SRCDIR([src/ofile.cpp])
AC_CONFIG_MACRO_DIR([m4])

##AC_CONFIG_HEADER([config.h])

AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LIBTOOL

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST

# Checks for library functions.
AC_HEADER_STDC
AC_FUNC_STRTOD
AC_CHECK_FUNCS([memset strtol])

AC_CONFIG_FILES([
	Makefile
	libodb.spec
	libodb.info
	src/Makefile
	testing/Makefile
	])
AC_OUTPUT

####
