/*=============================================================================
 * Product        : ProBT 
 * File           : plCndLogNormal.h
 * Author         : Emmanuel Mazer
 * Creation       : March 25 2005
 *
 *=============================================================================
 *     (c) Copyright 2005, Probayes
 *=============================================================================
 *
 *------------------------- Description ---------------------------------------
 *
 *   
 *-----------------------------------------------------------------------------
 */

#ifndef plCndLogNormal_h
#define plCndLogNormal_h

#include "plConfig.h"


#include <plCndDistribution.h>
#include <plSymbol.h>

class plExternalFunction;

/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/


/** This class implements conditional lognormal distributions. A plCndLogNormal
    is a plLogNormal family for which m and sigma are not constant but
    are variables or user external functions (theta = 0). When it is instantiated, a
    plLogNormal is obtained.
*/
class PL_DLL_API plCndLogNormal: public plCndDistribution
{
 public:

  /** Default contructor */
  plCndLogNormal();
  
  /** Constructs a one-dimensional conditional log-normal distribution on the \em left variable.
      \em sigma is a know variable
      \em m is a fixed value corresponding to the m parameter 
 */
  plCndLogNormal(const plSymbol& left,
                 const plSymbol& sigma,
                 plFloat m);
  
  /** Constructs  a one-dimensional conditional log-normal distribution on the \em left variable.
      \em sigma is a fixed value corresponding to the sigma value.
      \em m is a know variable
  */
  plCndLogNormal(const plSymbol& left,
                 plFloat sigma,
                 const plSymbol& m);
  
  /** Constructs a one-dimensional conditionnel log-normal distribution on the \em left variable.
      with \em sigma = \em fsigma(right) and 
      \em m is a fixed value corresponding to the m parameter.

      @anchor param_func_example_cnd_lognormal
      An example assuming that the sigma value of the conditional log-normal P(X | Y Z) is the squared sum
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
      void sum2(plValues &sigma, const plValues &Y_Z_vals)
      {
      const double s = Y_Z_vals[Y] + Y_Z_vals[Z];
      // sigma is indexed by an integer 
      sigma[0] = s*s;
      }
      
      plExternalFunction F_sigma__YZ(Y^Z, &sum2);
      plCndLogNormal P_X__YZ(X, Y^Z, F_sigma__YZ, 1);
      @endcode
     
      2) Using a member function:
      @code 
      struct YZSum
      {
      void sum2(plValues &sigma, const plValues &Y_Z_vals) const
      {
      const double s = Y_Z_vals[Y] + Y_Z_vals[Z];
      // sigma is indexed by an integer 
      sigma[0] = s*s;
      }
     
      };
     
      YZSum *sum_obj_ptr = new YZSum;
      plExternalFunction  F_sigma__YZ(Y^Z, sum_obj_ptr, &YZSum::sum2);
      plCndLogNormal P_X__YZ(X, Y^Z, F_sigma__YZ, 1);
      @endcode
     
      3) Using a C source code function:
      @code 
      const std::string sum2_code = "output[0] = (Y+Z)*(Y+Z);";
      plExternalFunctionFromC  F_sigma__YZ = createExternalFunctionFromC(Y^Z, sum2_code, "sum2_func");
      plCndLogNormal P_X__YZ(X, Y^Z, F_sigma__YZ, 1);
      @endcode
  */
  plCndLogNormal(const plSymbol& left,
                 const plVariablesConjunction& right,
                 const plExternalFunction &fsigma,
                 plFloat m);
   
  /** Constructs a one-dimensional contitional log-normal distribution on the \em left variable
      with  \em m = \em fm(right) and 
      \em sigma is a fixed value corresponding to the sigma value.

      An example of how to create and use plExternalFunction as conditional distribution
      parameters can be found in @ref param_func_example_cnd_lognormal "'using external functions example'".
  */
  plCndLogNormal(const plSymbol& left,
                 const plVariablesConjunction& right,
                 plFloat sigma,
                 const plExternalFunction &fm);

  /** Constructs a contitional log-normal distribution on \em left variables.
      \em sigma and \em m are known variables.
  */
  plCndLogNormal(const plSymbol& left,
                 const plSymbol& sigma,
                 const plSymbol& m);

  /** Constructs a contitional log-normal distribution on the \em left variables
      with  \em m = \em fm(right) and \em sigma = \em fsigma(right).

      An example of how to create and use plExternalFunction as conditional distribution
      parameters can be found in @ref param_func_example_cnd_lognormal "'using external functions example'".
  */
  plCndLogNormal(const plSymbol& left,
                 const plVariablesConjunction& right,
                 const plExternalFunction &fsigma,
                 const plExternalFunction &fm);

  /** Constructs a contitional log-normal distribution on the \em left variables
      with  \em [sigma,m] = \em f_sigma_m(right).

      An example of how to create and use plExternalFunction as conditional distribution
      parameters can be found in @ref param_func_example_cnd_lognormal "'using external functions example'".
  */
  plCndLogNormal(const plSymbol& left,
                 const plVariablesConjunction& right,
                 const plExternalFunction &f_sigma_m);

  /** Destructor */
  virtual ~plCndLogNormal(){};

  /** Promote from a plCndDistribution. If the object being copied is
      actually of the wrong type, an exception is raised.
  */
  explicit plCndLogNormal(const plCndDistribution&);

  

  ///@name Parameters accesors.
  ///@{

  /** Returns true if the sigma parameter is a constant. */
  bool sigma_is_constant() const;
  /** Returns the value of the sigma parameter. Do not call this method if
      sigma_is_constant() returned false. */
  plFloat get_sigma_constant() const;

  /** Returns true if the sigma parameter is a variable. */
  bool sigma_is_variable() const;
  /** Returns the sigma variable. Do not call this method if
      sigma_is_variable() returned false. */
  plSymbol get_sigma_variable() const;

  /** Returns true if the sigma parameter is a function. */
  bool sigma_is_function() const;
  /** Returns the sigma function. Do not call this method if
      sigma_is_function() returned false. */
  plExternalFunction get_sigma_function() const;

  /** Returns true if the m  parameter is a constant. */
  bool m_is_constant() const;
  /** Returns the value of the m  parameter. Do not call this
      method if m_is_constant() returned false. */
  plFloat get_m_constant() const;

  /** Returns true if the m  parameter is a variable. */
  bool m_is_variable() const;
  /** Returns the m  variable. Do not call this method if
      m_is_variable() returned false. */
  plSymbol get_m_variable() const;

  /** Returns true if the m parameter is a function. */
  bool m_is_function() const;
  /** Returns the m  function. Do not call this method if
      m_is_function() returned false. */
  plExternalFunction get_m_function() const;

  /** Returns true if the sigma and m  parameters were
      given as a single function. */
  bool sigma_and_m_are_one_function() const;
  /** Returns the parameters function. Do not call this method if
      sigma_and_m_are_one_function() returned false. */
  plExternalFunction get_sigma_m_function() const;

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
