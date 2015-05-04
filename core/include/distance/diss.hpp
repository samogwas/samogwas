/****************************************************************************************
 * File: diss.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 01/04/2015

 ***************************************************************************************/
#ifndef SAMOGWAS_DISS_HPP
#define SAMOGWAS_DISS_HPP

#define MAX_DISSIMILARITY 1.0
#define MIN_DISSIMILARITY 0.0

#include "simi.hpp"

#include "distance_common.hpp"
namespace samogwas
{

struct GraphDissimilarity {
  
  GraphDissimilarity(GraphPtr g, Local2GlobalPtr& l2gp): graph(g), l2g(l2gp) {}
  virtual double compute( const int varA, const int varB)  = 0;
  virtual void invalidate_entropy_cache() = 0;
  virtual GraphDissimilarity& set_criteria(CriteriaPtr c) { criteria = c; return *this; }
  virtual size_t nbr_variables() const = 0;

  // virtual void precompute_all();

  virtual ~GraphDissimilarity() {}
  
 protected:  
  GraphPtr graph;
  Local2GlobalPtr l2g;
  CriteriaPtr criteria;

};

//////////////////////////
struct GraphMutInfoDissimilarity: public GraphDissimilarity {
 
  GraphMutInfoDissimilarity(GraphPtr g, Local2GlobalPtr& l2gp): GraphDissimilarity(g,l2gp), simi(g,l2gp) {}

  virtual double compute( const int varA, const int varB) {
    return MAX_DISSIMILARITY - simi.compute(varA, varB);
  }
  virtual void invalidate_entropy_cache() {
    simi.invalidate_entropy_cache();
  }
  virtual GraphDissimilarity& set_criteria(CriteriaPtr c) { simi.set_criteria(c); return *this; }
  virtual size_t nbr_variables() const { return simi.nbr_variables(); }


  // virtual void precompute_all() {
  //   for ( size_t a = 0; a < l2g->size(); ++a) {
  //     for ( size_t b = a+1; b < l2g->size(); ++b ) {
        
  //     }
  //   }
  // }
  

 private:
  GraphMutInfoSimilarity simi;
};

typedef std::shared_ptr<GraphMutInfoDissimilarity> MutInfoDissPtr;
typedef std::shared_ptr<GraphDissimilarity> DissPtr;

} // namespace samogwas ends here. 

/****************************************************************************************/
#endif // SAMOGWAS_DISS_HPP
