
# common fastjet definitions
include ../../Makefile


CXXSRC = PxConePlugin.cc 
F77SRC = pxcone.f
COMMONOBJ = $(patsubst %.cc,%.o,$(CXXSRC)) $(patsubst %.f,%.o,$(F77SRC))

F77 = g77
FFLAGS = -O3

INCLUDE += -I../../include

libPxConePlugin.a: $(COMMONOBJ)
	ar cru libPxConePlugin.a $(COMMONOBJ)
	ranlib libPxConePlugin.a


depend:
	makedepend  $(INCLUDE) -I. --   -- $(CXXSRC) $(F77SRC)

# DO NOT DELETE

PxConePlugin.o: PxConePlugin.hh ../../include/fastjet/JetDefinition.hh
PxConePlugin.o: ../../include/fastjet/internal/numconsts.hh
PxConePlugin.o: ../../include/fastjet/internal/base.hh
PxConePlugin.o: ../../include/fastjet/PseudoJet.hh
PxConePlugin.o: ../../include/fastjet/ClusterSequence.hh
PxConePlugin.o: ../../include/fastjet/internal/DynamicNearestNeighbours.hh
PxConePlugin.o: ../../include/fastjet/Error.hh pxcone.h
