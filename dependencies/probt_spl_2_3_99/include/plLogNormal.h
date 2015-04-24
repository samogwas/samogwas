/*=============================================================================
 * Product        : ProBT 
 * File           : plLogNormal.h
 * Author         : Kamel Mekhnacha
 * Creation       : Wed Jul 11 13:24:45 CEST 2004
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

#ifndef plLogNormal_h
#define plLogNormal_h

#include "plConfig.h"

#include <plMath.h>
#include <plDistribution.h>

/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/

/** A plLogNormal is a one-dimensional probability distribution
    on a single continuous variable. A variable X is
    said to be lognormally distributed if \f$ Y = ln(X) \f$ is normally
    distributed with @em ln denoting the natural logarithm.  The general
    formula for the probability density function of the lognormal
    distribution is:
	
    \f[ p(x) = \frac {\exp (- (\ln( (x-\theta)/m) )^2/(2
    \sigma^2))} { (x - \theta) \sigma \sqrt{2 \pi}} \hspace{2cm} x
    \geq \theta; m, \sigma > 0 \f]

    \f$ \sigma \f$ is the shape parameter, \f$ \theta \f$ is the location
    parameter, and @em m is the scale parameter. The case where \f$ \theta = 0
    \f$ and \em m = 1 is called the standard lognormal distribution.  The case
    where \f$ \theta = 0 \f$ is called the 2-parameter lognormal distribution.
    
    There are several common parameterizations of the lognormal
    distribution. The form given here is from: Evans, Hastings, and
    Peacock (2000), Statistical Distributions, 3rd. Ed., John Wiley
    and Sons.
    
    In this parametrization, @em X is is said to be lognormally
    distributed with parameters \f$ \sigma \f$, \f$ \theta \f$, and @em
    m if \f$ Y = LN( (X - \theta)/m ) \f$ is normally distributed with
    0 as mean and \f$ \sigma \f$ as standard deviation.
*/
class PL_DLL_API plLogNormal : public plDistribution
{
public:
  /** Default constructor, needed by the serialization code. */
  plLogNormal() 
    : plDistribution()
  { 
    signature_.set(PLLogNormal); 
  }

  /** Constructs a \em plLogNormal on the Variable \em V, where \f$ \sigma \f$
	is the shape parameter, \em m is the scale parameter and \f$ \theta \f$
	the location parameter. The case where \f$ \theta = 0 \f$ and \em m = 1
	is called the standard lognormal distribution.  The case where \f$
	\theta = 0 \f$ is called the 2-parameter lognormal distribution.
  */
  plLogNormal(const plSymbol& V,  plFloat sigma,  plFloat m = PL_ONE,
              plFloat theta = PL_ZERO);
  
  /** Destructor */
  virtual ~plLogNormal(){};

  /** Get the parameters used for constructing this object. */
  void get_parameters(plFloat& sigma, plFloat& m, plFloat& theta) const;

  /** Promote from a plDistribution. If the object being copied is
      actually of the wrong type, an exception is raised.
  */
  explicit plLogNormal(const plDistribution&);

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
