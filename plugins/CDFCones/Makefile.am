SUBDIRS = CDFcode fastjet .

lib_LTLIBRARIES = libCDFConesPlugin.la
libCDFConesPlugin_la_CXXFLAGS = $(AM_CXXFLAGS) -I$(srcdir) -I$(srcdir)/../../include -I$(srcdir)/CDFcode
libCDFConesPlugin_la_SOURCES = CDFJetCluPlugin.cc CDFMidPointPlugin.cc
libCDFConesPlugin_la_LIBADD = CDFcode/libCDFcode.la

include_HEADERS = CDFJetCluPlugin.hh CDFMidPointPlugin.hh

EXTRA_DIST = makefile.static
