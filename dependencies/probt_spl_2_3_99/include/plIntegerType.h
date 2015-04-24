/// \file plIntegerType.h
/// Definition of types for integer-valued random variables.

/*=============================================================================
 * Product        : ProBT 
 * File           : plIntegerType.h
 * Author         : Juan-Manuel Ahuactzin
 * Creation       : Wed Mar 21 12:28:12 2001
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

#ifndef plIntegerType_h
#define plIntegerType_h

#include "plConfig.h"

#include <plType.h>


/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/


//=============================================================================
/**
   The \em plIntegerType class is used to create integer types with
   particular interval [min,max].
*/
class PL_DLL_API plIntegerType: public plDiscreteType
{
public:
  /** Default constructor, needed by the serialization code. */
  plIntegerType();

  /**
     Creates an integer type with interval  [min,max]
  */
  plIntegerType( int min, int max);

  /** Destructor of the type */
  virtual ~plIntegerType();

  /** Promote from a plType. This makes sense because all the information
      is actually in parent class plType. If the object being copied is
      actually of the wrong type, an exception is raised.
  */
  explicit plIntegerType(const plType&);

};

/** Macro for creating a binary type. 
    It's equivalent to calling plIntegerType(0, 1).
*/
#define PL_BINARY_TYPE plIntegerType(0, 1)


#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
