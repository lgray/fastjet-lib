# fastjet/example/Makefile

# include the Makefile in the top directory, which has the bits and pieces
# needed to get CGAL working
include ../Makefile

CDFCONEDIR=../../JetClu+MidPoint-StandAlone/src
FASTJET_INCLUDE = -I../include 
# also put in the CDF cone cone
#INCLUDE += $(FASTJET_INCLUDE) -I$(CDFCONEDIR) -I../plugins/SISCone -I../plugins/CDCones
INCLUDE += $(FASTJET_INCLUDE)  -I../plugins/SISCone -I../plugins/CDFCones
#LDFLAGS += -L../lib -lfastjet
#LDFLAGS += -L../lib -lfastjet -L$(CDFCONEDIR) -lJetAlgs -L../plugins/SISCone -l SISConePlugin -L../plugins/CDFCones -lCDFCones
LDFLAGS += -L../lib -lfastjet  -L../plugins/SISCone -l SISConePlugin -L../plugins/CDFCones -lCDFConesPlugin
#LDFLAGS += -L../lib -lfastjet -L$(CDFCONEDIR) -lJetAlgs -ltcmalloc -lprofiler

# get google malloc for debugging purposes...
ifeq ($(shell whoami),salam)
  LDFLAGS += -L$(HOME)/software/local/lib -ltcmalloc -lprofiler
endif

#LDFLAGS += -L$(HOME)/software/local/lib 

PROG_SRCS = areas-hist.cc areas.cc areas-native.cc areas-histogramming.cc areas-test-mean-class.cc subtraction-tests.cc subtraction-tests-mass.cc subtraction-tests-inclpt.cc areas-2point.cc subtraction-correlation.cc

MINSRCS =  CmdLine.cc  FlavourHolder.cc jet_def_from_cmdline.cc
#SRCS =  $(MINSRCS) ClusterSequencePassiveArea.cc
SRCS =  $(MINSRCS) 
MINOBJS = $(patsubst %.cc,%.o,$(MINSRCS))
OBJS = $(patsubst %.cc,%.o,$(SRCS))

areas: areas.o $(OBJS) ../lib/libfastjet.a
	$(CXX)  -o areas areas.o  $(OBJS)  $(LDFLAGS)

areas-native: areas-native.o $(OBJS) ../lib/libfastjet.a
	$(CXX)  -o areas-native areas-native.o $(OBJS)  $(LDFLAGS)

areas-histogramming: areas-histogramming.o $(OBJS) ../lib/libfastjet.a
	$(CXX)  -o areas-histogramming areas-histogramming.o $(OBJS)  $(LDFLAGS)

areas-hist: areas-hist.o $(MINOBJS) ../lib/libfastjet.a
	$(CXX)  -o areas-hist areas-hist.o $(MINOBJS)  $(LDFLAGS)

areas-test-mean-class: areas-test-mean-class.o $(OBJS) ../lib/libfastjet.a
	$(CXX)  -o areas-test-mean-class areas-test-mean-class.o $(OBJS)  $(LDFLAGS)

areas-2point: areas-2point.o $(OBJS) ../lib/libfastjet.a
	$(CXX)  -o areas-2point areas-2point.o $(OBJS)  $(LDFLAGS)

subtraction-tests: subtraction-tests.o $(OBJS) ../lib/libfastjet.a
	$(CXX)  -o subtraction-tests subtraction-tests.o $(OBJS)  $(LDFLAGS)

subtraction-correlation: subtraction-correlation.o $(OBJS) ../lib/libfastjet.a
	$(CXX)  -o subtraction-correlation subtraction-correlation.o $(OBJS)  $(LDFLAGS)

subtraction-tests-mass: subtraction-tests-mass.o $(OBJS) ../lib/libfastjet.a
	$(CXX)  -o subtraction-tests-mass subtraction-tests-mass.o $(OBJS)  $(LDFLAGS)

subtraction-tests-ttbar: subtraction-tests-ttbar.o $(OBJS) ../lib/libfastjet.a
	$(CXX)  -o subtraction-tests-ttbar subtraction-tests-ttbar.o $(OBJS)  $(LDFLAGS)

subtraction-tests-inclpt: subtraction-tests-inclpt.o $(OBJS) ../lib/libfastjet.a
	$(CXX)  -o subtraction-tests-inclpt subtraction-tests-inclpt.o $(OBJS)  $(LDFLAGS)

subtraction-tests-inclpt-2pass: subtraction-tests-inclpt-2pass.o $(OBJS) ../lib/libfastjet.a
	$(CXX)  -o subtraction-tests-inclpt-2pass  subtraction-tests-inclpt-2pass.o $(OBJS)  $(LDFLAGS)

