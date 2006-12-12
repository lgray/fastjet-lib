
# common fastjet definitions
include ../../Makefile


CXXSRC = SConesPlugin.cc 
OBJS = $(patsubst %.cc,%.o,$(CXXSRC)) 

# fastjet includes
INCLUDE += -I../../include

# scones includes
INCLUDE += -I$(HOME)/tex/jets/IRSafeCone/scones/src

libSConesPlugin.a: $(OBJS)
	ar cru libSConesPlugin.a $(OBJS)
	ranlib libSConesPlugin.a

clean:
	rm -f $(OBJS)

distclean: clean
	rm -f libSConesPlugin.a *~

depend:
	makedepend  $(INCLUDE) -I. --   -- $(CXXSRC)

# DO NOT DELETE

SConesPlugin.o: ../../include/fastjet/ClusterSequence.hh
SConesPlugin.o: ../../include/fastjet/internal/DynamicNearestNeighbours.hh
SConesPlugin.o: ../../include/fastjet/internal/numconsts.hh
SConesPlugin.o: ../../include/fastjet/internal/base.hh
SConesPlugin.o: ../../include/fastjet/PseudoJet.hh
SConesPlugin.o: ../../include/fastjet/Error.hh
SConesPlugin.o: ../../include/fastjet/JetDefinition.hh SConesPlugin.hh
SConesPlugin.o: /ada1/lpthe/salam/tex/jets/IRSafeCone/scones/src/momentum.h
SConesPlugin.o: /usr/include/math.h /usr/include/features.h
SConesPlugin.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
SConesPlugin.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
SConesPlugin.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
SConesPlugin.o: /usr/include/bits/mathcalls.h
SConesPlugin.o: /ada1/lpthe/salam/tex/jets/IRSafeCone/scones/src/reference.h
SConesPlugin.o: /ada1/lpthe/salam/tex/jets/IRSafeCone/scones/src/scones.h
SConesPlugin.o: /ada1/lpthe/salam/tex/jets/IRSafeCone/scones/src/protocones.h
SConesPlugin.o: /ada1/lpthe/salam/tex/jets/IRSafeCone/scones/src/vicinity.h
SConesPlugin.o: /ada1/lpthe/salam/tex/jets/IRSafeCone/scones/src/quadtree.h
SConesPlugin.o: /usr/include/stdio.h /usr/include/bits/types.h
SConesPlugin.o: /usr/include/bits/typesizes.h /usr/include/libio.h
SConesPlugin.o: /usr/include/_G_config.h /usr/include/wchar.h
SConesPlugin.o: /usr/include/bits/wchar.h /usr/include/gconv.h
SConesPlugin.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
SConesPlugin.o: /ada1/lpthe/salam/tex/jets/IRSafeCone/scones/src/hash.h
SConesPlugin.o: /ada1/lpthe/salam/tex/jets/IRSafeCone/scones/src/split_merge.h
