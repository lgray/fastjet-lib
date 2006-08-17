# fastjet/src/Makefile

# include the Makefile in the top directory, which has the bits and pieces
# needed to get CGAL working
include ../Makefile

FASTJET_INCLUDE = -I../include
INCLUDE += $(FASTJET_INCLUDE)
#INCLUDE = 

SRCS = DnnPlane.cc Dnn4piCylinder.cc Dnn3piCylinder.cc Dnn2piCylinder.cc ClusterSequence.cc PseudoJet.cc ClusterSequence_N2.cc ClusterSequence_TiledN2.cc ClusterSequence_Delaunay.cc ClusterSequence_DumbN3.cc ClusterSequence_CP2DChan.cc ClosestPair2D.cc MinHeap.cc

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
ClosestPair2D.o: ../include/fastjet/internal/ClosestPair2D.hh
ClosestPair2D.o: ../include/fastjet/internal/ClosestPair2DBase.hh
ClosestPair2D.o: ../include/fastjet/internal/base.hh
ClosestPair2D.o: ../include/fastjet/internal/SearchTree.hh
ClosestPair2D.o: ../include/fastjet/internal/MinHeap.hh
MinHeap.o: ../include/fastjet/internal/MinHeap.hh
MinHeap.o: ../include/fastjet/internal/base.hh
