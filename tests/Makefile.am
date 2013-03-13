check_PROGRAMS = runtest
runtest_CXXFLAGS = $(AM_CXXFLAGS) @QT_CXXFLAGS@ -I$(top_srcdir)/src
runtest_LDADD = @QT_LIBS@

TESTS = $(check_PROGRAMS)

test_suites =         \
	BasicTests

runtest_SOURCES =         \
	tests.cpp             \
	BasicTests.hpp

runtest_MOC_SOURCES = \
	moc_BasicTests.cpp

nodist_runtest_SOURCES =      \
	$(runtest_MOC_SOURCES)

mostlyclean-local:
	rm -f $(runtest_MOC_SOURCES)
.PHONY: mostlyclean-local

tests.cpp: make_wrapper.sh $(addsuffix .hpp, $(test_suites))
	rm -f $@
	./make_wrapper.sh $^ >$@-wip
	mv $@-wip $@
	chmod -w $@

moc_%.cpp: %.hpp
	@QT_MOC@ -o $@ $<
