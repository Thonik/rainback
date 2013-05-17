lib_LTLIBRARIES = librainback.la
librainback_la_CPPFLAGS = @QT_CXXFLAGS@ @LUA_CXX_INCLUDE@ -I$(top_srcdir)/include
librainback_la_LIBADD = @LUA_CXX_LIBS@ @QT_LIBS@
librainback_la_LDFLAGS = -version-info 0:0:0

# moc-generated source files
librainback_la_MOC_SOURCES = \
	protocol/moc_Human.cpp \
	protocol/moc_Pascal.cpp \
	protocol/moc_AbstractLine.cpp \
	proxy/moc_TCPServer.cpp \
	proxy/moc_Font.cpp \
	proxy/moc_Painter.cpp \
	moc_Rainback.cpp \
	moc_LuaWidget.cpp

nodist_librainback_la_SOURCES = $(librainback_la_MOC_SOURCES)

librainback_la_SOURCES = \
	protocol/Human.cpp \
	protocol/Pascal.cpp \
	protocol/AbstractLine.cpp \
	proxy/TCPServer.cpp \
	proxy/QObject.cpp \
	proxy/Socket.cpp \
	proxy/Font.cpp \
	proxy/Painter.cpp \
	Rainback.cpp \
	LuaWidget.cpp

mostlyclean-local:
	rm -f $(librainback_la_MOC_SOURCES)
.PHONY: mostlyclean-local

protocol/moc_%.cpp: $(top_srcdir)/include/protocol/%.hpp
	mkdir -p protocol
	cd protocol && @QT_MOC@ -o `basename $@` ../$<

proxy/moc_%.cpp: $(top_srcdir)/include/proxy/%.hpp
	mkdir -p `dirname $@`
	cd `dirname $@` && @QT_MOC@ -o `basename $@` ../$<

moc_%.cpp: $(top_srcdir)/include/%.hpp
	@QT_MOC@ -o $@ $<
