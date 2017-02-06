#!/usr/bin/python
from fastjet import *

jet_def = JetDefinition(antikt_algorithm, 0.4)
a=PtYPhiM(100.0, 0.0, 0.0, 0.0)
b=PtYPhiM(100.0, 0.2, 0.0, 0.0)

vec = vectorPJ();
vec.push_back(a)
vec.push_back(b)

sel = SelectorAbsRapMax(0.1)

#jets = jet_def.cluster(sel(vec))
#print jets[0].pt()

cs = ClusterSequence(vec, jet_def)
jets = cs.inclusive_jets();
print jets[0].pt()
