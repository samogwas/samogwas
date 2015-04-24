/*=============================================================================
 * Product        : ProBT 
 * File           : plExternalProbFunctionFromSource.h
 * Author         : David Raulo
 * Creation       : Thi Oct 12 11:44:13 2006
 *
 *=============================================================================
 *        (c) Copyright 2008, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */

#ifndef plExternalProbFunctionFromSource_h
#define plExternalProbFunctionFromSource_h

#include "plConfig.h"

#include <plExternalProbFunction.h>

//-----------------------------------------------------------------------------
/** A special kind of plExternalProbFunction defined by dynamically compiling
    source code provided by the user.  The advantage over normal
    plExternalProbFunction objects is that instances of this class are
    guaranteed to be serializable.
*/
class PL_DLL_API plExternalProbFunctionFromSource: public plExternalProbFunction
{
public:
  plExternalProbFunctionFromSource() {}

  /** Promote from a plExternalProbFunction. If the object being copied is
      actually of the wrong type, an exception is raised. */
  explicit plExternalProbFunctionFromSource(const plExternalProbFunction& other);

  /** Get the source code. */
  const std::string& get_code() const;

};

//-----------------------------------------------------------------------------
#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
