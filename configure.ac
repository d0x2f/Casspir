#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([Casspir], [1.0], [me@dyl.dog])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_MACRO_DIRS([m4])

# Checks for programs.
AC_PROG_CXX
AM_PROG_AR

LT_INIT

# Checks for c++14 support
AX_CXX_COMPILE_STDCXX([14], [noext], [mandatory])

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_OFF_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_CHECK_FUNCS([gettimeofday])

AM_CPPFLAGS="$AM_CPPFLAGS -I\$(top_srcdir)/src -iquote \$(srcdir)"
AC_SUBST([AM_CPPFLAGS])

AC_OUTPUT(Makefile src/Makefile test/Makefile)
