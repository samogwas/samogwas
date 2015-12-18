/****************************************************************************************
 * File: Graph.hpp
 * Description: This module provides the structure for the FLTM model.
 *
 * @author: Duc-Thanh Phan siolag161 (thanh.phan@outlook.com), under the supervision of Christine Sinoquet
 * @date: 13/07/2014

 ***************************************************************************************/
#ifndef SAMOGWAS_GRAPH_HPP
#define SAMOGWAS_GRAPH_HPP

#include <memory> // shared_ptr
#include <math.h>
#include <map>

#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_traits.hpp"
#include "pl.h"

namespace samogwas
{

typedef plJointDistribution JointDist;
typedef std::shared_ptr<plDistribution> DistPtr;
typedef std::shared_ptr<plCndDistribution> CndDistPtr;
typedef int IndexType;
typedef std::string LabelType;
typedef plSymbol RandomVariable;
typedef std::map<IndexType, LabelType> Index2Label;
typedef std::map<LabelType, IndexType> Label2Position;
typedef std::map<LabelType, IndexType> Label2Index;
typedef plVariablesConjunction::const_iterator VarIter;


/** Forward declaration for Node class as it is part of the Graph declaration
 *
 */
struct Node;

/*
 *
 */
typedef boost::adjacency_list< boost::vecS,
                               boost::vecS,
                               boost::directedS,
                               Node,
                               boost::no_property> Graph;

typedef std::shared_ptr<Graph> GraphPtr;
typedef std::shared_ptr<JointDist> JointDistPtr;
typedef std::vector<double> CondObsDist;
typedef std::shared_ptr<CondObsDist> CondObsDistPtr;
typedef std::vector<int> DataVec;
typedef std::shared_ptr<DataVec> DataVecPtr;
typedef plVariablesConjunction Variables;

/**
 * Node
 */
struct Node {
  typedef plComputableObjectList::const_iterator plCompIte;

  ~Node() { }
  Node(): position(-1), level(0) {}

  /**
   */
  Node& set_children_distributions( CondObsDistPtr dist);

  /**
   */
  Node& set_children_distributions(plCompIte beg, plCompIte end);

  /**
   */
  Node& set_joint_distribution( const JointDist& jointDistri );

  /**
   */
  Node& set_joint_distribution( const plComputableObjectList& jointDistri );

  /**
   */
  Node& set_marginal_distribution( plCompIte it);

  /**
   */
  Node& set_marginal_distribution( DistPtr dist);

  /**
   */
  Variables get_children_variables() const;

  /**
   */
  std::vector<int> get_children_global_indexes() const;

  /**
   */
  LabelType getLabel() const;

  /**
   */
  int nbr_children() const;

  /**
   */
  int cardinality() const;

  /**
   */
  int local2GlobalIdx(const int local) const;

  /** global index
   */
  IndexType getIndex() const;

  /** @todo: private vs public (local)
   */
  Node& set_local_indexes( VarIter beg, VarIter endx, const Label2Index &label2GlobalIndex);

  /**
   */
  Node& set_local_indexes( const Label2Index &label2GlobalIndex );

  /**
   */
  Node& set_level(const int level) { this->level = level; return *this; }


  /** Computes in function of its children's positions
   */
  Node& set_position();

  /**
   */
  Node& set_position(const int position) { this->position = position; return *this; }

  /**
   */
  Node& update_level();

  /**
   */
  Node& set_variable(plSymbol& var) { this->variable = var; return *this; }

  /**
   */
  Node& set_graph(GraphPtr graph) { this->graph = graph; return *this; }


  /** global index
   */
  Node& set_index(const int index) { this->index = index; return *this; }

  /** Data for observed variable. Imputed data for latent variable.
   *
   */
  Node& set_data_vec(DataVecPtr dtvt, const bool computeEmpDist = true);

  /** @todo: remove?
   */
  Node& set_cnd_obs_vec(CondObsDistPtr cndObsDist, const bool computeEmpDist = true);

  /** Computes P(X=a) // X: variable associated to the node
   */
  double compute_prob(const int a) const;

  /**  Computes the probability of observing value val at the observation obs
   *   P(val,obs) = 0/1 for observed variable
   *              = probability for latent variable
   */
  double compute_cond_prob_obs(const int val, const int obs) const;

