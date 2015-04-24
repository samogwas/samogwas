/*=============================================================================
 * Product        : ProBT 
 * File           : plCndVonMises.h
 * Author         : Kamel Mekhnacha
 * Creation       : February 2014
 *
 *=============================================================================
 *     (c) Copyright 2014, Probayes SAS
 *=============================================================================
 *
 *------------------------- Description ---------------------------------------
 *   
 *   
 *-----------------------------------------------------------------------------
 */

#ifndef plCndVonMises_h
#define plCndVonMises_h

#include "plConfig.h"


#include <plCndDistribution.h>
#include <plSymbol.h>

class plExternalFunction;

/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/


/** This class implements conditional Von Mises distributions. A plCndVonMises is a
    plVonMises family for which 'mean' and 'k' are not constant but are variables or user
    external functions. When it is instantiated, a
    plVonMises is obtained.  */
class PL_DLL_API plCndVonMises: public plCndDistribution
{
 public:

  /** Default contructor */
  plCndVonMises();
  
  /** Constructs a conditional Von Mises distribution on the \em left variable.
      \em mean is a know variable,
      \em k is a fixed value corresponding to the k parameter */
  plCndVonMises(const plSymbol& left,
                const plSymbol& mean,
                plFloat k);
  
  /** Constructs a conditional Von Mises distribution on the \em left variable.
      \em mean is a fixed value corresponding to the mean parameter,
      \em k is a know variable
  */
  plCndVonMises(const plSymbol& left,
                plFloat mean,
                const plSymbol& k);
  
  /** Constructs a conditionnel Von Mises distribution on the \em left variable.
      with \em mean = \em fmean(right) and 
      \em k is a fixed value corresponding to the k parameter.

      @anchor param_func_example_cnd_vonvises
      An example assuming that the mean value of the conditional Von Mises P(X | Y Z) is the squared sum
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
      plCndVonMises P_X__YZ(X, Y^Z, F_mean__YZ, 1);
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
      plCndVonMises P_X__YZ(X, Y^Z, F_mean__YZ, 1);
      @endcode
     
      3) Using a C source code function:
      @code 
      const std::string sum2_code = "output[0] = (Y+Z)*(Y+Z);";
      plExternalFunctionFromC  F_mean__YZ = createExternalFunctionFromC(Y^Z, sum2_code, "sum2_func");
      plCndVonMises P_X__YZ(X, Y^Z, F_mean__YZ, 1);
      @endcode
  */
  plCndVonMises(const plSymbol& left,
                const plVariablesConjunction& right,
                const plExternalFunction &fmean,
                plFloat k);
   
  /** Constructs a one-dimensional contitional Von Mises distribution on the \em left variable
      with  \em k = \em fk(right) and 
      \em mean is a fixed value corresponding to the mean value.

      An example of how to create and use plExternalFunction as conditional distribution
      parameters can be found in @ref param_func_example_cnd_vonvises "'using external functions example'".
  */
  plCndVonMises(const plSymbol& left,
                const plVariablesConjunction& right,
                plFloat mean,
                const plExternalFunction &fk);

  /** Constructs a contitional Von Mises distribution on \em left variables.
      \em mean and \em k are known variables.
  */
  plCndVonMises(const plSymbol& left,
                const plSymbol& mean,
                const plSymbol& k);

  /** Constructs a contitional Von Mises distribution on the \em left variables
      with  \em mean = \em fmean(right) and \em k = \em fk(right).

      An example of how to create and use plExternalFunction as conditional distribution
      parameters can be found in @ref param_func_example_cnd_vonvises "'using external functions example'".
  */
  plCndVonMises(const plSymbol& left,
                const plVariablesConjunction& right,
                const plExternalFunction &fmean,
                const plExternalFunction &fk);

  /** Constructs a contitional Von Mises distribution on the \em left variables
      with  \em [mean,k] = \em f_mean_k(right).

      An example of how to create and use plExternalFunction as conditional distribution
      parameters can be found in @ref param_func_example_cnd_vonvises "'using external functions example'".
  */
  plCndVonMises(const plSymbol& left,
                const plVariablesConjunction& right,
                const plExternalFunction &f_mean_k);

  /** Destructor */
  virtual ~plCndVonMises(){};

  /** Promote from a plCndDistribution. If the object being copied is
      actually of the wrong type, an exception is raised.
  */
  explicit plCndVonMises(const plCndDistribution&);

  

  ///@name Parameters accesors.
  ///@{

  /** Returns true if the mean parameter is a constant. */
  bool mean_is_constant() const;
  /** Returns the value of the mean parameter. Do not call this method if
      mean_is_constant() returned false. */
  plFloat get_mean_constant() const;

  /** Returns true if the mean parameter is a variable. */
  bool mean_is_variable() const;
  /** Returns the mean variable. Do not call this method if
      mean_is_variable() returned false. */
  plSymbol get_mean_variable() const;

  /** Returns true if the mean parameter is a function. */
  bool mean_is_function() const;
  /** Returns the mean function. Do not call this method if
      mean_is_function() returned false. */
  plExternalFunction get_mean_function() const;

  /** Returns true if the k  parameter is a constant. */
  bool k_is_constant() const;
  /** Returns the value of the k parameter. Do not call this
      method if k_is_constant() returned false. */
  plFloat get_k_constant() const;

  /** Returns true if the k parameter is a variable. */
  bool k_is_variable() const;
  /** Returns the k variable. Do not call this method if
      k_is_variable() returned false. */
  plSymbol get_k_variable() const;

  /** Returns true if the k parameter is a function. */
  bool k_is_function() const;
  /** Returns the k function. Do not call this method if
      k_is_function() returned false. */
  plExternalFunction get_k_function() const;

  /** Returns true if the mean and k  parameters were
      given as a single function. */
  bool mean_and_k_are_one_function() const;
  /** Returns the parameters function. Do not call this method if
      mean_and_k_are_one_function() returned false. */
  plExternalFunction get_mean_k_function() const;

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
