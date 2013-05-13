check_PROGRAMS = runtest runluatest
TESTS = $(check_PROGRAMS)

runtest_CXXFLAGS = $(AM_CXXFLAGS) @QT_CXXFLAGS@ @LUA_CXX_INCLUDE@ -I$(top_srcdir)/include \
	-DBOOST_TEST_DYN_LINK

# Don't try changing "../src" to use "top_srcdir" - this is referring to the
# built artifact, not something in the source directory
runtest_LDADD = ../src/librainback.la @LUA_CXX_LIBS@ @QT_LIBS@ @BOOST_UNIT_TEST_FRAMEWORK_LIB@

# moc-generated source files
runtest_MOC_SOURCES = \
	moc_mocks.cpp

nodist_runtest_SOURCES = $(runtest_MOC_SOURCES)

noinst_HEADERS = \
	TestIODevice.hpp \
	mocks.hpp \
	init.hpp

runtest_SOURCES = \
	basic.cpp \
	TestIODevice.cpp \
	mocks.cpp \
	main.cpp

runluatest_CXXFLAGS = $(runtest_CXXFLAGS)
runluatest_LDADD = $(runtest_LDADD)

runluatest_SOURCES = \
	test_lua.cpp

EXTRA_DIST = \
	rainback \
	test_lua.lua \
	testrunner.lua

moc_%.cpp: %.hpp
	@QT_MOC@ -o $@ $<
