
#include <graph/graph.hpp>

namespace samogwas {

struct InferenceFLTM {
 public:
  virtual void compile_evidence(const int option) = 0;
  // virtual double log_likelihood(const std::vector<int>& sample) const = 0;
  virtual double log_likelihood(const std::vector<std::vector<int>>& sample) const = 0;
  virtual double bic_score(const std::vector<std::vector<int>>& sample,
                   bool recompile = false, int option = 0) = 0;

  virtual double model_complexity(const std::vector<std::vector<int>>& sample) const = 0;

  virtual long number_of_free_parameters() const = 0;
};

struct MCMCInferenceFLTM: public InferenceFLTM {
 public:
  MCMCInferenceFLTM(const Graph& g);

  virtual void compile_evidence(const int option);
  virtual double log_likelihood(const std::vector<std::vector<int>>& sample) const;
  virtual double bic_score(const std::vector<std::vector<int>>& sample,
                   bool recompile = false, int option = 0);

  virtual double model_complexity(const std::vector<std::vector<int>>& sample) const;

  virtual long number_of_free_parameters() const;

 private:
  plJointDistribution jointDist;
  Variables observedVariables;
  plDistribution ovJointDist;

};

/**
 *
 */
struct JTInferenceFLTM: public InferenceFLTM {

 public:
  JTInferenceFLTM(const Graph& g);

  virtual void compile_evidence(const int option);
  virtual double log_likelihood(const std::vector<std::vector<int>>& sample) const;
  virtual double bic_score(const std::vector<std::vector<int>>& sample,
                   bool recompile = false, int option = 0);

  virtual double model_complexity(const std::vector<std::vector<int>>& sample) const;

  virtual long number_of_free_parameters() const;

 private:
  plJointDistribution jointDist;
  Variables observedVariables;

  plJtDistribution ovJointDist;
  plJunctionTree jt;


};

/******************************************************************************************/



}
