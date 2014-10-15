#
# libopflex: a framework for developing opflex-based policy agents
# Copyright (c) 2014 Cisco Systems, Inc. and others.  All rights reserved.
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License v1.0 which accompanies this distribution,
# and is available at http://www.eclipse.org/legal/epl-v10.html
#
###########
#
# Process this file with automake to produce a Makefile.in

ACLOCAL_AMFLAGS = -I m4

SUBDIRS  =
SUBDIRS += 3rdparty
SUBDIRS += util
SUBDIRS += logging
SUBDIRS += modb
SUBDIRS += comms
SUBDIRS += engine
SUBDIRS += ofcore
SUBDIRS += cwrapper
SUBDIRS += ietf
SUBDIRS += .

modb_includedir = $(includedir)/opflex/modb
modb_include_HEADERS = \
	include/opflex/modb/ClassInfo.h \
	include/opflex/modb/ModelMetadata.h \
	include/opflex/modb/Mutator.h \
	include/opflex/modb/ObjectListener.h \
	include/opflex/modb/PropertyInfo.h \
	include/opflex/modb/URIBuilder.h \
	include/opflex/modb/URI.h
modb_mo_includedir = $(includedir)/opflex/modb/mo-internal
modb_mo_include_HEADERS = \
	include/opflex/modb/mo-internal/MO.h \
	include/opflex/modb/mo-internal/ObjectInstance.h \
	include/opflex/modb/mo-internal/StoreClient.h 
core_includedir = $(includedir)/opflex/ofcore
core_include_HEADERS = \
	include/opflex/ofcore/OFFramework.h 
logging_includedir = $(includedir)/opflex/logging
logging_include_HEADERS = \
	include/opflex/logging/OFLogHandler.h \
	include/opflex/logging/StdOutLogHandler.h
c_includedir = $(includedir)/opflex/c
c_include_HEADERS = \
	include/opflex/c/ofcore_c.h \
	include/opflex/c/ofuri_c.h \
	include/opflex/c/ofloghandler_c.h \
	include/opflex/c/ofobjectlistener_c.h \
	include/opflex/c/ofmutator_c.h \
	include/opflex/c/offramework_c.h

lib_LTLIBRARIES = libopflex.la
libopflex_la_SOURCES = 
# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libopflex_la_SOURCES = dummy.cxx
libopflex_la_LIBADD  =
libopflex_la_LIBADD += modb/libmodb.la
libopflex_la_LIBADD += comms/libcomms.la
libopflex_la_LIBADD += engine/libengine.la
libopflex_la_LIBADD += ofcore/libcore.la
libopflex_la_LIBADD += cwrapper/libcwrapper.la
libopflex_la_LIBADD += util/libutil.la
libopflex_la_LIBADD += logging/liblogging.la

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libopflex.pc

# make a binary tgz package
tgz:
	rm -rf "${top_builddir}/tgz-tmp"
	mkdir "${top_builddir}/tgz-tmp"
	make ${tgztarget} DESTDIR="`cd ${top_builddir}/tgz-tmp; pwd`"
	test -z "${extratar}" || tar -xC "${top_builddir}/tgz-tmp" -f "${extratar}"
	(cd "${top_builddir}/tgz-tmp"; $(AMTAR) cf - --owner 0 --group 0 usr | GZIP=$(GZIP_ENV) $(GZIP_BIN) -c >../${pkgtgz})
	rm -rf "${top_builddir}/tgz-tmp"

if HAVE_DOXYGEN
  noinst_DATA = \
	doc/html
endif

doc/html: $(modb_include_HEADERS) $(modb_mo_include_HEADERS) $(core_include_HEADERS)  $(logging_include_HEADERS) $(c_include_HEADERS) doc/Doxyfile 
	cd doc && ${DOXYGEN} Doxyfile
doc: doc/html
doc/html-internal: clean-doc-internal $(modb_include_HEADERS) $(modb_mo_include_HEADERS) $(core_include_HEADERS)  $(logging_include_HEADERS) $(c_include_HEADERS) doc/Doxyfile-internal
	cd doc && ${DOXYGEN} Doxyfile-internal
doc-internal: doc/html-internal
install-data-local: doc
	@$(NORMAL_INSTALL)
	test -z "${DESTDIR}/${docdir}/html" || $(mkdir_p) "${DESTDIR}/${docdir}/html"
	for i in `ls $(top_builddir)/doc/html`; do \
	  $(INSTALL) -m 0644 $(top_builddir)/doc/html/$$i "${DESTDIR}/${docdir}/html"; \
	done
uninstall-local: 
	@$(NORMAL_UNINSTALL)
	rm -rf "${DESTDIR}/${docdir}/html"
	rm -rf "${DESTDIR}/${includedir}/opflex"

clean-doc:
	rm -rf doc/html doc/latex
clean-doc-internal:
	rm -rf doc/html-internal
clean-local: clean-doc clean-doc-internal
	rm -f *.rpm