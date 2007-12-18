# redirect to subdirectories
SUBDIRS = src include plugins example fortran_wrapper doc

# install the fastjet-config script in the bin directory
bin_SCRIPTS = fastjet-config

# tests done using make check
TEST_ENVIRONMENT = $(SHELL)
TESTS = test-compare.sh

# local macros for aclocal
ACLOCAL_AMFLAGS = -I m4

# additional files we need in the distribution
EXTRA_DIST = test-compare.sh makefile.static test-static.sh test-script-output-orig.txt Doxyfile
