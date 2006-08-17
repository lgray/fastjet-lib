# fastjet/example/Makefile

# include the Makefile in the top directory, which has the bits and pieces
# needed to get CGAL working
include ../Makefile

CDFCONEDIR=../../JetClu+MidPoint-StandAlone/src
FASTJET_INCLUDE = -I../include 
# also put in the CDF cone cone
INCLUDE += $(FASTJET_INCLUDE) -I$(CDFCONEDIR)
#LDFLAGS += -L../lib -lfastjet
LDFLAGS += -L../lib -lfastjet -L$(CDFCONEDIR) -lJetAlgs
#LDFLAGS += -L../lib -lfastjet -L$(CDFCONEDIR) -lJetAlgs -ltcmalloc -lprofiler

# get google malloc for debugging purposes...
ifeq ($(shell whoami),salam)
  LDFLAGS += -L$(HOME)/software/local/lib -ltcmalloc -lprofiler
endif

#LDFLAGS += -L$(HOME)/software/local/lib 

PROG_SRCS = areas.cc areas-native.cc areas-histogramming.cc areas-test-mean-class.cc subtraction-tests.cc subtraction-tests-mass.cc subtraction-tests-inclpt.cc areas-2point.cc

SRCS =  CmdLine.cc  FlavourHolder.cc ClusterSequencePassiveArea.cc
OBJS = $(patsubst %.cc,%.o,$(SRCS))

areas: areas.o $(OBJS) ../lib/libfastjet.a
	$(CXX)  -o areas areas.o  $(OBJS)  $(LDFLAGS)

areas-native: areas-native.o $(OBJS) ../lib/libfastjet.a
	$(CXX)  -o areas-native areas-native.o $(OBJS)  $(LDFLAGS)

areas-histogramming: areas-histogramming.o $(OBJS) ../lib/libfastjet.a
	$(CXX)  -o areas-histogramming areas-histogramming.o $(OBJS)  $(LDFLAGS)

areas-test-mean-class: areas-test-mean-class.o $(OBJS) ../lib/libfastjet.a
	$(CXX)  -o areas-test-mean-class areas-test-mean-class.o $(OBJS)  $(LDFLAGS)

areas-2point: areas-2point.o $(OBJS) ../lib/libfastjet.a
	$(CXX)  -o areas-2point areas-2point.o $(OBJS)  $(LDFLAGS)

subtraction-tests: subtraction-tests.o $(OBJS) ../lib/libfastjet.a
	$(CXX)  -o subtraction-tests subtraction-tests.o $(OBJS)  $(LDFLAGS)

subtraction-tests-mass: subtraction-tests-mass.o $(OBJS) ../lib/libfastjet.a
	$(CXX)  -o subtraction-tests-mass subtraction-tests-mass.o $(OBJS)  $(LDFLAGS)

subtraction-tests-ttbar: subtraction-tests-ttbar.o $(OBJS) ../lib/libfastjet.a
	$(CXX)  -o subtraction-tests-ttbar subtraction-tests-ttbar.o $(OBJS)  $(LDFLAGS)

subtraction-tests-inclpt: subtraction-tests-inclpt.o $(OBJS) ../lib/libfastjet.a
	$(CXX)  -o subtraction-tests-inclpt subtraction-tests-inclpt.o $(OBJS)  $(LDFLAGS)


areas-empty-event: areas-empty-event.o $(OBJS) ../lib/libfastjet.a
	$(CXX)  -o areas-empty-event areas-empty-event.o $(OBJS)  $(LDFLAGS)

clean: 
	rm -f *.o

realclean: clean
	rm -f   areas 

depend:
	makedepend $(FASTJET_INCLUDE) --   -- $(SRCS) $(PROG_SRCS)
# DO NOT DELETE

