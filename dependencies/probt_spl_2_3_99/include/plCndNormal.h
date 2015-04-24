/*=============================================================================
 * Product        : ProBT 
 * File           : plCndNormal.h
 * Author         : Christophe Coue
 * Creation       : Jun 5 18:08:40 2001
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

#ifndef plCndNormal_h
#define plCndNormal_h

#include "plConfig.h"


#include <plCndDistribution.h>
#include <plDistribution.h>
#include <plArray.h>
#include <plExternalFunction.h>
#include <plFloatMatrix.h>
#include <plFloatVector.h>


/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/


/** This class implements conditional normal distributions on one or multiple
    dimensional spaces. A plCndNormal is a plNormal family for which mean (or
    mean vector) and/or standard deviation (or variance matrix) are not
    constant but are variables or user external functions.  When it is
    instantiated, a plNormal is obtained.
*/
class PL_DLL_API plCndNormal : public plCndDistribution
{

 public:

  
  /** Default constructor */
  plCndNormal();
  

  // ONE D CASES CONSTRUCTORS

  /** Constructs a one-dimensional conditional normal distribution on the \em left variable.
      \em mean is a know variable,
      \em std_dev is a fixed value corresponding to the standard deviation */
  plCndNormal(const plSymbol& left,
	      const plSymbol& mean,
	      plFloat std_dev);
  
  /** Constructs  a one-dimensional conditional normal distribution on the \em left variable.
      \em std_dev_var is a know variable
      \em mean is a fixed value corresponding to the mean value. */
  plCndNormal(const plSymbol& left,
	      plFloat mean,
	      const plSymbol& std_dev_var);
  
  /** Constructs a one-dimensional conditionnel normal distribution on the \em left variable.
      with \em mean = \em fmean(right) and 
      \em std_dev is a fixed value corresponding to the standard deviation.

      @anchor param_func_example_cnd_normal
      An example assuming that the mean value of the conditional normal P(X | Y Z) is the squared sum
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
      void sum2(plValues &mean, const plValues &Y_Z_vals)
      {
      const double s = Y_Z_vals[Y] + Y_Z_vals[Z];
      // mean is indexed by an integer 
      mean[0] = s*s;
      }
      
      plExternalFunction F_mean__YZ(Y^Z, &sum2);
      plCndNormal P_X__YZ(X, Y^Z, F_mean__YZ, 1);
      @endcode
     
      2) Using a member function:
      @code 
      struct YZSum
      {
      void sum2(plValues &mean, const plValues &Y_Z_vals) const
      {
      const double s = Y_Z_vals[Y] + Y_Z_vals[Z];
      // mean is indexed by an integer 
      mean[0] = s*s;
      }
     
      };
     
      YZSum *sum_obj_ptr = new YZSum;
      plExternalFunction  F_mean__YZ(Y^Z, sum_obj_ptr, &YZSum::sum2);
      plCndNormal P_X__YZ(X, Y^Z, F_mean__YZ, 1);
      @endcode
     
      3) Using a C source code function:
      @code 
      const std::string sum2_code = "output[0] = (Y+Z)*(Y+Z);";
      plExternalFunctionFromC  F_mean__YZ = createExternalFunctionFromC(Y^Z, sum2_code, "sum2_func");
      plCndNormal P_X__YZ(X, Y^Z, F_mean__YZ, 1);
      @endcode
  */
  plCndNormal(const plSymbol& left,
	      const plVariablesConjunction& right,
	      const plExternalFunction &fmean,
	      plFloat std_dev);
   
  /** Constructs a one-dimensional contitional normal distribution on the \em left variable
      with  \em std_dev = \em fsd(right) and 
      \em mean is a fixed value corresponding to the mean value.

      An example of how to create and use plExternalFunction as conditional distribution
      parameters can be found in @ref param_func_example_cnd_normal "'using external functions example'".
  */
  plCndNormal(const plSymbol& left,
	      const plVariablesConjunction& right,
	      plFloat mean,
	      const plExternalFunction &fsd);
  
  
  // N D CASES CONSTRUCTORS
  
  /** Constructs a multi dimensional contitional normal distribution on the continuous \em left
      variables. \em mean are known variables. \em var_matrix is a fixed
      covariance matrix. */
  plCndNormal(const plVariablesConjunction& left,
	      const plVariablesConjunction& mean,
	      const plFloatMatrix &var_matrix);
 
  /** Constructs a multi dimensional contitional normal distribution on the continuous\em left
      variables. \em variance_vars are known variables. \em mean is a fixed
      mean vector. */
  plCndNormal(const plVariablesConjunction& left,
	      const plFloatVector &mean,
	      const plVariablesConjunction& variance_vars);

