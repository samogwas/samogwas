/*=============================================================================
 * Product        : ProBT 
 * File           : plSymbol.h
 * Author         : Juan-Manuel Ahuactzin
 * Creation       : Wed Mar 21 11:49:21 2001
 *
 *=============================================================================
 *     (c) Copyright 2000, Centre National de la Recherche Scientifique,
 *                              all rights reserved
 *=============================================================================
 *
 *------------------------- Description ---------------------------------------
 *   
 *   
 *-----------------------------------------------------------------------------
*/

#ifndef plSymbol_h
#define plSymbol_h

#include "plConfig.h"

#include <plVariablesConjunction.h>
#include <plType.h>


/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/


//============================================================================
/** 
    A \em plSymbol is a set containing one and only one unidimensional
    variable.
*/
class PL_DLL_API plSymbol : public plVariablesConjunction
{
public:

  /** Constructor from a \em print_name as a \em string, selected by the
      user, and a previously defined \em variable_type. */
  plSymbol(const std::string &print_name, 
	   const plType &variable_type);

  /** Constructor from a \em print_name as a \em string, selected by the
      user, and a previously defined \em symbol. */
  plSymbol(const std::string &print_name, 
	   const plSymbol & symbol);

  /** Default empty plSymbol constructor. */
  plSymbol();
  

  /** Destructor, erase the variable. */
  virtual ~plSymbol();

  /** Promote from a plVariablesConjunction. If the object being copied
      contains more than one symbol, an exception is raised. */
  explicit plSymbol(const plVariablesConjunction&);


#ifdef PL_GLOBAL_SYMBOL_TABLE
  /** Implicit conversion of a name string to a plSymbol. Returns an existing
      plSymbol (the last created one) having \em name as name if any and throws
      an exception if no plSymbol with this name exists.
  */
  plSymbol(const std::string &name);

  plSymbol(const char *name);

  /** Returns an existing plSymbol (the last created one) having \em name as
      name if any and throws an exception if no plSymbol with this name exists.
  */
  static plSymbol get_symbol_with_name(const std::string &name);
  
#endif // PL_GLOBAL_SYMBOL_TABLE

protected:

  explicit plSymbol(kplVariable* kpl_symbol);

#ifdef PL_GLOBAL_SYMBOL_TABLE
  void construct_from_name(const char *name);
#endif // PL_GLOBAL_SYMBOL_TABLE

  friend class plArray;
  friend class plVariablesConjunction;
  friend class plVariablesConjunction_const_iterator;
  friend class plCndLogNormal;
  friend bool PL_DLL_API operator<(plSymbol const & a, plSymbol const & b);
  friend class plCndUniform;
  friend class plCndBeta;
  friend class plCndExponential;
  friend class plCndGamma;
  friend class plCndWeibull;
  friend class plCndVonMises;
  
 private:
  plVariablesConjunction &operator^=(const plVariablesConjunction &var);

};

/** Defines a total, stable order over plSymbols, suitable for putting
    plSymbols into a std::map or std::set. The order is arbitrary, and is
    not guaranteed to be related to cardinalities or variable names.
 */
bool PL_DLL_API operator<(plSymbol const & a, plSymbol const & b);

#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
