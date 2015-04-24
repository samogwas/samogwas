/*=============================================================================
 * Product        : ProBT 
 * File           : plCndWeibull.h
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

#ifndef plCndWeibull_h
#define plCndWeibull_h

#include "plConfig.h"


#include <plCndDistribution.h>
#include <plSymbol.h>

class plExternalFunction;

/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/


/** This class implements conditional Weibull distributions. A plCndWeibull
    is a plWeibull family for which alpha and gamma are not constant but
    are variables or user external functions (mu = 0). When it is instantiated, a
    plWeibull is obtained.
*/
class PL_DLL_API plCndWeibull: public plCndDistribution
{
 public:

  /** Default constructor */
  plCndWeibull();
  
  /** Constructs a one-dimensional conditional Weibull distribution on the \em left variable.
      \em gamma is a known variable,
      \em alpha is a fixed value corresponding to the alpha parameter */
  plCndWeibull(const plSymbol& left,
               const plSymbol& gamma,
               plFloat alpha);
  
  /** Constructs  a one-dimensional conditional Weibull distribution on the \em left variable.
      \em gamma is a fixed value corresponding to the gamma value. 
      \em alpha is a known variable
*/
  plCndWeibull(const plSymbol& left,
               plFloat gamma,
               const plSymbol& alpha);
  
  /** Constructs a one-dimensional conditionnel Weibull distribution on the \em left variable.
      with \em gamma = \em fgamma(right) and 
      \em alpha is a fixed value corresponding to the alpha parameter.

      @anchor param_func_example_cnd_weibull
      An example assuming that the gamma value of the conditional Weibull P(X | Y Z) is the squared sum
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
      void sum2(plValues &gamma, const plValues &Y_Z_vals)
      {
      const double s = Y_Z_vals[Y] + Y_Z_vals[Z];
      // gamma is indexed by an integer 
      gamma[0] = s*s;
      }
      
      plExternalFunction F_gamma__YZ(Y^Z, &sum2);
      plCndWeibull P_X__YZ(X, Y^Z, F_gamma__YZ, 2);
      @endcode
     
      2) Using a member function:
      @code 
      struct YZSum
      {
      void sum2(plValues &gamma, const plValues &Y_Z_vals) const
      {
      const double s = Y_Z_vals[Y] + Y_Z_vals[Z];
      // gamma is indexed by an integer 
      gamma[0] = s*s;
      }
     
      };
     
      YZSum *sum_obj_ptr = new YZSum;
      plExternalFunction  F_gamma__YZ(Y^Z, sum_obj_ptr, &YZSum::sum2);
      plCndWeibull P_X__YZ(X, Y^Z, F_gamma__YZ, 2);
      @endcode
     
      3) Using a C source code function:
      @code 
      const std::string sum2_code = "output[0] = (Y+Z)*(Y+Z);";
      plExternalFunctionFromC  F_gamma__YZ = createExternalFunctionFromC(Y^Z, sum2_code, "sum2_func");
      plCndWeibull P_X__YZ(X, Y^Z, F_gamma__YZ, 2);
      @endcode
  */
  plCndWeibull(const plSymbol& left,
               const plVariablesConjunction& right,
               const plExternalFunction &fgamma,
               plFloat alpha);
   
  /** Constructs a one-dimensional contitional Weibull distribution on the \em left variable
      with  \em alpha = \em falpha(right) and 
      \em gamma is a fixed value corresponding to the gamma value.

      An example of how to create and use plExternalFunction as conditional distribution
      parameters can be found in @ref param_func_example_cnd_weibull "'using external functions example'".
  */
  plCndWeibull(const plSymbol& left,
               const plVariablesConjunction& right,
               plFloat gamma,
               const plExternalFunction &falpha);

  /** Constructs a contitional Weibull distribution on \em left variables.
      \em gamma and \em alpha are known variables.
  */
  plCndWeibull(const plSymbol& left,
               const plSymbol& gamma,
               const plSymbol& alpha);

  /** Constructs a contitional Weibull distribution on the \em left variables
      with  \em alpha = \em falpha(right) and \em gamma = \em fgamma(right).

      An example of how to create and use plExternalFunction as conditional distribution
      parameters can be found in @ref param_func_example_cnd_weibull "'using external functions example'".
  */
  plCndWeibull(const plSymbol& left,
               const plVariablesConjunction& right,
               const plExternalFunction &fgamma,
               const plExternalFunction &falpha);

  /** Constructs a contitional Weibull distribution on the \em left variables
      with  \em [gamma,alpha] = \em f_gamma_alpha(right).

      An example of how to create and use plExternalFunction as conditional distribution
      parameters can be found in @ref param_func_example_cnd_weibull "'using external functions example'".
  */
  plCndWeibull(const plSymbol& left,
               const plVariablesConjunction& right,
               const plExternalFunction &f_gamma_alpha);

  /** Destructor */
  virtual ~plCndWeibull(){};

  /** Promote from a plCndDistribution. If the object being copied is
      actually of the wrong type, an exception is raised.
  */
  explicit plCndWeibull(const plCndDistribution&);

  

  ///@name Parameters accesors.
  ///@{

  /** Returns true if the gamma parameter is a constant. */
  bool gamma_is_constant() const;
  /** Returns the value of the gamma parameter. Do not call this method if
      gamma_is_constant() returned false. */
  plFloat get_gamma_constant() const;

  /** Returns true if the gamma parameter is a variable. */
  bool gamma_is_variable() const;
  /** Returns the gamma variable. Do not call this method if
      gamma_is_variable() returned false. */
  plSymbol get_gamma_variable() const;

  /** Returns true if the gamma parameter is a function. */
  bool gamma_is_function() const;
  /** Returns the gamma function. Do not call this method if
      gamma_is_function() returned false. */
  plExternalFunction get_gamma_function() const;

  /** Returns true if the alpha  parameter is a constant. */
  bool alpha_is_constant() const;
  /** Returns the value of the alpha  parameter. Do not call this
      method if alpha_is_constant() returned false. */
  plFloat get_alpha_constant() const;

  /** Returns true if the alpha  parameter is a variable. */
  bool alpha_is_variable() const;
  /** Returns the alpha  variable. Do not call this method if
      alpha_is_variable() returned false. */
  plSymbol get_alpha_variable() const;

  /** Returns true if the alpha parameter is a function. */
  bool alpha_is_function() const;
  /** Returns the alpha  function. Do not call this method if
      alpha_is_function() returned false. */
  plExternalFunction get_alpha_function() const;

  /** Returns true if the gamma and alpha  parameters were
      given as a single function. */
  bool gamma_and_alpha_are_one_function() const;
  /** Returns the parameters function. Do not call this method if
      gamma_and_alpha_are_one_function() returned false. */
  plExternalFunction get_gamma_alpha_function() const;

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
