/*=============================================================================
 * Product        : ProBT 
 * File           : plNoisyOR.h
 * Author         : Kamel Mekhnacha
 * Creation       : Thu Oct  12 13:45:57 2010
 *
 *=============================================================================
 *        (c) Copyright 2010, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */

#ifndef plNoisyOR_h
#define plNoisyOR_h

#include "plConfig.h"


#include <plMath.h>
#include <plCndDistribution.h>

#include <vector>


/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/

class plSymbol;
class plVariablesConjunction;

/** This class implements the computation part of the NoisyOR
    discrimination model. Given a binary class variable \f$C\f$, and a
    set of \f$ n \f$ discrete attribute variables \f$A_1, \cdots,A_n
    \f$, the idea of the NoisyOR model is to assume that the value of
    the binary class variable \f$C\f$ is the deterministic result of
    the OR operator on another set of \f$ n \f$ intermediate (latent)
    binary variables \f$B_1, \cdots,B_n \f$ (see the corresponding
    initial and transformed graphs at the end of this section).

    This leads to the formula:
     \f[ 
     P(C=0~|~A_1=a_1,\cdots,A_n=a_n) = \prod_{i=1}^{n} P(B_i=0~|~A_i=a_i).  
     \f]

    The parameter matrix \f$ P(B_i = 0~|~A_i = a_i^j)\f$ for \f$ i =
    1,\cdots,n; j = 1,\cdots,card(A_i) \f$ can be computed using an
    EM algorithm (see plLearnNoisyOR).

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

    \sa plLearnNoisyOR
    \sa plSoftmax
 */
class PL_DLL_API plNoisyOR : public plCndDistribution
{ 
  
public:
  /** Default constructor */
  plNoisyOR() { signature_.set(PLNoisyOR); }

  /** Constructs a \em plNoisyOR distribution.  

      @param[in] left_binary_var The binary class variable
      @param[in] right_variables The \f$ n \f$ attribute variables \f$
      A_i, i=0..n-1\f$
      @param[in] table_B_0_probabilities The parameter matrix \f$
      P(B_i = 0~|~A_i = a_i^j)\f$ for \f$ i = 1,\cdots,n; j =
      1,\cdots,card(A_i) \f$
  */
  plNoisyOR(const plSymbol& left_binary_var,
	    const plVariablesConjunction &right_variables,
	    const std::vector <std::vector <plProbValue> > &table_B_0_probabilities);

  /** Destructor */
  virtual ~plNoisyOR(){};

  /** Promote from a plCndDistribution. If the object being copied is actually of the
      wrong type, an exception is raised.
  */
  explicit plNoisyOR(const plCndDistribution&);

  /** Get the parameter matrix \f$ P(B_i = 0~|~A_i = a_i^j)\f$ for \f$
      i = 1,\cdots,n; j = 1,\cdots,card(A_i) \f$
  */
  const std::vector <std::vector <plProbValue> > &get_parameters() const;

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
