/*===========================================================================
 * Product        : ProBT
 * File           : plBayesLearnNormalWithKnownMean.h
 * Author         : Ronan Le Hy <ronan.lehy@probayes.com>
 *
 *===========================================================================
 *        (c) Copyright 2007, ProBayes SAS  -  all rights reserved
 *===========================================================================*/

#ifndef PLBAYESLEARNNORMALWITHKNOWNMEAN_H
#define PLBAYESLEARNNORMALWITHKNOWNMEAN_H

#include "plConfig.h"

#include <plNonCndLearnObject.h>

SPL_DECL1(plBayesLearn1dNormalWithKnownMean);

/** This class allows to make a Bayesian estimation of a one-dimensional Normal
    distribution with fixed mean when the prior distribution over the Normal
    precision (tau = 1 / sigma) is a Gamma distribution.

    The conjugacy property of the Gamma distribution with respect to the
    Normal distribution ensures that the posterior distribution over the Normal
    precision is itself a Gamma distribution whose parameters can be computed
    analytically.

    A Normal distribution with learned parameter @em tau (called @em precision)
    and fixed parameter @em mu (called @em mean) has the following probability
    density function:
    \f[ p(x) = \frac{\tau}{\sqrt{2 \pi}} e^{-\frac{\tau^2}{2} (x - \mu)^2} \f]

    The parameter to learn @em tau is the inverse of the more common standard
    deviation sigma = 1 / tau.

    A Gamma distribution with parameters @em alpha and @em theta has the
    following probability density function (for x > mu):
    \f[ p(x) = (x - \mu)^{\alpha - 1} \frac{e^{-\frac{x - \mu}{\theta}}}{\theta^{\alpha} \Gamma(\alpha)} \f]

    @sa plBayesLearnObject plNormal plGamma
 */
class PL_DLL_API plBayesLearn1dNormalWithKnownMean : public plBayesLearnObject
{
 public:
  /** Type of the variable (1D). */
  typedef plSymbol VarType;
  
  /** Default constructor. */
  plBayesLearn1dNormalWithKnownMean();

  /** Creates a learnable normal distribution with fixed mean, where the prior
      over the Normal precision parameter follows a Gamma distribution.
     
      @param[in] variable the variable of the learnt distribution.

      @param[in] init_alpha Initial value of the @em alpha parameter of the
      Gamma distribution over the Normal precision parameter. Must be strictly
      positive.

      @param[in] init_theta Initial value of the @em beta parameter of the
      Gamma distribution over the Normal precision parameter. Must be strictly
      positive.

      @param[in] mu Fixed mean of the Normal distribution.
  */
  plBayesLearn1dNormalWithKnownMean(plSymbol const & variable,
				  plFloat init_alpha,
				  plFloat init_theta,
				  plFloat mu = PL_ZERO);

 protected:
  /** Creates a learnable Normal distribution, where the prior over the
      precision parameter follows a Gamma distribution. @em alpha and @em theta
      are initialized arbitrarily to 1.  This constructor is here only for use
      with plCndLearnObject<>, it should be used with care as the arbitrary
      initialization of @em alpha and @em theta generally makes no sense.
  */
  explicit plBayesLearn1dNormalWithKnownMean(plSymbol const & variable);

 public:
  /** Destructor. */
  virtual ~plBayesLearn1dNormalWithKnownMean() {};

  /** Resets learning (forgets all learned data). */
  void reset();

  /** Returns the current @em alpha parameter of the Gamma distribution over
      the Exponential parameter lambda.
  */
  plFloat get_alpha() const 
  {
    if(all_vars_.is_empty())
      throw plError(75, "when calling 'plBayesLearn1dNormalWithKnownMean::get_alpha'");
    
    return init_alpha_ + total_weight_ / PL_TWO; 
}


  /** Returns the current @em beta parameter of the Gamma distribution over
      the Normal precision @em tau.
  */
  plFloat get_theta() const 
  {
    if(all_vars_.is_empty())
      throw plError(75, "when calling 'plBayesLearn1dNormalWithKnownMean::get_theta'");

    /// beta = beta_init + sum_squared_errors_
    /// theta = 1 / beta = 1 / ( (1/theta_init) + 0.5*sum_squared_errors ) = theta_init / (1 + theta_init * 0.5 * sum_squared_errors)
    return init_theta_ / (PL_ONE + init_theta_ * PL_HALF * sum_squared_errors_);
  }

  /** Returns the Gamma distribution corresponding to the posterior
      distribution over the Normal precision @em tau knowing all learned data.

      @param[in] tau Variable that corresponds to the Normal precision, on
      which to build the posterior distribution. Must be a variable with a real
      type.
   */
  plDistribution get_aposteriori_distribution(plVariablesConjunction const & tau) const;

  /** Returns the Normal distribution corresponding to the expected precision
      @em tau according to the learnt Gamma distribution on the Normal
      precision parameter.
  */
  plDistribution get_distribution() const;

  /** Returns the expectation of the posterior distribution on the Normal
      precision given learned data. */
  plFloat get_a_posteriori_expectation() const;

  void get_parameters(plValues &params) const
  {
    if(all_vars_.is_empty())
      throw plError(75, "when calling 'plBayesLearn1dNormalWithKnownMean::get_parameters'");

    params[0] = mu_;
    params[1] = PL_ONE/get_a_posteriori_expectation();
  }

  unsigned int get_n_parameters()const
  {
    if(all_vars_.is_empty())
      throw plError(75, "when calling 'plBayesLearn1dNormalWithKnownMean::get_n_parameters'");

    return 1;
  }

  virtual plLearnObject* clone()const
  {
    return new plBayesLearn1dNormalWithKnownMean(*this);
  }

 protected:
  /** Prints the object in a human-readable format.
      
      @param[in,out] out Output stream where the object is printed.
  */
  virtual void Output(std::ostream &out) const;

 protected:
  /** Adds a data point.
   */
  bool internal_addPoint(plDataValues const & point, plFloat weight);

  /// Initial alpha value. Needed to be able to reset().
  plFloat init_alpha_;
  /// Initial theta value. Needed to be able to reset().
  plFloat init_theta_;

  /// w0*(x0 - mu)² + ... + wn*(xn - mu)²
  plFloat sum_squared_errors_;

  /// Fixed mean of the Normal distribution.
  plFloat mu_;

  SPL_ACCESS1(plBayesLearn1dNormalWithKnownMean);
  template <class T> friend class plCndLearnObject;
};

#endif /* PLBAYESLEARNNORMALWITHKNOWNMEAN_H */

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
