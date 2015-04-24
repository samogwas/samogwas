/*=============================================================================
 * Product        : ProBT 
 * File           : plUnknown.h
 * Author         : Juan-Manuel Ahuactzin
 * Creation       : Tue Jun 12 10:48:23 2001
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


#ifndef plUnknown_h
#define plUnknown_h

#include "plConfig.h"


#include <plDistribution.h>

/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/

/** The \em plUnknown class permits to define an unknown distribution on a set of variables \em variable. */
class PL_DLL_API plUnknown : public plDistribution
{

public :
   /** Default constructor, needed by the serialization code. */
   inline plUnknown() 
     : plDistribution()
  { 
    signature_.set(PLUnknown); 
  }

  /** Constructs an unknown distribution on  a set of variables \em variable.\n
      When using an unknown probability distribution on v (P(v)), 
      we are only allowed to define inference problems that do not require this distribution 
      (i.e the inferred expressions do not contain the P(v) term).
      Unknown distributions are used to provide the
      user with a formal way of writing correct decompositions (joint distributions), but are
      not intended to produce any numerical results. 
      All methods applied to this distribution will result into an
      error.
  */
  explicit plUnknown(const plVariablesConjunction& variable);

  /** Default destructor */
  virtual ~plUnknown(){};

  /** Promote from a plDistribution. If the object being copied is
      actually of the wrong type, an exception is raised.
  */
  explicit plUnknown(const plDistribution&);

  virtual bool is_base_class()const
  {
    return false;
  }
};

#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
