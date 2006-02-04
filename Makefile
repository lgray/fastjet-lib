# fastjet/Makefile

# set this to "yes" if you have the CGAL library installed and want
# to have the N ln N clustering option. In this case you should make 
# sure that the CGAL_MAKEFILE environment variable is defined and points
# to the CGAL Makefile (which defines various flags)
USE_CGAL = yes

#CGAL_MAKEFILE = .../some/path/makefile...

# Things needed when compiling with ktjet -- adjust to correspond
# to your own setup
KTJET_INCLUDE =    -DKTDOUBLEPRECISION -I../../ktjet -I../../clhep/include
KTJET_LIBRARY = -L../../ktjet/lib -lKtEvent -L../../clhep/lib -lCLHEP -lm

#--------------------------------------------------------------------
# other config stuff
cppExtension := .cc          # File extension of source files
CPPFLAGS +=                 # List flags to pass to C/C++ preprocessor
CXXFLAGS += -g -O3 -Wall       # List flags to pass to C++ compiler
LDFLAGS +=                  # List flags to pass to linker
LDLIBS +=                   # List additional system libraries to link with


ifeq ($(USE_CGAL),no)
  # this directs the code to drop the parts that depend on CGAL 
  INCLUDE += -DDROP_CGAL
else
  #---------------------------------------------------------------------#
  # stuff to get CGAL working
  # 
  #**** 
  # the following line includes the CGAL_MAKEFILE which defines
  # various flags needed for linking and compiling with CGAL
  # $CGAL_MAKEFILE is an environment variable which you should
  # set to the location of that Makefile, as indicated
  # at the end of the CGAL installation process.
  include $(CGAL_MAKEFILE)

  # CGAL flags are usually held in variables with CGAL-specific names;
  # copy those flags into variables with standard names
  CXXFLAGS +=  $(CGAL_CXXFLAGS) $(LONG_NAME_PROBLEM_CXXFLAGS)
  #INCLUDE += -I$(CGAL_INCL_DIR)
  LIBPATH += $(CGAL_LIBPATH)
  LDFLAGS += $(LONG_NAME_PROBLEM_LDFLAGS) $(CGAL_LDFLAGS)
  CXX = $(CGAL_CXX)   #NB auto makefile stuff will fail without g++
endif


%.o: %.cc
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $<


# will the following work???!!!
#lib:
#	echo hello
#	cd src; make; cd ..
