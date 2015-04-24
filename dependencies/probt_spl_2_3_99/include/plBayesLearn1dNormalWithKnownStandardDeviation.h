/*===========================================================================
 * Product        : ProBT
 * File           : plBayesLearn1dNormalWithKnownStandardDeviation.h
 * Author         : Ronan Le Hy <ronan.lehy@probayes.com>
 *
 *===========================================================================
 *        (c) Copyright 2007, ProBayes SAS  -  all rights reserved
 *===========================================================================*/

#ifndef PLBAYESLEARNNORMALWITHKNOWNSTANDARDDEVIATION_H
#define PLBAYESLEARNNORMALWITHKNOWNSTANDARDDEVIATION_H

#include "plConfig.h"

#include <plNonCndLearnObject.h>

SPL_DECL1(plBayesLearn1dNormalWithKnownStandardDeviation);

/** This class allows to make a Bayesian estimation of a one-dimensional Normal
    distribution with fixed standard deviation @em sigma0 when the prior
    distribution over the Normal mean is itself a Normal distribution.

    The conjugacy property of the Normal distribution with respect to itself
    ensures that the posterior distribution over the Normal mean is itself a
    Normal distribution whose parameters can be computed analytically.

    A Normal distribution with learned parameter @em mu (called @em mean) and 
    fixed parameter @em sigma0 (called @em standard @em deviation, square root
    of the @em variance sigma0^2) has the following probability density function:
    \f[ P(x) = \frac{1}{\sigma_{0}\sqrt{2 \pi}} e^{-\frac{-(x - \mu)^2}{2 \sigma_{0}^2} } \f]
    

    @sa plBayesLearnObject plNormal
*/
class PL_DLL_API plBayesLearn1dNormalWithKnownStandardDeviation 
  : public plBayesLearnObject
{
public:
  /** Type of the variable (1D). */
  typedef plSymbol VarType;
  
  /** Default constructor. */
  plBayesLearn1dNormalWithKnownStandardDeviation();

  /** Creates a learnable normal distribution with fixed standard deviation,
      where the prior over the Normal mean follows a Normal distribution.

      @param[in] variable the variable of the learnt distribution.

      @param[in] init_mu Initial value of the @em mu parameter of the Normal
      distribution over the learned Normal mean.

      @param[in] init_sigma Initial value of the @em sigma parameter of the
      Normal distribution over the learned Normal mean. It must be strictly
      positive.

      @param[in] sigma0 Fixed standard deviation of the learned Normal
      distribution.
  */
  plBayesLearn1dNormalWithKnownStandardDeviation(plSymbol const & variable,
                                                 plFloat init_mu,
                                                 plFloat init_sigma,
                                                 plFloat sigma0);

protected:
  /** Creates a learnable Normal distribution, where the prior over the mean
      follows a Normal distribution. @em mu and @em sigma are initialized
      arbitrarily to 0 and 1.  This constructor is here only for use with
      plCndLearnObject<>, it should be used with care as the arbitrary
      initialization of @em mu and @em sigma generally makes no sense.
  */
  explicit
  plBayesLearn1dNormalWithKnownStandardDeviation(plSymbol const & variable);

public:
  /** Destructor. */
  virtual ~plBayesLearn1dNormalWithKnownStandardDeviation() {};

  /** Resets learning (forgets all learned data). */
  void reset();

  /** Returns the current @em mu parameter of the Normal distribution over
      the learned Normal mean.
  */
  plFloat get_mu() const;

  /** Returns the current @em sigma parameter of the Normal distribution over
      the learned Normal mean.
  */
  plFloat get_sigma() const;

  /** Returns the fixed parameter @em sigma0 of the learned Normal distribution.
   */
  plFloat get_sigma0() const { return sigma0_; };

  /** Returns the Normal distribution corresponding to the posterior
      distribution over the Normal mean @em mu knowing all learned data.

      @param[in] mu Variable that corresponds to the Normal mean, on which to
      build the posterior distribution. Must be a variable with a real type.
  */
  plDistribution get_aposteriori_distribution(plVariablesConjunction const & mu) const;

  /** Returns the Normal distribution corresponding to the expected mean @em mu
      according to the learnt Normal distribution over the Normal mean, and the
      fixed standard deviation @em sigma0.
  */
  plDistribution get_distribution() const;

  /** Returns the expectation of the posterior distribution on the Normal mean
      given learned data. */
  plFloat get_a_posteriori_expectation() const;

  void get_parameters(plValues &params) const
  {
    if(all_vars_.is_empty())
      throw plError(75, "when calling 'plBayesLearn1dNormalWithKnownStandardDeviation::get_parameters'");

    params[0] = get_a_posteriori_expectation();
    params[1] = sigma0_;
  }

  unsigned int get_n_parameters()const
  {
    if(all_vars_.is_empty())
      throw plError(75, "when calling 'plBayesLearn1dNormalWithKnownStandardDeviation::get_n_parameters'");

    return 1;
  }

  virtual plLearnObject* clone()const
  {
    return new plBayesLearn1dNormalWithKnownStandardDeviation(*this);
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

  /// Initial mu value.
  plFloat init_mu_;
  /// Initial sigma value.
  plFloat init_sigma_;

  /// w0*x0 + ... + wn*xn
  plFloat sum_samples_;

  /// Fixed standard deviation of the Normal distribution.
  plFloat sigma0_;

  SPL_ACCESS1(plBayesLearn1dNormalWithKnownStandardDeviation);
  template <class T> friend class plCndLearnObject;
  friend class plBayesLearnLogNormalWithKnownShape;
};

#endif /* PLBAYESLEARNNORMALWITHKNOWNSTANDARDDEVIATION_H */

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
