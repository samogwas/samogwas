/*=============================================================================
 * Product        : ProBT 
 * File           : plRealType.h
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

#ifndef plRealType_h
#define plRealType_h

#include "plConfig.h"

#include <plMath.h>
#include <plType.h>

/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/


//=============================================================================
/**
   The \em plRealType is used to create real types with a particular interval
   \em [min,max[.
 */
class PL_DLL_API plRealType: public plContinuousType
{
 public:

  /** Default constructor, needed by the serialization code. */
  plRealType();

  /** Creates a discretized real type with interval \em
     [min,max[. The interval \em [min,max[ is dicretized into \em
     n_intervals intervals. Distributions containing only discretized
     real or integer variables can use tabulation methods such as
     \em tablulate and \em compile.
     ATTENTION: this is an obsolete constructor. Use
     plContinuousIntervalType::plContinuousIntervalType(plFloat min, plFloat max,  unsigned int n_intervals)
     instead.
  */ 
  plRealType(plFloat min, plFloat max,  unsigned int n_intervals);

  /** Creates a non-discretized real type with interval \em
      [min,max[. No tabulation methods (e.g. \em tablulate and \em
      compile) are allowed for distributions containing non-discretized
      real variables.*/
  plRealType(plFloat min, plFloat max);

  /** Destructor of the type */
  virtual ~plRealType();

  /** Promote from a plType. This makes sense because all the information
      is actually in parent class plType. If the object being copied is
      actually of the wrong type, an exception is raised.
  */
  explicit plRealType(const plType&);
};


#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
