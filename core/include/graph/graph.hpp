/****************************************************************************************
 * File: Graph.hpp
 * Description: This module provides the structure for the FLTM model.
 *
 * @author: Duc-Thanh Phan siolag161 (thanh.phan@outlook.com), under the supervision of Christine Sinoquet
 * @date: 13/07/2014

 ***************************************************************************************/
#ifndef NOVO_GRAPH_HPP

#define NOVO_GRAPH_HPP
#include <memory>
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
typedef boost::adjacency_list< boost::vecS, boost::vecS, boost::directedS,
                               Node, boost::no_property> Graph;

typedef std::shared_ptr<Graph> GraphPtr;
typedef std::shared_ptr<JointDist> JointDistPtr;
typedef std::vector<double> CondObsDist;
typedef std::shared_ptr<CondObsDist> CondObsDistPtr;
typedef std::vector<int> DataVec;
typedef std::shared_ptr<std::vector<int>> DataVecPtr;
typedef plVariablesConjunction Variables;
/**
 * Node 
 */
struct Node {

  /****************************************************
   */
  IndexType index; 
  LabelType label; // cache for variable.name()

  /****************************************************
   */
  
  ~Node() { }
  Node(): position(-1), level(0) {}

  /**
   */
  Node& set_joint_distribution( const JointDist& jointDistri );

  typedef plComputableObjectList::const_iterator plCompIte;
  Node& set_marginal_distribution( plCompIte it) {
    auto dist = std::make_shared<plDistribution>(*it);
    return set_marginal_distribution(dist);
  }
  Node& set_marginal_distribution( DistPtr dist) { this->marginalDist = dist; return *this; }  
  Node& set_children_distributions( CondObsDistPtr dist);
  Node& set_children_distributions(plCompIte beg, plCompIte end);

  ///
  Variables get_children_variables() const {
    Variables vars;
    if (!cndChildrenDists.empty()) {
      for (auto &d: cndChildrenDists) {
        vars ^= d->get_left_variables()[0]; //
      }
    } else {
      for (int i = 0; i < nbr_children(); ++i) {
        auto idx = local2GlobalIdx(i);
        vars ^= (*graph)[idx].variable;
      }
    }
    return vars;
  }

  std::vector<int> get_children_global_indexes() const {
    std::vector<int> rs(this->nbr_children(), 0);
    for (int i = 0; i < nbr_children(); ++i) {
      rs[i] = local2GlobalIdx(i);
    }
    return rs;
  }

  Node& set_local_indexes( VarIter beg, VarIter endx, const Label2Index &label2Index);
  Node& set_position();
  Node& set_position(const int position) { this->position = position; return *this; }

  Node& set_level();

  Node& set_variable(plSymbol& var) { this->variable = var; return *this; }
  Node& set_graph(GraphPtr graph) { this->graph = graph; return *this; }
  Node& set_index(const int index) { this->index = index; return *this; }
  Node& set_data_vec(DataVecPtr dtv); 
  Node& set_cnd_obs_vec(CondObsDistPtr cndObsDist);
  double compute_prob(const int a) const;

  double compute_cond_prob_obs(const int val, const int obs) const;

  double compute_cond_prob( const Node& cNode, const int childVal, const int parentVal ) const;
  double compute_cond_prob( const int cIdx, const int childVal, const int parentVal ) const;
  double compute_cond_prob( const int cIdx, const plValues& cV, const plValues& pV ) const;

  Node& copy_data( const Node& n );
  
  IndexType getIndex() const {
    return index;
  }

  LabelType getLabel() const {
    return this->variable.name();
  }

  int nbr_children() const {
    return global2localIdx.size();    
  }

  int cardinality() const {
    return this->variable.cardinality();
  }

  int local2GlobalIdx(const int local) const {
    int global = -1;
    for (auto it = global2localIdx.begin(); it != global2localIdx.end(); ++it ) {
      if (it->second == local) {
        global = it->first;
        break;
      }
    }
    return global;
  }

