/// \file plVariableType.h
/*=============================================================================
 * Product        : ProBT 
 * File           : plVariableType.h
 * Author         : Kamel Mekhnacha
 * Creation       : Thu Jan 31 19:03:46 2002
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
#ifndef plVariableType_h
#define plVariableType_h

#include "plConfig.h"

/** 
    Enum type corresponding to a \em %plType.

    @internal The percent sign is here because Doxygen creates a broken link here.
*/
enum plVariableType { PL_INTEGER, /*!< Integer type. */
                      PL_REAL, /*!< Real type. */
                      PL_CONTINUOUS_INTERVAL, /*!< Continuous Interval type. */
                      PL_DISCRETE_INTERVAL, /*!< Discretized Interval type. */
                      PL_LABEL, /*!< LABEL type. */
                      PL_UNKNOWN_TYPE /*!< Unknown type. */
};

#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
