
#include <cmath>
#include <pl.h> // ProBT library
#include <vector>
#include <memory>
#include "graph/graph.hpp"

/** Interface common to by any type of EM underlying algorithm.
 *
 */
namespace samogwas {

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

  MultiMixtureEM(unsigned nbrR, int seed = 1): nbrRestarts(nbrR), generator(seed) {}

  virtual double run( const Graph& graph,
                      Node& latentNode,
                      const double threshold,
                      const unsigned iterMax = 100);

  static void fill_vector_with_random_floats( std::vector<double>& vec,
                                              std::default_random_engine& generator,
                                              double lower,
                                              double upper ) {
    std::uniform_real_distribution<double> distribution (lower, upper);
    for ( auto it = vec.begin(); it != vec.end(); it++){
      *it = distribution(generator);
    }
  }

  bool check_properties() const;

 private:
  void E_Step();
  void M_Step();

  void init_params();
  void randomize_parameters(const RandomVariable& latent, const Variables vars,
                                          const size_t nbrElems);
  MatrixPtr create_data_table( const Graph& graph, const std::vector<int>& indexes );
  MatrixPtr transpose(MatrixPtr mat );
  //  double scoreBIC( EMLearner& learner, plMatrixDataDescriptor<int>& dataDesc );
  void normalize_vals(RealVec& prob);

 private:
  unsigned nbrRestarts;
  std::default_random_engine generator;

  RealVecPtr probZ;
  std::shared_ptr<std::vector<RealMat>> cndProbs;
  RealMatPtr delta;
};


}
