/*=============================================================================
 * Product        : ProBT 
 * File           : plExponential.h
 * Author         : Kamel Mekhnacha
 * Creation       : Mon Oct 27 14:57:12 2003
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

#ifndef plExponential_h
#define plExponential_h

#include "plConfig.h"

#include <plDistribution.h>
#include <plSymbol.h>

/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/

/** This class implements the Exponential distribution.
    The Exponential distribution with parameters @em beta and @em mu follows the 
    following probability density function (for x > mu):
    \f[ p(x) = \frac{1}{\beta} \exp\left( -\frac{x-\mu}{\beta}\right)   \f]
 */
class PL_DLL_API plExponential: public plDistribution
{

public:
  /** Default constructor, needed by the serialization code. */
  plExponential() 
    : plDistribution()
  { 
    signature_[PLExponential] = true; 
  }
  
  /** Constructs a Exponential distribution on the domain of \em V.

      @param[in] V Variable over which the distribution is built. It must be continuous.
      @param[in] beta Scale parameter and must be > 0.
      @param[in] mu Location parameter. Zero by default. The inferior bound of @em variable must
      be superior or equal to @em mu.
      
      The Exponential distribution with parameters @em beta and @em and @em mu follows the following probability
      density function (for x > mu):
      \f[ p(x) = \frac{1}{\beta} \exp\left( -\frac{x-\mu}{\beta}\right)   \f]
  */
  plExponential(const plSymbol& V, plFloat beta, plFloat mu=PL_ZERO);

  /**  Destructor */
  virtual ~plExponential(){};
  
  /** Get the parameters used for constructing this object. */
  void get_parameters(plFloat& beta, plFloat& mu) const;

  /** Promote from a plDistribution. If the object being copied is
      actually of the wrong type, an exception is raised.
  */
  explicit plExponential(const plDistribution&);

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
