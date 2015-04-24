/*=============================================================================
 * Product        : ProBT 
 * File           : plStructureLearner.h
 * Author         : David Raulo
 * Creation       : Tue Feb 24 11:42:50 2009
 *
 *=============================================================================
 *        (c) Copyright 2009, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */

/// \file plStructureLearner.h
/// Definition of the plStructureLearner class.

#ifndef PL_STRUCTURE_LEARNER_H_
#define PL_STRUCTURE_LEARNER_H_

#include <plConfig.h>
#include <plDLLExport.h>
#include <plVariablesConjunction.h>
#include <plDataDescriptor.h>
#include <plJointDistribution.h>
#include <plScores.h>
#include <plLearnObject.h>

//-----------------------------------------------------------------------------
/// This is the main interface of ProBT Structure Learning functionality.
/// Given a set of variables and a dataset, this class allows to discover the
/// dependancy structure of the variables.
class PL_DLL_API plStructureLearner: public plLearner
{
 public:
  /// An edge sequence as a vector of vertex plSymbol pairs.
  typedef std::vector< std::pair<plSymbol, plSymbol> > symbol_edge_list_t;


  /// Construct a structure learner that will operate on the given joint
  /// distribution as an initial structure. Note that the parameters of the
  /// initial joint distribution will not be used, only the structure.
  explicit plStructureLearner(const plJointDistribution& joint);

  /// Construct a structure learner that will operate on the given symbols
  /// and the given initial structure.
  explicit plStructureLearner(const plVariablesConjunction& variables, 
                              const symbol_edge_list_t& init_edges=symbol_edge_list_t());

  plStructureLearner(const plStructureLearner&);
  plStructureLearner& operator=(const plStructureLearner&);
  ~plStructureLearner();



 private:
  /// An edge sequence as a vector of vertex index pairs.
  typedef std::vector< std::pair<unsigned int, unsigned int> > edge_list_t;

  bool DMST_internal(const plEdgeScore& score,
                     std::vector<unsigned int>& vertex_order,
                     unsigned int root_index,
                     const edge_list_t& included_edges,
                     const edge_list_t& excluded_edges);

  void K2_internal(const plNodeScore& score,
                   const std::vector<unsigned int>& vertex_order,
                   const edge_list_t& included_edges,
                   const edge_list_t& excluded_edges,
                   unsigned int max_parents,
                   unsigned int max_parent_card);

  void GS_internal(const plNodeScore& score,
                   const edge_list_t& included_edges,
                   const edge_list_t& excluded_edges,
                   unsigned int max_parents,
                   unsigned int max_parent_card);

  std::vector<plSymbol> indices_to_symbols(const std::vector<unsigned int> &indices) const;
  
  std::vector<unsigned int> symbols_to_indices(const std::vector<plSymbol> &nodes) const;
  
  unsigned int symbol_to_index(const plSymbol &s) const;

  const plSymbol& index_to_symbol(unsigned int i) const;

  edge_list_t symbol_pairs_to_indice_pairs(const symbol_edge_list_t& edges) const;

 public:

  /// Learn the dependancy structure from the dataset using the Directed
  /// Minimum Spanning Tree algorithm.  If a structure was previously learned
  /// with this plStructureLearner, it is lost.  To access the result of this
  /// algorithm, use the get_joint_distribution() method.
  /// \param score the chosen score function ;
  /// \param[out] vertex_order the order of the vertices in the
  /// resulting tree (which is usefull as input to the K2 algorithm).
  /// \param root_node the root used for directing the edges ;
  /// \param included_edges mandatory edges (undirected) ;
  /// \param excluded_edges forbiden edges (undirected) ;
  /// Note that the included_edges and excluded_edges constraints can be
  /// incompatible with the choice of the tree root. 
  /// \return true if the included_edges and excluded_edges constraints
  /// could be respected given the chosen root_index.
  /// \sa plEdgeScore and its derived classes.
  bool DMST(const plEdgeScore& score,
            std::vector<plSymbol>& vertex_order,
            const plSymbol &root_node,
            const symbol_edge_list_t& included_edges = symbol_edge_list_t(),
            const symbol_edge_list_t& excluded_edges = symbol_edge_list_t());

