/*=============================================================================
 * Product        : ProBT 
 * File           : plCndGamma.h
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

#ifndef plCndGamma_h
#define plCndGamma_h

#include "plConfig.h"


#include <plCndDistribution.h>
#include <plSymbol.h>

class plExternalFunction;

/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/


/** This class implements conditional Gamma distributions. A plCndGamma
    is a plGamma family for which alpha and theta are not constant but
    are variables or user external functions (mu = 0). When it is instantiated, a
    plGamma is obtained.
*/
class PL_DLL_API plCndGamma: public plCndDistribution
{
 public:

  /** Default contructor */
  plCndGamma();
  
  /** Constructs a one-dimensional conditional Gamma distribution on the \em left variable.
      \em alpha is a know variable,
      \em theta is a fixed value corresponding to the m parameter */
  plCndGamma(const plSymbol& left,
             const plSymbol& alpha,
             plFloat theta);
  
  /** Constructs  a one-dimensional conditional Gamma distribution on the \em left variable.
      \em alpha is a fixed value corresponding to the alpha value.
      \em theta is a know variable 
  */
  plCndGamma(const plSymbol& left,
             plFloat alpha,
             const plSymbol& theta);
  
  /** Constructs a one-dimensional conditionnel Gamma distribution on the \em left variable.
      with \em alpha = \em falpha(right) and 
      \em theta is a fixed value corresponding to the m parameter.

      @anchor param_func_example_cnd_gamma
      An example assuming that the alpha value of the conditional Gamma P(X | Y Z) is the squared sum
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
      void sum2(plValues &alpha, const plValues &Y_Z_vals)
      {
      const double s = Y_Z_vals[Y] + Y_Z_vals[Z];
      // alpha is indexed by an integer 
      alpha[0] = s*s;
      }
      
      plExternalFunction F_alpha__YZ(Y^Z, &sum2);
      plCndGamma P_X__YZ(X, Y^Z, F_alpha__YZ, 2);
      @endcode
     
      2) Using a member function:
      @code 
      struct YZSum
      {
      void sum2(plValues &alpha, const plValues &Y_Z_vals) const
      {
      const double s = Y_Z_vals[Y] + Y_Z_vals[Z];
      // alpha is indexed by an integer 
      alpha[0] = s*s;
      }
     
      };
     
      YZSum *sum_obj_ptr = new YZSum;
      plExternalFunction  F_alpha__YZ(Y^Z, sum_obj_ptr, &YZSum::sum2);
      plCndGamma P_X__YZ(X, Y^Z, F_alpha__YZ, 2);
      @endcode
     
      3) Using a C source code function:
      @code 
      const std::string sum2_code = "output[0] = (Y+Z)*(Y+Z);";
      plExternalFunctionFromC  F_alpha__YZ = createExternalFunctionFromC(Y^Z, sum2_code, "sum2_func");
      plCndGamma P_X__YZ(X, Y^Z, F_alpha__YZ, 2);
      @endcode
  */
  plCndGamma(const plSymbol& left,
             const plVariablesConjunction& right,
             const plExternalFunction &falpha,
             plFloat theta);
   
  /** Constructs a one-dimensional contitional Gamma distribution on the \em left variable
      with  \em theta = \em ftheta(right) and 
      \em alpha is a fixed value corresponding to the alpha value.

      An example of how to create and use plExternalFunction as conditional distribution
      parameters can be found in @ref param_func_example_cnd_gamma "'using external functions example'".
  */
  plCndGamma(const plSymbol& left,
             const plVariablesConjunction& right,
             plFloat alpha,
             const plExternalFunction &ftheta);

  /** Constructs a contitional Gamma distribution on \em left variables.
      \em alpha and \em theta are known variables.
  */
  plCndGamma(const plSymbol& left,
             const plSymbol& alpha,
             const plSymbol& theta);

  /** Constructs a contitional Gamma distribution on the \em left variables
      with  \em theta = \em ftheta(right) and \em alpha = \em falpha(right).

      An example of how to create and use plExternalFunction as conditional distribution
      parameters can be found in @ref param_func_example_cnd_gamma "'using external functions example'".
  */
  plCndGamma(const plSymbol& left,
             const plVariablesConjunction& right,
             const plExternalFunction &falpha,
             const plExternalFunction &ftheta);

  /** Constructs a contitional Gamma distribution on the \em left variables
      with  \em [alpha,theta] = \em f_alpha_theta(right).

      An example of how to create and use plExternalFunction as conditional distribution
      parameters can be found in @ref param_func_example_cnd_gamma "'using external functions example'".
  */
  plCndGamma(const plSymbol& left,
             const plVariablesConjunction& right,
             const plExternalFunction &f_alpha_theta);

  /** Destructor */
  virtual ~plCndGamma(){};

  /** Promote from a plCndDistribution. If the object being copied is
      actually of the wrong type, an exception is raised.
  */
  explicit plCndGamma(const plCndDistribution&);

  

  ///@name Parameters accesors.
  ///@{

  /** Returns true if the alpha parameter is a constant. */
  bool alpha_is_constant() const;
  /** Returns the value of the alpha parameter. Do not call this method if
      alpha_is_constant() returned false. */
  plFloat get_alpha_constant() const;

  /** Returns true if the alpha parameter is a variable. */
  bool alpha_is_variable() const;
  /** Returns the alpha variable. Do not call this method if
      alpha_is_variable() returned false. */
  plSymbol get_alpha_variable() const;

  /** Returns true if the alpha parameter is a function. */
  bool alpha_is_function() const;
  /** Returns the alpha function. Do not call this method if
      alpha_is_function() returned false. */
  plExternalFunction get_alpha_function() const;

  /** Returns true if the theta  parameter is a constant. */
  bool theta_is_constant() const;
  /** Returns the value of the theta  parameter. Do not call this
      method if theta_is_constant() returned false. */
  plFloat get_theta_constant() const;

  /** Returns true if the theta  parameter is a variable. */
  bool theta_is_variable() const;
  /** Returns the theta  variable. Do not call this method if
      theta_is_variable() returned false. */
  plSymbol get_theta_variable() const;

  /** Returns true if the theta parameter is a function. */
  bool theta_is_function() const;
  /** Returns the theta  function. Do not call this method if
      theta_is_function() returned false. */
  plExternalFunction get_theta_function() const;

  /** Returns true if the alpha and theta  parameters were
      given as a single function. */
  bool alpha_and_theta_are_one_function() const;
  /** Returns the parameters function. Do not call this method if
      alpha_and_theta_are_one_function() returned false. */
  plExternalFunction get_alpha_theta_function() const;

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
