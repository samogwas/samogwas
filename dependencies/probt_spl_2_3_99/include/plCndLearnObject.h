/*=============================================================================
 * Product        : ProBT 
 * File           : plCndLearnObject.h
 * Author         : Kamel Mekhnacha
 * Creation       : Fri Feb 23 11:45:42 2007
 *
 *=============================================================================
 *        (c) Copyright 2006, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */

#ifndef plCndLearnObject_h
#define plCndLearnObject_h

#include "plConfig.h"

#include <plLearnDistributionTable.h>

//-----------------------------------------------------------------------------
SPL_DECL1T(plCndLearnObject);

//-----------------------------------------------------------------------------
/** This template class implements conditional learning objects. Learning a
    conditional distribution P(X | Y) for a given learning object type
    (template parameter) T, consists in building a map of non-conditional
    learning objects of type T. This map will contain, for each possible value
    of Y a non-conditional learning object of type T on X. The learning objects
    are assumed to be of the same type T. Consider using plLearnDistributionTable if
    you want to use different types depending on the right variable (Y) values.

    This template class is instantiated for all plNonCndLearnObject subclasses.
    
    \sa plLearnDistributionTable
*/
template <class T>
class PL_DLL_API plCndLearnObject : public plLearnDistributionTable
{
 public:

  /** Empty constructor used for serialization */
  plCndLearnObject()
    : plLearnDistributionTable()
    {}

  /** Constructor to learn the conditional distribution P(left_vars |
      right_vars).  If \em init_object is not empty (the default value), then
      learning for each right value is initialized by \em init_object and the
      returned distribution when \em get_distribution_table (or \em
      get_computable_object) is called will contain a default function obtained
      by calling \em get_distribution on \em init_object.
  */
  plCndLearnObject(const plVariablesConjunction &left_vars, 
                   const plVariablesConjunction &right_vars, 
                   const T &init_object=T())
    : plLearnDistributionTable(left_vars, right_vars, 
                             init_object.get_left_variables().is_empty() ?
                             T(typename T::VarType(left_vars)) : init_object,
                             init_object.get_left_variables().is_empty())
    {}

  /** Returns a pointer to the T learning object corresponding to the @em
      right_value value of the right variables.  Returns 0 if @em right_value
      has never been seen.
  */
  const T* get_learnt_object_for_value(const plValues &right_value)const
  {
    return 
      static_cast<const T*>(plLearnDistributionTable::get_learnt_object_for_value(right_value));
  }

  virtual plLearnObject* clone()const
  {
    return new plCndLearnObject<T>(*this);
  }

 protected:

  virtual std::string get_name()const
  {
    return "plCndLearnObject";
  }

  
};

#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