  /** Constructs a multi dimensional contitional normal distribution on the continuous \em left variables
      with \em mean = \em fm(right) and 
      \em var_matrix is a fixed matrix corresponding to the variance.

      An example of how to create and use plExternalFunction as conditional distribution
      parameters can be found in @ref param_func_example_cnd_normal "'using external functions example'".
  */
  plCndNormal(const plVariablesConjunction& left,
	      const plVariablesConjunction& right,
	      const plExternalFunction &fm,
	      const plFloatMatrix &var_matrix);
    
  /** Constructs a multi dimensional contitional normal distribution on the continuous \em left variables
      with  \em variance = \em fvariance(right) and 
      \em mean is a fixed vector corresponding to the mean vector.

      An example of how to create and use plExternalFunction as conditional distribution
      parameters can be found in @ref param_func_example_cnd_normal "'using external functions example'".
  */
  plCndNormal(const plVariablesConjunction& left,
	      const plVariablesConjunction& right,
	      const plFloatVector &mean,
	      const plExternalFunction &fvariance);
  
  

  // MIXED CASES CONSTRUCTORS


  /** Constructs a contitional normal distribution on \em left variables (must be continuous when multi-dimensional).
      \em mean and \em variance_vars are known variables.

      ATTENTION: \em variance_vars are the variance variables (corresponding to
      the variance matrix elements) for nd cases and stantard deviation ones in
      1d cases.
  */
  plCndNormal(const plVariablesConjunction& left,
	      const plVariablesConjunction& mean,
	      const plVariablesConjunction& variance_vars);

  /** Constructs a contitional normal distribution on the \em left variables (must be continuous when multi-dimensional)
      with  \em variance = \em fvariance(right) and \em mean = \em fmean(right).

      ATTENTION: \em fvariance is the variance function (returning the variance
      matrix elements) for nd cases and simply the stantard deviation one in 1d
      ones.

      An example of how to create and use plExternalFunction as conditional distribution
      parameters can be found in @ref param_func_example_cnd_normal "'using external functions example'".
  */
  plCndNormal(const plVariablesConjunction& left,
	      const plVariablesConjunction& right,
	      const plExternalFunction &fmean,
	      const plExternalFunction &fvariance);

  /** Constructs a contitional normal distribution on the \em left variables (must be continuous when multi-dimensional)
      with  \em [mean,variance] = \em f_mean_variance(right).

      ATTENTION: the variance part of \em f_mean_variance is the variance function
      for nd cases and stantard deviation one in 1d ones.

      An example of how to create and use plExternalFunction as conditional distribution
      parameters can be found in @ref param_func_example_cnd_normal "'using external functions example'".
  */
  plCndNormal(const plVariablesConjunction& left,
	      const plVariablesConjunction& right,
	      const plExternalFunction &f_mean_variance);


  /** Destructor */
  virtual ~plCndNormal(){};

  /** Promote from a plCndDistribution. If the object being copied is
      actually of the wrong type, an exception is raised.
  */
  explicit plCndNormal(const plCndDistribution&);

  ///@name Parameters accesors.
  ///@{

  /** Returns true if the mean parameter is a constant. */
  bool mean_is_constant() const;
  /** Returns the value of the mean parameter. Do not call this method if
      mean_is_constant() returned false. */
  plFloatVector get_mean_constant() const;

  /** Returns true if the mean parameter is a variable. */
  bool mean_is_variable() const;
  /** Returns the mean variable. Do not call this method if
      mean_is_variable() returned false. */
  plVariablesConjunction get_mean_variable() const;

  /** Returns true if the mean parameter is a function. */
  bool mean_is_function() const;
  /** Returns the mean function. Do not call this method if
      mean_is_function() returned false. */
  plExternalFunction get_mean_function() const;

  /** Returns true if the variance (or stantard deviation in one-dimensional cases) parameter is a constant. */
  bool variance_is_constant() const;
  /** Returns the value of the variance (or stantard deviation in one-dimensional cases) parameter. Do not call this
      method if variance_is_constant() returned false. */
  plFloatMatrix get_variance_constant() const;

  /** Returns true if the variance (or stantard deviation in one-dimensional cases) parameter is a variable. */
  bool variance_is_variable() const;
  /** Returns the variance (or stantard deviation in one-dimensional cases) variable. Do not call this method if
      variance_is_variable() returned false. */
  plVariablesConjunction get_variance_variable() const;

  /** Returns true if the standard deviation parameter is a function. */
  bool variance_is_function() const;
  /** Returns the variance (or stantard deviation in one-dimensional cases) function. Do not call this method if
      variance_is_function() returned false. */
  plExternalFunction get_variance_function() const;

  /** Returns true if the mean and variance (or stantard deviation in one-dimensional cases) parameters were
      given as a single function. */
  bool mean_and_variance_are_one_function() const;
  /** Returns the parameters function. Do not call this method if
      mean_and_variance_are_one_function() returned false. */
  plExternalFunction get_mean_variance_function() const;

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
