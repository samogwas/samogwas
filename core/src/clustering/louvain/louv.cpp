/****************************************************************************************
 * File: louv.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 26/11/2014

 ***************************************************************************************/

#include <memory>
#include <algorithm>    // std::random_shuffle
#include <random>

#include "clustering/clustering.hpp"

#include "clustering/louvain/graph.hpp"
#include "clustering/louvain/community.hpp"
#include "clustering/louvain/louv.hpp"

namespace samogwas
{

namespace louvain {

MethodLouvain::MethodLouvain(WeightsPtr wt): changed(true) {
  auto graph = std::make_shared<Graph>(wt, false);
  network = std::make_shared<Network>(graph);
  partition = std::make_shared<Partition>();
  for ( size_t n = 0; n < network->nbr_items(); ++n ) {
    partition->setLabel(n, network->getCommunity(n));
  }

}

std::shared_ptr<AbstractPartition> MethodLouvain::run() {

  first_phase();
  if (changed) {
    second_phase();
  }
  return partition;
}

char* MethodLouvain::name() const {
  char* name = new char[80];
  sprintf( name, "LOUVAIN%s", "");
  return name;
}

}




} // namespace samogawsends here. samogaws

/****************************************************************************************/
