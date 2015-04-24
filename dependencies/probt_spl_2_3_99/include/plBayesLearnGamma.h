/*=============================================================================
 * Product        : ProBT 
 * File           : plBayesLearnGamma.h
 * Author         : David Raulo
 * Creation       : Wed Dec 13 16:17:14 2006
 *
 *=============================================================================
 *        (c) Copyright 2006, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */

#ifndef plBayesLearnGamma_h
#define plBayesLearnGamma_h

#include "plConfig.h"

#include <plNonCndLearnObject.h>
#include <plGamma.h>

SPL_DECL1(plBayesLearnGamma);

/** This class allows to make a Bayesian estimation of Gamma distributions.
    To take advantage of the conjugacy property, the prior distribution is 
    supposed to be a Gamma distribution and the a posteriori distribution is 
    a Gamma distribution too.
*/
class PL_DLL_API plBayesLearnGamma: public plBayesLearnObject
{
 public:
  /** Type of the variable (1D). */
  typedef plSymbol VarType;
  
  
  /** Constructor using given parameters v (corresponding to the alpha
      parameter in the plGamma definition) and mu, the initial values for the
      'alpha' and 'lambda' (1/beta) parameters of the prior Gamma
      distribution.\n  The 'alpha' corresponds to the number of observed
      (supposed) number of events while 'lambda' corresponds on the time
      intervals number in which these events have been observed (supposed).
  */
  plBayesLearnGamma(const plSymbol &vars, plFloat v, plFloat mu,
		    plFloat init_alpha, plFloat init_lambda)
    : plBayesLearnObject(vars)
    , v_(v)
    , mu_(mu)
    , init_alpha_(init_alpha)
    , init_lambda_(init_lambda)
    , alpha_(init_alpha_)
    , lambda_(init_lambda_)
  {}
  
  /** Empty constructor */
  plBayesLearnGamma()
    : plBayesLearnObject()
    , v_(PL_ZERO)
    , mu_(PL_ZERO)
    , init_alpha_(PL_ONE)
    , init_lambda_(PL_ONE)
    , alpha_(PL_ONE)
    , lambda_(PL_ONE)
  {}
  
  /** Dectructo. */
  virtual ~plBayesLearnGamma(){}
    
  /** Resets learning. */
  void reset();
  
  /** Returns the 'alpha' parameter. */
  plFloat get_alpha()const ;

  /** Returns the 'lambda' parameter. */
  plFloat get_lambda()const ;
  
  /** Returns the Gamma distribution corresponding to the aposteriori
   * distribution */
  plDistribution get_aposteriori_distribution(
                                              const plVariablesConjunction &gamma_scale_variable) const;
  
  /** Returns the Gamma distribution corresponding to the learnt distribution.
      The 'mean' parameter is the expectation of the a posteriori distribution.
  */
  plDistribution get_distribution() const;

  /** Returns the expectation of the a posteriori distribution */
  plFloat get_a_posteriori_expectation()const;

  void get_parameters(plValues &params) const;

  unsigned int get_n_parameters()const
  {
    if(all_vars_.is_empty())
      throw plError(75, "when calling 'plBayesLearnGamma::get_n_parameters");

    return 2;
  }
  
  virtual plLearnObject* clone()const
  {
    return new plBayesLearnGamma(*this);
  }

 protected:
  /** Adds a point \em point with a given weight \em weight and updates
      the statistics.  This is an internal method, do not use it
      directly. Use @em add_point instead.*/
  bool internal_addPoint(const plDataValues &point, plFloat weight);

  explicit
    plBayesLearnGamma(const plVariablesConjunction &vars)
    :plBayesLearnObject(vars)
    , v_(PL_ZERO)
    , mu_(PL_ZERO)
    , init_alpha_(PL_ONE)
    , init_lambda_(PL_ONE)
    , alpha_(PL_ONE)
    , lambda_(PL_ONE)
  {}

  virtual void Output(std::ostream &out) const;

 protected:  
  SPL_ACCESS1(plBayesLearnGamma);

  plFloat v_;
  plFloat mu_;
  plFloat init_alpha_;
  plFloat init_lambda_;

  plFloat alpha_;
  plFloat lambda_;

  template <class T> friend class plCndLearnObject;
};

#endif //plBayesLearnGamma_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
