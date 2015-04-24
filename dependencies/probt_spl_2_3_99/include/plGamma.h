/// \file plGamma.h
/*=============================================================================
 * Product        : ProBT 
 * File           : plGamma.h
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


#ifndef plGamma_h
#define plGamma_h

#include "plConfig.h"

#include <plDistribution.h>
#include <plSymbol.h>

/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/

/** This class implements the Gamma distribution.

    The Gamma distribution with parameters @em alpha, @em theta, and @em mu follows the following probability
    density function (for x > mu):
    \f[ p(x) = (x - \mu)^{\alpha - 1} \frac{e^{-\frac{x - \mu}{\theta}}}{\theta^{\alpha} \Gamma(\alpha)} \f]
*/
class PL_DLL_API plGamma: public plDistribution
{

public:
  /** Default constructor. */
  plGamma() 
    : plDistribution()
  { 
    signature_[PLGamma] = true; 
  }

  /** Constructs a Gamma distribution on the domain of @em variable.

      @param[in] variable Variable over which the distribution is built. It must be continuous.
      @param[in] alpha Shape parameter (also known as @em gamma or @em k) and must be > 0.
      @param[in] theta Scale parameter (also known as 1/beta, where @em beta is called the @em rate parameter) and must be > 0.
      @param[in] mu Location parameter. Zero by default. The inferior bound of @em variable must
      be be greater than or equal to \f$ \mu \f$.
  */
  plGamma(const plSymbol& variable, plFloat alpha, plFloat theta, plFloat mu = PL_ZERO );

  /** Destructor */
  virtual ~plGamma(){};
  
  /** Get the parameters used for constructing this object. */
  void get_parameters(plFloat& alpha, plFloat& theta, plFloat& mu) const;

  /** Promote from a plDistribution. If the object being copied is
      actually of the wrong type, an exception is raised.
  */
  explicit plGamma(const plDistribution&);

  virtual bool is_base_class()const
  {
    return false;
  }

};

/*****************************************************************************
 *                           Function Definitions
 *****************************************************************************/
/** Log-Gamma function */
PL_DLL_API plFloat plLgamma(plFloat x);

/** Gamma function */
inline plFloat plGammaFunc(plFloat x)
{
  return std::exp(plLgamma(x));
}

/** Incomplete Gamma integral.

    The function is defined by:
    \f[ igamma(a, x) = \frac {1} {\Gamma(a)} \int_0^x e^{-t} t^{a-1} dt. \f]

    \sa plIncompleteGammaCFunc
**/
PL_DLL_API plFloat plIncompleteGammaFunc(plFloat a, plFloat x);

/** Complemented incomplete Gamma integral.
    
   The function is defined by: 
   \f[ 
   igammac(a, x) 
   = 1 - igamma(a, x) 
   = \frac {1} {\Gamma(a)} \int_x^{\infty} e^{-t} t^{a-1} dt. 
   \f]

   \sa plIncompleteGammaFunc
**/
PL_DLL_API plFloat plIncompleteGammaCFunc(plFloat a, plFloat x);

#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
