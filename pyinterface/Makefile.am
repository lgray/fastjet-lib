pkgpython_PYTHON = fastjet.py
pkgpyexec_LTLIBRARIES = _fastjet.la
_fastjet_la_SOURCES = swig_wrap.cpp
_fastjet_la_CPPFLAGS = $(PYTHON_INCLUDE) $(AM_CXXFLAGS) $(CGAL_CPPFLAGS) -I../include -I$(top_srcdir)/include -I$(top_srcdir)/tools -DSWIG
_fastjet_la_LDFLAGS = -module
_fastjet_la_LIBADD = ../src/libfastjet.la ../tools/libfastjettools.la $(CGAL_LIBS)

# include the interface file for completeness
EXTRA_DIST=fastjet.i

#----------------------------------------------------------------------
# If we were to let the end-user run waig, then we should have the following:

#BUILT_SOURCES = $(srcdir)/swig_wrap.cpp
#SWIG_SOURCES = fastjet.i
#
#pkgpython_PYTHON = fastjet.py
#pkgpyexec_LTLIBRARIES = _fastjet.la
#_fastjet_la_SOURCES = swig_wrap.cpp
#_fastjet_la_CPPFLAGS = $(AX_SWIG_PYTHON_CPPFLAGS) $(PYTHON_INCLUDE) $(AM_CXXFLAGS) $(CGAL_CPPFLAGS) -I../include -I$(top_srcdir)/include -I$(top_srcdir)/tools -DSWIG
#_fastjet_la_LDFLAGS = -module
#_fastjet_la_LIBADD = ../src/libfastjet.la ../tools/libfastjettools.la
#
#$(srcdir)/swig_wrap.cpp : $(SWIG_SOURCES)
#	$(SWIG) $(AX_SWIG_PYTHON_OPT) -I$(top_srcdir)/include -I$(top_srcdir)/tools -o $@ $<

