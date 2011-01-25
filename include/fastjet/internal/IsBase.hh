#ifndef __IS_BASE_OF_HH__
#define __IS_BASE_OF_HH__

#include "fastjet/internal/numconsts.hh"

FASTJET_BEGIN_NAMESPACE

//---------------------------------------------------
// define a true and a false 'type'
// Note: 
//  we could actually template the type and recover 
//  the TR1 integral_constant type. This also 
//  includes adding a typedef for the type and a
//  typedef for the struct in the struct below
//
// This is going to be helpful to "split" a given 
// call into 2 options based on a type constraint
// at compilation-time (rather than doing an "if" 
// which would only be resolved at runtime and could
// thus resutl in compilation errors. 
//---------------------------------------------------

/// \if internal_doc
/// \class integral type 
/// a generic construct that promotes a generic value of a generic type
/// as a type
///
/// this has 2 template parameters: T, the considered type, and _t, a
/// value of type T
/// This object is a basic construct in type traits
/// \endif
template<typename T, T _t>
struct integral_type{
  static const T value = _t;         ///< the value (only member carrying info)
  typedef T value_type;		     ///< a typedef for the type T
  typedef integral_type<T,_t> type;  ///< a typedef for the whole structure
};

// definition of the static member in integral_type
template<typename T, T _t>
const T integral_type<T, _t>::value;

// shortcuts
typedef integral_type<bool, true>  true_type;  ///< the bool 'true'  value promoted to a type
typedef integral_type<bool, false> false_type; ///< the bool 'false' value promoted to a type


//---------------------------------------------------
// define a yes and a no type (based on their size)
//---------------------------------------------------
typedef char (&__yes_type)[1]; //< the yes type
typedef char (&__no_type) [2]; //< the no type


//---------------------------------------------------
// Now deal with inheritance checks
//
// We want to provide a IsBaseAndDerived<B,D> type 
// trait that contains a value that is true if D
// is derived from B and false otherwise.
//
// For an explanation of how the code below works, 
// have a look at 
//   http://groups.google.com/group/comp.lang.c++.moderated/msg/dd6c4e4d5160bd83
// and the links therein
//
// WARNING: according to 'boost', this may have some
//   issues with MSVC7.1. See their code for a description
//   of the workaround used below
//---------------------------------------------------

/// \if internal_doc
/// \class __inheritance_helper
/// helper for IsBasedAndDerived<B,D>
/// \endif
template<typename B, typename D>
struct __inheritance_helper{
#if !((_MSC_VER !=0 ) && (_MSC_VER == 1310))   // MSVC 7.1
  template <typename T>
  static __yes_type check_sig(D const volatile *, T);
#else
  static __yes_type check_sig(D const volatile *, long);
#endif
  static __no_type  check_sig(B const volatile *, int);
};

/// \if internal_doc
/// \class IsBaseAndDerived
/// check if the second template argument is derived from the first one
/// 
/// this class has 2 template dependencies: B and D. It contains a
/// static boolean value that will be true if D is derived from B and
/// false otherwise.
///
/// Note: This construct may have a problem with MSVC7.1. See the
/// boost implementation for a description and workaround
/// \endif
template<typename B, typename D>
struct IsBaseAndDerived{
#if ((_MSC_FULL_VER != 0) && (_MSC_FULL_VER >= 140050000))
#pragma warning(push)
#pragma warning(disable:6334)
#endif


  /// \if internal_doc
  /// a helper structure that will pick between a casting to B*const
  /// or D.
  ///
  /// precisely how this structure works involves advanced C++
  /// conversion rules
  /// \endif
  struct Host{
#if !((_MSC_VER !=0 ) && (_MSC_VER == 1310))
    operator B const volatile *() const;
#else
    operator B const volatile * const&() const;
#endif
    operator D const volatile *();
  };

  /// the boolean value being true if D is derived from B
  static const bool value = ((sizeof(B)!=0) && (sizeof(D)!=0) && (sizeof(__inheritance_helper<B,D>::check_sig(Host(), 0)) == sizeof(__yes_type)));

#if ((_MSC_FULL_VER != 0) && (_MSC_FULL_VER >= 140050000))
#pragma warning(pop)
#endif
};

FASTJET_END_NAMESPACE


#endif  // __IS_BASE_OF_HH__
