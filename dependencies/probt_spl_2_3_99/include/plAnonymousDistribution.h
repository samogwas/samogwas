/*=============================================================================
 * Product        : ProBT 
 * File           : plAnonymousDistribution.h
 * Author         : Kamel Mekhnacha
 * Creation       : Wed Mar 27 11:41:27 2002
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

#ifndef plAnonymousDistribution_h
#define plAnonymousDistribution_h

#include "plConfig.h"

#include <plDistribution.h>
#include <plVariablesConjunction.h>
#include <plExternalProbFunction.h>


/** The \em plAnonymousDistribution class implements a distribution having the
    user external function \em function as \em compute method. The
    only information we have is a way to compute the probability (or
    probability density) for a given value of the variables \em
    variable.
*/
class PL_DLL_API plAnonymousDistribution :public plDistribution
{
public:

  /** Default constructor, needed by the serialization code. */
  plAnonymousDistribution() 
    : plDistribution()
  { signature_[PLAnonymousDistribution] = true; }
  
  /** Constructs an Anonymous Distribution with external function \em
      function.

      @anchor prob_func_example
      
      Suppose one wants two define a custom density function P(dist) over a given "dist" variable:
      @code 
      const plSymbol dist("distance", plRealType(0.0, 10.0) );
      @endcode
      
      An example showing the 3 ways allowing creating and using
      plExternalProbFunction for defining a custom P(dist) is as follows:
      
      1) Using a non-member function:
       @code 
      plProbValue laplace(const plValues &distance_val)
      {
       const double B = 0.6;
       const double MU = 3.0;
      
       const double x = distance_val[0];
      
       return 1.0/(2.0*B)*std::exp(-std::abs(x-MU)/B);
      }

      plExternalProbFunction distance_density_func(dist, &laplace);

      plAnonymousDistribution P_distance(dist, distance_density_func);
      @endcode

      2) Using a member function:
      @code 
      class MyLaplaceClass
      {
      public:
       MyLaplaceClass(double B, double MU)
       : B_(B)
       , MU_(MU)
       {}
      
       plProbValue laplace(const plValues &distance_val)const
       {
        const double x = distance_val[0];
      
        return 1.0/(2.0*B_)*std::exp(-std::abs(x-MU_)/B_);
       }

      private:
       double B_;
       double MU_;
      };

      MyLaplaceClass *my_laplace_obj_ptr = new MyLaplaceClass(0.6, 3.0);
      plExternalProbFunction distance_density_func(dist, my_laplace_obj_ptr, &MyLaplaceClass::laplace);
      
      plAnonymousDistribution P_distance(dist, distance_density_func);
      @endcode

      3) Using a C source code function:

      @code
      const std::string laplace_C_code = "result = 1.0/(2.0*0.6)*exp(-fabs(dist-3.0)/0.6);";

      plExternalProbFunction distance_density_func = 
                createExternalProbFunctionFromC(dist, laplace_C_code,"laplace_func");

      plAnonymousDistribution P_distance(dist, distance_density_func);
      @endcode

      ATTENTION: for performance reasons, using this constructor requires that the passed variables have
      the same order as the function's variables. You can also use the
      constructor plAnonymousDistribution(const plExternalProbFunction& ) below.
  */
  plAnonymousDistribution(const plVariablesConjunction &variable, 
                          const plExternalProbFunction &function);

  /** Same as the constructr above however the variables are
      assumed to be the same as the ones provided by the passed function.
  */
  explicit plAnonymousDistribution(const plExternalProbFunction& function );
  
  /** Destructor */
  virtual ~plAnonymousDistribution(){};

  /** Promote from a plDistribution. If the object being copied is
      actually of the wrong type, an exception is raised.
  */
  explicit plAnonymousDistribution(const plDistribution&);

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
