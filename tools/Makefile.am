# shared library

SUBDIRS=fastjet
lib_LTLIBRARIES = libfastjettools.la
libfastjettools_la_CXXFLAGS = $(AM_CXXFLAGS) -I$(srcdir)/../include -I$(srcdir)
libfastjettools_la_SOURCES = BackgroundEstimator.cc \
  Filter.cc Subtractor.cc\
  MassDropTagger.cc NSubjettinessTagger.cc CASubJetTagger.cc

# EXTRA_DIST = makefile.static 
