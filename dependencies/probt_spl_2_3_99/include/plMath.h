/// \file plMath.h
/// Definition of mathematical macros and typedefs.

/*=============================================================================
 * Product        : ProBT 
 * File           : plMath.h
 * Author         : Kamel Mekhnacha
 * Creation       : August 07 2001
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

#ifndef plMath_h
#define plMath_h

#include "plConfig.h"

#include <limits>

//=============================================================================
//                 DLL IMPORT/EXPORT MACROS
//=============================================================================
#include <plDLLExport.h>

//=============================================================================
// Default values for serialization-related macros
// (they are redefined when compiling plSerializer)
//=============================================================================
#include <plSPL.h>

//=============================================================================
//                 STANDARD C INCLUDES
//=============================================================================
#ifdef _MSC_VER
// MSVC needs this before including cmath in order to get M_PI
#define _USE_MATH_DEFINES 1
#endif

#include <cmath>
#include <cfloat>
#include <limits>

//=============================================================================
//                 FLOATING NUMBERS RELATED CONSTANTS
//=============================================================================

#if !defined(PL_LONG_DOUBLE_PRECISION) && !defined(PL_DOUBLE_PRECISION) && !defined(PL_FLOAT_PRECISION)
#error "Choose between FLOAT, DOUBLE and LONG_DOUBLE_PRECISION in plConfig.h"
#endif

#ifdef PL_LONG_DOUBLE_PRECISION
/** Type for floating numbers representation */
typedef long double plFloat;

#define PL_LOG_OF_2     0.69314718055994530942l
#define PL_PI_2         1.57079632679489661923l
#define PL_PI           3.14159265358979323846l
#define PL_2PI          6.28318530717958647692l
#define PL_INV_SQRT_2PI 0.398942280401433l
#define PL_SQRT_PI      1.77245385090551602729816748334l
#define PL_LOG_ROOT_2_PI  0.9189385332046727418l
#define PL_ZERO         0.0l
#define PL_HALF         0.5l
#define PL_ONE          1.0l
#define PL_TWO          2.0l
#define PL_SQRT_2       1.414213562373095145475l
#define PL_E            2.7182818284590452354l  /* e */
#define PL_EPSILON      LDBL_EPSILON // smallest number such that 1+Eps != 1
#define PL_FLOAT_MIN    LDBL_MIN
#define PL_FLOAT_MAX    LDBL_MAX
#define PL_MIN_EXP      -745.0l
#define PL_LOG_OF_10_BASE2 3.321928094887362660l

#define PL_MINUS_ONE -1.0l

#define PL_LOG_SQRT_2PI 0.918938533204673l

#else /* ie, PL_LONG_DOUBLE_PRECISION not defined */

#ifdef PL_DOUBLE_PRECISION
//Use double precision floating numbers
/** Type for floating numbers representation */
typedef double plFloat;


#define PL_LOG_OF_2     0.69314718055994530942
#define PL_PI_2         1.57079632679489661923
#define PL_PI           3.14159265358979323846
#define PL_2PI          6.28318530717958647692
#define PL_INV_SQRT_2PI 0.398942280401433
#define PL_SQRT_PI      1.77245385090551602729816748334
#define PL_LOG_ROOT_2_PI  0.9189385332046727418
#define PL_ZERO         0.0
#define PL_HALF         0.5
#define PL_ONE          1.0
#define PL_TWO          2.0
#define PL_SQRT_2       1.414213562373095145475
#define PL_E            2.7182818284590452354   /* e */
#define PL_EPSILON      DBL_EPSILON // smallest number such that 1+Eps != 1
#define PL_FLOAT_MIN    DBL_MIN
#define PL_FLOAT_MAX    DBL_MAX
#define PL_MIN_EXP      -745.0
#define PL_LOG_OF_10_BASE2 3.321928094887362660

#define PL_MINUS_ONE -1.0

#define PL_LOG_SQRT_2PI 0.918938533204673

#else /* ie, PL_DOUBLE_PRECISION and PL_LONG_DOUBLE_PRECISION not defined */
//Use single precision floating numbers
/** Type for floating numbers representation */
typedef float plFloat;


#define PL_LOG_OF_2     0.69314718055994530942f
#define PL_PI_2         1.57079632679489661923f
#define PL_PI           3.14159265358979323846f
#define PL_2PI          6.28318530717958647692f
#define PL_INV_SQRT_2PI 0.398942280401433f
#define PL_SQRT_PI      1.77245385090551602729816748334f
#define PL_LOG_ROOT_2_PI  0.9189385332046727418f
#define PL_ZERO         0.0f
#define PL_HALF         0.5f
#define PL_ONE          1.0f
#define PL_TWO          2.0f
#define PL_SQRT_2       1.414213562373095145475f
#define PL_E            2.7182818284590452354f  /* e */
#define PL_EPSILON      FLT_EPSILON // smallest number such that 1+Eps != 1
#define PL_FLOAT_MIN    FLT_MIN
#define PL_FLOAT_MAX    FLT_MAX
#define PL_MIN_EXP      -745.0f
#define PL_LOG_OF_10_BASE2 3.321928094887362660f

