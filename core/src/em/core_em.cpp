
#include "em/core_em.hpp"
#include "utils/matrix_utils.hpp"

#include <cmath>
#include <pl.h> // ProBT library
#include <vector>
#include <memory>
#include "graph/graph.hpp"

#include "utils/random_tools.hpp"

namespace samogwas {

void fill_vector_with_random_floats( std::vector<double>& vec,
                                     // std::default_random_engine& generator,
                                     double lower,
                                     double upper ) {
  std::uniform_real_distribution<double> distribution (lower, upper);
  for ( auto it = vec.begin(); it != vec.end(); it++) {
    *it = distribution(generator);
  }
}

/////////////////////////////////////////////////////////////////////////////

std::shared_ptr<MultiMixtureModel> MultiMixtureEM::run_preset(const Graph& graph,
                                                              const Matrix& dataTable,
                                                              const RealVec& tabZ,
                                                              const std::vector<RealMat>& cndTabs,
                                                              Node& latentNode,
                                                              const double threshold,
                                                              const unsigned iterMax) {

  auto vars = latentNode.get_children_variables();
  auto latentVar = latentNode.variable;
  auto nbrElems = dataTable.size();

  std::shared_ptr<MultiMixtureModel> max_model;
  if (this->model) this->model.reset();
  this->model = std::make_shared<MultiMixtureModel>();
  this->model->set_params(tabZ, cndTabs);
  this->model->delta = std::make_shared<RealMat>();
  this->model->delta->reserve(nbrElems);
  for (size_t i = 0; i < nbrElems; ++i) {
    this->model->delta->push_back(std::vector<double>(latentVar.cardinality(), 0.0));
    fill_vector_with_random_floats(this->model->delta->at(i),
                                   // generator,
                                   1, 2); // (1,2) instead of (0,1) to avoid zeros
    normalize_vals(this->model->delta->at(i));
  }

  double prev_llh = threshold;
  for (unsigned step = 0; step < iterMax; ++step) {
    E_Step(dataTable);
    M_Step(dataTable);
    double curr_llk = this->log_likelihood(dataTable, true);
    if (std::abs(curr_llk - prev_llh) < std::abs(threshold)) {
      break;
    }
    prev_llh = curr_llk;
  }
  return this->model;
}

std::shared_ptr<MultiMixtureModel> MultiMixtureEM::run(const Graph& graph,
                                                       const Matrix& dataTable,
                                                       Node& latentNode,
                                                       const double threshold,
                                                       const unsigned iterMax) {
  auto vars = latentNode.get_children_variables();
  auto latentVar = latentNode.variable;
  auto nbrElems = dataTable.size();

  std::shared_ptr<MultiMixtureModel> max_model;
  init_params(latentVar, vars, nbrElems);

  double prev_llh = threshold;
  for (unsigned step = 0; step < iterMax; ++step) {
    E_Step(dataTable);
    M_Step(dataTable);
    double curr_llk = this->log_likelihood(dataTable, true);
    if (std::abs(curr_llk - prev_llh) < std::abs(threshold)) {
      break;
    }
    prev_llh = curr_llk;
  }
  return this->model;
}

void MultiMixtureEM::init_params(const RandomVariable& latent,
                                 const Variables vars,
                                 const size_t nbrElems) {
  if (!this->model) {
    this->model = std::make_shared<MultiMixtureModel>();
  } else {
    this->model.reset();
    this->model = std::make_shared<MultiMixtureModel>();
  }
  this->model->randomize_parameters(latent, vars, nbrElems// , this->generator
                                    ); // werid, todo
}

double MultiMixtureModel::log_likelihood(const Matrix& dat,
                                         const bool with_label) {
  auto nbrElems = dat.size();
  auto latentCard = probZ->size();
  auto nbrChild = cndProbs->at(0).size();
  double llh = 0.0;

  if (with_label) {
    for (size_t i = 0; i < nbrElems; ++i) {
    //   auto y = dat[i][0];
    //   llh += mylog(this->probZ->at(y));
    //   for (size_t X = 0; X < nbrChild; ++X) {
    //     auto x = dat[i][X+1];
    //     llh += mylog(cndProbs->at(y).at(X).at(x));
    //   }
    }
  }
  // else {
  for (size_t i = 0; i < nbrElems; ++i) {
    double llh_i = 0.0;
    for (size_t y = 0; y < latentCard; ++y) {
      double llh_iy = mylog( probZ->at(y));
      for (size_t X = 0; X < nbrChild; ++X) {
        auto x = dat[i][X+1];
        llh_iy += mylog(cndProbs->at(y).at(X).at(x));
      }
      llh_iy = exp(llh_iy);
      llh_i += llh_iy;
    }
    llh += mylog(llh_i);
  }
  //  }


  return llh;
}

void MultiMixtureEM::E_Step(const Matrix& dat) {
  auto nbrElems = dat.size();
  auto latentCard = this->model->probZ->size();
  auto nbrChild = this->model->cndProbs->at(0).size();
  for (size_t i = 0; i < nbrElems; ++i) {
    for (size_t y = 0; y < latentCard; ++y) {
      this->model->delta->at(i)[y] = mylog(model->probZ->at(y));
      for (size_t X = 0; X < nbrChild; ++X) {
        auto x = dat[i][X+1];
        this->model->delta->at(i)[y] += mylog(this->model->cndProbs->at(y).at(X).at(x));
      }
      this->model->delta->at(i)[y] = exp(this->model->delta->at(i)[y]);
    }
    normalize_vals(this->model->delta->at(i));
  }
}

void MultiMixtureEM::M_Step(const Matrix& dat) {
  auto latentCard = model->probZ->size();
  auto nbrElems = dat.size();
  auto nbrChild = model->cndProbs->at(0).size();
  assert(ncols(dat) == (nbrChild+1));

  for (size_t y = 0; y < latentCard; ++y) {
    model->probZ->at(y) = 0.0;
    reset_values_matrix(this->model->cndProbs->at(y), 0.0);
    for (size_t i = 0; i < nbrElems; ++i) {
      model->probZ->at(y) += this->model->delta->at(i)[y];
      for ( size_t X = 0; X < nbrChild; ++X) {
        auto x = dat[i][X+1];
        this->model->cndProbs->at(y)[X][x] += this->model->delta->at(i)[y];
      }
    }
    for ( size_t X = 0; X < nbrChild; ++X) {
      normalize_vals(this->model->cndProbs->at(y)[X]);
    }
  }
  normalize_vals(*this->model->probZ);
}

void MultiMixtureModel::randomize_parameters(const RandomVariable& latent,
                                             const Variables vars,
                                             const size_t nbrElems// ,
                                             // std::default_random_engine& generator
                                             ) {

  probZ = std::make_shared<RealVec>(latent.cardinality(), 0.0);
  fill_vector_with_random_floats(*probZ, // generator,
                                 1, 2); // (1,2) instead of (0,1) to avoid zeros
  normalize_vals(*probZ);

  cndProbs = std::make_shared<std::vector<RealMat>>(); // Z -> X -->
  cndProbs->reserve(latent.cardinality());
  for (size_t i = 0; i < latent.cardinality(); ++i) { // Z=z
    cndProbs->push_back(RealMat());
    for (size_t j = 0; j < vars.size(); ++j) { // X
      cndProbs->at(i).reserve(vars.size());
      cndProbs->at(i).push_back(RealVec(vars[j].cardinality(), 0.0));
      fill_vector_with_random_floats(cndProbs->at(i)[j],
                                    // generator,
                                     1, 2); // (1,2) instead of (0,1) to avoid zeros
      normalize_vals(cndProbs->at(i)[j]);
    }
  }

  delta = std::make_shared<RealMat>();
  delta->reserve(nbrElems);
  for (size_t i = 0; i < nbrElems; ++i) {
    delta->push_back(std::vector<double>(latent.cardinality(), 0.0));
    fill_vector_with_random_floats(delta->at(i),
                                   // generator,
                                   1, 2); // (1,2) instead of (0,1) to avoid zeros
    normalize_vals(delta->at(i));
  }

  this->update_dists();
}


CoreMultiEM::MatrixPtr CoreMultiEM::create_data_table( const Graph& graph,
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
  // return matrix;
  return transpose(matrix);
}

std::shared_ptr<MultiMixtureModel> CoreMultiEM::run(const Graph& graph,
                        Node& latentNode,
                        const double threshold,
                        const unsigned iterMax) {
  auto vars = latentNode.get_children_variables();
  auto latentVar = latentNode.variable;

  auto indexes = latentNode.get_children_global_indexes();
  auto dataTable = create_data_table( graph, indexes );

  max_llh = -std::numeric_limits<double>::max();

  for (size_t r = 0; r < nbrRestarts; ++r) {
    auto em = std::make_shared<MultiMixtureEM>(this->seed);
    auto model = em->run(graph, *dataTable, latentNode, threshold, iterMax);
    auto llh = em->log_likelihood(*dataTable, true);
    if (llh > max_llh) { // other thing here
      max_model = model;
      max_llh = llh;
    }
  }
  return max_model;
}

CoreMultiEM::MatrixPtr CoreMultiEM::transpose(MatrixPtr mat ) {
  size_t rows = samogwas::nrows(*mat), cols = samogwas::ncols(*mat);
  auto matrix = std::make_shared<Matrix>( cols, std::vector<int>(rows) );

  for (unsigned row = 0; row < rows; row++) {
    for (unsigned col = 0; col < cols; col++) {
      matrix->at(col).at(row) = mat->at(row).at(col);
    }
  }

  return matrix;
}

void MultiMixtureModel::print() const {
  auto latentCard = probZ->size();
  auto nbrChild = cndProbs->at(0).size();
  std::cout << "1-nbrChild: " << nbrChild << std::endl;
  std::cout << "marginal: ";
  for (size_t y = 0; y < latentCard; ++y) {
    std::cout << "P(Y=" << y << "): " << this->probZ->at(y) << ", ";
  }
  std::cout << std::endl;

  std::cout << "conditional:";
  for (unsigned y = 0; y < latentCard; ++y) {
    std::cout << "\nZ = " << y << ": \n";
    for (unsigned X = 0; X < nbrChild; ++X) {
      auto xCard = this->cndProbs->at(y).at(X).size();
      for (unsigned x = 0; x < xCard; ++x) {
        printf("P(X_%u=%u|Z=%u) = %f, ", X, x, y, this->cndProbs->at(y).at(X).at(x));
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }
  std::cout << std::endl << std::endl;
}


} //
