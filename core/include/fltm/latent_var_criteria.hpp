/****************************************************************************************
 * File: latent_var_criteria.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 24/04/2015

 ***************************************************************************************/
#ifndef SAMOGWAS_LATENT_VAR_CRITERIA_HPP
#define SAMOGWAS_LATENT_VAR_CRITERIA_HPP

#include <cmath>
#include "graph/graph_mut_info.hpp"

namespace samogwas
{


/** This functor computes (estimates) the average mutual information between the latent variable
 * and its children. The functor simply returns sum(scaledMutInfo) / nbrOfChildren.
 *
 */
struct AverageMutInfo {
  static double compute( const Graph& graph, const Node& latentNode ) {
    auto childIndexes = latentNode.get_children_global_indexes();
    ComputeNodeMutInfo mutinfo;
  
    double totalSum = 0.0;
    for ( auto idx: childIndexes ) {
      const Node& cNode = graph[idx];

      double eA = latentNode.marginalDist->compute_shannon_entropy();
      double eB = latentNode.marginalDist->compute_shannon_entropy();
      double mutInfo =  mutinfo.compute( latentNode, cNode );

      double minE = std::min(eA,eB);
      if (minE != 0) {
        totalSum += mutInfo / minE;
      }           
    }
    return totalSum / childIndexes.size();
  }
};


}
/****************************************************************************************/
#endif // SAMOGWAS_LATENT_VAR_CRITERIA_HPP
