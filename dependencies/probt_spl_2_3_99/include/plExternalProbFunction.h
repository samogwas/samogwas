/*=============================================================================
 * Product        : ProBT 
 * File           : plExternalProbFunction.h
 * Author         : Kamel Mekhnacha
 * Creation       : Tue Apr  9 10:41:31 2002
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

#ifndef plExternalProbFunction_h
#define plExternalProbFunction_h

#include "plConfig.h"


#include <plObject.h>
#include <plVariablesConjunction.h>
#include <plValues.h>
#include <plDataValues.h>

/*****************************************************************************
 *                           External Classes
 *****************************************************************************/

#include <plExternalProbMethod.h>


/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/

SPL_DECL2(plExternalProbFunction);

/** A function defined by the user to be used as a probability or a density
    function.  It's generaly used to define a \em plAnonymousDistribution or a \em
    plCndAnonymousDistribution.

    Examples of creating and using plExternalProbFunction can be found in \ref
    prob_func_example (plAnonymousDistribution) and in \ref
    prob_func_cnd_example (plCndAnonymousDistribution).
*/
class PL_DLL_API plExternalProbFunction : public plUserFunction
{

protected:
  SPL_ACCESS2(plExternalProbFunction);
  friend class plAnonymousDistribution;
  friend class plCndAnonymousDistribution;

  /** pointer to the Prob Function generated at kerel level */
  kplUserProbFunction *function_at_kernel_;
  
  void gen_name_if_empty(std::string& func_name);
  
  kplUserProbFunction* get_function_at_kernel() const;
  
  void set_function_at_kernel(kplUserProbFunction* func);

  explicit plExternalProbFunction(kplUserProbFunction* func);

  /** Writes the external function at the output given by "out" */
  void Output(std::ostream &out) const;

public:
  /** pointer to a non-member function used as callback for this class. */
  typedef plProbValue (*callback_t)(const plValues& input_values);

  /** Default constructor. */
  plExternalProbFunction();
  
  /** Destructor */
  virtual ~plExternalProbFunction();
  
  plExternalProbFunction(const plExternalProbFunction& f);
  
  /** Defines an anonymous probability (density) C function with print name
      \em func_name where \em P(variables) = \em function(variables). */
  plExternalProbFunction(const plVariablesConjunction &variables,
			 callback_t function, std::string func_name="");
  
  /** Defines an anonymous probability (density) method with print name \em
      func_name where \em P(variables) = \em obj->method(variables). */
  template <class T>
  plExternalProbFunction(const plVariablesConjunction &variables,
			 T *obj,
			 plProbValue (T::*method)(const plValues &input_values),
			 std::string func_name="")
    : plUserFunction()
    , function_at_kernel_(0)
  {
    gen_name_if_empty(func_name);
    set_function_at_kernel(
      new kplExternalProbMethod<T>(variables.get_variable_list(),
				   obj, method, func_name));
  }

#ifndef SWIG
  /** Defines an anonymous probability (density) method with print name \em
      func_name where \em P(variables) = \em obj->method(variables). */
  template <class T>
  plExternalProbFunction(const plVariablesConjunction &variables,
			 const T *obj,
			 plProbValue (T::*method)(const plValues &input_values)const,
			 std::string func_name="")
    : plUserFunction()
    , function_at_kernel_(0)
  {
    gen_name_if_empty(func_name);
    set_function_at_kernel(
                           new kplConstExternalProbMethod<T>(variables.get_variable_list(),
                                                             obj, method, func_name));
  }
#endif /* SWIG */
  
  /** Assignment operator */
  plExternalProbFunction &operator=(const plExternalProbFunction& f);

  /** Same as operator=() */
  plExternalProbFunction& assign_from(const plExternalProbFunction& other)
  {
    return operator=(other);
  }
  
  /** Evaluates the plExternalProbFunction */
  plProbValue operator()(const plValues &values) const;

  /** Returns the variables of the function. */
  plVariablesConjunction get_variables() const;

  /** Set/unset the @em has_side_effect flag of the function. 
      The @em has_side_effect flag is used for deciding if distributions
      using the function are cached or not in the inference process.
      The default value of this flag is @em false.
      
      \sa has_side_effect
  */
  void set_has_side_effect(bool hse){function_at_kernel_->set_has_side_effect(hse);}
  
  /** Return the @em has_side_effect flag of the function.

      \sa set_has_side_effect
  */
  bool has_side_effect()const{return function_at_kernel_->has_side_effect();}

  const std::string& get_name() const { return function_at_kernel_->get_name(); }

};

class PL_DLL_API plExternalProbObjectInterface {
 public:
  virtual ~plExternalProbObjectInterface() {}
  virtual plProbValue f(plValues const & input_values) = 0;
};

PL_DLL_API plExternalProbFunction plCreateExternalProbFunction(plVariablesConjunction const & input_variables,
                                                               plExternalProbObjectInterface * object,
                                                               std::string const & func_name = "");

#endif //plExternalProbFunction_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
