/// \file plPhi.h
/*=============================================================================
 * Product        : ProBT 
 * File           : plPhi.h
 * Author         : Kamel Mekhnacha
 * Creation       : February 8 16:29:42 CET 2006
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

#ifndef plPhi_h
#define plPhi_h

#include "plConfig.h"

#include <plErf.h>


/** This function implements the Phi function.
    Phi is the cumulative (repartition) function of the standard normal
    (Gaussian) distribution:
    Phi(x) = 0.5 + 0.5 * erf( x/sqrt(2) )
*/
inline plFloat plPhi(plFloat x)
{
  return PL_HALF*(PL_ONE +  plErf( x / PL_SQRT_2 ) );
}

/** This function implements the inverse of the Phi function above.
 */
PL_DLL_API plFloat plInvPhi(plFloat u);

#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
