/*=============================================================================
 * Product        : ProBT 
 * File           : plVonMises.cpp
 * Author         : Kamel Mekhnacha
 * Creation       : February 2014
 *
 *=============================================================================
 *     (c) Copyright 2014, Probayes SAS
 *=============================================================================
 *
 *------------------------- Description ---------------------------------------
 *   
 *   
 *-----------------------------------------------------------------------------
 */

#ifndef plVonMises_h
#define plVonMises_h

#include "plConfig.h"

#include <plDistribution.h>

class kplVonMises;

/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/


/** This class implements Von Mises distribution.

    Von Mises distribution (also known as the circular normal distribution or
    Tikhonov distribution) is a continuous probability distribution on the
    circle. It is a close approximation to the wrapped normal distribution, which
    is the circular analogue of the normal distribution.

    The probability density function of the Von Mises distribution is:
    \f[ p(x; \mu, k) = \frac {\exp \left( k \times cos(x - \mu) \right)} {2 \pi I_0(k)}   \f]
    where I0(x) is the modified Bessel function of order 0.
*/
class PL_DLL_API plVonMises : public plDistribution
{
 public:

  /** Default constructor, needed by the serialization code. */
  plVonMises() 
    : plDistribution()
    { 
      signature_.set(PLVonMises); 
    }

  /** Constructor
      @param[in] V The variable on which the distribution is defined
      @param[in] mean (mu) The mean value
      @param[in] k A measure of concentration (a reciprocal measure of dispersion, 
      so 1/κ is analogous to the variance in Normal distribution).
      
      The probability density function of the Von Mises distribution is:
      \f[ p(x; \mu, k) = \frac {\exp(k  \times cos(x - \mu))} {2 \pi I_0(k)}   \f]
      where I0(x) is the modified Bessel function of order 0.
  */
  plVonMises(const plSymbol& V, 
             plFloat mean, 
             plFloat k);
  
  /** Destroys the distribution */
  virtual ~plVonMises(){};

  /** Promote from a plDistribution. If the object being copied is
      actually of the wrong type, an exception is raised.
  */
  explicit plVonMises(const plDistribution&);
  

  /** Get the parameters used for constructing this object.
   */
  void get_parameters(plFloat& mean, plFloat& k) const;
  
  /** Returns the mean of this distribution. 
   */
  plFloat mean() const;
  
  /** Returns the 'k' parameter of this distribution. 
   */
  plFloat k() const;

 private:
  const kplVonMises* get_kernel_ptr() const;

  virtual bool is_base_class()const
  {
    return false;
  }

};


#endif //  plVonMises_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
