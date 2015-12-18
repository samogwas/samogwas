/****************************************************************************************
 * File: link.cpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 26/11/2014

 ***************************************************************************************/

#include <memory>
#include "clustering/louvain/link.hpp"

namespace samogwas
{

namespace louvain {

WeightedLink::WeightedLink( const size_t sz ) {
  weights = std::make_shared<std::vector<Weights>>() ;
  weights->reserve(sz);
  for ( size_t i = 0; i < sz; ++i ) {
    weights->push_back( Weights(i+1,0.0) );
  }
}


double WeightedLink::compute( const int varA, const int varB ) {
  if (varA < varB) return compute(varB, varA);
  return (*weights)[varA][varB];
}

void WeightedLink::setWeight( const size_t varA, const size_t varB, const double val ) {
  if (varA < varB) return setWeight(varB, varA, val);
  (*weights)[varA][varB] = val;
}

}

} // namespace samogwasends here. samogwas
