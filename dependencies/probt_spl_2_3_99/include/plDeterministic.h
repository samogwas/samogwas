/*=============================================================================
 * Product        : ProBT 
 * File           : plDeterministic.h
 * Author         : Juan-Manuel Ahuactzin
 * Creation       : Thu May 31 09:24:41 2001
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


#ifndef plDeterministic_h
#define plDeterministic_h

#include "plConfig.h"


#include <plDistribution.h>
#include <plVariablesConjunction.h>
#include <plValues.h>


/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/



/** The \em plDeterministic class implements the "Dirac" or "Delta" function.
    The probability (density for continuous variables) is 1.0 
    (infinity for continuous variables) in one and only one point of the domain 
    of the variable \em variable.
*/
class PL_DLL_API plDeterministic : public plDistribution
{
public:

  plDeterministic() 
    : plDistribution()
  { 
    signature_.set(PLDeterministic); 
  }

  /** Constructs a Dirac with variables \em variables at point \em values */
  plDeterministic(const plVariablesConjunction& variables, const plValues& values);
  
  /** Destructor */
  virtual ~plDeterministic(){};

  /** Get back the point where the probability is non null */
  plValues get_dirac_point() const;
  
  /** Promote from a plDistribution. If the object being copied is
      actually of the wrong type, an exception is raised.
  */
  explicit plDeterministic(const plDistribution&);

  virtual bool is_base_class()const
  {
    return false;
  }
};


#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
