
#include <cmath>
#include <pl.h> // ProBT library
#include <vector>
#include <memory>
#include "graph/graph.hpp"
#include "utils/matrix_utils.hpp"
#include "utils/random_tools.hpp"

/** Interface common to by any type of EM underlying algorithm.
 *
 */
namespace samogwas {

inline void normalize_vals(std::vector<double>& prob) {
  double s = 0;
  for (auto& v: prob) s+=v;
  for (auto it = prob.begin(); it != prob.end(); ++it) {
    *it /= s;
  }
}


inline const double mylog(const double r) {
  return r <= 0 ? 0 : log(r);
}

inline void reset_values_matrix(std::vector<std::vector<double>>& m, double v = 0.0) {
  size_t nbr_rows = nrows(m), nbr_cols = ncols(m);
  for (size_t r = 0; r < nbr_rows; ++r) {
    for (size_t c = 0; c < nbr_cols; ++c) {
      m[r][c] = v;
    }
  }
}

struct MultiMixtureModel {
  typedef std::vector< std::vector<int> > Matrix;
  typedef std::shared_ptr<Matrix> MatrixPtr;
  typedef plMatrixDataDescriptor<int> DataDesc;
  enum ImputationType { ARGMAX = 0, DRAW };
  typedef size_t Cardinality;
  typedef size_t Size;
  typedef std::vector<double> RealVec;
  typedef std::vector<std::vector<double>> RealMat;
  typedef std::shared_ptr<RealVec> RealVecPtr;
  typedef std::shared_ptr<RealMat> RealMatPtr;

  MultiMixtureModel() {}

 public:
  // void set_params();
  void randomize_parameters(const RandomVariable& latent,
                            const Variables vars,
                            const size_t nbrElems
                            );

  double log_likelihood(const Matrix& dat, const bool with_label = false);

  void print() const;

  size_t num_vars() const {
    return num_children() + 1;
  }

  size_t num_children() const {
    return cndProbs->at(0).size();
  }

  size_t latent_cardinality() const {
    return probZ->size();
  }

  void draw(std::vector<int>& vals) const {
    vals.resize( num_vars(), 0 );
    vals[0] = (*latDist)(generator);
    for (size_t X = 0; X < num_children(); ++X) {
      vals[X+1] = cndDists->at(vals[0])[X](generator);
    }
  }

  void init_model(const RandomVariable& latent,
                  const Variables& vars,
                  const size_t nbrElems) {

    if (probZ) probZ.reset();
    probZ = std::make_shared<RealVec>(latent.cardinality(), 0.0);

    if (cndProbs) cndProbs.reset();
    cndProbs = std::make_shared<std::vector<RealMat>>(); // Z -> X -->
    cndProbs->reserve(latent.cardinality());
    for (size_t i = 0; i < latent.cardinality(); ++i) { // Z=z
      cndProbs->push_back(RealMat());
      for (size_t j = 0; j < vars.size(); ++j) { // X
        cndProbs->at(i).reserve(vars.size());
        cndProbs->at(i).push_back(RealVec(vars[j].cardinality(), 0.0));
      }
    }

    if (delta) delta.reset();
    delta = std::make_shared<RealMat>();
    delta->reserve(nbrElems);
    for (size_t i = 0; i < nbrElems; ++i) {
      delta->push_back(std::vector<double>(latent.cardinality(), 0.0));
    }
  }

