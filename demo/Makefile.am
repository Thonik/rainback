bin_PROGRAMS = rainback

rainback_CPPFLAGS = @QT_CXXFLAGS@ @LUA_CXX_INCLUDE@ -I$(top_srcdir)/include

# Don't try changing "../src" to use "top_srcdir" - this is referring to the
# built artifact, not something in the source directory
rainback_LDADD = ../src/librainback.la @LUA_CXX_LIBS@ @QT_LIBS@

rainback_SOURCES = \
	Bootstrapper.cpp \
	Bootstrapper.hpp \
	util.cpp \
	util.hpp \
	main.cpp

nodist_rainback_SOURCES = $(rainback_MOC_SOURCES)

mostlyclean-local:
	rm -f $(librainback_la_MOC_SOURCES)
.PHONY: mostlyclean-local

moc_%.cpp: %.hpp
	@QT_MOC@ -o $@ $<
