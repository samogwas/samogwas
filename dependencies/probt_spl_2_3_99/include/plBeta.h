/// \file plBeta.h
/*=============================================================================
 * Product        : ProBT 
 * File           : plBeta.h
 * Author         : Kamel Mekhnacha
 * Creation       : Jan 2006
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


#ifndef plBeta_h
#define plBeta_h

#include "plConfig.h"

#include <plDistribution.h>
#include <plSymbol.h>
#include <plGamma.h>

/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/

/** This class implements the Beta distribution.

    A Beta distribution over variable @em x, with parameters @em p and @em q, has
    as density function (using the @em Beta function @em B):
    \f[ p(x) = \frac{(x-a)^{p-1} (b-x)^{q-1}}{B(p, q) (b - a)^{p+q-1}} \f]

    - @em p and @em q are the shape parameters. They must be strictly positive.
    - @em a and @em b are the bounds of the distribution: it is defined on \f$ [a; b] \f$. We must have \f$ a < b\f$.
    - @em a is also called the location parameter.
    - @em b-a is called the scale parameter.

    Keeping a = 0, b = 1 gives the standard Beta distribution.
*/
class PL_DLL_API plBeta: public plDistribution
{
public:
  /** Default constructor, needed by the serialization code. */
  plBeta() 
    : plDistribution()
  { 
    signature_[PLBeta] = true; 
  }

  /** Constructs a Beta distribution.
      @param V variable over which to build the distribution.
      @param p p shape parameter
      @param q q shape parameter
      @param a Lower bound
      @param b Higher bound
  */
  plBeta(const plSymbol& V, plFloat p, plFloat q,
	 plFloat a=PL_ZERO, plFloat b=PL_ONE);
  
  /** Destroys the distribution */
  virtual ~plBeta(){};
  
  /** Get the parameters used for constructing this object. */
  void get_parameters(plFloat& p, plFloat& q, plFloat& a, plFloat& b) const;
  
  /** Promote from a plDistribution. This makes sense because all the information
      is actually in parent class plDistribution. If the object being copied is
      actually of the wrong type, an exception is raised.
  */
  explicit plBeta(const plDistribution&);

  virtual bool is_base_class()const
  {
    return false;
  }

};

/*****************************************************************************
 *                           Function Definitions
 *****************************************************************************/

/// Log Multinomial Beta function.
template<typename T>
T plLogMultinomialBeta(const std::vector<T>& x)
{
  T sum_x = 0, sum_lg = 0;
  for (unsigned int i=0; i<x.size(); ++i) {
    sum_x += x[i];
    sum_lg += plLgamma(x[i]);
  }
  return sum_lg - plLgamma(sum_x);
}

/// Multinomial Beta function
template<typename T>
inline T plMultinomialBeta(const std::vector<T>& x)
{
  return std::exp(plLogMultinomialBeta(x));
}

/// Log Beta function
template<typename T>
inline T plLogBetaFunction(T x, T y)
{
  return plLgamma(x) + plLgamma(y) - plLgamma(x + y);
}

/// Beta function
template<typename T>
inline T plBetaFunction(T x, T y)
{
  return std::exp(plLogBetaFunction(x, y));
}

#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