  void estimate_params(const RandomVariable& latent,
                       const Variables& vars,
                       const Matrix& dataTab) {
    this->init_model(latent, vars, dataTab.size());
    for (size_t i = 0; i < dataTab.size(); ++i) {
      auto y = dataTab.at(i).at(0);
      this->probZ->at(y) += 1.0;

      for (unsigned X = 0; X < vars.size(); ++X) {
        auto x = dataTab.at(i).at(X+1);
        this->cndProbs->at(y).at(X).at(x) += 1.0;
      }
    }

    for (size_t y = 0; y < latent.cardinality(); ++y) {
      for (unsigned X = 0; X < vars.size(); ++X) {
        for (unsigned x = 0; x < vars[X].cardinality(); ++x) {
          this->cndProbs->at(y).at(X).at(x) /= this->probZ->at(y);
        }
      }
    }
    normalize_vals(*this->probZ);

    // delta
    for (size_t i = 0; i < dataTab.size(); ++i) {
      for (size_t y = 0; y < latent.cardinality(); ++y) {
        delta->at(i)[y] = mylog(probZ->at(y));
        for (size_t X = 0; X < vars.size(); ++X) {
          auto x = dataTab[i][X+1];
          delta->at(i)[y] += mylog(cndProbs->at(y).at(X).at(x));
        }
        delta->at(i)[y] = exp(delta->at(i)[y]);
      }
      normalize_vals(delta->at(i));
    }

    ///
    for (size_t y = 0; y < latent.cardinality(); ++y) {
      probZ->at(y) = 0.0;
      reset_values_matrix(this->cndProbs->at(y), 0.0);
      for (size_t i = 0; i < dataTab.size(); ++i) {
        probZ->at(y) += this->delta->at(i)[y];
        for ( size_t X = 0; X < vars.size(); ++X) {
          auto x = dataTab[i][X+1];
          this->cndProbs->at(y)[X][x] += this->delta->at(i)[y];
        }
      }
      for ( size_t X = 0; X < vars.size(); ++X) {
        normalize_vals(this->cndProbs->at(y)[X]);
      }
    }
    normalize_vals(*this->probZ);

  }

  void draw(std::vector<std::vector<int>>& vals, const size_t rows) const {
    vals.clear();
    vals.resize(0);
    vals.reserve(rows);
    for (size_t i = 0; i < rows; ++i) {
      std::vector<int> row;
      this->draw(row);
      vals.push_back(row);
    }
  }

  void set_params(const RealVec& tabZ,
                  const std::vector<RealMat>& cndTabs) {
    if (probZ) probZ.reset();
    probZ = std::make_shared<RealVec>(tabZ);

    if (cndProbs) cndProbs.reset();
    cndProbs = std::make_shared<std::vector<RealMat>>();
    cndProbs->reserve(cndTabs.size());
    for (size_t i = 0; i < cndTabs.size(); ++i) {
      cndProbs->push_back(RealMat());
      cndProbs->at(i).reserve(cndTabs[0].size());
      for (size_t j = 0; j < cndTabs[0].size(); ++j) {
        cndProbs->at(i).push_back(RealVec(cndTabs[i][j]));
      }
    }

    update_dists();
  }

 public:
  RealVecPtr probZ;
  std::shared_ptr<std::vector<RealMat>> cndProbs;
  RealMatPtr delta;

  void update_dists() {
    latDist.reset();
    latDist = std::make_shared<CatDist>(probZ->begin(), probZ->end());

    cndDists.reset();
    cndDists = std::make_shared<std::vector<std::vector<CatDist>>>();
    cndDists->reserve(latent_cardinality());
    for (size_t z = 0; z < latent_cardinality(); ++z) {
      //for (size_t )
      cndDists->push_back(std::vector<CatDist>());
      cndDists->at(z).reserve(num_children());
      for (size_t X = 0; X < num_children(); ++X) {
        cndDists->at(z).push_back(
            CatDist(cndProbs->at(z).at(X).begin(), cndProbs->at(z).at(X).end()));
      }
    }
  }

 private:
  typedef std::discrete_distribution<int> CatDist;
  typedef std::shared_ptr<CatDist> CatDistPtr;

