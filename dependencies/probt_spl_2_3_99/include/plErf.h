/// \file plErf.h
/*=============================================================================
 * Product        : ProBT 
 * File           : plErf.h
 * Author         : Kame Mekhnacha
 * Creation       : Wed Jul 25 16:28:52 CEST 2005
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

#ifndef pl_Erf_h
#define pl_Erf_h

#include "plConfig.h"
#include <plMath.h>

#ifndef PL_USE_TABLE_FOR_ERF
#include <cmath>
#endif /* PL_USE_TABLE_FOR_ERF */

#ifdef PL_USE_TABLE_FOR_ERF

/** Erf function */
PL_DLL_API plFloat plErf(plFloat x);

#else
/** Erf function */
inline plFloat plErf(plFloat x)
{
#ifdef PL_LONG_DOUBLE_PRECISION
  return erfl(x);
#else
  return erf(x);
#endif
}

#endif


#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
