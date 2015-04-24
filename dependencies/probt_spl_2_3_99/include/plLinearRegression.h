/*=============================================================================
 * Product        : ProBT 
 * File           : plLinearRegression.h
 * Author         : Kamel Mekhnacha
 * Creation       : Thu Oct  12 13:45:57 2010
 *
 *=============================================================================
 *        (c) Copyright 2010, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */

#ifndef plLinearRegression_h
#define plLinearRegression_h

#include "plConfig.h"


#include <plMath.h>
#include <plCndDistribution.h>

#include <vector>


/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/

class plSymbol;
class plVariablesConjunction;

/** This implements the computation part of the linear regression
    model (aka Linear Gaussian). Given a continuous variable \f$Y\f$,
    and a set of \f$ n \f$ quantitative attribute variables \f$X_1,
    \cdots,X_n \f$, the probability density function over \f$Y\f$ is
    defined as: \f[ p(Y~|~X_1=x_1,\cdots,X_n=x_n) = Normal(y,
    mean(x_1, ...,x_n), \sigma),\f] in which \f[ mean(x_1, \cdots,x_n)
    = \sum_{i=1}^n \beta_i \times x_i + \beta_{n+1}.\f]

    The parameter vector \f$ \beta \f$ can be learnt using
    plLearnLinearRegression.

    \sa plLearnLinearRegression
*/
class PL_DLL_API plLinearRegression : public plCndDistribution
{ 
  
public:
  /** Default constructor */
  plLinearRegression() { signature_.set(PLLinearRegression); }

  /** Constructs a \em plLinearRegression distribution.
      @param[in] left_var The regression quantitative output variable
      @param[in] right_variables  The right (parent) quantitative variables of which the regression is computed
      @param[in] beta The beta parameter vector
      @param[in] regression_std_deviation the standart deviation to be used around the regression value.
  */
  plLinearRegression(const plSymbol& left_var,
		     const plVariablesConjunction &right_variables,
		     const std::vector<plFloat> &beta,
		     plFloat regression_std_deviation);

  /** Destructor */
  virtual ~plLinearRegression(){};

  /** Promote from a plCndDistribution. If the object being copied is actually of the
      wrong type, an exception is raised.
  */
  explicit plLinearRegression(const plCndDistribution&);

  /** Get the beta parameter vector */
  const std::vector<plFloat> &get_beta()const;

  /** Get the regression residual strandard deviation */
  plFloat get_regression_std_deviation()const;

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
