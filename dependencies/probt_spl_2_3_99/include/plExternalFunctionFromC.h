///\file plExternalFunctionFromC.h
/*=============================================================================
 * Product        : ProBT 
 * File           : plExternalFunctionFromC.h
 * Author         : David Raulo
 * Creation       : Thi Oct 12 11:44:13 2006
 *
 *=============================================================================
 *        (c) Copyright 2008, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */

#ifndef plExternalFunctionFromC_h
#define plExternalFunctionFromC_h

#include "plConfig.h"

#include <plDLLExport.h>
#include <plExternalFunctionFromSource.h>

//-----------------------------------------------------------------------------
/** A special kind of plExternalFunction defined by dynamically compiling C
    source code provided by the user.  The advantage over normal
    plExternalFunction is that instances of this class can always be
    serialized.
*/
class PL_DLL_API plExternalFunctionFromC: public plExternalFunctionFromSource
{
public:
  plExternalFunctionFromC() {}

  /** Promote from a plExternalFunction. If the object being copied is
      actually of the wrong type, an exception is raised. */
  explicit plExternalFunctionFromC(const plExternalFunction& other);

  /** Defines an External Function with print name \em func_name where
      \em output_params = \em call_function(input_params).
      This version is for dynamically compiled C functions. */
  plExternalFunctionFromC(const plVariablesConjunction &input_params, 
                          std::string func_code, std::string func_name);
  
  /** Defines an External Function with print name \em func_name where
      \em output_params = \em call_function(input_params).
      This version is for dynamically compiled C functions. */
  plExternalFunctionFromC(const plVariablesConjunction &output_params, 
                          const plVariablesConjunction &input_params, 
                          std::string func_code, std::string func_name);

  /** Make a local symbol accessible to the dynamically compiled code.
      This must be done before the function is associated to a distribution. */
  void add_symbol(std::string name, void* addr);

};

//-----------------------------------------------------------------------------
/** Helper function for creating plExternalFunctionFromC objects.
    This takes the function body as input, and takes care of generating the
    function header and footer such as you can use directly the variable names
    in the code. This version is to be used with the plCndDeterministic
    distributions. */
PL_DLL_API plExternalFunctionFromC
createExternalFunctionFromC(const plVariablesConjunction& output,
                            const plVariablesConjunction& input,
                            std::string code, std::string name);

//-----------------------------------------------------------------------------
/** Helper function for creating plExternalFunctionFromC objects.
  This takes the function body as input, and takes care of generating the
  function header and footer such as you can use directly the input variable
  names in the code. You still have to use "output[0]" for the first output,
  "output[1]" for second, etc...
  This version is to be used for the parametric distributions such as
  conditional normals, conditional uniforms and so on. The user-code is given
  as one or several affectations. */
PL_DLL_API plExternalFunctionFromC
createExternalFunctionFromC(const plVariablesConjunction& input,
                            std::string code, std::string name);

//-----------------------------------------------------------------------------
/** Helper function for creating plExternalFunctionFromC objects.  This takes a
    C expression as input, and takes care of generating the function header and
    footer such as you can use directly the right variable names in the code.
    This version is to be used for the parametric distributions such as
    conditional normals, conditional uniforms and so on. The user-code is given
    as a single scalar expression which will be affected to the first output.
    If you need several output values, use the 3-parameters version of
    createExternalFunctionFromC() instead. */
PL_DLL_API plExternalFunctionFromC
createExternalFunctionFromCExpression(const plVariablesConjunction& input,
                                      std::string code, std::string name);

//-----------------------------------------------------------------------------
#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
