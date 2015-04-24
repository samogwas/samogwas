/*=============================================================================
 * Product        : ProBT 
 * File           : plSoftmax.h
 * Author         : Kamel Mekhnacha
 * Creation       : Thu Oct  12 13:45:57 2010
 *
 *=============================================================================
 *        (c) Copyright 2010, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */

#ifndef plSoftmax_h
#define plSoftmax_h

#include "plConfig.h"


#include <plMath.h>
#include <plCndDistribution.h>

#include <vector>


/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/

class plSymbol;
class plVariablesConjunction;

/** This implements the computation part of the Softmax (aka
    multinomial logistic or multinomial logit) regression. It's a
    generalization of the binary Sigmoid regression for multiclass
    cases.

    Given a discrete class variable \f$C\f$ with \f$m\f$ modalities,
    and a set of \f$ n \f$ quantitative attribute variables \f$A_1,
    \cdots,A_n \f$, the Softmax regression assumes that \f[ \log \frac
    {P(C=k~|~A_1=a_1,\cdots,A_n=a_n)}
    {P(C>k~|~A_1=a_1,\cdots,A_n=a_n)} = \sum_{i=1}^n \beta_i^k
    \times a_i + \beta_{n+1}^k, k=0,\cdots, m-2,\f] 
    and \f[P(C=m-1~|~A_1=a_1,\cdots,A_n=a_n) =
    1 - \sum_{k=0}^{m-2} P(C=k~|~A_1=a_1,\cdots,A_n=a_n). \f]

    This leads to the recursive formula:
\f[ P(C=0~|~A_1=a_1,\cdots,A_n=a_n) = P_0(A) = Sigmoid( z\left(A, \beta^0 ) \right),\f]
    \f[ P(C=k~|~A_1=a_1,\cdots,A_n=a_n) = P_k(A) = \left[ 1 - \sum_{i=0}^{k-1} P_i(A)  \right] \times Sigmoid( z\left(A, \beta^k ) \right), k=0,\cdots, m-2,\f]
\f[ P(C=m-1~|~A_1=a_1,\cdots,A_n=a_n) = P_{m-1}(A) = 1 - \sum_{i=0}^{m-2} P_i(A), \f]
 in which 
    \f[ z(A, \beta^k) = \sum_{i=1}^n \beta_i^k \times a_i + \beta_{n+1}^k, \f] 
    \f[ Sigmoid(t) = \frac {1} {1+\exp(-t)}. \f]

    The parameter matrix \f$\beta\f$ can be learnt using plLearnSoftmax.

    \sa plLearnSoftmax
    \sa plLearnNoisyOR
    \sa plNoisyOR
*/
class PL_DLL_API plSoftmax : public plCndDistribution
{ 
  
public:
  /** Default constructor */
  plSoftmax()
    : plCndDistribution()
    { signature_.set(PLSoftmax); }

  /** Constructs a \em plSoftmax distribution.
      @param[in] left_var The  class variable (n modalities)
      @param[in] right_variables The m quantitative attribute parent variables 
      @param[in] beta The beta parameter vector. It should be of size (n-1) x (m+1)
  */
  plSoftmax(const plSymbol& left_var,
	    const plVariablesConjunction &right_variables,
	    const std::vector <std::vector<plFloat> > &beta);

  /** Destructor */
  virtual ~plSoftmax(){};

  /** Promote from a plCndDistribution. If the object being copied is actually of the
      wrong type, an exception is raised.
  */
  explicit plSoftmax(const plCndDistribution&);

  /** Get the beta parameter vector */
  const std::vector <std::vector<plFloat> >  &get_beta()const;

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
