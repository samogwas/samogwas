/*=============================================================================
 * Product        : ProBT 
 * File           : plBayesLearnNdNormalWithKnownVariance.h
 * Author         : Kamel Mekhnacha
 * Creation       : Fri Mar 14 15:59:17 2008
 *
 *=============================================================================
 *        (c) Copyright 2008, ProBayes SAS  -  all rights reserved
 *=============================================================================
*/


#ifndef PLBAYESLEARNNDNORMALWITHKNOWNVARIANCE_H
#define PLBAYESLEARNNDNORMALWITHKNOWNVARIANCE_H

#include "plConfig.h"

#include <plNonCndLearnObject.h>

#include <plFloatVector.h>
#include <plFloatMatrix.h>

SPL_DECL1(plBayesLearnNdNormalWithKnownVariance);

/** This class allows to make a Bayesian estimation of a n-dimensional Normal
    distribution with fixed covariance matrix @em sigma0 when the prior
    distribution over the Normal mean is itself a Normal distribution.

    The conjugacy property of the Normal distribution with respect to itself
    ensures that the posterior distribution over the Normal mean is itself a
    Normal distribution whose parameters can be computed analytically.

    @sa plBayesLearnObject plNormal
*/
class PL_DLL_API plBayesLearnNdNormalWithKnownVariance 
  : public plBayesLearnObject
{
public:
  /** Type of the variable (nD). */
  typedef plVariablesConjunction VarType;
  
  /** Default constructor. */
  plBayesLearnNdNormalWithKnownVariance();

  /** Creates a learnable normal distribution with fixed covariance martrix,
      where the prior over the Normal mean follows a Normal distribution.

      @param[in] variable the variable of the learnt distribution.

      @param[in] init_mu Initial value of the @em mu parameter of the Normal
      distribution over the learned Normal mean.

      @param[in] init_sigma Initial value of the @em sigma parameter of the
      Normal distribution over the learned Normal mean. It must be strictly
      positive.

      @param[in] sigma0 Fixed covariance matrix of the learned Normal
      distribution.
  */
  plBayesLearnNdNormalWithKnownVariance(const plVariablesConjunction &variable,
					const plFloatVector &init_mu,
					const plFloatMatrix &init_sigma,
					const plFloatMatrix &sigma0);

protected:
  /** Creates a learnable Normal distribution, where the prior over the mean
      follows a Normal distribution. @em mu and @em sigma are initialized
      arbitrarily to 0 and 1.  This constructor is here only for use with
      plCndLearnObject<>, it should be used with care as the arbitrary
      initialization of @em mu and @em sigma generally makes no sense.
  */
  explicit
  plBayesLearnNdNormalWithKnownVariance(const plVariablesConjunction &variable);

public:
  /** Destructor. */
  virtual ~plBayesLearnNdNormalWithKnownVariance() {};

  /** Resets learning (forgets all learned data). */
  void reset();

  /** Returns the current @em mu parameter of the Normal distribution over
      the learned Normal mean.
  */
  const plFloatVector &get_mu() const;

  /** Returns the current @em sigma parameter of the Normal distribution over
      the learned Normal mean.
  */
  const plFloatMatrix &get_sigma() const;

  /** Returns the fixed parameter @em sigma0 of the learned Normal distribution.
   */
  const plFloatMatrix &get_sigma0() const { return sigma0_; };

  /** Returns the Normal distribution corresponding to the posterior
      distribution over the Normal mean @em mu knowing all learned data.

      @param[in] mu Variable that corresponds to the Normal mean, on which to
      build the posterior distribution. Must be a variable with a real type.
  */
  plDistribution get_aposteriori_distribution(const plVariablesConjunction &mu) const;

  /** Returns the Normal distribution corresponding to the expected mean @em mu
      according to the learnt Normal distribution over the Normal mean, and the
      fixed covariance matrix @em sigma0.
  */
  plDistribution get_distribution() const;

  /** Returns the current @em mu and the @em sigma parameters of the Normal
      distribution over the learned Normal mean.
  */
  void get_mu_sigma(plFloatVector &mu, plFloatMatrix &sigma) const;
  
  using plLearnObject::add_point;
  inline bool add_point(const plFloatVector &point, plFloat weight = PL_ONE)
  {
    return add_point(point.data(), weight);
  }

  void get_parameters(plValues &params) const
  {
    if(all_vars_.is_empty())
      throw plError(75, "when calling 'plBayesLearnNdNormalWithKnownVariance::get_parameters");

    const unsigned int dim = mean_.size();
    const plFloatVector &mean = get_mu();

 
    unsigned int index = 0;

    // mean
    for(unsigned int i = 0; i < dim; ++i){
      params[index++] = mean[i];
    }

    // covariance
    for(unsigned int i = 0; i < dim; ++i){
      for(unsigned int j = 0; j < dim; ++j){
        params[index++] = sigma0_[i][j];
      }
    }

  }

  unsigned int get_n_parameters()const
  {
    if(all_vars_.is_empty())
      throw plError(75, "when calling 'plBayesLearnNdNormalWithKnownVariance::get_n_parameters");

    return mean_.size();
  }

  virtual plLearnObject* clone()const
  {
    return new plBayesLearnNdNormalWithKnownVariance(*this);
  }

 protected:
  /** Prints the object in a human-readable format.

  @param[in,out] out Output stream where the object is printed.
  */
  virtual void Output(std::ostream &out) const;
protected:
  /** Adds a data point.
   */
  bool internal_addPoint(const plDataValues &point, plFloat weight);

  /// w0*x0 + ... + wn*xn
  plFloatVector sum_samples_;

  /// Fixed covariance matrix of the Normal distribution.
  plFloatMatrix sigma0_;


  plFloatVector init_sigma_inv_times_init_mu_;
  plFloatMatrix init_sigma_inv_;
  plFloatMatrix sigma0_inv_;

  mutable plFloatVector mean_;
  mutable plFloatMatrix sigma_;
  mutable plFloatMatrix sigma_inv_;
  

  SPL_ACCESS1(plBayesLearnNdNormalWithKnownVariance);
  template <class T> friend class plCndLearnObject;
};

#endif /* PLBAYESLEARNNDNORMALWITHKNOWNVARIANCE_H */

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
