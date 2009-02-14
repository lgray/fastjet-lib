//*******************************************************************************
// Filename : JetConeFinderTool.cc 
// Author   : Ambreesh Gupta
// Created  : Nov, 2000
//
// Jan 2004: Use CLHEP units. Use phi = (-pi,pi].
//
// File taken from SpartyJet v2.20.0
// Modifications:
//   removed the string name in the ctor
//   removed the Message m_log
//   replaced StatusCode by an int (as defined previously)
//   cleaned the comments
//*******************************************************************************

#include <iostream>

#include "JetConeFinderTool.hh"
#include "Jet.hh"
#include "JetDistances.hh"
#include "CommonUtils.hh"

#include <vector>
#include <math.h>

#include <fastjet/internal/base.hh>

FASTJET_BEGIN_NAMESPACE

namespace atlas { 

// set the default energy scale
  double GeV = 1.0; //1000;

JetConeFinderTool::JetConeFinderTool() :m_coneR(0.7)
  , m_ptcut(0.0*GeV)
  , m_eps(0.05)
  , m_seedPt(2.0*GeV)
  , m_etaMax(5.0)
{}

JetConeFinderTool::~JetConeFinderTool()
{}

/////////////////////////////////////////////////////////////////////////////////
//Execution                                                                     /
/////////////////////////////////////////////////////////////////////////////////
int JetConeFinderTool::execute(jetcollection_t & theJets)
{
  sort_jet_list<JetSorter_Et>(theJets);

  m_pjetV = &theJets;

  if(theJets.size()==0) return 0;

  // Initiale ctr/dctr counter for object counting.
  m_ctr = 0;
  m_dctr = 0;

  //////////////////////
  // Reconstruct Jets // 
  //////////////////////
  this->reconstruct();

  //////////////////////////
  // ReFill JetCollection //
  //////////////////////////
  clear_list(theJets);
  jetcollection_t::iterator it = m_jetOV->begin();
  jetcollection_t::iterator itE = m_jetOV->end();
  for(; it!=itE; ++it){
    theJets.push_back(*it);
  }


  delete m_jetOV;
  return 1;
}

///////////////////////////////////////////////////////////////////////////////
// Reconstruction algorithm specific methods                                  /
///////////////////////////////////////////////////////////////////////////////

void
JetConeFinderTool::reconstruct()
{
  m_jetOV = new jetcollection_t();

  jetcollection_t::iterator tItr;
  jetcollection_t::iterator tItr_begin = m_pjetV->begin();
  jetcollection_t::iterator tItr_end   = m_pjetV->end();

  // order towers in pt

  for ( tItr=tItr_begin; tItr!=tItr_end; ++tItr ) {    

    // Seed Cut
    double tEt = (*tItr)->et();
    if ( tEt < m_seedPt ) break;

    // Tower eta, phi
    double etaT = (*tItr)->eta();
    double phiT = (*tItr)->phi();    
    
    // Iteration logic
    bool stable = false;
    bool inGeom = true;
    
    Jet* preJet;
    
    int count = 1;
    do { // Iteration Loop

      // Make cone  
      preJet = calc_cone(etaT,phiT);
      double etaC = preJet->eta();
      double phiC = preJet->phi();
      
      double deta = fabs(etaT - etaC);
      double dphi = fabs(JetDistances::deltaPhi(phiT,phiC));

      // Is Stable ?
      if ( deta < m_eps && dphi < m_eps ) 
	stable = true;
      
      // In Geometry ?
      if ( fabs(etaC) > m_etaMax ) 
	inGeom = false;	

      etaT = etaC;
      phiT = phiC;

      if ( !stable && inGeom ) {
	delete preJet;
	m_dctr +=1;
      }
      ++count;

    }while ( !stable && inGeom && count < 10  );      
  
    if ( count > 9 && (!stable && inGeom) ) continue;  // FIXME 9 ?

    // If iteration was succesfull -- check if this is a new jet and 
    // add it to OV.

    if ( stable && inGeom ) {
      jetcollection_t::iterator pItr   = m_jetOV->begin();
      jetcollection_t::iterator pItrE  = m_jetOV->end();
    
      bool newJet = true;
      double etaT = preJet->eta();
      double phiT = preJet->phi();

      for ( ; pItr != pItrE ; ++pItr ) {
	double etaC = (*pItr)->eta();
	double phiC = (*pItr)->phi();

	double deta = fabs(etaT - etaC);
	double dphi = fabs(JetDistances::deltaPhi(phiT,phiC));

	if ( deta < 0.05  && dphi < 0.05 ) { 
	  newJet = false;
	  break;
	}
      }
      if ( newJet ) {
	m_jetOV->push_back( preJet );
      }
      else {
	delete preJet;
	m_dctr +=1;
      }
    }
    else {
      delete preJet;
      m_dctr +=1;
    }
  }    
}

Jet* JetConeFinderTool::calc_cone(double eta, double phi)
{
  // Create a new Jet   
  Jet* j = new Jet();
  m_ctr +=1; 

  // Add all ProtoJet within m_coneR to this Jet  
  jetcollection_t::iterator itr  = m_pjetV->begin();
  jetcollection_t::iterator itrE = m_pjetV->end();

  for ( ; itr!=itrE; ++itr ) {
    double dR = JetDistances::deltaR(eta,phi,(*itr)->eta(),(*itr)->phi());
    if ( dR < m_coneR ) {
      j->addJet( (*itr) );
    }
  }    

  return j;
}



}  // namespace atlas

FASTJET_END_NAMESPACE
