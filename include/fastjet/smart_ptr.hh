#ifndef __SMART_PTR_HH__
#define __SMART_PTR_HH__

//STARTHEADER
// $Id$
//
// Copyright (c) 2005-2010, Matteo Cacciari, Gavin Salam and Gregory Soyez
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

#include "fastjet/internal/base.hh"

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

/**
 * template class for a smart pointer with initialisation from an owner
 *
 * The smart pointer is first allocated from pointer of the same
 * type. One can then make copies of it pointing to the same parent
 * object. When the initial object goes out of scope, the internal
 * pointer is set to NULL.
 * 
 * Notes: 
 *  - the parent object remains free of its destiny, i.e. it is not
 *    deleted when the smart pointer that owns it is deleted.    
 *  - there is no transfer of ownership implemented so far. An option
 *    to do it would be to have add a pointer to a smart_ptr<T> *
 *    pointing to the owner (it is a bit redundant with the T **ptr but
 *    the latter is needed at least one in any case), transfer of
 *    ownership could then be done by setting the previous ownership's
 *    owner flag to false and switching the owner to
 *    "this". Alternatively one can have a smatr_ptr<T> ** common to
 *    all the copies and test it against 'this', which would allow to
 *    get rid of the "owner" variable.
 */
template<class T>
class smart_ptr{
public:
  /// default ctor
  smart_ptr(){
    // initialise things so that the dtor behaves nicely
    counts = NULL;  // makes sure we don't decrease the count when deleted
    ptr    = NULL;  // makes sure () returns NULL
    owner  = false; // avoids spurious delete on exit
  }
  
  /// initialise with the main data
  /// \param  t  : the object we want a smart pointer to
  smart_ptr(T* t){
    ptr = new (T*);
    *ptr = t;
    
    counts = new unsigned int;
    *counts = 1;
    
    owner = true;
  }
  
  /// overload the copy ctor so that it updates count
  /// \param  share : the object we want to copy
  smart_ptr(const smart_ptr<T> &share){
    copy(share);
  }

  /// overload the copy ctor so that it updates count
  /// \param  share : the object we want to copy
  smart_ptr(smart_ptr<T> &share){
    copy(share);
  }

  /// overload the = operator so that it updates count
  /// \param  share : the object we want to copy
  smart_ptr<T> operator=(const smart_ptr<T> &share){
    copy(share);
    return *this;
  }
  
  /// overload the = operator so that it updates count
  /// \param  share : the object we want to copy
  smart_ptr<T> operator=(smart_ptr<T> &share){
    copy(share);
    return *this;
  }
  
  /// do a smart copy
  /// \param  share : the object we want to copy
  void copy(const smart_ptr<T> &share){
    ptr = share.ptr;
    counts = share.counts;
    
    owner = false; 
    (*counts)++;
  }
  
  /// default dtor
  ~smart_ptr(){
    // if we're destroying the owner, set the data pointer to NULL
    if (owner)
      *ptr = NULL;
    
    // make sure the object has been allocated
    if (counts != NULL){
      (*counts)--;
      
      // if no one else is using it, free the allocated memory
      if ((*counts)==0){
	delete ptr;
	delete counts;
      }
    }
  }
  
  T* operator ()(){
    return *ptr; // automatically returns NULL when out-of-scope
  }
  
  // the real info
  T** ptr;
  unsigned int *counts;
  bool owner;
};

FASTJET_END_NAMESPACE      // defined in fastjet/internal/base.hh

#endif   // __SMART_PTR_HH__
