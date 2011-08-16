//STARTHEADER
// $Id$
//
// Copyright (c) 2005-2006, Matteo Cacciari and Gavin Salam
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



#ifndef __FASTJET_ERROR_HH__
#define __FASTJET_ERROR_HH__

#include<iostream>
#include<string>
#include "fastjet/internal/base.hh"

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

/// @ingroup error_handling
/// \class Error
/// base class corresponding to errors that will be thrown by fastjet
class Error {
public:
  /// default constructors
  Error() {}

  /// ctor from an error message
  ///   \param message to be printed
  /// Note: by default, in addition to the error message, the
  /// backtrace will be displayed (showing the last few calls before
  /// the error)
  Error(const std::string & message);

  /// virtual dummy dtor
  virtual ~Error() {}

  /// the error message
  std::string message() const {return _message;}

  /// controls whether the error message (and the backtrace) is
  /// printed out or not
  static void set_print_errors(bool print_errors) {_print_errors = print_errors;}

  /// controls whether the error message (and the backtrace) is
  /// printed out or not
  static void set_backtrace(bool enabled) {_print_backtrace = enabled;}

private:
  std::string _message;         ///< error message
  static bool _print_errors;    ///< do we print anything?
  static bool _print_backtrace; ///< do we show the backtrace?
};


FASTJET_END_NAMESPACE

#endif // __FASTJET_ERROR_HH__
