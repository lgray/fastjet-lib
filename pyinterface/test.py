#!/usr/bin/python
from fastjet import *
import re

def main():
    event = read_event("../example/data/single-event.dat")
    print "Event has {} particles and is of type {}".format(len(event), type(event))
    
    jet_def = JetDefinition(antikt_algorithm, 0.4)
    cs = ClusterSequence(event, jet_def)
    jets = SelectorPtMin(5.0)(sorted_by_pt(cs.inclusive_jets()))
    print jets
    print len(jets)
    for jet in jets:
        print jet.pt(), jet.rap()
    
    a=PtYPhiM(100.0, 0.0, 0.0, 0.0)
    b=PtYPhiM(100.0, 0.2, 0.0, 0.0)
    
    vec = vectorPJ();
    vec.push_back(a)
    vec.push_back(b)
    
    selA = SelectorAbsRapMax(0.1)
    selB = SelectorPtMin(20)
    selC = ~(selA | selB)
    print selC(jets)
    print
    
    #jets = jet_def.cluster(sel(vec))
    #print jets[0].pt()
    
    jets = cs.inclusive_jets();
    print jets[0].pt()
    
    #----------------------------------------------------------------------
    c = a-b
    print 2.0*a
    print b
    print c/2


#----------------------------------------------------------------------
def read_event(filename):
    f = open(filename, 'r')
    #event = []
    event = vectorPJ()
    while True:
        line = f.readline()
        if (not line): break
        if (line[0] == '#'): break
        p = line.split()
        event.append(PseudoJet(float(p[0]),float(p[1]),float(p[2]),float(p[3])));

    return event
    
main()

