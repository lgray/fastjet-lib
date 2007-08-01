# fastjet/example/Makefile

# include the Makefile in the top directory, which has the bits and pieces
# needed to get CGAL working
FASTJETDIR=../
include $(FASTJETDIR)/Makefile

FASTJET_INCLUDE = -I$(FASTJETDIR)/include 
INCLUDE += $(FASTJET_INCLUDE)  -I$(FASTJETDIR)/plugins/SISCone -I$(FASTJETDIR)/plugins/CDFCones
LDFLAGS += -L$(FASTJETDIR)/lib -lfastjet  -L$(FASTJETDIR)/plugins/SISCone -lSISConePlugin -L$(FASTJETDIR)/plugins/CDFCones -lCDFConesPlugin -lfastjet


siscone_example: siscone_example.o fastjetsiscone.o $(FASTJETDIR)/lib/libfastjet.a
	  g77  -o siscone_example  siscone_example.o fastjetsiscone.o $(LDFLAGS) -lstdc++

clean: 
	rm -f *.o

realclean: clean
	rm -f siscone_example

depend:
	makedepend $(FASTJET_INCLUDE) -Y --   -- $(SRCS) $(PROG_SRCS)
# DO NOT DELETE
