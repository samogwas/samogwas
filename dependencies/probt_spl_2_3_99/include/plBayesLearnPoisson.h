/*=============================================================================
 * Product        : ProBT 
 * File           : plBayesLearnPoisson.h
 * Author         : Kamel Mekhnacha
 * Creation       : Mon Sept 13 14:57:12 2005
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


#ifndef plBayesLearnPoisson_h
#define plBayesLearnPoisson_h

#include "plConfig.h"

#include <plNonCndLearnObject.h>
#include <plGamma.h>
#include <plPoisson.h>

SPL_DECL1(plBayesLearnPoisson);

/** This class allows to make a Bayesian estimation of Poisson distributions.
    To take advantage of the conjugacy property, the prior distribution is 
    supposed to be a Gamma distribution and the a posteriori distribution is 
    a Gamma distribution too.
*/
class PL_DLL_API plBayesLearnPoisson :public plBayesLearnObject
{
 public:
  /** Type of the variable (1D). */
  typedef plSymbol VarType;
  
  /** Constructor using given initial values for the 'alpha' and 'lambda'
      (1/beta) parameters of the prior Gamma distribution.\n
      The 'alpha' corresponds to the number of observed (supposed) number of
      events while 'lambda' corresponds on the time intervals number in which
      these events have been observed (supposed).
  */
  plBayesLearnPoisson(const plSymbol &vars, 
                      plFloat init_alpha, plFloat init_lambda)
    :plBayesLearnObject(vars),init_alpha_(init_alpha),init_lambda_(init_lambda), 
    alpha_(init_alpha_), lambda_(init_lambda_)
  {}  

  /** Empty constructor */
  plBayesLearnPoisson()
    : plBayesLearnObject()
    , init_alpha_(PL_ONE)
    , init_lambda_(PL_ONE)
    , alpha_(PL_ONE)
    , lambda_(PL_ONE)
  {}
  
  /** Dectructor. */
  virtual ~plBayesLearnPoisson(){}
    
  /** Resets learning. */
  void reset();


  /** Returns the 'alpha' parameter. */
  plFloat get_alpha()const;

  /** Returns the 'lambda' parameter. */
  plFloat get_lambda()const;
  
  /** Returns the Gamma distribution corresponding to the aposteriori
      distribution */
  plDistribution get_aposteriori_distribution(
                                              const plVariablesConjunction &poisson_mean_variable) const;
  
  /** Returns the Poisson distribution corresponding to the learnt distribution.
      The 'mean' parameter is the expectation of the a posteriori distribution.
  */
  plDistribution get_distribution() const;

  /** Returns the expectation of the a posteriori distribution */
  plFloat get_a_posteriori_expectation()const;

  void get_parameters(plValues &params) const;

  unsigned int get_n_parameters()const
  {
    if(all_vars_.is_empty())
      throw plError(75, "when calling 'plBayesLearnPoisson::get_n_parameters");

    return 1;
  }

  virtual plLearnObject* clone()const
  {
    return new plBayesLearnPoisson(*this);
  }

 protected:
  /** Adds a point \em point with a given weight \em weight and updates 
      the statistics. 
      This is an internal method, do not use it directly. Use add_point
      instead.
  */
  bool internal_addPoint(const plDataValues &point, plFloat weight);
  
  explicit 
    plBayesLearnPoisson(const plSymbol &vars)
    : plBayesLearnObject(vars)
    , init_alpha_(PL_ONE)
    , init_lambda_(PL_ONE)
    , alpha_(PL_ONE)
    , lambda_(PL_ONE)
  {}  
 
  virtual void Output(std::ostream &out) const;


 protected:  
  SPL_ACCESS1(plBayesLearnPoisson);

  plFloat init_alpha_;
  plFloat init_lambda_;

  plFloat alpha_;
  plFloat lambda_;

  template <class T> friend class plCndLearnObject;
};

#endif //plBayesLearnPoisson_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