  CatDistPtr latDist;
  std::shared_ptr<std::vector<std::vector<CatDist>>> cndDists;
};


// struct NaiveBayes {
//   typedef std::vector< std::vector<int> > Matrix;
//   typedef std::shared_ptr<Matrix> MatrixPtr;
//   typedef plMatrixDataDescriptor<int> DataDesc;
//   enum ImputationType { ARGMAX = 0, DRAW };
//   typedef size_t Cardinality;
//   typedef size_t Size;
//   typedef std::vector<double> RealVec;
//   typedef std::vector<std::vector<double>> RealMat;
//   typedef std::shared_ptr<RealVec> RealVecPtr;
//   typedef std::shared_ptr<RealMat> RealMatPtr;

//   virtual std::shared_ptr<MultiMixtureModel> run(const Graph& graph,
//                                                  const Matrix& dataTable,
//                                                  Node& latentNode) {
//     auto vars = latentNode.get_children_variables();
//     auto latentVar = latentNode.variable;
//     auto nbrElems = dataTable.size();
//     init_model(latentVar, vars, nbrElems);
//     for (size_t i = 0; i < nbrElems; ++i) {
//       auto y = dataTable->at(y)[0];
//       this->model->probZ->at(y) += 1.0;
//     }
//     // for (unsigned y = 0; y < model->latent_cardinality(); ++y) {
//     //   // this->model->probZ->at(y) =
//     //   unsigned count = 0;
//     //   for (unsigned i = 0; i < nbrElems; ++i) {
//     //     count +=
//     //   }
//     // }
//   }

//   void myprint() const {
//     this->model->print();
//   }

//   double log_likelihood(const Matrix& dat, bool recompute = false) {
//     return recompute ? this->model->log_likelihood(dat) : this->cached_llh;
//   }

//  private:
//   double cached_llh = 0.0;
//   std::shared_ptr<MultiMixtureModel> model;
// };


///////////////////////////////////////////////////////
struct MultiMixtureEM {

 public:
  typedef std::vector< std::vector<int> > Matrix;
  typedef std::shared_ptr<Matrix> MatrixPtr;
  typedef plMatrixDataDescriptor<int> DataDesc;
  enum ImputationType { ARGMAX = 0, DRAW };
  typedef size_t Cardinality;
  typedef size_t Size;
  typedef std::vector<double> RealVec;
  typedef std::vector<std::vector<double>> RealMat;
  typedef std::shared_ptr<RealVec> RealVecPtr;
  typedef std::shared_ptr<RealMat> RealMatPtr;

  MultiMixtureEM(int seed = 1)//: // generator(seed)
  {}

  virtual std::shared_ptr<MultiMixtureModel> run_preset( const Graph& graph,
                                                  const Matrix& dataTable,
                                                  const RealVec& tabZ,
                                                  const std::vector<RealMat>& cndTabs,
                                                  Node& latentNode,
                                                  const double threshold,
                                                  const unsigned iterMax = 100);
  
  virtual std::shared_ptr<MultiMixtureModel> run( const Graph& graph,
                                                  const Matrix& dataTable,
                                                  Node& latentNode,
                                                  const double threshold,
                                                  const unsigned iterMax = 100);

  void myprint() const {
    this->model->print();
  }

  double log_likelihood(const Matrix& dat, bool recompute = false) {
    return recompute ? this->model->log_likelihood(dat) : this->cached_llh;
  }

 private:
  void E_Step(const Matrix& dat);
  void M_Step(const Matrix& dat);

  void init_params(const RandomVariable& latent,
                    const Variables vars,
                    const size_t nbrElems);

 private:
  std::default_random_engine generator;
  double cached_llh = 0.0;
  std::shared_ptr<MultiMixtureModel> model;
};


///////////////////////////////////////////////////////////////////
struct CoreMultiEM {

 public:
  typedef std::vector< std::vector<int> > Matrix;
  typedef std::shared_ptr<Matrix> MatrixPtr;
  typedef plMatrixDataDescriptor<int> DataDesc;
  enum ImputationType { ARGMAX = 0, DRAW };
  typedef size_t Cardinality;
  typedef size_t Size;
  typedef std::vector<double> RealVec;
  typedef std::vector<std::vector<double>> RealMat;
  typedef std::shared_ptr<RealVec> RealVecPtr;
  typedef std::shared_ptr<RealMat> RealMatPtr;

  CoreMultiEM(unsigned nbrR, int mseed = 1): nbrRestarts(nbrR),
                                             seed(mseed) {}

  virtual std::shared_ptr<MultiMixtureModel> run( const Graph& graph,
                                                  Node& latentNode,
                                                  const double threshold,
                                                  const unsigned iterMax = 100);

  double log_likelihood() {
    return max_llh;
  }
  // void myprint() const {
  //   if (max_em) {sinit_s
  //     max_em->myprint();
  //   }
  // }

 private:
  MatrixPtr create_data_table( const Graph& graph, const std::vector<int>& indexes );
  MatrixPtr transpose(MatrixPtr mat );

 private:
  unsigned nbrRestarts;
  int seed;
  double max_llh;
  std::shared_ptr<MultiMixtureModel> max_model;

};


}
