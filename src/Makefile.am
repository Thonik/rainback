bin_PROGRAMS = rainback
rainback_CPPFLAGS = @QT_CXXFLAGS@ @LUA_CXX_INCLUDE@ @LUA_INCLUDE@
rainback_LDADD = @QT_LIBS@ @LUA_CXX_LIBS@ @LUA_LIB@

# moc-generated source files
rainback_MOC_SOURCES = \
	moc_Desktop.cpp \
	moc_LuaPainter.cpp

nodist_rainback_SOURCES = $(rainback_MOC_SOURCES)

rainback_SOURCES = \
	Desktop.cpp \
	Desktop.hpp \
	LuaPainter.cpp \
	LuaPainter.hpp \
	main.cpp

mostlyclean-local:
	rm -f $(rainback_MOC_SOURCES)
.PHONY: mostlyclean-local

moc_%.cpp: %.hpp
	@QT_MOC@ -o $@ $<
