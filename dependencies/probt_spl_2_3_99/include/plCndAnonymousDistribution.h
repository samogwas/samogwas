/*=============================================================================
 * Product        : ProBT 
 * File           : plCndAnonymousDistribution.h
 * Author         : Kamel Mekhnacha
 * Creation       : Mon Apr 22 15:09:25 2002
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

#ifndef plCndAnonymousDistribution_h
#define plCndAnonymousDistribution_h

#include "plConfig.h"

#include <plCndDistribution.h>
#include <plVariablesConjunction.h>
#include <plExternalProbFunction.h>


/** The \em plCndAnonymousDistribution class implements a conditional
    distribution having the user's external function \em function as \em
    compute method. The only information we have is a way to compute
    the probability (probability density) for a given value of the
    variables \em search_variables and the variables \em
    known_variables. 
*/
class PL_DLL_API plCndAnonymousDistribution :public plCndDistribution
{

public:
  /** Default constructor, needed by the serialization code. */
  plCndAnonymousDistribution() 
    : plCndDistribution()
  { 
    signature_[PLCndAnonymousDistribution] = true; 
  }

  /** Constructs a Conditional Anonymous Distribution with the function \em
      function provided by the user. 

      @anchor prob_func_cnd_example

      Suppose one wants two define a custom conditional density function P(distance | B) over a
       "distance" variable given a parameter variable "B": 

      @code 
      const plSymbol dist("distance", plRealType(0.0, 10.0) );
      const plSymbol B("B", plRealType(0.1, 1.0) );
      @endcode
      
      An example showing the 3 ways allowing creating and using
      plExternalProbFunction for defining a custom P(distance | B) is as follows:
      
      1) Using a non-member function:
       @code 
      plProbValue laplace(const plValues &distance_B_vals)
      {
       const double MU =  3.0;
      
       const double x = distance_B_vals[0];
       const double B = distance_B_vals[1];
       
       return 1.0/(2.0*B)*std::exp(-std::abs(x-MU)/B);
      }

      plExternalProbFunction distance_density_func(dist^B, &laplace);

      plCndAnonymousDistribution P_distance(dist, B, distance_density_func);
      @endcode

      2) Using a member function:
      @code 
      class MyLaplaceClass
      {
      public:
       MyLaplaceClass(double MU)
       : MU_(MU)
       {}
      
       plProbValue laplace(const plValues &distance_B_vals)const
       {
        const double x = distance_B_vals[0];
        const double B = distance_B_vals[1];
      
        return 1.0/(2.0*B)*std::exp(-std::abs(x-MU_)/B);
       }
       private:
       double MU_;
      };

      MyLaplaceClass *my_laplace_obj_ptr = new MyLaplaceClass(3.0);
      plExternalProbFunction distance_density_func(dist^B, my_laplace_obj_ptr, &MyLaplaceClass::laplace);
      
      plCndAnonymousDistribution P_distance(dist, B, distance_density_func);
      @endcode

      3) Using a C source code function:

      @code
      const std::string laplace_C_code = "result = 1.0/(2.0*B)*exp(-fabs(distance-3.0)/B);";

      plExternalProbFunction distance_density_func = 
                createExternalProbFunctionFromC(dist^B, laplace_C_code,"laplace_func");

      plCndAnonymousDistribution P_distance(dist, B, distance_density_func);
      @endcode

      ATTENTION: for performance reasons, using this constructor requires that the passed conjunction left_variables^right_variables
      have the same order as the function's variables.
  */
  plCndAnonymousDistribution(const plVariablesConjunction &left_variables,
                             const plVariablesConjunction &right_variables,
                             const plExternalProbFunction &function);

  
  /** Destructor */
  virtual ~plCndAnonymousDistribution(){};

  /** Promote from a plCndDistribution. If the object being copied is
      actually of the wrong type, an exception is raised.
  */
  explicit plCndAnonymousDistribution(const plCndDistribution&);

  /** Get the external function used to construct this object. */
  plExternalProbFunction get_prob_function() const;

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
