/*=============================================================================
 * Product        : ProBT 
 * File           : plLearnSoftmax.h
 * Author         : Kamel Mekhnacha
 * Creation       : Thu Oct  12 13:45:57 2010
 *
 *=============================================================================
 *        (c) Copyright 2010, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */

#ifndef plLearnSoftmax_h
#define plLearnSoftmax_h
 
#include "plConfig.h"

#include <plLearnCndObject.h>
#include <plVariablesConjunction.h>
#include <plFloatVector.h>


class plFloatMatrix;
class plSymbol;


SPL_DECL1(plLearnSoftmax);

/** This implements the learning part of the Softmax (aka multinomial
    logistic or multinomial logit) regression. It's a generalization
    of the binary Sigmoid regression for multiclass cases.

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

    The learning algorithm computes the parameter matrix \f$\beta\f$
    using an iterative algorithm (newton-raphson). Thus, unlike the
    other learning objects, plLearnSoftmax is not incremental and
    needs to store all the data points added using
    plLearnObject::add_point().

    \sa plSoftmax
    \sa plLearnNoisyOR
*/
class PL_DLL_API plLearnSoftmax : public plLearnCndObject
{
 public:
  /** Constructor.
      @param[in] left_var The class variable
      @param[in] right_vars The quantitative attribute parent variables
      @param[in] conv_threshold The convergence threshold to be used for the newton-raphson algorithm
      @param[in] inv_threshold The threshold used for \em Greville's algorithm used for the Hessian matrix inversion
  */
  plLearnSoftmax(const plSymbol& left_var,
		 const plVariablesConjunction& right_vars,
		 plFloat conv_threshold = 0.00001,
		 plFloat inv_threshold = 0.001);
  
  /** Default constructor to be used for serialisation */
  plLearnSoftmax();

  // The default copy and assignment are OK
  
  /** Destructor */
  virtual ~plLearnSoftmax();
  
  /** Resets learning to its initial state. */
  void reset();
  
  
  /** Constructs the corresponding learnt distribution. It computes
      the \f$\beta\f$ matrix using an iterative algorithm
      (newton-raphson) and returns a plSoftmax with the computed
      \f$\beta\f$.
  */
  plCndDistribution get_cnd_distribution()const;


  unsigned int get_n_parameters()const;

  virtual plLearnObject* clone()const
  {
    return new plLearnSoftmax(*this);
  }


 protected:
  bool internal_addPoint(const plDataValues &point, plFloat weight = PL_ONE);

  virtual void Output(std::ostream &out)const
  {
    out << "plLearnSoftmax(" 
	<< left_vars_
	<< ", " 
	<< right_vars_ << ")";
  }

 private:

  void compute_beta()const;
  void newton_raphson(unsigned int c,
		      plFloatMatrix &hess, plFloatVector &grad,
		      plFloatVector &xir,
		      plFloatMatrix &xi,
		      plFloatMatrix &xip,
		      plFloatMatrix &inv_hess,
		      plFloatVector &dbeta,
		      plFloatMatrix &xip_xi)const;
  
  plFloat update_hess_grad(unsigned int c,
			plFloatMatrix &hess, plFloatVector &grad,
			plFloatVector &xir,
			plFloatMatrix &xi,
			plFloatMatrix &xip,
			plFloatMatrix &xip_xi)const;

  plFloat get_err2(unsigned int c,
                   plFloatVector &xir)const;

 protected:
  SPL_ACCESS1(plLearnSoftmax);
  friend class plLearnSoftmaxFamily;

  std::vector<plDataValues> class_attribs_values_;
  std::vector<plFloat> weights_;
  unsigned int nattributs_;
  unsigned int card_minus_one_;
  mutable std::vector<plFloatVector> beta_;
  mutable bool is_up_to_date_;
   
  plFloat conv_threshold_;
  plFloat inv_threshold_;
  plType class_type_;
};

#endif // plLearnSoftmax_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
