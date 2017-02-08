#!/usr/bin/env python
from fastjet import *
import re

def main():
    #----------------------------------------------------------------------
    # read the event
    event = read_event("../example/data/single-event.dat")
    print "Event has {} particles and is of type {}".format(len(event), type(event))
    
    #----------------------------------------------------------------------
    # cluster the event
    jet_def = JetDefinition(antikt_algorithm, 0.4)
    area_def = AreaDefinition(active_area, GhostedAreaSpec(5.0))
    cs = ClusterSequenceArea(event, jet_def, area_def)
    jets = SelectorPtMin(5.0)(sorted_by_pt(cs.inclusive_jets()))

    print "#-------------------- initial jets --------------------"
    print_jets(jets)

    #----------------------------------------------------------------------
    # estimate the background
    maxrap       = 4.0
    grid_spacing = 0.55
    gmbge = GridMedianBackgroundEstimator(maxrap, grid_spacing)
    gmbge.set_particles(event)
    print "#-------------------- background properties --------------------"
    print "rho   = ", gmbge.rho()
    print "sigma = ", gmbge.sigma()
    print
    
    #----------------------------------------------------------------------
    # subtract the jets
    subtractor = Subtractor(gmbge)
    subtracted_jets = subtractor(jets)
    print "#-------------------- subtracteed jets --------------------"
    print_jets(subtracted_jets)
    

#----------------------------------------------------------------------
def read_event(filename):
    f = open(filename, 'r')
    event = []
    while True:
        line = f.readline()
        if (not line): break
        if (line[0] == '#'): break
        p = line.split()
        event.append(PseudoJet(float(p[0]),float(p[1]),float(p[2]),float(p[3])));

    return event

#----------------------------------------------------------------------
def print_jets(jets):
    print len(jets)," jets"
    print "columns are pt, rap, area"
    for jet in jets:
        print jet.pt(), jet.rap(), jet.area()
    print
    

main()

