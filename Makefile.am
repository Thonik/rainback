ACLOCAL_AMFLAGS = -I m4
SUBDIRS = src include demo tests

pkgdata_DATA = \
	rainback.m4

export RPMDIR ?= $$HOME/rpmbuild

checkdebug: $(TESTS)
	cd tests && gdb .libs/lt-runluatest

checkvalgrind:
	cd tests && valgrind -v --leak-check=full .libs/lt-runtest

# This is a common typo for me
cehck: check
.PHONY: cehck

run:
	cd demo && ./rainback

debug:
	cd demo && gdb .libs/lt-rainback

valgrind:
	cd demo && valgrind -v --leak-check=full --trace-children=yes .libs/lt-rainback

RPMFLAGS ?= --ba
SRCRPM=rainback-@PACKAGE_VERSION@-@PACKAGE_RELEASE@.src.rpm

rpm:
	$(MAKE) dist-gzip $(RPMDIR)
	cp -u $(PACKAGE_NAME)-$(PACKAGE_VERSION).tar.gz $(RPMDIR)/SOURCES
	cp -u rpm.spec $(RPMDIR)/SPECS/$(PACKAGE_NAME).spec
	rpmbuild $(RPMFLAGS) $(RPMDIR)/SPECS/$(PACKAGE_NAME).spec
	for package in `rpm -q --specfile rpm.spec`; do \
		arch=`echo $$package | grep -E -o '[^.]+$$'`; \
		filename="$(RPMDIR)/RPMS/$$arch/$$package.rpm"; \
		[ -e `basename $$filename` ] || ln -s $$filename; \
	done
	rm -f $(SRCRPM)
	ln -s $(RPMDIR)/SRPMS/rainback-@PACKAGE_VERSION@-@PACKAGE_RELEASE@`rpmbuild -E '%{?dist}' rpm.spec`.src.rpm $(SRCRPM)
.PHONY: rpm

$(RPMDIR):
	mkdir -p $@
	cd $@ && mkdir -p SOURCES SPECS BUILD RPMS SRPMS
