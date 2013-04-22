lib_LTLIBRARIES = librainback.la
librainback_la_CPPFLAGS = @QT_CXXFLAGS@ @LUA_CXX_INCLUDE@ @LUA_INCLUDE@ -I$(top_srcdir)/include
librainback_la_LIBADD = @LUA_CXX_LIBS@ @QT_LIBS@ @LUA_LIB@
librainback_la_LDFLAGS = -version-info 0:0:0

# moc-generated source files
librainback_la_MOC_SOURCES = \
	moc_Rainback.cpp \
	moc_LuaWidget.cpp \
	moc_LuaFont.cpp \
	moc_LuaPainter.cpp

nodist_librainback_la_SOURCES = $(librainback_la_MOC_SOURCES)

librainback_la_SOURCES = \
	util.cpp \
	Rainback.cpp \
	LuaWidget.cpp \
	LuaFont.cpp \
	LuaPainter.cpp

mostlyclean-local:
	rm -f $(librainback_la_MOC_SOURCES)
.PHONY: mostlyclean-local

moc_%.cpp: $(top_srcdir)/include/%.hpp
	@QT_MOC@ -o $@ $<