CmdLine.o: CmdLine.hh
FjClusterSequenceWithArea.o: FjClusterSequenceWithArea.hh
FjClusterSequenceWithArea.o: ../include/FjPseudoJet.hh
FjClusterSequenceWithArea.o: ../include/numconsts.hh
FjClusterSequenceWithArea.o: ../include/FjClusterSequence.hh
FjClusterSequenceWithArea.o: ../include/DynamicNearestNeighbours.hh
FjClusterSequenceWithArea.o: ../include/FjError.hh
FjClusterSequenceWithArea.o: ../include/FjJetDefinition.hh
FjClusterSequenceWithMeanArea.o: ../include/FjPseudoJet.hh
FjClusterSequenceWithMeanArea.o: ../include/numconsts.hh
FjClusterSequenceWithMeanArea.o: ../include/FjClusterSequence.hh
FjClusterSequenceWithMeanArea.o: ../include/DynamicNearestNeighbours.hh
FjClusterSequenceWithMeanArea.o: ../include/FjError.hh
FjClusterSequenceWithMeanArea.o: ../include/FjJetDefinition.hh
FjClusterSequenceWithMeanArea.o: FjClusterSequenceWithMeanArea.hh
FjClusterSequenceWithMeanArea.o: FjClusterSequenceWithArea.hh
FlavourHolder.o: FlavourHolder.hh
FjClusterSequenceWithPassiveArea.o: FjClusterSequenceWithPassiveArea.hh
FjClusterSequenceWithPassiveArea.o: ../include/FjPseudoJet.hh
FjClusterSequenceWithPassiveArea.o: ../include/numconsts.hh
FjClusterSequenceWithPassiveArea.o: ../include/FjClusterSequence.hh
FjClusterSequenceWithPassiveArea.o: ../include/DynamicNearestNeighbours.hh
FjClusterSequenceWithPassiveArea.o: ../include/FjError.hh
FjClusterSequenceWithPassiveArea.o: ../include/FjJetDefinition.hh
areas.o: ../include/FjPseudoJet.hh ../include/numconsts.hh
areas.o: ../include/FjClusterSequence.hh
areas.o: ../include/DynamicNearestNeighbours.hh ../include/FjError.hh
areas.o: ../include/FjJetDefinition.hh CmdLine.hh
areas-native.o: ../include/FjPseudoJet.hh ../include/numconsts.hh
areas-native.o: ../include/FjClusterSequence.hh
areas-native.o: ../include/DynamicNearestNeighbours.hh ../include/FjError.hh
areas-native.o: ../include/FjJetDefinition.hh FjClusterSequenceWithArea.hh
areas-native.o: CmdLine.hh
areas-histogramming.o: ../include/FjPseudoJet.hh ../include/numconsts.hh
areas-histogramming.o: ../include/FjClusterSequence.hh
areas-histogramming.o: ../include/DynamicNearestNeighbours.hh
areas-histogramming.o: ../include/FjError.hh ../include/FjJetDefinition.hh
areas-histogramming.o: FjClusterSequenceWithArea.hh CmdLine.hh
areas-test-mean-class.o: ../include/FjPseudoJet.hh ../include/numconsts.hh
areas-test-mean-class.o: ../include/FjClusterSequence.hh
areas-test-mean-class.o: ../include/DynamicNearestNeighbours.hh
areas-test-mean-class.o: ../include/FjError.hh ../include/FjJetDefinition.hh
areas-test-mean-class.o: FjClusterSequenceWithMeanArea.hh
areas-test-mean-class.o: FjClusterSequenceWithArea.hh CmdLine.hh
subtraction-tests.o: ../include/FjPseudoJet.hh ../include/numconsts.hh
subtraction-tests.o: ../include/FjClusterSequence.hh
subtraction-tests.o: ../include/DynamicNearestNeighbours.hh
subtraction-tests.o: ../include/FjError.hh ../include/FjJetDefinition.hh
subtraction-tests.o: FjClusterSequenceWithMeanArea.hh
subtraction-tests.o: FjClusterSequenceWithArea.hh CmdLine.hh CSHisto.hh
subtraction-tests-mass.o: ../include/FjPseudoJet.hh ../include/numconsts.hh
subtraction-tests-mass.o: ../include/FjClusterSequence.hh
subtraction-tests-mass.o: ../include/DynamicNearestNeighbours.hh
subtraction-tests-mass.o: ../include/FjError.hh ../include/FjJetDefinition.hh
subtraction-tests-mass.o: FjClusterSequenceWithMeanArea.hh
subtraction-tests-mass.o: FjClusterSequenceWithArea.hh CmdLine.hh CSHisto.hh
subtraction-tests-inclpt.o: ../include/FjPseudoJet.hh ../include/numconsts.hh
subtraction-tests-inclpt.o: ../include/FjClusterSequence.hh
subtraction-tests-inclpt.o: ../include/DynamicNearestNeighbours.hh
subtraction-tests-inclpt.o: ../include/FjError.hh
subtraction-tests-inclpt.o: ../include/FjJetDefinition.hh
subtraction-tests-inclpt.o: FjClusterSequenceWithMeanArea.hh
subtraction-tests-inclpt.o: FjClusterSequenceWithArea.hh CmdLine.hh
subtraction-tests-inclpt.o: CSHisto.hh
areas-2point.o: ../include/FjPseudoJet.hh ../include/numconsts.hh
areas-2point.o: ../include/FjClusterSequence.hh
areas-2point.o: ../include/DynamicNearestNeighbours.hh ../include/FjError.hh
areas-2point.o: ../include/FjJetDefinition.hh
areas-2point.o: FjClusterSequenceWithMeanArea.hh FjClusterSequenceWithArea.hh
areas-2point.o: FjClusterSequenceWithPassiveArea.hh CmdLine.hh