subtraction-tests-HIeff: subtraction-tests-HIeff.o $(OBJS) ../lib/libfastjet.a
	$(CXX)  -o subtraction-tests-HIeff subtraction-tests-HIeff.o $(OBJS)  $(LDFLAGS)

subtraction-HIeff-iter: subtraction-HIeff-iter.o  CmdLine.o ../lib/libfastjet.a
	$(CXX)  -o subtraction-HIeff-iter subtraction-HIeff-iter.o  CmdLine.o $(LDFLAGS)

subtraction-HIeff-native: subtraction-HIeff-native.o  CmdLine.o ../lib/libfastjet.a
	$(CXX)  -o subtraction-HIeff-native subtraction-HIeff-native.o  CmdLine.o $(LDFLAGS)

areas-empty-event: areas-empty-event.o $(OBJS) ../lib/libfastjet.a
	$(CXX)  -o areas-empty-event areas-empty-event.o $(OBJS)  $(LDFLAGS)

areas-1gluon-for-root: areas-1gluon-for-root.o $(OBJS) ../lib/libfastjet.a
	$(CXX)  -o areas-1gluon-for-root areas-1gluon-for-root.o $(OBJS)  $(LDFLAGS)

areas-anomal-dim: areas-anomal-dim.o  CmdLine.o ../lib/libfastjet.a
	$(CXX)  -o areas-anomal-dim areas-anomal-dim.o  CmdLine.o $(LDFLAGS)

areas-fractal-scaling: areas-fractal-scaling.o CmdLine.o jet_def_from_cmdline.o ../lib/libfastjet.a
	$(CXX)  -o areas-fractal-scaling areas-fractal-scaling.o  CmdLine.o jet_def_from_cmdline.o $(LDFLAGS)

clean: 
	rm -f *.o

realclean: clean
	rm -f   areas 

depend:
	makedepend $(FASTJET_INCLUDE) --   -- $(SRCS) $(PROG_SRCS)
# DO NOT DELETE

