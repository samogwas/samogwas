/****************************************************************************************
 * File: link.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 26/11/2014

 ***************************************************************************************/
#ifndef SAMOGWAS_LOUVAIN_LINK_HPP
#define SAMOGWAS_LOUVAIN_LINK_HPP

#include <memory>
#include <vector>

#include "distance/simi.hpp"


namespace samogwas
{

namespace louvain {

class WeightedLink: public GraphSimilarity {
  
  typedef std::vector<double> Weights;
  typedef std::shared_ptr< std::vector<Weights> > WeightPtr;

 public:
  // WeightedLink( WeightPtr w );
  WeightedLink(  const size_t sz );

  // virtual double compute( const int varA, const int varB)  = 0;

  virtual double compute( const int varA, const int varB );
  virtual size_t nbr_variables() const { return weights->size(); }

  virtual void invalidate() {
    
  }
  
  virtual void invalidate_entropy_cache() {
    
  }
  
  void setWeight( const size_t varA, const size_t varB, const double val );
  
 protected:  
  WeightPtr weights;
  // SimiPtr simi;
};

}

} // namespace samogwasends here. samogwas

/****************************************************************************************/
#endif // SAMOGWAS_LINK_HPP
