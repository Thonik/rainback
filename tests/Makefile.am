check_PROGRAMS = runtest runluatest
TESTS = $(check_PROGRAMS)

runtest_CXXFLAGS = $(AM_CXXFLAGS) @QT_CXXFLAGS@ @LUA_CXX_INCLUDE@ @LUA_INCLUDE@ -I$(top_srcdir)/include \
	-DBOOST_TEST_DYN_LINK

# Don't try changing "../src" to use "top_srcdir" - this is referring to the
# built artifact, not something in the source directory
runtest_LDADD = ../src/librainback.la @LUA_CXX_LIBS@ @LUA_LIB@ @QT_LIBS@ @BOOST_UNIT_TEST_FRAMEWORK_LIB@

noinst_HEADERS = \
	init.hpp

runtest_SOURCES = \
	basic.cpp \
	main.cpp

runluatest_CXXFLAGS = $(runtest_CXXFLAGS)
runluatest_LDADD = $(runtest_LDADD)

runluatest_SOURCES = \
	test_lua.cpp

EXTRA_DIST = \
	rainback/AnchoredBound.lua
