# fastjet/src/Makefile

# include the Makefile in the top directory, which has the bits and pieces
# needed to get CGAL working
include ../Makefile

FASTJET_INCLUDE = -I../include
INCLUDE += $(FASTJET_INCLUDE)
#INCLUDE = 

SRCS = DnnPlane.cc Dnn4piCylinder.cc Dnn3piCylinder.cc Dnn2piCylinder.cc ClusterSequence.cc PseudoJet.cc ClusterSequence_N2.cc ClusterSequence_TiledN2.cc ClusterSequence_Delaunay.cc ClusterSequence_DumbN3.cc ClusterSequence_CP2DChan.cc ClosestPair2D.cc MinHeap.cc ClusterSequenceActiveAreaExplicitGhosts.cc ActiveAreaSpec.cc ClusterSequenceActiveArea.cc BasicRandom.cc JetDefinition.cc Error.cc

OBJS = $(patsubst %.cc,%.o,$(SRCS))



all: libfastjet.a

libfastjet.a: $(OBJS)
	ar cru libfastjet.a $(OBJS)
	ranlib libfastjet.a

install: libfastjet.a
	cp -p libfastjet.a ../lib/

clean:
	rm -f $(OBJS) libfastjet.a


depend:
	makedepend $(FASTJET_INCLUDE) --   -- $(SRCS)
# DO NOT DELETE

