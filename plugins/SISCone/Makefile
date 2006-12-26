
# common fastjet definitions
include ../../Makefile


CXXSRC = SConesPlugin.cc 
OBJS = $(patsubst %.cc,%.o,$(CXXSRC)) 

# fastjet includes
FASTJET_INCLUDE = -I../../include
INCLUDE += $(FASTJET_INCLUDE)

# scones includes
INCLUDE += -I$(HOME)/tex/jets/IRSafeCone/scones/src

libSConesPlugin.a: $(OBJS)
	ar cru libSConesPlugin.a $(OBJS)
	ranlib libSConesPlugin.a

clean:
	rm -f $(OBJS)

distclean: clean
	rm -f libSConesPlugin.a *~

# NB: only use "relative" includes here (others are too subject
#     to change according to the system one is on...)
depend:
	makedepend   $(FASTJET_INCLUDE) -I. --   -- $(CXXSRC)

# DO NOT DELETE

SConesPlugin.o: ../../include/fastjet/ClusterSequence.hh
SConesPlugin.o: ../../include/fastjet/internal/DynamicNearestNeighbours.hh
SConesPlugin.o: ../../include/fastjet/internal/numconsts.hh
SConesPlugin.o: ../../include/fastjet/internal/base.hh
SConesPlugin.o: ../../include/fastjet/PseudoJet.hh
SConesPlugin.o: ../../include/fastjet/Error.hh
SConesPlugin.o: ../../include/fastjet/JetDefinition.hh SConesPlugin.hh
