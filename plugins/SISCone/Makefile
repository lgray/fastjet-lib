
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

all: libSConesPlugin.a

libSConesPlugin.a: $(OBJS) $(MAKESUBDIR)
	ar cru libSISConePlugin.a $(OBJS) $(SUBDIR)/*.o
	ranlib libSISConePlugin.a

siscone_src:
	@cd $(SUBDIR) && $(MAKE) $(MAKECMDGOALS)

clean:
	rm -f $(OBJS)
	@cd $(SUBDIR) && $(MAKE) $(MAKECMDGOALS)

distclean: clean
	rm -f libSConesPlugin.a *~

# NB: only use "relative" includes here (others are too subject
#     to change according to the system one is on...)
depend:
	makedepend   $(FASTJET_INCLUDE) -I. --   -- $(CXXSRC)

# DO NOT DELETE

SISConePlugin.o: ../../include/fastjet/ClusterSequence.hh
SISConePlugin.o: ../../include/fastjet/internal/DynamicNearestNeighbours.hh
SISConePlugin.o: ../../include/fastjet/internal/numconsts.hh
SISConePlugin.o: ../../include/fastjet/internal/base.hh
SISConePlugin.o: ../../include/fastjet/PseudoJet.hh
SISConePlugin.o: ../../include/fastjet/Error.hh
SISConePlugin.o: ../../include/fastjet/JetDefinition.hh SISConePlugin.hh
