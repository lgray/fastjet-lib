SUBDIRS = siscone fastjet .

lib_LTLIBRARIES = libSISConePlugin.la
libSISConePlugin_la_CXXFLAGS = ${AM_CXXFLAGS} -I$(srcdir) -I$(srcdir)/siscone -I$(srcdir)/../../include
libSISConePlugin_la_SOURCES = SISConePlugin.cc

EXTRA_DIST = makefile.static
