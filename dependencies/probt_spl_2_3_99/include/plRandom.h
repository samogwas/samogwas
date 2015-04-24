/// \file plRandom.h
/*=============================================================================
 * Product        : ProBT 
 * File           : plRandom.h
 * Author         : Kamel Mekhnacha
 * Creation       : Wed Nov  7 13:06:08 CET 2003
 *
 *=============================================================================
 *     (c) Copyright 2000, Centre National de la Recherche Scientifique,
 *                              all rights reserved
 *=============================================================================
 *
 *------------------------- Description ---------------------------------------        
 *   
 *-----------------------------------------------------------------------------
 */

#ifndef plRandom_h
#define plRandom_h

#include "plConfig.h"

#include <vector>
#include <plMath.h>
#include <plPhi.h>


#define PL_USE_MT19937

#ifdef PL_USE_MT19937

#include <plMt19937.h>

//=============================================================================
/** Draw uniformly an integer random value in the range [0,max-1] */
inline  unsigned int plRandomInt(unsigned int max) 
{
  const unsigned int res = plMt19937::generator_.genrand_int32()%max;
  return res;
}

//=============================================================================
/** Draw uniformly a floating number random value in the range [0,max[ */
inline plFloat plRandomFloat(plFloat max) 
{
  const plFloat res = max*plMt19937::generator_.genrand_res53();
  return res;
}

#else // not PL_USE_MT19937 (Default compiler random generator rand() )

static const plFloat PL_RAND_MAX_PLUS_ONE = RAND_MAX+PL_ONE;

//=============================================================================
/** Draw uniformly an integer random value in the range [0,max-1] */
inline  unsigned int plRandomInt(unsigned int max) 
{
  return (unsigned int)(max* (rand()/PL_RAND_MAX_PLUS_ONE));
}

//=============================================================================
/** Draw uniformly a floating number random value in the range [0,max[ */
inline plFloat plRandomFloat(plFloat max) 
{ 
  return max*rand()/PL_RAND_MAX_PLUS_ONE;
}

#endif

//=============================================================================
/** Seed the random generator */
PL_DLL_API void plSRandom( int seed );


//=============================================================================
/** Draw a floating number random value normally distributed widh a 
    @em m as mean and @em sd as standard deviation
*/
PL_DLL_API plFloat plNormalRandom(plFloat m, plFloat sd);


//=============================================================================
/** Draw a floating number random value normally distributed widh a 
    @em m as mean and @em sd as standard deviation in the range [a,b[
*/
inline plFloat plNormalRandom(plFloat m, plFloat sd, plFloat a, plFloat b)
{
  plFloat result;
  do{
    result = plNormalRandom(m, sd);
  }
  while((result < a) || (result >= b));
  return result;
}

//=============================================================================
inline plFloat plNormalRandomUsingInvPhi(plFloat m, plFloat sd, 
					 plFloat two_inta_minus_half,
					 plFloat two_intb_minus_half)
{
  const plFloat u = 
    PL_HALF*( PL_ONE + two_inta_minus_half + plRandomFloat( two_intb_minus_half - two_inta_minus_half));
  
  return m + sd*plInvPhi(u);
}

//=============================================================================
inline plFloat plHalfNormalRandom(plFloat m, plFloat sd)
{
  if (sd > PL_ZERO)
    return (m + plFabs(plNormalRandom(PL_ZERO, sd)));
  else 
    return (m - plFabs(plNormalRandom(PL_ZERO, -sd)));
}

// Drawing tools using a repartition function


//=============================================================================
template<class T>
inline unsigned long  plDrawUsingRepartitionFunctionBetween(const T *&repartition, 
							    unsigned long  start_index, 
							    unsigned long  end_index, const T& u)
{
  if( end_index == start_index)
    return start_index;
  
  const unsigned long  middle_index = (start_index + end_index) / 2;

  if(repartition[middle_index] <= u)
    return plDrawUsingRepartitionFunctionBetween(repartition, middle_index+1,  end_index, u);
  
  return plDrawUsingRepartitionFunctionBetween(repartition, start_index, middle_index, u);
}

//=============================================================================
/** Return a random index drawn from the distribution corresponding to the repartition
    function (table) @em repartition
*/
template<class T>
inline unsigned long  plDrawUsingRepartitionFunction( unsigned long size, const T *repartition)
{
  const T u = repartition[size - 1] * plRandomFloat(PL_ONE);
  return plDrawUsingRepartitionFunctionBetween(repartition, (unsigned long)0, size - 1, u);
}

//=============================================================================
/** Return a random index drawn from the distribution corresponding to the repartition
    function (table) @em repartition
*/
template<class T>
inline unsigned long  plDrawUsingRepartitionFunction(const std::vector <T> &repartition )
{
  if(!repartition.empty()) {
    return plDrawUsingRepartitionFunction(repartition.size(), &repartition[0]);
  }

  // error ??
  return 0;
}

/** Basic random number generation utility functions */
class plRandom
{
public:

  /** Seed the random generator */
  inline static void seed(int seed_value)
  {
    plSRandom(seed_value);
  }
  
  /** Draw uniformly an integer random value in the range [0,max-1].
      It uses the MT1937 random generator.
   */
  inline static unsigned int uniform_integer(unsigned int max)
  {
    return plRandomInt(max);
  }
  
  /** Draw uniformly a floating number random value in the range [0,max[.
      It uses the MT1937 random generator.
  */
  inline static plFloat uniform_float(plFloat max)
  {
    return plRandomFloat(max);
  }
  
  /** Draw a floating number random value normally distributed widh
      @em m as mean and @em sd as standard deviation.
      
      It uses the Box-Muller algorithm and is based on the MT1937 uniform random
      generator.
  */
  inline static plFloat normal_float(plFloat m, plFloat sd)
  {
    return plNormalRandom(m, sd);
  }

  /** Draw a floating number random value normally distributed widh
      @em m as mean and @em sd as standard deviation in the range [a,b[.

      It uses the Box-Muller algorithm and is based on the MT1937 uniform random
      generator.
  */
  inline static plFloat normal_float(plFloat m, plFloat sd, plFloat a, plFloat b)
  {
    return plNormalRandom(m, sd, a, b);
  }

  /** Return a random index drawn from the distribution corresponding to the repartition
      function (table) @em repartition
  */
  template<typename T>
  inline static unsigned long  draw_using_repartition( unsigned long size, const T *repartition)
  {
    return plDrawUsingRepartitionFunction(size, repartition);
  }

  /** Return a random index drawn from the distribution corresponding to the repartition
    function (table) @em repartition
  */
  template<typename T>
  inline static unsigned long  draw_using_repartition(const std::vector<T> &repartition)
  {
    return plDrawUsingRepartitionFunction(repartition);
  }

  
};

#endif //plRandom_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
