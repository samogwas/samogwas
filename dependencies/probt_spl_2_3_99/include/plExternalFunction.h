/*=============================================================================
 * Product        : ProBT 
 * File           : plExternalFunction.h
 * Author         : Juan-Manuel Ahuactzin
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

#ifndef plExternalFunction_h
#define plExternalFunction_h

#include "plConfig.h"


#include <plObject.h>
#include <plVariablesConjunction.h>
#include <plValues.h>
#include <plDataValues.h>


// Work around a compilation problem under MSVC10
#undef min
#undef max 

/*****************************************************************************
 *                           External Classes
 *****************************************************************************/

#include <plExternalMethod.h>

/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/

SPL_DECL2(plExternalFunction);

/** A function defined by the user, generaly used to define a \em
    plCndDeterministic. It's also used with conditional distributions
    accepting user functions as a way to fix the \em
    known (\em right) parameters. 

    Examples of creating and using plExternalFunction can be found in \ref
    dirac_func_example (deterministic distributions) and in \ref
    param_func_example (functions as distribution parameters).
*/
class PL_DLL_API plExternalFunction : public plUserFunction
{
 protected:
  SPL_ACCESS2(plExternalFunction);

  friend class plCndDeterministic;
  friend class plCndNormal;
  friend class plIneqConstraint;
  friend class plCndLogNormal;
  friend class plCndUniform;
  friend class plCndPoisson;
  friend class plCndBinomial;
  friend class plCndBeta;
  friend class plCndExponential;
  friend class plCndGamma;
  friend class plCndWeibull;
  friend class plCndVonMises;
  
  /** Pointer to the External Function generated at kernel level */
  kplUserFunction *function_at_kernel_;

  static void gen_name_if_empty(std::string& func_name);
  
  kplUserFunction* get_function_at_kernel() const;

  void set_function_at_kernel(kplUserFunction* func);

  explicit plExternalFunction(kplUserFunction* func);

  /** Writes the External function at the output given by "out" */
  void Output(std::ostream &out) const;
  
 public:
  /** Pointer to a non-member function used as callback for this class.
      It takes values for input variables, and fills in values for output variables.
   */
  typedef void (*callback_t)(plValues & output_values,
                             const plValues& input_values);

  /** Default constructor. */
  plExternalFunction();
  
  /** Destructor. */
  virtual ~plExternalFunction();
  
  /** Copy constructor. */
  plExternalFunction(const plExternalFunction &f);

  /** Defines a user-function with print name \em func_name where
      \em output_params = \em call_function(input_params), and
      the callback \em call_function is a non-member function. 
      This constructor variant is to be used with conditional parametric
      forms (the output_params are implicitly defined as the parameter
      you want to express as a user-function). */
  plExternalFunction(const plVariablesConjunction &input_params, 
                     callback_t call_function,
		     std::string func_name = "");
  
  /** Defines a user-function with print name \em func_name where
      \em output_params = \em call_function(input_params), and
      the callback \em call_function is a non-member function. 
      This constructor variant is to be used with plCndDeterministic. */
  plExternalFunction(const plVariablesConjunction &output_params, 
		     const plVariablesConjunction &input_params, 
                     callback_t call_function,
		     std::string func_name = "");

  /** Defines a user-function with print name \em func_name where \em
      output_params = \em obj->method(input_params), and the callback \em
      method is a member function.  This constructor variant is to be used with
      conditional parametric forms (the output_params are implicitly defined as
      the parameter you want to express as a user-function). */
  template <class T>
    plExternalFunction(const plVariablesConjunction &input_params,
                       T *obj,
                       void (T::*method)(plValues &output_values, 
                                         const plValues &input_values),
                       std::string func_name = "")
    : plUserFunction()
    , function_at_kernel_(0)
    {
      gen_name_if_empty(func_name);
      set_function_at_kernel(
                             new kplExternalMethod<T>(input_params.get_variable_list(),
                                                      obj, method, func_name));
    }

  /** Defines a user-function with print name \em func_name where \em
      output_params = \em obj->method(input_params), and the callback \em
      method is a member function.  This constructor variant is to be used with
      conditional parametric forms (the output_params are implicitly defined as
      the parameter you want to express as a user-function). */
#ifndef SWIG // the "const" on the method type trips up Swig's parser
  template <class T>
    plExternalFunction(const plVariablesConjunction &input_params,
                       const T *obj,
                       void (T::*method)(plValues &output_values, 
                                         const plValues &input_values) const,
                       std::string func_name = "")
    : plUserFunction()
    , function_at_kernel_(0)
    {
      gen_name_if_empty(func_name);
      set_function_at_kernel(
                             new kplConstExternalMethod<T>(input_params.get_variable_list(),
                                                           obj, method, func_name));
    }
#endif /* SWIG */

