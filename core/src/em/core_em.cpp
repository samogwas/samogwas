
#include "em/core_em.hpp"
#include "utils/matrix_utils.hpp"

#include <cmath>
#include <pl.h> // ProBT library
#include <vector>
#include <memory>
#include "graph/graph.hpp"

namespace samogwas {

double MultiMixtureEM::run(const Graph& graph,
                           Node& latentNode,
                           const double threshold,
                           const unsigned iterMax) {
  auto vars = latentNode.get_children_variables();
  auto latentVar = latentNode.variable;

  auto indexes = latentNode.get_children_global_indexes();
  auto dataTable = create_data_table( graph, indexes );

  randomize_parameters(latentVar, vars, nrows(*dataTable));

  for (unsigned r = 0; r < nbrRestarts; ++r) {
    for (unsigned step = 0; step < iterMax; ++step) {
      E_Step();
      // M_Step();
    }
  }
}

void MultiMixtureEM::init_params() {
  if (probZ) probZ.reset();
  if (cndProbs) cndProbs.reset();
  if (delta) delta.reset();
}

void MultiMixtureEM::randomize_parameters(const RandomVariable& latent,
                                          const Variables vars,
                                          const size_t nbrElems) {
  init_params();

  probZ = std::make_shared<RealVec>(latent.cardinality(), 0.0);
  fill_vector_with_random_floats(*probZ, this->generator, 1, 2); // (1,2) instead of (0,1) to avoid zeros
  normalize_vals(*probZ);

  cndProbs = std::make_shared<std::vector<RealMat>>();
  //auto cardX = vars[0].cardinality();
  cndProbs->reserve(latent.cardinality());
  for (size_t i = 0; i < latent.cardinality(); ++i) { // Z=z
    cndProbs->push_back(RealMat());
    for (int j = 0; j < vars.size(); ++j) { // X
      cndProbs->at(i).reserve(vars.size());
      cndProbs->at(i).push_back(RealVec(vars[j].cardinality(), 0.0));
      fill_vector_with_random_floats(cndProbs->at(i)[j],
                                     this->generator, 1, 2); // (1,2) instead of (0,1) to avoid zeros
      normalize_vals(cndProbs->at(i)[j]);
    }
  }

  delta = std::make_shared<RealMat>();
  delta->reserve(nbrElems);
  for (size_t i = 0; i < nbrElems; ++i) {
    delta->push_back(std::vector<double>(latent.cardinality(), 0.0));
    fill_vector_with_random_floats(delta->at(i),
                                     this->generator, 1, 2); // (1,2) instead of (0,1) to avoid zeros
    normalize_vals(delta->at(i));
  }
}

void MultiMixtureEM::normalize_vals(RealVec& prob) {
  double s = 0;
  for (auto& v: prob) s+=v;
  for (auto it = prob.begin(); it != prob.end(); ++it) {
    *it /= s;
  }
}

bool MultiMixtureEM::check_properties() const {
  double s = 0.0;
  for (auto v: *this->probZ) {
    s += v;
  }
  if ( s != 1.0 ) {
    return false;
  }
  for (auto Z: *this->cndProbs) {
    for (auto cndProb: Z) {
      double sum  = 0;
      for (auto v: cndProb) {
        sum += v;
      }
      if ( abs(sum-1.0) > 0.0001 ) return false;
    }
  }
  return true;
}

MultiMixtureEM::MatrixPtr MultiMixtureEM::create_data_table( const Graph& graph,
                                                             const std::vector<int>& indexes ) {
  auto nbrVars = indexes.size() + 1; // chgildren + (-1)
  auto matrix = std::make_shared<Matrix>();
  matrix->reserve(nbrVars);
  auto nbrInds = graph[0].dataVec->size();
  matrix->push_back(std::vector<int>( nbrInds, -1));

  for ( auto index: indexes ) {
    const Node& n = graph[index];
    matrix->push_back(*n.dataVec);
  }
  return transpose(matrix);
}

MultiMixtureEM::MatrixPtr MultiMixtureEM::transpose(MatrixPtr mat ) {
  size_t rows = samogwas::nrows(*mat), cols = samogwas::ncols(*mat);
  auto matrix = std::make_shared<Matrix>( cols, std::vector<int>(rows) );

  for (unsigned row = 0; row < rows; row++) {
    for (unsigned col = 0; col < cols; col++) {
      matrix->at(col).at(row) = mat->at(row).at(col);
    }
  }

  return matrix;
}

void MultiMixtureEM::E_Step() {

}

void MultiMixtureEM::M_Step() {

}

} //
