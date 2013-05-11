# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([rainback], [1.0], [dafrito@gmail.com], [rainback], [http://www.dafrito.com])
AC_SUBST([PACKAGE_RELEASE], [1])
AM_INIT_AUTOMAKE([-Wall foreign])
AC_CONFIG_SRCDIR([demo/main.cpp])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_MACRO_DIR([m4])

# Check for build programs
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S

# Initialize libtool
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])
LT_PREREQ([2.2])
LT_INIT
AC_PROG_LIBTOOL

# Check for compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_LANG([C++])
AX_CXX_COMPILE_STDCXX_11([noext],[AM_CXXFLAGS])
AC_SUBST([AM_CXXFLAGS])

# Check for Boost
AX_BOOST_BASE
AX_BOOST_UNIT_TEST_FRAMEWORK

# Check for Lua-cxx
if ! pkg-config --print-errors --exists "lua-cxx"; then
    AC_MSG_ERROR([Lua-cxx is required to build Rainback])
fi

AC_SUBST(LUA_CXX_INCLUDE)
AC_SUBST(LUA_CXX_LIBS)
LUA_CXX_INCLUDE=`pkg-config --cflags lua-cxx`
LUA_CXX_LIBS=`pkg-config --libs lua-cxx`

# Check for Qt
AX_HAVE_QT_CORE
AX_HAVE_QT_GUI
AX_HAVE_QT_WIDGETS
AX_HAVE_QT_MOC

if test x"$have_qt_core" = xno || test x"$have_qt_gui" = xno || test x"$have_qt_moc" = xno; then
    AC_MSG_ERROR([Qt is required to build rainback])
fi;

AC_CONFIG_FILES([rainback.m4 Makefile src/Makefile include/Makefile demo/Makefile tests/Makefile rpm.spec])
AC_OUTPUT