  /** Defines a user-function with print name \em func_name where \em
      output_params = \em obj->method(input_params), and the callback \em
      method is a member function.  This constructor variant is to be used with
      plCndDeterministic. 
  */
  template <class T>
    plExternalFunction(const plVariablesConjunction &output_params, 
                       const plVariablesConjunction &input_params,
                       T *obj,
                       void (T::*method)(plValues &output_values, 
                                         const plValues &input_values),
                       std::string func_name = "")
    : plUserFunction()
    , function_at_kernel_(0)
    {
      gen_name_if_empty(func_name);
      set_function_at_kernel(
                             new kplExternalMethod<T>(output_params.get_variable_list(),
                                                      input_params.get_variable_list(),
                                                      obj, method, func_name));
    }
  
  /** Defines a user-function with print name \em func_name where \em
      output_params = \em obj->method(input_params), and the callback \em
      method is a member function.  This constructor variant is to be used with
      plCndDeterministic. 
  */
#ifndef SWIG // the "const" on the method type trips up Swig's parser
  template <class T>
    plExternalFunction(const plVariablesConjunction &output_params, 
                       const plVariablesConjunction &input_params,
                       const T *obj,
                       void (T::*method)(plValues &output_values, 
                                         const plValues &input_values) const,
                       std::string func_name = "")
    : plUserFunction()
    , function_at_kernel_(0)
    {
      gen_name_if_empty(func_name);
      set_function_at_kernel(
                             new kplConstExternalMethod<T>(output_params.get_variable_list(),
                                                           input_params.get_variable_list(),
                                                           obj, method, func_name));
    }
#endif /* SWIG */
  
  /** Assignment operator. */
  plExternalFunction &operator=(const plExternalFunction &f);

  /** Same as operator=() */
  plExternalFunction& assign_from(const plExternalFunction& other)
  {
    return operator=(other);
  }
  
  /** Evaluation operator. */
  plDataValues operator()(const plValues &values) const;
  
  /** Returns all the variables (left and right) of the function. This is
      equivalent to get_left_variables() ^ get_right_variables(). */
  plVariablesConjunction get_variables() const;

  /** Returns the left variables of the function. */
  plVariablesConjunction get_left_variables() const;

  /** Returns the right variables of the function. */
  plVariablesConjunction get_right_variables() const;

  /** Set/unset the @em has_side_effect flag of the function. 
      The @em has_side_effect flag is used for deciding if distributions
      using the function are cached or not in the inference process.
      The default value of this flag is @em false.
      
      \sa has_side_effect
  */
  void set_has_side_effect(bool hse);
  
  /** Return the @em has_side_effect flag of the function.

      \sa set_has_side_effect
  */
  bool has_side_effect()const;

  /** Get function's name */
  const std::string& get_name() const ;


  /** The Sum function */
  static void sum(plValues &the_sum, const plValues &vals);

  /** The Product function */
  static void product(plValues &the_product, const plValues &vals);

  /** The Average function */
  static void average(plValues &the_average, const plValues &vals);

  /** The Identity function */
  static void identity(plValues &the_id, const plValues &vals);

  /** The Min function */
  static void min(plValues &the_min, const plValues &vals);

  /** The Max function */
  static void max(plValues &the_max, const plValues &vals);

  /** The OR function */
  static void OR(plValues &or_result, const plValues &vals);

  /** The AND function */
  static void AND(plValues &and_result, const plValues &vals);
};

/** Interface of object that provides the callback for an external function.
 */
class PL_DLL_API plExternalObjectInterface {
public:
  virtual ~plExternalObjectInterface() {};
  virtual void f(plValues & output, plValues const & input) = 0;
};

/** Creates a plExternalFunction from an object with the appropriate f() method.

    @param[in] input_variables Input variables of the function.
    @param[in] object Pointer to object implementing the plExternalObjectInterface interface. This object
    must not be destroyed before the returned plExternalFunction.
    @func_name[in] Optional function name for pretty-printing.

    To use it, inherit plExternalObjectInterface, and implement f() :
    @code
    class A : plExternalObjectInterface {
    public:
      ...
      virtual void f(plValues & output, plValues const & input) {
        ... // fill in output from input
      }
    };
    int main() {
      A a;
      plExternalFunction f = plCreateExternalFunction(input_variables, &a);
      ...
    }
    @endcode
 */
PL_DLL_API plExternalFunction plCreateExternalFunction(plVariablesConjunction const & input_variables,
                                                       plExternalObjectInterface * object,
                                                       std::string const & func_name = "");

#endif //plExternalFunction_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
