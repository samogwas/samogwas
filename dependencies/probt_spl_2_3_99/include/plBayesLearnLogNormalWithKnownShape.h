/*===========================================================================
 * Product        : ProBT
 * File           : plBayesLearnLogNormalWithKnownShape.h
 * Author         : Ronan Le Hy <ronan.lehy@probayes.com>
 *
 *===========================================================================
 *        (c) Copyright 2007, ProBayes SAS  -  all rights reserved
 *===========================================================================*/

#ifndef PLBAYESLEARNLOGNORMALWITHKNOWNSHAPE_H
#define PLBAYESLEARNLOGNORMALWITHKNOWNSHAPE_H

#include "plConfig.h"

#include <plNonCndLearnObject.h>
#include <plBayesLearn1dNormalWithKnownStandardDeviation.h>

SPL_DECL1(plBayesLearnLogNormalWithKnownShape);

/** This class allows to make a Bayesian estimation of a log-normal
    distribution with fixed shape parameter @em s when the prior distribution
    over the natural logarithm of the log-normal scale @em ln(m) is a normal
    distribution.

    The conjugacy property of the normal distribution with respect to the
    log-normal ensures that the posterior distribution over the natural
    logarithm of the log-normal scale parameter is a normal distribution whose
    parameters can be computed analytically.

    A log-normal distribution with learned parameter @em m (called @em scale)
    and fixed parameters @em s (called @em shape) and @em theta (called @em
    location) has the following probability density function:

    \f[ p(x) = \frac {\exp (- (\ln( (x-\theta)/m) )^2/(2 s^2))}
    { (x - \theta) s \sqrt{2 \pi}} \hspace{2cm} x
    \geq \theta; m, s > 0 \f]

    A Normal distribution with parameters @em mu (called @em mean) and 
    @em sigma (called @em standard @em deviation, square root
    of the @em variance sigma^2) has the following probability density function:
    \f[ p(x) = \frac{1}{\sigma \sqrt{2 \pi}} e^{-\frac{-(x - \mu)^2}{2 \sigma^2} } \f]

    @sa plBayesLearnObject plLogNormal plNormal
*/
class PL_DLL_API plBayesLearnLogNormalWithKnownShape : public plBayesLearnObject
{
public:
  /** Type of the variable (1D). */
  typedef plSymbol VarType;
  
  /** Default constructor. */
  plBayesLearnLogNormalWithKnownShape();

  /** Creates a learnable log-normal distribution with fixed standard
      deviation, where the prior over the log-normal @em mu (@em mu = @em
      exp(m) ) parameter follows a normal distribution.
     
      @param[in] variable the variable of the learnt distribution.

      @param[in] init_mu Initial value of the @em mu parameter of the normal
      distribution over the learned log-normal mean.

      @param[in] init_sigma Initial value of the @em sigma parameter of the
      Normal distribution over the learned log-normal mean. It must be strictly
      positive.

      @param[in] s Fixed shape of the learned log-normal distribution.

      @param[in] theta Fixed location parameter of the learned log-normal
      distribution.
  */
  plBayesLearnLogNormalWithKnownShape(plSymbol const & variable,
				      plFloat init_mu,
				      plFloat init_sigma,
				      plFloat s,
				      plFloat theta);

protected:
  /** Creates a learnable log-normal distribution, where the prior over the
      mean follows a normal distribution. @em mu and @em sigma are initialized
      arbitrarily to 0 and 1.  This constructor is here only for use with
      plCndLearnObject<>, it should be used with care as the arbitrary
      initialization of @em mu and @em sigma generally makes no sense.
  */
  explicit plBayesLearnLogNormalWithKnownShape(plSymbol const & variable);

public:
  /** Destructor. */
  virtual ~plBayesLearnLogNormalWithKnownShape() {};

  /** Resets learning (forgets all learned data). */
  void reset();

  /** Returns the current @em mu parameter of the normal distribution over
      the learned log-normal mean.
  */
  plFloat get_mu() const;

  /** Returns the current @em sigma parameter of the normal distribution over
      the learned log-normal mean.
  */
  plFloat get_sigma() const;

  /** Returns the fixed @em s shape parameter of the learned log-normal
      distribution.
  */
  plFloat get_s() const;

  /** Returns the fixed @em theta location parameter of the learned log-normal
      distribution.
  */
  plFloat get_theta() const 
  { 
    if(all_vars_.is_empty())
      throw plError(75, "when calling 'plBayesLearnLogNormalWithKnownShape::get_theta()");

    return theta_; 
  }

  /** Returns the normal distribution corresponding to the posterior
      distribution over the log-normal mean @em mu knowing all learned data.

      @param[in] mu Variable that corresponds to the log-normal mean, on which
      to build the posterior distribution. Must be a variable with a real type.
  */
  plDistribution get_aposteriori_distribution(plVariablesConjunction const & mu) const;

  /** Returns the log-normal distribution corresponding to the expected mean
      @em mu according to the learnt normal distribution over the log-normal
      mean, and the fixed standard deviation @em sigma0.
  */
  plDistribution get_distribution() const;

  /** Returns the expectation of the posterior distribution on the log-normal
      mean given learned data. */
  plFloat get_a_posteriori_expectation() const;

  void get_parameters(plValues &params) const
  {
    if(all_vars_.is_empty())
      throw plError(75, "when calling 'plBayesLearnLogNormalWithKnownShape::get_parameters()");

    params[0] = get_s();
    params[1] = std::exp( get_a_posteriori_expectation() );
    params[2] = theta_;
  }

  unsigned int get_n_parameters()const
  {
    if(all_vars_.is_empty())
      throw plError(75, "when calling 'plBayesLearnLogNormalWithKnownShape::get_n_parameters()");

    return 1;
  }

  virtual plLearnObject* clone()const
  {
    return new plBayesLearnLogNormalWithKnownShape(*this);
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

  /// Fixed theta of the log-normal. 
  plFloat theta_;

  /// Learnable normal to which the actual learning is delegated.
  plBayesLearn1dNormalWithKnownStandardDeviation learnable_normal_;

  SPL_ACCESS1(plBayesLearnLogNormalWithKnownShape);
  template <class T> friend class plCndLearnObject;
};

#endif /* PLBAYESLEARNLOGNORMALWITHKNOWNSHAPE_H */

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