  bool is_leaf() const { return level == 0; }

  bool is_parent_of( const Node& ne) const ;
  bool is_child_of( const Node& ne) const  { return ne.is_parent_of(*this); }

  // Node& set_children(JointDist& jointDist, Label2Index &label2Index);

  /****************************************************/  
  int position; // physical position on the genome
  int level; // indicates the level to which this node belongs.
  GraphPtr graph; // reference to its graph
  RandomVariable variable; // represents the underlying random variable.

  DistPtr marginalDist;

  DataVecPtr dataVec;
  CondObsDistPtr cndObsDist;

  std::vector<CndDistPtr> cndChildrenDists;

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
  for (int i = 0; i < dataVec.size(); ++i) {
    ++prob[dataVec[i]];
  }

  for (int i = 0; i < variable.cardinality(); ++i) {
    prob[i] /= dataVec.size();
  }

  auto dist = std::make_shared<plDistribution>(variable, prob);

  return dist;
  
}

inline DistPtr create_emp_distribution( plSymbol& variable, const std::vector<double>& cndDist) {
  std::vector<double>prob( variable.cardinality(), 0.0);
  int N = cndDist.size() / variable.cardinality();
  for (int i = 0; i < N; ++i) {
    for ( int j = 0; j < variable.cardinality(); ++j ) {
      prob[j] += cndDist[i*variable.cardinality()+j];
    }
  }

  for (int i = 0; i < variable.cardinality(); ++i) {
    prob[i] /= N;
  }
  return std::make_shared<plDistribution>(variable, prob);
  
}



//////////////////
inline Node& createNode( std::shared_ptr<Graph> graph,
                         const unsigned cardinality,
                         const std::string label = "",
                         const int position = -1,
                         const int level = -1) {
  
  vertex_t vertexId = boost::add_vertex(*graph); // adds a new Node to the graph and returns the newly added node's index.

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
      .set_position().set_level();
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
      .set_position().set_level();
  return newNode;
}


// inline Node& createLatentNode( std::shared_ptr<Graph> graph,
//                                plSymbol& var,
//                                JointDist& jointDist,
//                                CondObsDistPtr cndDist,                               
//                                Label2Index &label2Index ) {
//   vertex_t vertexId = boost::add_vertex(*graph); // adds a new Node to the graph and returns the newly added node's index.
//   Node &newNode = (*graph)[vertexId];  
//   newNode.set_index(vertexId).set_graph(graph).set_variable(var)
//       .set_position(label2Index).set_level(label2Index);
//   label2Index[var.name()] = vertexId;
//   newNode.cndObsDist = cndDist;
//   return newNode;
// }

// inline Node& createLatentNode( std::shared_ptr<Graph> graph,
//                                plSymbol& var,
//                                JointDist& jointDist,
//                                CondObsDistPtr cndDist,                               
//                                Label2Index &label2Index ) {
//   vertex_t vertexId = boost::add_vertex(*graph); // adds a new Node to the graph and returns the newly added node's index.
//   Node &newNode = (*graph)[vertexId];  
//   newNode.setIndex(vertexId).setGraph(graph).setVariable(var).set_joint_distribution(jointDist, label2Index)
//       .setPosition(label2Index).setLevel(label2Index);
//   label2Index[var.name()] = vertexId;
//   newNode.cndObsDist = cndDist;
//   return newNode;
// }

inline Node& createLatentNode( std::shared_ptr<Graph> graph,
                               const unsigned cardinality,
                               const std::string label = "",
                               const int position = -1, const int level = 1 ) {
  assert(level >= 1);
  return createNode(graph, cardinality, label, position, level );
}


inline Node& createLatentNode( std::shared_ptr<Graph> graph, plSymbol& var, int position = -1, const int level =1 ) {
  return createLatentNode(graph, var.cardinality(), var.name(), position, level);
}


////////////////////////////////////////

inline double compute_node_entropy( const Node& node)  {
  assert(node.marginalDist);
  return node.marginalDist->compute_shannon_entropy();
}

