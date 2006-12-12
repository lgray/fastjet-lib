
# remember to make sure that F77LIB (in ../../Makefile) contains what
# it's supposed to for linking with fortran (if you plan to use the
# pxcone_example or many_algs_example programs), by defining the
# environment variable G77LIBDIR

include ../../Makefile

FASTJET_INCLUDE = -I../../include
INCLUDE += $(FASTJET_INCLUDE)
LDFLAGS += -L../../lib -lfastjet

INCLUDE += -I../PxCone
INCLUDE += -I../CDFCones
INCLUDE += -I../SCones

many_algs_example: many_algs_example.o run_jet_finder.o
	$(CXX) $(CXXFLAGS) -o many_algs_example many_algs_example.o run_jet_finder.o $(LDFLAGS) -L../CDFCones -lCDFConesPlugin -L../PxCone -lPxConePlugin $(F77LIB)


pxcone_example: pxcone_example.o run_jet_finder.o
	$(CXX) $(CXXFLAGS) -o pxcone_example pxcone_example.o run_jet_finder.o $(LDFLAGS) -L../PxCone -lPxConePlugin $(F77LIB)


scones_example: scones_example.o run_jet_finder.o
	$(CXX) $(CXXFLAGS) -o scones_example scones_example.o run_jet_finder.o $(LDFLAGS) -L../SCones -lSConesPlugin -L$(HOME)/tex/jets/IRSafeCone/scones/src -lscones


cdfmidpoint_example: cdfmidpoint_example.o run_jet_finder.o
	$(CXX) $(CXXFLAGS) -o cdfmidpoint_example cdfmidpoint_example.o run_jet_finder.o $(LDFLAGS) -L../CDFCones -lCDFConesPlugin

clean:
	rm -f *.o

distclean: clean
	rm -f pxcone_example scones_example many_algs_example cdfmidpoint_example *~


depend:
	makedepend  $(INCLUDE) -I. --   -- pxcone_example.cc scones_example.cc run_jet_finder.cc many_algs_example.cc cdfmidpoint_example.cc

# DO NOT DELETE

pxcone_example.o: run_jet_finder.hh ../../include/fastjet/JetDefinition.hh
pxcone_example.o: ../../include/fastjet/internal/numconsts.hh
pxcone_example.o: ../../include/fastjet/internal/base.hh
pxcone_example.o: ../../include/fastjet/PseudoJet.hh
pxcone_example.o: ../PxCone/PxConePlugin.hh
scones_example.o: run_jet_finder.hh ../../include/fastjet/JetDefinition.hh
scones_example.o: ../../include/fastjet/internal/numconsts.hh
scones_example.o: ../../include/fastjet/internal/base.hh
scones_example.o: ../../include/fastjet/PseudoJet.hh
scones_example.o: ../PxCone/PxConePlugin.hh
run_jet_finder.o: ../../include/fastjet/PseudoJet.hh
run_jet_finder.o: ../../include/fastjet/internal/numconsts.hh
run_jet_finder.o: ../../include/fastjet/internal/base.hh
run_jet_finder.o: ../../include/fastjet/ClusterSequence.hh
run_jet_finder.o: ../../include/fastjet/internal/DynamicNearestNeighbours.hh
run_jet_finder.o: ../../include/fastjet/Error.hh
run_jet_finder.o: ../../include/fastjet/JetDefinition.hh
many_algs_example.o: run_jet_finder.hh ../../include/fastjet/JetDefinition.hh
many_algs_example.o: ../../include/fastjet/internal/numconsts.hh
many_algs_example.o: ../../include/fastjet/internal/base.hh
many_algs_example.o: ../../include/fastjet/PseudoJet.hh
many_algs_example.o: ../CDFCones/CDFMidPointPlugin.hh
many_algs_example.o: ../PxCone/PxConePlugin.hh
cdfmidpoint_example.o: run_jet_finder.hh
cdfmidpoint_example.o: ../../include/fastjet/JetDefinition.hh
cdfmidpoint_example.o: ../../include/fastjet/internal/numconsts.hh
cdfmidpoint_example.o: ../../include/fastjet/internal/base.hh
cdfmidpoint_example.o: ../../include/fastjet/PseudoJet.hh
cdfmidpoint_example.o: ../CDFCones/CDFMidPointPlugin.hh
