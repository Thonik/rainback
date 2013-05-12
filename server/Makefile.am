bin_PROGRAMS = rainbackserver

rainbackserver_CPPFLAGS = @QT_CXXFLAGS@ @LUA_CXX_INCLUDE@ -I$(top_srcdir)/include -DTOP_BUILDDIR='"$(top_builddir)"' -DTOP_SRCDIR='"$(top_srcdir)"'

# Don't try changing "../src" to use "top_srcdir" - this is referring to the
# built artifact, not something in the source directory
rainbackserver_LDADD = ../src/librainback.la @LUA_CXX_LIBS@ @QT_LIBS@

rainbackserver_MOC_SOURCES = \
	moc_Server.cpp

rainbackserver_SOURCES = \
	Bootstrapper.cpp \
	Bootstrapper.hpp \
	Server.cpp \
	Server.hpp \
	main.cpp

nodist_rainbackserver_SOURCES = $(rainbackserver_MOC_SOURCES)

mostlyclean-local:
	rm -f $(rainbackserver_la_MOC_SOURCES)
.PHONY: mostlyclean-local

moc_%.cpp: %.hpp
	@QT_MOC@ -o $@ $<
