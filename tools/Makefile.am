# shared library

SUBDIRS=fastjet
lib_LTLIBRARIES = libfastjettools.la
libfastjettools_la_CXXFLAGS = $(AM_CXXFLAGS) -I$(srcdir)/../include -I$(srcdir)
libfastjettools_la_SOURCES = Filter.cc Transformer.cc BackgroundEstimator.cc

# EXTRA_DIST = makefile.static 
