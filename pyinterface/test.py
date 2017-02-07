#!/usr/bin/python

# 
from fastjet import *
import re

def main():

    # set up our jet definition and a jet selector
    jet_def = JetDefinition(antikt_algorithm, 0.4)
    selector = SelectorPtMin(5.0) & SelectorAbsRapMax(4.5)
    print jet_def;
    print selector

    # get the event
    event = read_event("../example/data/single-event.dat")
    print "Event has {} particles and is of type {}".format(len(event), type(event))

    # cluster it
    jets = selector(jet_def(event))
    for jet in jets:
        print jet.pt(), jet.rap()

    # make sure jet-related information is correctly held
    print "Number of constituents of jets[0] is {}".format(jets[0].constituents().size())

    check_operators()
    
def check_operators():
    #----------------------------------------------------------------------
    # some random manipulations to check operators
    a=PtYPhiM(100.0, 0.0, 0.0, 0.0)
    b=PtYPhiM(100.0, 0.2, 0.0, 0.0)
    c = a-b
    print 2.0*a
    print b
    print c/2


    a = PseudoJet()
    print (a==0),a 
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

