lib_LTLIBRARIES = librainback.la
librainback_la_CPPFLAGS = @QT_CXXFLAGS@ -I$(top_srcdir)/include
librainback_la_LIBADD = @QT_LIBS@
librainback_la_LDFLAGS = -version-info 0:0:0

# moc-generated source files
librainback_la_MOC_SOURCES =

nodist_librainback_la_SOURCES = $(librainback_la_MOC_SOURCES)

librainback_la_SOURCES = \
	Bootstrapper.cpp \
	util.cpp

pkginclude_HEADERS = \
	../include/Bootstrapper.hpp \
	../include/util.hpp

bin_PROGRAMS = rainback
rainback_CPPFLAGS = $(librainback_la_CPPFLAGS)
rainback_LDADD = librainback.la $(librainback_la_LIBADD)

rainback_SOURCES = \
	main.cpp

nodist_rainback_SOURCES = $(rainback_MOC_SOURCES)

mostlyclean-local:
	rm -f $(librainback_la_MOC_SOURCES)
.PHONY: mostlyclean-local

moc_%.cpp: $(top_srcdir)/src/%.hpp
	@QT_MOC@ -o $@ $<

moc_%.cpp: $(top_srcdir)/include/%.hpp
	@QT_MOC@ -o $@ $<
