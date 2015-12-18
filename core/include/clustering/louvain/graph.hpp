/****************************************************************************************
 * File: graph.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 19/11/2014

 ***************************************************************************************/
#ifndef SAMOGWAS_LOUVAIN_GRAPH_HPP 
#define SAMOGWAS_LOUVAIN_GRAPH_HPP

#include <memory>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>

#include "distance/simi.hpp"
#include "clustering/louvain/link.hpp"

namespace samogwas
{

namespace louvain {

typedef int NodeIndex;

typedef boost::property<boost::edge_weight_t, float> LinkWeightProperty;
typedef std::pair<NodeIndex, NodeIndex> Link;
typedef double Weight;

typedef boost::adjacency_list<  // adjacency_list is a template depending on :
  boost::listS,                 //  The container used for egdes : here, std::list.
  boost::vecS,                  //  The container used for vertices: here, std::vector.
  boost::undirectedS,           //  undirected
  boost::no_property,           //  The type that describes a Node.
  LinkWeightProperty            //  The type that describes a Link
  > BaseGraph;

/**
 *
 */
class Graph: public BaseGraph {

 public:

  static const int INVALID_WEIGHT = -1;
  typedef double Weight;

  typedef GraphSimilarity WeightMat;
  typedef std::vector<Weight> Weights;
  typedef std::shared_ptr<Weights> WeightsPtr;
  typedef std::shared_ptr<WeightMat> WeightMatPtr;
  typedef Graph::edge_descriptor LinkIndex;
  typedef boost::graph_traits<Graph>::edge_descriptor LinkDesc;
  typedef boost::property_map<Graph, boost::edge_weight_t>::type LinkWeightMap;

  Graph( WeightMatPtr l, bool keep_loops = false);
  /**
   *
   */
  size_t nbr_nodes() const { return weights->nbr_variables(); }


  /**
   *
   */
  size_t nbr_links() const ;

  /**
   *
   */
  Weight weight( const NodeIndex& i, const NodeIndex& j) const;

  /**
   *
   */
  Weight weight( const LinkIndex& j) const { return weight( source(j), target(j) ); }


  void initialize(bool keep_loops = false);
  /**
   *
   */
  Weight selfLoopWeight( const NodeIndex& i ) const { return self_loops->at(i); }


  Weight totalWeights(); // sum over all the edges (links)' weights

  typedef adjacency_iterator AdjIte; // from boost::graph
  typedef std::pair<AdjIte,AdjIte> AdjIteRng; // iterator range for accessing the vertices
  AdjIteRng adjacentNodes( const NodeIndex& i ) const;

  NodeIndex source(LinkIndex link) const { return boost::source(link, *this); }
  NodeIndex target(LinkIndex link) const { return boost::target(link, *this); }

  typedef edge_iterator LinkIte; // from boost::graph
  typedef std::pair<LinkIte, LinkIte> LinkIteRng; // iterator range for accessing the vertices
  LinkIteRng allLinks() const;

  typedef out_edge_iterator OutLinkIte; // from boost::graph
  typedef std::pair<OutLinkIte, OutLinkIte> OutLinkIteRng; // iterator range for accessing the vertices
  OutLinkIteRng linksFrom( const NodeIndex& i ) const;

  Weight linkedWeights( const NodeIndex& node) { // sum of the weights of all adjacent edges
    if ( linked_weights->at(node) == INVALID_WEIGHT ) { // computes the sum if not yet done
      (*linked_weights)[node] = 0;
      for ( auto vp = linksFrom(node); vp.first != vp.second; ++vp.first ) {
        (*linked_weights)[node] += weight(*vp.first);
      }
      (*linked_weights)[node] += selfLoopWeight(node); // self-loops
    }

    return linked_weights->at(node);
  }


  inline NodeIndex addNode() { return boost::add_vertex(*this); }
  inline LinkDesc addLink( const NodeIndex& s, const NodeIndex& t, const Weight w ) {
    return boost::add_edge(s,t,w,*this).first;
  }

  virtual void invalidate() {
    weights->invalidate_entropy_cache();
  }

 private:
  /**
   */
  WeightMatPtr weights; // gives the weighted link between vertices links[a][b]
  size_t nbrLinks;
  Weight total_weights;

  WeightsPtr linked_weights; // weighted_degrees
  WeightsPtr self_loops;
};


typedef boost::graph_traits<Graph>::vertex_descriptor node_desc;
typedef boost::graph_traits<Graph>::edge_descriptor link_desc;


} // namespace louvain ends here

} // namespace samogwas ends here. samogwas

/****************************************************************************************/


/****************************************************************************************/
#endif // SAMOGWAS_LOUVAIN_GRAPH_HPP
