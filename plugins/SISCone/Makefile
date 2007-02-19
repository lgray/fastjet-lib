
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

libSISConePlugin.a: $(OBJS) $(MAKESUBDIR) $(SUBDIR)/*.o
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
SISConePlugin.o: /usr/include/features.h /usr/include/sys/cdefs.h
SISConePlugin.o: /usr/include/gnu/stubs.h /usr/include/bits/huge_val.h
SISConePlugin.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
SISConePlugin.o: siscone/src/reference.h siscone/src/siscone.h
SISConePlugin.o: siscone/src/protocones.h siscone/src/vicinity.h
SISConePlugin.o: siscone/src/defines.h siscone/src/quadtree.h
SISConePlugin.o: /usr/include/stdio.h /usr/include/bits/types.h
SISConePlugin.o: /usr/include/bits/wordsize.h /usr/include/bits/typesizes.h
SISConePlugin.o: /usr/include/libio.h /usr/include/_G_config.h
SISConePlugin.o: /usr/include/wchar.h /usr/include/bits/wchar.h
SISConePlugin.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
SISConePlugin.o: /usr/include/bits/sys_errlist.h siscone/src/hash.h
SISConePlugin.o: siscone/src/split_merge.h
