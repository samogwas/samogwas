/*=============================================================================
 * Product        : ProBT 
 * File           : plStatFunctions.h
 * Author         : Kamel Mekhnacha
 * Creation       : Wed Jan 28 11:36:51 2009
 *
 *=============================================================================
 *        (c) Copyright 2008, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */

#ifndef plStatFunctions_h
#define plStatFunctions_h

#include "plConfig.h"

#include <plErf.h>
#include <plPhi.h>
#include <plBeta.h>
#include <plFactorial.h>
#include <plGamma.h>

/** Statistics utility functions. */
class plStatFunctions
{
public:

  /** Erf function. */
  inline static plFloat erf(plFloat x)
  {
    return plErf(x);
  }

  /** Normal Probability Density Function (pdf). */
  inline static plFloat normal_pdf(plFloat x, plFloat mu=0.0, plFloat sigma=1.0)
  {
    assert(sigma > 0.0);
    double y = (x-mu)/sigma;
    return  (std::exp(-y*y*PL_HALF)*PL_INV_SQRT_2PI)/sigma;
  }

  /** Phi is the Cumulative (repartition) Distribution Function (cdf) of the
      stantard normal (Gaussian) distribution: 
      Phi(x) = 0.5 + 0.5 * erf(x/sqrt(2) ).
  */
  inline static plFloat phi(plFloat x)
  {
    return plPhi(x);
  }

  /** The inverse of the Phi function above. */
  inline static plFloat inv_phi(plFloat u)
  {
    return plInvPhi(u);
  }

  /** Log Multinomial Beta function. */
  inline static plFloat log_multinomial_beta(const std::vector<plFloat>& x)
  {
    return plLogMultinomialBeta(x);
  }

  /** Multinomial Beta function. */
  inline static plFloat multinomial_beta(const std::vector<plFloat>& x)
  {
    return plMultinomialBeta(x);
  }

  /** Log Beta function. */
  inline static plFloat log_beta(plFloat x, plFloat y)
  {
    return plLogBetaFunction(x, y);
  }

  /** Beta function. */
  inline static plFloat beta(plFloat x, plFloat y)
  {
    return plBetaFunction(x, y);
  }

  /** Log-Gamma function. */
  inline static plFloat log_gamma(plFloat x)
  {
    return plLgamma(x);
  }

  /** Gamma function. */
  inline static plFloat gamma(plFloat x)
  {
    return plGammaFunc(x);
  }

  /** Incomplete Gamma integral.

      The function is defined by:
      \f[ igamma(a, x) = \frac {1} {\Gamma(a)} \int_0^x e^{-t} t^{a-1} dt. \f]

      \sa incomplete_gamma_c
  **/
  inline static plFloat incomplete_gamma(plFloat a, plFloat x)
  {
    return plIncompleteGammaFunc(a, x);
  }

  /** Complemented incomplete Gamma integral.
    
      The function is defined by: 
      \f[ 
      igammac(a, x) 
      = 1 - igamma(a, x) 
      = \frac {1} {\Gamma(a)} \int_x^{\infty} e^{-t} t^{a-1} dt. 
      \f]

      \sa incomplete_gamma
  **/
  inline static plFloat incomplete_gamma_c(plFloat a, plFloat x)
  {
    return plIncompleteGammaCFunc(a, x);
  }


  /** Return the log of n!. */
  inline static plFloat log_factorial(unsigned int n)
  {
    return plLogFactorial(n);
  }

  /** Return n!. */
  inline static plFloat factorial(unsigned int n)
  {
    return plFactorial(n);
  }


};

#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