CmdLine.o: CmdLine.hh
FlavourHolder.o: FlavourHolder.hh
jet_def_from_cmdline.o: jet_def_from_cmdline.hh CmdLine.hh
jet_def_from_cmdline.o: ../include/fastjet/JetDefinition.hh
jet_def_from_cmdline.o: ../include/fastjet/internal/numconsts.hh
jet_def_from_cmdline.o: ../include/fastjet/internal/base.hh
jet_def_from_cmdline.o: ../include/fastjet/PseudoJet.hh
areas-hist.o: ../include/fastjet/PseudoJet.hh
areas-hist.o: ../include/fastjet/internal/numconsts.hh
areas-hist.o: ../include/fastjet/internal/base.hh
areas-hist.o: ../include/fastjet/ClusterSequence.hh
areas-hist.o: ../include/fastjet/internal/DynamicNearestNeighbours.hh
areas-hist.o: ../include/fastjet/Error.hh ../include/fastjet/JetDefinition.hh
areas-hist.o: ../include/fastjet/ClusterSequenceArea.hh
areas-hist.o: ../include/fastjet/ClusterSequenceAreaBase.hh
areas-hist.o: ../include/fastjet/ClusterSequenceActiveArea.hh
areas-hist.o: ../include/fastjet/ClusterSequenceActiveAreaExplicitGhosts.hh
areas-hist.o: ../include/fastjet/GhostedAreaSpec.hh
areas-hist.o: ../include/fastjet/internal/BasicRandom.hh
areas-hist.o: ../include/fastjet/ActiveAreaSpec.hh
areas-hist.o: ../include/fastjet/ClusterSequenceWithArea.hh
areas-hist.o: ../include/fastjet/ClusterSequencePassiveArea.hh
areas-hist.o: ../include/fastjet/ClusterSequence1GhostPassiveArea.hh
areas-hist.o: ../include/fastjet/ClusterSequenceVoronoiArea.hh
areas-hist.o: ../include/fastjet/AreaDefinition.hh CmdLine.hh SimpleHist.hh
areas-hist.o: jet_def_from_cmdline.hh
areas.o: ../include/fastjet/PseudoJet.hh
areas.o: ../include/fastjet/internal/numconsts.hh
areas.o: ../include/fastjet/internal/base.hh
areas.o: ../include/fastjet/ClusterSequence.hh
areas.o: ../include/fastjet/internal/DynamicNearestNeighbours.hh
areas.o: ../include/fastjet/Error.hh ../include/fastjet/JetDefinition.hh
areas.o: CmdLine.hh
areas-native.o: ../include/fastjet/PseudoJet.hh
areas-native.o: ../include/fastjet/internal/numconsts.hh
areas-native.o: ../include/fastjet/internal/base.hh
areas-native.o: ../include/fastjet/ClusterSequence.hh
areas-native.o: ../include/fastjet/internal/DynamicNearestNeighbours.hh
areas-native.o: ../include/fastjet/Error.hh
areas-native.o: ../include/fastjet/JetDefinition.hh
areas-native.o: ../include/fastjet/ClusterSequenceActiveAreaExplicitGhosts.hh
areas-native.o: ../include/fastjet/ClusterSequenceAreaBase.hh
areas-native.o: ../include/fastjet/GhostedAreaSpec.hh
areas-native.o: ../include/fastjet/internal/BasicRandom.hh CmdLine.hh
areas-histogramming.o: ../include/fastjet/PseudoJet.hh
areas-histogramming.o: ../include/fastjet/internal/numconsts.hh
areas-histogramming.o: ../include/fastjet/internal/base.hh
areas-histogramming.o: ../include/fastjet/ClusterSequence.hh
areas-histogramming.o: ../include/fastjet/internal/DynamicNearestNeighbours.hh
areas-histogramming.o: ../include/fastjet/Error.hh
areas-histogramming.o: ../include/fastjet/JetDefinition.hh
areas-histogramming.o: ../include/fastjet/ClusterSequenceActiveAreaExplicitGhosts.hh
areas-histogramming.o: ../include/fastjet/ClusterSequenceAreaBase.hh
areas-histogramming.o: ../include/fastjet/GhostedAreaSpec.hh
areas-histogramming.o: ../include/fastjet/internal/BasicRandom.hh CmdLine.hh
areas-test-mean-class.o: ../include/fastjet/PseudoJet.hh
areas-test-mean-class.o: ../include/fastjet/internal/numconsts.hh
areas-test-mean-class.o: ../include/fastjet/internal/base.hh
areas-test-mean-class.o: ../include/fastjet/ClusterSequence.hh
areas-test-mean-class.o: ../include/fastjet/internal/DynamicNearestNeighbours.hh
areas-test-mean-class.o: ../include/fastjet/Error.hh
areas-test-mean-class.o: ../include/fastjet/JetDefinition.hh
areas-test-mean-class.o: ../include/fastjet/ClusterSequenceActiveArea.hh
areas-test-mean-class.o: ../include/fastjet/ClusterSequenceAreaBase.hh
areas-test-mean-class.o: ../include/fastjet/ClusterSequenceActiveAreaExplicitGhosts.hh
areas-test-mean-class.o: ../include/fastjet/GhostedAreaSpec.hh
areas-test-mean-class.o: ../include/fastjet/internal/BasicRandom.hh
areas-test-mean-class.o: ../include/fastjet/ActiveAreaSpec.hh
areas-test-mean-class.o: ../include/fastjet/ClusterSequenceWithArea.hh
areas-test-mean-class.o: ClusterSequencePassiveArea.hh CmdLine.hh
subtraction-tests.o: ../include/fastjet/PseudoJet.hh
subtraction-tests.o: ../include/fastjet/internal/numconsts.hh
subtraction-tests.o: ../include/fastjet/internal/base.hh
subtraction-tests.o: ../include/fastjet/ClusterSequence.hh
subtraction-tests.o: ../include/fastjet/internal/DynamicNearestNeighbours.hh
subtraction-tests.o: ../include/fastjet/Error.hh
subtraction-tests.o: ../include/fastjet/JetDefinition.hh
subtraction-tests.o: ../include/fastjet/ClusterSequenceActiveArea.hh
subtraction-tests.o: ../include/fastjet/ClusterSequenceAreaBase.hh
subtraction-tests.o: ../include/fastjet/ClusterSequenceActiveAreaExplicitGhosts.hh
subtraction-tests.o: ../include/fastjet/GhostedAreaSpec.hh
subtraction-tests.o: ../include/fastjet/internal/BasicRandom.hh
subtraction-tests.o: ../include/fastjet/ActiveAreaSpec.hh
subtraction-tests.o: ../include/fastjet/ClusterSequenceWithArea.hh CmdLine.hh
subtraction-tests.o: jet_def_from_cmdline.hh CSHisto.hh
subtraction-tests-mass.o: ../include/fastjet/PseudoJet.hh
subtraction-tests-mass.o: ../include/fastjet/internal/numconsts.hh
subtraction-tests-mass.o: ../include/fastjet/internal/base.hh
subtraction-tests-mass.o: ../include/fastjet/ClusterSequence.hh
subtraction-tests-mass.o: ../include/fastjet/internal/DynamicNearestNeighbours.hh
subtraction-tests-mass.o: ../include/fastjet/Error.hh
subtraction-tests-mass.o: ../include/fastjet/JetDefinition.hh
subtraction-tests-mass.o: ../include/fastjet/ClusterSequenceActiveArea.hh
subtraction-tests-mass.o: ../include/fastjet/ClusterSequenceAreaBase.hh
subtraction-tests-mass.o: ../include/fastjet/ClusterSequenceActiveAreaExplicitGhosts.hh
subtraction-tests-mass.o: ../include/fastjet/GhostedAreaSpec.hh
subtraction-tests-mass.o: ../include/fastjet/internal/BasicRandom.hh
subtraction-tests-mass.o: ../include/fastjet/ActiveAreaSpec.hh
subtraction-tests-mass.o: ../include/fastjet/ClusterSequenceWithArea.hh
subtraction-tests-mass.o: CmdLine.hh CSHisto.hh
subtraction-tests-inclpt.o: ../include/fastjet/PseudoJet.hh
subtraction-tests-inclpt.o: ../include/fastjet/internal/numconsts.hh
subtraction-tests-inclpt.o: ../include/fastjet/internal/base.hh
subtraction-tests-inclpt.o: ../include/fastjet/ClusterSequence.hh
subtraction-tests-inclpt.o: ../include/fastjet/internal/DynamicNearestNeighbours.hh
subtraction-tests-inclpt.o: ../include/fastjet/Error.hh
subtraction-tests-inclpt.o: ../include/fastjet/JetDefinition.hh
subtraction-tests-inclpt.o: ../include/fastjet/ClusterSequenceActiveArea.hh
subtraction-tests-inclpt.o: ../include/fastjet/ClusterSequenceAreaBase.hh
subtraction-tests-inclpt.o: ../include/fastjet/ClusterSequenceActiveAreaExplicitGhosts.hh
subtraction-tests-inclpt.o: ../include/fastjet/GhostedAreaSpec.hh
subtraction-tests-inclpt.o: ../include/fastjet/internal/BasicRandom.hh
subtraction-tests-inclpt.o: ../include/fastjet/ActiveAreaSpec.hh
subtraction-tests-inclpt.o: ../include/fastjet/ClusterSequenceWithArea.hh
subtraction-tests-inclpt.o: CmdLine.hh CSHisto.hh SimpleHist.hh
areas-2point.o: ../include/fastjet/PseudoJet.hh
areas-2point.o: ../include/fastjet/internal/numconsts.hh
areas-2point.o: ../include/fastjet/internal/base.hh
areas-2point.o: ../include/fastjet/ClusterSequence.hh
areas-2point.o: ../include/fastjet/internal/DynamicNearestNeighbours.hh
areas-2point.o: ../include/fastjet/Error.hh
areas-2point.o: ../include/fastjet/JetDefinition.hh
areas-2point.o: ../include/fastjet/ClusterSequenceActiveArea.hh
areas-2point.o: ../include/fastjet/ClusterSequenceAreaBase.hh
areas-2point.o: ../include/fastjet/ClusterSequenceActiveAreaExplicitGhosts.hh
areas-2point.o: ../include/fastjet/GhostedAreaSpec.hh
areas-2point.o: ../include/fastjet/internal/BasicRandom.hh
areas-2point.o: ../include/fastjet/ActiveAreaSpec.hh
areas-2point.o: ../include/fastjet/ClusterSequenceWithArea.hh CmdLine.hh
subtraction-correlation.o: ../include/fastjet/PseudoJet.hh
subtraction-correlation.o: ../include/fastjet/internal/numconsts.hh
subtraction-correlation.o: ../include/fastjet/internal/base.hh
subtraction-correlation.o: ../include/fastjet/ClusterSequence.hh
subtraction-correlation.o: ../include/fastjet/internal/DynamicNearestNeighbours.hh
subtraction-correlation.o: ../include/fastjet/Error.hh
subtraction-correlation.o: ../include/fastjet/JetDefinition.hh
subtraction-correlation.o: ../include/fastjet/ClusterSequenceWithArea.hh
subtraction-correlation.o: ../include/fastjet/ClusterSequenceAreaBase.hh
subtraction-correlation.o: CmdLine.hh jet_def_from_cmdline.hh CSHisto.hh
