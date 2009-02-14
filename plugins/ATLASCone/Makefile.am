SUBDIRS = fastjet .

lib_LTLIBRARIES = libATLASConePlugin.la
libATLASConePlugin_la_CXXFLAGS = ${AM_CXXFLAGS} -I$(srcdir) -I$(srcdir)/../../include
libATLASConePlugin_la_SOURCES = LorentzVector.cc CommonUtils.cc Jet.cc JetConeFinderTool.cc JetSplitMergeTool.cc ATLASConePlugin.cc

EXTRA_DIST = makefile.static
