
// File: index.xml

// File: structfastjet_1_1____inheritance__helper.xml


%feature("docstring") fastjet::__inheritance_helper "
";

%feature("docstring") fastjet::__inheritance_helper::check_sig "
";

%feature("docstring") fastjet::__inheritance_helper::check_sig "
";

// File: classfastjet_1_1SharedPtr_1_1____SharedCountingPtr.xml


%feature("docstring") fastjet::SharedPtr::__SharedCountingPtr "
";

%feature("docstring") fastjet::SharedPtr::__SharedCountingPtr::set_count "

force the count to be set to a specified value  

Parameters
----------
* `count` :  
    the value that we ned to reset to  
";

%feature("docstring") fastjet::SharedPtr::__SharedCountingPtr::~__SharedCountingPtr "

default dtor  
";

%feature("docstring") fastjet::SharedPtr::__SharedCountingPtr::get "

return a pointer to the object  
";

%feature("docstring") fastjet::SharedPtr::__SharedCountingPtr::__SharedCountingPtr "

default ctor  
";

%feature("docstring") fastjet::SharedPtr::__SharedCountingPtr::__SharedCountingPtr "

ctor with initialisation  
";

%feature("docstring") fastjet::SharedPtr::__SharedCountingPtr::use_count "

return the count  
";

// File: classfastjet_1_1ClusterSequence_1_1__Line.xml

// File: classfastjet_1_1__NoInfo.xml


%feature("docstring") fastjet::_NoInfo "

internal dummy class, used as a default template argument  

C++ includes: fastjet/NNBase.hh
";

// File: classfastjet_1_1ClusterSequence_1_1__Parabola.xml

// File: classfastjet_1_1AreaDefinition.xml


%feature("docstring") fastjet::AreaDefinition "

class that holds a generic area definition  

C++ includes: fastjet/AreaDefinition.hh
";

%feature("docstring") fastjet::AreaDefinition::AreaDefinition "

default constructor, which provides a ghosted active area, with sensible
defaults for the ghosts.  
";

%feature("docstring") fastjet::AreaDefinition::AreaDefinition "

constructor for an area definition based on an area type and a ghosted area
specification  
";

%feature("docstring") fastjet::AreaDefinition::AreaDefinition "

constructor for an area definition based on an area type and a voronoi area
specification (type must be voronoi_area)  
";

%feature("docstring") fastjet::AreaDefinition::AreaDefinition "

constructor for an area definition based on an area type and which attempts to
provide sensible defaults for everything else  
";

%feature("docstring") fastjet::AreaDefinition::AreaDefinition "

constructor for an area definition based on an ghosted area specification, and
an option to select which ghosted area you want  
";

%feature("docstring") fastjet::AreaDefinition::AreaDefinition "

constructor for an area definition based on a voronoi area specification  
";

%feature("docstring") fastjet::AreaDefinition::area_type "

return info about the type of area being used by this defn  
";

%feature("docstring") fastjet::AreaDefinition::voronoi_spec "

return a reference to the voronoi area spec  
";

%feature("docstring") fastjet::AreaDefinition::ghost_spec "

return a reference to the active area spec  
";

%feature("docstring") fastjet::AreaDefinition::ghost_spec "
";

%feature("docstring") fastjet::AreaDefinition::description "

return a description of the current area definition  

return info about the type of area being used by this defn  
";

// File: classfastjet_1_1ATLASConePlugin.xml


%feature("docstring") fastjet::ATLASConePlugin "

Implementation of the ATLAS Cone (plugin for fastjet v2.4 upwards)  

C++ includes: fastjet/ATLASConePlugin.hh
";

%feature("docstring") fastjet::ATLASConePlugin::description "

return a textual description of the jet-definition implemented in this plugin  
";

%feature("docstring") fastjet::ATLASConePlugin::ATLASConePlugin "

Main constructor for the ATLASCone Plugin class.  

Apparently the default parameters in the ATLAS software are the ones used here.
SpartyJet uses a radius of 0.7, a seed threshold of 1 GeV and an overlap
threshold of 0.75 For the ATLAS SW defaults, see http://atlas-sw.cern.ch/cgi-bin
/viewcvs-atlas.cgi/groups/JetRoutines/SpartyJet/atlas/ in the
JetdoneFinderTools.cxx (rev1.1) and JetSplitMergeTool.cxx (rev1.1) For
SpartyJet, see atlas/ConeFinderTool.h  

Finally, to agree with FastJet standards, we do not specify a default R, that in
the ATLAS code is 0.7  
";

%feature("docstring") fastjet::ATLASConePlugin::ATLASConePlugin "

copy constructor  
";

%feature("docstring") fastjet::ATLASConePlugin::R "

the plugin mechanism's standard way of accessing the jet radius here we return
the R of the last alg in the list  
";

%feature("docstring") fastjet::ATLASConePlugin::seedPt "

seed threshold  
";

%feature("docstring") fastjet::ATLASConePlugin::run_clustering "

given a ClusterSequence that has been filled up with initial particles, the
following function should fill up the rest of the ClusterSequence, using the
following member functions of ClusterSequence:  

*   plugin_do_ij_recombination(...)  
*   plugin_do_iB_recombination(...)  
";

%feature("docstring") fastjet::ATLASConePlugin::f "

split-merge overlap threshold  
";

// File: classfastjet_1_1BackgroundEstimatorBase.xml


%feature("docstring") fastjet::BackgroundEstimatorBase "

Abstract base class that provides the basic interface for classes that estimate
levels of background radiation in hadron and heavy-ion collider events.  

C++ includes: fastjet/tools/BackgroundEstimatorBase.hh
";

/*
 constructors and destructors 
*/

/*
 setting a new event 
*/

/*
 retrieving fundamental information 
*/

/*
 configuring the behaviour 
*/

/*
 description 
*/

/*
 helpers for derived classes 
*/

/*
Note that these helpers are related to median-based estimation of the
background, so there is no guarantee that they will remain in this base class in
the long term  

*/

%feature("docstring") fastjet::BackgroundEstimatorBase::rho "

get rho, the background density per unit area  
";

%feature("docstring") fastjet::BackgroundEstimatorBase::rho "

get rho, the background density per unit area, locally at the position of a
given jet.  

Note that this is not const, because a user may then wish to query other aspects
of the background that could depend on the position of the jet last used for a
rho(jet) determination.  
";

%feature("docstring") fastjet::BackgroundEstimatorBase::set_particles "

tell the background estimator that it has a new event, composed of the specified
particles.  
";

%feature("docstring") fastjet::BackgroundEstimatorBase::BackgroundEstimatorBase "
";

%feature("docstring") fastjet::BackgroundEstimatorBase::description "

returns a textual description of the background estimator  
";

%feature("docstring") fastjet::BackgroundEstimatorBase::rescaling_class "

return the pointer to the jet density class  
";

%feature("docstring") fastjet::BackgroundEstimatorBase::has_sigma "

returns true if this background estimator has support for determination of sigma  
";

%feature("docstring") fastjet::BackgroundEstimatorBase::has_rho_m "

Returns true if this background estimator has support for determination of
rho_m.  

Note that support for sigma_m is automatic is one has sigma and rho_m support.  
";

%feature("docstring") fastjet::BackgroundEstimatorBase::~BackgroundEstimatorBase "

a default virtual destructor that does nothing  
";

%feature("docstring") fastjet::BackgroundEstimatorBase::sigma "

get sigma, the background fluctuations per unit area; must be multipled by
sqrt(area) to get fluctuations for a region of a given area.  
";

%feature("docstring") fastjet::BackgroundEstimatorBase::sigma "

get sigma, the background fluctuations per unit area, locally at the position of
a given jet.  

As for rho(jet), it is non-const.  
";

%feature("docstring") fastjet::BackgroundEstimatorBase::set_rescaling_class "

Set a pointer to a class that calculates the rescaling factor as a function of
the jet (position).  

Note that the rescaling factor is used both in the determination of the
\"global\" rho (the pt/A of each jet is divided by this factor) and when asking
for a local rho (the result is multiplied by this factor).  

The BackgroundRescalingYPolynomial class can be used to get a rescaling that
depends just on rapidity.  

There is currently no support for different rescaling classes for rho and rho_m
determinations.  
";

%feature("docstring") fastjet::BackgroundEstimatorBase::sigma_m "

returns sigma_m, a measure of the fluctuations in the purely longitudinal,
particle-mass-induced component of the background density per unit area; must be
multipled by sqrt(area) to get fluctuations for a region of a given area.  
";

%feature("docstring") fastjet::BackgroundEstimatorBase::sigma_m "

Returns sigma_m locally at the jet position. As for rho(jet), it is non-const.  
";

%feature("docstring") fastjet::BackgroundEstimatorBase::rho_m "

returns rho_m, the purely longitudinal, particle-mass-induced component of the
background density per unit area  
";

%feature("docstring") fastjet::BackgroundEstimatorBase::rho_m "

Returns rho_m locally at the jet position. As for rho(jet), it is non-const.  
";

// File: classfastjet_1_1BackgroundJetPtDensity.xml


%feature("docstring") fastjet::BackgroundJetPtDensity "

Class that implements pt/area_4vector.perp() for background estimation *(this is
a preliminary class)*.  

C++ includes: fastjet/tools/JetMedianBackgroundEstimator.hh
";

%feature("docstring") fastjet::BackgroundJetPtDensity::result "

the action of the function this *has* to be overloaded in derived classes  

Parameters
----------
* `pj` :  
    the PseudoJet input to the function  
";

%feature("docstring") fastjet::BackgroundJetPtDensity::description "

returns a description of the function (an empty string by default)  
";

// File: classfastjet_1_1BackgroundJetPtMDensity.xml


%feature("docstring") fastjet::BackgroundJetPtMDensity "

Class that implements $ \\frac{1}{A} \\sum_{i \\in jet} (\\sqrt{p_{ti}^2+m^2} -
p_{ti}) $ for background estimation *(this is a preliminary class)*.  

This is useful for correcting jet masses in cases where the event involves
massive particles.  

C++ includes: fastjet/tools/JetMedianBackgroundEstimator.hh
";

%feature("docstring") fastjet::BackgroundJetPtMDensity::result "

the action of the function this *has* to be overloaded in derived classes  

Parameters
----------
* `pj` :  
    the PseudoJet input to the function  
";

%feature("docstring") fastjet::BackgroundJetPtMDensity::description "

returns a description of the function (an empty string by default)  
";

// File: classfastjet_1_1BackgroundJetScalarPtDensity.xml


%feature("docstring") fastjet::BackgroundJetScalarPtDensity "

Class that implements (scalar pt sum of jet)/(scalar area of jet) for background
estimation *(this is a preliminary class)*.  

Optionally it can return a quantity based on the sum of pt^n, e.g. for use in
subtracting fragementation function moments.  

C++ includes: fastjet/tools/JetMedianBackgroundEstimator.hh
";

%feature("docstring") fastjet::BackgroundJetScalarPtDensity::description "

returns a description of the function (an empty string by default)  
";

%feature("docstring") fastjet::BackgroundJetScalarPtDensity::BackgroundJetScalarPtDensity "

Default constructor provides background estimation with scalar pt sum.  
";

%feature("docstring") fastjet::BackgroundJetScalarPtDensity::BackgroundJetScalarPtDensity "

Constructor to provide background estimation based on $ sum_{i\\in jet}
p_{ti}^{n} $.  
";

%feature("docstring") fastjet::BackgroundJetScalarPtDensity::result "

the action of the function this *has* to be overloaded in derived classes  

Parameters
----------
* `pj` :  
    the PseudoJet input to the function  
";

// File: classfastjet_1_1BackgroundRescalingYPolynomial.xml


%feature("docstring") fastjet::BackgroundRescalingYPolynomial "

A background rescaling that is a simple polynomial in y.  

C++ includes: fastjet/tools/BackgroundEstimatorBase.hh
";

%feature("docstring") fastjet::BackgroundRescalingYPolynomial::result "

return the rescaling factor associated with this jet  
";

%feature("docstring") fastjet::BackgroundRescalingYPolynomial::BackgroundRescalingYPolynomial "

construct a background rescaling polynomial of the form a0 + a1*y + a2*y^2 +
a3*y^3 + a4*y^4  

The following values give a reasonable reproduction of the Pythia8 tune 4C
background shape for pp collisions at sqrt(s)=7TeV:  

*   a0 = 1.157  
*   a1 = 0  
*   a2 = -0.0266  
*   a3 = 0  
*   a4 = 0.000048  
";

// File: classfastjet_1_1BasicRandom.xml


%feature("docstring") fastjet::BasicRandom "
";

%feature("docstring") fastjet::BasicRandom::max "
";

%feature("docstring") fastjet::BasicRandom::randomize "
";

%feature("docstring") fastjet::BasicRandom::print_info "
";

%feature("docstring") fastjet::BasicRandom::min "
";

// File: classfastjet_1_1BasicRandom_3_01double_01_4.xml


%feature("docstring") fastjet::BasicRandom< double > "
";

%feature("docstring") fastjet::BasicRandom< double >::set_status "
";

%feature("docstring") fastjet::BasicRandom< double >::max "

maximum value returned by the generator  
";

%feature("docstring") fastjet::BasicRandom< double >::BasicRandom "

constructor that takes two integers to specify the seed  
";

%feature("docstring") fastjet::BasicRandom< double >::randomize "

(re)initialize the random number generator from an array of seeds  
";

%feature("docstring") fastjet::BasicRandom< double >::min "

minimum value returned by the generator  
";

%feature("docstring") fastjet::BasicRandom< double >::get_status "
";

%feature("docstring") fastjet::BasicRandom< double >::print_info "

print information about the generator to the stream  
";

// File: classfastjet_1_1BasicRandom_3_01int_01_4.xml


%feature("docstring") fastjet::BasicRandom< int > "
";

%feature("docstring") fastjet::BasicRandom< int >::set_status "
";

%feature("docstring") fastjet::BasicRandom< int >::min "
";

%feature("docstring") fastjet::BasicRandom< int >::BasicRandom "
";

%feature("docstring") fastjet::BasicRandom< int >::print_info "
";

%feature("docstring") fastjet::BasicRandom< int >::get_status "
";

%feature("docstring") fastjet::BasicRandom< int >::randomize "
";

%feature("docstring") fastjet::BasicRandom< int >::max "
";

// File: classfastjet_1_1Boost.xml


%feature("docstring") fastjet::Boost "

Class to boost a PseudoJet.  

This is a FunctionOfPseudoJet with return type PseudoJet. Its action if to boost
the PseudoJet by a boost vector passed to its constructor  

C++ includes: fastjet/tools/Boost.hh
";

%feature("docstring") fastjet::Boost::Boost "

default ctor  
";

%feature("docstring") fastjet::Boost::result "

the action of the function: boost the PseudoJet by a boost vector _jet_rest  
";

// File: structfastjet_1_1ClusterSequence_1_1BriefJet.xml

// File: classfastjet_1_1CASubJetTagger.xml


%feature("docstring") fastjet::CASubJetTagger "

clean (almost parameter-free) tagger searching for the element in the clustering
history that maximises a chosen distance  

class to help us get a clean (almost parameter-free) handle on substructure
inside a C/A jet. It follows the logic described in arXiv:0906.0728 (and is
inspired by the original Cambridge algorithm paper in its use of separate
angular and dimensionful distances), but provides some extra flexibility.  

It searches for all splittings that pass a symmetry cut (zcut) and then selects
the one with the largest auxiliary scale choice (e.g. jade distance of the
splitting, kt distance of the splitting, etc.)  

By default, the zcut is calculated from the fraction of the child pt carried by
the parent jet. If one calls set_absolute_z_cut the fraction of transverse
momentum will be computed wrt the original jet.  

original code copyright (C) 2009 by Gavin Salam, released under the GPL.  
Options
*   the distance choice: options are kt2_distance : usual
    min(kti^2,ktj^2)DeltaR_{ij}^2 jade_distance : kti . ktj DeltaR_{ij}^2 (LI
    version of jade) jade2_distance : kti . ktj DeltaR_{ij}^4 (LI version of
    jade * DR^2) plain_distance : DeltaR_{ij}^2 mass_drop_distance : m_jet -
    max(m_parent1,m_parent2) dot_product_distance: parent1.parent2 (kt2_distance
    by default)  
*   the z cut (0 by default)  
*   by calling set_absolute_z_cut(), one can ask that the pt fraction if
    calculated wrt the original jet  
*   by calling set_dr_min(drmin), one can ask that only the recombinations where
    the 2 objects are (geometrically) distant by at least drmin are kept in the
    maximisation.  
Input conditions
*   the jet must have been obtained from a Cambridge/Aachen cluster sequence  
Output/structure
*   the element of the cluster sequence maximising the requested distance (and
    satisfying the zcut) is returned.  
*   if the original jet has no parents, it will be returned  
*   the value of the \"z\" and distance corresponding to that history element
    are stored and accessible through result.structure_of<CASubJetTagger>().z();
    result.structure_of<CASubJetTagger>().distance();  

C++ includes: fastjet/tools/CASubJetTagger.hh
";

%feature("docstring") fastjet::CASubJetTagger::CASubJetTagger "

just constructs  
";

%feature("docstring") fastjet::CASubJetTagger::set_dr_min "

sets a minimum delta R below which spliting will be ignored (only relevant if
set prior to calling run())  
";

%feature("docstring") fastjet::CASubJetTagger::set_absolute_z_cut "

If (abs_z_cut) is set to false (the default) then for a splitting to be
considered, each subjet must satisfy.  

p_{t,sub} > z_threshold * p_{t,parent}  

whereas if it is set to true, then each subject must satisfy  

       p_{t,sub} > z_threshold * p_{t,original-jet}  

where parent is the immediate parent of the splitting, and original jet is the
one supplied to the run() function.  

Only relevant is called prior to run().  
";

%feature("docstring") fastjet::CASubJetTagger::result "

runs the tagger on the given jet and returns the tagged PseudoJet if successful,
or a PseudoJet==0 otherwise (standard access is through operator()).  
";

%feature("docstring") fastjet::CASubJetTagger::description "

returns a textual description of the tagger  
";

// File: classfastjet_1_1CASubJetTaggerStructure.xml


%feature("docstring") fastjet::CASubJetTaggerStructure "

the structure returned by a CASubJetTagger  

Since this is directly an element of the ClusterSequence, we keep basically the
original ClusterSequenceStructure (wrapped for memory-management reasons) and
add information about the pt fraction and distance of the subjet structure  

C++ includes: fastjet/tools/CASubJetTagger.hh
";

%feature("docstring") fastjet::CASubJetTaggerStructure::absolute_z "

returns the pt fraction contained by the softer of the two component pieces of
this jet (normalised relative to the original jet)  
";

%feature("docstring") fastjet::CASubJetTaggerStructure::CASubJetTaggerStructure "

default ctor  

Parameters
----------
* `result_jet` :  
    the jet for which we have to keep the structure  
";

%feature("docstring") fastjet::CASubJetTaggerStructure::distance "

returns the value of the distance measure (corresponding to ScaleChoice) for
this jet's splitting  
";

%feature("docstring") fastjet::CASubJetTaggerStructure::z "

returns the pt fraction contained by the softer of the two component pieces of
this jet (normalised relative to this jet)  
";

%feature("docstring") fastjet::CASubJetTaggerStructure::scale_choice "

returns the scale choice asked for the maximisation  
";

// File: classfastjet_1_1CDFJetCluPlugin.xml


%feature("docstring") fastjet::CDFJetCluPlugin "

Implementation of the JetClu algorithm from CDF (plugin for fastjet-v2.1
upwards)  

C++ includes: fastjet/CDFJetCluPlugin.hh
";

%feature("docstring") fastjet::CDFJetCluPlugin::run_clustering "

given a ClusterSequence that has been filled up with initial particles, the
following function should fill up the rest of the ClusterSequence, using the
following member functions of ClusterSequence:  

*   plugin_do_ij_recombination(...)  
*   plugin_do_iB_recombination(...)  
";

%feature("docstring") fastjet::CDFJetCluPlugin::adjacency_cut "
";

%feature("docstring") fastjet::CDFJetCluPlugin::description "

return a textual description of the jet-definition implemented in this plugin  
";

%feature("docstring") fastjet::CDFJetCluPlugin::R "

the plugin mechanism's standard way of accessing the jet radius  
";

%feature("docstring") fastjet::CDFJetCluPlugin::CDFJetCluPlugin "

a compact constructor  
";

%feature("docstring") fastjet::CDFJetCluPlugin::CDFJetCluPlugin "

a constructor that looks like the one provided by CDF  
";

%feature("docstring") fastjet::CDFJetCluPlugin::max_iterations "
";

%feature("docstring") fastjet::CDFJetCluPlugin::seed_threshold "
";

%feature("docstring") fastjet::CDFJetCluPlugin::overlap_threshold "
";

%feature("docstring") fastjet::CDFJetCluPlugin::iratch "
";

%feature("docstring") fastjet::CDFJetCluPlugin::cone_radius "
";

// File: classfastjet_1_1CDFMidPointPlugin.xml


%feature("docstring") fastjet::CDFMidPointPlugin "

Implementation of the MidPoint algorithm from CDF (plugin for fastjet-v2.1
upwards)  

A plugin for fastjet-v2.1 that provides an interface to the CDF midpoint
algorithm  

CDFMidPointPlugin is a plugin for fastjet (v2.1 upwards) that provides an
interface to the CDF version of Run-II iterative cone algorithm with midpoint
seeds (also known as the Iterative Legacy Cone Algorithm, ILCA).  

The CDF code has been taken from Joey Huston's webpage
http://www.pa.msu.edu/~huston/Les_Houches_2005/Les_Houches_SM.html  

Note that the CDF midpoint code contains options that go beyond those described
in the Tevatron run-II document (hep-ex/0005012), notably search-cones, as
described in hep-ph/0111434, and midpoints bewteen multiplets of stable cones.  

Additionally, the version of the CDF midpoint code distributed here has been
modified by the FastJet authors, so as to allow one to choose the scale used in
the split-merge step.  

C++ includes: fastjet/CDFMidPointPlugin.hh
";

%feature("docstring") fastjet::CDFMidPointPlugin::description "

return a textual description of the jet-definition implemented in this plugin  
";

%feature("docstring") fastjet::CDFMidPointPlugin::cone_area_fraction "
";

%feature("docstring") fastjet::CDFMidPointPlugin::seed_threshold "
";

%feature("docstring") fastjet::CDFMidPointPlugin::CDFMidPointPlugin "

A CDFMidPointPlugin constructor that looks like the one provided by CDF.  

Its arguments should have the following meaning:  

*   seed_threshold: minimum pt for a particle to be considered a seed of the
    iteration.  
*   cone_radius: standard meaning  
*   cone_area_fraction: stable-cones are searched for with a radius Rsearch = R
    * sqrt(cone_area_fraction), and then expanded to size R afterwards; note
    (hep-ph/0610012) that this introduces IR unsafety at NLO for X+2-jet
    observables (where X any hard object).  
*   max_pair_size: \"midpoints\" can be added between pairs of stable cones,
    triplets of stable cones, etc.; max_pair_size indicates the maximum number
    of stable cones that are assembled when adding midpoints.  
*   max_iterations: the maximum number of iterations to carry out when looking
    for a stable cone.  
*   overlap_threshold: if (overlapping_Et)/(Et_of_softer_protojet) <
    overlap_threshold, overlapping jets are split, otherwise they are merged.  
*   sm_scale: a choice for the scale to be used in the split-merge step (both
    for ordering the momenta and quantifying the overlap); the three options are  

    . SM_pt: pt (default -- source of small IR safety issue in purely hadronic
    events)  

    . SM_Et: Et (not boost invariant, reduces to mt at zero rapidity and to pt
    and infinite rapidity)  

    . SM_mt: transverse mass = sqrt(m^2+pt^2)  
";

%feature("docstring") fastjet::CDFMidPointPlugin::CDFMidPointPlugin "

a compact constructor  

NB: as of version 2.4, the default value for the overlap_threshold threshold has
been removed, to avoid misleading people into using the value of 0.5 without
thinking, which is known to have adverse effects in high-noise environments. A
recommended value is 0.75.  
";

%feature("docstring") fastjet::CDFMidPointPlugin::run_clustering "

given a ClusterSequence that has been filled up with initial particles, the
following function should fill up the rest of the ClusterSequence, using the
following member functions of ClusterSequence:  

*   plugin_do_ij_recombination(...)  
*   plugin_do_iB_recombination(...)  
";

%feature("docstring") fastjet::CDFMidPointPlugin::max_pair_size "
";

%feature("docstring") fastjet::CDFMidPointPlugin::R "

the plugin mechanism's standard way of accessing the jet radius  
";

%feature("docstring") fastjet::CDFMidPointPlugin::overlap_threshold "
";

%feature("docstring") fastjet::CDFMidPointPlugin::max_iterations "
";

%feature("docstring") fastjet::CDFMidPointPlugin::cone_radius "
";

// File: classfastjet_1_1CircularRange.xml


%feature("docstring") fastjet::CircularRange "
";

%feature("docstring") fastjet::CircularRange::__attribute__ "

constructor  
";

%feature("docstring") fastjet::CircularRange::__attribute__ "

initialise CircularRange with a jet  
";

%feature("docstring") fastjet::CircularRange::__attribute__ "

initialise CircularRange with a (rap,phi) point  
";

%feature("docstring") fastjet::CircularRange::__attribute__ "

initialise CircularRange with just the radius parameter  
";

%feature("docstring") fastjet::CircularRange::is_localizable "

returns true since this range is localizable (i.e.  

set_position does something meaningful)  
";

%feature("docstring") fastjet::CircularRange::is_in_range "

return bool according to whether (rap,phi) is in range  
";

%feature("docstring") fastjet::CircularRange::get_rap_limits "

return the minimal and maximal rapidity of this range  
";

%feature("docstring") fastjet::CircularRange::description "

return description of range  
";

%feature("docstring") fastjet::CircularRange::~CircularRange "

destructor  
";

// File: classfastjet_1_1SearchTree_1_1circulator.xml


%feature("docstring") fastjet::SearchTree::circulator "
";

%feature("docstring") fastjet::SearchTree::circulator::circulator "
";

%feature("docstring") fastjet::SearchTree::circulator::circulator "
";

%feature("docstring") fastjet::SearchTree::circulator::SearchTree< T > "
";

%feature("docstring") fastjet::SearchTree::circulator::next "

return a circulator referring to the next node  
";

%feature("docstring") fastjet::SearchTree::circulator::previous "

return a circulator referring to the previous node  
";

// File: classfastjet_1_1ClosestPair2D.xml


%feature("docstring") fastjet::ClosestPair2D "
";

%feature("docstring") fastjet::ClosestPair2D::replace_many "

replaces IDs_to_remove with points at the new_positions indicating the IDs
allocated to the new points in new_IDs  
";

%feature("docstring") fastjet::ClosestPair2D::closest_pair "

provides the IDs of the closest pair as well as the distance between them  
";

%feature("docstring") fastjet::ClosestPair2D::replace "

removes ID1 and ID2 and inserts position, returning the ID corresponding to
position...  
";

%feature("docstring") fastjet::ClosestPair2D::print_tree_depths "
";

%feature("docstring") fastjet::ClosestPair2D::ClosestPair2D "

constructor from a vector of 2D positions -- number of objects after insertion
and deletion must never exceed positions.size(); objects are given IDs that
correspond to their index in the vector of positions  
";

%feature("docstring") fastjet::ClosestPair2D::ClosestPair2D "

constructor which allows structure to grow beyond positions.size(), up to
max_size  
";

%feature("docstring") fastjet::ClosestPair2D::size "
";

%feature("docstring") fastjet::ClosestPair2D::insert "

inserts the position into the closest pair structure and returns the ID that has
been allocated for the object.  
";

%feature("docstring") fastjet::ClosestPair2D::remove "

removes the entry labelled by ID from the object;  
";

// File: classfastjet_1_1ClosestPair2DBase.xml


%feature("docstring") fastjet::ClosestPair2DBase "
";

%feature("docstring") fastjet::ClosestPair2DBase::~ClosestPair2DBase "
";

%feature("docstring") fastjet::ClosestPair2DBase::replace_many "

replaces IDs_to_remove with points at the new_positions indicating the IDs
allocated to the new points in new_IDs  
";

%feature("docstring") fastjet::ClosestPair2DBase::replace "

replaces the specified ID1 and ID2 with something at a new position assuming
that ID1 and ID2 are in sequence wrt position; it returns the ID of the new
object...  
";

%feature("docstring") fastjet::ClosestPair2DBase::insert "

inserts the position into the closest pair structure and returns the ID that has
been allocated for the object.  
";

%feature("docstring") fastjet::ClosestPair2DBase::size "
";

%feature("docstring") fastjet::ClosestPair2DBase::remove "

removes the entry labelled by ID from the object;  
";

%feature("docstring") fastjet::ClosestPair2DBase::closest_pair "

provides the IDs of the closest pair as well as the squared distance between
them  
";

// File: classfastjet_1_1ClusterSequence.xml


%feature("docstring") fastjet::ClusterSequence "

deals with clustering  

C++ includes: fastjet/ClusterSequence.hh
";

%feature("docstring") fastjet::ClusterSequence::jet_scale_for_algorithm "

returns the scale associated with a jet as required for this clustering
algorithm (kt^2 for the kt-algorithm, 1 for the Cambridge algorithm).  

Intended mainly for internal use and not valid for plugin algorithms.  
";

%feature("docstring") fastjet::ClusterSequence::exclusive_subdmerge "

returns the dij that was present in the merging nsub+1 -> nsub subjets inside
this jet.  

return the dij that was present in the merging nsub+1 -> nsub subjets inside
this jet.  

Returns 0 if there were nsub or fewer constituents in the jet.  

If the jet has nsub or fewer constituents, it will return 0.  

will be zero if nconst <= nsub, since highest will be an original particle have
zero dij  
";

%feature("docstring") fastjet::ClusterSequence::signal_imminent_self_deletion "

tell the ClusterSequence it's about to be self deleted (internal use only)  
";

%feature("docstring") fastjet::ClusterSequence::exclusive_subjets "

return a vector of all subjets of the current jet (in the sense of the exclusive
algorithm) that would be obtained when running the algorithm with the given
dcut.  

Time taken is O(m ln m), where m is the number of subjets that are found. If m
gets to be of order of the total number of constituents in the jet, this could
be substantially slower than just getting that list of constituents.  
";

%feature("docstring") fastjet::ClusterSequence::exclusive_subjets "

return the list of subjets obtained by unclustering the supplied jet down to
nsub subjets.  

Throws an error if there are fewer than nsub particles in the jet.  

This requires nsub ln nsub time  

Throws an error if there are fewer than nsub particles in the jet.  
";

%feature("docstring") fastjet::ClusterSequence::has_partner "

if this jet has a child (and so a partner) return true and give the partner,
otherwise return false and set the partner to zero  
";

%feature("docstring") fastjet::ClusterSequence::delete_self_when_unused "

by calling this routine you tell the ClusterSequence to delete itself when all
the Pseudojets associated with it have gone out of scope.  

At the time you call this, there must be at least one jet or other object
outside the CS that is associated with the CS (e.g. the result of
inclusive_jets()).  

NB: after having made this call, the user is still allowed to delete the CS.
Jets associated with it will then simply not be able to access their
substructure after that point.  
";

%feature("docstring") fastjet::ClusterSequence::~ClusterSequence "
";

%feature("docstring") fastjet::ClusterSequence::transfer_from_sequence "

transfer the sequence contained in other_seq into our own; any plugin \"extras\"
contained in the from_seq will be lost from there.  

It also sets the ClusterSequence pointers of the PseudoJets in the history to
point to this ClusterSequence  

When specified, the second argument is an action that will be applied on every
jets in the resulting ClusterSequence  
";

%feature("docstring") fastjet::ClusterSequence::history "

allow the user to access the raw internal history.  

This is present (as for jets()) in part so that protected derived classes can
access this information about other ClusterSequences.  

A user who wishes to follow the details of the ClusterSequence can also make use
of this information (and should consult the history_element documentation for
more information), but should be aware that these internal structures may evolve
in future FastJet versions.  
";

%feature("docstring") fastjet::ClusterSequence::exclusive_dmerge "

return the dmin corresponding to the recombination that went from n+1 to n jets
(sometimes known as d_{n n+1}).  

return the dmin corresponding to the recombination that went from n+1 to n jets  

If the number of particles in the event is <= njets, the function returns 0.  
";

%feature("docstring") fastjet::ClusterSequence::jets "

allow the user to access the internally stored _jets() array, which contains
both the initial particles and the various intermediate and final stages of
recombination.  

The first n_particles() entries are the original particles, in the order in
which they were supplied to the ClusterSequence constructor. It can be useful to
access them for example when examining whether a given input object is part of a
specific jet, via the objects_in_jet(...) member function (which only takes
PseudoJets that are registered in the ClusterSequence).  

One of the other (internal uses) is related to the fact because we don't seem to
be able to access protected elements of the class for an object that is not
\"this\" (at least in case where \"this\" is of a slightly different kind from
the object, both derived from ClusterSequence).  
";

%feature("docstring") fastjet::ClusterSequence::will_delete_self_when_unused "

return true if the object has been told to delete itself when unused  
";

%feature("docstring") fastjet::ClusterSequence::has_child "

if the jet has a child then return true and give the child jet otherwise return
false and set the child to zero  
";

%feature("docstring") fastjet::ClusterSequence::has_child "

Version of has_child that sets a pointer to the child if the child exists;.  
";

%feature("docstring") fastjet::ClusterSequence::n_exclusive_subjets "

return the size of exclusive_subjets(...); still n ln n with same coefficient,
but marginally more efficient than manually taking exclusive_subjets.size()  
";

%feature("docstring") fastjet::ClusterSequence::plugin_simple_N2_cluster "

allows a plugin to run a templated clustering (nearest-neighbour heuristic)  

This has N^2 behaviour on \"good\" distance, but a worst case behaviour of N^3
(and many algs trigger the worst case behaviour)  

For more details on how this works, see GenBriefJet below  
";

%feature("docstring") fastjet::ClusterSequence::Q2 "

return Q()^2  
";

%feature("docstring") fastjet::ClusterSequence::print_jets_for_root "

output the supplied vector of jets in a format that can be read by an
appropriate root script; the format is: jet-n jet-px jet-py jet-pz jet-E
particle-n particle-rap particle-phi particle-pt particle-n particle-rap
particle-phi particle-pt ...  

#END ... [i.e. above repeated]  
";

%feature("docstring") fastjet::ClusterSequence::print_jets_for_root "

print jets for root to the file labelled filename, with an optional comment at
the beginning  
";

%feature("docstring") fastjet::ClusterSequence::inclusive_jets "

return a vector of all jets (in the sense of the inclusive algorithm) with pt >=
ptmin.  

Time taken should be of the order of the number of jets returned.  
";

%feature("docstring") fastjet::ClusterSequence::_bj_dist "
";

%feature("docstring") fastjet::ClusterSequence::unclustered_particles "

return the set of particles that have not been clustered.  

For kt and cam/aachen algorithms this should always be null, but for cone type
algorithms it can be non-null;  
";

%feature("docstring") fastjet::ClusterSequence::exclusive_jets_up_to "

return a vector of all jets when the event is clustered (in the exclusive sense)
to exactly njets.  

If there are fewer than njets particles in the ClusterSequence the function just
returns however many particles there were.  
";

%feature("docstring") fastjet::ClusterSequence::plugin_record_ij_recombination "

record the fact that there has been a recombination between jets()[jet_i] and
jets()[jet_k], with the specified dij, and return the index (newjet_k) allocated
to the new jet, whose momentum is assumed to be the 4-vector sum of that of
jet_i and jet_j  
";

%feature("docstring") fastjet::ClusterSequence::plugin_record_ij_recombination "

as for the simpler variant of plugin_record_ij_recombination, except that the
new jet is attributed the momentum and user_index of newjet  
";

%feature("docstring") fastjet::ClusterSequence::exclusive_ymerge_max "

same as exclusive_dmerge_max, but normalised to squared total energy  
";

%feature("docstring") fastjet::ClusterSequence::exclusive_ymerge "

return the ymin corresponding to the recombination that went from n+1 to n jets
(sometimes known as y_{n n+1}).  
";

%feature("docstring") fastjet::ClusterSequence::object_in_jet "

returns true iff the object is included in the jet.  

NB: this is only sensible if the object is already registered within the cluster
sequence, so you cannot use it with an input particle to the CS (since the
particle won't have the history index set properly).  

For nice clustering structures it should run in O(ln(N)) time but in worst cases
(certain cone plugins) it can take O(n) time, where n is the number of particles
in the jet.  
";

%feature("docstring") fastjet::ClusterSequence::unique_history_order "

routine that returns an order in which to read the history such that clusterings
that lead to identical jet compositions but different histories (because of
degeneracies in the clustering order) will have matching constituents for each
matching entry in the unique_history_order.  

The order has the property that an entry's parents will always appear prior to
that entry itself.  

Roughly speaking the order is such that we first provide all steps that lead to
the final jet containing particle 1; then we have the steps that lead to
reconstruction of the jet containing the next-lowest-numbered unclustered
particle, etc... [see GPS CCN28-12 for more info -- of course a full explanation
here would be better...]  
";

%feature("docstring") fastjet::ClusterSequence::exclusive_jets "

return a vector of all jets (in the sense of the exclusive algorithm) that would
be obtained when running the algorithm with the given dcut.  
";

%feature("docstring") fastjet::ClusterSequence::exclusive_jets "

return a vector of all jets when the event is clustered (in the exclusive sense)
to exactly njets.  

If there are fewer than njets particles in the ClusterSequence an error is
thrown  
";

%feature("docstring") fastjet::ClusterSequence::exclusive_jets_ycut "

the exclusive jets obtained at the given ycut  
";

%feature("docstring") fastjet::ClusterSequence::exclusive_subjets_up_to "

return the list of subjets obtained by unclustering the supplied jet down to
nsub subjets (or all constituents if there are fewer than nsub).  

This requires nsub ln nsub time  
";

%feature("docstring") fastjet::ClusterSequence::n_particles "

returns the number of particles that were provided to the clustering algorithm
(helps the user find their way around the history and jets objects if they
weren't paying attention beforehand).  
";

%feature("docstring") fastjet::ClusterSequence::has_parents "

if the jet has parents in the clustering, it returns true and sets parent1 and
parent2 equal to them.  

if it has no parents it returns false and sets parent1 and parent2 to zero  
";

%feature("docstring") fastjet::ClusterSequence::plugin_associate_extras "

the plugin can associate some extra information with the ClusterSequence object
by calling this function.  

The ClusterSequence takes ownership of the pointer (and responsibility for
deleting it when the CS gets deleted).  
";

%feature("docstring") fastjet::ClusterSequence::ClusterSequence "

default constructor  
";

%feature("docstring") fastjet::ClusterSequence::ClusterSequence "

create a ClusterSequence, starting from the supplied set of PseudoJets and
clustering them with jet definition specified by jet_def (which also specifies
the clustering strategy)  

constructor of a jet-clustering sequence from a vector of four-momenta, with the
jet definition specified by jet_def  
";

%feature("docstring") fastjet::ClusterSequence::ClusterSequence "

copy constructor for a ClusterSequence  
";

%feature("docstring") fastjet::ClusterSequence::add_constituents "

add on to subjet_vector the constituents of jet (for internal use mainly)  
";

%feature("docstring") fastjet::ClusterSequence::strategy_string "

return the name of the strategy used to cluster the event  
";

%feature("docstring") fastjet::ClusterSequence::strategy_string "

return the name of the strategy associated with the enum strategy_in  
";

%feature("docstring") fastjet::ClusterSequence::print_banner "

This is the function that is automatically called during clustering to print the
FastJet banner.  

Only the first call to this function will result in the printout of the banner.
Users may wish to call this function themselves, during the initialization phase
of their program, in order to ensure that the banner appears before other
output. This call will not affect 3rd-party banners, e.g. those from plugins.  
";

%feature("docstring") fastjet::ClusterSequence::exclusive_dmerge_max "

return the maximum of the dmin encountered during all recombinations up to the
one that led to an n-jet final state; identical to exclusive_dmerge, except in
cases where the dmin do not increase monotonically.  
";

%feature("docstring") fastjet::ClusterSequence::plugin_activated "

returns true when the plugin is allowed to run the show.  
";

%feature("docstring") fastjet::ClusterSequence::n_exclusive_jets "

return the number of jets (in the sense of the exclusive algorithm) that would
be obtained when running the algorithm with the given dcut.  
";

%feature("docstring") fastjet::ClusterSequence::exclusive_subdmerge_max "

returns the maximum dij that occurred in the whole event at the stage that the
nsub+1 -> nsub merge of subjets occurred inside this jet.  

return the maximum dij that occurred in the whole event at the stage that the
nsub+1 -> nsub merge of subjets occurred inside this jet.  

Returns 0 if there were nsub or fewer constituents in the jet.  

If the jet has nsub or fewer constituents, it will return 0.  

will be zero if nconst <= nsub, since highest will be an original particle have
zero dij  
";

%feature("docstring") fastjet::ClusterSequence::contains "

returns true if the object (jet or particle) is contained by (ie belongs to)
this cluster sequence.  

Tests performed: if thejet's interface is this cluster sequence and its cluster
history index is in a consistent range.  
";

%feature("docstring") fastjet::ClusterSequence::Q "

returns the sum of all energies in the event (relevant mainly for e+e-)  
";

%feature("docstring") fastjet::ClusterSequence::extras "

returns a pointer to the extras object (may be null)  
";

%feature("docstring") fastjet::ClusterSequence::_bj_set_jetinfo "
";

%feature("docstring") fastjet::ClusterSequence::plugin_record_iB_recombination "

record the fact that there has been a recombination between jets()[jet_i] and
the beam, with the specified diB; when looking for inclusive jets, any iB
recombination will returned to the user as a jet.  
";

%feature("docstring") fastjet::ClusterSequence::strategy_used "

return the enum value of the strategy used to cluster the event  
";

%feature("docstring") fastjet::ClusterSequence::constituents "

return a vector of the particles that make up jet  
";

%feature("docstring") fastjet::ClusterSequence::childless_pseudojets "

Return the list of pseudojets in the ClusterSequence that do not have children
(and are not among the inclusive jets).  

They may result from a clustering step or may be one of the pseudojets returned
by unclustered_particles().  
";

%feature("docstring") fastjet::ClusterSequence::jet_def "

return a reference to the jet definition  
";

%feature("docstring") fastjet::ClusterSequence::fastjet_banner_stream "

returns a pointer to the stream to be used to print banners (cout by default).  

This function is used by plugins to determine where to direct their banners.
Plugins should properly handle the case where the pointer is null.  
";

%feature("docstring") fastjet::ClusterSequence::particle_jet_indices "

returns a vector of size n_particles() which indicates, for each of the initial
particles (in the order in which they were supplied), which of the supplied jets
it belongs to; if it does not belong to any of the supplied jets, the index is
set to -1;  
";

%feature("docstring") fastjet::ClusterSequence::__attribute__ "

the plugin can associate some extra information with the ClusterSequence object
by calling this function  

As of FJ v3.1, this is deprecated, in line with the deprecation of auto_ptr in
C++11  
";

%feature("docstring") fastjet::ClusterSequence::n_exclusive_jets_ycut "

the number of exclusive jets at the given ycut  
";

%feature("docstring") fastjet::ClusterSequence::structure_shared_ptr "

retrieve a shared pointer to the wrapper to this ClusterSequence  

this may turn useful if you want to track when this ClusterSequence goes out of
scope  
";

// File: classfastjet_1_1ClusterSequence1GhostPassiveArea.xml


%feature("docstring") fastjet::ClusterSequence1GhostPassiveArea "

Like ClusterSequence with computation of the passive jet area by adding a single
ghost.  

Class that behaves essentially like ClusterSequence except that it also provides
access to the area of a jet (which will be a random quantity... Figure out what
to do about seeds later...)  

This class should not be used directly. Rather use ClusterSequenceArea  

C++ includes: fastjet/ClusterSequence1GhostPassiveArea.hh
";

%feature("docstring") fastjet::ClusterSequence1GhostPassiveArea::ClusterSequence1GhostPassiveArea "
";

%feature("docstring") fastjet::ClusterSequence1GhostPassiveArea::ClusterSequence1GhostPassiveArea "

constructor based on JetDefinition and 1GhostPassiveAreaSpec  
";

%feature("docstring") fastjet::ClusterSequence1GhostPassiveArea::n_empty_jets "

return an estimate for the number of empty jets -- one uses the AreaBase one
rather than the ActiveArea one (which for which we do not have the information).  
";

// File: classfastjet_1_1ClusterSequenceActiveArea.xml


%feature("docstring") fastjet::ClusterSequenceActiveArea "

Like ClusterSequence with computation of the active jet area.  

Class that behaves essentially like ClusterSequence except that it also provides
access to the area of a jet (which will be a random quantity... Figure out what
to do about seeds later...)  

This class should not be used directly. Rather use ClusterSequenceArea with the
appropriate AreaDefinition  

C++ includes: fastjet/ClusterSequenceActiveArea.hh
";

%feature("docstring") fastjet::ClusterSequenceActiveArea::area "

return the area associated with the given jet; this base class returns 0.  
";

%feature("docstring") fastjet::ClusterSequenceActiveArea::ClusterSequenceActiveArea "

default constructor  
";

%feature("docstring") fastjet::ClusterSequenceActiveArea::ClusterSequenceActiveArea "

constructor based on JetDefinition and GhostedAreaSpec  
";

%feature("docstring") fastjet::ClusterSequenceActiveArea::empty_area "

rewrite the empty area from the parent class, so as to use all info at our
disposal return the total area, corresponding to a given Selector, that consists
of ghost jets or unclustered ghosts  

The selector passed as an argument needs to apply jet by jet.  
";

%feature("docstring") fastjet::ClusterSequenceActiveArea::pt_per_unit_area "

return the transverse momentum per unit area according to one of the above
strategies; for some strategies (those with \"cut\" in their name) the parameter
\"range\" allows one to exclude a subset of the jets for the background
estimation, those that have pt/area > median(pt/area)*range.  

NB: This call is OBSOLETE and deprecated; use a JetMedianBackgroundEstimator or
GridMedianBackgroundEstimator instead.  
";

%feature("docstring") fastjet::ClusterSequenceActiveArea::area_error "

return the error (uncertainty) associated with the determination of the area of
this jet; this base class returns 0.  
";

%feature("docstring") fastjet::ClusterSequenceActiveArea::area_4vector "

return a PseudoJet whose 4-vector is defined by the following integral  

drap d PseudoJet(\"rap,phi,pt=one\") *  

*   Theta(\"rap,phi inside jet boundary\")  

where PseudoJet(\"rap,phi,pt=one\") is a 4-vector with the given rapidity (rap),
azimuth (phi) and pt=1, while Theta(\"rap,phi
inside jet boundary\") is a function that is 1 when rap,phi define a direction
inside the jet boundary and 0 otherwise.  

This base class returns a null 4-vector.  
";

%feature("docstring") fastjet::ClusterSequenceActiveArea::n_empty_jets "

return the true number of empty jets (replaces
ClusterSequenceAreaBase::n_empty_jets(...))  
";

// File: classfastjet_1_1ClusterSequenceActiveAreaExplicitGhosts.xml


%feature("docstring") fastjet::ClusterSequenceActiveAreaExplicitGhosts "

Like ClusterSequence with computation of the active jet area with the addition
of explicit ghosts.  

Class that behaves essentially like ClusterSequence except that it also provides
access to the area of a jet (which will be a random quantity... Figure out what
to do about seeds later...)  

This class should not be used directly. Rather use ClusterSequenceArea with the
appropriate AreaDefinition  

C++ includes: fastjet/ClusterSequenceActiveAreaExplicitGhosts.hh
";

%feature("docstring") fastjet::ClusterSequenceActiveAreaExplicitGhosts::has_dangerous_particles "

returns true if there are any particles whose transverse momentum if so low that
there's a risk of the ghosts having modified the clustering sequence  
";

%feature("docstring") fastjet::ClusterSequenceActiveAreaExplicitGhosts::has_explicit_ghosts "

this class does have explicit ghosts  
";

%feature("docstring") fastjet::ClusterSequenceActiveAreaExplicitGhosts::ClusterSequenceActiveAreaExplicitGhosts "

constructor using a GhostedAreaSpec to specify how the area is to be measured  
";

%feature("docstring") fastjet::ClusterSequenceActiveAreaExplicitGhosts::ClusterSequenceActiveAreaExplicitGhosts "
";

%feature("docstring") fastjet::ClusterSequenceActiveAreaExplicitGhosts::area_4vector "

returns a four vector corresponding to the sum (E-scheme) of the ghost four-
vectors composing the jet area, normalised such that for a small contiguous area
the p_t of the extended_area jet is equal to area of the jet.  
";

%feature("docstring") fastjet::ClusterSequenceActiveAreaExplicitGhosts::empty_area "

return the total area, corresponding to a given Selector, that consists of
unclustered ghosts  

The selector needs to apply jet by jet  
";

%feature("docstring") fastjet::ClusterSequenceActiveAreaExplicitGhosts::area "

returns the area of a jet  
";

%feature("docstring") fastjet::ClusterSequenceActiveAreaExplicitGhosts::n_hard_particles "

returns the number of hard particles (i.e. those supplied by the user).  
";

%feature("docstring") fastjet::ClusterSequenceActiveAreaExplicitGhosts::is_pure_ghost "

true if a jet is made exclusively of ghosts  
";

%feature("docstring") fastjet::ClusterSequenceActiveAreaExplicitGhosts::is_pure_ghost "

true if the entry in the history index corresponds to a ghost; if hist_ix does
not correspond to an actual particle (i.e.  

hist_ix < 0), then the result is false.  
";

%feature("docstring") fastjet::ClusterSequenceActiveAreaExplicitGhosts::_initialise "

does the actual work of initialisation  
";

%feature("docstring") fastjet::ClusterSequenceActiveAreaExplicitGhosts::max_ghost_perp2 "

returns the largest squared transverse momentum among all ghosts  
";

%feature("docstring") fastjet::ClusterSequenceActiveAreaExplicitGhosts::total_area "

returns the total area under study  
";

// File: classfastjet_1_1ClusterSequenceArea.xml


%feature("docstring") fastjet::ClusterSequenceArea "

General class for user to obtain ClusterSequence with additional area
information.  

Based on the area_def, it automatically dispatches the work to the appropriate
actual ClusterSequenceAreaBase-derived-class to do the real work.  

C++ includes: fastjet/ClusterSequenceArea.hh
";

%feature("docstring") fastjet::ClusterSequenceArea::area_4vector "

return the 4-vector area  
";

%feature("docstring") fastjet::ClusterSequenceArea::empty_area "

return the total area, corresponding to the given selector, that is free of jets  

The selector needs to have a finite area and be applicable jet by jet (see the
BackgroundEstimator and Subtractor tools for more advanced usage)  
";

%feature("docstring") fastjet::ClusterSequenceArea::is_pure_ghost "

true if a jet is made exclusively of ghosts  
";

%feature("docstring") fastjet::ClusterSequenceArea::area_def "

return a reference to the area definition  
";

%feature("docstring") fastjet::ClusterSequenceArea::area_error "

return the error (uncertainty) associated with the determination of the area of
this jet  
";

%feature("docstring") fastjet::ClusterSequenceArea::parabolic_pt_per_unit_area "

overload version of what's in the ClusterSequenceAreaBase class, which
additionally checks compatibility between \"range\" and region in which ghosts
are thrown.  
";

%feature("docstring") fastjet::ClusterSequenceArea::has_explicit_ghosts "

true if this ClusterSequence has explicit ghosts  
";

%feature("docstring") fastjet::ClusterSequenceArea::area "

return the area associated with the given jet  
";

%feature("docstring") fastjet::ClusterSequenceArea::get_median_rho_and_sigma "

overload version of what's in the ClusterSequenceAreaBase class, which
additionally checks compatibility between \"selector\" and region in which
ghosts are thrown.  

The selector needs to have a finite area and be applicable jet by jet (see the
BackgroundEstimator and Subtractor tools for more advanced usage)  
";

%feature("docstring") fastjet::ClusterSequenceArea::get_median_rho_and_sigma "

overload version of what's in the ClusterSequenceAreaBase class, which actually
just does the same thing as the base version (but since we've overridden the
5-argument version above, we have to override the 4-argument version too.  
";

%feature("docstring") fastjet::ClusterSequenceArea::get_median_rho_and_sigma "

overload version of what's in the ClusterSequenceAreaBase class, which actually
just does the same thing as the base version (but since we've overridden the
multi-argument version above, we have to override the 5-argument version too.  
";

%feature("docstring") fastjet::ClusterSequenceArea::n_empty_jets "

return something similar to the number of pure ghost jets in the given rap-phi
range in an active area case.  

For the local implementation we return empty_area/(0.55 pi R^2), based on
measured properties of ghost jets with kt and cam. Note that the number returned
is a double.  

The selector needs to have a finite area and be applicable jet by jet (see the
BackgroundEstimator and Subtractor tools for more advanced usage)  
";

%feature("docstring") fastjet::ClusterSequenceArea::ClusterSequenceArea "

main constructor  
";

%feature("docstring") fastjet::ClusterSequenceArea::ClusterSequenceArea "

constructor with a GhostedAreaSpec  
";

%feature("docstring") fastjet::ClusterSequenceArea::ClusterSequenceArea "

constructor with a VoronoiAreaSpec  
";

// File: classfastjet_1_1ClusterSequenceAreaBase.xml


%feature("docstring") fastjet::ClusterSequenceAreaBase "

base class that sets interface for extensions of ClusterSequence that provide
information about the area of each jet  

the virtual functions here all return 0, since no area determination is
implemented.  

C++ includes: fastjet/ClusterSequenceAreaBase.hh
";

%feature("docstring") fastjet::ClusterSequenceAreaBase::parabolic_pt_per_unit_area "

fits a form pt_per_unit_area(y) = a + b*y^2 in the selector range.  

fits a form pt_per_unit_area(y) = a + b*y^2 for jets in range.  

exclude_above allows one to exclude large values of pt/area from fit. (if
negative, the cut is discarded) use_area_4vector = true uses the 4vector areas.  

The selector passed as an argument has to have a finite area and apply jet-by-
jet (see the BackgroundEstimator and Subtractor tools for more generic usages)  

exclude_above allows one to exclude large values of pt/area from fit.
use_area_4vector = true uses the 4vector areas.  
";

%feature("docstring") fastjet::ClusterSequenceAreaBase::area_4vector "

return a PseudoJet whose 4-vector is defined by the following integral  

drap d PseudoJet(\"rap,phi,pt=one\") *  

*   Theta(\"rap,phi inside jet boundary\")  

where PseudoJet(\"rap,phi,pt=one\") is a 4-vector with the given rapidity (rap),
azimuth (phi) and pt=1, while Theta(\"rap,phi
inside jet boundary\") is a function that is 1 when rap,phi define a direction
inside the jet boundary and 0 otherwise.  

This base class returns a null 4-vector.  
";

%feature("docstring") fastjet::ClusterSequenceAreaBase::is_pure_ghost "

true if a jet is made exclusively of ghosts  

NB: most area classes do not give any explicit ghost jets, but some do, and they
should replace this function with their own version.  
";

%feature("docstring") fastjet::ClusterSequenceAreaBase::empty_area_from_jets "

return the total area, corresponding to the given Selector, that is free of
jets, based on the supplied all_jets  

return the total area, within range, that is free of jets.  

The selector passed as an argument has to have a finite area and apply jet-by-
jet (see the BackgroundEstimator and Subtractor tools for more generic usages)  

Calculate this as (range area) - {i in range} A_i  
";

%feature("docstring") fastjet::ClusterSequenceAreaBase::ClusterSequenceAreaBase "

a constructor which just carries out the construction of the parent class  
";

%feature("docstring") fastjet::ClusterSequenceAreaBase::ClusterSequenceAreaBase "

default constructor  
";

%feature("docstring") fastjet::ClusterSequenceAreaBase::area "

return the area associated with the given jet; this base class returns 0.  
";

%feature("docstring") fastjet::ClusterSequenceAreaBase::empty_area "

return the total area, corresponding to the given Selector, that is free of
jets, in general based on the inclusive jets.  

return the total area, within the selector's range, that is free of jets.  

The selector passed as an argument has to have a finite area and apply jet-by-
jet (see the BackgroundEstimator and Subtractor tools for more generic usages)  

Calculate this as (range area) - {i in range} A_i  

for ClusterSequences with explicit ghosts, assume that there will never be any
empty area, i.e. it is always filled in by pure ghosts jets. This holds for
seq.rec. algorithms  
";

%feature("docstring") fastjet::ClusterSequenceAreaBase::get_median_rho_and_sigma "

using jets withing the selector range (and with 4-vector areas if
use_area_4vector), calculate the median pt/area, as well as an \"error\"
(uncertainty), which is defined as the 1-sigma half-width of the distribution of
pt/A, obtained by looking for the point below which we have (1-0.6827)/2 of the
jets (including empty jets).  

The subtraction for a jet with uncorrected pt pt^U and area A is  

pt^S = pt^U - median*A +- sigma*sqrt(A)  

where the error is only that associated with the fluctuations in the noise and
not that associated with the noise having caused changes in the hard-particle
content of the jet.  

The selector passed as an argument has to have a finite area and apply jet-by-
jet (see the BackgroundEstimator and Subtractor tools for more generic usages)  

NB: subtraction may also be done with 4-vector area of course, and this is
recommended for jets with larger values of R, as long as rho has also been
determined with a 4-vector area; using a scalar area causes one to neglect terms
of relative order $R^2/8$ in the jet $p_t$.  
";

%feature("docstring") fastjet::ClusterSequenceAreaBase::get_median_rho_and_sigma "

a more advanced version of get_median_rho_and_sigma, which allows one to use any
\"view\" of the event containing all jets (so that, e.g.  

one might use Cam on a different resolution scale without have to rerun the
algorithm).  

By default it will assume that \"all\" are not inclusive jets, so that in
dealing with empty area it has to calculate the number of empty jets based on
the empty area and the the observed <area> of jets rather than a surmised area  

Note that for small effective radii, this can cause problems because the harder
jets get an area >> <ghost-jet-area> and so the estimate comes out all wrong. In
these situations it is highly advisable to use an area with explicit ghosts,
since then the \"empty\" jets are actually visible.  

The selector passed as an argument has to have a finite area and apply jet-by-
jet (see the BackgroundEstimator and Subtractor tools for more generic usages)  
";

%feature("docstring") fastjet::ClusterSequenceAreaBase::get_median_rho_and_sigma "

same as the full version of get_median_rho_and_error, but without access to the
mean_area  

The selector passed as an argument has to have a finite area and apply jet-by-
jet (see the BackgroundEstimator and Subtractor tools for more generic usages)  
";

%feature("docstring") fastjet::ClusterSequenceAreaBase::has_explicit_ghosts "

returns true if ghosts are explicitly included within jets for this
ClusterSequence;  

Derived classes that do include explicit ghosts should provide an alternative
version of this routine and set it properly.  
";

%feature("docstring") fastjet::ClusterSequenceAreaBase::area_error "

return the error (uncertainty) associated with the determination of the area of
this jet; this base class returns 0.  
";

%feature("docstring") fastjet::ClusterSequenceAreaBase::__attribute__ "

the median of (pt/area) for jets contained within the selector range, making use
also of the info on n_empty_jets  

The selector passed as an argument has to have a finite area and apply jet-by-
jet (see the BackgroundEstimator and Subtractor tools for more generic usages)  
";

%feature("docstring") fastjet::ClusterSequenceAreaBase::__attribute__ "

the median of (pt/area_4vector) for jets contained within the selector range,
making use also of the info on n_empty_jets  

The selector passed as an argument has to have a finite area and apply jet-by-
jet  
";

%feature("docstring") fastjet::ClusterSequenceAreaBase::__attribute__ "

the function that does the work for median_pt_per_unit_area and
median_pt_per_unit_area_4vector:  

*   something_is_area_4vect = false -> use plain area  
*   something_is_area_4vect = true -> use 4-vector area  
";

%feature("docstring") fastjet::ClusterSequenceAreaBase::__attribute__ "

return a vector of all subtracted jets, using area_4vector, given rho.  

Only inclusive_jets above ptmin are subtracted and returned. the ordering is the
same as that of sorted_by_pt(cs.inclusive_jets()), i.e. not necessarily ordered
in pt once subtracted return a vector of subtracted jets, using area_4vector.
Only inclusive_jets above ptmin are subtracted and returned. the ordering is the
same as that of sorted_by_pt(cs.inclusive_jets()), i.e. not necessarily ordered
in pt once subtracted  

The selector passed as an argument has to have a finite area and apply jet-by-
jet (see the BackgroundEstimator and Subtractor tools for more generic usages)
return a subtracted jet, using area_4vector, given rho  
";

%feature("docstring") fastjet::ClusterSequenceAreaBase::__attribute__ "

return a subtracted jet, using area_4vector; note that this is potentially
inefficient if repeatedly used for many different jets, because rho will be
recalculated each time around.  

The selector passed as an argument has to have a finite area and apply jet-by-
jet (see the BackgroundEstimator and Subtractor tools for more generic usages)  
";

%feature("docstring") fastjet::ClusterSequenceAreaBase::__attribute__ "

return the subtracted pt, given rho  
";

%feature("docstring") fastjet::ClusterSequenceAreaBase::__attribute__ "

return the subtracted pt; note that this is potentially inefficient if
repeatedly used for many different jets, because rho will be recalculated each
time around.  

The selector passed as an argument has to have a finite area and apply jet-by-
jet (see the BackgroundEstimator and Subtractor tools for more generic usages)  
";

%feature("docstring") fastjet::ClusterSequenceAreaBase::~ClusterSequenceAreaBase "

destructor  
";

%feature("docstring") fastjet::ClusterSequenceAreaBase::n_empty_jets "

return something similar to the number of pure ghost jets in the given
selector's range in an active area case.  

For the local implementation we return empty_area/(0.55 pi R^2), based on
measured properties of ghost jets with kt and cam (cf arXiv:0802.1188).  

Note that the number returned is a double.  

The selector passed as an argument has to have a finite area and apply jet-by-
jet (see the BackgroundEstimator and Subtractor tools for more generic usages)  
";

// File: classfastjet_1_1ClusterSequencePassiveArea.xml


%feature("docstring") fastjet::ClusterSequencePassiveArea "

Like ClusterSequence with computation of the passive jet area.  

Class that behaves essentially like ClusterSequence except that it also provides
access to the area of a jet (which will be a random quantity... Figure out what
to do about seeds later...)  

This class should not be used directly. Rather use ClusterSequenceArea with the
appropriate AreaDefinition  

C++ includes: fastjet/ClusterSequencePassiveArea.hh
";

%feature("docstring") fastjet::ClusterSequencePassiveArea::ClusterSequencePassiveArea "

constructor based on JetDefinition and PassiveAreaSpec  
";

%feature("docstring") fastjet::ClusterSequencePassiveArea::empty_area "

return an empty area that's appropriate to the passive area determination
carried out  
";

// File: classfastjet_1_1ClusterSequenceStructure.xml


%feature("docstring") fastjet::ClusterSequenceStructure "

Contains any information related to the clustering that should be directly
accessible to PseudoJet.  

By default, this class implements basic access to the ClusterSequence related to
a PseudoJet (like its constituents or its area). But it can be overloaded in
order e.g. to give access to the jet substructure.  

C++ includes: fastjet/ClusterSequenceStructure.hh
";

/*
 Direct access to the associated ClusterSequence object. 
*/

/*
Get access to the associated ClusterSequence (if any)  

*/

/*
 Methods for access to information about jet structure 
*/

/*
These allow access to jet constituents, and other jet subtructure information.  

They only work if the jet is associated with a ClusterSequence.  

*/

%feature("docstring") fastjet::ClusterSequenceStructure::area "

return the jet (scalar) area.  

throws an Error if there is no support for area in the parent CS  
";

%feature("docstring") fastjet::ClusterSequenceStructure::has_area "

check if it has a defined area  
";

%feature("docstring") fastjet::ClusterSequenceStructure::description "

description  
";

%feature("docstring") fastjet::ClusterSequenceStructure::constituents "

retrieve the constituents.  

an Error is thrown if this PseudoJet has no currently valid associated
ClusterSequence  
";

%feature("docstring") fastjet::ClusterSequenceStructure::has_child "

check if it has been recombined with another PseudoJet in which case, return its
child through the argument.  

Otherwise, 'child' is set to 0.  

an Error is thrown if this PseudoJet has no currently valid associated
ClusterSequence  
";

%feature("docstring") fastjet::ClusterSequenceStructure::has_exclusive_subjets "

return true if the structure supports exclusive_subjets.  

an Error is thrown if this PseudoJet has no currently valid associated
ClusterSequence  
";

%feature("docstring") fastjet::ClusterSequenceStructure::n_exclusive_subjets "

return the size of exclusive_subjets(...); still n ln n with same coefficient,
but marginally more efficient than manually taking exclusive_subjets.size()  

an Error is thrown if this PseudoJet has no currently valid associated
ClusterSequence  
";

%feature("docstring") fastjet::ClusterSequenceStructure::is_pure_ghost "

true if this jet is made exclusively of ghosts.  

throws an Error if there is no support for area in the parent CS  
";

%feature("docstring") fastjet::ClusterSequenceStructure::has_parents "

check if it is the product of a recombination, in which case return the 2
parents through the 'parent1' and 'parent2' arguments.  

Otherwise, set these to 0.  

an Error is thrown if this PseudoJet has no currently valid associated
ClusterSequence  
";

%feature("docstring") fastjet::ClusterSequenceStructure::has_constituents "

return true if the structure supports constituents.  

an Error is thrown if this PseudoJet has no currently valid associated
ClusterSequence  
";

%feature("docstring") fastjet::ClusterSequenceStructure::object_in_jet "

check if the reference PseudoJet is contained in the second one passed as
argument.  

an Error is thrown if this PseudoJet has no currently valid associated
ClusterSequence  

false is returned if the 2 PseudoJet do not belong the same ClusterSequence  
";

%feature("docstring") fastjet::ClusterSequenceStructure::validated_cs "

if the jet has a valid associated cluster sequence then return a pointer to it;
otherwise throw an error  
";

%feature("docstring") fastjet::ClusterSequenceStructure::area_error "

return the error (uncertainty) associated with the determination of the area of
this jet.  

throws an Error if there is no support for area in the parent CS  
";

%feature("docstring") fastjet::ClusterSequenceStructure::exclusive_subdmerge "

return the dij that was present in the merging nsub+1 -> nsub subjets inside
this jet.  

an Error is thrown if this PseudoJet has no currently valid associated
ClusterSequence  
";

%feature("docstring") fastjet::ClusterSequenceStructure::exclusive_subdmerge_max "

return the maximum dij that occurred in the whole event at the stage that the
nsub+1 -> nsub merge of subjets occurred inside this jet.  

an Error is thrown if this PseudoJet has no currently valid associated
ClusterSequence  
";

%feature("docstring") fastjet::ClusterSequenceStructure::~ClusterSequenceStructure "

default (virtual) dtor  
";

%feature("docstring") fastjet::ClusterSequenceStructure::associated_cluster_sequence "

get a (const) pointer to the parent ClusterSequence (NULL if inexistent)  
";

%feature("docstring") fastjet::ClusterSequenceStructure::area_4vector "

return the jet 4-vector area.  

throws an Error if there is no support for area in the parent CS  
";

%feature("docstring") fastjet::ClusterSequenceStructure::exclusive_subjets_up_to "

return the list of subjets obtained by unclustering the supplied jet down to
nsub subjets (or all constituents if there are fewer than nsub).  

requires nsub ln nsub time  

an Error is thrown if this PseudoJet has no currently valid associated
ClusterSequence  
";

%feature("docstring") fastjet::ClusterSequenceStructure::set_associated_cs "

set the associated csw  
";

%feature("docstring") fastjet::ClusterSequenceStructure::has_pieces "

by convention, a jet associated with a ClusterSequence will have its parents as
pieces  
";

%feature("docstring") fastjet::ClusterSequenceStructure::validated_csab "

if the jet has valid area information then return a pointer to the associated
ClusterSequenceAreaBase object; otherwise throw an error  
";

%feature("docstring") fastjet::ClusterSequenceStructure::has_partner "

check if it has been recombined with another PseudoJet in which case, return its
partner through the argument.  

Otherwise, 'partner' is set to 0.  

an Error is thrown if this PseudoJet has no currently valid associated
ClusterSequence  
";

%feature("docstring") fastjet::ClusterSequenceStructure::has_associated_cluster_sequence "

returns true if there is an associated ClusterSequence  
";

%feature("docstring") fastjet::ClusterSequenceStructure::ClusterSequenceStructure "

default ctor  
";

%feature("docstring") fastjet::ClusterSequenceStructure::ClusterSequenceStructure "

ctor with initialisation to a given ClusterSequence  

In principle, this is reserved for initialisation by the parent ClusterSequence  
";

%feature("docstring") fastjet::ClusterSequenceStructure::exclusive_subjets "

return a vector of all subjets of the current jet (in the sense of the exclusive
algorithm) that would be obtained when running the algorithm with the given
dcut.  

Time taken is O(m ln m), where m is the number of subjets that are found. If m
gets to be of order of the total number of constituents in the jet, this could
be substantially slower than just getting that list of constituents.  

an Error is thrown if this PseudoJet has no currently valid associated
ClusterSequence  
";

%feature("docstring") fastjet::ClusterSequenceStructure::has_valid_cluster_sequence "

returns true if there is a valid associated ClusterSequence  
";

%feature("docstring") fastjet::ClusterSequenceStructure::pieces "

by convention, a jet associated with a ClusterSequence will have its parents as
pieces  

if it has no parents, then there will only be a single piece: itself  

Note that to answer that question, we need to access the cluster sequence. If
the cluster sequence has gone out of scope, an error will be thrown  
";

// File: classfastjet_1_1ClusterSequenceVoronoiArea.xml


%feature("docstring") fastjet::ClusterSequenceVoronoiArea "

Like ClusterSequence with computation of the Voronoi jet area.  

Handle the computation of Voronoi jet area.  

This class should not be used directly. Rather use ClusterSequenceArea with the
appropriate AreaDefinition  

C++ includes: fastjet/ClusterSequenceVoronoiArea.hh
";

%feature("docstring") fastjet::ClusterSequenceVoronoiArea::ClusterSequenceVoronoiArea "

template ctor  

template constructor need to be specified in the header!  

Parameters
----------
* `pseudojet` :  
    list of jets (template type)  
* `jet_def` :  
    jet definition  
* `effective_Rfact` :  
    effective radius  
* `writeout_combinations` :  
    ??????  
";

%feature("docstring") fastjet::ClusterSequenceVoronoiArea::area_error "

return the error of the area associated with the given jet (0 by definition for
a voronoi area)  
";

%feature("docstring") fastjet::ClusterSequenceVoronoiArea::~ClusterSequenceVoronoiArea "

default dtor  
";

%feature("docstring") fastjet::ClusterSequenceVoronoiArea::area_4vector "

return a 4-vector area associated with the given jet -- strictly this is not the
exact 4-vector area, but rather an approximation made of sums of centres of all
Voronoi cells in jet, each contributing with a normalisation equal to the area
of the cell  
";

%feature("docstring") fastjet::ClusterSequenceVoronoiArea::area "

return the area associated with the given jet  
";

// File: classCmdLine.xml


%feature("docstring") CmdLine "
";

%feature("docstring") CmdLine::all_options_used "

return true if all options have been asked for at some point or other  
";

%feature("docstring") CmdLine::value "

returns the value of the argument converted to type T  
";

%feature("docstring") CmdLine::value "
";

%feature("docstring") CmdLine::value "

for the string case, just copy the string...  
";

%feature("docstring") CmdLine::CmdLine "
";

%feature("docstring") CmdLine::CmdLine "

initialise a CmdLine from a C-style array of command-line arguments  
";

%feature("docstring") CmdLine::CmdLine "

initialise a CmdLine from a C++ vector of arguments  

constructor from a vector of strings, one argument per string  
";

%feature("docstring") CmdLine::present_and_set "

true if the option is present and corresponds to a value  
";

%feature("docstring") CmdLine::int_val "

return the integer value corresponding to the given option  
";

%feature("docstring") CmdLine::int_val "

return the integer value corresponding to the given option or default if option
is absent  
";

%feature("docstring") CmdLine::string_val "

return the string value corresponding to the given option  
";

%feature("docstring") CmdLine::string_val "

return the string value corresponding to the given option or default if option
is absent  
";

%feature("docstring") CmdLine::arguments "

return a reference to the vector of command-line arguments (0 is command).  
";

%feature("docstring") CmdLine::double_val "

return the double value corresponding to the given option  
";

%feature("docstring") CmdLine::double_val "

return the double value corresponding to the given option or default if option
is absent  
";

%feature("docstring") CmdLine::present "

true if the option is present  
";

%feature("docstring") CmdLine::command_line "

return the full command line  
";

// File: classfastjet_1_1CMSIterativeConePlugin.xml


%feature("docstring") fastjet::CMSIterativeConePlugin "

Implementation of the CMS Iterative Cone (plugin for fastjet v2.4 upwards)  

C++ includes: fastjet/CMSIterativeConePlugin.hh
";

%feature("docstring") fastjet::CMSIterativeConePlugin::CMSIterativeConePlugin "

Main constructor for the CMSIterativeCone Plugin class.  

The arguments are ConeRadius the radius of the cone SeedThreshold a threshold
for the seeds to iterate from  

NOTE: to be more coherent with all other fastjet plugins, we've put the radius
before the seed threshold. CMS does the opposite. In this way, we also put a
default value of 0 for the seed threshold.  
";

%feature("docstring") fastjet::CMSIterativeConePlugin::CMSIterativeConePlugin "

copy constructor  
";

%feature("docstring") fastjet::CMSIterativeConePlugin::seed_threshold "

get the seed threshold  
";

%feature("docstring") fastjet::CMSIterativeConePlugin::run_clustering "

given a ClusterSequence that has been filled up with initial particles, the
following function should fill up the rest of the ClusterSequence, using the
following member functions of ClusterSequence:  

*   plugin_do_ij_recombination(...)  
*   plugin_do_iB_recombination(...)  
";

%feature("docstring") fastjet::CMSIterativeConePlugin::R "

the plugin mechanism's standard way of accessing the jet radius here we return
the R of the last alg in the list  
";

%feature("docstring") fastjet::CMSIterativeConePlugin::description "

return a textual description of the jet-definition implemented in this plugin  
";

// File: classfastjet_1_1CompositeJetStructure.xml


%feature("docstring") fastjet::CompositeJetStructure "

The structure for a jet made of pieces.  

This stores the vector of the pieces that make the jet and provide the methods
to access them  

C++ includes: fastjet/CompositeJetStructure.hh
";

%feature("docstring") fastjet::CompositeJetStructure::has_pieces "

true if it has pieces (always the case)  
";

%feature("docstring") fastjet::CompositeJetStructure::description "

description  
";

%feature("docstring") fastjet::CompositeJetStructure::area_error "

return the error (uncertainty) associated with the determination of the area of
this jet.  

Be conservative: return the sum of the errors  
";

%feature("docstring") fastjet::CompositeJetStructure::pieces "

returns the pieces  
";

%feature("docstring") fastjet::CompositeJetStructure::constituents "

return the constituents (i.e.  

the union of the constituents of each piece)  

If any of the pieces has no constituent, the piece itself is considered as a
constituent Note that as a consequence, a composite jet with no pieces will have
an empty vector as constituents  
";

%feature("docstring") fastjet::CompositeJetStructure::has_constituents "

true unless the jet has no pieces (see also the description of constituents()
below)  
";

%feature("docstring") fastjet::CompositeJetStructure::CompositeJetStructure "

default ctor  
";

%feature("docstring") fastjet::CompositeJetStructure::CompositeJetStructure "

ctor with initialisation  
";

%feature("docstring") fastjet::CompositeJetStructure::has_area "

check if it has a well-defined area  
";

%feature("docstring") fastjet::CompositeJetStructure::area "

return the jet (scalar) area.  
";

%feature("docstring") fastjet::CompositeJetStructure::~CompositeJetStructure "

default dtor  
";

%feature("docstring") fastjet::CompositeJetStructure::area_4vector "

return the jet 4-vector area.  
";

%feature("docstring") fastjet::CompositeJetStructure::discard_area "

disable the area of the composite jet  

this can be used e.g. to discard the area of a composite jet made of pieces with
non-explicit-ghost area since the area may by erroneous in that case  
";

%feature("docstring") fastjet::CompositeJetStructure::is_pure_ghost "

true if this jet is made exclusively of ghosts.  

In this case, it will be true if all pieces are pure ghost  
";

// File: classfastjet_1_1d0runi_1_1ConeClusterAlgo.xml


%feature("docstring") fastjet::d0runi::ConeClusterAlgo "
";

%feature("docstring") fastjet::d0runi::ConeClusterAlgo::makeClusters "
";

%feature("docstring") fastjet::d0runi::ConeClusterAlgo::~ConeClusterAlgo "
";

%feature("docstring") fastjet::d0runi::ConeClusterAlgo::print "
";

%feature("docstring") fastjet::d0runi::ConeClusterAlgo::ConeClusterAlgo "
";

%feature("docstring") fastjet::d0runi::ConeClusterAlgo::ConeClusterAlgo "
";

%feature("docstring") fastjet::d0runi::ConeClusterAlgo::ConeClusterAlgo "
";

// File: classfastjet_1_1d0_1_1D0RunIIconeJets__CONEJETINFO_1_1ConeJetInfo.xml


%feature("docstring") fastjet::d0::D0RunIIconeJets_CONEJETINFO::ConeJetInfo "
";

%feature("docstring") fastjet::d0::D0RunIIconeJets_CONEJETINFO::ConeJetInfo::splitted "
";

%feature("docstring") fastjet::d0::D0RunIIconeJets_CONEJETINFO::ConeJetInfo::nbMerge "
";

%feature("docstring") fastjet::d0::D0RunIIconeJets_CONEJETINFO::ConeJetInfo::ConeJetInfo "
";

%feature("docstring") fastjet::d0::D0RunIIconeJets_CONEJETINFO::ConeJetInfo::ConeJetInfo "
";

%feature("docstring") fastjet::d0::D0RunIIconeJets_CONEJETINFO::ConeJetInfo::ConeJetInfo "
";

%feature("docstring") fastjet::d0::D0RunIIconeJets_CONEJETINFO::ConeJetInfo::merged "
";

%feature("docstring") fastjet::d0::D0RunIIconeJets_CONEJETINFO::ConeJetInfo::seedET "
";

%feature("docstring") fastjet::d0::D0RunIIconeJets_CONEJETINFO::ConeJetInfo::~ConeJetInfo "
";

%feature("docstring") fastjet::d0::D0RunIIconeJets_CONEJETINFO::ConeJetInfo::nbSplit "
";

%feature("docstring") fastjet::d0::D0RunIIconeJets_CONEJETINFO::ConeJetInfo::initialET "
";

%feature("docstring") fastjet::d0::D0RunIIconeJets_CONEJETINFO::ConeJetInfo::initialET "
";

%feature("docstring") fastjet::d0::D0RunIIconeJets_CONEJETINFO::ConeJetInfo::SplitMergeWord "
";

// File: classfastjet_1_1d0_1_1ConeSplitMerge.xml


%feature("docstring") fastjet::d0::ConeSplitMerge "
";

%feature("docstring") fastjet::d0::ConeSplitMerge::~ConeSplitMerge "
";

%feature("docstring") fastjet::d0::ConeSplitMerge::ConeSplitMerge "
";

%feature("docstring") fastjet::d0::ConeSplitMerge::ConeSplitMerge "
";

%feature("docstring") fastjet::d0::ConeSplitMerge::ConeSplitMerge "
";

%feature("docstring") fastjet::d0::ConeSplitMerge::split_merge "
";

// File: classfastjet_1_1SearchTree_1_1const__circulator.xml


%feature("docstring") fastjet::SearchTree::const_circulator "
";

%feature("docstring") fastjet::SearchTree::const_circulator::const_circulator "
";

%feature("docstring") fastjet::SearchTree::const_circulator::const_circulator "
";

%feature("docstring") fastjet::SearchTree::const_circulator::const_circulator "
";

%feature("docstring") fastjet::SearchTree::const_circulator::next "

return a circulator referring to the next node  
";

%feature("docstring") fastjet::SearchTree::const_circulator::previous "

return a circulator referring to the previous node  
";

// File: classfastjet_1_1Coord2D.xml


%feature("docstring") fastjet::Coord2D "
";

%feature("docstring") fastjet::Coord2D::Coord2D "
";

%feature("docstring") fastjet::Coord2D::Coord2D "
";

%feature("docstring") fastjet::Coord2D::distance2 "

return the squared distance between two coordinates  
";

%feature("docstring") fastjet::Coord2D::distance2 "

return the squared distance between two coordinates  
";

// File: classfastjet_1_1D0RunIBaseConePlugin.xml


%feature("docstring") fastjet::D0RunIBaseConePlugin "

D0RunIBaseConePlugin is base class for a plugin for FastJet (v3.0 or later) that
provides an interface to the D0 version of Run-I cone algorithm.  

Note that this base class is purely virtual and thus needs to be overloaded. In
practice this means that you should use one of D0RunIConePlugin or
D0RunIpre96ConePlugin.  

The D0 code has been obtained from Lars Sonnenschein's web-space
http://www-d0.fnal.gov/~sonne/D0RunIcone.tgz  

The version of the D0 Run I code distributed here has been modified by the
FastJet authors, so as to provide access to the contents of the jets (as is
necessary for the plugin). This does not modify the results of the clustering.  

C++ includes: fastjet/D0RunIBaseConePlugin.hh
";

%feature("docstring") fastjet::D0RunIBaseConePlugin::Kill_Far_Clusters "
";

%feature("docstring") fastjet::D0RunIBaseConePlugin::Jet_Et_Min_On_Iter "
";

%feature("docstring") fastjet::D0RunIBaseConePlugin::TWOrad "
";

%feature("docstring") fastjet::D0RunIBaseConePlugin::overlap_threshold "

access the split_ratio() also by the name overlap_threshold()  
";

%feature("docstring") fastjet::D0RunIBaseConePlugin::D0RunIBaseConePlugin "

A D0RunIConePlugin constructor which sets the \"free\" parameters of the
algorithm:  

Parameters
----------
* `CONErad` :  
    is the cone radius  
* `JETmne` :  
    is a minimum ET requirement on every iteration (jet dropped if Et < JETmne *
    Et_min_ratio ). The value that has been used by D0 for JETmne: 8 GeV (and
    Et_min_ratio is 0.5)  
* `SPlifr` :  
    is the shared Et fraction splitting threshold, and a value of 0.5 was
    usually used by D0  

The remaining parameters of the algorithm are not to be modified if the
algorithm is to correspond to the one actually used by D0.  
";

%feature("docstring") fastjet::D0RunIBaseConePlugin::run_clustering "

given a ClusterSequence that has been filled up with initial particles, the
following function should fill up the rest of the ClusterSequence, using the
following member functions of ClusterSequence:  

*   plugin_do_ij_recombination(...)  
*   plugin_do_iB_recombination(...)  
";

%feature("docstring") fastjet::D0RunIBaseConePlugin::SPLifr "
";

%feature("docstring") fastjet::D0RunIBaseConePlugin::Et_Min_Ratio "
";

%feature("docstring") fastjet::D0RunIBaseConePlugin::CONErad "
";

%feature("docstring") fastjet::D0RunIBaseConePlugin::description "

return a textual description of the jet-definition implemented in this plugin  
";

%feature("docstring") fastjet::D0RunIBaseConePlugin::Thresh_Diff_Et "
";

%feature("docstring") fastjet::D0RunIBaseConePlugin::Far_Ratio "
";

%feature("docstring") fastjet::D0RunIBaseConePlugin::Eitem_Negdrop "
";

%feature("docstring") fastjet::D0RunIBaseConePlugin::Increase_Delta_R "
";

%feature("docstring") fastjet::D0RunIBaseConePlugin::JETmne "
";

%feature("docstring") fastjet::D0RunIBaseConePlugin::R "

the plugin mechanism's standard way of accessing the jet radius  
";

%feature("docstring") fastjet::D0RunIBaseConePlugin::D0_Angle "
";

// File: classfastjet_1_1D0RunIConePlugin.xml


%feature("docstring") fastjet::D0RunIConePlugin "

A plugin for FastJet (v3.0 or later) that provides an interface to the D0
version of Run-I cone algorithm.  

The D0 code has been obtained from Lars Sonnenschein's web-space
http://www-d0.fnal.gov/~sonne/D0RunIcone.tgz  

The version of the D0 Run I code distributed here has been modified by the
FastJet authors, so as to provide access to the contents of the jets (as is
necessary for the plugin). This does not modify the results of the clustering.  

The difference between this algorithm and the post-1996 version relates to the
way the final jet momenta are calculated. Details are to be found in FERMILAB-
PUB-97-242-E.  

C++ includes: fastjet/D0RunIConePlugin.hh
";

%feature("docstring") fastjet::D0RunIConePlugin::run_clustering "

given a ClusterSequence that has been filled up with initial particles, the
following function should fill up the rest of the ClusterSequence, using the
following member functions of ClusterSequence:  

*   plugin_do_ij_recombination(...)  
*   plugin_do_iB_recombination(...)  
";

%feature("docstring") fastjet::D0RunIConePlugin::D0RunIConePlugin "

The D0RunIConePlugin constructor, which sets the \"free\" parameters of the
algorithm:  

Parameters
----------
* `CONErad` :  
    is the cone radius  
* `JETmne` :  
    is a minimum ET requirement on every iteration (jet dropped if Et < JETmne *
    Et_min_ratio ). The value that has been used by D0 for JETmne: 8 GeV (and
    Et_min_ratio is 0.5)  
* `SPlifr` :  
    is the shared Et fraction splitting threshold, and a value of 0.5 was
    usually used by D0  

The remaining parameters of the algorithm are not to be modified if the
algorithm is to correspond to the one actually used by D0.  
";

%feature("docstring") fastjet::D0RunIConePlugin::description "

return a textual description of the jet-definition implemented in this plugin  
";

// File: classfastjet_1_1D0RunIIConePlugin.xml


%feature("docstring") fastjet::D0RunIIConePlugin "

Implementation of the D0 Run II Cone (plugin for fastjet v2.1 upwards)  

D0RunIIConePlugin is a plugin for fastjet (v2.1 upwards) that provides an
interface to the D0 version of Run-II iterative cone algorithm with midpoint
seeds (also known as the Iterative Legacy Cone Algorithm, ILCA).  

The D0 code has been taken from Lars Sonnenschein's web-space
http://www-d0.fnal.gov/~sonne/D0RunIIcone.tgz  

The version of the D0 Run II code distributed here has been modified by the
FastJet authors, so as to provide access to the contents of the jets (as is
necessary for the plugin). This does not modify the results of the clustering.  

C++ includes: fastjet/D0RunIIConePlugin.hh
";

%feature("docstring") fastjet::D0RunIIConePlugin::kill_duplicate "
";

%feature("docstring") fastjet::D0RunIIConePlugin::merge_max "
";

%feature("docstring") fastjet::D0RunIIConePlugin::overlap_threshold "

access the split_ratio() also by the name overlap_threshold()  
";

%feature("docstring") fastjet::D0RunIIConePlugin::cone_radius "
";

%feature("docstring") fastjet::D0RunIIConePlugin::duplicate_dPT "
";

%feature("docstring") fastjet::D0RunIIConePlugin::run_clustering "

given a ClusterSequence that has been filled up with initial particles, the
following function should fill up the rest of the ClusterSequence, using the
following member functions of ClusterSequence:  

*   plugin_do_ij_recombination(...)  
*   plugin_do_iB_recombination(...)  
";

%feature("docstring") fastjet::D0RunIIConePlugin::duplicate_dR "
";

%feature("docstring") fastjet::D0RunIIConePlugin::search_factor "
";

%feature("docstring") fastjet::D0RunIIConePlugin::D0RunIIConePlugin "

A D0RunIIConePlugin constructor which sets the \"free\" parameters of the
algorithm:  

*   the cone_radius has the usual meaning  
*   the min_jet_Et causes cones to be discarded at if at any iteration they have
    pt < Et_min_ratio * min_jet_Et. Two values have been used by D0 for
    min_jet_Et: 8 GeV in earlier Run II publicatinos, 6 GeV in later
    publications  
*   split_ratio is equivalent to the overlap threshold during the split/merge
    step. Default: 0.5.  

The remaining parameters of the algorithm are not to be modified if the
algorithm is to correspond to the one actually used by D0.  
";

%feature("docstring") fastjet::D0RunIIConePlugin::pT_min_nomerge "
";

%feature("docstring") fastjet::D0RunIIConePlugin::pT_min_second_protojet "
";

%feature("docstring") fastjet::D0RunIIConePlugin::split_ratio "
";

%feature("docstring") fastjet::D0RunIIConePlugin::far_ratio "
";

%feature("docstring") fastjet::D0RunIIConePlugin::description "

return a textual description of the jet-definition implemented in this plugin  
";

%feature("docstring") fastjet::D0RunIIConePlugin::pT_min_leading_protojet "
";

%feature("docstring") fastjet::D0RunIIConePlugin::min_jet_Et "
";

%feature("docstring") fastjet::D0RunIIConePlugin::R "

the plugin mechanism's standard way of accessing the jet radius  
";

%feature("docstring") fastjet::D0RunIIConePlugin::Et_min_ratio "
";

// File: classfastjet_1_1D0RunIpre96ConePlugin.xml


%feature("docstring") fastjet::D0RunIpre96ConePlugin "

A plugin for FastJet (v3.0 or later) that provides an interface to the pre 1996
D0 version of Run-I cone algorithm.  

The D0 code has been obtained from Lars Sonnenschein's web-space
http://www-d0.fnal.gov/~sonne/D0RunIcone.tgz  

The version of the D0 Run I code distributed here has been modified by the
FastJet authors, so as to provide access to the contents of the jets (as is
necessary for the plugin). This does not modify the results of the clustering.  

The difference between this algorithm and the post-1996 version relates to the
way the final jet momenta are calculated. Details are to be found in FERMILAB-
PUB-97-242-E.  

C++ includes: fastjet/D0RunIpre96ConePlugin.hh
";

%feature("docstring") fastjet::D0RunIpre96ConePlugin::run_clustering "

given a ClusterSequence that has been filled up with initial particles, the
following function should fill up the rest of the ClusterSequence, using the
following member functions of ClusterSequence:  

*   plugin_do_ij_recombination(...)  
*   plugin_do_iB_recombination(...)  
";

%feature("docstring") fastjet::D0RunIpre96ConePlugin::D0RunIpre96ConePlugin "

The D0RunIpre96ConePlugin constructor, which sets the \"free\" parameters of the
algorithm:  

Parameters
----------
* `CONErad` :  
    is the cone radius  
* `JETmne` :  
    is a minimum ET requirement on every iteration (jet dropped if Et < JETmne *
    Et_min_ratio ). The value that has been used by D0 for JETmne: 8 GeV (and
    Et_min_ratio is 0.5)  
* `SPlifr` :  
    is the shared Et fraction splitting threshold, and a value of 0.5 was
    usually used by D0  

The remaining parameters of the algorithm are not to be modified if the
algorithm is to correspond to the one actually used by D0.  
";

%feature("docstring") fastjet::D0RunIpre96ConePlugin::description "

return a textual description of the jet-definition implemented in this plugin  
";

// File: classfastjet_1_1JetDefinition_1_1DefaultRecombiner.xml


%feature("docstring") fastjet::JetDefinition::DefaultRecombiner "

A class that will provide the recombination scheme facilities and/or allow a
user to extend these facilities.  

This class is derived from the (abstract) class Recombiner. It simply \"sums\"
PseudoJets using a specified recombination scheme (E-scheme by default)  

C++ includes: fastjet/JetDefinition.hh
";

%feature("docstring") fastjet::JetDefinition::DefaultRecombiner::recombine "

recombine pa and pb and put result into pab  

keep y,phi and m from the hardest, sum pt  

keep 3-momentum direction and mass from the hardest, sum modp  

If the hardest particle is at rest, the sum remains at rest (the energy of the
sum is therefore the mass of pa)  
";

%feature("docstring") fastjet::JetDefinition::DefaultRecombiner::DefaultRecombiner "
";

%feature("docstring") fastjet::JetDefinition::DefaultRecombiner::scheme "

return the index of the recombination scheme  
";

%feature("docstring") fastjet::JetDefinition::DefaultRecombiner::description "

return a textual description of the recombination scheme implemented here  
";

%feature("docstring") fastjet::JetDefinition::DefaultRecombiner::preprocess "

routine called to preprocess each input jet (to make all input jets compatible
with the scheme requirements (e.g.  

massless).  
";

// File: classfastjet_1_1NNFJN2Tiled_1_1diJ__plus__link.xml

// File: classfastjet_1_1Dnn2piCylinder.xml


%feature("docstring") fastjet::Dnn2piCylinder "
";

%feature("docstring") fastjet::Dnn2piCylinder::Valid "

Returns true iff the given index corresponds to a point that exists in the DNN
structure (meaning that it has been added, and not removed in the meantime)  
";

%feature("docstring") fastjet::Dnn2piCylinder::RemoveAndAddPoints "

insertion and removal of points  
";

%feature("docstring") fastjet::Dnn2piCylinder::~Dnn2piCylinder "
";

%feature("docstring") fastjet::Dnn2piCylinder::Dnn2piCylinder "

empty initaliser  
";

%feature("docstring") fastjet::Dnn2piCylinder::Dnn2piCylinder "

Initialiser from a set of points on an Eta-Phi plane, where eta can have an
arbitrary ranges and phi must be in range 0 <= phi < 2pi;.  

NB: this class is more efficient than the plain Dnn4piCylinder class, but can
give wrong answers when the nearest neighbour is further away than 2pi (in this
case a point's nearest neighbour becomes itself, because it is considered to be
a distance 2pi away). For the kt-algorithm (e.g.) this is actually not a problem
(the distance need only be accurate when it is less than R, assuming R<2pi [not
necessarily always the case as of 2010-11-19, when we've removed the requirement
R<pi/2 in the JetDefinition constructor]), so we can tell the routine to ignore
this problem -- alternatively the routine will crash if it detects it occurring
(only when finding the nearest neighbour index, not its distance).  
";

%feature("docstring") fastjet::Dnn2piCylinder::NearestNeighbourDistance "

Returns the distance to the nearest neighbour of point labelled by index ii
(assumes ii is valid)  
";

%feature("docstring") fastjet::Dnn2piCylinder::NearestNeighbourIndex "

Returns the index of the nearest neighbour of point labelled by ii (assumes ii
is valid)  

Note: one of the difficulties of the 0--2pi mapping is that a point may have its
mirror copy as its own nearest neighbour (if no other point is within a distance
of 2pi).  

This does not matter for the kt_algorithm with reasonable values of radius, but
might matter for a general use of this algorithm -- depending on whether or not
the user has initialised the class with instructions to ignore this problem the
program will detect and ignore it, or crash.  
";

// File: classfastjet_1_1Dnn3piCylinder.xml


%feature("docstring") fastjet::Dnn3piCylinder "
";

%feature("docstring") fastjet::Dnn3piCylinder::NearestNeighbourDistance "

Returns the distance to the nearest neighbour of point labelled by index ii
(assumes ii is valid)  
";

%feature("docstring") fastjet::Dnn3piCylinder::Valid "

Returns true iff the given index corresponds to a point that exists in the DNN
structure (meaning that it has been added, and not removed in the meantime)  
";

%feature("docstring") fastjet::Dnn3piCylinder::~Dnn3piCylinder "
";

%feature("docstring") fastjet::Dnn3piCylinder::RemoveAndAddPoints "

insertion and removal of points  
";

%feature("docstring") fastjet::Dnn3piCylinder::Dnn3piCylinder "

empty initaliser  
";

%feature("docstring") fastjet::Dnn3piCylinder::Dnn3piCylinder "

Initialiser from a set of points on an Eta-Phi plane, where eta can have an
arbitrary ranges and phi must be in range 0 <= phi < 2pi;.  

NB: this class is more efficient than the plain Dnn4piCylinder class, but can
give wrong answers when the nearest neighbour is further away than 2pi (in this
case a point's nearest neighbour becomes itself, because it is considered to be
a distance 2pi away). For the kt-algorithm (e.g.) this is actually not a problem
(the distance need only be accurate when it is less than R), so we can tell the
routine to ignore this problem -- alternatively the routine will crash if it
detects it occurring (only when finding the nearest neighbour index, not its
distance).  
";

%feature("docstring") fastjet::Dnn3piCylinder::NearestNeighbourIndex "

Returns the index of the nearest neighbour of point labelled by ii (assumes ii
is valid)  

Note: one of the difficulties of the 0--3pi mapping is that a point may have its
mirror copy as its own nearest neighbour (if no other point is within a distance
of 2pi).  

This does not matter for the kt_algorithm with reasonable values of radius, but
might matter for a general use of this algorithm -- depending on whether or not
the user has initialised the class with instructions to ignore this problem the
program will detect and ignore it, or crash.  
";

// File: classfastjet_1_1Dnn4piCylinder.xml


%feature("docstring") fastjet::Dnn4piCylinder "
";

%feature("docstring") fastjet::Dnn4piCylinder::Valid "

Returns true iff the given index corresponds to a point that exists in the DNN
structure (meaning that it has been added, and not removed in the meantime)  
";

%feature("docstring") fastjet::Dnn4piCylinder::NearestNeighbourIndex "

Returns the index of the nearest neighbour of point labelled by ii (assumes ii
is valid)  
";

%feature("docstring") fastjet::Dnn4piCylinder::RemoveAndAddPoints "

insertion and removal of points  
";

%feature("docstring") fastjet::Dnn4piCylinder::NearestNeighbourDistance "

Returns the distance to the nearest neighbour of point labelled by index ii
(assumes ii is valid)  
";

%feature("docstring") fastjet::Dnn4piCylinder::Dnn4piCylinder "

empty initaliser  
";

%feature("docstring") fastjet::Dnn4piCylinder::Dnn4piCylinder "

Initialiser from a set of points on an Eta-Phi plane, where eta can have an
arbitrary ranges and phi must be in range 0 <= phi < 2pi.  
";

%feature("docstring") fastjet::Dnn4piCylinder::~Dnn4piCylinder "
";

// File: classfastjet_1_1DnnError.xml


%feature("docstring") fastjet::DnnError "
";

%feature("docstring") fastjet::DnnError::DnnError "
";

// File: classfastjet_1_1DnnPlane.xml


%feature("docstring") fastjet::DnnPlane "
";

%feature("docstring") fastjet::DnnPlane::eta "

returns the eta point with index i.  
";

%feature("docstring") fastjet::DnnPlane::NearestNeighbourDistance "

Returns the distance to the nearest neighbour of point labelled by index ii
(assumes ii is valid)  
";

%feature("docstring") fastjet::DnnPlane::etaphi "

returns the EtaPhi of point with index i.  
";

%feature("docstring") fastjet::DnnPlane::NearestNeighbourIndex "

Returns the index of the nearest neighbour of point labelled by ii (assumes ii
is valid)  
";

%feature("docstring") fastjet::DnnPlane::phi "

returns the phi point with index i.  
";

%feature("docstring") fastjet::DnnPlane::Valid "

Returns true iff the given index corresponds to a point that exists in the DNN
structure (meaning that it has been added, and not removed in the meantime)  
";

%feature("docstring") fastjet::DnnPlane::DnnPlane "

empty initaliser  
";

%feature("docstring") fastjet::DnnPlane::DnnPlane "

Initialiser from a set of points on an Eta-Phi plane, where both eta and phi can
have arbitrary ranges.  
";

%feature("docstring") fastjet::DnnPlane::RemoveAndAddPoints "

remove the points labelled by the vector indices_to_remove, and add the points
specified by the vector points_to_add (corresponding indices will be calculated
automatically); the idea behind this routine is that the points to be added will
somehow be close to the one or other of the points being removed and this can be
used by the implementation to provide hints for inserting the new points in
whatever structure it is using.  

In a kt-algorithm the points being added will be a result of a combination of
the points to be removed -- hence the proximity is (more or less) guaranteed.  
";

// File: classfastjet_1_1DynamicNearestNeighbours.xml


%feature("docstring") fastjet::DynamicNearestNeighbours "
";

%feature("docstring") fastjet::DynamicNearestNeighbours::RemovePoint "

Remove the point labelled by index and return the list of points whose nearest
neighbours have changed in the process.  
";

%feature("docstring") fastjet::DynamicNearestNeighbours::RemoveCombinedAddCombination "

Removes the two points labelled by index1, index2 and adds in the a point with
coordinates newpoint; it returns an index for the new point (index 3) and a
std::vector of indices of neighbours whose nearest neighbour has changed (the
list includes index3, i.e.  

the new point).  
";

%feature("docstring") fastjet::DynamicNearestNeighbours::~DynamicNearestNeighbours "

destructor -- here it is now implemented  
";

%feature("docstring") fastjet::DynamicNearestNeighbours::RemoveAndAddPoints "

remove the points labelled by the std::vector indices_to_remove, and add the
points specified by the std::vector points_to_add (corresponding indices will be
calculated automatically); the idea behind this routine is that the points to be
added will somehow be close to the one or other of the points being removed and
this can be used by the implementation to provide hints for inserting the new
points in whatever structure it is using.  

In a kt-algorithm the points being added will be a result of a combination of
the points to be removed -- hence the proximity is (more or less) guaranteed.  
";

%feature("docstring") fastjet::DynamicNearestNeighbours::NearestNeighbourIndex "

Dummy initialiser --- does nothing!  

Initialiser --- sets up the necessary structures to allow efficient nearest-
neighbour finding on the std::vector<EtaPhi> of input points Returns the index
of the nearest neighbour of point labelled by ii (assumes ii is valid)  
";

%feature("docstring") fastjet::DynamicNearestNeighbours::NearestNeighbourDistance "

Returns the distance to the nearest neighbour of point labelled by index ii
(assumes ii is valid)  
";

%feature("docstring") fastjet::DynamicNearestNeighbours::Valid "

Returns true iff the given index corresponds to a point that exists in the DNN
structure (meaning that it has been added, and not removed in the meantime)  
";

// File: classDynamicRfilt.xml


%feature("docstring") DynamicRfilt "
";

%feature("docstring") DynamicRfilt::DynamicRfilt "
";

%feature("docstring") DynamicRfilt::result "
";

// File: classfastjet_1_1Edge.xml


%feature("docstring") fastjet::Edge "
";

// File: structfastjet_1_1ClusterSequence_1_1EEBriefJet.xml

// File: classfastjet_1_1EECambridgePlugin.xml


%feature("docstring") fastjet::EECambridgePlugin "

Implementation of the e+e- Cambridge algorithm (plugin for fastjet v2.4 upwards)  

EECambridgePlugin is a plugin for fastjet (v2.4 upwards) It implements the
Cambridge algorithm, as defined in  

Better jet clustering algorithms Yuri Dokshitzer, Garth Leder, Stefano Moretti,
Bryan Webber JHEP 9708 (1997) 001 http://www-
spires.slac.stanford.edu/spires/find/hep/www?rawcmd=FIND+j+JHEPA%2C9708%2C001  

On construction one must supply a ycut value.  

To get the jets at the end call ClusterSequence::inclusive_jets();  

C++ includes: fastjet/EECambridgePlugin.hh
";

%feature("docstring") fastjet::EECambridgePlugin::description "

return a textual description of the jet-definition implemented in this plugin  
";

%feature("docstring") fastjet::EECambridgePlugin::run_clustering "

given a ClusterSequence that has been filled up with initial particles, the
following function should fill up the rest of the ClusterSequence, using the
following member functions of ClusterSequence:  

*   plugin_do_ij_recombination(...)  
*   plugin_do_iB_recombination(...)  
";

%feature("docstring") fastjet::EECambridgePlugin::ycut "
";

%feature("docstring") fastjet::EECambridgePlugin::EECambridgePlugin "

Main constructor for the EECambridge Plugin class.  

It takes the dimensionless parameter ycut (the Q value for normalisation of the
kt-distances is taken from the sum of all particle energies).  
";

%feature("docstring") fastjet::EECambridgePlugin::EECambridgePlugin "

copy constructor  
";

%feature("docstring") fastjet::EECambridgePlugin::exclusive_sequence_meaningful "

avoid the warning whenever the user requests \"exclusive\" jets from the cluster
sequence  
";

%feature("docstring") fastjet::EECambridgePlugin::R "

the plugin mechanism's standard way of accessing the jet radius.  

This must be set to return something sensible, even if R does not make sense for
this algorithm!  
";

%feature("docstring") fastjet::EECambridgePlugin::is_spherical "

returns true because this plugin is intended for spherical geometries (i.e.  

it's an e+e- algorithm).  
";

// File: classfastjet_1_1EECamBriefJet.xml


%feature("docstring") fastjet::EECamBriefJet "

class to help run an e+e- Cambridge algorithm  
";

%feature("docstring") fastjet::EECamBriefJet::init "
";

%feature("docstring") fastjet::EECamBriefJet::distance "
";

%feature("docstring") fastjet::EECamBriefJet::beam_distance "
";

// File: classfastjet_1_1Error.xml


%feature("docstring") fastjet::Error "

base class corresponding to errors that can be thrown by FastJet  

C++ includes: fastjet/Error.hh
";

%feature("docstring") fastjet::Error::Error "

default constructors  
";

%feature("docstring") fastjet::Error::Error "

ctor from an error message  

Parameters
----------
* `message` :  
    to be printed Note: in addition to the error message, one can choose to
    print the backtrace (showing the last few calls before the error) by using
    set_print_backtrace(true). The default is \"false\".  
";

%feature("docstring") fastjet::Error::set_print_errors "

controls whether the error message (and the backtrace, if its printing is
enabled) is printed out or not  
";

%feature("docstring") fastjet::Error::set_default_stream "

sets the default output stream for all errors; by default cerr; if it's null
then error output is suppressed.  
";

%feature("docstring") fastjet::Error::message "

the error message  
";

%feature("docstring") fastjet::Error::set_print_backtrace "

controls whether the backtrace is printed out with the error message or not.  

The default is \"false\".  
";

%feature("docstring") fastjet::Error::~Error "

virtual dummy dtor  
";

// File: classfastjet_1_1EtaPhi.xml


%feature("docstring") fastjet::EtaPhi "

Shortcut for dealing with eta-phi coordinates.  

C++ includes: fastjet/internal/DynamicNearestNeighbours.hh
";

%feature("docstring") fastjet::EtaPhi::sanitize "

put things into the desired range.  
";

%feature("docstring") fastjet::EtaPhi::EtaPhi "
";

%feature("docstring") fastjet::EtaPhi::EtaPhi "
";

// File: classfastjet_1_1ClusterSequence_1_1Extras.xml


%feature("docstring") fastjet::ClusterSequence::Extras "

base class to store extra information that plugins may provide  

a class intended to serve as a base in case a plugin needs to associate extra
information with a ClusterSequence (see SISConePlugin.* for an example).  

C++ includes: fastjet/ClusterSequence.hh
";

%feature("docstring") fastjet::ClusterSequence::Extras::description "
";

%feature("docstring") fastjet::ClusterSequence::Extras::~Extras "
";

// File: classfastjet_1_1Filter.xml


%feature("docstring") fastjet::Filter "

Class that helps perform filtering (Butterworth, Davison, Rubin and Salam,
arXiv:0802.2470) and trimming (Krohn, Thaler and Wang, arXiv:0912.1342) on jets,
optionally in conjunction with subtraction (Cacciari and Salam,
arXiv:0707.1378).  

For example, to apply filtering that reclusters a jet's constituents with the
Cambridge/Aachen jet algorithm with R=0.3 and then selects the 3 hardest
subjets, one can use the following code:  

To obtain trimming, involving for example the selection of all subjets carrying
at least 3% of the original jet's pt, the selector would be replaced by
SelectorPtFractionMin(0.03).  

To additionally perform subtraction on the subjets prior to selection, either
include a 3rd argument specifying the background density rho, or call the
set_subtractor(...) member function. If subtraction is requested, the original
jet must be the result of a clustering with active area with explicit ghosts
support or a merging of such pieces.  

The information on the subjets that were kept and rejected can be obtained
using:  
Implementation Note
If the original jet was defined with the Cambridge/Aachen algorithm (or is made
of pieces each of which comes from the C/A alg) and the filtering definition is
C/A, then the filter does not rerun the C/A algorithm on the constituents, but
instead makes use of the existent C/A cluster sequence in the original jet. This
increases the speed of the filter.  

See also 11 - use of filtering for a further usage example.  

Support for areas, reuse of C/A cluster sequences, etc., considerably
complicates the implementation of Filter. For an explanation of how a simpler
filter might be coded, see the \"User-defined transformers\" appendix of the
manual.  

C++ includes: fastjet/tools/Filter.hh
";

%feature("docstring") fastjet::Filter::set_subtractor "

Set a subtractor that is applied to all individual subjets before deciding which
ones to keep.  

It takes precedence over a non-zero rho.  
";

%feature("docstring") fastjet::Filter::Filter "

trivial ctor Note: this is just for derived classes a Filter initialised through
this constructor will not work!  
";

%feature("docstring") fastjet::Filter::Filter "

define a filter that decomposes a jet into subjets using a generic JetDefinition
and then keeps only a subset of these subjets according to a Selector.  

Optionally, each subjet may be internally bakground-subtracted prior to
selection.  

Parameters
----------
* `subjet_def` :  
    the jet definition applied to obtain the subjets  
* `selector` :  
    the Selector applied to compute the kept subjets  
* `rho` :  
    if non-zero, backgruond-subtract each subjet befor selection  

Note: internal subtraction only applies on jets that are obtained with a cluster
sequence with area support and explicit ghosts  
";

%feature("docstring") fastjet::Filter::Filter "

Same as the full constructor (see above) but just specifying the radius By
default, Cambridge-Aachen is used If the jet (or all its pieces) is obtained
with a non-default recombiner, that one will be used.  

Parameters
----------
* `Rfilt` :  
    the filtering radius  
";

%feature("docstring") fastjet::Filter::Filter "

Same as the full constructor (see above) but just specifying a filtering radius
that will depend on the jet being filtered As for the previous case, Cambridge-
Aachen is used If the jet (or all its pieces) is obtained with a non-default
recombiner, that one will be used.  

Parameters
----------
* `Rfilt_func` :  
    the filtering radius function of a PseudoJet  
";

%feature("docstring") fastjet::Filter::result "

runs the filtering and sets kept and rejected to be the jets of interest (with
non-zero rho, they will have been subtracted).  

Parameters
----------
* `jet` :  
    the jet that gets filtered  

Returns
-------
the filtered jet  
";

%feature("docstring") fastjet::Filter::description "

class description  
";

%feature("docstring") fastjet::Filter::~Filter "

default dtor  
";

%feature("docstring") fastjet::Filter::subtractor "

Set a subtractor that is applied to all individual subjets before deciding which
ones to keep.  

It takes precedence over a non-zero rho.  
";

// File: classfastjet_1_1FilterStructure.xml


%feature("docstring") fastjet::FilterStructure "

Class to contain structure information for a filtered jet.  

C++ includes: fastjet/tools/Filter.hh
";

/*
 The filter-specific information 
*/

%feature("docstring") fastjet::FilterStructure::FilterStructure "

constructor from an original ClusterSequenceInfo We just share the original
ClusterSequenceWrapper and initialise the rest  
";

%feature("docstring") fastjet::FilterStructure::~FilterStructure "

virtual dtor to allow further overloading  
";

%feature("docstring") fastjet::FilterStructure::description "

description  
";

%feature("docstring") fastjet::FilterStructure::rejected "

returns the subjets that were not kept during the filtering procedure
(subtracted if the filter requests it, and valid in the original cs)  
";

%feature("docstring") fastjet::FilterStructure::Filter "
";

// File: classFlavourRecombiner.xml


%feature("docstring") FlavourRecombiner "
";

%feature("docstring") FlavourRecombiner::description "
";

%feature("docstring") FlavourRecombiner::description "
";

%feature("docstring") FlavourRecombiner::description "
";

%feature("docstring") FlavourRecombiner::FlavourRecombiner "
";

%feature("docstring") FlavourRecombiner::FlavourRecombiner "
";

%feature("docstring") FlavourRecombiner::FlavourRecombiner "
";

%feature("docstring") FlavourRecombiner::recombine "

recombine pa and pb and put result into pab  
";

%feature("docstring") FlavourRecombiner::recombine "

recombine pa and pb and put result into pab  
";

%feature("docstring") FlavourRecombiner::recombine "

recombine pa and pb and put result into pab  
";

// File: classfastjet_1_1Freelist.xml


%feature("docstring") fastjet::Freelist "
";

// File: classfastjet_1_1Freenode.xml


%feature("docstring") fastjet::Freenode "
";

// File: classfastjet_1_1FreeNodeArrayList.xml


%feature("docstring") fastjet::FreeNodeArrayList "
";

// File: classfastjet_1_1FunctionOfPseudoJet.xml


%feature("docstring") fastjet::FunctionOfPseudoJet "

base class providing interface for a generic function of a PseudoJet  

This class serves as a base class to provide a standard interface for a function
that returns an object of a given (templated) type that depends on a PseudoJet
argument. The rationale for using a class (rather than a pointer to a function)
is that a class can be constructed with (and store) additional arguments.  

C++ includes: fastjet/FunctionOfPseudoJet.hh
";

%feature("docstring") fastjet::FunctionOfPseudoJet::~FunctionOfPseudoJet "

default dtor (virtual to allow safe polymorphism)  
";

%feature("docstring") fastjet::FunctionOfPseudoJet::result "

the action of the function this *has* to be overloaded in derived classes  

Parameters
----------
* `pj` :  
    the PseudoJet input to the function  
";

%feature("docstring") fastjet::FunctionOfPseudoJet::description "

returns a description of the function (an empty string by default)  
";

%feature("docstring") fastjet::FunctionOfPseudoJet::FunctionOfPseudoJet "

default ctor  
";

// File: classfastjet_1_1GhostedAreaSpec.xml


%feature("docstring") fastjet::GhostedAreaSpec "

Parameters to configure the computation of jet areas using ghosts.  

Class that defines the parameters that go into the measurement of active jet
areas.  

C++ includes: fastjet/GhostedAreaSpec.hh
";

%feature("docstring") fastjet::GhostedAreaSpec::kt_scatter "
";

%feature("docstring") fastjet::GhostedAreaSpec::ghost_area "
";

%feature("docstring") fastjet::GhostedAreaSpec::GhostedAreaSpec "

default constructor  
";

%feature("docstring") fastjet::GhostedAreaSpec::GhostedAreaSpec "

explicit constructor  
";

%feature("docstring") fastjet::GhostedAreaSpec::GhostedAreaSpec "

explicit constructor  
";

%feature("docstring") fastjet::GhostedAreaSpec::GhostedAreaSpec "

constructor based on a Selector  

explicit constructor  
";

%feature("docstring") fastjet::GhostedAreaSpec::add_ghosts "

push a set of ghost 4-momenta onto the back of the vector of PseudoJets  

adds the ghost 4-momenta to the vector of PseudoJet's  
";

%feature("docstring") fastjet::GhostedAreaSpec::set_pt_scatter "
";

%feature("docstring") fastjet::GhostedAreaSpec::ghost_maxrap "
";

%feature("docstring") fastjet::GhostedAreaSpec::ghost_etamax "
";

%feature("docstring") fastjet::GhostedAreaSpec::mean_ghost_pt "
";

%feature("docstring") fastjet::GhostedAreaSpec::set_ghost_maxrap "
";

%feature("docstring") fastjet::GhostedAreaSpec::actual_ghost_area "
";

%feature("docstring") fastjet::GhostedAreaSpec::set_kt_scatter "
";

%feature("docstring") fastjet::GhostedAreaSpec::fj2_placement "
";

%feature("docstring") fastjet::GhostedAreaSpec::nrap "
";

%feature("docstring") fastjet::GhostedAreaSpec::set_ghost_maxeta "
";

%feature("docstring") fastjet::GhostedAreaSpec::generator_at_own_risk "

very deprecated public access to the generator itself  
";

%feature("docstring") fastjet::GhostedAreaSpec::random_at_own_risk "

very deprecated public access to a random number from the internal generator  
";

%feature("docstring") fastjet::GhostedAreaSpec::__attribute__ "

if val is true, set ghost placement as it was in FastJet 2.X.  

The main differences between FJ2 and FJ3 ghost placement are  

*   in FJ2 the rapidity spacing was ceil((maxrap-minrap)/sqrt(area)), while in
    FJ3 it is int((maxrap-minrap)/sqrt(area) + 0.5) [similarly for phi]. The FJ3
    option offers more stability when trying to specify a spacing that exactly
    fits the extent.  

in FJ2, the ghosts are placed at the corners of grid cells (i.e. extending up to
maxrap), while in FJ3 they are placed at the centres of grid cells (i.e.
extending roughly up to maxrap-sqrt(area)). The FJ2 behaviour effectively skews
the total area coverage when maxrap is small, by an amount
sqrt(area)/(2*maxrap).  

FJ2 placement is now deprecated.  
";

%feature("docstring") fastjet::GhostedAreaSpec::ghost_rapmax "
";

%feature("docstring") fastjet::GhostedAreaSpec::grid_scatter "
";

%feature("docstring") fastjet::GhostedAreaSpec::n_ghosts "
";

%feature("docstring") fastjet::GhostedAreaSpec::get_random_status "

get all relevant information about the status of the random number generator, so
that it can be reset subsequently with set_random_status.  
";

%feature("docstring") fastjet::GhostedAreaSpec::description "

for a summary  
";

%feature("docstring") fastjet::GhostedAreaSpec::mean_ghost_kt "
";

%feature("docstring") fastjet::GhostedAreaSpec::repeat "
";

%feature("docstring") fastjet::GhostedAreaSpec::ghost_maxeta "
";

%feature("docstring") fastjet::GhostedAreaSpec::set_grid_scatter "
";

%feature("docstring") fastjet::GhostedAreaSpec::set_random_status "

set the status of the random number generator, as obtained previously with
get_random_status.  

Note that the random generator is a static member of the class, i.e. common to
all instances of the class --- so if you modify the random for this instance,
you modify it for all instances.  
";

%feature("docstring") fastjet::GhostedAreaSpec::checkpoint_random "
";

%feature("docstring") fastjet::GhostedAreaSpec::set_ghost_rapmax "
";

%feature("docstring") fastjet::GhostedAreaSpec::nphi "

return nphi (ghosts layed out (-nrap, 0..nphi-1), (-nrap+1,0..nphi-1), ...  

(nrap,0..nphi-1)  
";

%feature("docstring") fastjet::GhostedAreaSpec::pt_scatter "
";

%feature("docstring") fastjet::GhostedAreaSpec::restore_checkpoint_random "
";

%feature("docstring") fastjet::GhostedAreaSpec::_initialize "

does the initialization of actual ghost parameters  

sets the detailed parameters for the ghosts (which may not be quite the same as
those requested -- this is in order for things to fit in nicely into 2pi etc...  
";

%feature("docstring") fastjet::GhostedAreaSpec::set_ghost_etamax "
";

%feature("docstring") fastjet::GhostedAreaSpec::set_mean_ghost_pt "
";

%feature("docstring") fastjet::GhostedAreaSpec::set_mean_ghost_kt "
";

%feature("docstring") fastjet::GhostedAreaSpec::set_repeat "
";

%feature("docstring") fastjet::GhostedAreaSpec::set_ghost_area "
";

// File: classfastjet_1_1ClusterSequenceActiveArea_1_1GhostJet.xml

// File: classfastjet_1_1GraphEdge.xml


%feature("docstring") fastjet::GraphEdge "
";

// File: classfastjet_1_1GridJetPlugin.xml


%feature("docstring") fastjet::GridJetPlugin "

plugin for fastjet (v3.0 upwards) that clusters particles such that all
particles in a given cell of a rectangular rapidity-phi grid end up in a common
\"jet\".  

This is not intended for use as a regular jet clustering algorithm, but is
rather provided for comparison purposes with the GridMedianBackgroundEstimator
(which is even faster).  

C++ includes: fastjet/GridJetPlugin.hh
";

%feature("docstring") fastjet::GridJetPlugin::R "

This returns the sqrt(dphi*dy/pi) -- i.e.  

the radius that for a circular jet would give the same area.  
";

%feature("docstring") fastjet::GridJetPlugin::run_clustering "

given a ClusterSequence that has been filled up with initial particles, the
following function should fill up the rest of the ClusterSequence, using the
following member functions of ClusterSequence:  

*   plugin_do_ij_recombination(...)  
*   plugin_do_iB_recombination(...)  
";

%feature("docstring") fastjet::GridJetPlugin::description "

return a textual description of the jet-definition implemented in this plugin  
";

%feature("docstring") fastjet::GridJetPlugin::GridJetPlugin "

Basic constructor for the GridJetPlugin Plugin class.  

Parameters
----------
* `ymax` :  
    The maximal rapidity extent of the grid  
* `requested_grid_spacing` :  
    The requested grid spacing  
* `post_jet_def` :  
    if present, and not == JetDefinition() (which has undefined_jet_algorithm),
    then run the post_jet_def on the result of the grid clustering.  
";

%feature("docstring") fastjet::GridJetPlugin::GridJetPlugin "

Constructor for the GridJetPlugin Plugin class that allows full control over the
underlying grid.  

New in FastJet 3.1.  

Parameters
----------
* `grid` :  
    The maximal rapidity extent of the grid  
* `post_jet_def` :  
    if present, and not == JetDefinition() (which has undefined_jet_algorithm),
    then run the post_jet_def on the result of the grid clustering.  
";

// File: classfastjet_1_1GridMedianBackgroundEstimator.xml


%feature("docstring") fastjet::GridMedianBackgroundEstimator "

Background Estimator based on the median pt/area of a set of grid cells.  

Description of the method: This background estimator works by projecting the
event onto a grid in rapidity and azimuth. In each grid cell, the scalar pt sum
of the particles in the cell is computed. The background density is then
estimated by the median of (scalar pt sum/cell area) for all cells.  

Parameters: The class takes 2 arguments: the absolute rapidity extent of the
cells and the size of the grid cells. Note that the size of the cell will be
adjusted in azimuth to satisfy the 2pi periodicity and in rapidity to match the
requested rapidity extent.  

Rescaling: It is possible to use a rescaling profile. In this case, the profile
needs to be set before setting the particles and it will be applied to each
particle (i.e. not to each cell). Note also that in this case one needs to call
rho(jet) instead of rho() [Without rescaling, they are identical]  

C++ includes: fastjet/tools/GridMedianBackgroundEstimator.hh
";

/*
 constructors and destructors 
*/

/*
 setting a new event 
*/

/*
 retrieving fundamental information 
*/

/*
 configuring the behaviour 
*/

/*
 description 
*/

%feature("docstring") fastjet::GridMedianBackgroundEstimator::has_rho_m "

Returns true if this background estimator has support for determination of
rho_m.  

Note that support for sigma_m is automatic if one has sigma and rho_m support.  
";

%feature("docstring") fastjet::GridMedianBackgroundEstimator::mean_area "

returns the area of the grid cells (all identical, but referred to as \"mean\"
area for uniformity with JetMedianBGE).  
";

%feature("docstring") fastjet::GridMedianBackgroundEstimator::description "

returns a textual description of the background estimator  
";

%feature("docstring") fastjet::GridMedianBackgroundEstimator::sigma_m "

returns sigma_m, a measure of the fluctuations in the purely longitudinal,
particle-mass-induced component of the background density per unit area; must be
multipled by sqrt(area) to get fluctuations for a region of a given area.  
";

%feature("docstring") fastjet::GridMedianBackgroundEstimator::sigma_m "

Returns sigma_m locally at the jet position. As for rho(jet), it is non-const.  
";

%feature("docstring") fastjet::GridMedianBackgroundEstimator::has_sigma "

returns true if this background estimator has support for determination of sigma  
";

%feature("docstring") fastjet::GridMedianBackgroundEstimator::rho_m "

Returns rho_m, the purely longitudinal, particle-mass-induced component of the
background density per unit area.  
";

%feature("docstring") fastjet::GridMedianBackgroundEstimator::rho_m "

Returns rho_m locally at the jet position. As for rho(jet), it is non-const.  
";

%feature("docstring") fastjet::GridMedianBackgroundEstimator::rho "

returns rho, the median background density per unit area  
";

%feature("docstring") fastjet::GridMedianBackgroundEstimator::rho "

returns rho, the background density per unit area, locally at the position of a
given jet.  

Note that this is not const, because a user may then wish to query other aspects
of the background that could depend on the position of the jet last used for a
rho(jet) determination.  
";

%feature("docstring") fastjet::GridMedianBackgroundEstimator::set_rescaling_class "

Set a pointer to a class that calculates the rescaling factor as a function of
the jet (position).  

Note that the rescaling factor is used both in the determination of the
\"global\" rho (the pt/A of each jet is divided by this factor) and when asking
for a local rho (the result is multiplied by this factor).  

The BackgroundRescalingYPolynomial class can be used to get a rescaling that
depends just on rapidity.  

Note that this has to be called BEFORE any attempt to do an actual computation  

The same profile will be used for both pt and mt (this is probabaly a good
approximation since the particle density changes is what dominates the rapidity
profile)  
";

%feature("docstring") fastjet::GridMedianBackgroundEstimator::sigma "

returns sigma, the background fluctuations per unit area; must be multipled by
sqrt(area) to get fluctuations for a region of a given area.  
";

%feature("docstring") fastjet::GridMedianBackgroundEstimator::sigma "

returns sigma, the background fluctuations per unit area, locally at the
position of a given jet.  

As for rho(jet), it is non-const.  
";

%feature("docstring") fastjet::GridMedianBackgroundEstimator::GridMedianBackgroundEstimator "

Parameters
----------
* `ymax` :  
    maximal absolute rapidity extent of the grid  
* `requested_grid_spacing` :  
    size of the grid cell. The \"real\" cell size could differ due e.g. to the
    2pi periodicity in azimuthal angle (size, not area)  
";

%feature("docstring") fastjet::GridMedianBackgroundEstimator::GridMedianBackgroundEstimator "

Constructor based on a user's fully specified RectangularGrid.  
";

%feature("docstring") fastjet::GridMedianBackgroundEstimator::GridMedianBackgroundEstimator "

Constructor with the explicit parameters for the underlying RectangularGrid.  

Parameters
----------
* `rapmin` :  
    the minimum rapidity extent of the grid  
* `rapmax` :  
    the maximum rapidity extent of the grid  
* `drap` :  
    the grid spacing in rapidity  
* `dphi` :  
    the grid spacing in azimuth  
* `tile_selector` :  
    optional (geometric) selector to specify which tiles are good; a tile is
    good if a massless 4-vector at the center of the tile passes the selection  
";

%feature("docstring") fastjet::GridMedianBackgroundEstimator::set_compute_rho_m "

determine whether the automatic calculation of rho_m and sigma_m is enabled (by
default true)  
";

%feature("docstring") fastjet::GridMedianBackgroundEstimator::set_particles "

tell the background estimator that it has a new event, composed of the specified
particles.  
";

// File: classfastjet_1_1Halfedge.xml


%feature("docstring") fastjet::Halfedge "
";

// File: classfastjet_1_1d0_1_1HepEntity.xml


%feature("docstring") fastjet::d0::HepEntity "
";

%feature("docstring") fastjet::d0::HepEntity::Fill "
";

%feature("docstring") fastjet::d0::HepEntity::y "
";

%feature("docstring") fastjet::d0::HepEntity::phi "
";

%feature("docstring") fastjet::d0::HepEntity::HepEntity "
";

%feature("docstring") fastjet::d0::HepEntity::HepEntity "
";

%feature("docstring") fastjet::d0::HepEntity::HepEntity "
";

%feature("docstring") fastjet::d0::HepEntity::p4vec "
";

%feature("docstring") fastjet::d0::HepEntity::pT "
";

%feature("docstring") fastjet::d0::HepEntity::Add "
";

// File: classfastjet_1_1d0runi_1_1HepEntityI.xml


%feature("docstring") fastjet::d0runi::HepEntityI "
";

%feature("docstring") fastjet::d0runi::HepEntityI::HepEntityI "
";

%feature("docstring") fastjet::d0runi::HepEntityI::HepEntityI "
";

%feature("docstring") fastjet::d0runi::HepEntityI::HepEntityI "
";

%feature("docstring") fastjet::d0runi::HepEntityI::Add "
";

%feature("docstring") fastjet::d0runi::HepEntityI::p4vec "
";

%feature("docstring") fastjet::d0runi::HepEntityI::Fill "
";

%feature("docstring") fastjet::d0runi::HepEntityI::pT "
";

%feature("docstring") fastjet::d0runi::HepEntityI::pz "
";

%feature("docstring") fastjet::d0runi::HepEntityI::py "
";

%feature("docstring") fastjet::d0runi::HepEntityI::px "
";

%feature("docstring") fastjet::d0runi::HepEntityI::E "
";

// File: classfastjet_1_1d0runi_1_1HepEntityIpre96.xml


%feature("docstring") fastjet::d0runi::HepEntityIpre96 "
";

%feature("docstring") fastjet::d0runi::HepEntityIpre96::pz "
";

%feature("docstring") fastjet::d0runi::HepEntityIpre96::px "
";

%feature("docstring") fastjet::d0runi::HepEntityIpre96::py "
";

%feature("docstring") fastjet::d0runi::HepEntityIpre96::E "
";

%feature("docstring") fastjet::d0runi::HepEntityIpre96::Fill "
";

%feature("docstring") fastjet::d0runi::HepEntityIpre96::HepEntityIpre96 "
";

%feature("docstring") fastjet::d0runi::HepEntityIpre96::HepEntityIpre96 "
";

%feature("docstring") fastjet::d0runi::HepEntityIpre96::Add "
";

// File: structfastjet_1_1ClusterSequence_1_1history__element.xml


%feature("docstring") fastjet::ClusterSequence::history_element "

a single element in the clustering history  

(see vector _history below).  

C++ includes: fastjet/ClusterSequence.hh
";

// File: structfastjet_1_1IsBaseAndDerived_1_1Host.xml


%feature("docstring") fastjet::IsBaseAndDerived::Host "
";

// File: classfastjet_1_1IndexedSortHelper.xml


%feature("docstring") fastjet::IndexedSortHelper "
";

%feature("docstring") fastjet::IndexedSortHelper::IndexedSortHelper "
";

// File: classfastjet_1_1PseudoJet_1_1InexistentUserInfo.xml


%feature("docstring") fastjet::PseudoJet::InexistentUserInfo "

error class to be thrown if accessing user info when it doesn't exist  

C++ includes: fastjet/PseudoJet.hh
";

%feature("docstring") fastjet::PseudoJet::InexistentUserInfo::InexistentUserInfo "

provide a meaningful error message for InexistentUserInfo  
";

// File: classfastjet_1_1InitialisedInt.xml


%feature("docstring") fastjet::InitialisedInt "
";

%feature("docstring") fastjet::InitialisedInt::InitialisedInt "
";

%feature("docstring") fastjet::InitialisedInt::val "
";

// File: structfastjet_1_1integral__type.xml


%feature("docstring") fastjet::integral_type "
";

// File: classfastjet_1_1InternalError.xml


%feature("docstring") fastjet::InternalError "

class corresponding to critical internal errors  

This is an error class (derived from Error) meant for serious, critical,
internal errors that we still want to be catchable by an end-user [e.g. a
serious issue in clustering where the end-user can catch it and retry with a
different strategy]  

Please directly contact the FastJet authors if you see such an error.  

C++ includes: fastjet/Error.hh
";

%feature("docstring") fastjet::InternalError::InternalError "

ctor with error message: just add a bit of info to the message and pass it to
the base class  
";

// File: classfastjet_1_1Selector_1_1InvalidArea.xml


%feature("docstring") fastjet::Selector::InvalidArea "

class that gets thrown when the area is requested from a Selector for which the
area is not meaningful  

C++ includes: fastjet/Selector.hh
";

%feature("docstring") fastjet::Selector::InvalidArea::InvalidArea "
";

// File: classfastjet_1_1Selector_1_1InvalidWorker.xml


%feature("docstring") fastjet::Selector::InvalidWorker "

class that gets thrown when a Selector is applied despite it not having a valid
underlying worker.  

C++ includes: fastjet/Selector.hh
";

%feature("docstring") fastjet::Selector::InvalidWorker::InvalidWorker "
";

// File: structfastjet_1_1IsBaseAndDerived.xml


%feature("docstring") fastjet::IsBaseAndDerived "
";

// File: classfastjet_1_1JadeBriefJet.xml


%feature("docstring") fastjet::JadeBriefJet "

class to help run a JADE algorithm  

This class works both with NNH and NNFJN2Plain clustering helpers. They both use
the same init(...) call, but for the clustering:  

*   NNH uses distance(...) and beam_distance()  
*   NNFJPlainN2 uses geometrical_distance(...), momentum_factor() and
    geometrical_beam_distance()  

For NNFJPlainN2 the 2 E_i E_j (1-cos theta_{ij}) factor gets broken up into  

    sqrt(2)*min(E_i,E_j) * [sqrt(2)*max(E_i,E_j) (1 - cos \\theta_{ij})]  

The second factor is what we call the \"geometrical_distance\" even though it
isn't actually purely geometrical. But the fact that it gets multiplied by
min(E_i,E_j) to get the full distance is sufficient for the validity of the FJ
lemma, allowing for the use of NNFJN2Plain.  
";

%feature("docstring") fastjet::JadeBriefJet::geometrical_beam_distance "
";

%feature("docstring") fastjet::JadeBriefJet::beam_distance "
";

%feature("docstring") fastjet::JadeBriefJet::init "
";

%feature("docstring") fastjet::JadeBriefJet::distance "
";

%feature("docstring") fastjet::JadeBriefJet::geometrical_distance "
";

%feature("docstring") fastjet::JadeBriefJet::momentum_factor "
";

// File: classfastjet_1_1JadePlugin.xml


%feature("docstring") fastjet::JadePlugin "

Implementation of the e+e- Jade algorithm (plugin for fastjet v2.4 upwards)  

JadePlugin is a plugin for fastjet (v2.4 upwards) It implements the JADE
algorithm, which is an e+e- sequential recombination algorithm with
interparticle distance  

dij = 2 E_i E_j (1 - cos theta_ij)  

or equivalently  

yij = dij/E_{vis}^2  

This corresponds to the distance measured used in  

\"Experimental Investigation of the Energy Dependence of the Strong Coupling
Strength.\" JADE Collaboration (S. Bethke et al.) Phys.Lett.B213:235,1988  

The JADE article carries out particle recombinations in the E-scheme (4-vector
recombination), which is the default procedure for this plugin.  

NOTE: other widely used schemes include E0, P, P0; however they also involve
modifications to the distance measure. Be sure of what you're doing before
running a JADE type algorithm.  

To access the jets with a given ycut value (clustering stops once all yij >
ycut), use  

vector<PseudoJet> jets = cluster_sequence.exclusive_jets_ycut(ycut);  

and related routines.  

C++ includes: fastjet/JadePlugin.hh
";

%feature("docstring") fastjet::JadePlugin::JadePlugin "

Main constructor for the Jade Plugin class.  
";

%feature("docstring") fastjet::JadePlugin::JadePlugin "

copy constructor  
";

%feature("docstring") fastjet::JadePlugin::run_clustering "

given a ClusterSequence that has been filled up with initial particles, the
following function should fill up the rest of the ClusterSequence, using the
following member functions of ClusterSequence:  

*   plugin_do_ij_recombination(...)  
*   plugin_do_iB_recombination(...)  
";

%feature("docstring") fastjet::JadePlugin::R "

the plugin mechanism's standard way of accessing the jet radius.  

This must be set to return something sensible, even if R does not make sense for
this algorithm!  
";

%feature("docstring") fastjet::JadePlugin::description "

return a textual description of the jet-definition implemented in this plugin  
";

%feature("docstring") fastjet::JadePlugin::exclusive_sequence_meaningful "

avoid the warning whenever the user requests \"exclusive\" jets from the cluster
sequence  
";

// File: classfastjet_1_1atlas_1_1Jet.xml


%feature("docstring") fastjet::atlas::Jet "
";

%feature("docstring") fastjet::atlas::Jet::addJet "

The standard way of merging jets.  
";

%feature("docstring") fastjet::atlas::Jet::addJet "
";

%feature("docstring") fastjet::atlas::Jet::addConstituent "
";

%feature("docstring") fastjet::atlas::Jet::addConstituent "
";

%feature("docstring") fastjet::atlas::Jet::hlv "

Atlas compatibility code :  
";

%feature("docstring") fastjet::atlas::Jet::getConstituentNum "

Access jet constituents.  
";

%feature("docstring") fastjet::atlas::Jet::set_index "
";

%feature("docstring") fastjet::atlas::Jet::Jet "
";

%feature("docstring") fastjet::atlas::Jet::Jet "
";

%feature("docstring") fastjet::atlas::Jet::Jet "
";

%feature("docstring") fastjet::atlas::Jet::Jet "
";

%feature("docstring") fastjet::atlas::Jet::Jet "
";

%feature("docstring") fastjet::atlas::Jet::removeConstituent "
";

%feature("docstring") fastjet::atlas::Jet::lastConstituent "
";

%feature("docstring") fastjet::atlas::Jet::firstConstituent "
";

%feature("docstring") fastjet::atlas::Jet::index "
";

%feature("docstring") fastjet::atlas::Jet::addConstituent_notMoment "
";

// File: classfastjet_1_1CASubJetTagger_1_1JetAux.xml

// File: classfastjet_1_1atlas_1_1JetConeFinderTool.xml


%feature("docstring") fastjet::atlas::JetConeFinderTool "
";

%feature("docstring") fastjet::atlas::JetConeFinderTool::calc_cone "
";

%feature("docstring") fastjet::atlas::JetConeFinderTool::reconstruct "
";

%feature("docstring") fastjet::atlas::JetConeFinderTool::~JetConeFinderTool "
";

%feature("docstring") fastjet::atlas::JetConeFinderTool::JetConeFinderTool "
";

%feature("docstring") fastjet::atlas::JetConeFinderTool::execute "
";

// File: classfastjet_1_1JetDefinition.xml


%feature("docstring") fastjet::JetDefinition "

class that is intended to hold a full definition of the jet clusterer  

C++ includes: fastjet/JetDefinition.hh
";

%feature("docstring") fastjet::JetDefinition::plugin "

return a pointer to the plugin  
";

%feature("docstring") fastjet::JetDefinition::extra_param "
";

%feature("docstring") fastjet::JetDefinition::description_no_recombiner "

returns a description not including the recombiner information  
";

%feature("docstring") fastjet::JetDefinition::R "
";

%feature("docstring") fastjet::JetDefinition::n_parameters_for_algorithm "

the number of parameters associated to a given jet algorithm  
";

%feature("docstring") fastjet::JetDefinition::jet_algorithm "

return information about the definition...  
";

%feature("docstring") fastjet::JetDefinition::__attribute__ "

constructor to fully specify a jet-definition (together with information about
how algorithically to run it).  

the ordering of arguments here is old and deprecated (except as the common
constructor for internal use)  
";

%feature("docstring") fastjet::JetDefinition::delete_plugin_when_unused "

calling this causes the JetDefinition to handle the deletion of the plugin when
it is no longer used  

allows to let the JetDefinition handle the deletion of the plugin when it is no
longer used  
";

%feature("docstring") fastjet::JetDefinition::set_jet_finder "

same as above for backward compatibility  
";

%feature("docstring") fastjet::JetDefinition::description "

return a textual description of the current jet definition  
";

%feature("docstring") fastjet::JetDefinition::strategy "
";

%feature("docstring") fastjet::JetDefinition::has_same_recombiner "

returns true if the current jet definitions shares the same recombiner as the
one passed as an argument  
";

%feature("docstring") fastjet::JetDefinition::is_spherical "

returns true if the jet definition involves an algorithm intended for use on a
spherical geometry (e.g.  

e+e- algorithms, as opposed to most pp algorithms, which use a cylindrical,
rapidity-phi geometry).  
";

%feature("docstring") fastjet::JetDefinition::JetDefinition "

constructor with alternative ordering or arguments -- note that we have not
provided a default jet finder, to avoid ambiguous JetDefinition() constructor.  
";

%feature("docstring") fastjet::JetDefinition::JetDefinition "

constructor for algorithms that have no free parameters (e.g.  

ee_kt_algorithm)  
";

%feature("docstring") fastjet::JetDefinition::JetDefinition "

constructor for algorithms that require R + one extra parameter to be set (the
gen-kt series for example)  
";

%feature("docstring") fastjet::JetDefinition::JetDefinition "

constructor in a form that allows the user to provide a pointer to an external
recombiner class (which must remain valid for the life of the JetDefinition
object).  
";

%feature("docstring") fastjet::JetDefinition::JetDefinition "

constructor for case with 0 parameters (ee_kt_algorithm) and and external
recombiner  
";

%feature("docstring") fastjet::JetDefinition::JetDefinition "

constructor allowing the extra parameter to be set and a pointer to a recombiner  
";

%feature("docstring") fastjet::JetDefinition::JetDefinition "

a default constructor which creates a jet definition that is in a well-defined
internal state, but not actually usable for jet clustering.  
";

%feature("docstring") fastjet::JetDefinition::JetDefinition "

constructor based on a pointer to a user's plugin; the object pointed to must
remain valid for the whole duration of existence of the JetDefinition and any
related ClusterSequences  
";

%feature("docstring") fastjet::JetDefinition::JetDefinition "

constructor to fully specify a jet-definition (together with information about
how algorithically to run it).  
";

%feature("docstring") fastjet::JetDefinition::algorithm_description "

a short textual description of the algorithm jet_alg  
";

%feature("docstring") fastjet::JetDefinition::set_extra_param "

(re)set the general purpose extra parameter  
";

%feature("docstring") fastjet::JetDefinition::set_recombination_scheme "

set the recombination scheme to the one provided  
";

%feature("docstring") fastjet::JetDefinition::delete_recombiner_when_unused "

calling this tells the JetDefinition to handle the deletion of the recombiner
when it is no longer used.  

causes the JetDefinition to handle the deletion of the recombiner when it is no
longer used  

(Should not be called if the recombiner was initialised from a JetDef whose
recombiner was already scheduled to delete itself - memory handling will already
be automatic across both JetDef's in that case).  
";

%feature("docstring") fastjet::JetDefinition::recombiner "

returns a pointer to the currently defined recombiner.  

Warning: the pointer may be to an internal recombiner (for default recombination
schemes), in which case if the JetDefinition becomes invalid (e.g. is deleted),
the pointer will then point to an object that no longer exists.  

Note also that if you copy a JetDefinition with a default recombination scheme,
then the two copies will have distinct recombiners, and return different
recombiner() pointers.  
";

%feature("docstring") fastjet::JetDefinition::set_recombiner "

set the recombiner class to the one provided  

Note that in order to associate to a jet definition a recombiner from another
jet definition, it is strongly recommended to use the set_recombiner(const
JetDefinition &) method below. The latter correctly handles the situations where
the jet definition owns the recombiner (i.e. where delete_recombiner_when_unused
has been called). In such cases, using set_recombiner(const Recombiner *) may
lead to memory corruption.  
";

%feature("docstring") fastjet::JetDefinition::set_recombiner "

set the recombiner to be the same as the one of 'other_jet_def'  

Note that this is the recommended method to associate to a jet definition the
recombiner from another jet definition. Compared to the set_recombiner(const
Recombiner *) above, it correctly handles the case where the jet definition owns
the recombiner (i.e. where delete_recombiner_when_unused has been called)  
";

%feature("docstring") fastjet::JetDefinition::jet_finder "

same as above for backward compatibility  
";

%feature("docstring") fastjet::JetDefinition::recombination_scheme "
";

%feature("docstring") fastjet::JetDefinition::set_jet_algorithm "

(re)set the jet finder  
";

// File: structfastjet_1_1atlas_1_1JetDistances.xml


%feature("docstring") fastjet::atlas::JetDistances "
";

%feature("docstring") fastjet::atlas::JetDistances::deltaPhi "
";

%feature("docstring") fastjet::atlas::JetDistances::deltaPhi "
";

%feature("docstring") fastjet::atlas::JetDistances::deltaPhi "
";

%feature("docstring") fastjet::atlas::JetDistances::deltaEta "
";

%feature("docstring") fastjet::atlas::JetDistances::deltaEta "
";

%feature("docstring") fastjet::atlas::JetDistances::deltaEta "
";

%feature("docstring") fastjet::atlas::JetDistances::fixedPhi "
";

%feature("docstring") fastjet::atlas::JetDistances::deltaR "
";

%feature("docstring") fastjet::atlas::JetDistances::deltaR "
";

%feature("docstring") fastjet::atlas::JetDistances::deltaR "
";

// File: classfastjet_1_1JetMedianBackgroundEstimator.xml


%feature("docstring") fastjet::JetMedianBackgroundEstimator "

Class to estimate the pt density of the background per unit area, using the
median of the distribution of pt/area from jets that pass some selection
criterion.  

Events are passed either in the form of the event particles (in which they're
clustered by the class), a ClusterSequenceArea (in which case the jets used are
those returned by \"inclusive_jets()\") or directly as a set of jets.  

The selection criterion is typically a geometrical one (e.g. all jets with
|y|<2) sometimes supplemented with some kinematical restriction (e.g. exclusion
of the two hardest jets). It is passed to the class through a Selector.  

Beware: by default, to correctly handle partially empty events, the class
attempts to calculate an \"empty area\", based (schematically) on  

       range.total_area() - sum_{jets_in_range} jets.area()  

For ranges with small areas, this can be inaccurate (particularly relevant in
dense events where empty_area should be zero and ends up not being zero).  

This calculation of empty area can be avoided if a ClusterSequenceArea class
with explicit ghosts (ActiveAreaExplicitGhosts) is used. This is *recommended*
unless speed requirements cause you to use Voronoi areas. For speedy background
estimation you could also consider using GridMedianBackgroundEstimator.  

C++ includes: fastjet/tools/JetMedianBackgroundEstimator.hh
";

/*
 constructors and destructors 
*/

/*
 setting a new event 
*/

/*
 retrieving fundamental information 
*/

/*
 retrieving additional useful information 
*/

/*
 configuring behaviour 
*/

/*
 description 
*/

%feature("docstring") fastjet::JetMedianBackgroundEstimator::set_jets "

(re)set the jets (which must have area support) to be used by future calls to
rho() etc.  

; for the conditions that must be satisfied by the jets, see the Constructor
that takes jets.  
";

%feature("docstring") fastjet::JetMedianBackgroundEstimator::mean_area "

Returns the mean area of the jets used to actually compute the background
properties in the last call of rho() or sigma() If the configuration has changed
in the meantime, throw an error.  
";

%feature("docstring") fastjet::JetMedianBackgroundEstimator::rho_m "

returns rho_m, the purely longitudinal, particle-mass-induced component of the
background density per unit area  
";

%feature("docstring") fastjet::JetMedianBackgroundEstimator::rho_m "

Returns rho_m locally at the jet position. As for rho(jet), it is non-const.  
";

%feature("docstring") fastjet::JetMedianBackgroundEstimator::set_rescaling_class "

Set a pointer to a class that calculates the rescaling factor as a function of
the jet (position).  

Note that the rescaling factor is used both in the determination of the
\"global\" rho (the pt/A of each jet is divided by this factor) and when asking
for a local rho (the result is multiplied by this factor).  

The BackgroundRescalingYPolynomial class can be used to get a rescaling that
depends just on rapidity.  
";

%feature("docstring") fastjet::JetMedianBackgroundEstimator::set_compute_rho_m "

determine whether the automatic calculation of rho_m and sigma_m is enabled (by
default true)  
";

%feature("docstring") fastjet::JetMedianBackgroundEstimator::set_particles "

tell the background estimator that it has a new event, composed of the specified
particles.  
";

%feature("docstring") fastjet::JetMedianBackgroundEstimator::set_use_area_4vector "

By default when calculating pt/Area for a jet, it is the transverse component of
the 4-vector area that is used in the ratiof $p_t/A$.  

Calling this function with a \"false\" argument causes the scalar area to be
used instead.  

While the difference between the two choices is usually small, for high-
precision work it is usually the 4-vector area that is to be preferred.  

Parameters
----------
* `use_it` :  
    whether one uses the 4-vector area or not (true by default)  
";

%feature("docstring") fastjet::JetMedianBackgroundEstimator::set_provide_fj2_sigma "

The FastJet v2.X sigma calculation had a small spurious offset in the limit of a
small number of jets.  

This is fixed by default in versions 3 upwards. The old behaviour can be
obtained with a call to this function.  
";

%feature("docstring") fastjet::JetMedianBackgroundEstimator::reset "

Resets the class to its default state, including the choice to use 4-vector
areas.  
";

%feature("docstring") fastjet::JetMedianBackgroundEstimator::jets_used "

returns the jets used to actually compute the background properties  
";

%feature("docstring") fastjet::JetMedianBackgroundEstimator::~JetMedianBackgroundEstimator "

default dtor  
";

%feature("docstring") fastjet::JetMedianBackgroundEstimator::has_sigma "

returns true if this background estimator has support for determination of sigma  
";

%feature("docstring") fastjet::JetMedianBackgroundEstimator::set_selector "

(re)set the selector to be used for future calls to rho() etc.  
";

%feature("docstring") fastjet::JetMedianBackgroundEstimator::has_rho_m "

Returns true if this background estimator has support for determination of
rho_m.  

In te presence of a density class, support for rho_m is automatically disabled  

Note that support for sigma_m is automatic is one has sigma and rho_m support.  
";

%feature("docstring") fastjet::JetMedianBackgroundEstimator::set_cluster_sequence "

(re)set the cluster sequence (with area support) to be used by future calls to
rho() etc.  

Parameters
----------
* `csa` :  
    the cluster sequence area  

Pre-conditions:  

*   one should be able to estimate the \"empty area\" (i.e. the area not
    occupied by jets). This is feasible if at least one of the following
    conditions is satisfied: ( i) the ClusterSequence has explicit ghosts (ii)
    the range selected has a computable area.  
*   the jet algorithm must be suited for median computation (otherwise a warning
    will be issues)  

Note that selectors with e.g. hardest-jets exclusion do not have a well-defined
area. For this reasons, it is STRONGLY advised to use an area with explicit
ghosts.  
";

%feature("docstring") fastjet::JetMedianBackgroundEstimator::set_jet_density_class "

Set a pointer to a class that calculates the quantity whose median will be
calculated; if the pointer is null then pt/area is used (as occurs also if this
function is not called).  

Note that this is still *preliminary* in FastJet 3.0 and that backward
compatibility is not guaranteed in future releases of FastJet  
";

%feature("docstring") fastjet::JetMedianBackgroundEstimator::rho "

get rho, the median background density per unit area  
";

%feature("docstring") fastjet::JetMedianBackgroundEstimator::rho "

get rho, the median background density per unit area, locally at the position of
a given jet.  

If the Selector associated with the range takes a reference jet (i.e. is
relocatable), then for subsequent operations the Selector has that jet set as
its reference.  
";

%feature("docstring") fastjet::JetMedianBackgroundEstimator::JetMedianBackgroundEstimator "

Constructor that sets the rho range as well as the jet definition and area
definition to be used to cluster the particles.  

Prior to the estimation of rho, one has to provide the particles to cluster
using set_particles(...)  

Parameters
----------
* `rho_range` :  
    the Selector specifying which jets will be considered  
* `jet_def` :  
    the jet definition to use for the clustering  
* `area_def` :  
    the area definition to use for the clustering  
";

%feature("docstring") fastjet::JetMedianBackgroundEstimator::JetMedianBackgroundEstimator "

ctor from a ClusterSequenceAreaBase with area  

Parameters
----------
* `rho_range` :  
    the Selector specifying which jets will be considered  
* `csa` :  
    the ClusterSequenceArea to use  

Pre-conditions:  

*   one should be able to estimate the \"empty area\" (i.e. the area not
    occupied by jets). This is feasible if at least one of the following
    conditions is satisfied: ( i) the ClusterSequence has explicit ghosts (ii)
    the range has a computable area.  
*   the jet algorithm must be suited for median computation (otherwise a warning
    will be issues)  

Note that selectors with e.g. hardest-jets exclusion do not have a well-defined
area. For this reasons, it is STRONGLY advised to use an area with explicit
ghosts.  
";

%feature("docstring") fastjet::JetMedianBackgroundEstimator::JetMedianBackgroundEstimator "

Default constructor that optionally sets the rho range.  

The configuration must be done later calling set_cluster_sequence(...) or
set_jets(...).  

Parameters
----------
* `rho_range` :  
    the Selector specifying which jets will be considered  
";

%feature("docstring") fastjet::JetMedianBackgroundEstimator::sigma "

get sigma, the background fluctuations per unit area  
";

%feature("docstring") fastjet::JetMedianBackgroundEstimator::sigma "

get sigma, the background fluctuations per unit area, locally at the position of
a given jet.  

If the Selector associated with the range takes a reference jet (i.e. is
relocatable), then for subsequent operations the Selector has that jet set as
its reference.  
";

%feature("docstring") fastjet::JetMedianBackgroundEstimator::empty_area "

Returns the estimate of the area (within the range defined by the selector) that
is not occupied by jets.  

The value is that for the last call of rho() or sigma() If the configuration has
changed in the meantime, throw an error.  

The answer is defined to be zero if the area calculation involved explicit
ghosts; if the area calculation was an active area, then use is made of the
active area's internal list of pure ghost jets (taking those that pass the
selector); otherwise it is based on the difference between the selector's total
area and the area of the jets that pass the selector.  

The result here is just the cached result of the corresponding call to the
ClusterSequenceAreaBase function.  
";

%feature("docstring") fastjet::JetMedianBackgroundEstimator::sigma_m "

returns sigma_m, a measure of the fluctuations in the purely longitudinal,
particle-mass-induced component of the background density per unit area; must be
multipled by sqrt(area) to get fluctuations for a region of a given area.  
";

%feature("docstring") fastjet::JetMedianBackgroundEstimator::sigma_m "

Returns sigma_m locally at the jet position. As for rho(jet), it is non-const.  
";

%feature("docstring") fastjet::JetMedianBackgroundEstimator::n_jets_used "

returns the number of jets used to actually compute the background properties in
the last call of rho() or sigma() If the configuration has changed in the
meantime, throw an error.  
";

%feature("docstring") fastjet::JetMedianBackgroundEstimator::description "

returns a textual description of the background estimator  
";

%feature("docstring") fastjet::JetMedianBackgroundEstimator::n_empty_jets "

Returns the number of empty jets used when computing the background properties.  

The value is that for the last call of rho() or sigma(). If the configuration
has changed in the meantime, throw an error.  

If the area has explicit ghosts the result is zero; for active areas it is the
number of internal pure ghost jets that pass the selector; otherwise it is
deduced from the empty area, divided by $ 0.55 \\pi R^2 $ (the average pure-
ghost-jet area).  

The result here is just the cached result of the corresponding call to the
ClusterSequenceAreaBase function.  
";

%feature("docstring") fastjet::JetMedianBackgroundEstimator::jet_density_class "

return the pointer to the jet density class  
";

%feature("docstring") fastjet::JetMedianBackgroundEstimator::use_area_4vector "

check if the estimator uses the 4-vector area or the scalar area  
";

// File: classfastjet_1_1atlas_1_1JetSorter__E.xml


%feature("docstring") fastjet::atlas::JetSorter_E "
";

// File: classfastjet_1_1atlas_1_1JetSorter__Et.xml


%feature("docstring") fastjet::atlas::JetSorter_Et "
";

// File: classfastjet_1_1atlas_1_1JetSorter__Eta.xml


%feature("docstring") fastjet::atlas::JetSorter_Eta "
";

// File: classfastjet_1_1atlas_1_1JetSorter__Pt.xml


%feature("docstring") fastjet::atlas::JetSorter_Pt "
";

// File: classfastjet_1_1atlas_1_1JetSplitMergeTool.xml


%feature("docstring") fastjet::atlas::JetSplitMergeTool "
";

%feature("docstring") fastjet::atlas::JetSplitMergeTool::JetSplitMergeTool "
";

%feature("docstring") fastjet::atlas::JetSplitMergeTool::~JetSplitMergeTool "
";

%feature("docstring") fastjet::atlas::JetSplitMergeTool::etaTrue "
";

%feature("docstring") fastjet::atlas::JetSplitMergeTool::phiTrue "
";

%feature("docstring") fastjet::atlas::JetSplitMergeTool::split_merge "
";

%feature("docstring") fastjet::atlas::JetSplitMergeTool::execute "
";

// File: classfastjet_1_1JHTopTagger.xml


%feature("docstring") fastjet::JHTopTagger "

Class that helps perform boosted top tagging using the \"Johns Hopkins\" method
from arXiv:0806.0848 (Kaplan, Rehermann, Schwartz and Tweedie)  

The tagger proceeds as follows:  

*   start from a jet J obtained with the Cambridge/Aachen algorithm  
*   undo the last iteration j -> j_1,j_2 (with pt_1>pt_2) until the two subjets
    satisfy pt_1 > delta_p pt_J (with pt_J the pt of the original jet) and |y_1
    - y_2| + |phi_1 - phi_2| > delta_r.  
*   if one of these criteria is not satisfied, carry on the procedure with j_1
    (discarding j_2)  
*   for each of the subjets found, repeat the procedure. If some new
    substructure is found, keep these 2 new subjets, otherwise keep the original
    subjet (found during the first iteration)  
*   at this stage, one has at most 4 subjets. If one has less than 3, the tagger
    has failed.  
*   reconstruct the W from the 2 subjets with a mass closest to the W mass  
*   impose that the W helicity angle be less than a threshold cos_theta_W_max.  
Input conditions
*   the original jet must have an associated (and valid) ClusterSequence  
*   the tagger is designed to work with jets formed by the Cambridge/Aachen
    (C/A) algorithm; if a non-C/A jet is passed to the tagger, a warning will be
    issued  
Example
A JHTopTagger can be used as follows:  


The full set of information available from the structure_of<JHTopTagger>() call
is  

*   PseudoJet W() : the W subjet of the top candidate  
*   PseudoJet non_W(): non-W subjet(s) of the top candidate (i.e. the b)  
*   double cos_theta_W(): the W helicity angle  
*   PseudoJet W1(): the harder of the two prongs of the W  
*   PseudoJet W2(): the softer of the two prongs of the W  

The structure of the top_candidate can also be accessed through its pieces()
function:  

*   top_candidate.pieces()[0]: W  
*   top_candidate.pieces()[1]: non_W  

The W itself has two pieces (corresponding to W1, W2).  

The existence of the first two of the structural calls (W(), non_W()) and the
fact that the top is made of two pieces (W, non_W) are features that should be
common to all taggers derived from TopTaggerBase.  

See also 13 - boosted top tagging for a full usage example.  

C++ includes: fastjet/tools/JHTopTagger.hh
";

%feature("docstring") fastjet::JHTopTagger::description "

returns a textual description of the tagger  
";

%feature("docstring") fastjet::JHTopTagger::JHTopTagger "

default ctor The parameters are the following:  

Parameters
----------
* `delta_p` :  
    fractional pt cut imposed on the subjets (computed as a fraction of the
    original jet)  
* `delta_r` :  
    minimal distance between 2 subjets (computed as |y1-y2|+|phi1-phi2|)  
* `cos_theta_W_max` :  
    the maximal value for the polarisation angle of the W  
* `mW` :  
    the W mass  

The default values of all these parameters are taken from arXiv:0806:0848  
";

%feature("docstring") fastjet::JHTopTagger::result "

runs the tagger on the given jet and returns the tagged PseudoJet if successful,
or a PseudoJet==0 otherwise (standard access is through operator()).  

Parameters
----------
* `jet` :  
    the PseudoJet to tag  
";

// File: classfastjet_1_1JHTopTaggerStructure.xml


%feature("docstring") fastjet::JHTopTaggerStructure "

the structure returned by the JHTopTagger transformer.  

See the JHTopTagger class description for the details of what is inside this
structure  

C++ includes: fastjet/tools/JHTopTagger.hh
";

%feature("docstring") fastjet::JHTopTaggerStructure::W "

returns the W subjet  
";

%feature("docstring") fastjet::JHTopTaggerStructure::JHTopTaggerStructure "

ctor with pieces initialisation  
";

%feature("docstring") fastjet::JHTopTaggerStructure::W2 "

returns the second W subjet  
";

%feature("docstring") fastjet::JHTopTaggerStructure::W1 "

returns the first W subjet (the harder)  
";

%feature("docstring") fastjet::JHTopTaggerStructure::non_W "

returns the non-W subjet It will have 1 or 2 pieces depending on whether the
tagger has found 3 or 4 pieces  
";

%feature("docstring") fastjet::JHTopTaggerStructure::cos_theta_W "

returns the W helicity angle  
";

// File: structfastjet_1_1K.xml


%feature("docstring") fastjet::K "
";

// File: classfastjet_1_1LazyTiling25.xml


%feature("docstring") fastjet::LazyTiling25 "
";

%feature("docstring") fastjet::LazyTiling25::run "
";

%feature("docstring") fastjet::LazyTiling25::LazyTiling25 "
";

// File: classfastjet_1_1LazyTiling9.xml


%feature("docstring") fastjet::LazyTiling9 "
";

%feature("docstring") fastjet::LazyTiling9::run "
";

%feature("docstring") fastjet::LazyTiling9::LazyTiling9 "
";

// File: classfastjet_1_1LazyTiling9Alt.xml


%feature("docstring") fastjet::LazyTiling9Alt "
";

%feature("docstring") fastjet::LazyTiling9Alt::LazyTiling9Alt "
";

%feature("docstring") fastjet::LazyTiling9Alt::run "
";

// File: classfastjet_1_1LazyTiling9SeparateGhosts.xml


%feature("docstring") fastjet::LazyTiling9SeparateGhosts "
";

%feature("docstring") fastjet::LazyTiling9SeparateGhosts::LazyTiling9SeparateGhosts "
";

%feature("docstring") fastjet::LazyTiling9SeparateGhosts::run "
";

// File: classfastjet_1_1LimitedWarning.xml


%feature("docstring") fastjet::LimitedWarning "

class to provide facilities for giving warnings up to some maximum number of
times and to provide global summaries of warnings that have been issued.  

C++ includes: fastjet/LimitedWarning.hh
";

%feature("docstring") fastjet::LimitedWarning::LimitedWarning "

constructor that provides a default maximum number of warnings  
";

%feature("docstring") fastjet::LimitedWarning::LimitedWarning "

constructor that provides a user-set max number of warnings  
";

%feature("docstring") fastjet::LimitedWarning::max_warn "

the maximum number of warning messages that will be printed by this instance of
the class  
";

%feature("docstring") fastjet::LimitedWarning::warn "

outputs a warning to standard error (or the user's default warning stream if
set)  
";

%feature("docstring") fastjet::LimitedWarning::warn "

outputs a warning to standard error (or the user's default warning stream if
set)  
";

%feature("docstring") fastjet::LimitedWarning::warn "

outputs a warning to the specified stream  
";

%feature("docstring") fastjet::LimitedWarning::warn "

outputs a warning to the specified stream  
";

%feature("docstring") fastjet::LimitedWarning::set_default_stream "

sets the default output stream for all warnings (by default cerr; passing a null
pointer prevents warnings from being output)  
";

%feature("docstring") fastjet::LimitedWarning::set_default_max_warn "

sets the default maximum number of warnings of a given kind before warning
messages are silenced.  
";

%feature("docstring") fastjet::LimitedWarning::summary "

returns a summary of all the warnings that came through the LimiteWarning class  
";

%feature("docstring") fastjet::LimitedWarning::n_warn_so_far "

the number of times so far that a warning has been registered with this instance
of the class.  
";

// File: classfastjet_1_1atlas_1_1LorentzVector.xml


%feature("docstring") fastjet::atlas::LorentzVector "
";

%feature("docstring") fastjet::atlas::LorentzVector::phi "
";

%feature("docstring") fastjet::atlas::LorentzVector::et "
";

%feature("docstring") fastjet::atlas::LorentzVector::pt "
";

%feature("docstring") fastjet::atlas::LorentzVector::mt "
";

%feature("docstring") fastjet::atlas::LorentzVector::add "
";

%feature("docstring") fastjet::atlas::LorentzVector::isEqual "
";

%feature("docstring") fastjet::atlas::LorentzVector::subtract "
";

%feature("docstring") fastjet::atlas::LorentzVector::e "
";

%feature("docstring") fastjet::atlas::LorentzVector::y "
";

%feature("docstring") fastjet::atlas::LorentzVector::eta "
";

%feature("docstring") fastjet::atlas::LorentzVector::p "
";

%feature("docstring") fastjet::atlas::LorentzVector::LorentzVector "
";

%feature("docstring") fastjet::atlas::LorentzVector::LorentzVector "
";

%feature("docstring") fastjet::atlas::LorentzVector::LorentzVector "
";

%feature("docstring") fastjet::atlas::LorentzVector::Et "
";

// File: classfastjet_1_1MassDropTagger.xml


%feature("docstring") fastjet::MassDropTagger "

Class that helps perform 2-pronged boosted tagging using the \"mass-drop\"
technique (with asymmetry cut) introduced by Jonathan Butterworth, Adam Davison,
Mathieu Rubin and Gavin Salam in arXiv:0802.2470 in the context of a boosted
Higgs search.  

The tagger proceeds as follows:  

0. start from a jet obtained from with the Cambridge/Aachen algorithm  

1.  undo the last step of the clustering step j -> j1 + j2 (label them such as
    j1 is the most massive).  
2.  if there is a mass drop, i.e. m_j1/m_j < mu_cut, and the splitting is
    sufficiently symmetric, ${\\rm min}(p_{tj1}^2,p_{tj2}^2)\\Delta R_{j1,j2}^2
    > y_{\\rm cut} m_j^2$, keep j as the result of the tagger (with j1 and j2
    its 2 subjets)  
3.  otherwise, redefine j to be equal to j1 and return to step 1.  

Note that in the original proposal, j1 and j2 are both required to be b-tagged
and a filter (with Rfilt=min(0.3,Rbb/2) and n_filt=3) is also applied to j to
obtain the final \"Higgs candidate\". See the example 12 - boosted Higgs tagging
for details.  
Options
The constructor has the following arguments:  

*   The first argument is the minimal mass drop that is required (mu_cut) [0.67
    by default]  
*   The second argument is the asymmetry cut (y_cut) [0.09 by default]  
Input conditions
*   one must be able to successively \"uncluster\" the original jet using
    \"has_parents\"  
Output/structure
*   the 2 subjets are kept as pieces if some substructure is found, otherwise a
    single 0-momentum piece is returned  
*   the 'mu' and 'y' values corresponding to the unclustering step that passed
    the tagger's cuts  

See also 12 - boosted Higgs tagging for a usage example.  

C++ includes: fastjet/tools/MassDropTagger.hh
";

%feature("docstring") fastjet::MassDropTagger::description "

returns a textual description of the tagger  
";

%feature("docstring") fastjet::MassDropTagger::MassDropTagger "

default ctor  
";

%feature("docstring") fastjet::MassDropTagger::result "

runs the tagger on the given jet and returns the tagged PseudoJet if successful,
a PseudoJet==0 otherwise (standard access is through operator()).  

Parameters
----------
* `jet` :  
    the PseudoJet to tag  
";

// File: classfastjet_1_1MassDropTaggerStructure.xml


%feature("docstring") fastjet::MassDropTaggerStructure "

the structure returned by the MassDropTagger transformer.  

See the MassDropTagger class description for the details of what is inside this
structure  

C++ includes: fastjet/tools/MassDropTagger.hh
";

%feature("docstring") fastjet::MassDropTaggerStructure::MassDropTaggerStructure "

ctor with initialisation  

Parameters
----------
* `pieces` :  
    the pieces of the created jet  
* `rec` :  
    the recombiner from the underlying cluster sequence  
";

%feature("docstring") fastjet::MassDropTaggerStructure::y "

returns the value of y = (squared kt distance) / (squared mass) for the
splitting that triggered the mass-drop condition  
";

%feature("docstring") fastjet::MassDropTaggerStructure::mu "

returns the mass-drop ratio, pieces[0].m()/jet.m(), for the splitting that
triggered the mass-drop condition  
";

// File: classfastjet_1_1MinHeap.xml


%feature("docstring") fastjet::MinHeap "
";

%feature("docstring") fastjet::MinHeap::remove "

remove the value at the specified location (i.e.  

replace it with the largest possible value).  
";

%feature("docstring") fastjet::MinHeap::minloc "

return the location of the minimal value on the heap  
";

%feature("docstring") fastjet::MinHeap::update "

update the value at the specified location  
";

%feature("docstring") fastjet::MinHeap::initialise "

initialise the heap with the supplied values.  

construct the MinHeap; structure will be as follows: Should only be called if
the constructor did not supply values.  

_heap[0].minloc points to globally smallest entry _heap[1].minloc points to
smallest entry in one half of heap _heap[2].minloc points to smallest entry in
other half of heap  

. for _heap[i], the \"parent\" is to be found at (i-1)/2  
";

%feature("docstring") fastjet::MinHeap::MinHeap "

construct a MinHeap from the vector of values, allowing future expansion to a
maximum size max_size;  
";

%feature("docstring") fastjet::MinHeap::MinHeap "

do the minimal setup for a MinHeap that can reach max_size; initialisation must
be performed later with the actual values.  
";

%feature("docstring") fastjet::MinHeap::MinHeap "

constructor in which the the maximum size is the size of the values array  
";

%feature("docstring") fastjet::MinHeap::minval "

return the minimal value on the heap  
";

// File: classfastjet_1_1Private_1_1MirrorInfo.xml


%feature("docstring") fastjet::Private::MirrorInfo "

class for helping us deal with mirror-image particles.  
";

%feature("docstring") fastjet::Private::MirrorInfo::MirrorInfo "
";

%feature("docstring") fastjet::Private::MirrorInfo::MirrorInfo "
";

// File: structfastjet_1_1Dnn2piCylinder_1_1MirrorVertexInfo.xml

// File: structfastjet_1_1Dnn3piCylinder_1_1MirrorVertexInfo.xml

// File: classMyUserInfo.xml


%feature("docstring") MyUserInfo "
";

%feature("docstring") MyUserInfo::MyUserInfo "
";

%feature("docstring") MyUserInfo::pdg_id "

access to the PDG id  
";

%feature("docstring") MyUserInfo::vertex_number "

access to the vertex number  
";

// File: classfastjet_1_1NestedDefsPlugin.xml


%feature("docstring") fastjet::NestedDefsPlugin "

Plugin to run multiple jet definitions successively (plugin for fastjet v2.4
upwards)  

NestedAglsPlugin is a plugin for fastjet (v2.4 upwards) that, given a list of
jet definitions, performs the clustering by feeding the particles to the first
algorithm and then, successively feeding the output to the next algorithm in the
list.  

C++ includes: fastjet/NestedDefsPlugin.hh
";

%feature("docstring") fastjet::NestedDefsPlugin::description "

return a textual description of the jet-definition implemented in this plugin  
";

%feature("docstring") fastjet::NestedDefsPlugin::NestedDefsPlugin "

Main constructor for the NestedDefs Plugin class.  

The argument is an initialised list of jet algorithms  
";

%feature("docstring") fastjet::NestedDefsPlugin::NestedDefsPlugin "

copy constructor  
";

%feature("docstring") fastjet::NestedDefsPlugin::run_clustering "

given a ClusterSequence that has been filled up with initial particles, the
following function should fill up the rest of the ClusterSequence, using the
following member functions of ClusterSequence:  

*   plugin_do_ij_recombination(...)  
*   plugin_do_iB_recombination(...)  
";

%feature("docstring") fastjet::NestedDefsPlugin::R "

the plugin mechanism's standard way of accessing the jet radius here we return
the R of the last alg in the list  
";

// File: classfastjet_1_1NNBase.xml


%feature("docstring") fastjet::NNBase "

Helps solve closest pair problems with generic interparticle and particle-beam
distances.  
Description and derived classes:
This is an abstract base class which defines the interface for several classes
that help carry out nearest-neighbour clustering:  

*   NNH provides an implementation for generic measures,  
*   NNFJN2Plain provides an implementation for distances satisfying the FastJet
    lemma i.e. distances for which the minimum dij has the property that i is
    the geometrical nearest neighbour of j, or vice versa. I.e. the distance can
    be factorised in a momentum factor and a geometric piece. This is based on
    the fastjet N2Plain clustering strategy  
*   NNFJN2Tiled is a tiled version of NNFJN2Plain (based on the N2Tiled FastJet
    clustering strategy). Like NNPlain2 it applies to distance measures that
    satisfy the FastJet lemma, with the additional restriction that: (a) the
    underlying geometry should be cylindrical (e.g. rapidity--azimuth) and (b)
    the search for the geometric nearest neighbour of each particle can be
    limited to that particle's tile and its neighbouring tiles.  

If you can use NNFJN2Plain it will usually be faster than NNH. NNFJN2Tiled,
where it can be used, will be faster for multiplicities above a few tens of
particles.  

NOTE: IN ALL CASES, THE DISTANCE MUST BE SYMMETRIC (dij=dji)!!!  
Underlying BriefJet (BJ) class:
All derived classes must be templated with a BriefJet (BJ) class --- BJ should
basically cache the minimal amount of information that is needed to efficiently
calculate interparticle distances and particle-beam distances.  

This class can be used with or without an extra \"Information\" template, i.e.
`NN*<BJ>` or `NN*<BJ,I>`. Accordingly BJ must provide one of the two following
init functions:  


where info might be a pointer to a class that contains, e.g., information about
R, or other parameters of the jet algorithm  

The BJ then provides information about interparticle and particle-beam
distances. The exact requirements depend on whether you use NNH, NNFJN2Plain or
NNFJN2Tiled. (See the corresponding classes for details).  
Workflow:
In all cases, the usage of NNBase classes works as follows:  

First, from the list of particles, create an `NN*<BJ>` object of the appropriate
type with the appropriate BJ class (and optional extra info).  

Then, cluster using a loop like this (assuming a FastJet plugin)  


For an example of how the NNH<BJ> class is used, see the JadePlugin or
EECambridgePlugin.  

C++ includes: fastjet/NNBase.hh
";

%feature("docstring") fastjet::NNBase::dij_min "

returns the dij_min and indices iA, iB, for the corresponding jets.  

If iB < 0 then iA recombines with the beam  
";

%feature("docstring") fastjet::NNBase::merge_jets "

merges the jets pointed to by indices A and B and replaces them with jet,
assigning it an index jet_index.  
";

%feature("docstring") fastjet::NNBase::start "

initialisation from a given list of particles  
";

%feature("docstring") fastjet::NNBase::NNBase "

Default constructor.  
";

%feature("docstring") fastjet::NNBase::NNBase "

Constuctor with additional Info.  
";

%feature("docstring") fastjet::NNBase::remove_jet "

removes the jet pointed to by index iA  
";

%feature("docstring") fastjet::NNBase::~NNBase "
";

// File: classfastjet_1_1NNFJN2Plain_1_1NNBJ.xml

// File: classfastjet_1_1NNH_1_1NNBJ.xml

// File: classfastjet_1_1NNFJN2Plain.xml


%feature("docstring") fastjet::NNFJN2Plain "

Helps solve closest pair problems with factorised interparticle and beam
distances (ie satisfying the FastJet lemma)  

(see NNBase.hh for an introductory description)  

This variant provides an implementation based on the N2Plain clustering strategy
in FastJet. The interparticle and beam distances should be of the form  


The class is templated with a BJ (brief jet) class and can be used with or
without an extra \"Information\" template, i.e. NNFJN2Plain<BJ> or
NNFJN2Plain<BJ,I>  

For the NNH_N2Plain<BJ> version of the class to function, BJ must provide four
member functions  


For the NNH_N2Plain<BJ,I> version to function, the BJ::init(...) member must
accept an extra argument  


NOTE: THE DISTANCE MUST BE SYMMETRIC I.E. SATISFY  

Note that you are strongly advised to add the following lines to your BJ class
to allow it to be used also with NNH:  

  

C++ includes: fastjet/NNFJN2Plain.hh
";

%feature("docstring") fastjet::NNFJN2Plain::NNFJN2Plain "

constructor with an initial set of jets (which will be assigned indices
`0...jets.size()-1`)  
";

%feature("docstring") fastjet::NNFJN2Plain::NNFJN2Plain "
";

%feature("docstring") fastjet::NNFJN2Plain::start "

initialisation from a given list of particles  
";

%feature("docstring") fastjet::NNFJN2Plain::~NNFJN2Plain "

a destructor  
";

%feature("docstring") fastjet::NNFJN2Plain::merge_jets "

merges the jets pointed to by indices A and B and replace them with jet,
assigning it an index jet_index.  
";

%feature("docstring") fastjet::NNFJN2Plain::remove_jet "

removes the jet pointed to by index iA  
";

%feature("docstring") fastjet::NNFJN2Plain::dij_min "

returns the dij_min and indices iA, iB, for the corresponding jets.  

If iB < 0 then iA recombines with the beam  
";

// File: classfastjet_1_1NNFJN2Tiled.xml


%feature("docstring") fastjet::NNFJN2Tiled "

Helps solve closest pair problems with factorised interparticle and beam
distances (ie satisfying the FastJet lemma) that are on a cylindrical geometry
and allow tiling.  

(see NNBase.hh for an introductory description)  

This variant provides an implementation based on the N2Tiled clustering strategy
in FastJet. As for the NNFJN2Plain case, the interparticle and beam distances
should be of the form  


Additionally, the NNFJN2Tiled class takes a tile_size parameter that controls
the size of the tiles. It must be such that, for any two points in non-
neighbouring (and non-identical) tiles, the geometrical distance between the 2
points is larger than the geometrical beam distance of each of the 2 points.  

It is templated with a BJ (brief jet) class and can be used with or without an
extra \"Information\" template, i.e. NNFJN2Tiled<BJ> or NNFJN2Tiled<BJ,I>  

For the NNFJN2Tiled<BJ> version of the class to function, BJ must provide three
member functions  


For the NNFJN2Tiled<BJ,I> version to function, the BJ::init(...) member must
accept an extra argument  


NOTE: THE DISTANCE MUST BE SYMMETRIC I.E. SATISFY  

Finally, the BJ class needs to provide access to the variables used for the
rectangular tiling:  


Note that you are strongly advised to add the following lines to your BJ class
to allow it to be used also with NNH:  

  

C++ includes: fastjet/NNFJN2Tiled.hh
";

%feature("docstring") fastjet::NNFJN2Tiled::~NNFJN2Tiled "

a destructor  
";

%feature("docstring") fastjet::NNFJN2Tiled::start "

initialisation from a given list of particles  
";

%feature("docstring") fastjet::NNFJN2Tiled::merge_jets "

merge the jets pointed to by indices A and B and replace them with jet,
assigning it an index jet_index.  
";

%feature("docstring") fastjet::NNFJN2Tiled::NNFJN2Tiled "

constructor with an initial set of jets (which will be assigned indices
`0...jets.size()-1`)  
";

%feature("docstring") fastjet::NNFJN2Tiled::NNFJN2Tiled "
";

%feature("docstring") fastjet::NNFJN2Tiled::remove_jet "

remove the jet pointed to by index iA  
";

%feature("docstring") fastjet::NNFJN2Tiled::dij_min "

return the dij_min and indices iA, iB, for the corresponding jets.  

If iB < 0 then iA recombines with the beam  
";

// File: classfastjet_1_1NNH.xml


%feature("docstring") fastjet::NNH "

Help solve closest pair problems with generic interparticle and beam distance
(generic case)  

(see NNBase.hh for an introductory description)  

This variant provides an implementation for any distance measure. It is
templated with a BJ (brief jet) classand can be used with or without an extra
\"Information\" template, i.e. NNH<BJ> or NNH<BJ,I>  

For the NNH<BJ> version of the class to function, BJ must provide three member
functions  

*   void BJ::init(const PseudoJet & jet); // initialise with a PseudoJet  
*   double BJ::distance(const BJ * other_bj_jet); // distance between this and
    other_bj_jet  
*   double BJ::beam_distance() ; // distance to the beam  

For the NNH<BJ,I> version to function, the BJ::init(...) member must accept an
extra argument  

*   void BJ::init(const PseudoJet & jet, I * info); // initialise with a
    PseudoJet + info  

NOTE: THE DISTANCE MUST BE SYMMETRIC I.E. SATISFY a.distance(b) == b.distance(a)  

For an example of how the NNH<BJ> class is used, see the Jade (and EECambridge)
plugins  

NB: the NNH algorithm is expected N^2, but has a worst case of N^3. Many QCD
problems tend to place one closer to the N^3 end of the spectrum than one would
like. There is scope for further progress (cf Eppstein, Cardinal & Eppstein),
nevertheless the current class is already significantly faster than standard N^3
implementations.  

C++ includes: fastjet/NNH.hh
";

%feature("docstring") fastjet::NNH::dij_min "

return the dij_min and indices iA, iB, for the corresponding jets.  

If iB < 0 then iA recombines with the beam  
";

%feature("docstring") fastjet::NNH::~NNH "

a destructor  
";

%feature("docstring") fastjet::NNH::merge_jets "

merge the jets pointed to by indices A and B and replace them with jet,
assigning it an index jet_index.  
";

%feature("docstring") fastjet::NNH::NNH "

constructor with an initial set of jets (which will be assigned indices 0 ...  

jets.size()-1  
";

%feature("docstring") fastjet::NNH::NNH "
";

%feature("docstring") fastjet::NNH::remove_jet "

remove the jet pointed to by index iA  
";

%feature("docstring") fastjet::NNH::start "

initialisation from a given list of particles  
";

// File: classfastjet_1_1NNInfo.xml


%feature("docstring") fastjet::NNInfo "

internal helper template class to facilitate initialisation of a BJ with a
PseudoJet and extra information.  

Implementations of NN-based clustering do not need to explicitly use or refer to
this class!  

C++ includes: fastjet/NNBase.hh
";

%feature("docstring") fastjet::NNInfo::NNInfo "
";

%feature("docstring") fastjet::NNInfo::NNInfo "
";

%feature("docstring") fastjet::NNInfo::init_jet "
";

// File: classfastjet_1_1NNInfo_3_01__NoInfo_01_4.xml


%feature("docstring") fastjet::NNInfo< _NoInfo > "

for cases where there is no extra info  

C++ includes: fastjet/NNBase.hh
";

%feature("docstring") fastjet::NNInfo< _NoInfo >::NNInfo "
";

%feature("docstring") fastjet::NNInfo< _NoInfo >::NNInfo "
";

%feature("docstring") fastjet::NNInfo< _NoInfo >::init_jet "
";

// File: classfastjet_1_1SearchTree_1_1Node.xml


%feature("docstring") fastjet::SearchTree::Node "
";

%feature("docstring") fastjet::SearchTree::Node::nullify_treelinks "

set all the tree-related links are set to null for this node  
";

%feature("docstring") fastjet::SearchTree::Node::reset_parents_link_to_me "

if my parent exists, determine whether I am it's left or right node and set the
relevant link equal to XX.  
";

%feature("docstring") fastjet::SearchTree::Node::Node "
";

%feature("docstring") fastjet::SearchTree::Node::treelinks_null "

default constructor  

returns tree if all the tree-related links are set to null for this node  
";

// File: structfastjet_1_1cms_1_1NumericSafeGreaterByEt.xml


%feature("docstring") fastjet::cms::NumericSafeGreaterByEt "
";

// File: classfastjet_1_1JetDefinition_1_1Plugin.xml


%feature("docstring") fastjet::JetDefinition::Plugin "

a class that allows a user to introduce their own \"plugin\" jet finder  

Note that all the plugins provided with FastJet are derived from this class  

C++ includes: fastjet/JetDefinition.hh
";

%feature("docstring") fastjet::JetDefinition::Plugin::is_spherical "

returns true if the plugin implements an algorithm intended for use on a
spherical geometry (e.g.  

e+e- algorithms, as opposed to most pp algorithms, which use a cylindrical,
rapidity-phi geometry).  
";

%feature("docstring") fastjet::JetDefinition::Plugin::~Plugin "

a destructor to be replaced if necessary in derived classes...  
";

%feature("docstring") fastjet::JetDefinition::Plugin::set_ghost_separation_scale "

set the ghost separation scale for passive area determinations in future runs
(strictly speaking that makes the routine a non const, so related internal info
must be stored as a mutable)  
";

%feature("docstring") fastjet::JetDefinition::Plugin::description "

return a textual description of the jet-definition implemented in this plugin  
";

%feature("docstring") fastjet::JetDefinition::Plugin::run_clustering "

given a ClusterSequence that has been filled up with initial particles, the
following function should fill up the rest of the ClusterSequence, using the
following member functions of ClusterSequence:  

*   plugin_do_ij_recombination(...)  
*   plugin_do_iB_recombination(...)  
";

%feature("docstring") fastjet::JetDefinition::Plugin::R "
";

%feature("docstring") fastjet::JetDefinition::Plugin::exclusive_sequence_meaningful "

if this returns false then a warning will be given whenever the user requests
\"exclusive\" jets from the cluster sequence  
";

%feature("docstring") fastjet::JetDefinition::Plugin::ghost_separation_scale "
";

%feature("docstring") fastjet::JetDefinition::Plugin::supports_ghosted_passive_areas "

return true if there is specific support for the measurement of passive areas,
in the sense that areas determined from all particles below the ghost separation
scale will be a passive area.  

[If you don't understand this, ignore it!]  
";

// File: classfastjet_1_1ClosestPair2D_1_1Point.xml


%feature("docstring") fastjet::ClosestPair2D::Point "
";

%feature("docstring") fastjet::ClosestPair2D::Point::distance2 "

returns the distance between two of these objects  
";

// File: classfastjet_1_1d0_1_1ProtoJet.xml


%feature("docstring") fastjet::d0::ProtoJet "
";

%feature("docstring") fastjet::d0::ProtoJet::setJet "
";

%feature("docstring") fastjet::d0::ProtoJet::updateJet "
";

%feature("docstring") fastjet::d0::ProtoJet::phi "
";

%feature("docstring") fastjet::d0::ProtoJet::info "
";

%feature("docstring") fastjet::d0::ProtoJet::merged "
";

%feature("docstring") fastjet::d0::ProtoJet::print "
";

%feature("docstring") fastjet::d0::ProtoJet::~ProtoJet "
";

%feature("docstring") fastjet::d0::ProtoJet::LItems "
";

%feature("docstring") fastjet::d0::ProtoJet::ProtoJet "
";

%feature("docstring") fastjet::d0::ProtoJet::ProtoJet "
";

%feature("docstring") fastjet::d0::ProtoJet::ProtoJet "
";

%feature("docstring") fastjet::d0::ProtoJet::erase "
";

%feature("docstring") fastjet::d0::ProtoJet::pT "
";

%feature("docstring") fastjet::d0::ProtoJet::NowStable "
";

%feature("docstring") fastjet::d0::ProtoJet::splitted "
";

%feature("docstring") fastjet::d0::ProtoJet::y "
";

%feature("docstring") fastjet::d0::ProtoJet::addItem "
";

// File: classfastjet_1_1d0_1_1ProtoJet__ET__seedET__order.xml


%feature("docstring") fastjet::d0::ProtoJet_ET_seedET_order "
";

// File: classfastjet_1_1Pruner.xml


%feature("docstring") fastjet::Pruner "

Transformer that prunes a jet.  

This transformer prunes a jet according to the ideas presented in
arXiv:0903.5081 (S.D. Ellis, C.K. Vermilion and J.R. Walsh).  

The jet's constituents are reclustered with a user-specified jet definition,
with the modification that objects i and j are only recombined if at least one
of the following two criteria is satisfied:  

*   the geometric distance between i and j is smaller than 'Rcut' with Rcut =
    Rcut_factor*2m/pt (Rcut_factor is a parameter of the Pruner and m and pt
    obtained from the jet being pruned)  
*   the transverse momenta of i and j are at least 'zcut' p_t(i+j)  

If both these criteria fail, i and j are not recombined, the harder of i and j
is kept, and the softer is rejected.  

Usage:  

The pruned_jet has a valid associated cluster sequence. In addition the subjets
of the original jet that have been vetoed by pruning (i.e. have been 'pruned
away') can be accessed using  


If the re-clustering happens to find more than a single inclusive jet (this
should normally not happen if the radius of the jet definition used for the
reclustering was set large enough), the hardest of these jets is retured as the
result of the Pruner. The other jets can be accessed through  


Instead of using Rcut_factor and zcut, one can alternatively construct a Pruner
by passing two (pointers to) functions of PseudoJet that dynamically compute the
Rcut and zcut to be used for the jet being pruned.  

When the jet being pruned has area support and explicit ghosts, the resulting
pruned jet will likewise have area.  

C++ includes: fastjet/tools/Pruner.hh
";

%feature("docstring") fastjet::Pruner::Pruner "

minimal constructor, which takes a jet algorithm, sets the radius to
JetDefinition::max_allowable_R (practically equivalent to infinity) and also
tries to use a recombiner based on the one in the jet definition of the
particular jet being pruned.  

Parameters
----------
* `jet_alg` :  
    the jet algorithm for the internal clustering  
* `zcut` :  
    pt-fraction cut in the pruning  
* `Rcut_factor` :  
    the angular distance cut in the pruning will be Rcut_factor * 2m/pt  
";

%feature("docstring") fastjet::Pruner::Pruner "

alternative ctor in which the full reclustering jet definition can be specified.  

Parameters
----------
* `jet_def` :  
    the jet definition for the internal clustering  
* `zcut` :  
    pt-fraction cut in the pruning  
* `Rcut_factor` :  
    the angular distance cut in the pruning will be Rcut_factor * 2m/pt  
";

%feature("docstring") fastjet::Pruner::Pruner "

alternative ctor in which the pt-fraction cut and angular distance cut are
functions of the jet being pruned.  

Parameters
----------
* `jet_def` :  
    the jet definition for the internal clustering  
* `zcut_dyn` :  
    dynamic pt-fraction cut in the pruning  
* `Rcut_dyn` :  
    dynamic angular distance cut in the pruning  
";

%feature("docstring") fastjet::Pruner::result "

action on a single jet  
";

%feature("docstring") fastjet::Pruner::description "

description  
";

// File: classfastjet_1_1PrunerStructure.xml


%feature("docstring") fastjet::PrunerStructure "

The structure associated with a PseudoJet thas has gone through a Pruner
transformer.  

C++ includes: fastjet/tools/Pruner.hh
";

%feature("docstring") fastjet::PrunerStructure::rejected "

return the constituents that have been rejected  
";

%feature("docstring") fastjet::PrunerStructure::extra_jets "

return the other jets that may have been found along with the result of the
pruning The resulting vector is sorted in pt  
";

%feature("docstring") fastjet::PrunerStructure::description "

description  
";

%feature("docstring") fastjet::PrunerStructure::PrunerStructure "

default ctor  

Parameters
----------
* `result_jet` :  
    the jet for which we have to keep the structure  
";

%feature("docstring") fastjet::PrunerStructure::zcut "

return the value of Rcut that was used for this specific pruning.  
";

%feature("docstring") fastjet::PrunerStructure::Rcut "

return the value of Rcut that was used for this specific pruning.  
";

// File: classfastjet_1_1PruningPlugin.xml


%feature("docstring") fastjet::PruningPlugin "
";

%feature("docstring") fastjet::PruningPlugin::R "

returns the radius  
";

%feature("docstring") fastjet::PruningPlugin::PruningPlugin "

ctor  

Parameters
----------
* `jet_def` :  
    the jet definition to be used for the internal clustering  
* `zcut` :  
    transverse momentum fraction cut  
* `Rcut` :  
    angular separation cut  
";

%feature("docstring") fastjet::PruningPlugin::run_clustering "

the actual clustering work for the plugin  
";

%feature("docstring") fastjet::PruningPlugin::description "

description of the plugin  
";

// File: classfastjet_1_1PruningRecombiner.xml


%feature("docstring") fastjet::PruningRecombiner "
";

%feature("docstring") fastjet::PruningRecombiner::PruningRecombiner "

ctor  

Parameters
----------
* `zcut` :  
    transverse momentum fraction cut  
* `Rcut` :  
    angular separation cut  
* `recomb` :  
    pointer to a recombiner to use to cluster pairs  
";

%feature("docstring") fastjet::PruningRecombiner::description "

returns the description of the recombiner  
";

%feature("docstring") fastjet::PruningRecombiner::rejected "

return the history indices that have been pruned away  
";

%feature("docstring") fastjet::PruningRecombiner::clear_rejected "

clears the list of rejected indices  

If one decides to use this recombiner standalone, one has to call this after
each clustering in order for the rejected() vector to remain sensible and not
grow to infinite size.  
";

%feature("docstring") fastjet::PruningRecombiner::recombine "

perform a recombination taking into account the pruning conditions  
";

// File: classfastjet_1_1PseudoJet.xml


%feature("docstring") fastjet::PseudoJet "

Class to contain pseudojets, including minimal information of use to jet-
clustering routines.  

C++ includes: fastjet/PseudoJet.hh
";

/*
 Constructors and destructor 
*/

/*
 Kinematic access functions 
*/

/*
 Kinematic modification functions 
*/

/*
 User index functions 
*/

/*
To allow the user to set and access an integer index which can be exploited by
the user to associate extra information with a particle/jet (for example pdg id,
or an indication of a particle's origin within the user's analysis)  

*/

/*
 User information types and functions 
*/

/*
Allows PseudoJet to carry extra user info (as an object derived from
UserInfoBase).  

*/

/*
 Description 
*/

/*
Since a PseudoJet can have a structure that contains a variety of information,
we provide a description that allows one to check exactly what kind of PseudoJet
we are dealing with  

*/

/*
 Access to the associated ClusterSequence object. 
*/

/*
In addition to having kinematic information, jets may contain a reference to an
associated ClusterSequence (this is the case, for example, if the jet has been
returned by a ClusterSequence member function).  

*/

/*
 Access to the associated PseudoJetStructureBase object. 
*/

/*
In addition to having kinematic information, jets may contain a reference to an
associated ClusterSequence (this is the case, for example, if the jet has been
returned by a ClusterSequence member function).  

*/

/*
 Methods for access to information about jet structure 
*/

/*
These allow access to jet constituents, and other jet subtructure information.  

They only work if the jet is associated with a ClusterSequence.  

*/

/*
 Members mainly intended for internal use 
*/

%feature("docstring") fastjet::PseudoJet::validated_cs "

shorthand for validated_cluster_sequence()  
";

%feature("docstring") fastjet::PseudoJet::four_mom "

return a valarray containing the four-momentum (components 0-2 are 3-mom,
component 3 is energy).  
";

%feature("docstring") fastjet::PseudoJet::cluster_sequence_history_index "

alternative name for cluster_hist_index() [perhaps more meaningful]  
";

%feature("docstring") fastjet::PseudoJet::set_user_info "

sets the internal shared pointer to the user information.  

Note that the PseudoJet will now *own* the pointer, and delete the corresponding
object when it (the jet, and any copies of the jet) goes out of scope.  
";

%feature("docstring") fastjet::PseudoJet::has_partner "

check if it has been recombined with another PseudoJet in which case, return its
partner through the argument.  

Otherwise, 'partner' is set to 0.  

an Error is thrown if this PseudoJet has no currently valid associated
ClusterSequence  
";

%feature("docstring") fastjet::PseudoJet::phi_02pi "

returns phi in the range 0..2pi  
";

%feature("docstring") fastjet::PseudoJet::e "
";

%feature("docstring") fastjet::PseudoJet::m "

returns the invariant mass (If m2() is negative then -sqrt(-m2()) is returned,
as in CLHEP)  
";

%feature("docstring") fastjet::PseudoJet::kt_distance "

returns kt distance (R=1) between this jet and another  
";

%feature("docstring") fastjet::PseudoJet::modp2 "

return the squared 3-vector modulus = px^2+py^2+pz^2  
";

%feature("docstring") fastjet::PseudoJet::area_4vector "

return the jet 4-vector area.  

throws an Error if there is no support for area in the parent CS  
";

%feature("docstring") fastjet::PseudoJet::E "
";

%feature("docstring") fastjet::PseudoJet::validated_cluster_sequence "

if the jet has a valid associated cluster sequence then return a pointer to it;
otherwise throw an error  
";

%feature("docstring") fastjet::PseudoJet::phi_std "

returns phi in the range -pi..pi  
";

%feature("docstring") fastjet::PseudoJet::exclusive_subjets "

return a vector of all subjets of the current jet (in the sense of the exclusive
algorithm) that would be obtained when running the algorithm with the given
dcut.  

Time taken is O(m ln m), where m is the number of subjets that are found. If m
gets to be of order of the total number of constituents in the jet, this could
be substantially slower than just getting that list of constituents.  

an Error is thrown if this PseudoJet has no currently valid associated
ClusterSequence  
";

%feature("docstring") fastjet::PseudoJet::exclusive_subjets "

return the list of subjets obtained by unclustering the supplied jet down to
nsub subjets.  

Throws an error if there are fewer than nsub particles in the jet.  

For ClusterSequence type jets, requires nsub ln nsub time  

An Error is thrown if this PseudoJet has no currently valid associated
ClusterSequence  
";

%feature("docstring") fastjet::PseudoJet::Et "

return the transverse energy  
";

%feature("docstring") fastjet::PseudoJet::structure_ptr "

return a pointer to the structure (of type PseudoJetStructureBase*) associated
with this PseudoJet.  

return NULL if there is no associated structure  
";

%feature("docstring") fastjet::PseudoJet::phi "

returns phi (in the range 0..2pi)  
";

%feature("docstring") fastjet::PseudoJet::exclusive_subdmerge_max "

Returns the maximum dij that occurred in the whole event at the stage that the
nsub+1 -> nsub merge of subjets occurred inside this jet.  

Returns 0 if there were nsub or fewer constituents in the jet.  

an Error is thrown if this PseudoJet has no currently valid associated
ClusterSequence  
";

%feature("docstring") fastjet::PseudoJet::kt2 "

returns the squared transverse momentum  
";

%feature("docstring") fastjet::PseudoJet::structure_of "

this is a helper to access any structure created by a Transformer (that is, of
type Transformer::StructureType).  

If there is no structure, or if the structure is not compatible with
TransformerType, an error is thrown.  
";

%feature("docstring") fastjet::PseudoJet::validated_csab "

shorthand for validated_cluster_sequence_area_base()  
";

%feature("docstring") fastjet::PseudoJet::PseudoJet "

default constructor, which as of FJ3.0 provides an object for which all
operations are now valid and which has zero momentum  
";

%feature("docstring") fastjet::PseudoJet::PseudoJet "

construct a pseudojet from explicit components  
";

%feature("docstring") fastjet::PseudoJet::PseudoJet "

constructor from any object that has px,py,pz,E = some_four_vector[0--3],  
";

%feature("docstring") fastjet::PseudoJet::PseudoJet "
";

%feature("docstring") fastjet::PseudoJet::PseudoJet "

shortcut for converting siscone Cmomentum into PseudoJet  
";

%feature("docstring") fastjet::PseudoJet::PseudoJet "

shortcut for converting siscone CSphmomentum into PseudoJet  
";

%feature("docstring") fastjet::PseudoJet::has_associated_cs "

shorthand for has_associated_cluster_sequence()  
";

%feature("docstring") fastjet::PseudoJet::~PseudoJet "

default (virtual) destructor  
";

%feature("docstring") fastjet::PseudoJet::has_parents "

check if it is the product of a recombination, in which case return the 2
parents through the 'parent1' and 'parent2' arguments.  

Otherwise, set these to 0.  

an Error is thrown if this PseudoJet has no currently valid associated
ClusterSequence  
";

%feature("docstring") fastjet::PseudoJet::associated_cs "
";

%feature("docstring") fastjet::PseudoJet::pieces "

retrieve the pieces that make up the jet.  

If the jet does not support pieces, an error is throw  
";

%feature("docstring") fastjet::PseudoJet::delta_phi_to "

returns other.phi() - this.phi(), constrained to be in range -pi .  

returns other.phi() - this.phi(), i.e.  

. pi  

the phi distance to other, constrained to be in range -pi .. pi  
";

%feature("docstring") fastjet::PseudoJet::reset "

reset the 4-momentum according to the supplied components and put the user and
history indices back to their default values  
";

%feature("docstring") fastjet::PseudoJet::reset "

reset the PseudoJet to be equal to psjet (including its indices); NB if the
argument is derived from a PseudoJet then the \"reset\" used will be the
templated version  

Note: this is included on top of the templated version because PseudoJet is not
\"derived\" from PseudoJet, so the templated reset would not handle this case
properly.  
";

%feature("docstring") fastjet::PseudoJet::reset "

reset the 4-momentum according to the supplied generic 4-vector (accessible via
indexing, [0]==px,...[3]==E) and put the user and history indices back to their
default values.  
";

%feature("docstring") fastjet::PseudoJet::has_associated_cluster_sequence "

returns true if this PseudoJet has an associated ClusterSequence.  
";

%feature("docstring") fastjet::PseudoJet::user_index "

return the user_index,  
";

%feature("docstring") fastjet::PseudoJet::constituents "

retrieve the constituents.  

an Error is thrown if this PseudoJet has no currently valid associated
ClusterSequence or other substructure information  
";

%feature("docstring") fastjet::PseudoJet::reset_PtYPhiM "

reset the PseudoJet according to the specified pt, rapidity, azimuth and mass
(also resetting indices, etc.) (phi should satisfy -2pi<phi<4pi)  
";

%feature("docstring") fastjet::PseudoJet::structure_non_const_ptr "

return a non-const pointer to the structure (of type PseudoJetStructureBase*)
associated with this PseudoJet.  

return NULL if there is no associated structure  

Only use this if you know what you are doing. In any case, prefer the
'structure_ptr()' (the const version) to this method, unless you really need a
write access to the PseudoJet's underlying structure.  
";

%feature("docstring") fastjet::PseudoJet::area "

return the jet (scalar) area.  

throws an Error if there is no support for area in the parent CS  
";

%feature("docstring") fastjet::PseudoJet::perp "

returns the scalar transverse momentum  
";

%feature("docstring") fastjet::PseudoJet::has_pieces "

returns true if a jet has pieces  

By default a single particle or a jet coming from a ClusterSequence have no
pieces and this methos will return false.  

In practice, this is equivalent to have an structure of type
CompositeJetStructure.  
";

%feature("docstring") fastjet::PseudoJet::modp "

return the 3-vector modulus = sqrt(px^2+py^2+pz^2)  
";

%feature("docstring") fastjet::PseudoJet::pt "

returns the scalar transverse momentum  
";

%feature("docstring") fastjet::PseudoJet::plain_distance "

returns squared cylinder (rap-phi) distance between this jet and another  
";

%feature("docstring") fastjet::PseudoJet::px "
";

%feature("docstring") fastjet::PseudoJet::py "
";

%feature("docstring") fastjet::PseudoJet::pz "
";

%feature("docstring") fastjet::PseudoJet::pt2 "

returns the squared transverse momentum  
";

%feature("docstring") fastjet::PseudoJet::area_error "

return the error (uncertainty) associated with the determination of the area of
this jet.  

throws an Error if there is no support for area in the parent CS  
";

%feature("docstring") fastjet::PseudoJet::n_exclusive_subjets "

return the size of exclusive_subjets(...); still n ln n with same coefficient,
but marginally more efficient than manually taking exclusive_subjets.size()  

an Error is thrown if this PseudoJet has no currently valid associated
ClusterSequence  
";

%feature("docstring") fastjet::PseudoJet::has_valid_cluster_sequence "

returns true if this PseudoJet has an associated and still valid(ated)
ClusterSequence.  
";

%feature("docstring") fastjet::PseudoJet::user_info_shared_ptr "

retrieve a (const) shared pointer to the user information  
";

%feature("docstring") fastjet::PseudoJet::user_info_shared_ptr "

retrieve a (non-const) shared pointer to the user information; you can use this,
for example, to set the shared pointer, eg  


or  

  
";

%feature("docstring") fastjet::PseudoJet::set_cached_rap_phi "

in some cases when setting a 4-momentum, the user/program knows what rapidity
and azimuth are associated with that 4-momentum; by calling this routine the
user can provide the information directly to the PseudoJet and avoid expensive
rap-phi recalculations.  

*  
    Parameters:  
    * `rap` :  
        rapidity  
*  
    Parameters:  
    * `phi` :  
        (in range -twopi...4*pi)  

    USE WITH CAUTION: there are no checks that the rapidity and azimuth supplied
    are sensible, nor does this reset the 4-momentum components if things don't
    match.  
";

%feature("docstring") fastjet::PseudoJet::set_structure_shared_ptr "

set the associated structure  
";

%feature("docstring") fastjet::PseudoJet::validated_structure_ptr "

return a pointer to the structure (of type PseudoJetStructureBase*) associated
with this PseudoJet.  

throw an error if there is no associated structure  
";

%feature("docstring") fastjet::PseudoJet::has_area "

check if it has a defined area  
";

%feature("docstring") fastjet::PseudoJet::delta_R "

return the cylinder (rap-phi) distance between this jet and another, $\\Delta_R
= \\sqrt{\\Delta y^2 + \\Delta \\phi^2}$.  
";

%feature("docstring") fastjet::PseudoJet::is_pure_ghost "

true if this jet is made exclusively of ghosts.  

throws an Error if there is no support for area in the parent CS  
";

%feature("docstring") fastjet::PseudoJet::contains "

check if the current PseudoJet contains the one passed as argument.  

an Error is thrown if this PseudoJet has no currently valid associated
ClusterSequence  
";

%feature("docstring") fastjet::PseudoJet::unboost "

transform this jet (given in lab) into a jet in the rest frame of prest  
";

%feature("docstring") fastjet::PseudoJet::has_constituents "

returns true if the PseudoJet has constituents  
";

%feature("docstring") fastjet::PseudoJet::Et2 "

return the transverse energy squared  
";

%feature("docstring") fastjet::PseudoJet::mt2 "

returns the squared transverse mass = kt^2+m^2  
";

%feature("docstring") fastjet::PseudoJet::rapidity "

the same as rap()  
";

%feature("docstring") fastjet::PseudoJet::structure_shared_ptr "

return a reference to the shared pointer to the PseudoJetStructureBase
associated with this PseudoJet  
";

%feature("docstring") fastjet::PseudoJet::user_info "

returns a reference to the dynamic cast conversion of user_info to type L.  

Usage: suppose you have previously set the user info with a pointer to an object
of type MyInfo,  

class MyInfo: public PseudoJet::UserInfoBase { MyInfo(int id) : _pdg_id(id); int
pdg_id() const {return _pdg_id;} int _pdg_id; };  

PseudoJet particle(...); particle.set_user_info(new MyInfo(its_pdg_id));  

Then you would access that pdg_id() as  

particle.user_info<MyInfo>().pdg_id();  

It's overkill for just a single integer, but scales easily to more extensive
information.  

Note that user_info() throws an InexistentUserInfo() error if there is no user
info; throws a std::bad_cast if the conversion doesn't work  

If this behaviour does not fit your needs, use instead the the user_info_ptr()
or user_info_shared_ptr() member functions.  
";

%feature("docstring") fastjet::PseudoJet::rap "

returns the rapidity or some large value when the rapidity is infinite  
";

%feature("docstring") fastjet::PseudoJet::eta "
";

%feature("docstring") fastjet::PseudoJet::validated_cluster_sequence_area_base "

if the jet has valid area information then return a pointer to the associated
ClusterSequenceAreaBase object; otherwise throw an error  
";

%feature("docstring") fastjet::PseudoJet::exclusive_subjets_up_to "

return the list of subjets obtained by unclustering the supplied jet down to
nsub subjets (or all constituents if there are fewer than nsub).  

For ClusterSequence type jets, requires nsub ln nsub time  

An Error is thrown if this PseudoJet has no currently valid associated
ClusterSequence  
";

%feature("docstring") fastjet::PseudoJet::cluster_hist_index "

return the cluster_hist_index, intended to be used by clustering routines.  
";

%feature("docstring") fastjet::PseudoJet::exclusive_subdmerge "

Returns the dij that was present in the merging nsub+1 -> nsub subjets inside
this jet.  

Returns 0 if there were nsub or fewer constituents in the jet.  

an Error is thrown if this PseudoJet has no currently valid associated
ClusterSequence  
";

%feature("docstring") fastjet::PseudoJet::description "

return a string describing what kind of PseudoJet we are dealing with  
";

%feature("docstring") fastjet::PseudoJet::mperp "

returns the transverse mass = sqrt(kt^2+m^2)  
";

%feature("docstring") fastjet::PseudoJet::mt "

returns the transverse mass = sqrt(kt^2+m^2)  
";

%feature("docstring") fastjet::PseudoJet::perp2 "

returns the squared transverse momentum  
";

%feature("docstring") fastjet::PseudoJet::reset_momentum "

reset the 4-momentum according to the supplied components but leave all other
information (indices, user info, etc.) untouched  
";

%feature("docstring") fastjet::PseudoJet::reset_momentum "

reset the 4-momentum according to the components of the supplied PseudoJet,
including cached components; note that the template version (below) will be
called for classes derived from PJ.  
";

%feature("docstring") fastjet::PseudoJet::reset_momentum "

reset the 4-momentum according to the supplied generic 4-vector (accessible via
indexing, [0]==px,...[3]==E), but leave all other information (indices, user
info, etc.) untouched  
";

%feature("docstring") fastjet::PseudoJet::has_valid_cs "

shorthand for has_valid_cluster_sequence()  
";

%feature("docstring") fastjet::PseudoJet::has_exclusive_subjets "

returns true if the PseudoJet has support for exclusive subjets  
";

%feature("docstring") fastjet::PseudoJet::has_child "

check if it has been recombined with another PseudoJet in which case, return its
child through the argument.  

Otherwise, 'child' is set to 0.  

an Error is thrown if this PseudoJet has no currently valid associated
ClusterSequence  
";

%feature("docstring") fastjet::PseudoJet::has_user_info "

returns true if the PseudoJet has user information  
";

%feature("docstring") fastjet::PseudoJet::has_user_info "

returns true if the PseudoJet has user information than can be cast to the
template argument type.  
";

%feature("docstring") fastjet::PseudoJet::is_inside "

check if the current PseudoJet is contained the one passed as argument.  

an Error is thrown if this PseudoJet has no currently valid associated
ClusterSequence  
";

%feature("docstring") fastjet::PseudoJet::squared_distance "

returns squared cylinder (rap-phi) distance between this jet and another  
";

%feature("docstring") fastjet::PseudoJet::set_user_index "

set the user_index, intended to allow the user to add simple identifying
information to a particle/jet  
";

%feature("docstring") fastjet::PseudoJet::structure "

returns a reference to the structure casted to the requested structure type  

If there is no structure associated, an Error is thrown. If the type is not met,
a std::bad_cast error is thrown.  
";

%feature("docstring") fastjet::PseudoJet::m2 "

returns the squared invariant mass // like CLHEP  
";

%feature("docstring") fastjet::PseudoJet::associated_cluster_sequence "

get a (const) pointer to the parent ClusterSequence (NULL if inexistent)  
";

%feature("docstring") fastjet::PseudoJet::boost "

transform this jet (given in the rest frame of prest) into a jet in the lab
frame  
";

%feature("docstring") fastjet::PseudoJet::beam_distance "

returns distance between this jet and the beam  
";

%feature("docstring") fastjet::PseudoJet::set_cluster_hist_index "

set the cluster_hist_index, intended to be used by clustering routines.  
";

%feature("docstring") fastjet::PseudoJet::user_info_ptr "

retrieve a pointer to the (const) user information  
";

%feature("docstring") fastjet::PseudoJet::mperp2 "

returns the squared transverse mass = kt^2+m^2  
";

%feature("docstring") fastjet::PseudoJet::has_structure "

return true if there is some structure associated with this PseudoJet  
";

%feature("docstring") fastjet::PseudoJet::pseudorapidity "

returns the pseudo-rapidity or some large value when the rapidity is infinite  
";

%feature("docstring") fastjet::PseudoJet::has_structure_of "

check if the PseudoJet has the structure resulting from a Transformer (that is,
its structure is compatible with a Transformer::StructureType).  

If there is no structure, false is returned.  
";

%feature("docstring") fastjet::PseudoJet::reset_momentum_PtYPhiM "

reset the 4-momentum according to the specified pt, rapidity, azimuth and mass
(phi should satisfy -2pi<phi<4pi)  
";

%feature("docstring") fastjet::PseudoJet::set_cluster_sequence_history_index "

alternative name for set_cluster_hist_index(...) [perhaps more meaningful]  
";

// File: classfastjet_1_1PseudoJetStructureBase.xml


%feature("docstring") fastjet::PseudoJetStructureBase "

Contains any information related to the clustering that should be directly
accessible to PseudoJet.  

By default, this class implements basic access to the ClusterSequence related to
a PseudoJet (like its constituents or its area). But it can be overloaded in
order e.g. to give access to the jet substructure.  

C++ includes: fastjet/PseudoJetStructureBase.hh
";

/*
 Direct access to the associated ClusterSequence object. 
*/

/*
Get access to the associated ClusterSequence (if any)  

*/

/*
 Methods for access to information about jet structure 
*/

/*
These allow access to jet constituents, and other jet subtructure information.  

They only work if the jet is associated with a ClusterSequence.  

*/

%feature("docstring") fastjet::PseudoJetStructureBase::exclusive_subjets "

return a vector of all subjets of the current jet (in the sense of the exclusive
algorithm) that would be obtained when running the algorithm with the given
dcut.  

Time taken is O(m ln m), where m is the number of subjets that are found. If m
gets to be of order of the total number of constituents in the jet, this could
be substantially slower than just getting that list of constituents.  

By default, throws an Error  

Note: in a future major release of FastJet (4 or higher), \"const double &
dcut\" may be replaced with \"const double dcut\", requiring a modification of
derived classes that overload this function.  
";

%feature("docstring") fastjet::PseudoJetStructureBase::PseudoJetStructureBase "

default ctor  
";

%feature("docstring") fastjet::PseudoJetStructureBase::description "

description  
";

%feature("docstring") fastjet::PseudoJetStructureBase::object_in_jet "

check if the reference PseudoJet is contained the second one passed as argument.  

By default, throws an Error  
";

%feature("docstring") fastjet::PseudoJetStructureBase::validated_csab "

if the jet has valid area information then return a pointer to the associated
ClusterSequenceAreaBase object; otherwise throw an error  
";

%feature("docstring") fastjet::PseudoJetStructureBase::area_4vector "

return the jet 4-vector area.  

By default, throws an Error  
";

%feature("docstring") fastjet::PseudoJetStructureBase::has_exclusive_subjets "

return true if the structure supports exclusive_subjets.  
";

%feature("docstring") fastjet::PseudoJetStructureBase::has_parents "

check if it is the product of a recombination, in which case return the 2
parents through the 'parent1' and 'parent2' arguments.  

Otherwise, set these to 0.  

By default, throws an Error  
";

%feature("docstring") fastjet::PseudoJetStructureBase::constituents "

retrieve the constituents.  

By default, throws an Error  
";

%feature("docstring") fastjet::PseudoJetStructureBase::has_area "

check if it has a defined area  

false by default  
";

%feature("docstring") fastjet::PseudoJetStructureBase::is_pure_ghost "

true if this jet is made exclusively of ghosts.  

By default, throws an Error  
";

%feature("docstring") fastjet::PseudoJetStructureBase::exclusive_subdmerge_max "

return the maximum dij that occurred in the whole event at the stage that the
nsub+1 -> nsub merge of subjets occurred inside this jet.  

By default, throws an Error  
";

%feature("docstring") fastjet::PseudoJetStructureBase::n_exclusive_subjets "

return the size of exclusive_subjets(...); still n ln n with same coefficient,
but marginally more efficient than manually taking exclusive_subjets.size()  

By default, throws an Error  

Note: in a future major release of FastJet (4 or higher), \"const double &
dcut\" may be replaced with \"const double dcut\", requiring a modification of
derived classes that overload this function.  
";

%feature("docstring") fastjet::PseudoJetStructureBase::has_valid_cluster_sequence "

returns true if this PseudoJet has an associated and still valid
ClusterSequence.  
";

%feature("docstring") fastjet::PseudoJetStructureBase::exclusive_subdmerge "

return the dij that was present in the merging nsub+1 -> nsub subjets inside
this jet.  

By default, throws an Error  
";

%feature("docstring") fastjet::PseudoJetStructureBase::~PseudoJetStructureBase "

default (virtual) dtor  
";

%feature("docstring") fastjet::PseudoJetStructureBase::pieces "

retrieve the pieces building the jet.  

By default, throws an Error. NB: \"reference\" is commented to avoid unused-
variable compiler warnings  
";

%feature("docstring") fastjet::PseudoJetStructureBase::has_associated_cluster_sequence "

returns true if there is an associated ClusterSequence  
";

%feature("docstring") fastjet::PseudoJetStructureBase::validated_cs "

if the jet has a valid associated cluster sequence then return a pointer to it;
otherwise throw an error  
";

%feature("docstring") fastjet::PseudoJetStructureBase::has_partner "

check if it has been recombined with another PseudoJet in which case, return its
partner through the argument.  

Otherwise, 'partner' is set to 0.  

By default, throws an Error  
";

%feature("docstring") fastjet::PseudoJetStructureBase::has_child "

check if it has been recombined with another PseudoJet in which case, return its
child through the argument.  

Otherwise, 'child' is set to 0.  

By default, throws an Error  
";

%feature("docstring") fastjet::PseudoJetStructureBase::exclusive_subjets_up_to "

return the list of subjets obtained by unclustering the supplied jet down to
nsub subjets (or all constituents if there are fewer than nsub).  

By default, throws an Error  
";

%feature("docstring") fastjet::PseudoJetStructureBase::area_error "

return the error (uncertainty) associated with the determination of the area of
this jet.  

By default, throws an Error  
";

%feature("docstring") fastjet::PseudoJetStructureBase::associated_cluster_sequence "

get a (const) pointer to the parent ClusterSequence (NULL if inexistent)  
";

%feature("docstring") fastjet::PseudoJetStructureBase::has_pieces "

return true if the structure supports pieces.  

false by default NB: \"reference\" is commented to avoid unused-variable
compiler warnings  
";

%feature("docstring") fastjet::PseudoJetStructureBase::area "

return the jet (scalar) area.  

By default, throws an Error  
";

%feature("docstring") fastjet::PseudoJetStructureBase::has_constituents "

return true if the structure supports constituents.  

false by default  
";

// File: classfastjet_1_1PxConePlugin.xml


%feature("docstring") fastjet::PxConePlugin "

Implementation of the PxCone algorithm (plugin for fastjet v2.1 upwards)  

PxConePlugin is a plugin for fastjet (v2.1 upwards) that provides an interface
to the fortran pxcone iterative cone algorithm with midpoint seeds.  

Pxcone was written by Luis del Pozo and Michael H. Seymour. It is not a
\"supported\" program, so if you encounter problems, you are on your own...  

Note that pxcone sometimes encounters non-stable iterations; in such cases it
returns an error -- the plugin propagates this by throwing a fastjet::Error
exception; if the user wishes to have robust code, they should catch this
exception.  

Pxcone has a hard-coded limit (by default 4000) on the maximum number of
particles and protojets; if the number of particles or protojets exceeds this,
again a fastjet::Error exception will be thrown.  

The functionality of pxcone is described at
http://www.hep.man.ac.uk/u/wplano/ConeJet.ps  

C++ includes: fastjet/PxConePlugin.hh
";

%feature("docstring") fastjet::PxConePlugin::R "

the plugin mechanism's standard way of accessing the jet radius  
";

%feature("docstring") fastjet::PxConePlugin::overlap_threshold "

Maximum fraction of overlap energy in a jet -- called ovlim in pxcone.  
";

%feature("docstring") fastjet::PxConePlugin::E_scheme_jets "

if true then the final jets are returned as the E-scheme recombination of the
particle momenta (by default, pxcone returns massless jets with a mean phi,eta
type of recombination); regardless of what is returned, the internal pxcone jet-
finding procedure is unaffected.  
";

%feature("docstring") fastjet::PxConePlugin::cone_radius "

the cone radius  
";

%feature("docstring") fastjet::PxConePlugin::description "

return a textual description of the jet-definition implemented in this plugin  
";

%feature("docstring") fastjet::PxConePlugin::min_jet_energy "

minimum jet energy (protojets below this are thrown own before
merging/splitting) -- called epslon in pxcone  
";

%feature("docstring") fastjet::PxConePlugin::PxConePlugin "

constructor for the PxConePlugin, whose arguments have the following meaning:  

*   the cone_radius is as usual in cone algorithms  
*   stables cones (protojets) below min_jet_energy are discarded before calling
    the splitting procedure to resolve overlaps (called epslon in pxcone).  
*   when two protojets overlap, if (overlapping_Et)/(Et_of_softer_protojet) <
    overlap_threshold the overlapping energy is split between the two protojets;
    otherwise the less energetic protojet is discarded. Called ovlim in pxcone.  
*   pxcone carries out p-scheme recombination, and the resulting jets are
    massless; setting E_scheme_jets = true (default false) doesn't change the
    jet composition, but the final momentum sum for the jets is carried out by
    direct four-vector addition instead of p-scheme recombination.  
";

%feature("docstring") fastjet::PxConePlugin::run_clustering "

given a ClusterSequence that has been filled up with initial particles, the
following function should fill up the rest of the ClusterSequence, using the
following member functions of ClusterSequence:  

*   plugin_do_ij_recombination(...)  
*   plugin_do_iB_recombination(...)  
";

// File: classfastjet_1_1QuantityAbsEta.xml


%feature("docstring") fastjet::QuantityAbsEta "

helper for selecting on |pseudo-rapidities|  
";

%feature("docstring") fastjet::QuantityAbsEta::QuantityAbsEta "
";

%feature("docstring") fastjet::QuantityAbsEta::is_geometric "
";

%feature("docstring") fastjet::QuantityAbsEta::description "
";

// File: classfastjet_1_1QuantityAbsRap.xml


%feature("docstring") fastjet::QuantityAbsRap "

helper for selecting on |rapidities|  
";

%feature("docstring") fastjet::QuantityAbsRap::is_geometric "
";

%feature("docstring") fastjet::QuantityAbsRap::QuantityAbsRap "
";

%feature("docstring") fastjet::QuantityAbsRap::description "
";

// File: classfastjet_1_1QuantityBase.xml


%feature("docstring") fastjet::QuantityBase "
";

%feature("docstring") fastjet::QuantityBase::description "
";

%feature("docstring") fastjet::QuantityBase::description_value "
";

%feature("docstring") fastjet::QuantityBase::~QuantityBase "
";

%feature("docstring") fastjet::QuantityBase::is_geometric "
";

%feature("docstring") fastjet::QuantityBase::QuantityBase "
";

%feature("docstring") fastjet::QuantityBase::comparison_value "
";

// File: classfastjet_1_1QuantityE.xml


%feature("docstring") fastjet::QuantityE "

helper class for selecting on energy  
";

%feature("docstring") fastjet::QuantityE::description "
";

%feature("docstring") fastjet::QuantityE::QuantityE "
";

// File: classfastjet_1_1QuantityEt2.xml


%feature("docstring") fastjet::QuantityEt2 "

helper class for selecting on transverse energy  
";

%feature("docstring") fastjet::QuantityEt2::description "
";

%feature("docstring") fastjet::QuantityEt2::QuantityEt2 "
";

// File: classfastjet_1_1QuantityEta.xml


%feature("docstring") fastjet::QuantityEta "

helper for selecting on pseudo-rapidities  
";

%feature("docstring") fastjet::QuantityEta::QuantityEta "
";

%feature("docstring") fastjet::QuantityEta::description "
";

// File: classfastjet_1_1QuantityM2.xml


%feature("docstring") fastjet::QuantityM2 "

helper class for selecting on mass  
";

%feature("docstring") fastjet::QuantityM2::description "
";

%feature("docstring") fastjet::QuantityM2::QuantityM2 "
";

// File: classfastjet_1_1QuantityPt2.xml


%feature("docstring") fastjet::QuantityPt2 "

helper class for selecting on pt  
";

%feature("docstring") fastjet::QuantityPt2::QuantityPt2 "
";

%feature("docstring") fastjet::QuantityPt2::description "
";

// File: classfastjet_1_1QuantityRap.xml


%feature("docstring") fastjet::QuantityRap "

helper for selecting on rapidities: quantity  
";

%feature("docstring") fastjet::QuantityRap::description "
";

%feature("docstring") fastjet::QuantityRap::is_geometric "
";

%feature("docstring") fastjet::QuantityRap::QuantityRap "
";

// File: classfastjet_1_1QuantitySquareBase.xml


%feature("docstring") fastjet::QuantitySquareBase "
";

%feature("docstring") fastjet::QuantitySquareBase::QuantitySquareBase "
";

%feature("docstring") fastjet::QuantitySquareBase::description_value "
";

// File: classfastjet_1_1RangeDefinition.xml


%feature("docstring") fastjet::RangeDefinition "

class for holding a range definition specification, given by limits on rapidity
and azimuth.  

C++ includes: fastjet/RangeDefinition.hh
";

%feature("docstring") fastjet::RangeDefinition::description "

textual description of range  
";

%feature("docstring") fastjet::RangeDefinition::is_in_range "

return bool according to whether the jet is within the given range  
";

%feature("docstring") fastjet::RangeDefinition::is_in_range "

return bool according to whether a (rap,phi) point is in range  
";

%feature("docstring") fastjet::RangeDefinition::~RangeDefinition "

destructor does nothing  
";

%feature("docstring") fastjet::RangeDefinition::__attribute__ "

default constructor  
";

%feature("docstring") fastjet::RangeDefinition::__attribute__ "

constructor for a range definition given by |y|<rapmax  
";

%feature("docstring") fastjet::RangeDefinition::is_localizable "

returns true if the range is localizable (i.e.  

set_position is meant to do something meaningful).  

This version of the class is not localizable and so it returns false.  

For localizable classes override this function with a function that returns true  
";

%feature("docstring") fastjet::RangeDefinition::get_rap_limits "

return the minimal and maximal rapidity of this range; remember to replace this
if you write a derived class with more complex ranges;  
";

%feature("docstring") fastjet::RangeDefinition::RangeDefinition "

constructor for a range definition given by rapmin <= y <= rapmax, phimin <= phi
<= phimax  
";

%feature("docstring") fastjet::RangeDefinition::area "

area of the range region  
";

%feature("docstring") fastjet::RangeDefinition::set_position "

place the range on the rap-phi position  

THIS DOES NOT DO ANYTHING FOR THIS CLASS AND IS ONLY THERE TO FACILITATE DERIVED
CLASSES  

DON'T NECESSARILY COUNT ON IT IN THE FUTURE EITHER???  
";

%feature("docstring") fastjet::RangeDefinition::set_position "

place the range on the jet position  
";

// File: classfastjet_1_1Recluster.xml


%feature("docstring") fastjet::Recluster "

Recluster a jet's constituents with a new jet definition.  

When Recluster is constructed from a JetDefinition, it is that definition that
will be used to obtain the new jets. The user may then decide if the recombiner
should be the one from that jet definition or if it should be acquired from the
jet being processed (the default).  

Alternatively, Recluster can be constructed from a jet algorithm and an optional
radius. In that case the recombiner is systematically obtained from the jet
being processed (unless you call set_acquire_recombiner(false)). If only the jet
algorithm is specified, a default radius of max_allowable_R will be assumed if
needed.  

Recluster has two possible behaviours:  

*   if it is constructed with keep=keep_only_hardest the hardest inclusive jet
    is returned as a \"standard\" jet with an associated cluster sequence
    (unless there were no inclusive jets, in which case a zero jet is returned,
    with no associated cluster sequence)  
*   if it is constructed with keep=keep_all all the inclusive jets are joined
    into a composite jet  

[Note that since the structure of the resulting PseudoJet depends on its usage,
this class inherits from FunctionOfPseudoJet<PseudoJet> (including a
description) rather than being a full-fledged Transformer]  

C++ includes: fastjet/tools/Recluster.hh
";

%feature("docstring") fastjet::Recluster::cambridge_optimisation "
";

%feature("docstring") fastjet::Recluster::set_cambridge_optimisation "

sets whether to try to optimise reclustering with Cambridge/Aachen algorithms
(by not reclustering if the requested C/A reclustering can be obtained by using
subjets of an input C/A jet or one composed of multiple C/A pieces from the same
clustering sequence).  

By default this is enabled, and *should* always be correct; disable it to test
this statement!  
";

%feature("docstring") fastjet::Recluster::Recluster "

default constructor (uses an undefined JetDefinition, and so cannot be used
directly).  
";

%feature("docstring") fastjet::Recluster::Recluster "

Constructs a Recluster object that reclusters a jet into a new jet using a
generic JetDefinition.  

Parameters
----------
* `new_jet_def` :  
    the jet definition applied to do the reclustering  
* `acquire_recombiner` :  
    when true, the reclustering will guess the recombiner from the input jet
    instead of the one in new_jet_def. An error is then thrown if no consistent
    recombiner is found  
* `keep_in` :  
    Recluster::keep_only_hardest: the result is the hardest inclusive jet after
    reclustering, returned as a \"standard\" jet. Recluster::keep_all: the
    result is a composite jet with the inclusive jets as pieces.  
";

%feature("docstring") fastjet::Recluster::Recluster "

Constructs a Recluster object that reclusters a jet into a new jet using a
JetAlgorithm and its parameters.  

Parameters
----------
* `new_jet_alg` :  
    the jet algorithm applied to obtain the new clustering  
* `new_jet_radius` :  
    the jet radius  
* `keep_in` :  
    Recluster::keep_only_hardest: the result is the hardest inclusive jet after
    reclustering, returned as a \"standard\" jet. Recluster::keep_all: the
    result is a composite jet with the inclusive jets as pieces.  

This ctor will always acquire the recombiner from the jet being reclustered (it
will throw if none can be found). If you wish to use Recluster with an algorithm
that requires an extra parameter (like the genkt algorithm), please specify the
jet definition fully using the constructor above.  
";

%feature("docstring") fastjet::Recluster::Recluster "

constructor with just a jet algorithm, but no jet radius.  

If the algorithm requires a jet radius, JetDefinition::max_allowable_R will be
used.  
";

%feature("docstring") fastjet::Recluster::generate_output_jet "

given a set of inclusive jets and a jet definition used, create the resulting
PseudoJet;  

If ca_optimisation_used then special care will be taken in deciding whether the
final jet can legitimately have an area.  
";

%feature("docstring") fastjet::Recluster::result "

runs the reclustering and sets kept and rejected to be the jets of interest
(with non-zero rho, they will have been subtracted).  

Normally this will be accessed through the base class's operator().  

Parameters
----------
* `jet` :  
    the jet that gets reclustered  

Returns
-------
the reclustered jet  
";

%feature("docstring") fastjet::Recluster::keep "

returns the current \"keep\" mode i.e.  

whether only the hardest inclusive jet is returned or all of them (see the Keep
enum above)  
";

%feature("docstring") fastjet::Recluster::set_keep "

set the behaviour with regards to keeping all resulting jets or just the
hardest.  
";

%feature("docstring") fastjet::Recluster::get_new_jets_and_def "

A lower-level method that does the actual work of reclustering the input jet.  

The resulting jets are stored in output_jets. The jet definition that has been
used can be accessed from the output_jets' ClusterSequence.  

Parameters
----------
* `input_jet` :  
    the (input) jet that one wants to recluster  
* `output_jets` :  
    inclusive jets resulting from the new clustering  

Returns true if the C/A optimisation has been used (this means that
generate_output_jet then has to watch out for non-explicit-ghost areas that
might be leftover)  
";

%feature("docstring") fastjet::Recluster::set_cambridge_optimization "

sets whether to try to optimise reclustering with Cambridge/Aachen algorithms
(US spelling!)  
";

%feature("docstring") fastjet::Recluster::cambridge_optimization "

returns true if the reclusterer tries to optimise reclustering with
Cambridge/Aachen algorithms  
";

%feature("docstring") fastjet::Recluster::acquire_recombiner "

returns true if this reclusterer is set to acquire the recombiner from the input
jet  
";

%feature("docstring") fastjet::Recluster::set_acquire_recombiner "

set whether the reclustering should attempt to acquire a recombiner from the
input jet  
";

%feature("docstring") fastjet::Recluster::~Recluster "

default dtor  
";

%feature("docstring") fastjet::Recluster::description "

class description  
";

// File: classfastjet_1_1JetDefinition_1_1Recombiner.xml


%feature("docstring") fastjet::JetDefinition::Recombiner "

An abstract base class that will provide the recombination scheme facilities
and/or allow a user to extend these facilities.  

C++ includes: fastjet/JetDefinition.hh
";

%feature("docstring") fastjet::JetDefinition::Recombiner::description "

return a textual description of the recombination scheme implemented here  
";

%feature("docstring") fastjet::JetDefinition::Recombiner::plus_equal "

pa += pb in the given recombination scheme.  

Not virtual -- the user should have no reason to want to redefine this!  
";

%feature("docstring") fastjet::JetDefinition::Recombiner::recombine "

recombine pa and pb and put result into pab  
";

%feature("docstring") fastjet::JetDefinition::Recombiner::preprocess "

routine called to preprocess each input jet (to make all input jets compatible
with the scheme requirements (e.g.  

massless).  
";

%feature("docstring") fastjet::JetDefinition::Recombiner::~Recombiner "

a destructor to be replaced if necessary in derived classes...  
";

// File: classfastjet_1_1RectangularGrid.xml


%feature("docstring") fastjet::RectangularGrid "

Class that holds a generic rectangular tiling.  

C++ includes: fastjet/RectangularGrid.hh
";

%feature("docstring") fastjet::RectangularGrid::tile_is_good "

returns whether a given tile is good  
";

%feature("docstring") fastjet::RectangularGrid::is_initialised "

returns true if the grid is in a suitably initialised state  
";

%feature("docstring") fastjet::RectangularGrid::mean_tile_area "

returns the mean area of tiles.  
";

%feature("docstring") fastjet::RectangularGrid::n_tiles "

returns the total number of tiles in the tiling; valid tile indices run from 0
...  

n_tiles()-1;  
";

%feature("docstring") fastjet::RectangularGrid::n_good_tiles "

returns the number of tiles that are \"good\"; i.e.  

there is scope for having tiles that, for whatever reason, should be ignored;
there are situations in which having \"non-good\" tiles may be the simplest
mechanism to obtain a tiling with holes in it  
";

%feature("docstring") fastjet::RectangularGrid::description "

returns a textual description of the grid  
";

%feature("docstring") fastjet::RectangularGrid::rapmax "

returns the maxmium rapidity extent of the grid  
";

%feature("docstring") fastjet::RectangularGrid::RectangularGrid "

ctor with simple initialisation  

Parameters
----------
* `rapmax` :  
    the maximal absolute rapidity extent of the grid  
* `cell_size` :  
    the grid spacing (equivalently, cell size)  
";

%feature("docstring") fastjet::RectangularGrid::RectangularGrid "

ctor with more control over initialisation  

Parameters
----------
* `rapmin` :  
    the minimum rapidity extent of the grid  
* `rapmax` :  
    the maximum rapidity extent of the grid  
* `drap` :  
    the grid spacing in rapidity  
* `dphi` :  
    the grid spacing in azimuth  
* `tile_selector` :  
    optional (geometric) selector to specify which tiles are good; a tile is
    good if a massless 4-vector at the center of the tile passes the selection  
";

%feature("docstring") fastjet::RectangularGrid::RectangularGrid "

dummy ctor (will give an unusable grid)  
";

%feature("docstring") fastjet::RectangularGrid::tile_index "

returns the index of the tile in which p is located, or -1 if p is outside the
tiling region  
";

%feature("docstring") fastjet::RectangularGrid::drap "

returns the spacing of the grid in rapidity  
";

%feature("docstring") fastjet::RectangularGrid::tile_area "

returns the area of tile itile.  
";

%feature("docstring") fastjet::RectangularGrid::rapmin "

returns the minimum rapidity extent of the grid  
";

%feature("docstring") fastjet::RectangularGrid::dphi "

returns the spacing of the grid in azimuth  
";

// File: classfastjet_1_1RestFrameNSubjettinessTagger.xml


%feature("docstring") fastjet::RestFrameNSubjettinessTagger "

Class that helps perform 2-pronged boosted tagging using a reclustering in the
jet's rest frame, supplemented with a cut on N-subjettiness (and a decay angle),
as discussed by Ji-Hun Kim in arXiv:1011.1493.  

To tag a fat jet, the tagger proceeds as follows:  

*   boost its constituents into the rest frame of the jet  
*   recluster them using another jet definition (the original choice was SISCone
    in spherical coordinates with R=0.6 and f=0.75.  
*   keep the 2 most energetic subjets ( $q_{1,2}$) and compute the
    2-subjettiness \\[ \\tau_2^j = \\frac{2}{m_{\\rm jet}^2}\\, \\sum_{k\\in
    {\\rm jet}} {\\rm min}(q_1.p_k,q_2.p_k) \\] where the sum runs over the
    constituents of the jet.  
*   require $\\tau_2^j < \\tau_2^{\\rm cut}$ [0.08 by default]  
*   impose that (in the rest frame of the fat jet), the angles between the 2
    most energetic subjets and the boost axis are both large enough:
    $\\cos(\\theta_s)<c_\\theta^{\\rm cut}$ [0.8 by default]  

Note that in the original version, the jets to be tagged were reconstructed
using SISCone with R=0.8 and f=0.75. Also, b-tagging was imposed on the 2
subjets found in the rest-frame tagging procedure.  
Options
The constructor has the following arguments:  

*   The first argument is the jet definition to be used to recluster the
    constituents of the jet to be filtered (in the rest frame of the tagged
    jet).  
*   The second argument is the cut on tau_2 [0.08 by default]  
*   The 3rd argument is the cut on cos(theta_s) [0.8 by default]  
*   If the 4th argument is true, 2 exclusive rest-frame jets will be considered
    in place of the 2 most energetic inclusive jets  
Input conditions
*   the original jet must have constituents  
Output/structure
*   the 2 subjets are kept as pieces if some substructure is found, otherwise a
    single 0-momentum piece  
*   the tau2 and maximal cos(theta_s) values computed during the tagging can be
    obtained via the resulting jet's structure_of<...>() function  

C++ includes: fastjet/tools/RestFrameNSubjettinessTagger.hh
";

%feature("docstring") fastjet::RestFrameNSubjettinessTagger::RestFrameNSubjettinessTagger "

ctor with arguments (see the class description above)  
";

%feature("docstring") fastjet::RestFrameNSubjettinessTagger::description "

returns a textual description of the tagger  
";

%feature("docstring") fastjet::RestFrameNSubjettinessTagger::result "

runs the tagger on the given jet and returns the tagged PseudoJet if successful,
a PseudoJet==0 otherwise (standard access is through operator()).  

impose the cut on cos(theta_s)  
";

// File: classfastjet_1_1RestFrameNSubjettinessTaggerStructure.xml


%feature("docstring") fastjet::RestFrameNSubjettinessTaggerStructure "

the structure returned by the RestFrameNSubjettinessTagger transformer.  

See the RestFrameNSubjettinessTagger class description for the details of what
is inside this structure  

C++ includes: fastjet/tools/RestFrameNSubjettinessTagger.hh
";

%feature("docstring") fastjet::RestFrameNSubjettinessTaggerStructure::RestFrameNSubjettinessTaggerStructure "

ctor with pieces initialisation  
";

%feature("docstring") fastjet::RestFrameNSubjettinessTaggerStructure::costhetas "

returns the associated angle with the boosted axis  
";

%feature("docstring") fastjet::RestFrameNSubjettinessTaggerStructure::tau2 "

returns the associated N-subjettiness  
";

// File: classfastjet_1_1SearchTree.xml


%feature("docstring") fastjet::SearchTree "
";

%feature("docstring") fastjet::SearchTree::size "

return the number of elements currently in the search tree  
";

%feature("docstring") fastjet::SearchTree::_find_successor "

return successor by walking through the tree  
";

%feature("docstring") fastjet::SearchTree::_find_predecessor "

return predecessor by walking through the tree  
";

%feature("docstring") fastjet::SearchTree::remove "

remove the node corresponding to node_index from the search tree  
";

%feature("docstring") fastjet::SearchTree::remove "
";

%feature("docstring") fastjet::SearchTree::remove "
";

%feature("docstring") fastjet::SearchTree::loc "
";

%feature("docstring") fastjet::SearchTree::somewhere "

return a circulator to some place in the tree (with a circulator you don't care
where...)  
";

%feature("docstring") fastjet::SearchTree::somewhere "
";

%feature("docstring") fastjet::SearchTree::verify_structure_recursive "
";

%feature("docstring") fastjet::SearchTree::print_elements "

print out all elements...  
";

%feature("docstring") fastjet::SearchTree::verify_structure_linear "
";

%feature("docstring") fastjet::SearchTree::insert "

insert the supplied value into the tree and return a pointer to the relevant
SearchTreeNode.  
";

%feature("docstring") fastjet::SearchTree::SearchTree "

constructor for a search tree from an ordered vector  

initialise from a sorted initial array  
";

%feature("docstring") fastjet::SearchTree::SearchTree "

constructor for a search tree from an ordered vector allowing for future growth
beyond the current size, up to max_size  

initialise from a sorted initial array allowing for a larger maximum size of the
array...  
";

%feature("docstring") fastjet::SearchTree::max_depth "
";

%feature("docstring") fastjet::SearchTree::verify_structure "

check that the structure we've obtained makes sense...  
";

// File: classfastjet_1_1Selector.xml


%feature("docstring") fastjet::Selector "

Class that encodes information about cuts and other selection criteria that can
be applied to PseudoJet(s).  

C++ includes: fastjet/Selector.hh
";

%feature("docstring") fastjet::Selector::worker "

returns a (reference to) the underlying worker's shared pointer  
";

%feature("docstring") fastjet::Selector::sift "

sift the input jets into two vectors -- those that pass the selector and those
that do not  
";

%feature("docstring") fastjet::Selector::get_rapidity_extent "

returns the rapidity range for which it may return \"true\"  
";

%feature("docstring") fastjet::Selector::pass "

return true if the jet passes the selection  
";

%feature("docstring") fastjet::Selector::set_reference "

set the reference jet for this Selector  
";

%feature("docstring") fastjet::Selector::has_finite_area "

returns true if it has a meaningful and finite area (i.e.  

the Selector has the property that is_geometric() returns true and the rapidity
extent is finite).  
";

%feature("docstring") fastjet::Selector::validated_worker "

returns a worker if there is a valid one, otherwise throws an InvalidWorker
error  
";

%feature("docstring") fastjet::Selector::~Selector "

dummy virtual dtor  
";

%feature("docstring") fastjet::Selector::takes_reference "

returns true if this can be applied jet by jet  
";

%feature("docstring") fastjet::Selector::is_geometric "

returns true if it is a geometric selector (i.e.  

one that only puts constraints on rapidities and azimuthal angles)  
";

%feature("docstring") fastjet::Selector::description "

returns a textual description of the selector  
";

%feature("docstring") fastjet::Selector::scalar_pt_sum "

Return the scalar pt sum of the objects that pass the selection.  

This will often be more efficient that getting the vector of objects that passes
and then evaluating the size of the vector  
";

%feature("docstring") fastjet::Selector::area "

returns the rapidity-phi area associated with the Selector (throws InvalidArea
if the area does not make sense).  

If the result is not known analytically, the area will be estimated using a
pseudo Monte Carlo method (as for jet areas), using the default ghost area from
the GhostedAreaSpec class (0.01). The Monte Carlo estimate involves a time
penalty proportional to the ratio of the rapidity extent of the Selector divided
by the ghost area.  
";

%feature("docstring") fastjet::Selector::area "

returns the rapidity-phi area associated with the Selector (throws InvalidArea
if the area does not make sense).  

The behaviour is the as with the area() call, but with the ability to
additionally specify the ghost area to be used in the case of a Monte Carlo area
evaluation.  
";

%feature("docstring") fastjet::Selector::applies_jet_by_jet "

returns true if this can be applied jet by jet  
";

%feature("docstring") fastjet::Selector::Selector "

default constructor produces a Selector whose action is undefined (any attempt
to use it will lead to an error)  
";

%feature("docstring") fastjet::Selector::Selector "

constructor that causes the Selector to use the supplied worker  

Note that the Selector takes ownership of the pointer to the worker (and so will
delete automatically when appropriate).  
";

%feature("docstring") fastjet::Selector::Selector "

ctor from a RangeDefinition  

This is provided for backward compatibility and will be removed in a future
major release of FastJet  

Watch out that the Selector will only hold a pointer to the range so the
selector will crash if one tries to use it after the range has gone out of
scope. We thus strongly advise against the direct use of this constructor.  
";

%feature("docstring") fastjet::Selector::count "

Return a count of the objects that pass the selection.  

This will often be more efficient that getting the vector of objects that passes
and then evaluating the size of the vector  
";

%feature("docstring") fastjet::Selector::nullify_non_selected "

For each jet that does not pass the cuts, this routine sets the pointer to 0.  

It is legitimate for some (or all) of the pointers that are passed to already be
NULL.  
";

%feature("docstring") fastjet::Selector::sum "

Return the 4-vector sum of the objects that pass the selection.  

This will often be more efficient that getting the vector of objects that passes
and then evaluating the size of the vector  
";

// File: classfastjet_1_1SelectorWorker.xml


%feature("docstring") fastjet::SelectorWorker "

default selector worker is an abstract virtual base class  

The Selector class is only an interface, it is the SelectorWorker that really
does the work. To implement various selectors, one thus has to overload this
class.  

C++ includes: fastjet/Selector.hh
";

%feature("docstring") fastjet::SelectorWorker::description "

returns a description of the worker  
";

%feature("docstring") fastjet::SelectorWorker::~SelectorWorker "

default dtor  
";

%feature("docstring") fastjet::SelectorWorker::set_reference "

sets the reference jet for the selector NB: \"reference\" is commented to avoid
unused-variable compiler warnings  
";

%feature("docstring") fastjet::SelectorWorker::has_known_area "

check if it has an analytically computable area  
";

%feature("docstring") fastjet::SelectorWorker::copy "

return a copy of the current object.  

This function is only called for objects that take a reference and need not be
reimplemented otherwise.  
";

%feature("docstring") fastjet::SelectorWorker::has_finite_area "

check if it has a finite area  
";

%feature("docstring") fastjet::SelectorWorker::applies_jet_by_jet "

returns true if this can be applied jet by jet  
";

%feature("docstring") fastjet::SelectorWorker::get_rapidity_extent "

returns the rapidity range for which it may return \"true\"  
";

%feature("docstring") fastjet::SelectorWorker::takes_reference "

returns true if the worker is defined with respect to a reference jet  
";

%feature("docstring") fastjet::SelectorWorker::pass "

returns true if a given object passes the selection criterion, and is the main
function that needs to be overloaded by derived workers.  

NB: this function is used only if applies_jet_by_jet() returns true. If it does
not, then derived classes are expected to (re)implement the terminator
function()  
";

%feature("docstring") fastjet::SelectorWorker::is_geometric "

check if it is a geometric selector (i.e.  

only puts constraints on rapidity and azimuthal angle)  
";

%feature("docstring") fastjet::SelectorWorker::terminator "

For each jet that does not pass the cuts, this routine sets the pointer to 0.  

It does not assume that the PseudoJet* passed as argument are not NULL  
";

%feature("docstring") fastjet::SelectorWorker::known_area "

if it has a computable area, return it  
";

// File: classfastjet_1_1SharedPtr.xml


%feature("docstring") fastjet::SharedPtr "

an implementation of C++0x shared pointers (or boost's)  

this class implements a smart pointer, based on the shared+ptr proposal. A
description of shared_ptr can be found in Section 2.2.3 of the first C++
Technical Report (TR1) http://www.open-
std.org/JTC1/SC22/WG21/docs/papers/2005/n1745.pdf or, alternatively, on the
Boost C++ library website at
http://www.boost.org/doc/libs/1_42_0/libs/smart_ptr/shared_ptr.htm  

Our implementation is compatible with both of these apart from a series of
members and functions that have not been implemented:  

*   conversion from weak and auto pointers  
*   support for deleters and allocators  
*   static, constant and dynamic casts  
*   constructor and assignment sharing ownership with a shared pointer r but
    storing a different pointer than r (needed for the previous item) In the
    last 2 cases, their implementation would require storing two pointers for
    every copies of the shared pointer, while our implementation only needs one.
    We did not implement then since we want to limit as much as possible memory
    and time consumption, and can easily avoid (at least for our needs so far)
    the casts.  

We also add the possibility to force an update of the count.  

The class has been tested against the existing boost (v1.42) implementation (for
the parts that we have implemented).  

C++ includes: fastjet/SharedPtr.hh
";

%feature("docstring") fastjet::SharedPtr::~SharedPtr "

default dtor  
";

%feature("docstring") fastjet::SharedPtr::swap "

exchange the content of the two pointers  
";

%feature("docstring") fastjet::SharedPtr::unique "

check if the instance is unique  
";

%feature("docstring") fastjet::SharedPtr::set_count "

force the count to be set to a specified value  

Parameters
----------
* `count` :  
    the value that we need to reset to  
";

%feature("docstring") fastjet::SharedPtr::__attribute__ "

return the pointer we're pointing to  

Since FastJet 3.2.0, this is depracated since it is no longer part of
std::shared_ptr<T>. Use SharedPtr<T>::get() instead  
";

%feature("docstring") fastjet::SharedPtr::SharedPtr "

default ctor  
";

%feature("docstring") fastjet::SharedPtr::SharedPtr "

initialise with the main data  

Parameters
----------
* `t` :  
    : the object we want a smart pointer to  
";

%feature("docstring") fastjet::SharedPtr::SharedPtr "

overload the copy ctor so that it updates count  

Parameters
----------
* `share` :  
    : the object we want to copy  
";

%feature("docstring") fastjet::SharedPtr::get "

get the stored pointer  
";

%feature("docstring") fastjet::SharedPtr::reset "

reset the pointer to default value (NULL)  
";

%feature("docstring") fastjet::SharedPtr::reset "

reset from a pointer  
";

%feature("docstring") fastjet::SharedPtr::reset "

do a smart copy  

Parameters
----------
* `share` :  
    : the object we want to copy Q? Do we need a non-template<Y> version as for
    the ctor and the assignment?  
";

%feature("docstring") fastjet::SharedPtr::use_count "

return the number of counts  
";

// File: classfastjet_1_1ClosestPair2D_1_1Shuffle.xml

// File: classfastjet_1_1SISConeBaseExtras.xml


%feature("docstring") fastjet::SISConeBaseExtras "

Class that provides extra information about a SISCone clustering.  

This is only the base class that the \"regular\" and \"spherical\"
implementations of SISCone will have to overload. The only thing that needs to
be done for the derived classes is to define '_jet_def_plugin', implement
jet_def_plugin(); and add the corresponding plugin class as a friend  

C++ includes: fastjet/SISConeBasePlugin.hh
";

%feature("docstring") fastjet::SISConeBaseExtras::protocones "

an old name for getting the vector of stable cones (aka protocones)  
";

%feature("docstring") fastjet::SISConeBaseExtras::SISConeBaseExtras "

constructor  
";

%feature("docstring") fastjet::SISConeBaseExtras::most_ambiguous_split "

return the smallest difference in squared distance encountered during splitting
between a particle and two overlapping protojets.  
";

%feature("docstring") fastjet::SISConeBaseExtras::description "

return a brief summary of the contents of the extras object (specifically, the
number of protocones.  
";

%feature("docstring") fastjet::SISConeBaseExtras::pass "

return the # of the pass at which a given jet was found; will return -1 if the
pass is invalid  
";

%feature("docstring") fastjet::SISConeBaseExtras::stable_cones "

returns a reference to the vector of stable cones (aka protocones)  
";

%feature("docstring") fastjet::SISConeBaseExtras::~SISConeBaseExtras "

purely virtual destructor  

give the destructor its required implementation  
";

// File: classfastjet_1_1SISConeBasePlugin.xml


%feature("docstring") fastjet::SISConeBasePlugin "
";

%feature("docstring") fastjet::SISConeBasePlugin::ghost_separation_scale "
";

%feature("docstring") fastjet::SISConeBasePlugin::description "

plugin description  
";

%feature("docstring") fastjet::SISConeBasePlugin::use_jet_def_recombiner "

indicate if the jet_def's recombination scheme is being used  
";

%feature("docstring") fastjet::SISConeBasePlugin::progressive_removal "

returns true if progressive_removal is enabled  
";

%feature("docstring") fastjet::SISConeBasePlugin::set_progressive_removal "

set whether to use SISCone with progressive removal instead of the default
split_merge step.  

If progressive removal is enabled, the following SISCone variables are not used:  

*   overlap_threshold  
*   caching  
*   split_merge_stopping_scale  

The split_merge_scale choice is reinterpreted as the ordering variable for
progressive removal. It is also possible for the user to supply his/her own
function for the scale that orders progressive removal, with set_user_scale(...)  
";

%feature("docstring") fastjet::SISConeBasePlugin::run_clustering "

really do the clustering work  
";

%feature("docstring") fastjet::SISConeBasePlugin::set_use_jet_def_recombiner "

allow the user to decide if one uses the jet_def's own recombination scheme  
";

%feature("docstring") fastjet::SISConeBasePlugin::n_pass_max "

the maximum number of passes of stable-cone searching (<=0 is same as infinity).  
";

%feature("docstring") fastjet::SISConeBasePlugin::set_split_merge_stopping_scale "

set the \"split_merge_stopping_scale\": if the scale variable for all protojets
is below this, then stop the split-merge procedure and keep only those jets
found so far.  

This is useful in determination of areas of hard jets because it can be used to
avoid running the split-merging on the pure ghost-part of the event.  
";

%feature("docstring") fastjet::SISConeBasePlugin::split_merge_stopping_scale "

return the value of the split_merge_stopping_scale (see
set_split_merge_stopping_scale(...) for description)  
";

%feature("docstring") fastjet::SISConeBasePlugin::set_user_scale "

set a user-defined scale for stable-cone ordering in progressive removal  
";

%feature("docstring") fastjet::SISConeBasePlugin::user_scale "

returns the user-defined scale in use (0 if none)  
";

%feature("docstring") fastjet::SISConeBasePlugin::R "

the plugin mechanism's standard way of accessing the jet radius  
";

%feature("docstring") fastjet::SISConeBasePlugin::overlap_threshold "

Fraction of overlap energy in a jet above which jets are merged and below which
jets are split.  
";

%feature("docstring") fastjet::SISConeBasePlugin::supports_ghosted_passive_areas "

return true since there is specific support for the measurement of passive
areas, in the sense that areas determined from all particles below the ghost
separation scale will be a passive area.  
";

%feature("docstring") fastjet::SISConeBasePlugin::caching "

indicates whether caching is turned on or not.  
";

%feature("docstring") fastjet::SISConeBasePlugin::cone_radius "

the cone radius  
";

%feature("docstring") fastjet::SISConeBasePlugin::SISConeBasePlugin "

default ctor  
";

%feature("docstring") fastjet::SISConeBasePlugin::SISConeBasePlugin "

copy constructor  
";

%feature("docstring") fastjet::SISConeBasePlugin::set_ghost_separation_scale "

set the ghost separation scale for passive area determinations *just* in the
next run (strictly speaking that makes the routine a non const, so related
internal info must be stored as a mutable)  
";

// File: classfastjet_1_1SISConeExtras.xml


%feature("docstring") fastjet::SISConeExtras "

Class that provides extra information about a SISCone clustering.  

C++ includes: fastjet/SISConePlugin.hh
";

%feature("docstring") fastjet::SISConeExtras::SISConeExtras "

constructor  
";

%feature("docstring") fastjet::SISConeExtras::jet_def_plugin "

access to the siscone jet def plugin (more convenient than getting it from the
original jet definition, because here it's directly of the right type (rather
than the base type)  
";

// File: classfastjet_1_1SISConePlugin.xml


%feature("docstring") fastjet::SISConePlugin "

Implementation of the SISCone algorithm (plugin for fastjet v2.1 upwards)  

SISConePlugin is a plugin for fastjet (v2.1 upwards) that provides an interface
to the seedless infrared safe cone jet finder by Gregory Soyez and Gavin Salam.  

SISCone uses geometrical techniques to exhaustively consider all possible
distinct cones. It then finds out which ones are stable and sends the result to
the Tevatron Run-II type split-merge procedure for overlapping cones.  

Four parameters govern the \"physics\" of the algorithm:  

*   the cone_radius (this should be self-explanatory!)  
*   the overlap_threshold is the parameter which dictates how much two jets must
    overlap (pt_overlap/min(pt1,pt2)) if they are to be merged  
*   Not all particles are in stable cones in the first round of searching for
    stable cones; one can therefore optionally have the the jet finder carry out
    additional passes of searching for stable cones among particles that were in
    no stable cone in previous passes --- the maximum number of passes carried
    out is n_pass_max. If this is zero then additional passes are carried out
    until no new stable cones are found.  
*   Protojet ptmin: protojets that are below this ptmin (default = 0) are
    discarded before each iteration of the split-merge loop.  

One parameter governs some internal algorithmic shortcuts:  

*   if \"caching\" is turned on then the last event clustered by siscone is
    stored -- if the current event is identical and the cone_radius and
    n_pass_mass are identical, then the only part of the clustering that needs
    to be rerun is the split-merge part, leading to significant speed gains;
    there is a small (O(N) storage and speed) penalty for caching, so it should
    be kept off (default) if only a single overlap_threshold is used.  

The final jets can be accessed by requestion the inclusive_jets(...) from the
ClusterSequence object. Note that these PseudoJets have their user_index() set
to the index of the pass in which they were found (first pass = 0). NB: This
does not currently work for jets that consist of a single particle.  

For further information on the details of the algorithm see the SISCone paper,
arXiv:0704.0292 [JHEP 0705:086,2007].  

For documentation about the implementation, see the siscone/doc/html/index.html
file.  

C++ includes: fastjet/SISConePlugin.hh
";

%feature("docstring") fastjet::SISConePlugin::SISConePlugin "

Main constructor for the SISCone Plugin class.  

Note: wrt version prior to 2.4 this constructor differs in that a the default
value has been removed for overlap_threshold. The former has been removed
because the old default of 0.5 was found to be unsuitable in high-noise
environments; so the user should now explicitly think about the value for this
-- we recommend 0.75.  
";

%feature("docstring") fastjet::SISConePlugin::SISConePlugin "

Backwards compatible constructor for the SISCone Plugin class.  
";

%feature("docstring") fastjet::SISConePlugin::SISConePlugin "

backwards compatible constructor for the SISCone Plugin class (avoid using this
in future).  
";

%feature("docstring") fastjet::SISConePlugin::protojet_or_ghost_ptmin "

return the scale to be passed to SISCone as the protojet_ptmin -- if we have a
ghost separation scale that is above the protojet_ptmin, then the
ghost_separation_scale becomes the relevant one to use here  
";

%feature("docstring") fastjet::SISConePlugin::split_merge_on_transverse_mass "

indicates whether the split-merge orders on transverse mass or not.  

retained for backwards compatibility with 2.1.0b3  
";

%feature("docstring") fastjet::SISConePlugin::description "

return a textual description of the jet-definition implemented in this plugin  
";

%feature("docstring") fastjet::SISConePlugin::run_clustering "

given a ClusterSequence that has been filled up with initial particles, the
following function should fill up the rest of the ClusterSequence, using the
following member functions of ClusterSequence:  

*   plugin_do_ij_recombination(...)  
*   plugin_do_iB_recombination(...)  
";

%feature("docstring") fastjet::SISConePlugin::set_split_merge_on_transverse_mass "
";

%feature("docstring") fastjet::SISConePlugin::set_split_merge_use_pt_weighted_splitting "
";

%feature("docstring") fastjet::SISConePlugin::split_merge_use_pt_weighted_splitting "

indicates whether the split-merge orders on transverse mass or not.  

retained for backwards compatibility with 2.1.0b3  
";

%feature("docstring") fastjet::SISConePlugin::set_split_merge_scale "

sets scale used in split-merge  
";

%feature("docstring") fastjet::SISConePlugin::split_merge_scale "

indicates scale used in split-merge  
";

%feature("docstring") fastjet::SISConePlugin::protojet_ptmin "

minimum pt for a protojet to be considered in the split-merge step of the
algorithm  
";

// File: classfastjet_1_1SISConeSphericalExtras.xml


%feature("docstring") fastjet::SISConeSphericalExtras "

Class that provides extra information about a SISCone clustering.  

C++ includes: fastjet/SISConeSphericalPlugin.hh
";

%feature("docstring") fastjet::SISConeSphericalExtras::jet_def_plugin "

access to the siscone jet def plugin (more convenient than getting it from the
original jet definition, because here it's directly of the right type (rather
than the base type)  
";

%feature("docstring") fastjet::SISConeSphericalExtras::SISConeSphericalExtras "

constructor  
";

// File: classfastjet_1_1SISConeSphericalPlugin.xml


%feature("docstring") fastjet::SISConeSphericalPlugin "

Implementation of the spherical version of the SISCone algorithm (plugin for
fastjet v2.1 upwards)  

SISConeSphericalPlugin is a plugin for fastjet (v2.1 upwards) that provides an
interface to the seedless infrared safe cone jet finder by Gregory Soyez and
Gavin Salam.  

This is the version of SISCone using spherical coordinates. Compared to the
original cylindrical version:  

*   Particles are within a cone if their opening angle relative to the centre of
    the cone is less than R  
*   The split-merge step uses the total energy in the protojet as the ordering
    and overlap-measure variable  
*   The IR safety of the split-merge step is *not* guaranteed for events
    consisting of two back-to-back identical heavy particles that decay. This is
    because of potential degeneracies in the ordering for the split-merge step.  

    For moderate values of R the problem should not be too severe (or may even
    be absent for some values of the overlap parameter), however the user should
    be aware of the issue.  

    The default split-merge scale may change at a later date to resolve this
    issue.  

SISCone uses geometrical techniques to exhaustively consider all possible
distinct cones. It then finds out which ones are stable and sends the result to
the Tevatron Run-II type split-merge procedure for overlapping cones.  

Four parameters govern the \"physics\" of the algorithm:  

*   the cone_radius (this should be self-explanatory!)  
*   the overlap_threshold is the parameter which dictates how much two jets must
    overlap (E_overlap/min(E1,E2)) if they are to be merged  
*   Not all particles are in stable cones in the first round of searching for
    stable cones; one can therefore optionally have the the jet finder carry out
    additional passes of searching for stable cones among particles that were in
    no stable cone in previous passes --- the maximum number of passes carried
    out is n_pass_max. If this is zero then additional passes are carried out
    until no new stable cones are found.  
*   Protojet Emin: protojets that are below this Emin (default = 0) are
    discarded before each iteration of the split-merge loop.  

One parameter governs some internal algorithmic shortcuts:  

*   if \"caching\" is turned on then the last event clustered by siscone is
    stored -- if the current event is identical and the cone_radius and
    n_pass_max are identical, then the only part of the clustering that needs to
    be rerun is the split-merge part, leading to significant speed gains; there
    is a small (O(N) storage and speed) penalty for caching, so it should be
    kept off (default) if only a single overlap_threshold is used.  

The final jets can be accessed by requestion the inclusive_jets(...) from the
ClusterSequence object. Note that these PseudoJets have their user_index() set
to the index of the pass in which they were found (first pass = 0). NB: This
does not currently work for jets that consist of a single particle.  

For further information on the details of the algorithm see the SISCone paper,
arXiv:0704.0292 [JHEP 0705:086,2007].  

For documentation about the implementation, see the siscone/doc/html/index.html
file.  

C++ includes: fastjet/SISConeSphericalPlugin.hh
";

%feature("docstring") fastjet::SISConeSphericalPlugin::run_clustering "

given a ClusterSequence that has been filled up with initial particles, the
following function should fill up the rest of the ClusterSequence, using the
following member functions of ClusterSequence:  

*   plugin_do_ij_recombination(...)  
*   plugin_do_iB_recombination(...)  
";

%feature("docstring") fastjet::SISConeSphericalPlugin::protojet_Emin "

minimum energy for a protojet to be considered in the split-merge step of the
algorithm  
";

%feature("docstring") fastjet::SISConeSphericalPlugin::description "

return a textual description of the jet-definition implemented in this plugin  
";

%feature("docstring") fastjet::SISConeSphericalPlugin::split_merge_scale "

indicates scale used in split-merge  
";

%feature("docstring") fastjet::SISConeSphericalPlugin::split_merge_use_E_weighted_splitting "

indicate if the splittings are done using the anti-kt distance  
";

%feature("docstring") fastjet::SISConeSphericalPlugin::is_spherical "

returns true because this plugin is intended for spherical geometries (i.e.  

it's an e+e- algorithm).  
";

%feature("docstring") fastjet::SISConeSphericalPlugin::protojet_or_ghost_Emin "

return the scale to be passed to SISCone as the protojet_Emin -- if we have a
ghost separation scale that is above the protojet_ptmin, then the
ghost_separation_scale becomes the relevant one to use here  
";

%feature("docstring") fastjet::SISConeSphericalPlugin::SISConeSphericalPlugin "

Main constructor for the SISConeSpherical Plugin class.  
";

%feature("docstring") fastjet::SISConeSphericalPlugin::set_split_merge_scale "

sets scale used in split-merge  
";

%feature("docstring") fastjet::SISConeSphericalPlugin::supports_ghosted_passive_areas "

overload the default as we don't provide support for passive areas.  
";

%feature("docstring") fastjet::SISConeSphericalPlugin::set_split_merge_use_E_weighted_splitting "
";

// File: classfastjet_1_1siscone__plugin__internal_1_1SISConeSphericalUserScale.xml


%feature("docstring") fastjet::siscone_plugin_internal::SISConeSphericalUserScale "
";

%feature("docstring") fastjet::siscone_plugin_internal::SISConeSphericalUserScale::is_larger "

returns true id the scasle associated to jet a is larger than the scale
associated to jet b  
";

%feature("docstring") fastjet::siscone_plugin_internal::SISConeSphericalUserScale::SISConeSphericalUserScale "

ctor takes the \"fastjet-style\" user-defined scale as well as a reference to
the current cluster sequence (to access the particles if needed)  
";

// File: classfastjet_1_1siscone__plugin__internal_1_1SISConeUserScale.xml


%feature("docstring") fastjet::siscone_plugin_internal::SISConeUserScale "

class that makes the transition between the internal SISCone user-defined scale
choice (using SISCone's Cjet) and user-defined scale choices in the plugn above
(using FastJet's PseudoJets)  
";

%feature("docstring") fastjet::siscone_plugin_internal::SISConeUserScale::is_larger "

returns true id the scasle associated to jet a is larger than the scale
associated to jet b  
";

%feature("docstring") fastjet::siscone_plugin_internal::SISConeUserScale::SISConeUserScale "

ctor takes the \"fastjet-style\" user-defined scale as well as a reference to
the current cluster sequence (to access the particles if needed)  
";

// File: classfastjet_1_1Site.xml


%feature("docstring") fastjet::Site "
";

// File: classfastjet_1_1atlas_1_1stopwatch.xml


%feature("docstring") fastjet::atlas::stopwatch "
";

%feature("docstring") fastjet::atlas::stopwatch::resume "
";

%feature("docstring") fastjet::atlas::stopwatch::stopwatch "
";

%feature("docstring") fastjet::atlas::stopwatch::start "
";

%feature("docstring") fastjet::atlas::stopwatch::stop "
";

%feature("docstring") fastjet::atlas::stopwatch::pause "
";

// File: classfastjet_1_1SISConeBasePlugin_1_1UserScaleBase_1_1StructureType.xml


%feature("docstring") fastjet::SISConeBasePlugin::UserScaleBase::StructureType "

the structure that allows to store the information contained into a
siscone::Cjet (built internally in SISCone from a stable cone) into a PseudoJet  

C++ includes: fastjet/SISConeBasePlugin.hh
";

%feature("docstring") fastjet::SISConeBasePlugin::UserScaleBase::StructureType::constituent_index "

returns the index (in the original particle list) of the ith constituent  
";

%feature("docstring") fastjet::SISConeBasePlugin::UserScaleBase::StructureType::constituents "

retrieve the constituents  

if you simply need to iterate over the constituents, it will be faster to access
them via constituent(i)  
";

%feature("docstring") fastjet::SISConeBasePlugin::UserScaleBase::StructureType::size "

returns the number of constituents  
";

%feature("docstring") fastjet::SISConeBasePlugin::UserScaleBase::StructureType::StructureType "

base ctor (constructed from a ClusterSequence tin order to have access to the
initial particles  
";

%feature("docstring") fastjet::SISConeBasePlugin::UserScaleBase::StructureType::description "

the textual descripotion  
";

%feature("docstring") fastjet::SISConeBasePlugin::UserScaleBase::StructureType::has_constituents "

this structure has constituents  
";

%feature("docstring") fastjet::SISConeBasePlugin::UserScaleBase::StructureType::~StructureType "

empty virtual dtor  
";

%feature("docstring") fastjet::SISConeBasePlugin::UserScaleBase::StructureType::ordering_var2 "

returns the sm_var2 (signed ordering variable squared) for this stable cone  
";

%feature("docstring") fastjet::SISConeBasePlugin::UserScaleBase::StructureType::constituent "

returns the ith constituent (as a PseusoJet)  
";

// File: classfastjet_1_1Subtractor.xml


%feature("docstring") fastjet::Subtractor "

Class that helps perform jet background subtraction.  

This class derives from Transformer and makes use of a pointer to a
BackgroundEstimatorBase object in order to determine the background in the
vicinity of a given jet and then subtract area*background from the jet. It can
also be initialised with a specific fixed value for the background pt density.  
Input conditions
The original jet must have area support (4-vector)  
Output/structure
The underlying structure of the returned, subtracted jet (i.e. constituents,
pieces, etc.) is identical to that of the original jet.  

C++ includes: fastjet/tools/Subtractor.hh
";

/*
 configuring the behaviour 
*/

/*
 description and action 
*/

%feature("docstring") fastjet::Subtractor::Subtractor "

define a subtractor based on a BackgroundEstimator  
";

%feature("docstring") fastjet::Subtractor::Subtractor "

define a subtractor that uses a fixed value of rho, the background pt density
per unit area (which must be positive)  
";

%feature("docstring") fastjet::Subtractor::Subtractor "

define a subtractor that uses a fixed value of rho and rho_m; both must be >= 0;  
";

%feature("docstring") fastjet::Subtractor::Subtractor "

default constructor  
";

%feature("docstring") fastjet::Subtractor::set_safe_mass "

when 'safe_mass' is true, ensure that the mass of the subtracted 4-vector remain
positive  

when true, if the subtracted mass is negative, we return a 4-vector with 0 mass,
pt and phi from the subtracted 4-vector and the rapidity of the original,
unsubtracted jet.  

Note: this will be switched off by default (for backwards compatibility with
FastJet 3.0) but is highly likely to change in a future release of FastJet  
";

%feature("docstring") fastjet::Subtractor::safe_mass "

returns whether or not safety tests on the mass are included  
";

%feature("docstring") fastjet::Subtractor::result "

returns a jet that's subtracted  

Parameters
----------
* `jet` :  
    the jet that is to be subtracted  

Returns
-------
the subtracted jet  
";

%feature("docstring") fastjet::Subtractor::set_known_selectors "

This is mostly intended for cherge-hadron-subtracted type of events where we
wich to use vertex information to improve the subtraction.  

Given the following parameters:  

Parameters
----------
* `sel_known_vertex` :  
    selects the particles with a known vertex origin  
* `sel_leading_vertex` :  
    amongst the particles with a known vertex origin, select those coming from
    the leading vertex Momentum identified as coming from the leading vertex
    will be kept, momentum identified as coming from a non-leading vertex will
    be eliminated and a regular area-median subtraction will be applied on the
    4-vector sum of the particles with unknown vertex origin.  

When this is set, we shall ensure that the pt of the subtracted 4-vector is at
least the pt of the particles that are known to come from the leading vertex (if
it fails, subtraction returns the component that is known to come from the
leading vertex --- or, the original unsubtracted jet if it contains no particles
from the leading vertex). Furthermore, when safe_mass() is on, we also impose a
similar constraint on the mass of the subtracted 4-vector (if the test fails,
the longitudinal part of the subtracted 4-vector is taken from the component
that is known to come from the leading vertex).  
";

%feature("docstring") fastjet::Subtractor::set_defaults "

reset all parameters to default values  

Note: by default, the rho_m term is not included and the safety test for the
mass is not done. This is mostly for backwards compatibility with FastJet 3.0
and is highly likely to change in a future release of FastJet  
";

%feature("docstring") fastjet::Subtractor::description "

class description  
";

%feature("docstring") fastjet::Subtractor::set_use_rho_m "

when 'use_rho_m' is true, include in the subtraction the correction from rho_m,
the purely longitudinal, particle-mass-induced component of the background
density per unit area  

Note: this will be switched off by default (for backwards compatibility with
FastJet 3.0) but is highly likely to change in a future release of FastJet  
";

%feature("docstring") fastjet::Subtractor::use_rho_m "

returns whether or not the rho_m component is used  
";

%feature("docstring") fastjet::Subtractor::~Subtractor "

default dtor  
";

// File: structfastjet_1_1DnnPlane_1_1SuperVertex.xml

// File: classfastjet_1_1SW__AbsRapMax.xml


%feature("docstring") fastjet::SW_AbsRapMax "

helper for selecting on |rapidities|: max  
";

%feature("docstring") fastjet::SW_AbsRapMax::SW_AbsRapMax "
";

%feature("docstring") fastjet::SW_AbsRapMax::get_rapidity_extent "
";

%feature("docstring") fastjet::SW_AbsRapMax::known_area "
";

%feature("docstring") fastjet::SW_AbsRapMax::has_known_area "

the area is analytically known  
";

// File: classfastjet_1_1SW__AbsRapRange.xml


%feature("docstring") fastjet::SW_AbsRapRange "

helper for selecting on |rapidities|: max  
";

%feature("docstring") fastjet::SW_AbsRapRange::SW_AbsRapRange "
";

%feature("docstring") fastjet::SW_AbsRapRange::get_rapidity_extent "
";

%feature("docstring") fastjet::SW_AbsRapRange::known_area "
";

%feature("docstring") fastjet::SW_AbsRapRange::has_known_area "

the area is analytically known  
";

// File: classfastjet_1_1SW__And.xml


%feature("docstring") fastjet::SW_And "

helper for combining selectors with a logical and  
";

%feature("docstring") fastjet::SW_And::get_rapidity_extent "

returns the rapidity range for which it may return \"true\"  
";

%feature("docstring") fastjet::SW_And::description "

returns a description of the worker  
";

%feature("docstring") fastjet::SW_And::terminator "

select the jets in the list that pass both selectors  
";

%feature("docstring") fastjet::SW_And::SW_And "

ctor  
";

%feature("docstring") fastjet::SW_And::pass "

returns true if a given object passes the selection criterium this has to be
overloaded by derived workers  
";

%feature("docstring") fastjet::SW_And::copy "

return a copy of this  
";

// File: classfastjet_1_1SW__BinaryOperator.xml


%feature("docstring") fastjet::SW_BinaryOperator "

Base class for binary operators.  
";

%feature("docstring") fastjet::SW_BinaryOperator::set_reference "

sets the reference jet  
";

%feature("docstring") fastjet::SW_BinaryOperator::SW_BinaryOperator "

ctor  
";

%feature("docstring") fastjet::SW_BinaryOperator::takes_reference "

returns true if this takes a reference jet  
";

%feature("docstring") fastjet::SW_BinaryOperator::applies_jet_by_jet "

returns true if this can be applied jet by jet  
";

%feature("docstring") fastjet::SW_BinaryOperator::is_geometric "

check if it has a finite area  
";

// File: classfastjet_1_1SW__Circle.xml


%feature("docstring") fastjet::SW_Circle "

helper for selecting on objects within a distance 'radius' of a reference  
";

%feature("docstring") fastjet::SW_Circle::pass "

returns true if a given object passes the selection criterium this has to be
overloaded by derived workers  
";

%feature("docstring") fastjet::SW_Circle::has_finite_area "

regardless of the reference  
";

%feature("docstring") fastjet::SW_Circle::has_known_area "

the area is analytically known  
";

%feature("docstring") fastjet::SW_Circle::get_rapidity_extent "

returns the rapidity range for which it may return \"true\"  
";

%feature("docstring") fastjet::SW_Circle::SW_Circle "
";

%feature("docstring") fastjet::SW_Circle::description "

returns a description of the worker  
";

%feature("docstring") fastjet::SW_Circle::is_geometric "

implies a finite area  
";

%feature("docstring") fastjet::SW_Circle::known_area "
";

%feature("docstring") fastjet::SW_Circle::copy "

return a copy of the current object  
";

// File: classfastjet_1_1SW__Doughnut.xml


%feature("docstring") fastjet::SW_Doughnut "

helper for selecting on objects with a distance to a reference betwene
'radius_in' and 'radius_out'  
";

%feature("docstring") fastjet::SW_Doughnut::has_finite_area "

regardless of the reference  
";

%feature("docstring") fastjet::SW_Doughnut::is_geometric "

implies a finite area  
";

%feature("docstring") fastjet::SW_Doughnut::known_area "
";

%feature("docstring") fastjet::SW_Doughnut::get_rapidity_extent "

returns the rapidity range for which it may return \"true\"  
";

%feature("docstring") fastjet::SW_Doughnut::description "

returns a description of the worker  
";

%feature("docstring") fastjet::SW_Doughnut::has_known_area "

the area is analytically known  
";

%feature("docstring") fastjet::SW_Doughnut::copy "

return a copy of the current object  
";

%feature("docstring") fastjet::SW_Doughnut::SW_Doughnut "
";

%feature("docstring") fastjet::SW_Doughnut::pass "

returns true if a given object passes the selection criterium this has to be
overloaded by derived workers  
";

// File: classfastjet_1_1SW__Identity.xml


%feature("docstring") fastjet::SW_Identity "

helper for selecting the n hardest jets  
";

%feature("docstring") fastjet::SW_Identity::SW_Identity "

ctor with specification of the number of objects to keep  
";

%feature("docstring") fastjet::SW_Identity::terminator "

For each jet that does not pass the cuts, this routine sets the pointer to 0.  
";

%feature("docstring") fastjet::SW_Identity::description "

returns a description of the worker  
";

%feature("docstring") fastjet::SW_Identity::is_geometric "

strictly speaking, this is geometric  
";

%feature("docstring") fastjet::SW_Identity::pass "

just let everything pass  
";

// File: classSW__IsPi0Gamma.xml


%feature("docstring") SW_IsPi0Gamma "
";

%feature("docstring") SW_IsPi0Gamma::SW_IsPi0Gamma "
";

%feature("docstring") SW_IsPi0Gamma::description "
";

%feature("docstring") SW_IsPi0Gamma::pass "
";

// File: classfastjet_1_1SW__IsPureGhost.xml


%feature("docstring") fastjet::SW_IsPureGhost "

helper for selecting the pure ghost  
";

%feature("docstring") fastjet::SW_IsPureGhost::SW_IsPureGhost "

ctor  
";

%feature("docstring") fastjet::SW_IsPureGhost::pass "

return true if the jet is a pure-ghost jet  
";

%feature("docstring") fastjet::SW_IsPureGhost::description "

rereturns a description of the worker  
";

// File: classfastjet_1_1SW__IsZero.xml


%feature("docstring") fastjet::SW_IsZero "

helper for selecting the 0-momentum jets  
";

%feature("docstring") fastjet::SW_IsZero::SW_IsZero "

ctor  
";

%feature("docstring") fastjet::SW_IsZero::pass "

return true if the jet has zero momentum  
";

%feature("docstring") fastjet::SW_IsZero::description "

rereturns a description of the worker  
";

// File: classfastjet_1_1SW__Mult.xml


%feature("docstring") fastjet::SW_Mult "

helper for multiplying two selectors (in an operator-like way)  
";

%feature("docstring") fastjet::SW_Mult::SW_Mult "

ctor  
";

%feature("docstring") fastjet::SW_Mult::terminator "

select the jets in the list that pass both selectors  
";

%feature("docstring") fastjet::SW_Mult::copy "

return a copy of this  
";

%feature("docstring") fastjet::SW_Mult::description "

returns a description of the worker  
";

// File: classfastjet_1_1SW__NHardest.xml


%feature("docstring") fastjet::SW_NHardest "

helper for selecting the n hardest jets  
";

%feature("docstring") fastjet::SW_NHardest::terminator "

For each jet that does not pass the cuts, this routine sets the pointer to 0.  
";

%feature("docstring") fastjet::SW_NHardest::pass "

pass makes no sense here normally the parent selector will throw an error but
for internal use in the SW, we'll throw one from here by security  
";

%feature("docstring") fastjet::SW_NHardest::applies_jet_by_jet "

returns true if this can be applied jet by jet  
";

%feature("docstring") fastjet::SW_NHardest::description "

returns a description of the worker  
";

%feature("docstring") fastjet::SW_NHardest::SW_NHardest "

ctor with specification of the number of objects to keep  
";

// File: classfastjet_1_1SW__Not.xml


%feature("docstring") fastjet::SW_Not "

helper for combining selectors with a logical not  
";

%feature("docstring") fastjet::SW_Not::pass "

returns true if a given object passes the selection criterium this has to be
overloaded by derived workers  
";

%feature("docstring") fastjet::SW_Not::copy "

return a copy of the current object  
";

%feature("docstring") fastjet::SW_Not::terminator "

select the jets in the list that pass both selectors  
";

%feature("docstring") fastjet::SW_Not::SW_Not "

ctor  
";

%feature("docstring") fastjet::SW_Not::set_reference "

set the reference jet for this selector  
";

%feature("docstring") fastjet::SW_Not::description "

returns a description of the worker  
";

%feature("docstring") fastjet::SW_Not::is_geometric "

is geometric if the underlying selector is  
";

%feature("docstring") fastjet::SW_Not::takes_reference "

returns true if the worker can be set_referenced  
";

%feature("docstring") fastjet::SW_Not::applies_jet_by_jet "

returns true if this can be applied jet by jet  
";

// File: classfastjet_1_1SW__Or.xml


%feature("docstring") fastjet::SW_Or "

helper for combining selectors with a logical or  
";

%feature("docstring") fastjet::SW_Or::get_rapidity_extent "

returns the rapidity range for which it may return \"true\"  
";

%feature("docstring") fastjet::SW_Or::description "

returns a description of the worker  
";

%feature("docstring") fastjet::SW_Or::applies_jet_by_jet "

returns true if this can be applied jet by jet  
";

%feature("docstring") fastjet::SW_Or::SW_Or "

ctor  
";

%feature("docstring") fastjet::SW_Or::copy "

return a copy of this  
";

%feature("docstring") fastjet::SW_Or::terminator "

select the jets in the list that pass both selectors  
";

%feature("docstring") fastjet::SW_Or::pass "

returns true if a given object passes the selection criterium this has to be
overloaded by derived workers  
";

// File: classfastjet_1_1SW__PhiRange.xml


%feature("docstring") fastjet::SW_PhiRange "

helper for selecting on azimuthal angle  

Note that the bounds have to be specified as min<max phimin=\"\" has=\"\"
to=\"\" be=\"\"> -2pi phimax has to be < 4pi  
";

%feature("docstring") fastjet::SW_PhiRange::SW_PhiRange "

detfault ctor (initialises the pt cut)  
";

%feature("docstring") fastjet::SW_PhiRange::description "

returns a description of the worker  
";

%feature("docstring") fastjet::SW_PhiRange::is_geometric "
";

%feature("docstring") fastjet::SW_PhiRange::pass "

returns true is the given object passes the selection pt cut  
";

// File: classfastjet_1_1SW__PtFractionMin.xml


%feature("docstring") fastjet::SW_PtFractionMin "

helper for selecting the jets that carry at least a given fraction of the
reference jet  
";

%feature("docstring") fastjet::SW_PtFractionMin::SW_PtFractionMin "

ctor with specification of the number of objects to keep  
";

%feature("docstring") fastjet::SW_PtFractionMin::pass "

return true if the jet carries a large enough fraction of the reference.  

Throw an error if the reference is not initialised.  
";

%feature("docstring") fastjet::SW_PtFractionMin::description "

returns a description of the worker  
";

%feature("docstring") fastjet::SW_PtFractionMin::copy "

return a copy of the current object  
";

// File: classfastjet_1_1SW__QuantityMax.xml


%feature("docstring") fastjet::SW_QuantityMax "
";

%feature("docstring") fastjet::SW_QuantityMax::SW_QuantityMax "

detfault ctor (initialises the pt cut)  
";

%feature("docstring") fastjet::SW_QuantityMax::pass "

returns true is the given object passes the selection pt cut  
";

%feature("docstring") fastjet::SW_QuantityMax::description "

returns a description of the worker  
";

%feature("docstring") fastjet::SW_QuantityMax::is_geometric "
";

// File: classfastjet_1_1SW__QuantityMin.xml


%feature("docstring") fastjet::SW_QuantityMin "
";

%feature("docstring") fastjet::SW_QuantityMin::SW_QuantityMin "

detfault ctor (initialises the pt cut)  
";

%feature("docstring") fastjet::SW_QuantityMin::is_geometric "
";

%feature("docstring") fastjet::SW_QuantityMin::pass "

returns true is the given object passes the selection pt cut  
";

%feature("docstring") fastjet::SW_QuantityMin::description "

returns a description of the worker  
";

// File: classfastjet_1_1SW__QuantityRange.xml


%feature("docstring") fastjet::SW_QuantityRange "
";

%feature("docstring") fastjet::SW_QuantityRange::pass "

returns true is the given object passes the selection pt cut  
";

%feature("docstring") fastjet::SW_QuantityRange::is_geometric "
";

%feature("docstring") fastjet::SW_QuantityRange::SW_QuantityRange "

detfault ctor (initialises the pt cut)  
";

%feature("docstring") fastjet::SW_QuantityRange::description "

returns a description of the worker  
";

// File: classfastjet_1_1SW__RangeDefinition.xml


%feature("docstring") fastjet::SW_RangeDefinition "

helper for selecting on both rapidity and azimuthal angle  
";

%feature("docstring") fastjet::SW_RangeDefinition::known_area "

if it has a computable area, return it  
";

%feature("docstring") fastjet::SW_RangeDefinition::get_rapidity_extent "

returns the rapidity range for which it may return \"true\"  
";

%feature("docstring") fastjet::SW_RangeDefinition::description "

returns a description of the worker  
";

%feature("docstring") fastjet::SW_RangeDefinition::SW_RangeDefinition "

ctor from a RangeDefinition  
";

%feature("docstring") fastjet::SW_RangeDefinition::is_geometric "

check if it has a finite area  
";

%feature("docstring") fastjet::SW_RangeDefinition::pass "

transfer the selection creterium to the underlying RangeDefinition  
";

%feature("docstring") fastjet::SW_RangeDefinition::has_known_area "

check if it has an analytically computable area  
";

// File: classfastjet_1_1SW__RapMax.xml


%feature("docstring") fastjet::SW_RapMax "

helper for selecting on rapidities: max  
";

%feature("docstring") fastjet::SW_RapMax::SW_RapMax "
";

%feature("docstring") fastjet::SW_RapMax::get_rapidity_extent "
";

// File: classfastjet_1_1SW__RapMin.xml


%feature("docstring") fastjet::SW_RapMin "

helper for selecting on rapidities: min  
";

%feature("docstring") fastjet::SW_RapMin::SW_RapMin "
";

%feature("docstring") fastjet::SW_RapMin::get_rapidity_extent "
";

// File: classfastjet_1_1SW__RapPhiRange.xml


%feature("docstring") fastjet::SW_RapPhiRange "

helper for selecting on both rapidity and azimuthal angle  
";

%feature("docstring") fastjet::SW_RapPhiRange::SW_RapPhiRange "
";

%feature("docstring") fastjet::SW_RapPhiRange::known_area "

if it has a computable area, return it  
";

// File: classfastjet_1_1SW__RapRange.xml


%feature("docstring") fastjet::SW_RapRange "

helper for selecting on rapidities: range  
";

%feature("docstring") fastjet::SW_RapRange::SW_RapRange "
";

%feature("docstring") fastjet::SW_RapRange::known_area "
";

%feature("docstring") fastjet::SW_RapRange::has_known_area "

the area is analytically known  
";

%feature("docstring") fastjet::SW_RapRange::get_rapidity_extent "
";

// File: classfastjet_1_1SW__Rectangle.xml


%feature("docstring") fastjet::SW_Rectangle "

helper for selecting on objects with rapidity within a distance 'delta_rap' of a
reference and phi within a distanve delta_phi of a reference  
";

%feature("docstring") fastjet::SW_Rectangle::is_geometric "

implies a finite area  
";

%feature("docstring") fastjet::SW_Rectangle::has_finite_area "

regardless of the reference  
";

%feature("docstring") fastjet::SW_Rectangle::get_rapidity_extent "

returns the rapidity range for which it may return \"true\"  
";

%feature("docstring") fastjet::SW_Rectangle::description "

returns a description of the worker  
";

%feature("docstring") fastjet::SW_Rectangle::has_known_area "

the area is analytically known  
";

%feature("docstring") fastjet::SW_Rectangle::pass "

returns true if a given object passes the selection criterium this has to be
overloaded by derived workers  
";

%feature("docstring") fastjet::SW_Rectangle::SW_Rectangle "
";

%feature("docstring") fastjet::SW_Rectangle::known_area "
";

%feature("docstring") fastjet::SW_Rectangle::copy "

return a copy of the current object  
";

// File: classfastjet_1_1SW__Strip.xml


%feature("docstring") fastjet::SW_Strip "

helper for selecting on objects with rapidity within a distance 'delta' of a
reference  
";

%feature("docstring") fastjet::SW_Strip::description "

returns a description of the worker  
";

%feature("docstring") fastjet::SW_Strip::pass "

returns true if a given object passes the selection criterium this has to be
overloaded by derived workers  
";

%feature("docstring") fastjet::SW_Strip::known_area "
";

%feature("docstring") fastjet::SW_Strip::SW_Strip "
";

%feature("docstring") fastjet::SW_Strip::is_geometric "

implies a finite area  
";

%feature("docstring") fastjet::SW_Strip::copy "

return a copy of the current object  
";

%feature("docstring") fastjet::SW_Strip::get_rapidity_extent "

returns the rapidity range for which it may return \"true\"  
";

%feature("docstring") fastjet::SW_Strip::has_finite_area "

regardless of the reference  
";

%feature("docstring") fastjet::SW_Strip::has_known_area "

the area is analytically known  
";

// File: classSW__VertexNumber.xml


%feature("docstring") SW_VertexNumber "
";

%feature("docstring") SW_VertexNumber::description "
";

%feature("docstring") SW_VertexNumber::SW_VertexNumber "
";

%feature("docstring") SW_VertexNumber::pass "
";

// File: classfastjet_1_1SW__WithReference.xml


%feature("docstring") fastjet::SW_WithReference "

a generic class for objects that contain a position  
";

%feature("docstring") fastjet::SW_WithReference::set_reference "

sets the reference jet  
";

%feature("docstring") fastjet::SW_WithReference::SW_WithReference "

ctor  
";

%feature("docstring") fastjet::SW_WithReference::takes_reference "

returns true if the worker takes a reference jet  
";

// File: classfastjet_1_1d0runi_1_1ConeClusterAlgo_1_1TemporaryJet.xml

// File: classfastjet_1_1NNFJN2Tiled_1_1Tile.xml

// File: classfastjet_1_1Tile.xml


%feature("docstring") fastjet::Tile "
";

%feature("docstring") fastjet::Tile::distance_to_top "
";

%feature("docstring") fastjet::Tile::distance_to_left "
";

%feature("docstring") fastjet::Tile::distance_to_left_top "
";

%feature("docstring") fastjet::Tile::distance_to_centre "
";

%feature("docstring") fastjet::Tile::distance_to_bottom "
";

%feature("docstring") fastjet::Tile::distance_to_left_bottom "
";

%feature("docstring") fastjet::Tile::distance_to_right "
";

%feature("docstring") fastjet::Tile::distance_to_right_bottom "
";

%feature("docstring") fastjet::Tile::distance_to_right_top "
";

// File: structfastjet_1_1ClusterSequence_1_1Tile.xml

// File: classfastjet_1_1Tile2Base.xml


%feature("docstring") fastjet::Tile2Base "
";

%feature("docstring") fastjet::Tile2Base::jet_count "

returns the number of jets in the tile; useful principally for diagnostics  
";

// File: classfastjet_1_1Tile3.xml


%feature("docstring") fastjet::Tile3 "
";

%feature("docstring") fastjet::Tile3::is_near_zero_phi "
";

// File: classfastjet_1_1TiledJet.xml


%feature("docstring") fastjet::TiledJet "

structure analogous to BriefJet, but with the extra information needed for
dealing with tiles  

C++ includes: fastjet/internal/LazyTiling9Alt.hh
";

%feature("docstring") fastjet::TiledJet::label_minheap_update_done "
";

%feature("docstring") fastjet::TiledJet::minheap_update_needed "
";

%feature("docstring") fastjet::TiledJet::label_minheap_update_needed "
";

// File: classfastjet_1_1NNFJN2Tiled_1_1TiledJet.xml

// File: classfastjet_1_1ClusterSequence_1_1TiledJet.xml

// File: classfastjet_1_1TiledJet3.xml


%feature("docstring") fastjet::TiledJet3 "
";

%feature("docstring") fastjet::TiledJet3::label_minheap_update_done "
";

%feature("docstring") fastjet::TiledJet3::minheap_update_needed "
";

%feature("docstring") fastjet::TiledJet3::label_minheap_update_needed "
";

// File: classfastjet_1_1TilingBase.xml


%feature("docstring") fastjet::TilingBase "

Class to indicate generic structure of tilings.  

C++ includes: fastjet/RectangularGrid.hh
";

%feature("docstring") fastjet::TilingBase::mean_tile_area "

returns the mean area of the tiles.  
";

%feature("docstring") fastjet::TilingBase::is_initialized "
";

%feature("docstring") fastjet::TilingBase::description "

returns a string to describe the tiling  
";

%feature("docstring") fastjet::TilingBase::all_tiles_good "

returns whether all tiles are good  
";

%feature("docstring") fastjet::TilingBase::tile_is_good "

returns whether a given tile is good  
";

%feature("docstring") fastjet::TilingBase::n_tiles "

returns the total number of tiles in the tiling; valid tile indices run from 0
...  

n_tiles()-1;  
";

%feature("docstring") fastjet::TilingBase::~TilingBase "

virtual destructor  
";

%feature("docstring") fastjet::TilingBase::tile_area "

returns the area of tile itile.  

Here with a default implementation to return mean_tile_area(), consistent with
the fact that all_tiles_equal_area() returns true.  
";

%feature("docstring") fastjet::TilingBase::n_good_tiles "

returns the number of tiles that are \"good\"; i.e.  

there is scope for having tiles that, for whatever reason, should be ignored;
there are situations in which having \"non-good\" tiles may be the simplest
mechanism to obtain a tiling with holes in it  
";

%feature("docstring") fastjet::TilingBase::tile_index "

returns the index of the tile in which p is located, or -1 if p is outside the
tiling region  
";

%feature("docstring") fastjet::TilingBase::is_initialised "

returns true if the Tiling structure is in a suitably initialised state  
";

%feature("docstring") fastjet::TilingBase::all_tiles_equal_area "

returns true if all tiles have the same area  
";

// File: classfastjet_1_1TilingExtent.xml


%feature("docstring") fastjet::TilingExtent "

class to perform a fast analysis of the appropriate rapidity range in which to
perform tiling  

C++ includes: fastjet/internal/TilingExtent.hh
";

%feature("docstring") fastjet::TilingExtent::maxrap "

returns the suggested maximum rapidity for the tiling  
";

%feature("docstring") fastjet::TilingExtent::minrap "

returns the suggested minimum rapidity for the tiling  
";

%feature("docstring") fastjet::TilingExtent::TilingExtent "

constructor that takes a ClusterSequence in a state where the initial particles
have been set up, but before clustering has started.  
";

%feature("docstring") fastjet::TilingExtent::TilingExtent "

constructor that takes a list of PseudoJets  
";

%feature("docstring") fastjet::TilingExtent::sum_of_binned_squared_multiplicity "

internally, the class bins the particle multiplicity versus rapidity, in bins of
size 1 running roughly from minrap to maxrap (including overflows); this
function returns the sum of squares of bin contents, which may be informative
for deciding strategy choices.  
";

// File: classfastjet_1_1TopTaggerBase.xml


%feature("docstring") fastjet::TopTaggerBase "

A base class that provides a common interface for top taggers that are able to
return a W (in addition to the top itself).  

Top taggers that derive from this should satisfy the following criteria:  

*   their underlying structure should derive from TopTaggerBaseStructure  
*   tagged tops should have two pieces, the first of which is the W candidate  
*   they should apply the top and W selectors to decide if the top has been
    tagged  

C++ includes: fastjet/tools/TopTaggerBase.hh
";

%feature("docstring") fastjet::TopTaggerBase::description_of_selectors "

returns a description of the top and W selectors  
";

%feature("docstring") fastjet::TopTaggerBase::TopTaggerBase "
";

%feature("docstring") fastjet::TopTaggerBase::set_top_selector "

sets the selector that is applied to the top candidate  
";

%feature("docstring") fastjet::TopTaggerBase::set_W_selector "

sets the selector that is applied to the W candidate  
";

// File: classfastjet_1_1TopTaggerBaseStructure.xml


%feature("docstring") fastjet::TopTaggerBaseStructure "

class that specifies the structure common to all top taggers  

Note that this specifies only the W, non_W part of the interface. An actual top
tagger structure class will also need to derive from a PseudoJetStructureBase
type class (e.g. CompositeJetStructure)  

C++ includes: fastjet/tools/TopTaggerBase.hh
";

%feature("docstring") fastjet::TopTaggerBaseStructure::~TopTaggerBaseStructure "
";

%feature("docstring") fastjet::TopTaggerBaseStructure::W "
";

%feature("docstring") fastjet::TopTaggerBaseStructure::non_W "
";

// File: classfastjet_1_1TrackJetParticlePtr.xml


%feature("docstring") fastjet::TrackJetParticlePtr "
";

%feature("docstring") fastjet::TrackJetParticlePtr::TrackJetParticlePtr "
";

// File: classfastjet_1_1TrackJetPlugin.xml


%feature("docstring") fastjet::TrackJetPlugin "

Implementation of the TrackJet algorithm (plugin for fastjet v2.4 upwards)  

C++ includes: fastjet/TrackJetPlugin.hh
";

%feature("docstring") fastjet::TrackJetPlugin::TrackJetPlugin "

Main constructor for the TrackJet Plugin class.  

The argument is an initialised list of jet algorithms  

Parameters
----------
* `_radius` :  
    the distance at which point a particle is no longer recombied into the jet  
* `jet_recombination_scheme` :  
    the recombination scheme used to sum the 4-vecors inside the jet  
* `track_recombination_scheme` :  
    the recombination scheme used to sum the 4-vecors when accumulating track
    into a the jet Both recombiners are defaulted to pt_scheme recomb as for the
    Rivet implementation.  
";

%feature("docstring") fastjet::TrackJetPlugin::TrackJetPlugin "

copy constructor  
";

%feature("docstring") fastjet::TrackJetPlugin::run_clustering "

given a ClusterSequence that has been filled up with initial particles, the
following function should fill up the rest of the ClusterSequence, using the
following member functions of ClusterSequence:  

*   plugin_do_ij_recombination(...)  
*   plugin_do_iB_recombination(...)  
";

%feature("docstring") fastjet::TrackJetPlugin::R "

the plugin mechanism's standard way of accessing the jet radius here we return
the R of the last alg in the list  
";

%feature("docstring") fastjet::TrackJetPlugin::description "

return a textual description of the jet-definition implemented in this plugin  
";

// File: classfastjet_1_1Transformer.xml


%feature("docstring") fastjet::Transformer "

Base (abstract) class for a jet transformer.  

A transformer, when it acts on a jet, returns a modified version of that jet,
one that may have a different momentum and/or different internal structure.  

The typical usage of a class derived from Transformer is  

For many transformers, the transformed jets have transformer-specific
information that can be accessed through the  


See the description of the Filter class for a more detailed usage example. See
the FastJet manual to find out how to implement new transformers.  

C++ includes: fastjet/tools/Transformer.hh
";

%feature("docstring") fastjet::Transformer::~Transformer "

default dtor  
";

%feature("docstring") fastjet::Transformer::result "

the result of the Transformer acting on the PseudoJet.  

this *has* to be overloaded in derived classes  

Parameters
----------
* `original` :  
    the PseudoJet input to the Transformer  
";

%feature("docstring") fastjet::Transformer::Transformer "

default ctor  
";

%feature("docstring") fastjet::Transformer::description "

This should be overloaded to return a description of the Transformer.  
";

// File: classfastjet_1_1ClosestPair2D_1_1triplet.xml

// File: classfastjet_1_1Unboost.xml


%feature("docstring") fastjet::Unboost "

Class to un-boost a PseudoJet.  

This is a FunctionOfPseudoJet with return type PseudoJet. Its action if to un-
boost the PseudoJet back in the restframe of the PseudoJet passed to its
constructor  

C++ includes: fastjet/tools/Boost.hh
";

%feature("docstring") fastjet::Unboost::result "

the action of the function: boost the PseudoJet to the rest frame of _jet_rest  
";

%feature("docstring") fastjet::Unboost::Unboost "

default ctor  
";

// File: classfastjet_1_1PseudoJet_1_1UserInfoBase.xml


%feature("docstring") fastjet::PseudoJet::UserInfoBase "

a base class to hold extra user information in a PseudoJet  

This is a base class to help associate extra user information with a jet. The
user should store their information in a class derived from this. This allows
information of arbitrary complexity to be easily associated with a PseudoJet (in
contrast to the user index). For example, in a Monte Carlo simulation, the user
information might include the PDG ID, and the position of the production vertex
for the particle.  

The PseudoJet is able to store a shared pointer to any object derived from
UserInfo. The use of a shared pointer frees the user of the need to handle the
memory management associated with the information.  

Having the user information derive from a common base class also facilitates
dynamic casting, etc.  

C++ includes: fastjet/PseudoJet.hh
";

%feature("docstring") fastjet::PseudoJet::UserInfoBase::~UserInfoBase "
";

%feature("docstring") fastjet::PseudoJet::UserInfoBase::UserInfoBase "
";

// File: classfastjet_1_1SISConeBasePlugin_1_1UserScaleBase.xml


%feature("docstring") fastjet::SISConeBasePlugin::UserScaleBase "

base class for user-defined ordering of stable cones (used for prorgessive
removal)  

derived classes have to implement the () operator that returns the scale
associated with a given jet.  

It is also highly recommended to implement the is_larger() method whenever
possible, in order to avoid rounding issues known to lead to possible infrared
unsafeties.  

The jets that are passed to this class will carry the structure of type
SISConePlugin::StructureType which allows to retreive easily the following
information:  

vector<PseudoJet> constituents = jet.constituents(); unsigned int n_constituents
= jet.structure_of<SISConePlugin::UserScaleBase>().size(); int index =
jet.structure_of<SISConePlugin::UserScaleBase>().constituent_index(index i);
const PseudoJet & p =
jet.structure_of<SISConePlugin::UserScaleBase>().constituent(index i); double
scalar_pt = jet.structure_of<SISConePlugin::UserScaleBase>().pt_tilde();  

see SISConePlugin::StructureType below for further details  

C++ includes: fastjet/SISConeBasePlugin.hh
";

%feature("docstring") fastjet::SISConeBasePlugin::UserScaleBase::result "

returns the scale associated with a given jet  

\"progressive removal\" iteratively removes the stable cone with the largest
scale  
";

%feature("docstring") fastjet::SISConeBasePlugin::UserScaleBase::~UserScaleBase "

empty virtual dtor  
";

%feature("docstring") fastjet::SISConeBasePlugin::UserScaleBase::is_larger "

returns true when the scale associated with jet a is larger than the scale
associated with jet b  

By default this does a simple direct comparison but it can be overloaded for
higher precision [recommended if possible]  
";

// File: classfastjet_1_1SISConeBasePlugin_1_1UserScaleBaseStructureType.xml


%feature("docstring") fastjet::SISConeBasePlugin::UserScaleBaseStructureType "

template class derived from UserScaleBase::StryctureType that works for both
SISCone jet classes implemented below  

C++ includes: fastjet/SISConeBasePlugin.hh
";

%feature("docstring") fastjet::SISConeBasePlugin::UserScaleBaseStructureType::~UserScaleBaseStructureType "

empty virtual dtor  
";

%feature("docstring") fastjet::SISConeBasePlugin::UserScaleBaseStructureType::ordering_var2 "

returns the sm_var2 (signed ordering variable squared) for this stable cone  
";

%feature("docstring") fastjet::SISConeBasePlugin::UserScaleBaseStructureType::constituent_index "

returns the index (in the original particle list) of the ith constituent  
";

%feature("docstring") fastjet::SISConeBasePlugin::UserScaleBaseStructureType::size "

returns the number of constituents  
";

%feature("docstring") fastjet::SISConeBasePlugin::UserScaleBaseStructureType::UserScaleBaseStructureType "
";

// File: structfastjet_1_1MinHeap_1_1ValueLoc.xml

// File: classfastjet_1_1ClusterSequenceVoronoiArea_1_1VoronoiAreaCalc.xml


%feature("docstring") fastjet::ClusterSequenceVoronoiArea::VoronoiAreaCalc "

class for carrying out a voronoi area calculation on a set of initial vectors  
";

%feature("docstring") fastjet::ClusterSequenceVoronoiArea::VoronoiAreaCalc::VoronoiAreaCalc "

constructor that takes a range of a vector together with the effective radius
for the intersection of discs with voronoi cells  
";

%feature("docstring") fastjet::ClusterSequenceVoronoiArea::VoronoiAreaCalc::area "

return the area of the particle associated with the given index  
";

// File: classfastjet_1_1VoronoiAreaSpec.xml


%feature("docstring") fastjet::VoronoiAreaSpec "

Specification for the computation of the Voronoi jet area.  

class for holding a \"Voronoi area\" specification; an area will be assigned to
each particle, which is the area of the intersection of the particle's Voronoi
cell with a circle of radius R*effective_Rfact.  

C++ includes: fastjet/AreaDefinition.hh
";

%feature("docstring") fastjet::VoronoiAreaSpec::effective_Rfact "

return the value of effective_Rfact  
";

%feature("docstring") fastjet::VoronoiAreaSpec::description "

return a textual description of the area definition.  
";

%feature("docstring") fastjet::VoronoiAreaSpec::VoronoiAreaSpec "

default constructor (effective_Rfact = 1);  
";

%feature("docstring") fastjet::VoronoiAreaSpec::VoronoiAreaSpec "

constructor that allows you to set effective_Rfact.  
";

// File: classfastjet_1_1VoronoiDiagramGenerator.xml


%feature("docstring") fastjet::VoronoiDiagramGenerator "
";

%feature("docstring") fastjet::VoronoiDiagramGenerator::getNext "
";

%feature("docstring") fastjet::VoronoiDiagramGenerator::VoronoiDiagramGenerator "
";

%feature("docstring") fastjet::VoronoiDiagramGenerator::resetIterator "
";

%feature("docstring") fastjet::VoronoiDiagramGenerator::~VoronoiDiagramGenerator "
";

%feature("docstring") fastjet::VoronoiDiagramGenerator::generateVoronoi "
";

// File: classfastjet_1_1VPoint.xml


%feature("docstring") fastjet::VPoint "
";

%feature("docstring") fastjet::VPoint::VPoint "

defailt ctor  
";

%feature("docstring") fastjet::VPoint::VPoint "

ctor with initialisation  
";

// File: classfastjet_1_1WrappedStructure.xml


%feature("docstring") fastjet::WrappedStructure "

This wraps a (shared) pointer to an underlying structure.  

The typical use-case is when a PseusoJet needs to share its structure with
another PseudoJet but also include extra information in its structure. For the
memory management to be handled properly, it should hold a shared pointer to the
shared structure. This is what this class ensures. Deriving a structure from
this class would then allow for the implementation of the extra features.  

C++ includes: fastjet/WrappedStructure.hh
";

/*
 Direct access to the associated ClusterSequence object. 
*/

/*
Get access to the associated ClusterSequence (if any)  

*/

/*
 Methods for access to information about jet structure 
*/

/*
These allow access to jet constituents, and other jet subtructure information.  

They only work if the jet is associated with a ClusterSequence.  

*/

%feature("docstring") fastjet::WrappedStructure::description "

description  
";

%feature("docstring") fastjet::WrappedStructure::associated_cluster_sequence "

get a (const) pointer to the parent ClusterSequence (NULL if inexistent)  
";

%feature("docstring") fastjet::WrappedStructure::has_valid_cluster_sequence "

returns true if this PseudoJet has an associated and still valid
ClusterSequence.  
";

%feature("docstring") fastjet::WrappedStructure::area "

return the jet (scalar) area.  

By default, throws an Error  
";

%feature("docstring") fastjet::WrappedStructure::validated_cs "

if the jet has a valid associated cluster sequence then return a pointer to it;
otherwise throw an error  
";

%feature("docstring") fastjet::WrappedStructure::has_area "

check if it has a defined area  

false by default  
";

%feature("docstring") fastjet::WrappedStructure::n_exclusive_subjets "

return the size of exclusive_subjets(...); still n ln n with same coefficient,
but marginally more efficient than manually taking exclusive_subjets.size()  

By default, throws an Error  
";

%feature("docstring") fastjet::WrappedStructure::constituents "

retrieve the constituents.  

By default, throws an Error  
";

%feature("docstring") fastjet::WrappedStructure::exclusive_subdmerge "

return the dij that was present in the merging nsub+1 -> nsub subjets inside
this jet.  

By default, throws an Error  
";

%feature("docstring") fastjet::WrappedStructure::pieces "

retrieve the pieces building the jet.  

By default, throws an Error  
";

%feature("docstring") fastjet::WrappedStructure::exclusive_subjets_up_to "

return the list of subjets obtained by unclustering the supplied jet down to n
subjets (or all constituents if there are fewer than n).  

By default, throws an Error  
";

%feature("docstring") fastjet::WrappedStructure::~WrappedStructure "

default (virtual) dtor  
";

%feature("docstring") fastjet::WrappedStructure::exclusive_subdmerge_max "

return the maximum dij that occurred in the whole event at the stage that the
nsub+1 -> nsub merge of subjets occurred inside this jet.  

By default, throws an Error  
";

%feature("docstring") fastjet::WrappedStructure::has_partner "

check if it has been recombined with another PseudoJet in which case, return its
partner through the argument.  

Otherwise, 'partner' is set to 0.  

By default, throws an Error  
";

%feature("docstring") fastjet::WrappedStructure::has_child "

check if it has been recombined with another PseudoJet in which case, return its
child through the argument.  

Otherwise, 'child' is set to 0.  

By default, throws an Error  
";

%feature("docstring") fastjet::WrappedStructure::has_pieces "

return true if the structure supports pieces.  

false by default  
";

%feature("docstring") fastjet::WrappedStructure::has_parents "

check if it is the product of a recombination, in which case return the 2
parents through the 'parent1' and 'parent2' arguments.  

Otherwise, set these to 0.  

By default, throws an Error  
";

%feature("docstring") fastjet::WrappedStructure::is_pure_ghost "

true if this jet is made exclusively of ghosts.  

By default, throws an Error  
";

%feature("docstring") fastjet::WrappedStructure::validated_csab "

if the jet has valid area information then return a pointer to the associated
ClusterSequenceAreaBase object; otherwise throw an error  
";

%feature("docstring") fastjet::WrappedStructure::has_associated_cluster_sequence "

returns true if there is an associated ClusterSequence  
";

%feature("docstring") fastjet::WrappedStructure::has_constituents "

return true if the structure supports constituents.  

false by default  
";

%feature("docstring") fastjet::WrappedStructure::has_exclusive_subjets "

return true if the structure supports exclusive_subjets.  
";

%feature("docstring") fastjet::WrappedStructure::WrappedStructure "

default ctor the argument is the structure we need to wrap  
";

%feature("docstring") fastjet::WrappedStructure::object_in_jet "

check if the reference PseudoJet is contained the second one passed as argument.  

By default, throws an Error  
";

%feature("docstring") fastjet::WrappedStructure::exclusive_subjets "

return a vector of all subjets of the current jet (in the sense of the exclusive
algorithm) that would be obtained when running the algorithm with the given
dcut.  

Time taken is O(m ln m), where m is the number of subjets that are found. If m
gets to be of order of the total number of constituents in the jet, this could
be substantially slower than just getting that list of constituents.  

By default, throws an Error  
";

%feature("docstring") fastjet::WrappedStructure::area_4vector "

return the jet 4-vector area.  

By default, throws an Error  
";

%feature("docstring") fastjet::WrappedStructure::area_error "

return the error (uncertainty) associated with the determination of the area of
this jet.  

By default, throws an Error  
";

// File: namespacecdf.xml

// File: namespacefastjet.xml

%feature("docstring") fastjet::atlas::SelectorEtMax "

select objects with Et <= Etmax  
";

%feature("docstring") fastjet::atlas::SelectorStrip "

select objets within a rapidity distance 'half_width' from the location of the
reference jet, set by Selector::set_reference(...)  
";

%feature("docstring") fastjet::atlas::__default_random_generator "
";

%feature("docstring") fastjet::atlas::SelectorEMin "

select objects with E >= Emin  
";

%feature("docstring") fastjet::atlas::SelectorEtMin "

select objects with Et >= Etmin  
";

%feature("docstring") fastjet::atlas::sort_indices "
";

%feature("docstring") fastjet::atlas::sort_indices "

sort the indices so that values[indices[0->n-1]] is sorted into increasing order  
";

%feature("docstring") fastjet::atlas::scomp "
";

%feature("docstring") fastjet::atlas::deltaPhi "
";

%feature("docstring") fastjet::atlas::SelectorRapRange "

select objects with rapmin <= rap <= rapmax  
";

%feature("docstring") fastjet::atlas::join "
";

%feature("docstring") fastjet::atlas::join "

build a \"CompositeJet\" from a single PseudoJet with an extended structure of
type T derived from CompositeJetStructure  

build a MergedJet from a single PseudoJet  
";

%feature("docstring") fastjet::atlas::join "

build a \"CompositeJet\" from two PseudoJet with an extended structure of type T
derived from CompositeJetStructure  

build a MergedJet from 2 PseudoJet  
";

%feature("docstring") fastjet::atlas::join "

build a \"CompositeJet\" from 3 PseudoJet with an extended structure of type T
derived from CompositeJetStructure  

build a MergedJet from 3 PseudoJet  
";

%feature("docstring") fastjet::atlas::join "

build a \"CompositeJet\" from 4 PseudoJet with an extended structure of type T
derived from CompositeJetStructure  

build a MergedJet from 4 PseudoJet  
";

%feature("docstring") fastjet::atlas::join "
";

%feature("docstring") fastjet::atlas::join "

build a \"CompositeJet\" from a single PseudoJet with an extended structure of
type T derived from CompositeJetStructure  

build a MergedJet from a single PseudoJet  
";

%feature("docstring") fastjet::atlas::join "

build a \"CompositeJet\" from two PseudoJet with an extended structure of type T
derived from CompositeJetStructure  

build a MergedJet from 2 PseudoJet  
";

%feature("docstring") fastjet::atlas::join "

build a \"CompositeJet\" from 3 PseudoJet with an extended structure of type T
derived from CompositeJetStructure  

build a MergedJet from 3 PseudoJet  
";

%feature("docstring") fastjet::atlas::join "

build a \"CompositeJet\" from 4 PseudoJet with an extended structure of type T
derived from CompositeJetStructure  

build a MergedJet from 4 PseudoJet  
";

%feature("docstring") fastjet::atlas::join "

build a \"CompositeJet\" from the vector of its pieces with an extended
structure of type T derived from CompositeJetStructure  

build a \"CompositeJet\" from the vector of its pieces  

In this case, E-scheme recombination is assumed to compute the total momentum  
";

%feature("docstring") fastjet::atlas::join "

build a \"CompositeJet\" from the vector of its pieces with an extended
structure of type T derived from CompositeJetStructure  

build a \"CompositeJet\" from the vector of its pieces  

In this case, E-scheme recombination is assumed to compute the total momentum  
";

%feature("docstring") fastjet::atlas::SelectorPtFractionMin "

select objects that carry at least a fraction \"fraction\" of the reference jet.  

The reference jet must have been set with Selector::set_reference(...)  
";

%feature("docstring") fastjet::atlas::fastjet_version_string "

return a string containing information about the release  
";

%feature("docstring") fastjet::atlas::SelectorAbsEtaRange "

select objects with absetamin <= |eta| <= absetamax  
";

%feature("docstring") fastjet::atlas::SelectorRapPhiRange "

select objects with rapmin <= rap <= rapmax && phimin <= phi <= phimax  

Note that this is essentially a combination of SelectorRapRange and
SelectorPhiRange. We provide it as a Selector on its own in order to use the
known area (which would otherwise be lost by the && operator)  
";

%feature("docstring") fastjet::atlas::get_pointer "

getting the pointer  
";

%feature("docstring") fastjet::atlas::SelectorRectangle "

select objets within rapidity distance 'half_rap_width' from the reference jet
and azimuthal-angle distance within 'half_phi_width'; the reference jet is set
by Selector::set_reference(...)  
";

%feature("docstring") fastjet::atlas::SelectorPtMin "

select objects with pt >= ptmin  
";

%feature("docstring") fastjet::atlas::objects_sorted_by_values "

given a vector of values with a one-to-one correspondence with the vector of
objects, sort objects into an order such that the associated values would be in
increasing order (but don't actually touch the values vector in the process).  
";

%feature("docstring") fastjet::atlas::deltaR2 "
";

%feature("docstring") fastjet::atlas::SelectorIsZero "

select PseudoJet with 0 momentum  
";

%feature("docstring") fastjet::atlas::SelectorAbsEtaMin "

select objects with |eta| >= absetamin  
";

%feature("docstring") fastjet::atlas::sorted_by_rapidity "

return a vector of jets sorted into increasing rapidity  
";

%feature("docstring") fastjet::atlas::sorted_by_rapidity "

return a vector of jets sorted into increasing rapidity  
";

%feature("docstring") fastjet::atlas::SelectorAbsRapRange "

select objects with absrapmin <= |rap| <= absrapmax  
";

%feature("docstring") fastjet::atlas::sorted_by_pz "

return a vector of jets sorted into increasing pz  
";

%feature("docstring") fastjet::atlas::sorted_by_pz "

return a vector of jets sorted into increasing pz  
";

%feature("docstring") fastjet::atlas::swap "

swapping  
";

%feature("docstring") fastjet::atlas::sorted_by_pt "

return a vector of jets sorted into decreasing kt2  
";

%feature("docstring") fastjet::atlas::sorted_by_pt "

return a vector of jets sorted into decreasing transverse momentum  
";

%feature("docstring") fastjet::atlas::SelectorIdentity "
";

%feature("docstring") fastjet::atlas::SelectorMassRange "

select objects with Mmin <= Mass <= Mmax  
";

%feature("docstring") fastjet::atlas::SelectorCircle "

select objets within a distance 'radius' from the location of the reference jet,
set by Selector::set_reference(...)  
";

%feature("docstring") fastjet::atlas::SelectorNHardest "

select the n hardest objects  
";

%feature("docstring") fastjet::atlas::SelectorPhiRange "

select objects with phimin <= phi <= phimax  
";

%feature("docstring") fastjet::atlas::SelectorAbsEtaMax "

select objects with |eta| <= absetamax  
";

%feature("docstring") fastjet::atlas::scalar_product "

scalar product  
";

%feature("docstring") fastjet::atlas::SelectorRapMax "

select objects with rap <= rapmax  
";

%feature("docstring") fastjet::atlas::SelectorDoughnut "

select objets with distance from the reference jet is between 'radius_in' and
'radius_out'; the reference jet is set by Selector::set_reference(...)  
";

%feature("docstring") fastjet::atlas::norm "

norm of a vector  
";

%feature("docstring") fastjet::atlas::SelectorAbsRapMax "

select objects with |rap| <= absrapmax  
";

%feature("docstring") fastjet::atlas::SelectorMassMin "

select objects with Mass >= Mmin  
";

%feature("docstring") fastjet::atlas::SelectorERange "

select objects with Emin <= E <= Emax  
";

%feature("docstring") fastjet::atlas::SelectorPtMax "

select objects with pt <= ptmax  
";

%feature("docstring") fastjet::atlas::SelectorEtaMax "

select objects with eta <= etamax  
";

%feature("docstring") fastjet::atlas::sorted_by_E "

return a vector of jets sorted into decreasing energy  
";

%feature("docstring") fastjet::atlas::sorted_by_E "

return a vector of jets sorted into decreasing energy  
";

%feature("docstring") fastjet::atlas::SelectorRapMin "

select objects with rap >= rapmin  
";

%feature("docstring") fastjet::atlas::SelectorEMax "

select objects with E <= Emax  
";

%feature("docstring") fastjet::atlas::SelectorMassMax "

select objects with Mass <= Mmax  
";

%feature("docstring") fastjet::atlas::SelectorIsPureGhost "

select objects that are (or are only made of) ghosts.  

PseudoJets for which has_area() are considered non-pure-ghost.  
";

%feature("docstring") fastjet::atlas::SelectorAbsRapMin "

select objects with |rap| >= absrapmin  
";

%feature("docstring") fastjet::atlas::cast_if_derived "

a little helper that returns a pointer to d of type B* if D is derived from B
and NULL otherwise  
";

%feature("docstring") fastjet::atlas::PtYPhiM "

return a pseudojet with the given pt, y, phi and mass  

return a pseudojet with the given pt, y, phi and mass (phi should satisfy
-2pi<phi<4pi)  
";

%feature("docstring") fastjet::atlas::SelectorEtaRange "

select objects with etamin <= eta <= etamax  
";

%feature("docstring") fastjet::atlas::SelectorPtRange "

select objects with ptmin <= pt <= ptmax  
";

%feature("docstring") fastjet::atlas::floor_ln2_less "

returns true if floor(ln_base2(x)) < floor(ln_base2(y)), using Chan's neat
trick...  
";

%feature("docstring") fastjet::atlas::SelectorEtRange "

select objects with Etmin <= Et <= Etmax  
";

%feature("docstring") fastjet::atlas::have_same_momentum "

returns true if the momenta of the two input jets are identical  
";

%feature("docstring") fastjet::atlas::vector_product "

2D vector product  
";

%feature("docstring") fastjet::atlas::dot_product "

returns the 4-vector dot product of a and b  
";

%feature("docstring") fastjet::atlas::SelectorEtaMin "

select objects with eta >= etamin  
";

// File: namespacefastjet_1_1atlas.xml

%feature("docstring") fastjet::atlas::sort_list_pt "
";

%feature("docstring") fastjet::atlas::to_zero_2PI "
";

%feature("docstring") fastjet::atlas::clear_list "
";

%feature("docstring") fastjet::atlas::jet_from_overlap "
";

%feature("docstring") fastjet::atlas::to_minusPI_PI "
";

%feature("docstring") fastjet::atlas::sort_list_et "
";

%feature("docstring") fastjet::atlas::find_jet_in_list "
";

%feature("docstring") fastjet::atlas::sort_jet_list "
";

// File: namespacefastjet_1_1cms.xml

// File: namespacefastjet_1_1d0.xml

%feature("docstring") fastjet::d0::D0RunIIconeJets_CONEJETINFO::P2y "
";

%feature("docstring") fastjet::d0::D0RunIIconeJets_CONEJETINFO::RDelta "
";

%feature("docstring") fastjet::d0::D0RunIIconeJets_CONEJETINFO::RD2 "
";

%feature("docstring") fastjet::d0::D0RunIIconeJets_CONEJETINFO::P2phi "
";

// File: namespacefastjet_1_1d0_1_1D0RunIIconeJets__CONEJETINFO.xml

// File: namespacefastjet_1_1d0_1_1inline__maths.xml

%feature("docstring") fastjet::d0::inline_maths::min "
";

%feature("docstring") fastjet::d0::inline_maths::sqr "
";

%feature("docstring") fastjet::d0::inline_maths::y "
";

%feature("docstring") fastjet::d0::inline_maths::delta_phi "
";

%feature("docstring") fastjet::d0::inline_maths::phi "
";

// File: namespacefastjet_1_1d0runi.xml

%feature("docstring") fastjet::d0runi::inline_maths::R2 "
";

%feature("docstring") fastjet::d0runi::inline_maths::R2_bis "
";

%feature("docstring") fastjet::d0runi::inline_maths::DELTA_r "
";

%feature("docstring") fastjet::d0runi::inline_maths::E2phi "
";

%feature("docstring") fastjet::d0runi::inline_maths::E2eta "
";

// File: namespacefastjet_1_1d0runi_1_1inline__maths.xml

%feature("docstring") fastjet::d0runi::inline_maths::delta_phi "
";

%feature("docstring") fastjet::d0runi::inline_maths::phi "
";

%feature("docstring") fastjet::d0runi::inline_maths::min "
";

%feature("docstring") fastjet::d0runi::inline_maths::theta "
";

%feature("docstring") fastjet::d0runi::inline_maths::theta "
";

%feature("docstring") fastjet::d0runi::inline_maths::y "
";

%feature("docstring") fastjet::d0runi::inline_maths::eta "
";

%feature("docstring") fastjet::d0runi::inline_maths::eta "
";

%feature("docstring") fastjet::d0runi::inline_maths::eta "
";

%feature("docstring") fastjet::d0runi::inline_maths::sqr "
";

// File: namespacefastjet_1_1gas.xml

// File: namespacefastjet_1_1Private.xml

%feature("docstring") fastjet::Private::make_mirror "

if there is a need for a mirror when looking for closest pairs up to distance D,
then return true and turn the supplied point into its mirror copy  
";

// File: namespacefastjet_1_1siscone__plugin__internal.xml

// File: namespaceKtJet.xml

// File: namespacesiscone.xml

// File: namespacesiscone__spherical.xml

// File: namespacestd.xml

// File: AUTHORS.xml

// File: BUGS.xml

// File: COPYING.xml

// File: 01-basic_8cc.xml

%feature("docstring") main "

an example program showing how to use fastjet  
";

// File: 02-jetdef_8cc.xml

%feature("docstring") main "

an example program showing how to use fastjet  
";

// File: 03-plugin_8cc.xml

%feature("docstring") main "
";

// File: 04-constituents_8cc.xml

%feature("docstring") main "

an example program showing how to use fastjet  
";

// File: 05-eplus__eminus_8cc.xml

%feature("docstring") main "

an example program showing how to use fastjet  
";

// File: 06-area_8cc.xml

%feature("docstring") main "

an example program showing how to use fastjet  
";

// File: 07-subtraction-old_8cc.xml

%feature("docstring") main "
";

// File: 07-subtraction_8cc.xml

%feature("docstring") main "
";

// File: 08-selector_8cc.xml

%feature("docstring") main "
";

// File: 09-user__info_8cc.xml

%feature("docstring") main "
";

%feature("docstring") SelectorVertexNumber "
";

%feature("docstring") SelectorIsPi0Gamma "
";

// File: 10-subjets_8cc.xml

%feature("docstring") main "
";

// File: 11-filter_8cc.xml

%feature("docstring") main "

an example program showing how to use Filter in FastJet  

(this shows that the Filter can also be applied to a composite jet)  
";

// File: 12-boosted__higgs-old_8cc.xml

%feature("docstring") main "
";

// File: 12-boosted__higgs_8cc.xml

%feature("docstring") main "
";

// File: 13-boosted__top_8cc.xml

%feature("docstring") main "
";

// File: 14-groomers_8cc.xml

%feature("docstring") main "

an example program showing how to use Filter and Pruner in FastJet  
";

// File: 88-HI-pt-asymmetry_8cc.xml

%feature("docstring") main "
";

// File: CmdLine_8cc.xml

// File: CmdLine_8hh.xml

// File: fastjet__areas_8cc.xml

%feature("docstring") main "

an example program showing how to use fastjet  
";

%feature("docstring") print_jets "

a function that pretty prints a list of jets  
";

// File: fastjet__boosted__higgs_8cc.xml

%feature("docstring") main "

now do the subjet decomposition;  

when unpeeling a C/A jet, often only a very soft piece may break off; the
mass_drop_threshold indicates how much \"lighter\" the heavier of the two
resulting pieces must be in order for us to consider that we've really seen some
form of substructure  

QCD backgrounds that give larger jet masses have a component where a quite soft
gluon is emitted; to eliminate part of this one can place a cut on the asymmetry
of the branching;  

Here the cut is expressed in terms of y, the kt-distance scaled to the squared
jet mass; an easier way to see it is in terms of a requirement on the momentum
fraction in the splitting: z/(1-z) and (1-z)/z > rtycut^2 [the correspondence
holds only at LO]  
";

// File: fastjet__example_8cc.xml

%feature("docstring") main "

an example program showing how to use fastjet  
";

%feature("docstring") print_jets "

a function that pretty prints a list of jets  
";

// File: fastjet__subjets_8cc.xml

%feature("docstring") main "

an example program showing how to use fastjet  
";

%feature("docstring") print_jets_and_sub "

a function that pretty prints a list of jets  
";

%feature("docstring") print_jet "

print a single jet  
";

%feature("docstring") print_jets "

a function that pretty prints a list of jets  
";

// File: fastjet__subtraction_8cc.xml

%feature("docstring") main "

an example program showing how to use fastjet  
";

%feature("docstring") print_jets "

a function that pretty prints a list of jets, and performs the subtraction in
two different ways, using a generic ClusterSequenceAreaBase type object.  
";

// File: fastjet__timing_8cc.xml

%feature("docstring") pow2 "
";

%feature("docstring") main "

a program to test and time the kt algorithm as implemented in fastjet  
";

// File: fastjet__timing__plugins_8cc.xml

%feature("docstring") print_jet "

print a single jet  
";

%feature("docstring") pow2 "
";

%feature("docstring") do_compare_strategy "
";

%feature("docstring") print_jets_and_sub "

a function that pretty prints a list of jets and the subjets for each one  
";

%feature("docstring") is_unavailable "
";

%feature("docstring") main "

a program to test and time a range of algorithms as implemented or wrapped in
fastjet  
";

%feature("docstring") signal_failed_comparison "
";

%feature("docstring") print_jets "
";

// File: ktjet__example_8cc.xml

%feature("docstring") KtJet::main "

an example program showing how the fastjet_example program would be translated
for use with ktjet.  
";

%feature("docstring") KtJet::print_jets "
";

// File: ktjet__timing_8cc.xml

%feature("docstring") pow2 "
";

%feature("docstring") main "

a program to test and time the kt algorithm as implemented in ktjet  

Retrieve the final state jets from KtEvent sorted by Pt  

Print out jets 4-momentum and Pt  
";

// File: example_2README.xml

// File: plugins_2ATLASCone_2README.xml

// File: plugins_2CMSIterativeCone_2README.xml

// File: plugins_2PxCone_2README.xml

// File: README.xml

// File: ActiveAreaSpec_8hh.xml

// File: AreaDefinition_8hh.xml

// File: CircularRange_8hh.xml

// File: ClusterSequence_8hh.xml

// File: ClusterSequence1GhostPassiveArea_8hh.xml

// File: ClusterSequenceActiveArea_8hh.xml

// File: ClusterSequenceActiveAreaExplicitGhosts_8hh.xml

// File: ClusterSequenceArea_8hh.xml

// File: ClusterSequenceAreaBase_8hh.xml

// File: ClusterSequencePassiveArea_8hh.xml

// File: ClusterSequenceStructure_8hh.xml

// File: ClusterSequenceVoronoiArea_8hh.xml

// File: ClusterSequenceWithArea_8hh.xml

// File: CompositeJetStructure_8hh.xml

// File: config_8h.xml

// File: config__auto_8h.xml

// File: config__raw_8h.xml

// File: config__win_8h.xml

// File: Error_8hh.xml

// File: FunctionOfPseudoJet_8hh.xml

// File: GhostedAreaSpec_8hh.xml

// File: base_8hh.xml

// File: BasicRandom_8hh.xml

// File: ClosestPair2D_8hh.xml

// File: ClosestPair2DBase_8hh.xml

// File: ClusterSequence__N2_8icc.xml

// File: deprecated_8hh.xml

// File: Dnn2piCylinder_8hh.xml

// File: Dnn3piCylinder_8hh.xml

// File: Dnn4piCylinder_8hh.xml

// File: DnnPlane_8hh.xml

// File: DynamicNearestNeighbours_8hh.xml

// File: IsBase_8hh.xml

// File: LazyTiling25_8hh.xml

// File: LazyTiling9_8hh.xml

// File: LazyTiling9Alt_8hh.xml

// File: LazyTiling9SeparateGhosts_8hh.xml

// File: MinHeap_8hh.xml

// File: numconsts_8hh.xml

// File: SearchTree_8hh.xml

// File: TilingExtent_8hh.xml

// File: Triangulation_8hh.xml

// File: Voronoi_8hh.xml

// File: JetDefinition_8hh.xml

// File: LimitedWarning_8hh.xml

// File: internal_2LimitedWarning_8hh.xml

// File: NNBase_8hh.xml

// File: NNFJN2Plain_8hh.xml

// File: NNFJN2Tiled_8hh.xml

// File: NNH_8hh.xml

// File: PseudoJet_8hh.xml

// File: PseudoJetStructureBase_8hh.xml

// File: RangeDefinition_8hh.xml

// File: RectangularGrid_8hh.xml

// File: Selector_8hh.xml

// File: SharedPtr_8hh.xml

// File: version_8hh.xml

// File: WrappedStructure_8hh.xml

// File: INSTALL.xml

// File: NEWS.xml

// File: ATLASConePlugin_8cc.xml

// File: CommonUtils_8cc.xml

// File: CommonUtils_8hh.xml

// File: ATLASConePlugin_8hh.xml

// File: Jet_8cc.xml

// File: Jet_8hh.xml

// File: JetConeFinderTool_8cc.xml

// File: JetConeFinderTool_8hh.xml

// File: JetDistances_8hh.xml

// File: JetSplitMergeTool_8cc.xml

// File: JetSplitMergeTool_8hh.xml

// File: LorentzVector_8cc.xml

// File: LorentzVector_8hh.xml

// File: CDFJetCluPlugin_8cc.xml

// File: CDFMidPointPlugin_8cc.xml

// File: CDFJetCluPlugin_8hh.xml

// File: CDFMidPointPlugin_8hh.xml

// File: CMSIterativeConePlugin_8cc.xml

// File: CMSIterativeConePlugin_8hh.xml

// File: SortByEt_8h.xml

// File: ConeClusterAlgo_8hpp.xml

// File: D0RunIBaseConePlugin_8cc.xml

// File: D0RunIBaseConePlugin_8hh.xml

// File: D0RunIConePlugin_8hh.xml

// File: D0RunIpre96ConePlugin_8hh.xml

// File: HepEntityI_8h.xml

// File: HepEntityIpre96_8h.xml

// File: ConeJetInfo_8hpp.xml

// File: ConeSplitMerge_8hpp.xml

// File: D0RunIIConePlugin_8cc.xml

// File: D0RunIIConePlugin_8hh.xml

// File: HepEntity_8h.xml

// File: D0RunIICone_2inline__maths_8h.xml

// File: D0RunICone_2inline__maths_8h.xml

// File: ProtoJet_8hpp.xml

// File: EECambridgePlugin_8cc.xml

// File: EECambridgePlugin_8hh.xml

// File: GridJetPlugin_8hh.xml

// File: GridJetPlugin_8cc.xml

// File: JadePlugin_8hh.xml

// File: JadePlugin_8cc.xml

// File: NestedDefsPlugin_8hh.xml

// File: NestedDefsPlugin_8cc.xml

// File: PxConePlugin_8hh.xml

// File: pxcone_8f.xml

%feature("docstring") pxzerv "
";

%feature("docstring") pxord "
";

%feature("docstring") pxnew "
";

%feature("docstring") pxuvec "
";

%feature("docstring") pxaddv "
";

%feature("docstring") pxmdpi "
";

%feature("docstring") pxang3 "
";

%feature("docstring") pxnorv "
";

%feature("docstring") pxsorv "
";

%feature("docstring") pxsear "
";

%feature("docstring") pxzeri "
";

%feature("docstring") pxolap "
";

%feature("docstring") pxsame "
";

%feature("docstring") pxtry "
";

%feature("docstring") pxcone "
";

// File: pxcone_8h.xml

%feature("docstring") pxcone_ "
";

// File: PxConePlugin_8cc.xml

// File: SISConeBasePlugin_8hh.xml

// File: SISConePlugin_8hh.xml

// File: SISConeSphericalPlugin_8hh.xml

// File: SISConeBasePlugin_8cc.xml

// File: SISConePlugin_8cc.xml

// File: SISConeSphericalPlugin_8cc.xml

// File: TrackJetPlugin_8hh.xml

// File: TrackJetPlugin_8cc.xml

// File: AreaDefinition_8cc.xml

// File: BasicRandom_8cc.xml

// File: ClosestPair2D_8cc.xml

// File: ClusterSequence_8cc.xml

// File: ClusterSequence1GhostPassiveArea_8cc.xml

// File: ClusterSequence__CP2DChan_8cc.xml

// File: ClusterSequence__Delaunay_8cc.xml

// File: ClusterSequence__DumbN3_8cc.xml

// File: ClusterSequence__N2_8cc.xml

// File: ClusterSequence__TiledN2_8cc.xml

// File: ClusterSequenceActiveArea_8cc.xml

// File: ClusterSequenceActiveAreaExplicitGhosts_8cc.xml

// File: ClusterSequenceArea_8cc.xml

// File: ClusterSequenceAreaBase_8cc.xml

// File: ClusterSequencePassiveArea_8cc.xml

// File: ClusterSequenceStructure_8cc.xml

// File: ClusterSequenceVoronoiArea_8cc.xml

// File: CompositeJetStructure_8cc.xml

// File: Dnn2piCylinder_8cc.xml

// File: Dnn3piCylinder_8cc.xml

// File: Dnn4piCylinder_8cc.xml

// File: DnnPlane_8cc.xml

// File: Error_8cc.xml

// File: FunctionOfPseudoJet_8cc.xml

// File: GhostedAreaSpec_8cc.xml

// File: JetDefinition_8cc.xml

// File: LazyTiling25_8cc.xml

// File: LazyTiling9_8cc.xml

// File: LazyTiling9Alt_8cc.xml

// File: LazyTiling9SeparateGhosts_8cc.xml

// File: LimitedWarning_8cc.xml

// File: MinHeap_8cc.xml

// File: PseudoJet_8cc.xml

// File: PseudoJetStructureBase_8cc.xml

// File: RangeDefinition_8cc.xml

// File: RectangularGrid_8cc.xml

// File: Selector_8cc.xml

// File: TilingExtent_8cc.xml

// File: Voronoi_8cc.xml

// File: BackgroundEstimatorBase_8cc.xml

// File: CASubJetTagger_8cc.xml

// File: BackgroundEstimatorBase_8hh.xml

// File: Boost_8hh.xml

// File: CASubJetTagger_8hh.xml

// File: Filter_8hh.xml

// File: GridMedianBackgroundEstimator_8hh.xml

// File: JetMedianBackgroundEstimator_8hh.xml

// File: JHTopTagger_8hh.xml

// File: MassDropTagger_8hh.xml

// File: Pruner_8hh.xml

// File: Recluster_8hh.xml

// File: RestFrameNSubjettinessTagger_8hh.xml

// File: Subtractor_8hh.xml

// File: TopTaggerBase_8hh.xml

// File: Transformer_8hh.xml

// File: Filter_8cc.xml

// File: GridMedianBackgroundEstimator_8cc.xml

// File: JetMedianBackgroundEstimator_8cc.xml

// File: JHTopTagger_8cc.xml

// File: MassDropTagger_8cc.xml

// File: Pruner_8cc.xml

// File: Recluster_8cc.xml

// File: RestFrameNSubjettinessTagger_8cc.xml

// File: Subtractor_8cc.xml

// File: TopTaggerBase_8cc.xml

// File: group__basic__classes.xml

// File: group__area__classes.xml

// File: group__sec__area__classes.xml

// File: group__plugins.xml

// File: group__selectors.xml

// File: group__tools.xml

// File: group__tools__generic.xml

// File: group__tools__background.xml

// File: group__tools__taggers.xml

// File: group__extra__info.xml

// File: group__error__handling.xml

// File: group__advanced__usage.xml

// File: Example01.xml

// File: Example02.xml

// File: Example03.xml

// File: Example04.xml

// File: Example05.xml

// File: Example06.xml

// File: Example07old.xml

// File: Example07.xml

// File: Example08.xml

// File: Example09.xml

// File: Example10.xml

// File: Example11.xml

// File: Example12old.xml

// File: Example12.xml

// File: Example13.xml

// File: Example14.xml

// File: Examples.xml

// File: dir_773ba2ebb95421dd57881ab712e99454.xml

// File: dir_45a592d84a1ac0f7a0a0807a66695d87.xml

// File: dir_6e6000f6d03f83c7a187c297c9e9454b.xml

// File: dir_67635ac8b24f289026533f07e7bc27fb.xml

// File: dir_8e7193a48d9ddbf8acc5cbed85fe0109.xml

// File: dir_dda7a5782d76461a19f5012b01811067.xml

// File: dir_cfafba98a580ce4b62f8a6fa96d7cbb0.xml

// File: dir_34bef8c6f8ce7ee1e031c1d191e66786.xml

// File: dir_ad0dbae69265dae42c3e35e817cbc306.xml

// File: dir_cddcfb2ee21f6e53d1f3dc5a116c0d36.xml

// File: dir_75481660fd7c9b63aed73a326f29f04a.xml

// File: dir_a023863dd7a85273e7ee722e4ce643ff.xml

// File: dir_9193d20845bc1c33dc62ce7452a61bb0.xml

// File: dir_3dad92c48851cfd1c702cff47a5c3cbf.xml

// File: dir_34938dbe357da640aa22e8f772c43883.xml

// File: dir_58fe3c7304f1d5c4ce480e827d7a0752.xml

// File: dir_c668720ad1afca13745bb82af8d0a6b0.xml

// File: dir_39a529f5b69b5c9052eb70bd669949a9.xml

// File: dir_e1647c1d0ed3c68f386889dc6d4ccc3a.xml

// File: dir_cea712e0e750af24dc4fd7a358209605.xml

// File: dir_9f3ce583c20f2094d80618690292c890.xml

// File: dir_de6035fe3ffe046a5a22c8e49f2a1a87.xml

// File: dir_d44c64559bbebec7f509842c48db8b23.xml

// File: dir_8153113e4123b0c5c7a9a7243fb44bfb.xml

// File: dir_0e90115063dc745ea0402a01cb6f9fee.xml

// File: dir_f2918591c8631abc98d8d4d32b9433ff.xml

// File: dir_38c8d24aef3972a7f87b834274e76e31.xml

// File: dir_831661c48e253444b31be9672c3a2386.xml

// File: dir_6132b87572e207dc8cea747deb31b89a.xml

// File: dir_68267d1309a1af8e8297ef4c3efbcdba.xml

// File: dir_a029937718abb8534345de1c6d3da416.xml

// File: dir_4eeb864c4eec08c7d6b9d3b0352cfdde.xml

// File: dir_edf3ebc88130e34541219e137b894fb0.xml

// File: indexpage.xml

