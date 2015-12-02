

#include <pl.h>

#include <vector>
#include <map>
#include <memory>
#include <string>
#include <omp.h>
#include <boost/graph/visitors.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/lockfree/queue.hpp>

#include <graph/graph.hpp>

#include "inference/inference_engine.hpp"
#include "global.hpp"

namespace samogwas {


typedef size_t VariableIndex;
typedef std::vector<VariableIndex> VarIndices;
typedef std::vector<VarIndices> VarIndicesByLevel;
VarIndicesByLevel get_var_indices_by_level( const samogwas::Graph& g );
VarIndices get_marginal_var_indices( const samogwas::Graph& g );

int get_max_level(const Graph& g);

std::vector<int> get_variable_parent( const samogwas::Graph& graph );

/**
 * Compile the inference using the FLTM graph, for legacy purposes
 **/
MCMCInferenceFLTM::MCMCInferenceFLTM(const Graph& graph) {
  // we move top-down to build the joint distribution
  plComputableObjectList jd;
  Variables vars;
  auto parentOf = get_variable_parent(graph);
  auto varIndicesByLevel = get_var_indices_by_level(graph);
  for (unsigned level = 0; level < varIndicesByLevel.size(); ++level) {
    for (auto& index: varIndicesByLevel[level]) {
      auto& node = graph[index];
      vars ^= node.variable;
      if (parentOf[index] == -1) {
        jd *= *(node.marginalDist);
      }
      for (auto cndDist: node.cndChildrenDists) {
        jd *= (*cndDist);
      }
      if (level == 0) {
        this->observedVariables ^= node.variable;
      }
    }
  }

  this->jointDist = plJointDistribution(vars, jd);
}
//////////////////////////////////////////////////////////
void MCMCInferenceFLTM::compile_evidence(const int option) {
  switch(option) {
    case 0:
      std::cout << "done building. now exact inferring...\n";
      this->ovJointDist = this->jointDist.ask(this->observedVariables);
      break;
    case 1:
      std::cout << "done building. now opt optimization (time)...\n";
      this->ovJointDist = this->jointDist.ask(this->observedVariables, PL_OPTIMIZE_UPDATE_TIME);
      break;
    case 2:
      std::cout << "done building. now opt optimization (memory)...\n";
      this->ovJointDist = this->jointDist.ask(this->observedVariables, PL_OPTIMIZE_MEMORY_USE);
      break;
    case 3:
      std::cout << "done building. now sampling with mcmc...\n";
      this->ovJointDist = this->jointDist.ask_mc_convergence(this->observedVariables, 0.001);
      break;
    default:
      break;
  }
  std::cout << "done sampling...\n";

}

/**
 */
double MCMCInferenceFLTM::model_complexity(const std::vector<std::vector<int>>& sample) const {

  auto n = sample.size(); // sample size
  auto k = this->number_of_free_parameters();
  std::cout << "n = " << n << ", k = " << k << std::endl;
  return k*log(n);
}

/**
 */
double MCMCInferenceFLTM::bic_score(const std::vector<std::vector<int>>& sample,
                                bool recompile, int option) {
  if (recompile) {
    this->compile_evidence(option);
  }

  double complexity = this->model_complexity(sample);
  double log_llh = this->log_likelihood(sample);

  double bic = -2*log_llh + complexity; // formula of the bic score
  return bic;
}

/**
 */
long MCMCInferenceFLTM::number_of_free_parameters() const {
  return jointDist.get_variables().size() - observedVariables.size();
}

/**
 */
double MCMCInferenceFLTM::log_likelihood(const std::vector<std::vector<int>>& sample) const {
  double log_llh = 0.0;
  auto nbr = sample.at(0).size();
  std::vector<int> vals(nbr, 0);

  for (size_t i = 0; i < sample.size(); ++i) {
    // std::cout << "i: " << i << std::endl;
    log_llh +=  ovJointDist.compute_log(sample[i]);
  }

  return log_llh;
}

VarIndicesByLevel get_var_indices_by_level( const samogwas::Graph& graph ) {
  int max_level = get_max_level(graph);

  VarIndicesByLevel indices(max_level+1, VarIndices());
  for ( auto vp = boost::vertices(graph); vp.first != vp.second; ++vp.first ) {
    auto vertex = *vp.first;
    auto &node = graph[vertex];
    int level = node.level;
    indices[level].push_back(vertex);
  }

  return indices;
}

// VarIndices get_marginal_var_indices( const samogwas::Graph& graph ) {
//   int max_level = get_max_level(graph);
//   VarIndices indices;
//   for ( auto vp = boost::vertices(graph); vp.first != vp.second; ++vp.first ) {
//     auto vertex = *vp.first;
//     auto &node = graph[vertex];
//     int level = node.level;
//     if (level == max_level) {
//       indices.push_back(vertex);
//     }
//   }

//   return indices;
// }

int get_max_level(const Graph& graph) {
  int max_level = -1;
  for ( auto vp = boost::vertices(graph); vp.first != vp.second; ++vp.first ) {
    auto vertex = *vp.first;
    auto &node = graph[vertex];
    max_level = std::max( node.level, max_level );
  }
  return max_level;
}


std::vector<int> get_variable_parent( const samogwas::Graph& graph ) {
  std::vector<int> parent( boost::num_vertices(graph), -1 );
  for ( auto ei = boost::edges(graph); ei.first != ei.second; ++ei.first ) {
    auto source = boost::source(*ei.first, graph);
    auto target = boost::target(*ei.first, graph);
    parent[target] = source;
  }

  return parent;
}

/******************************************************************************************/
/**
 * Compile the inference using the FLTM graph, for legacy purposes
 **/
JTInferenceFLTM::JTInferenceFLTM(const Graph& graph) {
  // we move top-down to build the joint distribution
  plComputableObjectList jd;
  Variables vars;
  auto parentOf = get_variable_parent(graph);
  auto varIndicesByLevel = get_var_indices_by_level(graph);
  for (unsigned level = 0; level < varIndicesByLevel.size(); ++level) {
    for (auto& index: varIndicesByLevel[level]) {
      auto& node = graph[index];
      vars ^= node.variable;
      if (parentOf[index] == -1) {
        jd *= *(node.marginalDist);
      }
      for (auto cndDist: node.cndChildrenDists) {
        jd *= (*cndDist);
      }
      if (level == 0) {
        this->observedVariables ^= node.variable;
      }
    }
  }

  this->jointDist = plJointDistribution(vars, jd);
  jt.set_decomposition(jointDist.get_computable_object_list());
}
//////////////////////////////////////////////////////////
void JTInferenceFLTM::compile_evidence(const int option) {
  std::cout << "begin compiling...\n";
  ovJointDist = jt.ask(observedVariables);
  // switch(option) {
    // case 0:
    //   std::cout << "done building. now exact inferring...\n";
    //   this->ovJointDist = this->jointDist.ask(this->observedVariables);
    //   break;
    // case 1:
    //   std::cout << "done building. now opt optimization (time)...\n";
    //   this->ovJointDist = this->jointDist.ask(this->observedVariables, PL_OPTIMIZE_UPDATE_TIME);
    //   break;
    // case 2:
    //   std::cout << "done building. now opt optimization (memory)...\n";
    //   this->ovJointDist = this->jointDist.ask(this->observedVariables, PL_OPTIMIZE_MEMORY_USE);
    //   break;
    // case 3:
    //   std::cout << "done building. now sampling with mcmc...\n";
    //   this->ovJointDist = this->jointDist.ask_mc_convergence(this->observedVariables, 0.001);
    //   break;
    // default:
    //   break;
    // }
  std::cout << "done compiling...\n";

}

/**
 */
double JTInferenceFLTM::model_complexity(const std::vector<std::vector<int>>& sample) const {

  auto n = sample.size(); // sample size
  auto k = this->number_of_free_parameters();
  std::cout << "n = " << n << ", k = " << k << std::endl;
  return k*log(n);
}

/**
 */
double JTInferenceFLTM::bic_score(const std::vector<std::vector<int>>& sample,
                                bool recompile, int option) {
  if (recompile) {
    this->compile_evidence(option);
  }

  double complexity = this->model_complexity(sample);
  double log_llh = this->log_likelihood(sample);

  double bic = -2*log_llh + complexity; // formula of the bic score
  return bic;
}

/**
 */
long JTInferenceFLTM::number_of_free_parameters() const {
  return jointDist.get_variables().size() - observedVariables.size();
}

/**
 */
double JTInferenceFLTM::log_likelihood(const std::vector<std::vector<int>>& sample) const {
  double log_llh = 0.0;
  auto nbr = sample.at(0).size();
  // std::vector<int> vals(nbr, 0);
  for (size_t i = 0; i < sample.size(); ++i) {
    double ll = ovJointDist.compute(sample[i]);
    if (ll>0)
      std::cout << i << ": " <<ll << std::endl;
    log_llh += ll;
  }

  return log_llh;
}

}
