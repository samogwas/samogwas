/*=============================================================================
 * Product        : ProBT 
 * File           : plExternalMethod.h
 * Author         : Kamel Mekhnacha & Juan Manuel Ahuactzin
 * Creation       : Wed Feb  6 14:07:54 2002
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

#ifndef plExternalMethod_h
#define plExternalMethod_h

#include "plConfig.h"


#include <plDataValues.h>
#include <plVariablesConjunction.h>
#include <plValues.h>
#include <plUserFunction.h>


template<class T>
class kplExternalMethod : public kplUserFunction {
private:
  
  /* Pointer to the object on which the method is called */
  T *user_object_;

  /* Pointer to the function defined by the user */
  void (T::*user_method_)(plValues &output, 
                          const plValues &input);

  
public:
  kplExternalMethod(const kplVariableList& known_params,
                    T *obj,
                    void (T::*method)(plValues &output, 
                                      const plValues &input),
                    const std::string& func_name)
    :kplUserFunction(known_params, func_name),
     user_object_(obj), user_method_(method)
  {}
  
  kplExternalMethod(const kplVariableList& search_params, 
                    const kplVariableList& known_params,
                    T *obj,
                    void (T::*method)(plValues &output, 
                                      const plValues &input),
                    const std::string& func_name)
    :kplUserFunction(search_params, known_params, func_name),
     user_object_(obj), user_method_(method)
  {}
  

  virtual ~kplExternalMethod(){};
  
  void evaluate(plDataValues& o_values,
		const plDataValues &i_values) const
  {
    input_values_->set_values_address(const_cast<plDataValues*>(&i_values));
    output_values_->set_values_address(&o_values);
    (user_object_->*user_method_)(*output_values_, *input_values_);
  }

};


template<class T>
class kplConstExternalMethod : public kplUserFunction {
private:
  
  /* Pointer to the object on which the method is called */
  const T *user_object_;

  /* Pointer to the function defined by the user */
  void (T::*user_method_)(plValues &output, 
                          const plValues &input)const;

  
public:
  kplConstExternalMethod(const kplVariableList& known_params,
                         const T *obj,
                         void (T::*method)(plValues &output, 
                                           const plValues &input)const,
                         const std::string& func_name)
    :kplUserFunction(known_params, func_name),
     user_object_(obj), user_method_(method)
  {}
  
  kplConstExternalMethod(const kplVariableList& search_params, 
                         const kplVariableList& known_params,
                         const T *obj,
                         void (T::*method)(plValues &output, 
                                           const plValues &input)const,
                         const std::string& func_name)
    :kplUserFunction(search_params, known_params, func_name),
     user_object_(obj), user_method_(method)
  {}
  
  
  virtual ~kplConstExternalMethod(){};
  
  void evaluate(plDataValues& o_values,
		const plDataValues &i_values) const
  {
    input_values_->set_values_address(const_cast<plDataValues*>(&i_values));
    output_values_->set_values_address(&o_values);
    (user_object_->*user_method_)(*output_values_, *input_values_);
  }

};




#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
