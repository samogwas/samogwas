/*=============================================================================
 * Product        : ProBT 
 * File           : plLearnCndObject.h
 * Author         : Kamel Mekhnacha
 * Creation       : Thu Oct  12 13:45:57 2010
 *
 *=============================================================================
 *        (c) Copyright 2010, ProBayes SAS  -  all rights reserved
 *=============================================================================
*/

#ifndef plLearnCndObject_h
#define plLearnCndObject_h

#include "plConfig.h"

#include <plLearnObject.h>
#include <plVariablesConjunction.h>
#include <plCndDistribution.h>

SPL_DECL1(plLearnCndObject);

/** This is an abstract class for all conditional learning objects.
 */
class PL_DLL_API plLearnCndObject : public plLearnObject
{
 public:
  /** Default constructor */
  plLearnCndObject()
    : plLearnObject()
    , left_vars_()
    , right_vars_()
    {}

  /** Constructor */
  plLearnCndObject(const plVariablesConjunction &left_vars,
		   const plVariablesConjunction &right_vars)
    : plLearnObject(left_vars^right_vars)
    , left_vars_(left_vars)
    , right_vars_(right_vars)
    {}
  
  /** Returns the left variables of this conditional learning object.
  */
  const plVariablesConjunction& get_left_variables()const
  {
    return left_vars_;
  }

  /** Returns the right variables of this conditional learning object.
  */
  const plVariablesConjunction& get_right_variables()const
  {
    return right_vars_;
  }

  /** Constructs and returns the conditional ditribution corresponding to the current state of this learner */
  virtual plCndDistribution get_cnd_distribution() const = 0;

  virtual plComputableObject get_computable_object()const
  {
    return get_cnd_distribution();
  }

 protected:    
  SPL_ACCESS1(plLearnCndObject);
  plVariablesConjunction left_vars_;
  plVariablesConjunction right_vars_;
};


#endif // plLearnCndObject_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
