#CXXFLAGS = -Wall -O3 -march=i686
#CXXFLAGSmidpoint = -Wall -O3 -march=i686

include ../../../Makefile

# with some gcc's midpoint can be a lot slower at -O3 than -O2, so
# ensure we only use -O2 for it
CXXFLAGSmidpoint = $(patsubst -O3,-O2,$(CXXFLAGS))

all : libJetAlgs.a

CalTower.o : CalTower.cc CalTower.hh
	$(CXX) $(CXXFLAGS) -c CalTower.cc

Centroid.o : Centroid.cc Centroid.hh
	$(CXX) $(CXXFLAGS) -c Centroid.cc

Cluster.o : Cluster.cc Cluster.hh CalTower.hh Centroid.hh LorentzVector.hh PhysicsTower.hh
	$(CXX) $(CXXFLAGS) -c Cluster.cc

ClusterComparisons.o : ClusterComparisons.cc ClusterComparisons.hh CalTower.hh Centroid.hh Cluster.hh LorentzVector.hh PhysicsTower.hh
	$(CXX) $(CXXFLAGS) -c ClusterComparisons.cc

JetCluAlgorithm.o : JetCluAlgorithm.cc JetCluAlgorithm.hh CalTower.hh Centroid.hh Cluster.hh ClusterComparisons.hh LorentzVector.hh PhysicsTower.hh
	$(CXX) $(CXXFLAGS) -c JetCluAlgorithm.cc

LorentzVector.o : LorentzVector.cc LorentzVector.hh
	$(CXX) $(CXXFLAGS) -c LorentzVector.cc

#MidPointAlgorithm.o : MidPointAlgorithm.cc MidPointAlgorithm.hh CalTower.hh Centroid.hh Cluster.hh ClusterComparisons.hh LorentzVector.hh PhysicsTower.hh
#	$(CXX) $(CXXFLAGS) -c MidPointAlgorithm.cc

# below we use the special CXXFLAGS designed for the midpoint case
MidPointAlgorithm.o : MidPointAlgorithm.cc MidPointAlgorithm.hh CalTower.hh Centroid.hh Cluster.hh ClusterComparisons.hh LorentzVector.hh PhysicsTower.hh
	$(CXX) $(CXXFLAGSmidpoint) -c MidPointAlgorithm.cc

PhysicsTower.o : PhysicsTower.cc PhysicsTower.hh CalTower.hh LorentzVector.hh
	$(CXX) $(CXXFLAGS) -c PhysicsTower.cc

libJetAlgs.a : CalTower.o Centroid.o Cluster.o ClusterComparisons.o JetCluAlgorithm.o LorentzVector.o MidPointAlgorithm.o PhysicsTower.o
	ar rs libJetAlgs.a CalTower.o Centroid.o Cluster.o ClusterComparisons.o JetCluAlgorithm.o LorentzVector.o MidPointAlgorithm.o PhysicsTower.o

clean:
	rm -f CalTower.o Centroid.o Cluster.o ClusterComparisons.o JetCluAlgorithm.o LorentzVector.o MidPointAlgorithm.o PhysicsTower.o libJetAlgs.a
