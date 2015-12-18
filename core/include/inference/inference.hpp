/****************************************************************************************
 * File: Graph.hpp
 * Description: This module provides the structure for the FLTM model.
 *
 * @author: Duc-Thanh Phan siolag161 (thanh.phan@outlook.com), under the supervision of Christine Sinoquet
 * @date: 13/07/2014

 ***************************************************************************************/
#ifndef SAMOGWAS_INFERENCE_HPP
#define SAMOGWAS_INFERENCE_HPP

#include <graph/graph.hpp>

#include <vector>
#include <memory>

namespace samogwas {

struct InferenceFLTM {
  typedef std::vector<int> IntVec;
  typedef std::shared_ptr<IntVec> IntVecPtr;
  typedef std::pair<plJtDistribution , std::vector<size_t>> TreeCouple;
  typedef plVariablesConjunction RandVars;

public:
  InferenceFLTM(const Graph& g);
   ~InferenceFLTM() {}
   double log_likelihood(const std::vector<IntVecPtr>& sample) const;
   double bic_score(const std::vector<IntVecPtr>& sample) const;
   double model_complexity(const std::vector<IntVecPtr>& sample) const;
   long number_of_free_parameters() const;

private:
  inline double m_log(double v) const {
    return v <= 0 ? 0 : log(v);
  }

private:
  std::vector<TreeCouple> get_separate_trees(const Graph& g) const;
  std::vector<size_t> get_root_nodes(const Graph& g) const;
  plJointDistribution construct_tree_joint_dist(const size_t root, const Graph& g) const;
  std::pair<RandVars, std::vector<size_t>> get_obs_vars(size_t root,  const samogwas::Graph& g) const;

private:
  const Graph& graph;
  std::vector<int> parent;
};



/******************************************************************************************/

}
#endif // NOVO_GRAPH_HPP
