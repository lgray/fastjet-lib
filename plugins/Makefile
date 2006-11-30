# makefile to speed up "making" of plugins

all: pxcone cdfcones

pxcone:
	cd PxCone && $(MAKE)

cdfcones:
	cd CDFCones && $(MAKE)

clean:
	cd PxCone && $(MAKE) clean
	cd CDFCones && $(MAKE) clean

distclean:
	cd PxCone && $(MAKE) distclean
	cd CDFCones && $(MAKE) distclean
	rm -f *~
