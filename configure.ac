# -*- mode: Autoconf; mode: font-lock -*-
#
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
AC_INIT([libodb],[1.1.0],[mortenkjeldgaard@gmail.com])
AC_CONFIG_SRCDIR([src/ofile.cpp])

## Put autotools' auxilliary files in a subdir
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIRS([m4])

## Enable automake
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AM_PROG_AR

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
LT_INIT

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST

AC_CHECK_INCLUDES_DEFAULT
AC_PROG_EGREP

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
