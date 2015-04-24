/*=============================================================================
 * Product        : ProBT 
 * File           : plLearnLinearRegression.h
 * Author         : Kamel Mekhnacha
 * Creation       : Thu Oct  12 13:45:57 2010
 *
 *=============================================================================
 *        (c) Copyright 2010, ProBayes SAS  -  all rights reserved
 *=============================================================================
*/

#ifndef plLearnLinearRegression_h
#define plLearnLinearRegression_h
 
#include "plConfig.h"

#include <plLearnCndObject.h>
#include <plVariablesConjunction.h>
#include <plFloatVector.h>

class plFloatMatrix;
class plFloatVector;
class plSymbol;


SPL_DECL1(plLearnLinearRegression);

/** This implements the learning part of the linear regression model
    (aka Linear Gaussian). Given a continuous variable \f$Y\f$, and a
    set of \f$ n \f$ quantitative attribute variables \f$X_1,
    \cdots,X_n \f$, the probability density function over \f$Y\f$ is
    defined as: \f[ p(Y~|~X_1=x_1,\cdots,X_n=x_n) = Normal(y,
    mean(x_1, ...,x_n), \sigma),\f] in which \f[ mean(x_1, \cdots,x_n)
    = \sum_{i=1}^n \beta_i \times x_i + \beta_{n+1},\f] and \f$ \sigma
    \f$ is either a user-provided constant or computed as the residual
    error of the regression.
 
    The learning algorithm computes the parameter vector \f$\beta\f$
    as a solution for a linear problem. Thus, unlike the other
    learning objects, plLearnLinearRegression is not incremental and
    needs to store all the data points added using
    plLearnObject::add_point().

    \sa plLinearRegression
*/
class PL_DLL_API plLearnLinearRegression : public plLearnCndObject
{
 public:
  /** Constructor.
      @param[in] left_var The regression output variables
      @param[in] right_vars The regression input variables
      @param[in] regression_std_deviation the standart deviation to be used around the regression value. If -1, the residual error will be used.
      @param[in] inv_threshold The threshold used for \em Greville's algorithm used for matrix inversion
  */
  plLearnLinearRegression(const plSymbol& left_var,
			  const plVariablesConjunction& right_vars,
			  plFloat regression_std_deviation = -PL_ONE,
			  plFloat inv_threshold = 0.001);
  
  /** Default constructor to be used for serialisation */
  plLearnLinearRegression();
  
  // The default copy and assignment are OK
  
  /** Destructor */
  virtual ~plLearnLinearRegression();
  
  /** Resets learning to its initial state. */
  void reset();
  
  
  /** Constructs the corresponding learnt conditional distribution. It
      computes the \f$\beta\f$ vector and returns a plLinearRegression
      with the computed \f$\beta\f$.
  */
  plCndDistribution get_cnd_distribution()const;

  unsigned int get_n_parameters()const;
  
  virtual plLearnObject* clone()const
  {
    return new plLearnLinearRegression(*this);
  }


  /** Get the current learnt beta */
  const plFloatVector& get_beta() const;
  
  /** Get the regression standard deviation **/
  inline plFloat get_regression_std_deviation() const
  {
    return regression_std_deviation_;
  }

 protected:
  bool internal_addPoint(const plDataValues &point, plFloat weight = PL_ONE);
  
 virtual void Output(std::ostream &out)const
  {
    out << "plLearnLinearRegression(" 
	<< left_vars_
	<< ", " 
	<< right_vars_ << ")";
  }
  
 protected:

  void compute_beta()const;
  
 protected:
  SPL_ACCESS1(plLearnLinearRegression);
  friend class plLearnLinearRegressionFamily;
   
  mutable std::vector<plDataValues> target_attribs_values_;
  std::vector<plFloat> weights_;
  
  mutable unsigned int nattributs_;
  mutable plFloatVector beta_;
  mutable plFloat regression_std_deviation_;
  mutable bool is_up_to_date_;
  
  plFloat inv_threshold_;
  bool use_residual_as_std_deviation_;
};

#endif // plLearnLinearRegression_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
