/// \file plExternalProbFunctionFromC.h
/*=============================================================================
 * Product        : ProBT 
 * File           : plExternalProbFunctionFromC.h
 * Author         : David Raulo
 * Creation       : Thi Oct 12 11:44:13 2006
 *
 *=============================================================================
 *        (c) Copyright 2008, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */

#ifndef plExternalProbFunctionFromC_h
#define plExternalProbFunctionFromC_h

#include "plConfig.h"

#include <plDLLExport.h>
#include <plExternalProbFunctionFromSource.h>

//-----------------------------------------------------------------------------
/** A special kind of plExternalProbFunction defined by dynamically compiling
    C source code provided by the user.  The advantage over normal
    plExternalProbFunction is that this class can be serialized.
*/
class PL_DLL_API plExternalProbFunctionFromC
: public plExternalProbFunctionFromSource
{
public:
  plExternalProbFunctionFromC() {}

  /** Promote from a plExternalProbFunction. If the object being copied is
      actually of the wrong type, an exception is raised. */
  explicit plExternalProbFunctionFromC(const plExternalProbFunction& other);

  /** Defines an anonymous probability (density)  C function with 
      print name \em func_name where
      \em P(variables) =  \em function(variables).
      This version is for dynamically compiled C functions. */
  plExternalProbFunctionFromC(const plVariablesConjunction &variables, 
                              std::string func_code, std::string func_name);
  
  /** Make a local symbol accessible to the dynamically compiled code.
      This must be done before the function is associated to a distribution. */
  void add_symbol(std::string name, void* addr);

};

//-----------------------------------------------------------------------------
/** Helper function for creating plExternalProbFunctionFromC objects.
    This takes the function body as input, and takes care of generating the
    function header and footer such as you can use directly the variable names
    in the code. The returned probability value of the function is the value
    of the variable "result". */
PL_DLL_API plExternalProbFunctionFromC
createExternalProbFunctionFromC(const plVariablesConjunction& input,
                                std::string code, std::string name);

//-----------------------------------------------------------------------------
#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
