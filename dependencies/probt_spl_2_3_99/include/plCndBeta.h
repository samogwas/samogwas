/*=============================================================================
 * Product        : ProBT 
 * File           : plCndBeta.h
 * Author         : Kamel Mekhnacha
 * Creation       : October 2011
 *
 *=============================================================================
 *     (c) Copyright 2011, Probayes
 *=============================================================================
 *
 *------------------------- Description ---------------------------------------
 *
 *   
 *-----------------------------------------------------------------------------
 */

#ifndef plCndBeta_h
#define plCndBeta_h

#include "plConfig.h"


#include <plCndDistribution.h>
#include <plSymbol.h>

class plExternalFunction;

/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/


/** This class implements conditional Beta distributions. A plCndBeta is a
    plBeta family for which p and q are not constant but are variables or user
    external functions (assuming a = 0 and b = 1). When it is instantiated, a
    plBeta is obtained.  */
class PL_DLL_API plCndBeta: public plCndDistribution
{
 public:

  /** Default contructor */
  plCndBeta();
  
  /** Constructs a one-dimensional conditional Beta distribution on the \em left variable.
      \em p is a know variable,
      \em q is a fixed value corresponding to the q parameter */
  plCndBeta(const plSymbol& left,
            const plSymbol& p,
            plFloat q);
  
  /** Constructs  a one-dimensional conditional Beta distribution on the \em left variable.
      \em q is a know variable
      \em p is a fixed value corresponding to the p value. */
  plCndBeta(const plSymbol& left,
            plFloat p,
            const plSymbol& q);
  
  /** Constructs a one-dimensional conditionnel Beta distribution on the \em left variable.
      with \em p = \em fp(right) and 
      \em q is a fixed value corresponding to the q parameter.

      @anchor param_func_example
      An example assuming that the p value of the conditional Beta P(X | Y Z) is the squared sum
      of the value of the two parent variables "Y" and "Z" is as follows. It
      assumes the following variable definitions:

      @code 
      const plSymbol X("X", plRealType(-1000, 1000) );
      const plSymbol Y("Y", plRealType(-1000, 1000) );	   
      const plSymbol Z("Z", plRealType(-1000, 1000) );
      @endcode

      1) Using a non-member function:
      @code 
      // Non member function
      void sum2(plValues &p, const plValues &Y_Z_vals)
      {
      const double s = Y_Z_vals[Y] + Y_Z_vals[Z];
      // p is indexed by an integer 
      p[0] = s*s;
      }
      
      plExternalFunction F_p__YZ(Y^Z, &sum2);
      plCndBeta P_X__YZ(X, Y^Z, F_p__YZ, 1);
      @endcode
     
      2) Using a member function:
      @code 
      struct YZSum
      {
      void sum2(plValues &p, const plValues &Y_Z_vals) const
      {
      const double s = Y_Z_vals[Y] + Y_Z_vals[Z];
      // p is indexed by an integer 
      p[0] = s*s;
      }
     
      };
     
      YZSum *sum_obj_ptr = new YZSum;
      plExternalFunction  F_p__YZ(Y^Z, sum_obj_ptr, &YZSum::sum2);
      plCndBeta P_X__YZ(X, Y^Z, F_p__YZ, 1);
      @endcode
     
      3) Using a C source code function:
      @code 
      const std::string sum2_code = "output[0] = (Y+Z)*(Y+Z);";
      plExternalFunctionFromC  F_p__YZ = createExternalFunctionFromC(Y^Z, sum2_code, "sum2_func");
      plCndBeta P_X__YZ(X, Y^Z, F_p__YZ, 1);
      @endcode
  */
  plCndBeta(const plSymbol& left,
            const plVariablesConjunction& right,
            const plExternalFunction &fp,
            plFloat q);
   
  /** Constructs a one-dimensional contitional Beta distribution on the \em left variable
      with  \em q = \em fq(right) and 
      \em p is a fixed value corresponding to the p value.

      An example of how to create and use plExternalFunction as conditional distribution
      parameters can be found in @ref param_func_example "'using external functions example'".
  */
  plCndBeta(const plSymbol& left,
            const plVariablesConjunction& right,
            plFloat p,
            const plExternalFunction &fq);

  /** Constructs a contitional Beta distribution on \em left variables.
      \em p and \em q are known variables.
  */
  plCndBeta(const plSymbol& left,
            const plSymbol& p,
            const plSymbol& q);

  /** Constructs a contitional Beta distribution on the \em left variables
      with  \em p = \em fp(right) and \em p = \em fq(right).

      An example of how to create and use plExternalFunction as conditional distribution
      parameters can be found in @ref param_func_example "'using external functions example'".
  */
  plCndBeta(const plSymbol& left,
            const plVariablesConjunction& right,
            const plExternalFunction &fp,
            const plExternalFunction &fq);

  /** Constructs a contitional Beta distribution on the \em left variables
      with  \em [p,q] = \em f_p_q(right).

      An example of how to create and use plExternalFunction as conditional distribution
      parameters can be found in @ref param_func_example "'using external functions example'".
  */
  plCndBeta(const plSymbol& left,
            const plVariablesConjunction& right,
            const plExternalFunction &f_p_q);

  /** Destructor */
  virtual ~plCndBeta(){};

  /** Promote from a plCndDistribution. If the object being copied is
      actually of the wrong type, an exception is raised.
  */
  explicit plCndBeta(const plCndDistribution&);

  

  ///@name Parameters accesors.
  ///@{

  /** Returns true if the p parameter is a constant. */
  bool p_is_constant() const;
  /** Returns the value of the p parameter. Do not call this method if
      p_is_constant() returned false. */
  plFloat get_p_constant() const;

  /** Returns true if the p parameter is a variable. */
  bool p_is_variable() const;
  /** Returns the p variable. Do not call this method if
      p_is_variable() returned false. */
  plSymbol get_p_variable() const;

  /** Returns true if the p parameter is a function. */
  bool p_is_function() const;
  /** Returns the p function. Do not call this method if
      p_is_function() returned false. */
  plExternalFunction get_p_function() const;

  /** Returns true if the q  parameter is a constant. */
  bool q_is_constant() const;
  /** Returns the value of the q  parameter. Do not call this
      method if q_is_constant() returned false. */
  plFloat get_q_constant() const;

  /** Returns true if the q  parameter is a variable. */
  bool q_is_variable() const;
  /** Returns the q  variable. Do not call this method if
      q_is_variable() returned false. */
  plSymbol get_q_variable() const;

  /** Returns true if the q parameter is a function. */
  bool q_is_function() const;
  /** Returns the q  function. Do not call this method if
      q_is_function() returned false. */
  plExternalFunction get_q_function() const;

  /** Returns true if the p and q  parameters were
      given as a single function. */
  bool p_and_q_are_one_function() const;
  /** Returns the parameters function. Do not call this method if
      p_and_q_are_one_function() returned false. */
  plExternalFunction get_p_q_function() const;

  ///@}
 
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
