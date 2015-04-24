/*=============================================================================
 * Product        : ProBT 
 * File           : plNormal.h
 * Author         : Christophe Coue
 * Creation       : Wed May 30 13:24:45 CEST 2001
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

#ifndef plNormal_h
#define plNormal_h

#include "plConfig.h"

#include <plDistribution.h>
#include <plFloatVector.h>
#include <plFloatMatrix.h>


/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/


/** This class implements Normal distributions on one or multiple
    dimensional space. */
class PL_DLL_API plNormal : public plDistribution
{
public:

  /** Default constructor, needed by the serialization code. */
  plNormal() 
    : plDistribution()
  { 
    signature_.set(PLNormal); 
  }

  
  /** Constructor of an N-dimensional normal (Gaussian) distribution.
      @param[in] V The  N-dimentional variable on which the distribution is defined. They must be continuous
      @param[in] mu The N-dimensional mean vector 
      @param[in] Sigma The NxN variance/covariance matrix (must be symetric positive)
      
      The probability density function of the N-dimensional normal (Gaussian) distribution is:
      \f[ p(x; \mu, \Sigma) = \frac {1} {\sqrt{|\Sigma|} (2\pi)^{N/2}} \exp\left( -0.5 (x-\mu) \Sigma^{-1} (x-\mu)^T \right) \f]
  */
  plNormal(const plVariablesConjunction& V, 
	   const plFloatVector& mu, 
	   const plFloatMatrix& Sigma);
  
 /** Constructor of a one-dimensional normal (Gaussian) distribution.
      @param[in] V The  one-dimentional variable on which the distribution is defined
      @param[in] mu The mean value
      @param[in] sigma The standard deviation (square root of the variance), must be positive
      
      The probability density function of the one-dimensional normal (Gaussian) distribution is:
      \f[ p(x; \mu, \sigma) = \frac {1} {\sigma \sqrt{2\pi} } \exp\left( - \frac{(x-\mu)^2}{2\sigma^2} \right) \f]
  */
  plNormal(const plSymbol& V, 
	   plFloat mu, 
	   plFloat sigma);
  
  /** Destroys the distribution */
  virtual ~plNormal(){};
  
  /** Get the parameters used for constructing this object. */
  void get_parameters(plFloatVector& mu, plFloatMatrix& variance_matrix) const;

  /** Promote from a plDistribution. If the object being copied is
      actually of the wrong type, an exception is raised.
  */
  explicit plNormal(const plDistribution&);
  
  /** Returns the Mahalanobis (normalized) distance.
      Instantiated for Tval in std::vector<float>, std::vector<double>, 
      std::vector<long double>, plValues, plFloatVector
  */
  template <typename Tval>
    plFloat mahalanobis(const Tval  &val)const;

  /** Get the parameters used for constructing this object.
      This function is valid only for 1D distributions.
  */
  void get_parameters(plFloat& m, plFloat& sd) const;
  
  /** Returns the mean of this distribution. 
      This function is valid only for 1D distributions.
  */
  plFloat mean() const;
  
  /** Returns the standard deviation of this distribution. 
      This function is valid only for 1D distributions.
  */
  plFloat standard_deviation() const;

  /** Returns the eigen values of the covariance matrix. 
      This function is valid only for nD distributions.
  */
  const plFloatVector &eigen_values() const;

  /** Returns the eigen matrix the covariance matrix (each column is an eigen vector). 
      This function is valid only for nD distributions.
  */
  const plFloatMatrix &eigen_matrix() const;

  
 private:
  void to_head_values(const plValues &val)const;
  
  template <typename Tval>
    void to_head_values(const Tval &val)const;

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
