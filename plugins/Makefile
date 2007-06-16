# makefile to speed up "making" of plugins

all: cdfcones siscone

# pxcone left out by default because it needs fortran compiler & libs.
#all: pxcone cdfcones siscone

pxcone:
	@cd PxCone && $(MAKE)  $(MAKECMDGOALS)

cdfcones:
	@cd CDFCones && $(MAKE)  $(MAKECMDGOALS)

siscone:
	@cd SISCone && $(MAKE)  $(MAKECMDGOALS)

clean:
	cd PxCone   && $(MAKE)  $(MAKECMDGOALS)
	cd CDFCones && $(MAKE)  $(MAKECMDGOALS)
	cd SISCone  && $(MAKE)  $(MAKECMDGOALS)

distclean:
	cd PxCone && $(MAKE)  $(MAKECMDGOALS)
	cd CDFCones && $(MAKE)  $(MAKECMDGOALS)
	cd SISCone && $(MAKE)  $(MAKECMDGOALS)
	rm -f *~
