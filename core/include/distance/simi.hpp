/****************************************************************************************
 * File: simi.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 31/03/2015

 ***************************************************************************************/
#ifndef SAMOGWAS_SIMI_HPP
#define SAMOGWAS_SIMI_HPP

#define MAX_SIMI 1.0
#define MIN_SIMI 0.0

#include "graph/graph.hpp"
#include "graph/graph_mut_info.hpp"
#include "distance.hpp"

#include "distance_common.hpp"

namespace samogwas
{

typedef std::shared_ptr<Graph> GraphPtr;
typedef std::shared_ptr<std::vector<int>> Local2GlobalPtr;

struct GraphSimilarity {
  GraphSimilarity(GraphPtr g, Local2GlobalPtr& l2gp): graph(g), l2g(l2gp) {}
  virtual double compute( const int varA, const int varB)  = 0;
  virtual void invalidate_entropy_cache() = 0;
  virtual GraphSimilarity& set_criteria(CriteriaPtr c) { criteria = c; return *this; }
  virtual size_t nbr_variables() const = 0;

  virtual void precompute_all() = 0;

 protected:  
  GraphPtr graph;
  Local2GlobalPtr l2g;
  CriteriaPtr criteria;

};

//////////////////////////
struct GraphMutInfoSimilarity: public GraphSimilarity {
 
  GraphMutInfoSimilarity(GraphPtr g, Local2GlobalPtr& l2gp);
  virtual void invalidate_entropy_cache();

  virtual double compute( const int varA, const int varB);
  virtual size_t nbr_variables() const { return l2g->size(); }
  
  virtual void precompute_all() {
    auto SIZE = l2g->size();
    cached_joint_entropies.resize(SIZE*(SIZE-1)/2, -1.0);
    cached_joint_entropies.resize(SIZE, -1.0);
    for ( size_t a = 0; a < SIZE; ++a) {
      auto globalA = this->l2g->at(a);
      Node& nA = (*graph)[globalA];
      cached_entropies[a] = entropy.compute(nA);
      for ( size_t b = a+1; b < SIZE; ++b ) {
        auto globalB = this->l2g->at(b);
        Node& nB = (*graph)[globalB];       

        double minEntropyAB = std::min(this->cached_entropies.at(a), this->cached_entropies.at(b));
        double norm_mutinfo = 0.0;

        if (minEntropyAB != 0) {
          double jE_AB = 1.0; //jEntropy.compute(nA,nB);
          double mi_AB = this->cached_entropies.at(a) + this->cached_entropies.at(b) - jE_AB;
          norm_mutinfo = mi_AB / minEntropyAB;
        }

        auto commonIdx = indexOfPair(a,b,SIZE);
        cached_joint_entropies[commonIdx] = norm_mutinfo;
      }
    }
  }

 private:
  ComputeNodeEntropy entropy;
  ComputeNodeJointEntropy jEntropy;
  std::vector<double> cached_entropies;
  std::vector<double> cached_joint_entropies;
};

typedef std::shared_ptr<GraphSimilarity> SimiPtr;

} // namespace samogwas ends here. 

/****************************************************************************************/
#endif // SAMOGWAS_SIMI_HPP
