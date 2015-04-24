/*=============================================================================
 * Product        : ProBT 
 * File           : plCndUniform.h
 * Author         : David Raulo
 * Creation       : Sun Sep 16 15:42:12 2007
 *
 *=============================================================================
 *        (c) Copyright 2007, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */

#ifndef PL_CND_UNIFORM_H
#define PL_CND_UNIFORM_H

#include "plConfig.h"
#include <plCndDistribution.h>
#include <plExternalFunction.h>
#include <plSymbol.h>


/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/

/** This class implements conditional uniform distributions on one dimensional
    space. A plCndUniform is a plUniform family for which the lower and/or
    upper bounds are not constant but are variables or user external
    functions. When it is instantiated, a plUniform is obtained.
*/
class PL_DLL_API plCndUniform: public plCndDistribution
{

 public:

  /** Default contructor */
  plCndUniform();
  
  /** Constructs a one-dimensional conditional uniform distribution on the \em variable variable.
      \em min is a know variable,
      \em max is a fixed value corresponding to the upper bound */
  plCndUniform(const plSymbol& variable,
               const plSymbol& min,
               plFloat max);
  
  /** Constructs  a one-dimensional conditional uniform distribution on the \em variable variable.
      \em min is a fixed value corresponding to the lower bound
      \em max is a know variable corresponding to the upper bound
 */
  plCndUniform(const plSymbol& variable,
               plFloat min,
               const plSymbol& max);
  
  /** Constructs a one-dimensional conditionnel uniform distribution on the \em variable variable.
      with \em min = \em fmin(right) and 
      \em max is a fixed value corresponding to the max upper bound.

      @anchor param_func_example_cnd_uniform
      An example assuming that the min value of the conditional uniform P(X | Y Z) is the squared sum
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
      void sum2(plValues &min, const plValues &Y_Z_vals)
      {
      const double s = Y_Z_vals[Y] + Y_Z_vals[Z];
      // min is indexed by an integer 
      min[0] = s*s;
      }
      
      plExternalFunction F_min__YZ(Y^Z, &sum2);
      plCndUniform P_X__YZ(X, Y^Z, F_min__YZ, 100);
      @endcode
     
      2) Using a member function:
      @code 
      struct YZSum
      {
      void sum2(plValues &min, const plValues &Y_Z_vals) const
      {
      const double s = Y_Z_vals[Y] + Y_Z_vals[Z];
      // min is indexed by an integer 
      min[0] = s*s;
      }
     
      };
     
      YZSum *sum_obj_ptr = new YZSum;
      plExternalFunction  F_min__YZ(Y^Z, sum_obj_ptr, &YZSum::sum2);
      plCndUniform P_X__YZ(X, Y^Z, F_min__YZ, 100);
      @endcode
     
      3) Using a C source code function:
      @code 
      const std::string sum2_code = "output[0] = (Y+Z)*(Y+Z);";
      plExternalFunctionFromC  F_min__YZ = createExternalFunctionFromC(Y^Z, sum2_code, "sum2_func");
      plCndUniform P_X__YZ(X, Y^Z, F_min__YZ, 100);
      @endcode
  */
  plCndUniform(const plSymbol& variable,
               const plVariablesConjunction& right,
               const plExternalFunction &fmin,
               plFloat max);
   
  /** Constructs a one-dimensional contitional uniform distribution on the \em variable variable
      with  \em max = \em fmax(right) and 
      \em min is a fixed value corresponding to the min value.

      An example of how to create and use plExternalFunction as conditional distribution
      parameters can be found in @ref param_func_example_cnd_uniform "'using external functions example'".
  */
  plCndUniform(const plSymbol& variable,
               const plVariablesConjunction& right,
               plFloat min,
               const plExternalFunction &fmax);

  /** Constructs a contitional uniform distribution on \em variable variables.
      \em min and \em max are known variables.
  */
  plCndUniform(const plSymbol& variable,
               const plSymbol& min,
               const plSymbol& max);

  /** Constructs a contitional uniform distribution on the \em variable variables
      with  \em max = \em fmax(right) and \em min = \em fmin(right).

      An example of how to create and use plExternalFunction as conditional distribution
      parameters can be found in @ref param_func_example_cnd_uniform "'using external functions example'".
  */
  plCndUniform(const plSymbol& variable,
               const plVariablesConjunction& right,
               const plExternalFunction &fmin,
               const plExternalFunction &fmax);

  /** Constructs a contitional uniform distribution on the \em variable variables
      with  \em [min,max] = \em f_min_max(right).

      An example of how to create and use plExternalFunction as conditional distribution
      parameters can be found in @ref param_func_example_cnd_uniform "'using external functions example'".
  */
  plCndUniform(const plSymbol& variable,
               const plVariablesConjunction& right,
               const plExternalFunction &f_min_max);

  /** Destructor */
  virtual ~plCndUniform(){};

  /** Promote from a plCndDistribution. If the object being copied is
      actually of the wrong type, an exception is raised.
  */
  explicit plCndUniform(const plCndDistribution&);

  

  ///@name Parameters accesors.
  ///@{

  /** Returns true if the min parameter is a constant. */
  bool min_is_constant() const;
  /** Returns the value of the min parameter. Do not call this method if
      min_is_constant() returned false. */
  plFloat get_min_constant() const;

  /** Returns true if the min parameter is a variable. */
  bool min_is_variable() const;
  /** Returns the min variable. Do not call this method if
      min_is_variable() returned false. */
  plSymbol get_min_variable() const;

  /** Returns true if the min parameter is a function. */
  bool min_is_function() const;
  /** Returns the min function. Do not call this method if
      min_is_function() returned false. */
  plExternalFunction get_min_function() const;

  /** Returns true if the max  parameter is a constant. */
  bool max_is_constant() const;
  /** Returns the value of the max  parameter. Do not call this
      method if max_is_constant() returned false. */
  plFloat get_max_constant() const;

  /** Returns true if the max  parameter is a variable. */
  bool max_is_variable() const;
  /** Returns the max  variable. Do not call this method if
      max_is_variable() returned false. */
  plSymbol get_max_variable() const;

  /** Returns true if the max parameter is a function. */
  bool max_is_function() const;
  /** Returns the max  function. Do not call this method if
      max_is_function() returned false. */
  plExternalFunction get_max_function() const;

  /** Returns true if the min and max  parameters were
      given as a single function. */
  bool min_and_max_are_one_function() const;
  /** Returns the parameters function. Do not call this method if
      min_and_max_are_one_function() returned false. */
  plExternalFunction get_min_max_function() const;

  ///@}

  virtual bool is_base_class()const
  {
    return false;
  }
};

#endif //PL_CND_UNIFORM_H

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
