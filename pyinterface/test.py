#!/usr/bin/python
#
# Simple example to try out fastjet from python. Some things to keep in mind
#
# - FastJet's vector<PseudoJet> is called vectorPJ in the python
# - to copy a jet you need to do "pjcopy = PseudoJet(pj)"
#   (instead "pjcopy = pj" just makes a reference and seems to cause memory issues)
# - for combinations of selectors, (&&, || and !) map to (&, | and ~)
# - 
# 
from fastjet import *
import copy

def main():

    # set up our jet definition and a jet selector
    jet_def = JetDefinition(antikt_algorithm, 0.4)
    selector = SelectorPtMin(5.0) & SelectorAbsRapMax(4.5)
    print jet_def;
    print selector

    #filename = '../example/data/Pythia-PtMin1000-LHC-10ev.dat'
    filename = '../example/data/single-event.dat'
    f = file(filename,'r')
    
    # get the event
    iev = 0
    while True:
        iev += 1
        event = read_event(f)
        if (len(event) == 0): break
        print "Event {} has {} particles and is of type {}".format(iev, len(event), type(event))
        
        # cluster it
        jets = selector(jet_def(event))
        for jet in jets:
            print "jet pt and rap: ", jet.pt(), jet.rap()
            
        # make sure jet-related information is correctly held
        if (len(jets) > 0):
            print "Number of constituents of jets[0] is {}".format(len(jets[0].constituents()))
            
    #check_operators()
    
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
def read_event(file_or_filename):
    
    if (isinstance(file_or_filename,basestring)) : f = open(file_or_filename, 'r')
    else                                         : f = file_or_filename
    #if (regexp != None)              : search(f,regexp)
    
    event = []
    while True:
        line = f.readline()
        if (not line): break
        if (len(line) >=4 and line[0:4] == '#END'): break
        elif   (line[0] == '#'): continue
        p = line.split()
        event.append(PseudoJet(float(p[0]),float(p[1]),float(p[2]),float(p[3])));

    return event
    
main()