DnnPlane.o: ../include/fastjet/internal/DnnPlane.hh
DnnPlane.o: ../include/fastjet/internal/Triangulation.hh
DnnPlane.o: ../include/fastjet/internal/base.hh
DnnPlane.o: ../include/fastjet/internal/DynamicNearestNeighbours.hh
DnnPlane.o: ../include/fastjet/internal/numconsts.hh
Dnn4piCylinder.o: ../include/fastjet/internal/Dnn4piCylinder.hh
Dnn4piCylinder.o: ../include/fastjet/internal/DynamicNearestNeighbours.hh
Dnn4piCylinder.o: ../include/fastjet/internal/numconsts.hh
Dnn4piCylinder.o: ../include/fastjet/internal/base.hh
Dnn4piCylinder.o: ../include/fastjet/internal/DnnPlane.hh
Dnn4piCylinder.o: ../include/fastjet/internal/Triangulation.hh
Dnn3piCylinder.o: ../include/fastjet/internal/Dnn3piCylinder.hh
Dnn3piCylinder.o: ../include/fastjet/internal/DynamicNearestNeighbours.hh
Dnn3piCylinder.o: ../include/fastjet/internal/numconsts.hh
Dnn3piCylinder.o: ../include/fastjet/internal/base.hh
Dnn3piCylinder.o: ../include/fastjet/internal/DnnPlane.hh
Dnn3piCylinder.o: ../include/fastjet/internal/Triangulation.hh
Dnn2piCylinder.o: ../include/fastjet/internal/Dnn2piCylinder.hh
Dnn2piCylinder.o: ../include/fastjet/internal/DynamicNearestNeighbours.hh
Dnn2piCylinder.o: ../include/fastjet/internal/numconsts.hh
Dnn2piCylinder.o: ../include/fastjet/internal/base.hh
Dnn2piCylinder.o: ../include/fastjet/internal/DnnPlane.hh
Dnn2piCylinder.o: ../include/fastjet/internal/Triangulation.hh
ClusterSequence.o: ../include/fastjet/Error.hh
ClusterSequence.o: ../include/fastjet/internal/base.hh
ClusterSequence.o: ../include/fastjet/PseudoJet.hh
ClusterSequence.o: ../include/fastjet/internal/numconsts.hh
ClusterSequence.o: ../include/fastjet/ClusterSequence.hh
ClusterSequence.o: ../include/fastjet/internal/DynamicNearestNeighbours.hh
ClusterSequence.o: ../include/fastjet/JetDefinition.hh
PseudoJet.o: ../include/fastjet/Error.hh ../include/fastjet/internal/base.hh
PseudoJet.o: ../include/fastjet/PseudoJet.hh
PseudoJet.o: ../include/fastjet/internal/numconsts.hh
ClusterSequence_N2.o: ../include/fastjet/PseudoJet.hh
ClusterSequence_N2.o: ../include/fastjet/internal/numconsts.hh
ClusterSequence_N2.o: ../include/fastjet/internal/base.hh
ClusterSequence_N2.o: ../include/fastjet/ClusterSequence.hh
ClusterSequence_N2.o: ../include/fastjet/internal/DynamicNearestNeighbours.hh
ClusterSequence_N2.o: ../include/fastjet/Error.hh
ClusterSequence_N2.o: ../include/fastjet/JetDefinition.hh
ClusterSequence_TiledN2.o: ../include/fastjet/PseudoJet.hh
ClusterSequence_TiledN2.o: ../include/fastjet/internal/numconsts.hh
ClusterSequence_TiledN2.o: ../include/fastjet/internal/base.hh
ClusterSequence_TiledN2.o: ../include/fastjet/ClusterSequence.hh
ClusterSequence_TiledN2.o: ../include/fastjet/internal/DynamicNearestNeighbours.hh
ClusterSequence_TiledN2.o: ../include/fastjet/Error.hh
ClusterSequence_TiledN2.o: ../include/fastjet/JetDefinition.hh
ClusterSequence_TiledN2.o: ../include/fastjet/internal/MinHeap.hh
ClusterSequence_Delaunay.o: ../include/fastjet/Error.hh
ClusterSequence_Delaunay.o: ../include/fastjet/internal/base.hh
ClusterSequence_Delaunay.o: ../include/fastjet/PseudoJet.hh
ClusterSequence_Delaunay.o: ../include/fastjet/internal/numconsts.hh
ClusterSequence_Delaunay.o: ../include/fastjet/ClusterSequence.hh
ClusterSequence_Delaunay.o: ../include/fastjet/internal/DynamicNearestNeighbours.hh
ClusterSequence_Delaunay.o: ../include/fastjet/JetDefinition.hh
ClusterSequence_Delaunay.o: ../include/fastjet/internal/Dnn4piCylinder.hh
ClusterSequence_Delaunay.o: ../include/fastjet/internal/DnnPlane.hh
ClusterSequence_Delaunay.o: ../include/fastjet/internal/Triangulation.hh
ClusterSequence_Delaunay.o: ../include/fastjet/internal/Dnn3piCylinder.hh
ClusterSequence_Delaunay.o: ../include/fastjet/internal/Dnn2piCylinder.hh
ClusterSequence_DumbN3.o: ../include/fastjet/PseudoJet.hh
ClusterSequence_DumbN3.o: ../include/fastjet/internal/numconsts.hh
ClusterSequence_DumbN3.o: ../include/fastjet/internal/base.hh
ClusterSequence_DumbN3.o: ../include/fastjet/ClusterSequence.hh
ClusterSequence_DumbN3.o: ../include/fastjet/internal/DynamicNearestNeighbours.hh
ClusterSequence_DumbN3.o: ../include/fastjet/Error.hh
ClusterSequence_DumbN3.o: ../include/fastjet/JetDefinition.hh
ClusterSequence_CP2DChan.o: ../include/fastjet/ClusterSequence.hh
ClusterSequence_CP2DChan.o: ../include/fastjet/internal/DynamicNearestNeighbours.hh
ClusterSequence_CP2DChan.o: ../include/fastjet/internal/numconsts.hh
ClusterSequence_CP2DChan.o: ../include/fastjet/internal/base.hh
ClusterSequence_CP2DChan.o: ../include/fastjet/PseudoJet.hh
ClusterSequence_CP2DChan.o: ../include/fastjet/Error.hh
ClusterSequence_CP2DChan.o: ../include/fastjet/JetDefinition.hh
ClusterSequence_CP2DChan.o: ../include/fastjet/internal/ClosestPair2D.hh
ClusterSequence_CP2DChan.o: ../include/fastjet/internal/ClosestPair2DBase.hh
ClusterSequence_CP2DChan.o: ../include/fastjet/internal/SearchTree.hh
ClusterSequence_CP2DChan.o: ../include/fastjet/internal/MinHeap.hh
ClosestPair2D.o: ../include/fastjet/internal/ClosestPair2D.hh
ClosestPair2D.o: ../include/fastjet/internal/ClosestPair2DBase.hh
ClosestPair2D.o: ../include/fastjet/internal/base.hh
ClosestPair2D.o: ../include/fastjet/internal/SearchTree.hh
ClosestPair2D.o: ../include/fastjet/internal/MinHeap.hh
MinHeap.o: ../include/fastjet/internal/MinHeap.hh
MinHeap.o: ../include/fastjet/internal/base.hh
ClusterSequenceActiveAreaExplicitGhosts.o: ../include/fastjet/ClusterSequenceActiveAreaExplicitGhosts.hh
ClusterSequenceActiveAreaExplicitGhosts.o: ../include/fastjet/PseudoJet.hh
ClusterSequenceActiveAreaExplicitGhosts.o: ../include/fastjet/internal/numconsts.hh
ClusterSequenceActiveAreaExplicitGhosts.o: ../include/fastjet/internal/base.hh
ClusterSequenceActiveAreaExplicitGhosts.o: ../include/fastjet/ClusterSequenceWithArea.hh
ClusterSequenceActiveAreaExplicitGhosts.o: ../include/fastjet/ClusterSequence.hh
ClusterSequenceActiveAreaExplicitGhosts.o: ../include/fastjet/internal/DynamicNearestNeighbours.hh
ClusterSequenceActiveAreaExplicitGhosts.o: ../include/fastjet/Error.hh
ClusterSequenceActiveAreaExplicitGhosts.o: ../include/fastjet/JetDefinition.hh
ClusterSequenceActiveAreaExplicitGhosts.o: ../include/fastjet/ActiveAreaSpec.hh
ClusterSequenceActiveAreaExplicitGhosts.o: ../include/fastjet/internal/BasicRandom.hh
ActiveAreaSpec.o: ../include/fastjet/ActiveAreaSpec.hh
ActiveAreaSpec.o: ../include/fastjet/PseudoJet.hh
ActiveAreaSpec.o: ../include/fastjet/internal/numconsts.hh
ActiveAreaSpec.o: ../include/fastjet/internal/base.hh
ActiveAreaSpec.o: ../include/fastjet/internal/BasicRandom.hh
ClusterSequenceActiveArea.o: ../include/fastjet/PseudoJet.hh
ClusterSequenceActiveArea.o: ../include/fastjet/internal/numconsts.hh
ClusterSequenceActiveArea.o: ../include/fastjet/internal/base.hh
ClusterSequenceActiveArea.o: ../include/fastjet/ClusterSequence.hh
ClusterSequenceActiveArea.o: ../include/fastjet/internal/DynamicNearestNeighbours.hh
ClusterSequenceActiveArea.o: ../include/fastjet/Error.hh
ClusterSequenceActiveArea.o: ../include/fastjet/JetDefinition.hh
ClusterSequenceActiveArea.o: ../include/fastjet/ClusterSequenceActiveArea.hh
ClusterSequenceActiveArea.o: ../include/fastjet/ClusterSequenceWithArea.hh
ClusterSequenceActiveArea.o: ../include/fastjet/ClusterSequenceActiveAreaExplicitGhosts.hh
ClusterSequenceActiveArea.o: ../include/fastjet/ActiveAreaSpec.hh
ClusterSequenceActiveArea.o: ../include/fastjet/internal/BasicRandom.hh
BasicRandom.o: ../include/fastjet/internal/BasicRandom.hh
BasicRandom.o: ../include/fastjet/internal/base.hh
JetDefinition.o: ../include/fastjet/JetDefinition.hh
JetDefinition.o: ../include/fastjet/internal/numconsts.hh
JetDefinition.o: ../include/fastjet/internal/base.hh
JetDefinition.o: ../include/fastjet/PseudoJet.hh ../include/fastjet/Error.hh
Error.o: ../include/fastjet/Error.hh ../include/fastjet/internal/base.hh
