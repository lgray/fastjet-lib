# fastjet/example/Makefile

# include the Makefile in the top directory, which has the bits and pieces
# needed to get CGAL working
include ../Makefile

FASTJET_INCLUDE = -I../include
INCLUDE += $(FASTJET_INCLUDE)
LDFLAGS += -L../lib -lfastjet

SRCS = fastjet_example.cc fastjet_timing.cc CmdLine.cc
OBJS = $(patsubst %.cc,%.o,$(SRCS))

# REMEMBER TO CHANGE THIS **********!!!!!!!!
#all: fastjet_timing

fastjet_example: fastjet_example.o  ../lib/libfastjet.a
	$(CXX)  -o fastjet_example fastjet_example.o  $(LDFLAGS)

fastjet_timing: fastjet_timing.o CmdLine.o ../lib/libfastjet.a
	$(CXX)  -o fastjet_timing fastjet_timing.o CmdLine.o  $(LDFLAGS)

clean: 
	rm -f *.o

realclean: clean
	rm -f fastjet_example fastjet_timing

depend:
	makedepend $(FASTJET_INCLUDE) --   -- $(SRCS)
# DO NOT DELETE

fastjet_example.o: ../include/FjPseudoJet.hh ../include/numconsts.hh
fastjet_example.o: ../include/FjClusterSequence.hh
fastjet_example.o: ../include/DynamicNearestNeighbours.hh
fastjet_timing.o: ../include/FjPseudoJet.hh ../include/numconsts.hh
fastjet_timing.o: ../include/FjClusterSequence.hh
fastjet_timing.o: ../include/DynamicNearestNeighbours.hh CmdLine.hh
CmdLine.o: CmdLine.hh
