/*=============================================================================
 * Product        : ProBT 
 * File           : plCndExponential.h
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

#ifndef plCndExponential_h
#define plCndExponential_h

#include "plConfig.h"


#include <plCndDistribution.h>
#include <plSymbol.h>

class plExternalFunction;

/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/


/** This class implements conditional Exponential distributions. A plCndExponential
    is a plExponential family for which mu and beta are not constant but
    are variables or user external functions. When it is instantiated, a
    plExponential is obtained.
*/
class PL_DLL_API plCndExponential: public plCndDistribution
{
 public:

  /** Default contructor */
  plCndExponential();
  
  /** Constructs a one-dimensional conditional Exponential distribution on the \em left variable.
      \em beta is a know variable,
      \em mu is a fixed value corresponding to the m parameter */
  plCndExponential(const plSymbol& left,
                   const plSymbol& beta,
                   plFloat mu);
  
  /** Constructs  a one-dimensional conditional Exponential distribution on the \em left variable.
      \em beta is a fixed value corresponding to the beta value.
      \em mu is a know variable
  */
  plCndExponential(const plSymbol& left,
                   plFloat beta,
                   const plSymbol& mu);
  
  /** Constructs a one-dimensional conditionnel Exponential distribution on the \em left variable.
      with \em beta = \em fbeta(right) and 
      \em mu is a fixed value corresponding to the m parameter.

      @anchor param_func_example_cnd_exp
      An example assuming that the beta value of the conditional Exponential P(X | Y Z) is the squared sum
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
      void sum2(plValues &beta, const plValues &Y_Z_vals)
      {
      const double s = Y_Z_vals[Y] + Y_Z_vals[Z];
      // beta is indexed by an integer 
      beta[0] = s*s;
      }
      
      plExternalFunction F_beta__YZ(Y^Z, &sum2);
      plCndExponential P_X__YZ(X, Y^Z, F_beta__YZ, 1);
      @endcode
     
      2) Using a member function:
      @code 
      struct YZSum
      {
      void sum2(plValues &beta, const plValues &Y_Z_vals) const
      {
      const double s = Y_Z_vals[Y] + Y_Z_vals[Z];
      // beta is indexed by an integer 
      beta[0] = s*s;
      }
     
      };
     
      YZSum *sum_obj_ptr = new YZSum;
      plExternalFunction  F_beta__YZ(Y^Z, sum_obj_ptr, &YZSum::sum2);
      plCndExponential P_X__YZ(X, Y^Z, F_beta__YZ, 1);
      @endcode
     
      3) Using a C source code function:
      @code 
      const std::string sum2_code = "output[0] = (Y+Z)*(Y+Z);";
      plExternalFunctionFromC  F_beta__YZ = createExternalFunctionFromC(Y^Z, sum2_code, "sum2_func");
      plCndExponential P_X__YZ(X, Y^Z, F_beta__YZ);
      @endcode
  */
  plCndExponential(const plSymbol& left,
                   const plVariablesConjunction& right,
                   const plExternalFunction &fbeta,
                   plFloat mu);
   
  /** Constructs a one-dimensional contitional Exponential distribution on the \em left variable
      with  \em mu = \em fmu(right) and 
      \em beta is a fixed value corresponding to the beta value.

      An example of how to create and use plExternalFunction as conditional distribution
      parameters can be found in @ref param_func_example_cnd_exp "'using external functions example'".
  */
  plCndExponential(const plSymbol& left,
                   const plVariablesConjunction& right,
                   plFloat beta,
                   const plExternalFunction &fmu);

  /** Constructs a contitional Exponential distribution on \em left variables.
      \em beta and \em mu are known variables.
  */
  plCndExponential(const plSymbol& left,
                   const plSymbol& beta,
                   const plSymbol& mu);

  /** Constructs a contitional Exponential distribution on the \em left variables
      with  \em mu = \em fmu(right) and \em beta = \em fbeta(right).

      An example of how to create and use plExternalFunction as conditional distribution
      parameters can be found in @ref param_func_example_cnd_exp "'using external functions example'".
  */
  plCndExponential(const plSymbol& left,
                   const plVariablesConjunction& right,
                   const plExternalFunction &fbeta,
                   const plExternalFunction &fmu);

  /** Constructs a contitional Exponential distribution on the \em left variables
      with  \em [beta,mu] = \em f_beta_mu(right).

      An example of how to create and use plExternalFunction as conditional distribution
      parameters can be found in @ref param_func_example_cnd_exp "'using external functions example'".
  */
  plCndExponential(const plSymbol& left,
                   const plVariablesConjunction& right,
                   const plExternalFunction &f_beta_mu);

  /** Destructor */
  virtual ~plCndExponential(){};

  /** Promote from a plCndDistribution. If the object being copied is
      actually of the wrong type, an exception is raised.
  */
  explicit plCndExponential(const plCndDistribution&);

  

  ///@name Parameters accesors.
  ///@{

  /** Returns true if the beta parameter is a constant. */
  bool beta_is_constant() const;
  /** Returns the value of the beta parameter. Do not call this method if
      beta_is_constant() returned false. */
  plFloat get_beta_constant() const;

  /** Returns true if the beta parameter is a variable. */
  bool beta_is_variable() const;
  /** Returns the beta variable. Do not call this method if
      beta_is_variable() returned false. */
  plSymbol get_beta_variable() const;

  /** Returns true if the beta parameter is a function. */
  bool beta_is_function() const;
  /** Returns the beta function. Do not call this method if
      beta_is_function() returned false. */
  plExternalFunction get_beta_function() const;

  /** Returns true if the mu  parameter is a constant. */
  bool mu_is_constant() const;
  /** Returns the value of the mu  parameter. Do not call this
      method if mu_is_constant() returned false. */
  plFloat get_mu_constant() const;

  /** Returns true if the mu  parameter is a variable. */
  bool mu_is_variable() const;
  /** Returns the mu  variable. Do not call this method if
      mu_is_variable() returned false. */
  plSymbol get_mu_variable() const;

  /** Returns true if the mu parameter is a function. */
  bool mu_is_function() const;
  /** Returns the mu  function. Do not call this method if
      mu_is_function() returned false. */
  plExternalFunction get_mu_function() const;

  /** Returns true if the beta and mu  parameters were
      given as a single function. */
  bool beta_and_mu_are_one_function() const;
  /** Returns the parameters function. Do not call this method if
      beta_and_mu_are_one_function() returned false. */
  plExternalFunction get_beta_mu_function() const;

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
