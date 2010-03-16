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
 * an implementation of C++0x shared pointers (or boost's)
 *
 * this class implements a smart pointer, based on the shared+ptr
 * proposal. A description of shared_ptr can be found in Section 2.2.3
 * of the first C++ Technical Report (TR1)
 *   http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2005/n1745.pdf
 * or, alternatively, on the Boost C++ library website at
 *   http://www.boost.org/doc/libs/1_42_0/libs/smart_ptr/shared_ptr.htm
 *
 * Our implementation is compatible with both of these apart from a
 * series of members and functions that have not been implemented:
 *  - conversion from weak and auto pointers
 *  - support for deleters and allocators
 *  - static, constant and dynamic casts
 *  - constructor and assignment sharing ownership with a shared
 *    pointer r but storing a different pointer than r (needed for the
 *    previous item)
 * In the last 2 cases, their implementation would require storing two
 * pointers for every copies of the shared pointer, while our
 * implementation only needs one. We did not implement then since we
 * want to limit as much as possible memory and time consumption, and
 * can easily avoid (at least for our needs so far) the casts.
 * 
 * The class has been tested against the existing boost (v1.42)
 * implementation (for the parts that we have implemented).
 */
template<class T>
class SharedPtr{
public:
  /// forward declaration of the counting container
  class __SharedCountingPtr;

  /// default ctor
  SharedPtr() : _ptr(NULL){}
  
  /// initialise with the main data
  /// \param  t  : the object we want a smart pointer to
  template<class Y> explicit SharedPtr(Y* ptr){
    _ptr = new __SharedCountingPtr(ptr);
  }
  
  /// overload the copy ctor so that it updates count
  /// \param  share : the object we want to copy
  SharedPtr(SharedPtr const & share) : _ptr(NULL){
    reset(share);
  }
    
  /// overload the copy ctor so that it updates count
  /// \param  share : the object we want to copy
  template<class Y> SharedPtr(SharedPtr<Y> const & share) : _ptr(NULL){
    reset(share);
  }

  /// default dtor
  ~SharedPtr(){
    // make sure the object has been allocated
    if (_ptr==NULL) return;

    _decrease_count();
  }

  /// reset the pointer to default value (NULL)
  void reset(){
    // // if we already are pointing to sth, be sure to decrease its count
    // if (_ptr!=NULL) _decrease_count();
    // _ptr = NULL;
    SharedPtr().swap(*this);
  }
  
  /// reset from a pointer
  template<class Y> void reset(Y * ptr){
    // // if we already are pointing to sth, be sure to decrease its count
    // if (_ptr!=NULL) _decrease_count();
    // 
    // _ptr = new __SharedCountingPtr(ptr);
    SharedPtr(ptr).swap(*this);
  }

  // not part of the standard
  /// do a smart copy
  /// \param  share : the object we want to copy
  /// Q? Do we need a non-template<Y> version as for the ctor and the assignment?
  template<class Y> void reset(SharedPtr<Y> const & share){
    // if we already are pointing to sth, be sure to decrease its count
    if (_ptr!=NULL){
      // in the specific case where we're having the same
      // share,reset() has actually no effect. However if *this is the
      // only instance still alive (implying share==*this) bringing
      // the count down to 0 and deleting the object will not have the
      // expected effect. So we just avoid that situation explicitly
      if (_ptr == share._get_container()) return;
    
      _decrease_count();
    }
    
    // Watch out: if share is empty, construct an empty shared_ptr
    
    // copy the container
    _ptr = share._get_container();  // Note: automatically set it to NULL if share is empty
    
    if (_ptr!=NULL)
      (*_ptr)++;
  }
  
  /// overload the = operator so that it updates count
  /// \param  share : the object we want to copy
  SharedPtr& operator=(SharedPtr const & share){
    reset(share);
    return *this;
  }
  
  /// overload the = operator so that it updates count
  /// \param  share : the object we want to copy
  template<class Y> SharedPtr& operator=(SharedPtr<Y> const & share){
    reset(share);
    return *this;
  }
  
  /// return the pointer we're pointing to  
  T* operator ()() const{
    if (_ptr==NULL) return NULL;
    return _ptr->get(); // automatically returns NULL when out-of-scope
  }
  
  /// indirection, get a reference to the stored pointer
  ///
  /// !!! WATCH OUT
  /// It fails the requirement that the stored pointer must no be NULL!!
  /// So you need explicitly to check the validity in your code
  inline T& operator*() const{
    return *(_ptr->get());
  }

  /// indirection, get the stored pointer
  ///
  /// !!! WATCH OUT
  /// It fails the requirement that the stored pointer must no be NULL!!
  /// So you need explicitly to check the validity in your code
  inline T* operator->() const{
    if (_ptr==NULL) return NULL;
    return _ptr->get();
  }  

  /// get the stored pointer
  inline T* get() const{
    if (_ptr==NULL) return NULL;
    return _ptr->get();
  }

  /// check if the instance is unique
  inline bool unique() const{
    return (use_count()==1);
  }

  /// return the number of counts
  inline long use_count() const{
    if (_ptr==NULL) return 0;
    return _ptr->use_count(); // automatically returns NULL when out-of-scope
  }

  /// conversion to bool
  /// This will allow you to use the indirection nicely
  inline operator bool() const{
    return (get()!=NULL);
  }

  /// exchange the content of teh two pointers
  inline void swap(SharedPtr & share){
    __SharedCountingPtr* share_container = share._ptr;
    share._ptr = _ptr;
    _ptr = share_container;
  }

  /**
   * A reference-counting pointer
   *
   * This is implemented as a container for that pointer together with
   * reference counting.
   * The pointer is deleted when the number of counts goes to 0;
   */
  class __SharedCountingPtr{
  public:
    /// default ctor
    __SharedCountingPtr() : _ptr(NULL), _count(0){}
    
    /// ctor with initialisation
    template<class Y> explicit __SharedCountingPtr(Y* ptr) : _ptr(ptr), _count(1){}
    
    /// default dtor
    ~__SharedCountingPtr(){ 
      // force the deletion of the object we keep track of
      if (_ptr!=NULL){ delete _ptr;}
    }

    /// return a pointer to the object
    inline T* get() const {return _ptr;}

    /// return the count
    inline long use_count() const {return _count;}

    /// postfix incrementation
    inline long operator++(int unused){return _count++;}

    /// postfix decrementation
    inline long operator--(int unused){return _count--;}

    /// prefix incrementation
    inline long operator++(){return ++_count;}

    /// prefix decrementation
    inline long operator--(){return --_count;}

  private:
    T *_ptr;              ///< the pointer we're counting the references to
    long _count;  ///< the number of references
  };

private:
  /// return the common container
  inline __SharedCountingPtr* _get_container() const{
    return _ptr;
  }

  /// decrease the pointer count and support deletion
  /// Warning: we don't test that the pointer is allocated
  void _decrease_count(){
    // decrease the count
    (*_ptr)--;
    
    // if no one else is using it, free the allocated memory
    if (_ptr->use_count()==0)
      delete _ptr; // that automatically deletes the object itself
  }

  // the real info
  __SharedCountingPtr *_ptr;
};


/// comparison: equality
template<class T,class U>
inline bool operator==(SharedPtr<T> const & t, SharedPtr<U> const & u){
  return t.get() == u.get();
}

/// comparison: difference
template<class T,class U>
inline bool operator!=(SharedPtr<T> const & t, SharedPtr<U> const & u){
  return t.get() != u.get();
}

/// comparison: orgering
template<class T,class U>
inline bool operator<(SharedPtr<T> const & t, SharedPtr<U> const & u){
  return t.get() < u.get();
}

/// swapping
template<class T>
inline void swap(SharedPtr<T> & a, SharedPtr<T> & b){
  return a.swap(b);
}

/// getting the pointer
template<class T>
inline T* get_pointer(SharedPtr<T> const & t){
  return t.get();
}


FASTJET_END_NAMESPACE      // defined in fastjet/internal/base.hh

#endif   // __FASTJET_SHARED_PTR_HH__
