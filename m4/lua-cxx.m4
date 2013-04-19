# ===========================================================================
#    http://www.dafrito.com
# ===========================================================================
#
# SYNOPSIS
#
#   AX_HAVE_LUA_CXX
#
# DESCRIPTION
#
#   Test for lua-cxx
#
#   This macro calls:
#
#     AC_SUBST(LUA_CXX_INCLUDE)
#     AC_SUBST(LUA_CXX_LIBS)
#
#   And sets:
#
#     HAVE_LUA_CXX
#
# LICENSE
#
#   Copyright (c) 2013 Aaron Faanes <dafrito@gmail.com>
#
#   Copying and distribution of this file, with or without modification, are
#   permitted in any medium without royalty provided the copyright notice
#   and this notice are preserved. This file is offered as-is, without any
#   warranty.

AC_DEFUN([AX_HAVE_LUA_CXX],
[
	AC_ARG_WITH([lua-cxx],
    AS_HELP_STRING([--with-lua-cxx], [use lua-cxx]),
    [
      if test x"$withval" = "xno"; then
        want_lua_cxx="no";
      else
        want_lua_cxx="yes";
      fi
    ],
    [want_lua_cxx="yes"]
  )

  if test x"$want_lua_cxx" = "xyes"; then
    # Ensure we have a suitable C++ compiler
    AC_REQUIRE([AC_PROG_CC])
    AC_REQUIRE([AX_HAVE_QT_CORE])
    AC_REQUIRE([AC_PROG_CXX])

    # Check for dependencies
    AX_CXX_COMPILE_STDCXX_11([noext],[ax_lua_cxx_cxx11_flags])
    AX_HAVE_QT_CORE

    AC_MSG_CHECKING([for lua-cxx])

    ax_lua_cxx_saved_CPPFLAGS="$CPPFLAGS"
    ax_lua_cxx_INCLUDE=""
    CPPFLAGS="$CPPFLAGS $ax_lua_cxx_cxx11_flags $QT_CXXFLAGS $ax_lua_cxx_INCLUDE"
    export CPPFLAGS

    ax_lua_cxx_saved_LIBS="$LIBS"
    ax_lua_cxx_LIBS=""
    LIBS="$LIBS $ax_lua_cxx_LIBS"
    export LIBS

    AC_LANG_PUSH([C++])
    AC_COMPILE_IFELSE([
      AC_LANG_PROGRAM(
        [[@%:@include <lua-cxx/LuaEnvironment.hpp>]],
        [[
          Lua lua;
          return 0;
        ]]
      )],
      ax_have_lua_cxx=yes,
      ax_have_lua_cxx=no
    )
    AC_LANG_POP([C++])

    AC_MSG_RESULT([$ax_have_lua_cxx])
    if test x"$ax_have_lua_cxx" = "xyes"; then
      AC_DEFINE(HAVE_LUA_CXX,,[define if lua-cxx is available])
      LUA_CXX_INCLUDE="$ax_lua_cxx_INCLUDE"
      dnl This doesn't yet test the location of this project
      LUA_CXX_LIBS="$ax_lua_cxx_LIBS -llua-cxx"
    else
      AC_MSG_ERROR([Could not find lua-cxx])
      LUA_CXX_INCLUDE=""
      LUA_CXX_LIBS=""
    fi
    AC_SUBST(LUA_CXX_INCLUDE)
    AC_SUBST(LUA_CXX_LIBS)

    CPPFLAGS="$ax_lua_cxx_saved_CPPFLAGS"
    LIBS="$ax_lua_cxx_saved_LIBS"
  fi;
])