// typedef std::vector<int> DataVec;
// typedef std::shared_ptr<DataVec> DataVecPtr;

// inline double compute_joint_entropy_obs_obs( const Node& nodeA, const Node& nodeB,
//                                              DataVec& datA, DataVec& datB ) {
//   assert(datA.size() == datB.size());
//   std::vector<std::vector<int>> jointDist( nodeA.cardinality(),
//                                            std::vector<int>(nodeB.cardinality(), 0) );
//   for (size_t i = 0; i < datA.size();++i) {
//     auto vA = datA.at(i);
//     auto vB = datB.at(i);
//     ++jointDist[vA][vB];
//   }

//   double result = 0.0;
//   for ( auto& vec: jointDist ) {
//     for (auto v: vec) {
//       double prob = v*1.0/datA.size();
//       if (prob > 0)
//         result += -prob*log(prob);
//     }    
//   }
//   return result;
// }

// inline double compute_joint_entropy_child_parent( const Node& cNode, const Node& pNode ) {
//   // auto childIdx = pNode.getChildIndex(cNode.variable.name());
//   double rs = 0.0;
//   for (int p = 0; p < pNode.cardinality(); ++p) {
//     double pProb = pNode.compute_prob(p);
//     for (int c = 0; c < cNode.cardinality(); ++c) {
//       double cndProb = pNode.compute_cond_prob(cNode, c, p);
//       double prob = pProb*cndProb;
//       if (prob)
//         rs += -prob*log(prob);
//     }
//   }
//   return rs;
// }

// /////////////////////////////////////////////////////////////////////////////
// typedef std::vector<std::vector<double>> CndDistGivenO;
// inline double compute_joint_entropy_obs_latent( const Node& oNode, const Node& lNode,
//                                                 const DataVec& datObs, const CndDistGivenO& cndDist ) {

//   auto size = datObs.size();
//   double rs = 0.0;
//   for (int o = 0; o < size; ++o) {
//     for (int i = 0; i < lNode.cardinality();++i ) {
//       double prob = cndDist[o][i]*(datObs[o]==i);
//       if (prob)
//         rs += -prob*log(prob);
//     }
//   }

//   return rs;
// }


// /// to be modified
// inline double compute_joint_entropy( const Node& nodeA, const Node& nodeB,
//                                      DataVecPtr datA=nullptr, DataVecPtr datB=nullptr )  {
 
// }



////////////////////////
// struct ComputeNodeJointEntropy {
//   typedef std::vector<std::vector<double>> JointProbTab;
//   typedef std::vector<int> VecT;
//   typedef std::vector<double> ProbVecT;
//   typedef std::shared_ptr<VecT> VecPtr;
//   typedef std::shared_ptr<ProbVecT> ProVecPtr;
  
//   /////////////////////////////////////////////////////////////

//   double compute_leaf_leaf(const Node& nA, const Node& nB,
//                            const VecT& dVecA, const VecT& dVecB) const {
//     auto jt = create_joint_tab_leaf_leaf(nA,nB,dVecA,dVecB);
//     return compute_from_joint_tab(jt);
//   }

//   double compute_leaf_latent(const Node& nA, const Node& nB,
//                              const VecT& dVecA, const ProbVecT& dVecB) const {
//     auto jt = create_joint_tab_leaf_latent(nA,nB,dVecA,dVecB);
//     return compute_from_joint_tab(jt);
//   }

//   double compute_latent_latent(const Node& nA, const Node& nB,
//                                const ProbVecT& dVecA, const ProbVecT& dVecB) const {
//     auto jt = create_joint_tab_latent_latent(nA,nB,dVecA,dVecB);
//     return compute_from_joint_tab(jt);
//   }

//   //////////////  
//   static double compute_from_joint_tab(const JointProbTab& probTab) {
//     assert(probTab.size() > 0);
//     const int CARD_X = probTab.size(), CARD_Y = probTab[0].size();

