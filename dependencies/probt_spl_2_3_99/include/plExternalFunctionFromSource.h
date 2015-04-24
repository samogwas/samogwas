/*=============================================================================
 * Product        : ProBT 
 * File           : plExternalFunctionFromSource.h
 * Author         : David Raulo
 * Creation       : Thi Oct 12 11:44:13 2006
 *
 *=============================================================================
 *        (c) Copyright 2008, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */

#ifndef plExternalFunctionFromSource_h
#define plExternalFunctionFromSource_h

#include "plConfig.h"

#include <plExternalFunction.h>

//-----------------------------------------------------------------------------
/** A special kind of plExternalFunction defined by dynamically compiling
    source code provided by the user.  The advantage over normal
    plExternalFunction objects is that instances of this class are guaranteed
    to be serializable.
*/
class PL_DLL_API plExternalFunctionFromSource: public plExternalFunction
{
public:
  plExternalFunctionFromSource() {}

  /** Promote from a plExternalFunction. If the object being copied is
      actually of the wrong type, an exception is raised. */
  explicit plExternalFunctionFromSource(const plExternalFunction& other);

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
