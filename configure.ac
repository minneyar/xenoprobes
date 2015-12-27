#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([xenoprobes], [0.1], [danielosmari@gmail.com])
AC_CONFIG_SRCDIR([src/optimize.cpp])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign subdir-objects])

AC_CANONICAL_HOST

AC_LANG(C++)

# Checks for programs.
AC_PROG_CXX

AX_CXX_COMPILE_STDCXX_11(noext)

# Checks for libraries.

AX_BOOST_BASE([1.48],,AC_MSG_ERROR([cannot use this program without Boost]))
AX_BOOST_PROGRAM_OPTIONS

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