//     double rs = 0.0;
//     for ( auto &vec: probTab ) {
//       for (auto &prob: vec ) {
//         if (prob)
//           rs += -prob*log(prob);
//       }
//     }
//     return rs;
//   }

//   static JointProbTab create_joint_tab_leaf_leaf(const Node& nA, const Node& nB,
//                                                  const VecT& dVecA, const VecT& dVecB ) {
//     int CARD_A = nA.cardinality(), CARD_B = nB.cardinality();

//     JointProbTab jointTab(CARD_A, std::vector<double>(CARD_B,0.0));
//     auto sz = dVecA.size();    
//     for (int i=0;i<sz;++i) {
//       int a = dVecA[i];
//       int b = dVecB[i];
//       jointTab[a][b] += 1;
//     }     
//     for (int a = 0; a < CARD_A; ++a)  
//       for (int b = 0; b < CARD_B; ++b)
//         jointTab[a][b] /= sz;
//     return jointTab;
//   }

//   static JointProbTab create_joint_tab_latent_latent( const Node& nA, const Node& nB,
//                                                     const ProbVecT& dVecA, const ProbVecT& dVecB) {
//     if (nA.level > nB.level) return create_joint_tab_latent_latent(nB,nA,dVecB,dVecA);
//     int CARD_A = nA.cardinality(), CARD_B = nB.cardinality();
//     JointProbTab jointTab(CARD_A, std::vector<double>(CARD_B,0.0));
//     auto sz = dVecA.size()/nA.cardinality();
//     if (nB.is_parent_of(nA)) {
//       for (int a = 0; a < CARD_A; ++a) {
//         for (int b = 0; b < CARD_B; ++b) {
//           jointTab[a][b] = nB.compute_cond_prob(nA,a,b)*nB.compute_prob(b);
//         }
//       }

//     } else {
//       for (int i=0;i<sz;++i) {
//         for (int a = 0; a < CARD_A; ++a) {
//           for (int b = 0; b < CARD_B; ++b) {
//             jointTab[a][b] += dVecB[i*CARD_B+b]*dVecA[i*CARD_A+a];
//           }
//         }
//       } // for observation

//       for (int a = 0; a < CARD_A; ++a) {
//         for (int b = 0; b < CARD_B; ++b) { 
//           jointTab[a][b] /= sz;
//         }
//       }
//     } // no relationship


//     return jointTab;
//   }

//   static JointProbTab create_joint_tab_leaf_latent( const Node& nA, const Node& nB,
//                                                     const VecT& dVecA, const ProbVecT& dVecB) {
//     int CARD_A = nA.cardinality(), CARD_B = nB.cardinality();
//     JointProbTab jointTab(CARD_A, std::vector<double>(CARD_B,0.0));
//     auto sz = dVecA.size();
//     if (nB.is_parent_of(nA)) {
//       for (int a = 0; a < CARD_A; ++a) {
//         for (int b = 0; b < CARD_B; ++b) { 
//           jointTab[a][b] = nB.compute_cond_prob(nA,a,b)*nB.compute_prob(b);
//         }
//       }
//     } else {
//       for (int i=0;i<sz;++i) {
//         int a = dVecA[i];         
//         for (int b = 0; b < CARD_B; ++b) {
//           jointTab[a][b] += dVecB[i*CARD_B+b];
//         }
//       }     
//       for (int a = 0; a < CARD_A; ++a) {
//         for (int b = 0; b < CARD_B; ++b) {
//           jointTab[a][b] /= sz;        
//         }
//       }
//     } // for
  
//     return jointTab;
//   }
       
  
  
//   };

/** <CS> Heterogeneous style. This provides Provides a convenient way to computes<CS> log(X) by taking care of the case where<CS when> X = 0
 *
 */
// template<typename T>
// double m_log( const T t) {
//   return t <= 0 ? 0.0 : std::log(t);
// }

} // namespace novo ends here.

/****************************************************************************************/
#endif // NOVO_GRAPH_HPP
