/*=============================================================================
 * Product        : ProBT 
 * File           : plBayesLearnBinomial.h
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

#ifndef plBayesLearnBinomial_h
#define plBayesLearnBinomial_h

#include "plConfig.h"

#include <plNonCndLearnObject.h>
#include <plBeta.h>

SPL_DECL1(plBayesLearnBinomial);

/** This class allows to make a Bayesian estimation of a Binomial distribution
    when the prior distribution over the Binomial parameter is a Beta
    distribution.

    The conjugacy property of the Beta distribution with respect to the
    Binomial distribution ensures that the posterior distribution over the
    Binomial parameter is itself a Beta distribution whose parameters can be
    computed analytically.

    A Binomial with learned parameter @em p and fixed parameter @em n has the
    following probability distribution:

    \f[ P(k) = \frac{n!}{k! (n-k)!} p^k (1-p)^{n-k} \f]

    A Beta distribution over variable @em x, with parameters @em p and @em q,
    with bounds @em a and @em b, has the the density function (using the @em
    Beta function @em B):
    \f[ p(x) = \frac{(x-a)^{p-1} (b-x)^{q-1}}{B(p, q) (b - a)^{p+q-1}} \f]

    In this special case, the parameters @em a and @em b have values 0 and 1
    respectively because the Beta distribution is over the parameter @em p
    representing a probability value.
    
    The fixed parameter @em n is extracted from the variable @em variable
    definition (the maximum value of @em variable).
    
    @sa plBayesLearnObject plBeta
*/
class PL_DLL_API plBayesLearnBinomial :public plBayesLearnObject
{
 public:
  /** Type of the variable (1D). */
  typedef plSymbol VarType;
  
  /** Constructor using given initial values for the parameters @em p, @em q,
      of the prior Beta distribution.

      The most common case is to keep the default values for @em n, @em a and
      @em b.

      @param variable Variable of the Binomial you want to learn. The variable
      must be one-dimensional and discrete.

      @param initp Initial value of learned parameter @em p of the Beta
      distribution over the Binomial parameter. Must be strictly positive.

      @param initq Initial value of learned parameter @em q of the Beta
      distribution over the Binomial parameter. Must be strictly positive.
  */
  plBayesLearnBinomial(const plSymbol &variable,
                       plFloat initp, plFloat initq);

    

  /** Empty constructor */
  plBayesLearnBinomial()
    : plBayesLearnObject()
    , init_p_(PL_HALF)
    , init_q_(PL_HALF)
    , p_(PL_HALF)
    , q_(PL_HALF)
    , n_(1)
    {}

    
  /** Destructor. */
  virtual ~plBayesLearnBinomial(){}
  
  /** Resets learning (forgets all learned data). */
  void reset();

  
  /** Returns the @em p parameter of the current Beta distribution on
      the parameter of the learned Binomial distribution. */
  plFloat get_p()const;

  /** Returns the @em q parameter of the current Beta distribution on
      the parameter of the learned Binomial distribution. */
  plFloat get_q()const;

  /** Returns the @em n parameter of the current Beta distribution on
      the parameter of the learned Binomial distribution. This parameter
      is fixed at construction -- it is not learned.
  */
  plFloat get_n()const;
  
  /** Returns the Beta distribution corresponding to the posterior distribution
      over the Binomial parameter knowing all learned data.
      @param theta Variable that corresponds to the Binomial parameter, on
      which to build the posterior distribution.
  */
  plDistribution get_aposteriori_distribution(const plVariablesConjunction &theta) const;
  
  /** Returns the Binomial distribution corresponding to the expected parameter
      according to the learnt Beta distribution on the Binomial parameter.
  */
  plDistribution get_distribution() const;

  /** Returns the expectation of the posterior distribution on the Binomial
      parameter given learned data. */
  plFloat get_aposteriori_expectation() const;

  void get_parameters(plValues &params) const;

  unsigned int get_n_parameters()const
  {
    if(all_vars_.is_empty())
      throw plError(75, "when calling 'plBayesLearnBinomial::get_n_parameters'");

    return 1;
  }

  virtual plLearnObject* clone()const
  {
    return new plBayesLearnBinomial(*this);
  }
  
 protected:

  bool internal_addPoint(const plDataValues &point, plFloat weight);

   
  explicit
    plBayesLearnBinomial(const plSymbol &variable);

  virtual void Output(std::ostream &out) const;

 protected:  
  SPL_ACCESS1(plBayesLearnBinomial);

  plFloat init_p_;
  plFloat init_q_;

  plFloat p_;
  plFloat q_;

  unsigned int n_;

  template <class T> friend class plCndLearnObject;
};

#endif //plBayesLearnBinomial_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
