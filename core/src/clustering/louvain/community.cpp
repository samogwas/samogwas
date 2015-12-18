
#include <assert.h>     /* assert */

#include "clustering/louvain/community.hpp"
#include "clustering/louvain/graph.hpp"
#include "clustering/clustering.hpp"


namespace samogwas
{
namespace louvain {

/**
 */
void Network::initialize() {
  m_labelSet.clear(); // clear labels
  m_index2Label.clear();

  community_member_counts.clear();
  community_member_counts.resize( nbr_nodes(), 0); //

  in_weights.clear();
  in_weights.resize( nbr_nodes(), 0);

  tot_linked_weights.clear();
  tot_linked_weights.resize( nbr_nodes(), 0);
  for (unsigned i = 0; i < nbr_nodes(); ++i ) { // creation of singleton communities
    addNode(i,i);
  }

  curr_modularity = INVALID_MODULARITY;
}

/**
 */
double Network::totalWeights() const {
  return graph->totalWeights();
}


/**
 */
CommunityIndex Network::getCommunity( const NodeIndex& node ) const {
  return get_label(node);
}

bool Network::sameCommunity( const NodeIndex& i, const NodeIndex& j ) const {
  return getCommunity(i) == getCommunity(j);
}


// @todo: verify
std::vector<NodeIndex> Network::membersOf( const CommunityIndex& comm ) const {
  assert( comm < nbrCommunities());
  std::vector<NodeIndex> members;

  for (unsigned i = 0; i < nbrCommunities(); ++i) { // when communities are singletons 
    if (getCommunity(i) == comm ) {
      members.push_back(i);
    }
  }

  return members;
}


/**
 */
double Network::modularity() {
  if ( totalWeights() <= 0 ) {
    return INVALID_MODULARITY;
  }
  curr_modularity = 0.0;
  double tw2 = totalWeights()*2;

  for ( auto comm: communities() ) {
    auto sm = tot_linked_weights[comm]/tw2;
    curr_modularity += in_weights[comm] / tw2 - sm*sm;
  }
  return curr_modularity;
}


/**
 */
void Network::moveNode( const NodeIndex& node, const CommunityIndex& target,
                        const double old_shared_weights, const double new_shared_weights ) {
  CommunityIndex from = getCommunity(node);
  if ( from == target ) return;
  removeNode(node, old_shared_weights);
  addNode(node, target, new_shared_weights);
}


/**
 */
void Network::moveNode( const NodeIndex& node, const CommunityIndex& target ) {
  CommunityIndex from = getCommunity(node);
  if ( from == target ) return;
  double old_shared_weights = sharedWeights( node, from );
  double new_shared_weights = sharedWeights( node, target );
  moveNode( node, target, old_shared_weights, new_shared_weights);
}

double Network::modularityGain( const NodeIndex& node,
                                const CommunityIndex& comm,
                                const double shared_weights) const
{
  double gain = 0.0;
  if ( getCommunity(node) == comm ) return gain;
  double sw = shared_weights;
  if ( shared_weights < 0 ) sw = sharedWeights(node,comm);
  double tw2 = 2*totalWeights();
  gain = sw - tot_linked_weights[comm]*linkedWeights(node)/tw2;

  return gain/totalWeights();
}

double Network::modularityLoss( const NodeIndex& node,
                                const double shared_weights ) const
{
  double loss = 0.0;
  const CommunityIndex comm = getCommunity(node);
  double sw = shared_weights;
  if ( shared_weights < 0 ) sw = sharedWeights(node,comm);

  double tw2 = 2*totalWeights();
  if ( shared_weights < 0 ) sw = sharedWeights( node,comm );
  loss = -sw + (tot_linked_weights[comm] - linkedWeights(node) )*linkedWeights(node)/tw2;

  return loss/totalWeights();
}

/**
 */
size_t Network::nbrCommunities() const {
  return communities().size();
}

size_t Network::nbr_nodes() const {
  return graph->nbr_nodes();
}


size_t Network::nbr_links() const {
  return graph->nbr_links();
}


////////////////////////////////////
/** forces node to belong to a given community
 *
 */
void Network::setCommunity( const NodeIndex& node, const CommunityIndex& comm ) {
  setLabel(node, comm);
}


/**
 */
void Network::removeNode( const NodeIndex& node,
                          const double shared_weights) {
  CommunityIndex comm = getCommunity(node);
  if ( comm >= 0 && comm < nbr_nodes() ) {
    setCommunity(node, Network::TEMP_COMMUNITY); // currently, node is unassigned to a community
    community_member_counts[comm]--;
    if (community_member_counts[comm] == 0) {
      removeLabel(comm);
    }

    tot_linked_weights[comm] -= linkedWeights(node);
    in_weights[comm] -=  2*shared_weights + graph->selfLoopWeight(node);
  }
}

/**
 */
void Network::addNode(const NodeIndex& node,
                      const CommunityIndex& comm,
                      const double shared_weights) {
  if ( comm >= 0 && comm < nbr_nodes() ) {
    setCommunity(node,comm);
    community_member_counts[comm]++;
    tot_linked_weights[comm] += linkedWeights(node);
    in_weights[comm] += 2*shared_weights + graph->selfLoopWeight(node);
  }
}



double Network::sharedWeights( const NodeIndex& node, const CommunityIndex& comm ) const {
  double sw = 0.0;
  for ( auto vp = graph->adjacentNodes(node); vp.first != vp.second; ++vp.first ) {
    NodeIndex adj_node = *vp.first;
    if ( getCommunity(adj_node) ==  comm ) {
      sw += graph->weight(node, adj_node);
    }
  }
  return sw;
}


double Network::interCommunityWeight( const CommunityIndex& i,
                                      const CommunityIndex& j ) {
  if ( i == j ) return innerCommunityWeight(i);

  double inter_weight = 0.0;
  for ( auto vp = graph->allLinks(); vp.first != vp.second; ++vp.first ) {
    NodeIndex l = graph->source(*vp.first), r = graph->target(*vp.first);
    CommunityIndex c_l = getCommunity(l), c_r = getCommunity(r);
    if ( c_l == i && c_r == j || c_l == j && c_r == i ) {
      inter_weight += graph->weight(l,r);
    }
  }
  return inter_weight;
}

double Network::innerCommunityWeight( const CommunityIndex& i ) {
  return in_weights[i];
}

} // namespace louvain ends here
} // namespace samogwas ends here








