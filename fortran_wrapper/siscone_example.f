C example program to run siscone from f77
C
C To compile, first make sure that the installation bin directory
C is in your path (so as to have access to fastjet-config) and then 
C type
C    make -f Makefile.alt siscone_example
C
C Given the complications inherent in mixing C++ and fortran, your 
C mileage may vary...
C
C To use, type: 
C    ./siscone_example < ../example/data/single-event.dat     
C    
C $Id$
C
      program siscone_example
      implicit none
      integer n
      parameter (n = 1000)
      integer i,j
      double precision p(4,n), R, f    
      double precision jets(4,n)         
      integer npart, njets ! < n        
c ... fill in p (NB, energy is p(4,i))
      do i=1,n
         read(*,*,end=500) p(1,i),p(2,i),p(3,i),p(4,i)
      enddo
      
 500  npart = i-1

      R = 0.7
      f = 0.5
c.....run the clustering        
      call fastjetsiscone(p,npart,R,f,jets,njets)   ! ... now you have the jets

c.....write out all inclusive jets, in order of decreasing pt
      write(*,*) '      px         py          pz         E         pT'
      do i=1,njets
         write(*,*) (jets(j,i), j=1,4), sqrt(jets(1,i)**2+jets(2,i)**2)
      enddo
            
      end
      
