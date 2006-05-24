# fastjet/src/Makefile

# include the Makefile in the top directory, which has the bits and pieces
# needed to get CGAL working
include ../Makefile

FASTJET_INCLUDE = -I../include
INCLUDE += $(FASTJET_INCLUDE)
#INCLUDE = 

SRCS = DnnPlane.cc Dnn4piCylinder.cc Dnn3piCylinder.cc Dnn2piCylinder.cc FjClusterSequence.cc FjPseudoJet.cc FjClusterSequence_N2.cc FjClusterSequence_TiledN2.cc FjClusterSequence_Delaunay.cc FjClusterSequence_DumbN3.cc

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

DnnPlane.o: ../include/DnnPlane.hh ../include/Triangulation.hh
DnnPlane.o: ../include/DynamicNearestNeighbours.hh ../include/numconsts.hh
Dnn4piCylinder.o: ../include/Dnn4piCylinder.hh
Dnn4piCylinder.o: ../include/DynamicNearestNeighbours.hh
Dnn4piCylinder.o: ../include/numconsts.hh ../include/DnnPlane.hh
Dnn4piCylinder.o: ../include/Triangulation.hh
Dnn3piCylinder.o: ../include/Dnn3piCylinder.hh
Dnn3piCylinder.o: ../include/DynamicNearestNeighbours.hh
Dnn3piCylinder.o: ../include/numconsts.hh ../include/DnnPlane.hh
Dnn3piCylinder.o: ../include/Triangulation.hh
Dnn2piCylinder.o: ../include/Dnn2piCylinder.hh
Dnn2piCylinder.o: ../include/DynamicNearestNeighbours.hh
Dnn2piCylinder.o: ../include/numconsts.hh ../include/DnnPlane.hh
Dnn2piCylinder.o: ../include/Triangulation.hh
FjClusterSequence.o: ../include/FjError.hh ../include/FjPseudoJet.hh
FjClusterSequence.o: ../include/numconsts.hh ../include/FjClusterSequence.hh
FjClusterSequence.o: ../include/DynamicNearestNeighbours.hh
FjPseudoJet.o: ../include/FjError.hh ../include/FjPseudoJet.hh
FjPseudoJet.o: ../include/numconsts.hh
FjClusterSequence_N2.o: ../include/FjPseudoJet.hh ../include/numconsts.hh
FjClusterSequence_N2.o: ../include/FjClusterSequence.hh
FjClusterSequence_N2.o: ../include/DynamicNearestNeighbours.hh
FjClusterSequence_TiledN2.o: ../include/FjPseudoJet.hh
FjClusterSequence_TiledN2.o: ../include/numconsts.hh
FjClusterSequence_TiledN2.o: ../include/FjClusterSequence.hh
FjClusterSequence_TiledN2.o: ../include/DynamicNearestNeighbours.hh
FjClusterSequence_Delaunay.o: ../include/FjError.hh ../include/FjPseudoJet.hh
FjClusterSequence_Delaunay.o: ../include/numconsts.hh
FjClusterSequence_Delaunay.o: ../include/FjClusterSequence.hh
FjClusterSequence_Delaunay.o: ../include/DynamicNearestNeighbours.hh
FjClusterSequence_Delaunay.o: ../include/Dnn4piCylinder.hh
FjClusterSequence_Delaunay.o: ../include/DnnPlane.hh
FjClusterSequence_Delaunay.o: ../include/Triangulation.hh
FjClusterSequence_Delaunay.o: ../include/Dnn3piCylinder.hh
FjClusterSequence_Delaunay.o: ../include/Dnn2piCylinder.hh
FjClusterSequence_DumbN3.o: ../include/FjPseudoJet.hh ../include/numconsts.hh
FjClusterSequence_DumbN3.o: ../include/FjClusterSequence.hh
FjClusterSequence_DumbN3.o: ../include/DynamicNearestNeighbours.hh
