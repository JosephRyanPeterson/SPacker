#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_LANG_PUSH([C++])

# Define package info
AC_INIT([spacker], [1.0.0], [jrtprsn3@illinois.edu])
AM_INIT_AUTOMAKE(summary, 1.0.0)

# Define source files
AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile
                 doc/Makefile
                 src/Makefile])


# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Define flags
AM_CXXFLAGS=-Wall -fPIC -std=c++11 -std=gnu++11

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_UINT32_T

# Checks for library functions.
AC_CHECK_FUNCS([floor])

AC_OUTPUT
