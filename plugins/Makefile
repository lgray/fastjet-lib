# makefile to speed up "making" of plugins

all: pxcone cdfcones siscone

pxcone:
	cd PxCone && $(MAKE)

cdfcones:
	cd CDFCones && $(MAKE)

siscone:
	cd SISCone && $(MAKE)

clean:
	cd PxCone   && $(MAKE) clean
	cd CDFCones && $(MAKE) clean
	cd SISCone  && $(MAKE) clean

distclean:
	cd PxCone && $(MAKE) distclean
	cd CDFCones && $(MAKE) distclean
	cd SISCone && $(MAKE) distclean
	rm -f *~
