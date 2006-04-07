# fastjet/example/Makefile

# include the Makefile in the top directory, which has the bits and pieces
# needed to get CGAL working
include ../Makefile

FASTJET_INCLUDE = -I../include
INCLUDE += $(FASTJET_INCLUDE)
#LDFLAGS += -L../lib -lfastjet
LDFLAGS += -L../lib -lfastjet 
# get google malloc for debugging purposes...
LDFLAGS += -L$(HOME)/software/local/lib -ltcmalloc -lprofiler
#LDFLAGS += -L$(HOME)/software/local/lib 

PROG_SRCS = areas.cc areas-native.cc

SRCS =  CmdLine.cc FjClusterSequenceWithArea.cc
OBJS = $(patsubst %.cc,%.o,$(SRCS))

areas: areas.o $(OBJS) ../lib/libfastjet.a
	$(CXX)  -o areas areas.o  $(OBJS)  $(LDFLAGS)

areas-native: areas-native.o $(OBJS) ../lib/libfastjet.a
	$(CXX)  -o areas-native areas-native.o $(OBJS)  $(LDFLAGS)

areas-histogramming: areas-histogramming.o $(OBJS) ../lib/libfastjet.a
	$(CXX)  -o areas-histogramming areas-histogramming.o $(OBJS)  $(LDFLAGS)

areas-empty-event: areas-empty-event.o $(OBJS) ../lib/libfastjet.a
	$(CXX)  -o areas-empty-event areas-empty-event.o $(OBJS)  $(LDFLAGS)

clean: 
	rm -f *.o

realclean: clean
	rm -f   areas 

depend:
	makedepend $(FASTJET_INCLUDE) --   -- $(SRCS)
# DO NOT DELETE

CmdLine.o: CmdLine.hh
FjClusterSequenceWithArea.o: FjClusterSequenceWithArea.hh
FjClusterSequenceWithArea.o: ../include/FjPseudoJet.hh
FjClusterSequenceWithArea.o: ../include/numconsts.hh
FjClusterSequenceWithArea.o: ../include/FjClusterSequence.hh
FjClusterSequenceWithArea.o: ../include/DynamicNearestNeighbours.hh
