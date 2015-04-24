/*=============================================================================
 * Product        : ProBT 
 * File           : plLearnSoftmaxFamily.h
 * Author         : Kamel Mekhnacha
 * Creation       : Thu Oct  12 13:45:57 2010
 *
 *=============================================================================
 *        (c) Copyright 2010, ProBayes SAS  -  all rights reserved
 *=============================================================================
*/

#ifndef plLearnSoftmaxFamily_h
#define plLearnSoftmaxFamily_h
 
#include "plConfig.h"

#include <plLearnCndObject.h>
#include <plVariablesConjunction.h>
#include <plLearnSoftmax.h>
#include <plVariableIndexer.h>
#include <vector>

class plSymbol;


SPL_DECL1(plLearnSoftmaxFamily);

/** Implements a family of softmax regressions (over a set of
    quantitative variables), indexed by a set of categorical
    variables. Actually, given a categorical variable (or conjunction of
    variables) \f$ A \f$ with a cardinality of \f$ N_A \f$, this class
    constructs and learns a set of \f$ N_A \f$ softmax regressions (see
    plLearnSoftmax).

    \sa plLearnSoftmax
 */
class PL_DLL_API plLearnSoftmaxFamily : public plLearnCndObject
{
 public:
  /** Constructor.
      @param[in] left_var The class variable
      @param[in] quantitative_right_vars The right (parent) quantitative variables on which the regression is computed
      @param[in] categorical_right_vars  The right (parent) categorical variables for which we have a regression for each
      modality
      @param[in] conv_threshold The convergence threshold to be used for the newton-raphson algorithm
      @param[in] inv_threshold The threshold to be used for \em Greville's algorithm for matrix inversion
  */
  plLearnSoftmaxFamily(const plSymbol& left_var,
		       const plVariablesConjunction& quantitative_right_vars,
		       const plVariablesConjunction& categorical_right_vars,
		       plFloat conv_threshold = 0.00001,
		       plFloat inv_threshold = 0.001);
  
  /** Default constructor to be used for serialisation */
  plLearnSoftmaxFamily();

  // The default copy and assignment are OK
  
  /** Destructor */
  virtual ~plLearnSoftmaxFamily();
  
  /** Resets learning to its initial state. */
  void reset();
  
  
  /** Constructs the corresponding learnt conditional distribution. It
      computes the \f$\beta\f$ vector for each regression and returns
      a plDistributionTable of plSoftmax.
  */
  plCndDistribution get_cnd_distribution()const;


  unsigned int get_n_parameters()const;

  virtual plLearnObject* clone()const
  {
    return new plLearnSoftmaxFamily(*this);
  }


 protected:
  bool internal_addPoint(const plDataValues &point, plFloat weight = PL_ONE);

  virtual void Output(std::ostream &out)const
  {
    out << "plLearnSoftmaxFamily(" 
	<< left_vars_
	<< ", " 
	<< right_vars_ << ")";
  }
    
 private:
  plVariablesConjunction index_variables_;
  plVariableIndexer index_variables_indexer_;
  std::vector<plLearnSoftmax> softmaxs_;
  
  mutable plDataValues class_attrib_values_;
  mutable plDataValues index_values_;
  
 protected:
  SPL_ACCESS1(plLearnSoftmaxFamily);
  
  
};

#endif // plLearnSoftmaxFamily_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
