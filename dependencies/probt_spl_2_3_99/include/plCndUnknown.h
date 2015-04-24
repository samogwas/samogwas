/*=============================================================================
 * Product        : ProBT 
 * File           : plCndUnknown.h
 * Author         : Emmanuel Mazer
 * Creation       :  July 6 :08:40 2001
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

#ifndef plCndUnknown_h
#define plCndUnknown_h

#include "plConfig.h"

#include <plCndDistribution.h>
#include <plDistribution.h>

/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/


/** This class allows defining an unknown distribution on a set of variables \em left knowing
 an other set of variables \em right.
*/
class PL_DLL_API plCndUnknown : public plCndDistribution
{
 public:

  /** Empty constructor */
  plCndUnknown();

  /** Construct an unknown conditional distribution.

      Unknown conditional distribution are used for to provide the user with a
      formal way of writing correct decomposition, but are not intended to
      produce any numerical results (i.e the inferred expressions do not
      contain the P(v) term).  All methods applied to this distribution will
      result into an error. Excepted for the method instantiate which will
      return an Unknown distribution.  Unknown distribution are used to build
      descriptions (joint distributions).  Inference (using "ask" method) using
      this joint distribution should always contain all the set of "left"
      variables into their known variables.  \param left is the set of
      variables on which the distribution applies. \param right is the set of
      variables which conditionalize the distribution.
  */
  plCndUnknown(const plVariablesConjunction& left,
	       const plVariablesConjunction& right);

  /** Destructor */
  virtual ~plCndUnknown(){};

  /** Promote from a plCndDistribution. If the object being copied is
      actually of the wrong type, an exception is raised.
  */
  explicit plCndUnknown(const plCndDistribution&);
};

#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
