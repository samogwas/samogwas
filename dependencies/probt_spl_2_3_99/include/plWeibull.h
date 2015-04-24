/*=============================================================================
 * Product        : ProBT 
 * File           : plWeibull.h
 * Author         : Kamel Mekhnacha
 * Creation       : Mon Sep 14 12:02:23 2009
 *
 *=============================================================================
 *        (c) Copyright 2008, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */


#ifndef plWeibull_h
#define plWeibull_h

#include "plConfig.h"

#include <plDistribution.h>
#include <plSymbol.h>

/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/

/** This class implements the Weibull distribution.

    The Weibull distribution with parameters \f$ \gamma \f$, \f$ \alpha \f$, and \f$ \mu \f$ follows the following probability
    density function (for \f$ x > \mu \f$):
    \f[ p(x) = \frac{\gamma}{\alpha} \left( \frac{x-\mu}{\alpha} \right)^{\gamma-1}\exp\left( -\left(\frac{x-\mu}{\alpha} \right)^\gamma \right) \f]
    
    The case where \f$ \mu = 0 \f$ and\f$ \alpha = 1 \f$ is called the standard Weibull
    distribution. The case where \f$ \mu = 0 \f$ is called the 2-parameter Weibull
    distribution.
*/
class PL_DLL_API plWeibull: public plDistribution
{

 public:
  /** Default constructor. */
  plWeibull() 
    : plDistribution()
    { 
      signature_[PLWeibull] = true; 
    }

  /** Constructs a Weibull distribution on the domain of @em variable.
      
      @param[in] variable Variable over which the distribution is built. It must be continuous.
      @param[in] gamma Shape parameter (also known as @em k) and must be > 0.
      @param[in] alpha Scale parameter (also known as \f$ \lambda \f$) and must be > 0.
      @param[in] mu Location parameter. Zero by default. The inferior bound of @em variable must
      be greater than or equal to \f$ \mu \f$.
  */
  plWeibull(const plSymbol& variable, plFloat gamma, plFloat alpha=PL_ONE, plFloat mu = PL_ZERO );

  /** Destructor */
  virtual ~plWeibull(){};
  
  /** Get the parameters used for constructing this object. */
  void get_parameters(plFloat& gamma, plFloat& alpha, plFloat& mu) const;

  /** Promote from a plDistribution. If the object being copied is
      actually of the wrong type, an exception is raised.
  */
  explicit plWeibull(const plDistribution&);

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
