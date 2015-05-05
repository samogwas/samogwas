/****************************************************************************************
 * File: distance_common.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 04/05/2015

 ***************************************************************************************/
#ifndef SAMOGWAS_DISTANCE_COMMON_HPP
#define SAMOGWAS_DISTANCE_COMMON_HPP

namespace samogwas
{


inline size_t indexOfPair( const size_t A, const size_t B, const size_t N) {
  assert( (B<N) && (A<B) );
  return (2*N-A-1)*A/2 + B - A - 1;
}

} // namespace samogwa ends here. 

/****************************************************************************************/
#endif // SAMOGWA_DISTANCE_COMMON_HPP
