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
FlavourHolder.o: FlavourHolder.hh
ClusterSequencePassiveArea.o: ClusterSequencePassiveArea.hh
ClusterSequencePassiveArea.o: ../include/fastjet/PseudoJet.hh
ClusterSequencePassiveArea.o: ../include/fastjet/internal/numconsts.hh
ClusterSequencePassiveArea.o: ../include/fastjet/internal/base.hh
ClusterSequencePassiveArea.o: ../include/fastjet/ClusterSequenceWithArea.hh
ClusterSequencePassiveArea.o: ../include/fastjet/ClusterSequence.hh
ClusterSequencePassiveArea.o: ../include/fastjet/internal/DynamicNearestNeighbours.hh
ClusterSequencePassiveArea.o: ../include/fastjet/Error.hh
ClusterSequencePassiveArea.o: ../include/fastjet/JetDefinition.hh
areas.o: ../include/FjPseudoJet.hh ../include/fastjet/PseudoJet.hh
areas.o: ../include/fastjet/internal/numconsts.hh
areas.o: ../include/fastjet/internal/base.hh ../include/FjClusterSequence.hh
areas.o: ../include/fastjet/ClusterSequence.hh
areas.o: ../include/fastjet/internal/DynamicNearestNeighbours.hh
areas.o: ../include/fastjet/Error.hh ../include/fastjet/JetDefinition.hh
areas.o: CmdLine.hh
areas-native.o: ../include/FjPseudoJet.hh ../include/fastjet/PseudoJet.hh
areas-native.o: ../include/fastjet/internal/numconsts.hh
areas-native.o: ../include/fastjet/internal/base.hh
areas-native.o: ../include/FjClusterSequence.hh
areas-native.o: ../include/fastjet/ClusterSequence.hh
areas-native.o: ../include/fastjet/internal/DynamicNearestNeighbours.hh
areas-native.o: ../include/fastjet/Error.hh
areas-native.o: ../include/fastjet/JetDefinition.hh CmdLine.hh
areas-histogramming.o: ../include/FjPseudoJet.hh
areas-histogramming.o: ../include/fastjet/PseudoJet.hh
areas-histogramming.o: ../include/fastjet/internal/numconsts.hh
areas-histogramming.o: ../include/fastjet/internal/base.hh
areas-histogramming.o: ../include/FjClusterSequence.hh
areas-histogramming.o: ../include/fastjet/ClusterSequence.hh
areas-histogramming.o: ../include/fastjet/internal/DynamicNearestNeighbours.hh
areas-histogramming.o: ../include/fastjet/Error.hh
areas-histogramming.o: ../include/fastjet/JetDefinition.hh CmdLine.hh
areas-test-mean-class.o: ../include/FjPseudoJet.hh
areas-test-mean-class.o: ../include/fastjet/PseudoJet.hh
areas-test-mean-class.o: ../include/fastjet/internal/numconsts.hh
areas-test-mean-class.o: ../include/fastjet/internal/base.hh
areas-test-mean-class.o: ../include/FjClusterSequence.hh
areas-test-mean-class.o: ../include/fastjet/ClusterSequence.hh
areas-test-mean-class.o: ../include/fastjet/internal/DynamicNearestNeighbours.hh
areas-test-mean-class.o: ../include/fastjet/Error.hh
areas-test-mean-class.o: ../include/fastjet/JetDefinition.hh CmdLine.hh
subtraction-tests.o: ../include/FjPseudoJet.hh
subtraction-tests.o: ../include/fastjet/PseudoJet.hh
subtraction-tests.o: ../include/fastjet/internal/numconsts.hh
subtraction-tests.o: ../include/fastjet/internal/base.hh
subtraction-tests.o: ../include/FjClusterSequence.hh
subtraction-tests.o: ../include/fastjet/ClusterSequence.hh
subtraction-tests.o: ../include/fastjet/internal/DynamicNearestNeighbours.hh
subtraction-tests.o: ../include/fastjet/Error.hh
subtraction-tests.o: ../include/fastjet/JetDefinition.hh CmdLine.hh
subtraction-tests.o: CSHisto.hh
subtraction-tests-mass.o: ../include/FjPseudoJet.hh
subtraction-tests-mass.o: ../include/fastjet/PseudoJet.hh
subtraction-tests-mass.o: ../include/fastjet/internal/numconsts.hh
subtraction-tests-mass.o: ../include/fastjet/internal/base.hh
subtraction-tests-mass.o: ../include/FjClusterSequence.hh
subtraction-tests-mass.o: ../include/fastjet/ClusterSequence.hh
subtraction-tests-mass.o: ../include/fastjet/internal/DynamicNearestNeighbours.hh
subtraction-tests-mass.o: ../include/fastjet/Error.hh
subtraction-tests-mass.o: ../include/fastjet/JetDefinition.hh CmdLine.hh
subtraction-tests-mass.o: CSHisto.hh
subtraction-tests-inclpt.o: ../include/FjPseudoJet.hh
subtraction-tests-inclpt.o: ../include/fastjet/PseudoJet.hh
subtraction-tests-inclpt.o: ../include/fastjet/internal/numconsts.hh
subtraction-tests-inclpt.o: ../include/fastjet/internal/base.hh
subtraction-tests-inclpt.o: ../include/FjClusterSequence.hh
subtraction-tests-inclpt.o: ../include/fastjet/ClusterSequence.hh
subtraction-tests-inclpt.o: ../include/fastjet/internal/DynamicNearestNeighbours.hh
subtraction-tests-inclpt.o: ../include/fastjet/Error.hh
subtraction-tests-inclpt.o: ../include/fastjet/JetDefinition.hh CmdLine.hh
subtraction-tests-inclpt.o: CSHisto.hh
areas-2point.o: ../include/fastjet/PseudoJet.hh
areas-2point.o: ../include/fastjet/internal/numconsts.hh
areas-2point.o: ../include/fastjet/internal/base.hh
areas-2point.o: ../include/fastjet/ClusterSequence.hh
areas-2point.o: ../include/fastjet/internal/DynamicNearestNeighbours.hh
areas-2point.o: ../include/fastjet/Error.hh
areas-2point.o: ../include/fastjet/JetDefinition.hh
areas-2point.o: ../include/fastjet/ClusterSequenceActiveArea.hh
areas-2point.o: ../include/fastjet/ClusterSequenceWithArea.hh
areas-2point.o: ../include/fastjet/ClusterSequenceActiveAreaExplicitGhosts.hh
areas-2point.o: ../include/fastjet/ActiveAreaSpec.hh
areas-2point.o: ClusterSequencePassiveArea.hh CmdLine.hh