#define PL_MINUS_ONE -1.0f

#define PL_LOG_SQRT_2PI 0.918938533204673f

#endif //PL_DOUBLE_PRECISION

#endif //PL_LONG_DOUBLE_PRECISION

/** how many digits to print for float values.
    See http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2005/n1822.pdf
 */
template<typename T>
inline unsigned int pl_maxdigits10(const T& /* v */)
{
  return std::numeric_limits<T>::digits * 30103UL / 100000UL + 2;
}

#ifdef PL_USE_PLPROBA
/** how many digits to print for plProbability. */
class plProbability;
template<>
inline unsigned int pl_maxdigits10<plProbability>(const plProbability&)
{
  return 33UL * 30103UL / 100000UL + 2;
}
#endif

//=============================================================================
//                 OTHER CONSTANTS AND TOOLS
//=============================================================================

//=============================================================================
/// Get the  base 2 logarithm
inline plFloat plLogBase2(plFloat v) 
{ 
  return std::log(v)/PL_LOG_OF_2; 
}

static const unsigned int PL_NUMBER_OF_BITS_BY_CHAR = sizeof(unsigned char)*8;

//=============================================================================
inline int plNbBits(int int_val) 
{ 
  int i = 0;
  int_val = int_val - 1;
  while( int_val != 0){
    int_val = int_val >> 1;
    ++i;
  }
  return i;
}

#define PL_MAX_ULONG ULONG_MAX

//=============================================================================
/// Get the closest integer value
inline int plRound( plFloat x )
{
  return (int)(x < PL_ZERO ? x - PL_HALF : x + PL_HALF);
} 


//=============================================================================
/// Get the absolute value of a floating number
inline plFloat plFabs( plFloat x )
{
  return (x < PL_ZERO) ? -x : x;
}


//=============================================================================
//                 TIMING TOOLS
//=============================================================================

#include <plTimer.h>

/** Returns execution time in seconds of the program "prg" in the double floating 
    number "time_in_seconds".
*/
#define PL_TIME_SEC(prg, time_in_seconds)		\
  plTimer::timer.start();				\
  prg;							\
  plTimer::timer.stop();				\
  time_in_seconds = plTimer::timer.get_time_sec();

/** Returns execution time in milliseconds of the program "prg" in the double floating 
    number "time_in_milliseconds".
*/
#define PL_TIME_MSEC(prg, time_in_milliseconds)			\
  plTimer::timer.start();					\
  prg;								\
  plTimer::timer.stop();					\
  time_in_milliseconds = plTimer::timer.get_time_msec();

/** Returns execution time in seconds of the program "prg" in the double floating 
    number "time_in_seconds".
*/
#define PL_TIME PL_TIME_SEC

//=============================================================================
//                 DEFINE A TYPE TO BE USED FOR PROBABILITIES
//=============================================================================

#ifndef PL_USE_PLPROBA
//We are using standard arithmetics using machine's floating numbers.

/** The 'plProbValue' type represents a probability value. Its actual
    representation depends on the compilation variables as follows:
    - if 'PL_USE_PLPROBA' is defined, a 'plProbValue' will be an instance of the
    class 'plProbability' implementing our own representation and arithmetics (see
    'plProbability' documentation). 
    - Otherwise, a 'plProbValue' will be simply a 'plFoat' (i.e. a 'long double', a
    'double' or 'float' depending on the definition of 'PL_LONG_DOUBLE_PRECISION'
    and 'PL_DOUBLE_PRECISION' compilation variables).
*/
typedef plFloat plProbValue;
#define PL_ZERO_PROB PL_ZERO
#define PL_HALF_PROB PL_HALF
#define PL_ONE_PROB PL_ONE
#define PL_TWO_PROB PL_TWO

#define PL_INFINITY_PROB std::numeric_limits<plFloat>::infinity()
#define PL_INVALID_PROB PL_MINUS_ONE
#else

//  We are using our own arithmetics.
#include <plProbability32.h>

/** The 'plProbValue' type represents a probability value. Its actual
    representation depends on the compilation variables as follows:
    - if 'PL_USE_PLPROBA' is defined, a 'plProbValue' will be an instance of
    the class 'plProbability' implementing our own representation and
    arithmetics (see 'plProbability' documentation).
    - Otherwise, a 'plProbValue' will be simply a 'plFoat' (i.e. a 'long
    double', a 'double' or 'float' depending on the definition of
    'PL_LONG_DOUBLE_PRECISION' and 'PL_DOUBLE_PRECISION' compilation
    variables).
*/
typedef plProbability plProbValue;
#define PL_ZERO_PROB plProbability::plZeroProbaConstant
#define PL_HALF_PROB plProbability::plHalfProbaConstant
#define PL_ONE_PROB plProbability::plOneProbaConstant
#define PL_TWO_PROB plProbability::plTwoProbaConstant

#define PL_INFINITY_PROB plProbability::plInfinityProbaConstant
#define PL_INVALID_PROB plProbability::plNaNProbaConstant
#endif

#define PL_DISTRIB_TRUNCATION_THRESHOLD 0.8

//==============================================================================
#endif //!defined(_plMath_h_)

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
