/*=============================================================================
 * Product        : ProBT 
 * File           : plExternalProbMethod.h
 * Author         : Kamel Mekhnacha
 * Creation       : Tue Apr  9 11:28:58 2002
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


#ifndef plExternalProbMethod_h
#define plExternalProbMethod_h

#include "plConfig.h"

#include <plVariablesConjunction.h>
#include <plValues.h>
#include <plUserProbFunction.h>


template<class T>
class  kplExternalProbMethod : public  kplUserProbFunction
{
private:
  
  /* Pointer to the object on which the method is called */
  T *user_object_;
  
  /* Pointer to the function defined by the user */
  plProbValue (T::*user_method_)(const plValues &input);

  
public:  
  kplExternalProbMethod(const kplVariableList& vars,
			T *obj,
			plProbValue (T::*method)(const plValues &input),
			const std::string& func_name)
    :kplUserProbFunction(vars, func_name),
     user_object_(obj), user_method_(method)
  {}
  
 

  virtual ~kplExternalProbMethod(){};

  plProbValue operator()(const plValues& values) const 
  {
    return (user_object_->*user_method_)(values);
  }
 
};


template<class T>
class  kplConstExternalProbMethod : public  kplUserProbFunction
{
private:
  
  /* Pointer to the object on which the method is called */
  const T *user_object_;
  
  /* Pointer to the function defined by the user */
  plProbValue (T::*user_method_)(const plValues &input)const;

  
public:
  kplConstExternalProbMethod(const kplVariableList& vars,
                             const T *obj,
                             plProbValue (T::*method)(const plValues &input)const,
                             const std::string& func_name)
    :kplUserProbFunction(vars, func_name),
     user_object_(obj), user_method_(method)
  {}

  virtual ~kplConstExternalProbMethod(){}

  plProbValue operator()(const plValues& values) const 
  {
    return (user_object_->*user_method_)(values);
  }
 
};



#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
