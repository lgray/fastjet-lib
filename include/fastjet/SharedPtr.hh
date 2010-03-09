#ifndef __FASTJET_SHARED_PTR_HH__
#define __FASTJET_SHARED_PTR_HH__

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
 * basically a reimplementation of C++0x shared pointers (or boost's)
 */
template<class T>
class SharedPtr{
public:
  /// default ctor
  SharedPtr(){
    // initialise things so that the dtor behaves nicely
    _counts = NULL;  // makes sure we don't decrease the count when deleted
    _ptr    = NULL;  // makes sure () returns NULL
  }
  
  /// initialise with the main data
  /// \param  t  : the object we want a smart pointer to
  SharedPtr(T* ptr){
    _ptr = new (T*);
    *_ptr = ptr;
    
    _counts = new unsigned int;
    *_counts = 1;
  }
  
  /// overload the copy ctor so that it updates count
  /// \param  share : the object we want to copy
  SharedPtr(const SharedPtr<T> &share){
    copy(share);
  }
  
  /// overload the copy ctor so that it updates count
  /// \param  share : the object we want to copy
  SharedPtr(SharedPtr<T> &share){
    copy(share);
  }
  
  /// overload the = operator so that it updates count
  /// \param  share : the object we want to copy
  SharedPtr<T> operator=(const SharedPtr<T> &share){
    copy(share);
    return *this;
  }
  
  /// overload the = operator so that it updates count
  /// \param  share : the object we want to copy
  SharedPtr<T> operator=(SharedPtr<T> &share){
    copy(share);
    return *this;
  }
  
  /// do a smart copy
  /// \param  share : the object we want to copy
  void copy(const SharedPtr<T> &share){
    _ptr = share.get_ptr();
    _counts = share.get_counts();
    (*_counts)++;
  }
  
  /// default dtor
  ~SharedPtr(){
    // make sure the object has been allocated
    if (_counts != NULL){
      (*_counts)--;
      
      // if no one else is using it, free the allocated memory
      if ((*_counts)==0){
	// we need to delete the object itself
	delete *_ptr;
	delete _ptr;
	delete _counts;
      }
    }
  }
  
  // return the pointer we're pointing to  
  T* operator ()() const{
    return *_ptr; // automatically returns NULL when out-of-scope
  }
  
  // return the common T**
  T** get_ptr() const{
    return _ptr; // automatically returns NULL when out-of-scope
  }

  // return the common T**
  unsigned int* get_counts() const{
    return _counts; // automatically returns NULL when out-of-scope
  }
  
private:
  // the real info
  T** _ptr;
  unsigned int *_counts;
};

FASTJET_END_NAMESPACE      // defined in fastjet/internal/base.hh

#endif   // __FASTJET_SHARED_PTR_HH__
