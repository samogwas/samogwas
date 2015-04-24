/*=============================================================================
 * Product        : ProBT 
 * File           : plPoisson.h
 * Author         : Hubert Althuser
 * Creation       :  2002
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


#ifndef plPoisson_h
#define plPoisson_h

#include "plConfig.h"

#include <plDistribution.h>
#include <plSymbol.h>

/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/

/** This class implements the Poisson distribution.*/
class PL_DLL_API plPoisson : public plDistribution
{

public:
    /** Default constructor, needed by the serialization code. */
    plPoisson() 
      : plDistribution()
  { 
    signature_[PLPoisson] = true; 
  }

  /** Constructs a Poisson distribution on the domain of the \em variable.
      The value "mean" (also called lambda) is the mean of the distribution.\n
      The variable "variable"  has to take solely positive or null integer values.\n
      The formula for Poisson distribution is: p(x)= exp(-mean) * mean^x / x!
  */
  plPoisson(const  plSymbol&v, plFloat mean);
  
  /** Destroys the distribution */
  virtual ~plPoisson(){};
  
  /** Get the parameters used for constructing this object. */
  void get_parameters(plFloat& mean) const;

  /** Promote from a plDistribution. If the object being copied is
      actually of the wrong type, an exception is raised.
  */
  explicit plPoisson(const plDistribution&);
  
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
