/*=============================================================================
 * Product        : ProBT 
 * File           : plLearnNoisyOR.h
 * Author         : Kamel Mekhnacha
 * Creation       : Thu Oct  12 13:45:57 2010
 *
 *=============================================================================
 *        (c) Copyright 2010, ProBayes SAS  -  all rights reserved
 *=============================================================================
*/

#ifndef plLearnNoisyOR_h
#define plLearnNoisyOR_h
 
#include "plConfig.h"

#include <plLearnCndObject.h>
#include <plVariablesConjunction.h>


class plFloatMatrix;
class plSymbol;


SPL_DECL1(plLearnNoisyOR);




/** This class implements the learning part of the NoisyOR
    discrimination model.  Given a binary class variable \f$C\f$, and
    a set of \f$ n \f$ discrete attribute variables \f$A_1, \cdots,A_n
    \f$, the idea of the NoisyOR model is to assume that the value of
    the binary class variable \f$C\f$ is the deterministic result of
    the OR operator on another set of \f$ n \f$ intermediate (latent)
    binary variables \f$B_1, \cdots,B_n \f$. The parameters of the \f$
    n \f$ distributions \f$ P(B_i~|~ A_i)\f$ are computed using an EM
    algorithm (see the corresponding initial and transformed graphs at
    the end of this section).

    This leads to the formula:
     \f[ 
     P(C=0~|~A_1=a_1,\cdots,A_n=a_n) = \prod_{i=1}^{n} P(B_i=0~|~A_i=a_i).  
     \f]

    The learning algorithm computes the parameter matrix \f$ P(B_i =
      0~|~A_i = a_i^j)\f$ for \f$ i = 1,\cdots,n; j =
      1,\cdots,card(A_i) \f$ using an iterative algorithm
      (EM). Thus, unlike the other learning objects, plLearnNoisyOR is
      not incremental and needs to store all the data points added
      using plLearnObject::add_point().

    @verbatim
                 C

              ^    ^
             /      \
            /        \
           /          \
          /            \
         /              \

        A1                An

          INITIAL MODEL



                |
                |
                |
                |
                |
                V 



                C

              ^    ^
             /      \
            /        \
           /          \
          /            \
         /              \

        B1  .........    Bn

        ^                 ^
        |                 |
        |   .........     |
        |                 |
        |                 |
        |                 |

        A1                An

          TRANSFORMED MODEL
    @endverbatim

    \sa plNoisyOR
    \sa plLearnSoftmax
 */
class PL_DLL_API plLearnNoisyOR : public plLearnCndObject
{
 public:
  /** Constructor.
      @param[in] left_binary_var The binary class variable
      @param[in] right_vars The \f$ n \f$ attribute variables \f$ A_i, i=1,\cdots,n\f$ 
      @param[in] n_em_it The number of iterations for the EM algorithm
  */
  plLearnNoisyOR(const plSymbol& left_binary_var,
		 const plVariablesConjunction& right_vars,
		 unsigned int n_em_it);
  
  /** Default constructor to be used for serialisation */
  plLearnNoisyOR();

  // The default copy and assignment are OK
  
  /** Destructor */
  virtual ~plLearnNoisyOR();
  
  /** Resets learning to its initial state. */
  void reset();
  
  
  /** Constructs the conditional distribution corresponding to the
      learnt conditional distribution. It computes the parameter
      matrix \f$ P(B_i = 0~|~A_i = a_i^j)\f$ for \f$ i = 1,\cdots,n; j
      = 1,\cdots,card(A_i) \f$ using an EM algorithm and returns a
      plNoisyOR with the computed parameters.
  */
  plCndDistribution get_cnd_distribution()const;

  unsigned int get_n_parameters()const;

  virtual plLearnObject* clone()const
  {
    return new plLearnNoisyOR(*this);
  }


 protected:
  bool internal_addPoint(const plDataValues &point, plFloat weight = PL_ONE);

  bool internal_addPointWithDef(const plDataValues &point,
				const std::vector <bool> &is_defined,
				plFloat weight = PL_ONE);

  virtual void Output(std::ostream &out)const
  {
    out << "plLearnNoisyOR(" 
	<< left_vars_
	<< ", " 
	<< right_vars_ << ")";
  }
 
 private:
  void compute_B_0_probabilities()const;
  int compute_nparams()const;
  void em_iteration(std::vector <std::vector <plProbValue> > &count_B_0,
		    std::vector <std::vector <plProbValue> > &count_a) const;
  void reset_B_0_probabilities();
  void init_tables(std::vector <std::vector <plProbValue> > &count_B_0,
		   std::vector <std::vector <plProbValue> > &count_a) const;
 void init_attrib_types();
 void compute_indices(const plDataValues &class_attribs_values_line,
		      const std::vector <bool>  &is_defined_line,
		      std::vector<unsigned int> &indices,
		      std::vector<bool> &defined_and_inrange)const;
 
 protected:
  SPL_ACCESS1(plLearnNoisyOR);
   
  std::vector<plDataValues> class_attribs_values_;
  std::vector<std::vector<bool> > class_attribs_definition_;
  std::vector<plFloat> weights_;
  
  unsigned int nattributs_;

  // P(Bi = 0 | Ai = aj), i = 0.._nattribs-1, j = 0..card(Ai)-1
  mutable std::vector <std::vector <plProbValue> > table_B_0_probabilities_; 
  
  mutable bool is_up_to_date_;
   
  unsigned int n_em_it_;
  std::vector <bool> is_def_true_;
  mutable int nparams_;
  std::vector<plType> attrib_types_;
  plType class_type_;
};

#endif // plLearnNoisyOR_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
