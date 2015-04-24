/*=============================================================================
 * Product        : ProBT 
 * File           : plIneqConstraint.h
 * Author         : Kamel Mekhnacha
 * Creation       : Mon May  6 18:43:01 2002
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


#ifndef plIneqConstraint_h
#define plIneqConstraint_h

#include "plConfig.h"


#include <plCndDistribution.h>
#include <plVariablesConjunction.h>
#include <plExternalFunction.h>
#include <plSymbol.h>


/** The \em plIneqConstraint class defines a conditional distribution representing an 
    \em constraint_size-dimensional inequality constraint on the Binary Variable \em constraint_variable.
    This inequality constraint is defined using the \em constraint_size-dimensional user's external 
    function \em constraint_function.
    
    This conditional distribution is defined as follows:
    
    P( [constraint_variable = 1] | [X = x0] ) = 
    - 1 if constraint_function(x0)[i] <= 0, for i = 0...constraint_size
    - 0 otherwise
*/
class PL_DLL_API plIneqConstraint : public plCndDistribution
{
public:
  /** Default constructor, needed by the serialization code. */
  plIneqConstraint() 
    : plCndDistribution()
  { 
    signature_[PLIneqConstraint] = true; 
  }

  /** Defines an inequality constraint on the binary variable \em constraint_variable using
      the \em constraint_size-dimensional user's external function \em constraint_function. 
  */
  plIneqConstraint(const plSymbol& constraint_variable,
		   const plExternalFunction& constraint_function,
		   unsigned int constraint_size);
  
  /** Default destructor */
  virtual ~plIneqConstraint(){};

  /** Promote from a plCndDistribution. If the object being copied is
      actually of the wrong type, an exception is raised.
  */
  explicit plIneqConstraint(const plCndDistribution&);

  /** Get the external function used to construct this object. */
  plExternalFunction get_function() const;

  /** Get the external function dimension. */
  unsigned int get_constraint_size() const;

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
