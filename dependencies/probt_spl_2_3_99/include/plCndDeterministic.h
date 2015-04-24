/*=============================================================================
 * Product        : ProBT 
 * File           : plCndDeterministic.h
 * Author         : Juan-Manuel Ahuactzin
 * Creation       : Fri Jun  1 15:15:54 2001
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

#ifndef plCndDeterministic_h
#define plCndDeterministic_h

#include "plConfig.h"

#include <plCndDistribution.h>
#include <plVariablesConjunction.h>
#include <plDistribution.h>
#include <plExternalFunction.h>


/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/


/** The \em plCndDeterministic class implements deterministic dependencies
    using user-provided functions */
class PL_DLL_API plCndDeterministic : public plCndDistribution
{
 public:
  
  /** Creates a deterministic (Dirac) conditional distribution.  The parameter
      \em func is a function provided by the user where \em left_variables =
      \em func(right_variable): \f[ P(left\_variables) = \left\{
      \begin{array}{ll} p & if~~ left\_variables = func(right\_variable) \\ 0 &
      otherwise \end{array} \right.  \f] where \f$ p = 1\f$ for discrete
      variables and \f$ p = \infty \f$ for continuous ones.  
      
      @anchor dirac_func_example
      In the following, we will present 3 ways for defining a plCndDeterministic
      stating that the "C" variable is the sum of the "A" and "B" variables. It
      assumes the folloiwng variable definitions:

      @code 
      const plSymbol A("A", plIntegerType(0, 10) );
      const plSymbol B("B", plIntegerType(0, 10) );	   
      const plSymbol C("C", plIntegerType(0, 20) );
      @endcode

      1) Using a non-member function:
      @code 
      // Non member function
      void sum(plValues &C_val, const plValues &A_B_vals)
      {
      C_val[C] = A_B_vals[A] + A_B_vals[B];
      }
      
      plExternalFunction F_C__AB(C, A^B, &sum);
      plCndDeterministic P_C__AB(C, A^B, F_C__AB);
      @endcode
     
      2) Using a member function:
      @code 
      struct ABSum
      {
      void sum_method(plValues &C_val, const plValues &A_B_vals) const
      {
      C_val[C] = A_B_vals[A] + A_B_vals[B];
      }
     
      };
     
      ABSum *sum_obj_ptr = new ABSum;
      plExternalFunction F_C__AB(C, A^B, sum_obj_ptr, &ABSum::sum_method);
      plCndDeterministic P_C__AB(C, A^B, F_C__AB);
      @endcode
     
      3) Using a C source code function:
      @code 
      const std::string sum_code = "C = A + B";
      plExternalFunctionFromC  F_C__AB = createExternalFunctionFromC(C, A^B, sum_code, "sum_func");
      plCndDeterministic P_C__AB(C, A^B, F_C__AB);
      @endcode

      ATTENTION: for performance reasons, using this constructor requires that the passed (left and right)
      variables have the same order as the function's (left and right)
      variables. You can also use the constructor plCndDeterministic(const plExternalFunction& func ) below.
  */
  plCndDeterministic(const plVariablesConjunction& left_variables, 
		    const plVariablesConjunction& right_variables, 
		    const plExternalFunction& func );

  
  /** Same as the constructr above however the left and right variables are
      assumed to be the same as the ones provided by the passed function.
  */
  explicit plCndDeterministic(const plExternalFunction& func );
 
  /** Default constructor */
  plCndDeterministic();

  /** Destroys the dirac function */
  virtual ~plCndDeterministic (){};

  /** Promote from a plCndDistribution. If the object being copied is
      actually of the wrong type, an exception is raised.
  */
  explicit plCndDeterministic(const plCndDistribution&);

  /** Get the external function used to construct this object. */
  plExternalFunction get_function() const;

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
