
#include <math.h>
#include <algorithm>
#include "distance/simi.hpp"

namespace samogwas {


GraphMutInfoSimilarity::GraphMutInfoSimilarity(GraphPtr g, Local2GlobalPtr& l2gp): GraphSimilarity(g,l2gp) {
  cached_entropies.resize(l2gp->size(), -1.0);
}

double GraphMutInfoSimilarity::compute( const int localA, const int localB) {
  if (localA==localB) return MAX_SIMI;
  if (localA > localB) return compute(localB,localA);
  if (criteria && !criteria->valid(localA,localB)) return MIN_SIMI;

  auto globalA = this->l2g->at(localA), globalB = this->l2g->at(localB);

  Node& nA = (*graph)[globalA], nB = (*graph)[globalB];

  if (this->cached_entropies.at(localA) < 0) {
    this->cached_entropies[localA] = nA.marginalDist->compute_shannon_entropy(); //entropy.compute(nA);
  }

  if (this->cached_entropies.at(localB) < 0) {
    this->cached_entropies[localB] = nB.marginalDist->compute_shannon_entropy();//entropy.compute(nB);
  }

  double eA = this->cached_entropies[localA], eB = this->cached_entropies[localB] ;

  double minEntropyAB = std::min(eA, eB);
  double norm_mutinfo = 0.0;

  if (minEntropyAB != 0) {
    double jE_AB = jEntropy.compute(nA,nB);
    double mi_AB = eA + eB - jE_AB;
    norm_mutinfo = mi_AB / minEntropyAB;

    if ( (norm_mutinfo > 1.000001) || (norm_mutinfo<0) ) {
      printf("COMPUTING: %f - %f - %f --> %f\n", eA, eB, jE_AB, norm_mutinfo);
      
    }
    //assert( (norm_mutinfo <= 1.0) && (norm_mutinfo>=0) ) ;

    
  }



  return norm_mutinfo;
}

void GraphMutInfoSimilarity::invalidate_entropy_cache() {
  std::vector<double>().swap(cached_entropies);
  std::vector<double>().swap(cached_joint_entropies);
}


}
