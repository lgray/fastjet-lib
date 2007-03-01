
# common fastjet definitions
include ../../Makefile


CXXSRC = SISConePlugin.cc 
OBJS = $(patsubst %.cc,%.o,$(CXXSRC)) 

MAKESUBDIR = siscone_src
SUBDIR = siscone/src

# fastjet includes
FASTJET_INCLUDE = -I../../include
INCLUDE += $(FASTJET_INCLUDE)

# scones includes
INCLUDE += -I$(SUBDIR)

all: libSISConePlugin.a

libSISConePlugin.a: $(OBJS) $(MAKESUBDIR) siscone_src
	ar cru libSISConePlugin.a $(OBJS) $(SUBDIR)/*.o
	ranlib libSISConePlugin.a

siscone_src:
	@cd $(SUBDIR) && $(MAKE) $(MAKECMDGOALS)

clean:
	rm -f $(OBJS)
	@cd $(SUBDIR) && $(MAKE) $(MAKECMDGOALS)

distclean: clean
	rm -f libSISConePlugin.a *~

# NB: only use "relative" includes here (others are too subject
#     to change according to the system one is on...)
depend:
	makedepend   $(FASTJET_INCLUDE) -I. -Isiscone/src --   -- $(CXXSRC)

# DO NOT DELETE

SISConePlugin.o: ../../include/fastjet/ClusterSequence.hh
SISConePlugin.o: ../../include/fastjet/internal/DynamicNearestNeighbours.hh
SISConePlugin.o: ../../include/fastjet/internal/numconsts.hh
SISConePlugin.o: ../../include/fastjet/internal/base.hh
SISConePlugin.o: ../../include/fastjet/PseudoJet.hh
SISConePlugin.o: ../../include/fastjet/Error.hh
SISConePlugin.o: ../../include/fastjet/JetDefinition.hh SISConePlugin.hh
SISConePlugin.o: siscone/src/momentum.h /usr/include/math.h
SISConePlugin.o: siscone/src/reference.h siscone/src/defines.h
SISConePlugin.o: siscone/src/siscone.h siscone/src/protocones.h
SISConePlugin.o: siscone/src/vicinity.h siscone/src/quadtree.h
SISConePlugin.o: siscone/src/split_merge.h
