#ifndef __D0RUNIPRE96CONEPLUGIN_HH__
#define __D0RUNIPRE96CONEPLUGIN_HH__

//STARTHEADER
// $Id: D0RunIpre96ConePlugin.hh 1778 2010-10-25 10:02:58Z soyez $
//
// Copyright (c) 2009-2010, Matteo Cacciari, Gavin Salam and Gregory Soyez
//
//----------------------------------------------------------------------
// This file is part of FastJet.
//
//  FastJet is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  The algorithms that underlie FastJet have required considerable
//  development and are described in hep-ph/0512210. If you use
//  FastJet as part of work towards a scientific publication, please
//  include a citation to the FastJet paper.
//
//  FastJet is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with FastJet; if not, write to the Free Software
//  Foundation, Inc.:
//      59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//----------------------------------------------------------------------
//ENDHEADER

#include "fastjet/D0RunIBaseConePlugin.hh"

// questionable whether this should be in fastjet namespace or not...

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

//----------------------------------------------------------------------
//
/// @ingroup plugins
/// \class D0RunIpre96ConePlugin
///
/// D0RunIConePlugin is a plugin for fastjet (v2.4 upwards) that
/// provides an interface to the pre 1996 D0 version of Run-I cone
/// algorithm
///
/// The D0 code has been taken from Lars Sonnenschein's web-space
/// http://www-d0.fnal.gov/~sonne/D0RunIcone.tgz
///
/// The version of the D0 Run I code distributed
/// here has been modified by the FastJet authors, so as to provide
/// access to the contents of the jets (as is necessary for the
/// plugin). This does not modify the results of the clustering.
//
//----------------------------------------------------------------------
class D0RunIpre96ConePlugin : public D0RunIBaseConePlugin {
public:
  // ctor
  D0RunIpre96ConePlugin (double CONrad, double JETmne , double SPLifr = _DEFAULT_SPLifr)
    : D0RunIBaseConePlugin(CONrad, JETmne , SPLifr){}

  // the things that are required by base class
  virtual std::string description () const;

  // the part that really does the clustering
  virtual void run_clustering(ClusterSequence &) const;
};


FASTJET_END_NAMESPACE      // defined in fastjet/internal/base.hh

#endif // __D0RUNIPRE96CONEPLUGIN_HH__
