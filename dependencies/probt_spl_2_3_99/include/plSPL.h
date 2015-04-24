/*=============================================================================
 * Product        : ProBT 
 * File           : plSPL.h
 * Author         : David Raulo
 * Creation       : Thu Jul 26 15:11:45 2007
 *
 *=============================================================================
 *        (c) Copyright 2007, ProBayes SAS  -  all rights reserved
 *=============================================================================
*/

#ifndef PL_SPL_H_
#define PL_SPL_H_

///\file plSPL.h
/// Default values for serialization-related macros.
/// (they are redefined when compiling plSerializer)

#ifndef SPL_ACCESS0
#define SPL_ACCESS0
#endif

#ifndef SPL_DECL1
#define SPL_DECL1(T) 
#endif
#ifndef SPL_ACCESS1
#define SPL_ACCESS1(T) 
#endif
#ifndef SPL_DECL2_NO_FWD
#define SPL_DECL2_NO_FWD(T) 
#endif
#ifndef SPL_DECL2
#define SPL_DECL2(T) 
#endif
#ifndef SPL_ACCESS2
#define SPL_ACCESS2(T) 
#endif
#ifndef SPL_DECL3
#define SPL_DECL3(T) 
#endif
#ifndef SPL_ACCESS3
#define SPL_ACCESS3(T) 
#endif

#ifndef SPL_DECL1T
#define SPL_DECL1T(T) 
#endif
#ifndef SPL_ACCESS1T
#define SPL_ACCESS1T(T) 
#endif
//
#ifndef SPL_DECL1T2
#define SPL_DECL1T2(T) 
#endif
#ifndef SPL_ACCESS1T2
#define SPL_ACCESS1T2(T) 
#endif
//

#ifndef SPL_DECL2T
#define SPL_DECL2T(T) 
#endif
#ifndef SPL_ACCESS2T
#define SPL_ACCESS2T(T) 
#endif
#ifndef SPL_DECL3T
#define SPL_DECL3T(T) 
#endif
#ifndef SPL_ACCESS3T
#define SPL_ACCESS3T(T) 
#endif
#ifndef SPL_DECL3T3
#define SPL_DECL3T3(T) 
#endif
#ifndef SPL_ACCESS3T3
#define SPL_ACCESS3T3(T) 
#endif

#ifndef SPL_DECL3T2
#define SPL_DECL3T2(T) 
#endif
#ifndef SPL_ACCESS3T2
#define SPL_ACCESS3T2(T) 
#endif

#endif // PL_SPL_H_

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