  /// Learn the dependancy structure from the dataset using the Directed
  /// Minimum Spanning Tree algorithm.  If a structure was previously learned
  /// with this plStructureLearner, it is lost.  To access the result of this
  /// algorithm, use the get_joint_distribution() method.
  /// \param score the chosen score function
  /// \param root_node the root used for directing the edges ;
  /// \param included_edges mandatory edges (undirected) ;
  /// \param excluded_edges forbiden edges (undirected) ;
  /// Note that the included_edges and excluded_edges constraints can be
  /// incompatible with the choice of the tree root. 
  /// \return true if the included_edges and excluded_edges constraints
  /// could be respected given the chosen root_index.
  /// \sa plEdgeScore and its derived classes.
  bool DMST(const plEdgeScore& score,
            const plSymbol &root_node,
            const symbol_edge_list_t& included_edges = symbol_edge_list_t(),
            const symbol_edge_list_t& excluded_edges = symbol_edge_list_t());
  
  /// Learn the structure with the K2 algorithm. If a structure was previously
  /// learned with this plStructureLearner, it is used as a starting point.  To
  /// access the result of this algorithm, use the get_joint_distribution()
  /// method.
  /// \param score the chosen score function ;
  /// \param included_edges mandatory edges (directed) ;
  /// \param excluded_edges forbiden edges (directed) ;
  /// \param vertex_order the order constraint on the graph vertices ;
  /// \param max_parents the maximum number of parents per node
  /// \param max_parent_card maximum parents cardinality
  /// \sa plNodeScore and its derived classes.
  void K2(const plNodeScore& score,
          const std::vector<plSymbol>& vertex_order,
          const symbol_edge_list_t& included_edges = symbol_edge_list_t(),
          const symbol_edge_list_t& excluded_edges = symbol_edge_list_t(),
          unsigned int max_parents = 100,
          unsigned int max_parent_card = 100000);

  /// Learn the structure with the Greedy Search algorithm. If a structure was
  /// previously learned with this plStructureLearner, it is used as a starting
  /// point. To access the result of this algorithm, use the
  /// get_joint_distribution() method.
  /// \param score the chosen score function
  /// \param max_parents maximum number of parents
  /// \param max_parent_card maximum parents cardinality
  /// \param included_edges mandatory edges (directed);
  /// \param excluded_edges forbiden edges (directed);
  /// \sa plNodeScore and its derived classes.
  void GS(const plNodeScore& score,
          const symbol_edge_list_t& included_edges = symbol_edge_list_t(),
          const symbol_edge_list_t& excluded_edges = symbol_edge_list_t(),
          unsigned int max_parents = 100,
          unsigned int max_parent_card = 100000);
  
  /// Perform the parameter learning, and returns the joint distribution.
  /// \param data_desc the data descriptor (for learning the parameters).
  /// \return a joint distribution corresponding to the dependancy structure
  /// obtained with DMST(), K2() or GS() and the probability tables learned
  /// from the data with the Laplace succession rule.
    plJointDistribution
    get_joint_distribution(plDataDescriptor& data_desc) const;

  /** Returns the variables passed when the object was constructed.
   */
  plVariablesConjunction const & get_variables() const;

  /** Get the current structure edges */
  std::vector< std::pair<plSymbol, plSymbol> > get_edges() const;

  /** Set/unset verbose mode */
  void set_verbose(bool verbose);

 protected:
  virtual void Output(std::ostream &out) const;
 protected:
  struct Impl;
  Impl* pimpl_;

};

#endif //PL_STRUCTURE_LEARNER_H_

//-----------------------------------------------------------------------------
// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
