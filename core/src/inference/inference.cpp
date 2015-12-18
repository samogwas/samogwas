#include <pl.h>


#include <boost/graph/visitors.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/lockfree/queue.hpp>

#include <graph/graph.hpp>
#include "inference/inference.hpp"


/******************************************************************************************/
namespace samogwas {

InferenceFLTM::InferenceFLTM(const Graph& g): graph(g) {
  parent.reserve(boost::num_vertices(g));
  for (int i = 0; i < boost::num_vertices(g); ++i) parent.push_back(-1);
  for ( auto eit = boost::edges(g); eit.first != eit.second; ++eit.first) {
    parent[boost::target(*eit.first, g)] = boost::source(*eit.first, g);
  }
}
std::vector<InferenceFLTM::TreeCouple> InferenceFLTM::get_separate_trees(const Graph& g) const {
  std::vector<TreeCouple> tree_dists;
  auto roots = get_root_nodes(g);

  for (auto root: roots) {
    auto tree_dist = construct_tree_joint_dist(root, g);
    plJunctionTree junc_tree(tree_dist.get_computable_object_list());
    auto obs_vars = get_obs_vars(root, g);
    auto evidence_dist = junc_tree.ask(obs_vars.first);
    tree_dists.push_back(std::make_pair(evidence_dist, obs_vars.second));

  }
  return tree_dists;
}

plJointDistribution InferenceFLTM::construct_tree_joint_dist(const size_t root, const Graph& g) const {
  plComputableObjectList jointDist;
  std::deque<size_t> queue;
  queue.push_back(root);
  typename boost::graph_traits <Graph>::out_edge_iterator ei, ei_end;
  RandVars vars;
  while(!queue.empty()) {
    auto node_id = queue.front(); queue.pop_front(); //
    const Node& node = g[node_id];
    vars ^= node.variable;

    if (node_id == root) {
      jointDist *= *node.marginalDist;
    }
    if (!node.is_leaf()) {
      for (boost::tie(ei, ei_end) = out_edges(node_id, g); ei != ei_end; ++ei) {
        auto target = boost::target(*ei, g);
        queue.push_back(target);
      }
      for (auto cndDist: node.cndChildrenDists) {
        jointDist *= *cndDist;
      }
    }
  }
  plJointDistribution dist(vars, jointDist);

  return dist;
}

std::pair<InferenceFLTM::RandVars, std::vector<size_t>> InferenceFLTM::get_obs_vars(size_t root,  const Graph& g) const {
  RandVars obs_vars;
  std::vector<size_t> var_indices;
  std::deque<size_t> queue;
  queue.push_back(root);

  typename boost::graph_traits <Graph>::out_edge_iterator ei, ei_end;
  while(!queue.empty()) {
    auto node_id = queue.front(); queue.pop_front(); //
    const Node& node = g[node_id];
    if (!node.is_leaf()) {
      for (boost::tie(ei, ei_end) = out_edges(node_id, g); ei != ei_end; ++ei) {
        auto source = boost::source(*ei, g);
        auto target = boost::target(*ei, g);
        queue.push_back(target);
      }
    } else {
      obs_vars ^= node.variable;
      var_indices.push_back(node.index);
    }
  }
  return std::make_pair(obs_vars, var_indices);

}


/**
 *
 */
std::vector<size_t> InferenceFLTM::get_root_nodes(const Graph& g) const {
  std::vector<size_t> roots;
  roots.reserve(boost::num_vertices(g));
  for (auto vp = boost::vertices(g); vp.first != vp.second; ++vp.first) {
    if (parent[*vp.first] == -1) {
      roots.push_back(*vp.first);
    }
  }
  roots.resize(roots.size());
  return roots;
}



double InferenceFLTM::log_likelihood(const std::vector<IntVecPtr>& mat) const {
  auto trees = get_separate_trees(graph);
  double r = 0.0;
  for (auto& tree_jd: trees) {
    std::vector<int> vec;
    for (auto idx: tree_jd.second) {
      vec.push_back(mat[idx]->at(0));
    }
    r += m_log(tree_jd.first.compute(vec)); // @todo
  }
  return r;
};



double InferenceFLTM::bic_score(const std::vector<IntVecPtr>& sample) const {
  auto complexity = this->model_complexity(sample);
  auto likelihood = this->log_likelihood(sample);
  std::cout << (-2*likelihood) << " " << complexity << std::endl;
  return -2*likelihood + complexity;
}


double InferenceFLTM::model_complexity(const std::vector<InferenceFLTM::IntVecPtr>& sample) const {
   auto param_num = this->number_of_free_parameters();
   return param_num*m_log(sample.size());
}

long InferenceFLTM::number_of_free_parameters() const {
  long param_num = 0;
  for (auto vp = boost::vertices(graph); vp.first != vp.second; ++vp.first) {
    const Node& n = graph[*vp.first];
    auto parent_idx = parent[*vp.first];
    if (parent_idx == -1) { // if free param
      param_num += n.variable.cardinality();
    } else {
      const Node& parent_node = graph[parent_idx];
      param_num += parent_node.variable.cardinality()*n.variable.cardinality();
    }
  }
  return param_num;
}

}
