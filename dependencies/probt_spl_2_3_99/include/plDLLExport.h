/*=============================================================================
 * Product        : ProBT 
 * File           : plDLLExport.h
 * Author         : David Raulo
 * Creation       : Fri Aug 17 11:38:48 2007
 *
 *=============================================================================
 *        (c) Copyright 2007, ProBayes SAS  -  all rights reserved
 *=============================================================================
 *   
 *   Defines the PL_DLL_API macro for managing the symbols visibility.
 *   
 *=============================================================================
*/

#ifndef PL_DLL_EXPORT_H
#define PL_DLL_EXPORT_H

#include "plConfig.h"

#if defined(_MSC_VER) // MS VC++

#  if defined(PL_DLL_EXPORT)               // generating the DLL
#    define PL_DLL_API __declspec(dllexport)
#  elif defined(PL_DLL)                    // using the DLL
#    define PL_DLL_API __declspec(dllimport)
#  else                                    // generating/using a static library
#    define PL_DLL_API /* empty */
#  endif

#elif __GNUC__ >= 4 // GNU gcc >= 4.x

#  ifdef PL_USE_VISIBILITY
#    define PL_DLL_API __attribute__ ((visibility("default")))
#  else
#    define PL_DLL_API /* empty */
#  endif

#else // not MS VC++, not GNU gcc

#  define PL_DLL_API /* empty */

#endif // _MSC_VER

#if __cplusplus >= 201103L
#define PL_OVERRIDE override
#else
#define PL_OVERRIDE /* nothing */
#endif

#endif //PL_DLL_EXPORT_H_

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
