#!/usr/bin/env python
"""Illustration of the use of selectors defined in Python that work on
pythonic user information associated with each particle. The
functionality is largely that of 05-user-info.py, just coded slightly
differently.


"""

import fastjet as fj
import gzip

def main():

    # get the banner out of the way early on
    fj.ClusterSequence.print_banner()
    print

    # set up our jet definition and a jet selector
    jet_def = fj.JetDefinition(fj.antikt_algorithm, 0.4)
    selector = fj.SelectorPtMin(15.0) & fj.SelectorAbsRapMax(4.5)
    print "jet definition is:",jet_def
    print "jet selector is:", selector,"\n"

    recombiner = UserRecombiner()
    jet_def_user_recomb = fj.JetDefinition(fj.antikt_algorithm, 0.4)
    jet_def_user_recomb.set_python_recombiner(recombiner)

    filename = '../data/Pythia-dijet-ptmin100-lhc-pileup-1ev.dat'
    f = file(filename,'r')
    
    # get the event
    iev = 0
    while True:
        event = read_event(f)
        iev += 1
        if (len(event) == 0): break
        
        # Create a FastJet selector based on a Python function (which
        # takes a PseudoJet and returns True if the PseudoJet passes the
        # selection condition). The resulting selector can be used in the
        # same way as any normal FastJet selector.
        sel_pileup = fj.SelectorPython(is_pileup)
        n_pileup_particles = sel_pileup.count(event)

        # cluster it with the default recombiner and print some info
        jets = selector(jet_def(event))
        print "Event {0} has {1} particles (of which {2} from pileup)".format(
            iev, len(event), n_pileup_particles)
        print_jets(jets)
        print ""

        # now re-cluster with our user-defined recombiner
        jets = selector(jet_def_user_recomb(event))
        print_jets(jets)

        #direct_test: fj_recombiner = fj.RecombinerPython(recombiner)
        #direct_test: print fj_recombiner
        #direct_test: pa = fj.PseudoJet()
        #direct_test: pb = fj.PseudoJet()
        #direct_test: pa.reset(jets[0])
        #direct_test: pb.reset(jets[1])
        #direct_test: fj_recombiner.preprocess(pa)
        #direct_test: fj_recombiner.preprocess(pb)
        #direct_test: print "pre-processed: ",jets[0]," --> ",pa,pa.user_index()
        #direct_test: print "pre-processed: ",jets[1]," --> ",pb,pb.user_index()
        #direct_test: pab=fj.PseudoJet()
        #direct_test: fj_recombiner.recombine(pa,pb,pab)  #works
        #direct_test: print "recombined: ", pab,pab.user_index()
        
#----------------------------------------------------------------------            
class ParticleInfo(object):
    """illustrative class for use in assigning pythonic user information
    to a PseudoJet.
    """
    def __init__(self, subevent_index, particle_index, pdg_id=0):
        self.subevent_index = subevent_index
        self.particle_index = particle_index
        self.pdg_id = pdg_id

    def set_pdg_id(self, pdg_id):
        self.pdg_id = pdg_id

    def __str__(self):
        return "subevent_index={0}, particle_index={1}, pdg_id={2}".format(
            self.subevent_index, self.particle_index, self.pdg_id)

    #----------------------------------------------------------------------
class HasPID:
    """Helps select particles with a specific PID"""
    def __init__(self, _pdg_id):
        self.pdg_id=_pdg_id

    def __str__(self):
        return "PDGID="+str(self.pdg_id)

    def __call__(self, particle):
        return (particle.python_info().pdg_id == self.pdg_id)

#----------------------------------------------------------------------
def is_pileup(particle):
    "Function for use with fj.SelectorPython"
    return (particle.python_info().subevent_index > 0)
        
#----------------------------------------------------------------------
class UserRecombiner(object):
    """illustrative class for use of user-defined recombiners.
    """
    def __init__(self):
        print "creating user-defined recombiner"
        
    def __str__(self):
        return "user-defined recombiner"

    def preprocess(self, pa):
        pa.reset_momentum_PtYPhiM(pa.pt(), pa.rap(), pa.phi(), 0.0)
        # store the number of photons
        if pa.has_user_info() and pa.python_info().pdg_id == 22:
            pa.set_user_index(1)
        else:
            pa.set_user_index(0)

    # this will sum the number of photons in the user index
    def recombine(self, pa, pb):
        pab=pa+pb
        pab.set_user_index(pa.user_index() + pb.user_index())
        return pab


#----------------------------------------------------------------------
def print_jets(jets):
    is_photon=HasPID(22)
    sel_photons = fj.SelectorPython(is_photon)
    sel_pileup  = fj.SelectorPython(is_pileup)

    # with classes, description uses the class __str__ fmethod
    print "Note: photon selection: "+str(sel_photons)
   
    print "{0:>5s} {1:>10s} {2:>10s} {3:>10s} {4:>12s} {5:>12s} {6:>12s} {7:>12s} {8:>12s}".format(
        "jet #", "pt", "rap", "phi", "primary pt", "N particles",
        "N photons", "N prim.phot", "user index")

    for ijet in range(len(jets)):
        jet = jets[ijet]

        # figure out how many particles and how many photons the jet contains
        # and how much pt comes from the primary vertex
        constituents = jet.constituents()
        n_photons = sel_photons.count(constituents)

        # invert the pileup selector to find the primary pileup
        primary_pt = (~sel_pileup).scalar_pt_sum(constituents)

        # and get the number of primary photons by combining two selectors
        n_primary_photons = ((~sel_pileup)*sel_photons).count(constituents)
            
        print "{0:5d} {1:10.3f} {2:10.4f} {3:10.4f} {4:10.3f} {5:12d} {6:12d} {7:12d} {8:12d}".format(
            ijet, jet.pt(), jet.rap(), jet.phi(), primary_pt, len(constituents),
            n_photons, n_primary_photons, jet.user_index())
        
#----------------------------------------------------------------------
event_index = 0
def read_event(file_or_filename):
    """
Routine that can take either an existing opened file object, or a
filename (which it will open itself) and then reads an event from that
file. An event is deemed to end when the file comes to an end or when
the reader encounters the string "#END".

The event is converted to a python list of PseudoJets
    """

    # open the file if necessary
    if (isinstance(file_or_filename,basestring)) : f = open(file_or_filename, 'r')
    else                                         : f = file_or_filename

    # create an empty list
    event = []
    particle_index = 0
    subevent_index = -1
    while True:
        line = f.readline()
        # exit if the file has come to an end
        if   (not line): break
        # or if we reach the string "#END"
        if   (len(line) >=4 and line[0:4] == '#END'): break

        # #SUBSTART means that we start a new subevent
        if (len(line)>=9 and line[0:9] == '#SUBSTART'):
            subevent_index += 1
            continue
        
        # ignore comment lines or empty lines
        elif (line[0] == '#' or len(line) <= 1): continue

        # assume we have a good line and split it into px, py, pz, E
        p = line.split()
        # create the PseudoJet
        pj = fj.PseudoJet(float(p[0]),float(p[1]),float(p[2]),float(p[3]))

        # then create its user info
        info = ParticleInfo(subevent_index, particle_index)
        # optionally with a pdg_id (if it was in the file)
        if (len(p) > 4): info.set_pdg_id(int(p[4]))
        
        # finally assign the user info and append the particle the event
        pj.set_python_info(info)
        event.append(pj);
        
        # don't forget to increment it
        particle_index += 1

    global event_index
    event_index += 1
    return event
    
if __name__ == '__main__':
    main()
