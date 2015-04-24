/// \file plFactorial.h
/*=======================================================================
 * Product        : ProBT 
 * File           : plFactorial.h
 * Author         : Juan-Manuel Ahuactzin-Larios
 * Creation       : Thu Sep  7 14:31:55 2006
 *
 *=======================================================================
 *                      (c) Copyright 2005, Probayes SAS,
 *                              all rights reserved
 *=======================================================================
 *
 *------------------------- Description ---------------------------------
 *   
 *   
 *-----------------------------------------------------------------------
 */


#ifndef plFactorial_h
#define plFactorial_h

#include "plConfig.h"

//============================================================================
/// Return the log of n!
inline plFloat plLogFactorial(unsigned int n)
{
  plFloat log_res = PL_ZERO;
  
  for (unsigned int i=2;i<=n;++i){ 
    log_res += std::log(plFloat(i));
  }

  return log_res;
}

//============================================================================
/// Return n!
inline plFloat plFactorial(unsigned int n)
{
  return std::exp(plLogFactorial(n));
}

#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
