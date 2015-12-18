/****************************************************************************************
 * File: louv.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 26/11/2014

 ***************************************************************************************/

#ifndef SAMOGWAS_LOUVAIN_LOUV_HPP 
#define SAMOGWAS_LOUVAIN_LOUV_HPP

#include <memory>
#include <map>

#include "distance/simi.hpp"
#include "clustering/clustering.hpp"

#include "community.hpp"
#include "link.hpp"

namespace samogwas
{

namespace louvain {

class MethodLouvain: public ClustAlgo {

 public:
  typedef GraphSimilarity Weights;
  typedef std::shared_ptr<Weights> WeightsPtr;
  typedef std::shared_ptr<Graph> GraphPtr;
  typedef std::shared_ptr<Network> NetworkPtr;
  typedef std::shared_ptr<Partition> PartitionPtr;

  MethodLouvain( WeightsPtr wts);
  virtual std::shared_ptr<AbstractPartition> run();
  virtual char* name() const;
  virtual void invalidate() {
    network->invalidate();
    partition = std::make_shared<Partition>();
    for ( NodeIndex n = 0; n < network->nbr_nodes(); ++n ) {
      partition->setLabel(n, network->getCommunity(n));
    }
  }

  virtual CriteriaPtr get_criteria() { return nullptr; }

  virtual void set_measure(GraphPtr g, Local2GlobalPtr& l2gp, CriteriaPtr criteria = nullptr) {

  }

  //protected:
 public:
  /////////
  inline bool first_phase();
  /////////
  inline void second_phase();
  /////////
  inline void randomize_order( std::vector<NodeIndex>& vt );
  /////////
  inline void resize_order( std::vector<NodeIndex>& node_order, const size_t sz );
  ////////
  inline CommunityIndex comm_best_gain(
      const NodeIndex& node,
      double own_shared,
      double& best_gain,
      double& best_shared ); // shared_weight corresponds to best community

  inline void update_partition( Partition& partition,
                                const Network& ntw,
                                const std::map<CommunityIndex,CommunityIndex>& comm_map );


 public:
  GraphPtr graph;
  NetworkPtr network;

  PartitionPtr partition;
  bool changed;
  std::vector<NodeIndex> node_order; // ensures the order of node processing is random
};


inline void MethodLouvain::randomize_order( std::vector<NodeIndex>& vt ) {
  auto engine = std::default_random_engine{};
  std::shuffle(std::begin(vt), std::end(vt), engine);
}

/**
 *
 */
bool MethodLouvain::first_phase() {
  resize_order(node_order, network->nbr_nodes());
  changed = false;
  double local_changed = true;
  while (local_changed) {
    local_changed = false;
    for ( auto node: node_order ) {
      double best_gain, best_shared;
      CommunityIndex own_comm = network->getCommunity(node);
      double own_shared = network->sharedWeights(node, network->getCommunity(node));
      CommunityIndex best_comm = comm_best_gain( node, own_shared, best_gain, best_shared);
      if ( own_comm != best_comm )
      {
        changed = true;
        local_changed = true;
        network->moveNode( node, best_comm, own_shared, best_shared );
      }
    }
  }
  return changed;
}


/**
 *
 */
void MethodLouvain::second_phase() {
  auto nbrComms = network->nbrCommunities();
  std::vector<CommunityIndex> new2old_comm_map;
  std::copy( network->communities().begin(), network->communities().end(), std::back_inserter(new2old_comm_map) );
  std::map<CommunityIndex, CommunityIndex> old2new_comm_map;  

  for ( CommunityIndex new_comm = 0; new_comm < new2old_comm_map.size(); ++new_comm ) {
    auto old_comm = new2old_comm_map[new_comm];
    old2new_comm_map[old_comm] = new_comm;
  }

  auto links = std::make_shared<WeightedLink>(nbrComms);

  for ( auto vp = network->allLinks(); vp.first != vp.second; ++vp.first ) {
    NodeIndex l = network->source(*vp.first), r = network->target(*vp.first);
    CommunityIndex c_l = old2new_comm_map[network->getCommunity(l)], c_r = old2new_comm_map[network->getCommunity(r)];
    if ( c_l != c_r ) {
      double curr_w = links->compute(c_l, c_r) + network->weight(l,r);
      links->setWeight(c_l, c_r, curr_w);
    }
  }

  for ( CommunityIndex c_i = 0; c_i < nbrComms; ++c_i ) {
    auto old_comm = new2old_comm_map[c_i];
    links->setWeight(c_i, c_i, network->innerCommunityWeight(old_comm) );
  }


  update_partition( *partition, *network, old2new_comm_map); 
  auto g = std::make_shared<Graph>(links, true);
  network->setGraph(g);

}

inline void MethodLouvain::resize_order( std::vector<NodeIndex>& node_order, const size_t sz ) {
  node_order.resize(sz, -1);
  for ( int i = 0; i < sz; ++ i ) {
    node_order[i] = i;
  }
}

////////////
inline CommunityIndex MethodLouvain::comm_best_gain(const NodeIndex& node,
                                                    double own_shared,
                                                    double& best_gain,
                                                    double& best_shared ) {
  // @todo:  # only consider neighbors of different communities
  double loss = network->modularityLoss( node, own_shared);
  best_shared = -2.0; best_gain = -loss;
  CommunityIndex best_comm = network->getCommunity(node), own_comm = network->getCommunity(node);

  std::set<CommunityIndex> processed_neigh_comms { best_comm };
  for ( auto vp = network->adjacentNodes(node); vp.first != vp.second; ++vp.first ) {
    NodeIndex neigh_node = *vp.first;
    CommunityIndex neigh_comm = network->getCommunity(neigh_node);
    if ( processed_neigh_comms.find(neigh_comm) != processed_neigh_comms.end() )  continue; // continues if already processed
    processed_neigh_comms.insert(neigh_comm); // already seen it

    double shared_weights = network->sharedWeights(node, neigh_comm);
    double gain = network->modularityGain(node, neigh_comm, shared_weights);

    if ( gain > best_gain ) {
      best_gain = gain;
      best_shared = shared_weights;
      best_comm = neigh_comm;
    }
  }

  return best_comm;
}


inline void MethodLouvain::update_partition( Partition& partition,
                                             const Network& ntw,
                                             const std::map<CommunityIndex,CommunityIndex>& old2new_comm_map ) {
  for (unsigned node = 0; node < partition.nbr_items(); ++node) {
    auto lab = partition.get_label(node);
    auto old_comm = ntw.getCommunity(lab);
    auto new_comm = old2new_comm_map.at(old_comm);
    partition.setLabel(node, new_comm);
  }
}


}} // namespace samogaws ends here.


/********************************************************/
#endif // SAMOGAWS_LOUV_HPP







