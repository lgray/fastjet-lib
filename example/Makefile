# fastjet/example/Makefile

# include the Makefile in the top directory, which has the bits and pieces
# needed to get CGAL working
include ../Makefile

FASTJET_INCLUDE = -I../include
INCLUDE += $(FASTJET_INCLUDE)
LDFLAGS += -L../lib -lfastjet

# plugin includes here are temporary and subject to change...
PLUGIN_INCLUDE = -I../plugins/CDFCones -I../plugins/PxCone -I../plugins/SCones
PLUGIN_LIBRARY = -L../plugins/CDFCones -L../plugins/PxCone -L../plugins/SCones -lCDFConesPlugin -lPxConePlugin -lSConesPlugin -L$(HOME)/tex/jets/IRSafeCone/scones/src -lscones $(F77LIB)

INCLUDE += $(PLUGIN_INCLUDE)

# Gavin sometimes likes to compile with google's tcmalloc and profiler
# libraries: tcmalloc gives a faster malloc (about 10% speed
# gain on the CGAL based algorithms), while the profiler is useful
# for a faithful estimation of where the time is being spent
ifeq ($(shell whoami),salam)
  LDFLAGS += -L/ada1/lpthe/salam/software/local/lib -ltcmalloc -lprofiler
endif

SRCS = fastjet_example.cc ktjet_example.cc fastjet_timing.cc fastjet_timing_plugins.cc CmdLine.cc
OBJS = $(patsubst %.cc,%.o,$(SRCS))

fastjet_example: fastjet_example.o  ../lib/libfastjet.a
	$(CXX)  -o fastjet_example fastjet_example.o  $(LDFLAGS)

fastjet_example_v1_interface: fastjet_example_v1_interface.o  ../lib/libfastjet.a
	$(CXX)  -o fastjet_example_v1_interface fastjet_example_v1_interface.o  $(LDFLAGS)

fastjet_areas: fastjet_areas.o  ../lib/libfastjet.a
	$(CXX)  -o fastjet_areas fastjet_areas.o  $(LDFLAGS)

fastjet_subtraction: fastjet_subtraction.o  ../lib/libfastjet.a
	$(CXX)  -o fastjet_subtraction fastjet_subtraction.o  $(LDFLAGS)

# for the following to compile the KTJET_INCLUDE and KTJET_LIBRARY
# variables must be set in ../Makefile
ktjet_example: ktjet_example.o 
	$(CXX)  -o ktjet_example ktjet_example.o $(KTJET_LIBRARY) $(LDFLAGS) 

# explicit compilation so as to add ktjet libaries only for this file
ktjet_example.o: ktjet_example.cc
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(KTJET_INCLUDE) -c $<

fastjet_timing: fastjet_timing.o CmdLine.o ../lib/libfastjet.a
	$(CXX)  -o fastjet_timing fastjet_timing.o CmdLine.o  $(LDFLAGS)

fastjet_timing_plugins: fastjet_timing_plugins.o CmdLine.o ../lib/libfastjet.a
	$(CXX)  -o fastjet_timing_plugins fastjet_timing_plugins.o CmdLine.o  $(LDFLAGS) $(PLUGIN_LIBRARY)

# for the following to compile the KTJET_INCLUDE and KTJET_LIBRARY
# variables must be set in ../Makefile
ktjet_timing: ktjet_timing.o CmdLine.o
	$(CXX)  -o ktjet_timing ktjet_timing.o CmdLine.o $(KTJET_LIBRARY) $(LDFLAGS) 

# explicit compilation so as to add ktjet libaries only for this file
ktjet_timing.o: ktjet_timing.cc
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(KTJET_INCLUDE) -c $<

clean: 
	rm -f *.o

realclean: clean
	rm -f  fastjet_example fastjet_timing ktjet_example ktjet_timing

depend:
	makedepend $(FASTJET_INCLUDE) --   -- $(SRCS)
# DO NOT DELETE

fastjet_example.o: ../include/fastjet/PseudoJet.hh
fastjet_example.o: ../include/fastjet/internal/numconsts.hh
fastjet_example.o: ../include/fastjet/internal/base.hh
fastjet_example.o: ../include/fastjet/ClusterSequence.hh
fastjet_example.o: ../include/fastjet/internal/DynamicNearestNeighbours.hh
fastjet_example.o: ../include/fastjet/Error.hh
fastjet_example.o: ../include/fastjet/JetDefinition.hh
fastjet_timing.o: ../include/fastjet/PseudoJet.hh
fastjet_timing.o: ../include/fastjet/internal/numconsts.hh
fastjet_timing.o: ../include/fastjet/internal/base.hh
fastjet_timing.o: ../include/fastjet/ClusterSequence.hh
fastjet_timing.o: ../include/fastjet/internal/DynamicNearestNeighbours.hh
fastjet_timing.o: ../include/fastjet/Error.hh
fastjet_timing.o: ../include/fastjet/JetDefinition.hh CmdLine.hh
CmdLine.o: CmdLine.hh
