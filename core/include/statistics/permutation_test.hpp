/****************************************************************************************
 * file: permutation_test.hpp
 * description: this module provides a generic procedure dedicated to the correction for multiple tests.
 * ************ the distribution of the statistic under the null hypothesis is obtained by permutations.
 *
 * @author: duc-thanh phan siolag161 (thanh.phan@outlook.com), under the supervision of christine sinoquet
 * @date: 25/06/2014

 ***************************************************************************************/
#ifndef stats_permutation_test_hpp
#define stats_permutation_test_hpp

#include <boost/random.hpp> // boost::mt19937, boost::uniform_int
#include <random> // std::random_shuffle

#include <cmath>
#include <chrono> // std::chrono::system_clock
#include <algorithm> // std::min, std::max
#include "statistics/association_test.hpp"
#include "graph/graph.hpp"
namespace samogwas
{

struct CollectionPermute {
  CollectionPermute( unsigned long seed = 1 ) {
    rng.seed(seed);
  }

  static unsigned long currentTime() {
    unsigned long time =
        std::chrono::system_clock::now().time_since_epoch() /
           std::chrono::nanoseconds(1);
    return time;
  }
  struct Rand: std::unary_function<unsigned, unsigned> {
    boost::mt19937 &_state;
    unsigned operator()(unsigned i) {
      boost::uniform_int<> rng(0, i - 1);
      return rng(_state);
    }
    Rand(boost::mt19937 &state) : _state(state) {}
  };

  template<typename VecT>
  void operator()(VecT& vec, boost::mt19937 &state) {
    Rand rand(state);
    std::random_shuffle(vec.begin(), vec.end());
  }

  template<typename VecT>
  void operator()(VecT& vec) {
    Rand rand(CollectionPermute::rng);
    std::random_shuffle(vec.begin(), vec.end(), rand );
  }
 private:
  boost::mt19937 rng;
};


//////////////////////////////////////////////////////////////////////////////////
// returns the p-value of observing a value less than or equal to v
// this value is sampled from the true theoretical distribution d and dist is an empirical sample of d.
template<typename T>
double p_value( const T v, const std::vector<T>& dist ) {
  double count = 0.0;
  for (auto& val: dist ) {
    if ( val < v) ++count;
  }
  return count / dist.size();
}
//////////////////////////////////////////////////////////////////////////////////
template< class Vector >
void permutation_procedure( std::vector<double> &distri,
                            std::vector<double> &pvals,
                            std::shared_ptr<StatTest> statTest,
                            const Graph& graph,
                            const Vector &yData,
                            const std::vector<size_t> &xCandidates,
                            const std::vector<int> &xCardinalities,
                            const int yCardinality,
                            const int permu = 2) {
  assert(xCardinalities.size() == xCandidates.size());

  size_t nvars = xCandidates.size();
  pvals.resize(nvars*2, 0.0);

  for ( size_t c = 0; c < nvars; ++c) {
    size_t var = xCandidates[c];
    pvals[2*c] = statTest->execute( // xData.at(var)
        *graph[var].dataVec,
        yData,
        xCardinalities[c], yCardinality );
  }

  if ( permu > 0 ) {
    Vector localYData = yData;
    distri.resize(permu, 2.0);
    CollectionPermute permutate;

    // #pragma omp parallel for
    for ( int p = 0; p < permu; ++p ) {
      //#pragma omp critical
      permutate(localYData);

      for ( size_t c = 0; c < nvars; ++c) {
        size_t var = xCandidates[c];
        double pval = statTest->execute( *graph[var].dataVec, localYData,
                                         xCardinalities[c], yCardinality );

        distri[p] = std::min( distri[p], pval );
      }
    }
    for ( size_t c = 0; c < nvars; ++c) {
      size_t var = xCandidates[c];
      pvals[2*c+1]= p_value( pvals[2*c], distri );
    }
  }
} //

} // namespace samogwas ends here.


/****************************************************************************************/
#endif //samogwas_permutation_test_hpp
