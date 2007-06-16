# makefile to speed up "making" of plugins

all: cdfcones-lib siscone-lib

# pxcone left out by default because it needs fortran compiler & libs.
#all: pxcone cdfcones siscone

pxcone-lib:
	cd PxCone && $(MAKE)  

cdfcones-lib:
	cd CDFCones && $(MAKE) 

siscone-lib:
	cd SISCone && $(MAKE)  

clean:
	cd PxCone   && $(MAKE)  $(MAKECMDGOALS)
	cd CDFCones && $(MAKE)  $(MAKECMDGOALS)
	cd SISCone  && $(MAKE)  $(MAKECMDGOALS)

distclean:
	cd PxCone && $(MAKE)  $(MAKECMDGOALS)
	cd CDFCones && $(MAKE)  $(MAKECMDGOALS)
	cd SISCone && $(MAKE)  $(MAKECMDGOALS)
	rm -f *~
