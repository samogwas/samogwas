/*===========================================================================
 * Product        : ProBT
 * File           : plBayesLearnExponential.h
 * Author         : Ronan Le Hy <ronan.lehy@probayes.com>
 *
 *===========================================================================
 *        (c) Copyright 2007, ProBayes SAS  -  all rights reserved
 *===========================================================================*/

#ifndef PLBAYESLEARNEXPONENTIAL_H
#define PLBAYESLEARNEXPONENTIAL_H

#include "plConfig.h"

#include <plNonCndLearnObject.h>

SPL_DECL1(plBayesLearnExponential);

/** This class allows to make a Bayesian estimation of an Exponential
    distribution when the prior distribution of the Exponential parameter is a
    Gamma distribution.

    The conjugacy property of the Gamma distribution with respect to the
    Exponential distribution ensures that the posterior distribution over the
    Exponential parameter is itself a Gamma distribution whose parameters can
    be computed analytically.

    An Exponential distribution with learned parameter @em beta (called @em
    scale parameter) has the following probability density function:
    \f[ P(x) = \frac{1}{\beta} e^{-\frac{x}{\beta}} \f]

    A Gamma distribution with parameters @em alpha and @em theta follows the
    following probability density function (for x > mu):

    \f[ P(x) = (x - \mu)^{\alpha - 1} \frac{e^{-\frac{x - \mu}{\theta}}}{\theta^{\alpha} \Gamma(\alpha)} \f]

    @sa plLearnExponential plExponential plGamma
 */
class PL_DLL_API plBayesLearnExponential : public plBayesLearnObject
{
 public:
  /** Type of the variable (1D). */
  typedef plSymbol VarType;
  
  /** Default constructor. */
  plBayesLearnExponential();

  /** Creates a learnable exponential distribution, where the prior over the
      parameter follows a Gamma distribution.
     
      @param[in] variable the variable of the learnt distribution

      @param[in] init_alpha Initial value of the @em alpha parameter of the
      Gamma distribution over the exponential parameter. Must be strictly
      positive.

      @param[in] init_theta Initial value of the @em beta parameter of the
      Gamma distribution over the exponential parameter. Must be strictly
      positive.

      @param[in] mu The shift parameter of the exponential distribution.
  */
  plBayesLearnExponential(plSymbol const & variable,
			  plFloat init_alpha,
			  plFloat init_theta,
			  plFloat mu = PL_ZERO);

 protected:
  /** Creates a learnable exponential distribution, where the prior over the
      parameter follows a Gamma distribution. @em alpha and @em theta are
      initialized arbitrarily to 1.  This constructor is here only for use with
      plCndLearnObject<>, it should be used with care as the arbitrary
      initialization of @em alpha and @em theta generally makes no sense.
  */
  explicit plBayesLearnExponential(plSymbol const & variable);

 public:
  /** Destructor. */
  virtual ~plBayesLearnExponential() {};

  /** Resets learning (forgets all learned data). */
  void reset();

  /** Returns the current @em alpha parameter of the Gamma distribution over
      the Exponential parameter lambda.
  */
  plFloat get_alpha() const 
  { 
    if(all_vars_.is_empty())
      throw plError(75, "when calling 'plBayesLearnExponential::get_alpha'");

    return init_alpha_ + total_weight_; 
  }

  /** Returns the current @em theta parameter of the Gamma distribution over
      the Exponential parameter @em lambda.
  */
  plFloat get_theta() const 
  {
    if(all_vars_.is_empty())
      throw plError(75, "when calling 'plBayesLearnExponential::get_theta'");

    /// beta parameter of the Gamma (not to be confused with the beta parameter
    /// of the exponential)
    // plFloat beta = 1. / init_theta_ + sum_samples_;
    return init_theta_ / (PL_ONE + init_theta_ * sum_samples_);
  };

  /** Returns the Gamma distribution corresponding to the posterior
      distribution over the exponential parameter knowing all learned data.

      @param[in] beta Variable that corresponds to the exponential parameter,
      on which to build the posterior distribution. Must be a variable with a
      real type.
   */
  plDistribution get_aposteriori_distribution(plVariablesConjunction const & beta) const;

  /** Returns the exponential distribution corresponding to the expected
      parameter according to the learnt Gamma distribution on the exponential
      parameter.
  */
  plDistribution get_distribution() const;

  /** Returns the expectation of the posterior distribution on the exponential
      parameter given learned data. */
  plFloat get_a_posteriori_expectation() const;

  void get_parameters(plValues &params) const
  {
    if(all_vars_.is_empty())
      throw plError(75, "when calling 'plBayesLearnExponential::get_parameters'");

    params[0] = get_a_posteriori_expectation();
    params[1] = all_vars_.get_type().get_min();
  }

  unsigned int get_n_parameters()const
  {
    if(all_vars_.is_empty())
      throw plError(75, "when calling 'plBayesLearnExponential::get_n_parameters'");

    return 1;
  }

  virtual plLearnObject* clone()const
  {
    return new plBayesLearnExponential(*this);
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

  /// Weighted sum of samples.
  plFloat sum_samples_;

  /// The shift parameter of the exponential distribution
  plFloat mu_;

  SPL_ACCESS1(plBayesLearnExponential);
  template <class T> friend class plCndLearnObject;

public:
  plFloat get_init_alpha() const { return init_alpha_; }
  plFloat get_init_theta() const { return init_theta_; }
  plFloat get_sum_samples() const { return sum_samples_; }
  plFloat get_mu() const { return mu_; }
};

#endif /* PLBAYESLEARNEXPONENTIAL_H */

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
