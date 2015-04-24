/*=============================================================================
 * Product        : ProBT 
 * File           : plObject.h
 * Author         : Juan-Manuel Ahuactzin
 * Creation       : Mon Jan 28 17:49:22 2002
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

#ifndef plObject_h
#define plObject_h

#include "plConfig.h"

#include <string>
#include <iostream>

#include <plDLLExport.h>
#include <plSPL.h>

//=============================================================================

SPL_DECL1(plObject);

/**
   Used as the base class of all ProBT API objects.
*/
class PL_DLL_API plObject 
{ 
public:

  /** Constructor */
  plObject();

  /** Destroys the object. */
  virtual ~plObject() {}
  
  /** Returns the object as a string. If @em s in non-null, then it
      will be used for generating the string using "*s << *this". This
      can be used for customizing the precision of the printing the
      numeric values for example.
  */
  std::string to_string(std::ostringstream *s=0)const;

  /** Output in stream */
  friend inline std::ostream& operator<<(std::ostream &out, const plObject& object) 
  {
    object.Output(out); 
    return out;
  }
  
 protected:
  /** Writes the object at the \em out stream. */
  virtual void Output(std::ostream &out) const = 0;
};

//=============================================================================
/** A Bayesian builtin model */
class PL_DLL_API plBuiltinModel : public plObject 
{ 
};

//=============================================================================
/** A State Space Object */
class PL_DLL_API plSampleSpaceObject: public plObject 
{ 
};

//=============================================================================
/** A User Function Object */
class PL_DLL_API plUserFunction: public plObject 
{ 
};

//=============================================================================
#endif //plObject_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
