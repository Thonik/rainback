bin_PROGRAMS = rainback
rainback_CPPFLAGS = @QT_CXXFLAGS@
rainback_LDADD = @QT_LIBS@

# moc-generated source files
rainback_MOC_SOURCES =

rainback_SOURCES = \
	$(rainback_MOC_SOURCES) \
	main.cpp

mostlyclean-local:
	rm -f $(rainback_MOC_SOURCES)
.PHONY: mostlyclean-local

moc_%.cpp: %.hpp
	@QT_MOC@ -o $@ $<
