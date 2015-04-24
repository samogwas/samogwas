/*=============================================================================
 * Product        : ProBT 
 * File           : plLearnLinearRegressionFamily.h
 * Author         : Kamel Mekhnacha
 * Creation       : Thu Oct  12 13:45:57 2010
 *
 *=============================================================================
 *        (c) Copyright 2010, ProBayes SAS  -  all rights reserved
 *=============================================================================
*/

#ifndef plLearnLinearRegressionFamily_h
#define plLearnLinearRegressionFamily_h
 
#include "plConfig.h"

#include <plLearnCndObject.h>
#include <plVariablesConjunction.h>
#include <plLearnLinearRegression.h>
#include <plVariableIndexer.h>
#include <vector>

class plSymbol;


SPL_DECL1(plLearnLinearRegressionFamily);

/** Implements a family of linear regressions (over a set of
    quantitative variables), indexed by a set of categorical
    variables. Actually, given a categorical variable (or conjunction of
    variables) \f$ A \f$ with a cardinality of \f$ N_A \f$, this class
    constructs and learns a set of \f$ N_A \f$ linear regressions (see
    plLearnLinearRegression).

    \sa plLearnLinearRegression
 */
class PL_DLL_API plLearnLinearRegressionFamily : public plLearnCndObject
{
 public:
 /** Constructor.
     @param[in] left_var The regression quantitative output variable
     @param[in] quantitative_right_vars The right (parent) quantitative variables on which the regression is computed
      @param[in] categorical_right_vars  The right (parent) categorical variables for which we have a regression for each
      modality
     @param[in] regression_std_deviation the standart deviation to be used around the regression value. If -1, the residual error will be used.
     @param[in] inv_threshold The threshold used for \em Greville's algorithm used for matrix inversion
  */
  plLearnLinearRegressionFamily(const plSymbol& left_var,
				const plVariablesConjunction& quantitative_right_vars,
				const plVariablesConjunction& categorical_right_vars,
				plFloat regression_std_deviation = -PL_ONE,
				plFloat inv_threshold = 0.001);
  
  /** Default constructor to be used for serialisation */
  plLearnLinearRegressionFamily();

  // The default copy and assignment are OK
  
  /** Destructor */
  virtual ~plLearnLinearRegressionFamily();
  
  /** Resets learning to its initial state. */
  void reset();
  
  
  /** Constructs the corresponding learnt conditional distribution. It
      computes the \f$\beta\f$ vector for each regression and returns
      a plDistributionTable of plLinearRegression.
  */
  plCndDistribution get_cnd_distribution()const;

  unsigned int get_n_parameters()const;

  virtual plLearnObject* clone()const
  {
    return new plLearnLinearRegressionFamily(*this);
  }


 protected:
  bool internal_addPoint(const plDataValues &point, plFloat weight = PL_ONE);
    
  virtual void Output(std::ostream &out)const
  {
    out << "plLearnLinearRegressionFamily(" 
	<< left_vars_
	<< ", " 
	<< right_vars_ << ")";
  }
  

 private:

  plVariablesConjunction index_variables_;
  plVariableIndexer index_variables_indexer_;
  std::vector<plLearnLinearRegression> regressions_;
  
  mutable plDataValues class_attrib_values_;
  mutable plDataValues index_values_;
  
 protected:
  SPL_ACCESS1(plLearnLinearRegressionFamily);
  
  
};

#endif // plLearnLinearRegression_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