  /** P(cNode = childVal | X=parentVal)
   */
  double compute_cond_prob( const Node& cNode, const int childVal, const int parentVal ) const;

  /** cIdx: local index of the child
   */
  double compute_cond_prob( const int cIdx, const int childVal, const int parentVal ) const;

  /**
   */
  double compute_cond_prob( const int cIdx, const plValues& cV, const plValues& pV ) const;

  /**
   */
  Node& copy_data( const Node& n );

  /**
   */
  bool is_leaf() const { return level == 0; }

  /**
   */
  bool is_parent_of( const Node& ne) const ;

  /**
   */
  bool is_child_of( const Node& ne) const  { return ne.is_parent_of(*this); }


  /****************************************************/
  IndexType index; // global index
  LabelType label; // cache for variable.name()

  int position; // physical position on the genome
  int level; // indicates the level to which this node belongs.
  GraphPtr graph; // reference to its graph
  RandomVariable variable; // represents the underlying random variable.

  DistPtr marginalDist;
  DataVecPtr dataVec; // observed variable: vector of size nbrObservations
  CondObsDistPtr cndObsDist; // flattened nbrObservations x cardinality matrix
  std::vector<CndDistPtr> cndChildrenDists; // cardinality(X) = P
  // vector of conditional distributions P(Ei|X)
  std::map<int,int> global2localIdx;

};

//////////////////////////////////////////////////////////////////
/**
 *
 */
typedef Graph::vertex_descriptor vertex_t;

/**
 *
 */
typedef Graph::edge_descriptor edge_t;

typedef boost::graph_traits<Graph >::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::vertex_iterator vertex_iterator;
typedef boost::graph_traits<Graph>::edge_iterator edge_iterator;

typedef std::pair<vertex_iterator, vertex_iterator> vertex_iter_pair;
typedef std::pair<edge_iterator, edge_iterator> edge_iter_pair;

inline DistPtr create_emp_distribution( plSymbol& variable, const std::vector<int>& dataVec) {
  std::vector<double>prob( variable.cardinality(), 0.0);
  for (unsigned i = 0; i < dataVec.size(); ++i) {
    ++prob[dataVec[i]];
  }

  for (unsigned i = 0; i < variable.cardinality(); ++i) {
    prob[i] /= dataVec.size();
  }

  auto dist = std::make_shared<plDistribution>(variable, prob);

  return dist;
  
}

inline DistPtr create_emp_distribution( plSymbol& variable, const std::vector<double>& cndDist) {
  std::vector<double>prob( variable.cardinality(), 0.0);
  unsigned N = cndDist.size() / variable.cardinality();
  for (unsigned i = 0; i < N; ++i) {
    for ( int j = 0; j < variable.cardinality(); ++j ) {
      prob[j] += cndDist[i*variable.cardinality()+j];
    }
  }

  double sum = 0.0;

  for (unsigned i = 0; i < variable.cardinality(); ++i) {
    sum += prob[i];
    prob[i] /= N;

  }
  auto dist = std::make_shared<plDistribution>(variable, prob);
  return dist;
}



//////////////////
inline Node& createNode( std::shared_ptr<Graph> graph,
                         const unsigned cardinality,
                         const std::string label = "",
                         const int position = -1,
                         const int level = -1) {
 
  vertex_t vertexId = boost::add_vertex(*graph); // adds a new Node to the graph and
  // returns the newly added node

  Node &newNode = (*graph)[vertexId];
  newNode.variable = RandomVariable(label, plIntegerType(0, cardinality - 1));
  newNode.position = position; // physical position on the genome
  newNode.label = label;
  newNode.index = vertexId;
  newNode.graph = graph;
  newNode.level = level;
  return newNode;
}

inline Node& createNode( std::shared_ptr<Graph> graph,
                         const std::vector<int>& dataVec,
                         const unsigned cardinality,
                         const std::string label = "",
                         const int position = -1,
                         const int level = -1) {
 
  Node& node = createNode(graph, cardinality, label, position, level);  
  auto dist = create_emp_distribution( node.variable, dataVec); 
  node.set_marginal_distribution(dist);
  return node;
}

/**
 *
 */
inline Node& createObsNode( std::shared_ptr<Graph> graph, plSymbol& var, int position,
                            DistPtr dist, Label2Index &label2Index ) {
  vertex_t vertexId = boost::add_vertex(*graph); // adds a new Node to the graph and returns the newly added node's index.
  Node &newNode = (*graph)[vertexId];
  newNode.set_index(vertexId).set_graph(graph).set_variable(var)
      .set_marginal_distribution(dist).set_position(position);
  label2Index[var.name()] = vertexId;
  return newNode;
}

/**
 *
 */
inline Node& createObsNode( std::shared_ptr<Graph> graph, plSymbol& var, 
                            DataVecPtr dataVec, int position, Label2Index &label2Index ) {
  vertex_t vertexId = boost::add_vertex(*graph); // adds a new Node to the graph and returns the newly added node's index.
  Node &newNode = (*graph)[vertexId];
  auto dist = create_emp_distribution( var, *dataVec);
  newNode.set_marginal_distribution(dist);
  newNode.dataVec = dataVec;
  newNode.set_index(vertexId).set_graph(graph).set_variable(var)//.set_data_vec(dataVec)
     .set_position(position);
  label2Index[var.name()] = vertexId;
  newNode.dataVec = dataVec;
  return newNode;
}

//////////////////////////////////////
inline Node& createLatentNode( std::shared_ptr<Graph> graph,
                               plSymbol& var,
                               JointDist& jointDist,
                               Label2Index &label2Index ) {
  vertex_t vertexId = boost::add_vertex(*graph); // adds a new Node to the graph and returns the newly added node's index.
  Node &newNode = (*graph)[vertexId];
  auto vars = jointDist.get_variables();
  newNode.set_index(vertexId).set_graph(graph).set_variable(var)
      .set_local_indexes(++vars.begin(), vars.end(), label2Index)
      .set_joint_distribution(jointDist)
      .set_position().update_level();
  label2Index[var.name()] = vertexId;
  return newNode;
}

inline Node& createLatentNode( std::shared_ptr<Graph> graph,
                               plSymbol& var,
                               CondObsDistPtr cndObsDist,
                               std::vector<CndDistPtr>& cdist,
                               Label2Index &label2Index ) {
  vertex_t vertexId = boost::add_vertex(*graph); // adds a new Node to the graph and returns the newly added node's index.
  Node &newNode = (*graph)[vertexId];
  //newNode.set_cnd_obs_vec(cndObsDist);
  newNode.cndObsDist = cndObsDist;
  newNode.cndChildrenDists = cdist;
  auto children_vars = newNode.get_children_variables();
   newNode.marginalDist = create_emp_distribution( var, *cndObsDist);
  newNode.set_index(vertexId).set_graph(graph).set_variable(var)
      .set_local_indexes(children_vars.begin(), children_vars.end(), label2Index)
      .set_position().update_level();
  return newNode;
}

///////////////////////////////////////////////////////////////////
inline Node& create_latent_node (std::shared_ptr<Graph> graph,
                                 const unsigned &cardinality,
                                 const std::string &label = "",
                                 const unsigned &position = -1,
                                 const unsigned &level = -1 ) {
  vertex_t vertexId = boost::add_vertex(*graph);
  plSymbol var( label, plIntegerType(0, cardinality - 1) );
  Node &node = (*graph)[vertexId];
  node.set_index(vertexId).set_graph(graph).set_variable(var)//.set_data_vec(dataVec)
      .set_position(position).set_level(level);
  return node;
}

inline Node& create_graph_node (std::shared_ptr<Graph> graph,
                                 const unsigned &cardinality,
                                 const std::string &label = "",
                                 const unsigned &position = -1,
                                 const unsigned &level = -1 ) {

  vertex_t vertexId = boost::add_vertex(*graph);

  plSymbol var( label, plIntegerType(0, cardinality - 1) );
  Node &node = (*graph)[vertexId];

  node.set_index(vertexId).set_graph(graph).set_variable(var)//.set_data_vec(dataVec)
      .set_position(position).set_level(level);
  return node;
}


} // namespace samogwas ends here.

/****************************************************************************************/
#endif // NOVO_GRAPH_HPP
