#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([gateway-roomsec], [1.0], [youngar@mcmaster.ca])
AC_CONFIG_SRCDIR([src/cpp/main.cpp])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([subdir-objects foreign -Wall -Werror])

AC_ARG_ENABLE([gateway],
	[AS_HELP_STRING([--enable-gateway], [enable gateway specific code (default is no)])],
	[roomsec_gateway=$enableval])

if test x"$roomsec_gateway" = xyes ; then
	echo "++++++++++++ Building for the Raspberry Pi"
	ENABLE_GATEWAY="TRUE"
else
	echo "------------ Not building for Raspberry Pi"
fi

AC_ARG_ENABLE([hardware-tests],
	[AS_HELP_STRING([--enable-hardware-tests], [enable gateway hardware tests (default is no)])],
	[test_hardware=$enableval])

AM_CONDITIONAL([TEST_HARDWARE], [test x"test_hardware" = xyes])
if test x"$hardware_tests" = xyes ; then
	echo "++++++++++++ Enable hardware tests"
else
	echo "------------ Disable hardware tests"
fi

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_LANG([C++], [C])
AM_PROG_AR
AC_PROG_INSTALL
AC_PROG_MKDIR_P

# * Check for pthreads library.
ACX_PTHREAD([have_pthread=yes], [have_pthread=no])

# Initialize libtool
LT_INIT([disable-shared])

# Define variables for unit test using gtest
GTEST_VERSION="1.6.0"
if test "x$have_pthread" = "xyes"; then
  GTEST_CPPFLAGS="-DGTEST_HAS_PTHREAD=1 -I\$(top_srcdir)/lib/gmock"
  GTEST_CXXFLAGS="$PTHREAD_CFLAGS"
  GTEST_LDFLAGS="-L\$(top_builddir)/lib/gmock"
  GTEST_LIBS="$PTHREAD_LIBS"
else
  GTEST_CPPFLAGS="-DGTEST_HAS_PTHREAD=0 -I\$(top_srcdir)/lib/gmock"
  GTEST_CXXFLAGS=
  GTEST_LDFLAGS="-L\$(top_builddir)/lib/gmock"
  GTEST_LIBS=
fi
AC_SUBST([GTEST_VERSION])
AC_SUBST([GTEST_CPPFLAGS])
AC_SUBST([GTEST_CXXFLAGS])
AC_SUBST([GTEST_LDFLAGS])
AC_SUBST([GTEST_LIBS])

# * Check for Doxygen.
# DX_???_FEATURE(ON|OFF) - control the default setting fo a Doxygen feature.
DX_DOXYGEN_FEATURE(ON)
DX_HTML_FEATURE(ON)
DX_MAN_FEATURE(OFF)
DX_PDF_FEATURE(OFF)
DX_PS_FEATURE(OFF)
# DX_INIT_DOXYGEN(PROJECT, [CONFIG-FILE], [OUTPUT-DOC-DIR])
DX_INIT_DOXYGEN($PACKAGE_NAME, [Doxyfile], [doc])

# Check python for the script checking coding style
AC_CHECK_PROG(HAVE_PYTHON, python, yes, no)

# Checks for existence of coverage tools and define variables for reporting coverage
AC_CHECK_COVERAGE

# Check for C++11 supports
#   ext: use extended mode (e.g. -std=gnu++11)
#   noext: use non-extended mode (e.g. -std=c++11)
AX_CXX_COMPILE_STDCXX_11([noext])

# Check for header files.


# Check for typedefs, structures, and compiler characteristics.

# Check for library functions.
# Thrift
PKG_CHECK_MODULES([THRIFT], [thrift])

# log4CXX
PKG_CHECK_MODULES([LOG4CXX], [liblog4cxx])

# Check for the existance of a file in the source directory.
AC_CONFIG_SRCDIR([src/])

# Output files.
# AC_CONFIG_HEADERS(HEADERS...)
# Create Header for all HEADER.in Use only one such header unless you know what you are doing.
# Declare config.h as output header.
AC_CONFIG_HEADERS([config.h])

# Declare output files.
AC_CONFIG_FILES([Makefile
                 Doxyfile])

# Actually output all declared files.
AC_OUTPUT

AC_MSG_RESULT([
Configure Information:
  C Compiler        : $CC
    DEFS            :   $DEFS
    CPPFLAGS        :   $CPPFLAGS
    CFLAGS          :   $CFLAGS

  C++ Compiler      : $CXX
    DEFS            :   $DEFS
    CPPFLAGS        :   $CPPFLAGS
    CXXFLAGS        :   $CXXFLAGS

  Linker            : $LD
    LDFLAGS         :   $LDFLAGS
    LIBS            :   $LIBS

Supported Build Features:
  Documents             : $(if test "x$DX_FLAG_doc" = "x1"; then echo yes; else echo no; fi)
    html                :   $(if test "x$DX_FLAG_html" = "x1"; then echo yes; else echo no; fi)
    man                 :   $(if test "x$DX_FLAG_man" = "x1"; then echo yes; else echo no; fi)
    pdf                 :   $(if test "x$DX_FLAG_pdf" = "x1"; then echo yes; else echo no; fi)
    ps                  :   $(if test "x$DX_FLAG_ps" = "x1"; then echo yes; else echo no; fi)

  Coding Style Checks   : $HAVE_PYTHON

  Coverage Reprots      : $ENABLE_COVERAGE
])

