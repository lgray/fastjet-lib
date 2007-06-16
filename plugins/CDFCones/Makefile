
# include basic definitions
include ../../Makefile

SRCS = CDFJetCluPlugin.cc CDFMidPointPlugin.cc
OBJS = $(patsubst %.cc,%.o,$(SRCS))

# fastjet includes
INCLUDE += -I../../include

# CDFcode/ includes
INCLUDE += -ICDFcode

libCDFConesPlugin.a: $(OBJS) cdfcode-dir
	ar cru libCDFConesPlugin.a $(OBJS) CDFcode/*.o
	ranlib libCDFConesPlugin.a

cdfcode-dir:
	cd CDFcode && $(MAKE)

clean: cdfcode-clean
	rm -f $(OBJS)

cdfcode-clean:
	cd CDFcode && $(MAKE) clean

distclean: clean
	rm -f libCDFConesPlugin.a  *~

depend:
	makedepend  $(INCLUDE) -I. --   -- $(SRCS)


# DO NOT DELETE

CDFJetCluPlugin.o: CDFJetCluPlugin.hh ../../include/fastjet/JetDefinition.hh
CDFJetCluPlugin.o: ../../include/fastjet/internal/numconsts.hh
CDFJetCluPlugin.o: ../../include/fastjet/internal/base.hh
CDFJetCluPlugin.o: ../../include/fastjet/PseudoJet.hh
CDFJetCluPlugin.o: ../../include/fastjet/ClusterSequence.hh
CDFJetCluPlugin.o: ../../include/fastjet/internal/DynamicNearestNeighbours.hh
CDFJetCluPlugin.o: ../../include/fastjet/Error.hh CDFcode/JetCluAlgorithm.hh
CDFJetCluPlugin.o: CDFcode/PhysicsTower.hh CDFcode/LorentzVector.hh
CDFJetCluPlugin.o: CDFcode/CalTower.hh CDFcode/Cluster.hh CDFcode/Centroid.hh
CDFMidPointPlugin.o: CDFMidPointPlugin.hh
CDFMidPointPlugin.o: ../../include/fastjet/JetDefinition.hh
CDFMidPointPlugin.o: ../../include/fastjet/internal/numconsts.hh
CDFMidPointPlugin.o: ../../include/fastjet/internal/base.hh
CDFMidPointPlugin.o: ../../include/fastjet/PseudoJet.hh
CDFMidPointPlugin.o: ../../include/fastjet/ClusterSequence.hh
CDFMidPointPlugin.o: ../../include/fastjet/internal/DynamicNearestNeighbours.hh
CDFMidPointPlugin.o: ../../include/fastjet/Error.hh
CDFMidPointPlugin.o: CDFcode/MidPointAlgorithm.hh CDFcode/PhysicsTower.hh
CDFMidPointPlugin.o: CDFcode/LorentzVector.hh CDFcode/CalTower.hh
CDFMidPointPlugin.o: CDFcode/Cluster.hh CDFcode/Centroid.hh
