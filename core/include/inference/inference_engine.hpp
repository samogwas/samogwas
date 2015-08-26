
#include <graph/graph.hpp>

namespace samogwas {

struct InferenceFLTM {
  InferenceFLTM(const Graph& g);

  void compile_evidence(const int option);

  double log_likelihood(const std::vector<int>& sample) const;
  //double log_likelihood(const std::vector<std::vector<int>>& sample) const;
  double log_likelihood(const std::vector<std::vector<int>>& sample) const;

  double bic_score(const std::vector<std::vector<int>>& sample,
                   bool recompile = false, int option = 0);

  virtual double model_complexity(const std::vector<std::vector<int>>& sample) const;

 private:
  long number_of_free_parameters() const;

 private:
  plJointDistribution jointDist;
  Variables observedVariables;
  plDistribution ovJointDist;

};

}
