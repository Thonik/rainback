check_PROGRAMS = runtest runluatest
runtest_CXXFLAGS = $(AM_CXXFLAGS) @QT_CXXFLAGS@ @LUA_CXX_INCLUDE@ @LUA_INCLUDE@ -I$(top_srcdir)/src
runtest_LDADD = @QT_LIBS@ @LUA_CXX_LIBS@ @LUA_LIB@

runluatest_CXXFLAGS = $(runtest_CXXFLAGS)
runluatest_LDADD = $(runtest_LDADD)

TESTS = $(check_PROGRAMS)

test_suites =         \
	BasicTests

runtest_SOURCES =         \
	tests.cpp             \
	BasicTests.hpp

runtest_MOC_SOURCES = \
	moc_BasicTests.cpp

nodist_runtest_SOURCES = $(runtest_MOC_SOURCES)

runluatest_SOURCES = \
	test_lua.cpp

mostlyclean-local:
	rm -f $(runtest_MOC_SOURCES)
.PHONY: mostlyclean-local

tests.cpp: $(top_srcdir)/tests/make_wrapper.sh $(addsuffix .hpp, $(test_suites))
	rm -f $@
	$(top_srcdir)/tests/make_wrapper.sh $^ >$@-wip
	mv $@-wip $@
	chmod -w $@

moc_%.cpp: %.hpp
	@QT_MOC@ -o $@ $<
