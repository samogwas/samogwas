#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <BoostTestTargetConfig.h>

#include <random>
#include <chrono>

#include "clustering/clustering.hpp"
#include "clustering/dbscan.hpp"
#include "em/multi_em.hpp"
#include "graph/graph.hpp"
#include <pl.h>

#include "test_tools/data_generation.hpp"
#include "test_tools/dataload.hpp"

#include "em/core_em.hpp"

using namespace samogwas_test;
using namespace samogwas;

using ms = std::chrono::milliseconds;
using get_time = std::chrono::steady_clock;

BOOST_AUTO_TEST_SUITE( EM_Test )

typedef plSymbol RandVar;
CndDistPtr create_cndDist(Variable& l, Variable& r, std::vector<double>& tab) {
  return std::make_shared<plCndDistribution>(l, r, tab);
}

Node& createLatentNode( GraphPtr g, plSymbol& var, Label2Index& l2i, const std::vector<int>& cluster );

//////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE( Test_Multi_Mixture_1 ) {
  unsigned seed = 1;
  std::default_random_engine generator(seed);

  auto data = loadDataTable("../tests/data/em/dat_5.csv");
  unsigned nrows = data->size(), CARD = 3;
  auto graph = std::make_shared<samogwas::Graph>();
  auto l2g = std::make_shared<std::vector<int>>(nrows,0);
  Label2Index lab2Idx;
  for (int i = 0; i < nrows; ++i) {
    (*l2g)[i]=i;
    plSymbol v(boost::lexical_cast<std::string>(i), plIntegerType(0, CARD-1));
    auto dataVec = std::make_shared<DataVec>(*data->at(i));
    createObsNode(graph, v, dataVec, 12, lab2Idx);
  }

  std::vector<int> cluster;
  for ( size_t i = 0; i < nrows; ++i ) {
    cluster.push_back(i);
  }

  RandVar var(boost::lexical_cast<std::string>(boost::num_vertices(*graph)), plIntegerType(0,2));
  Node n = createLatentNode(graph, var, lab2Idx, cluster);
  CoreMultiEM myem(1, 0); // only randomized
  auto start = get_time::now();
  auto model = myem.run(*graph, n, 0.1);
}


MatrixPtr generate_data(// std::default_random_engine& generator,
                        const MultiMixtureModel& model,
                        const size_t nrows) {
  auto mat = std::make_shared<Matrix>(model.num_vars(),
                                      std::vector<int>(nrows, 0));

  for (size_t i = 0; i < nrows; ++i) {
    std::vector<int> vals;
    model.draw(vals);
    for (size_t v = 0; v < vals.size(); ++v) {
      mat->at(v).at(i) = vals[v];
    }
  }

  return mat;
}


BOOST_AUTO_TEST_CASE( Test_Multi_Mixture_3 ) {
  unsigned seed = 1;
  RealVec lat_probs { 0.3, 0.2, 0.5 };
  std::vector<std::vector<RealVec>> cnd_child_probs {
    {{0.7, 0.2, 0.1}, {0.3, 0.6, 0.1}}, // z=0
    {{0.3, 0.5, 0.2}, {0.2, 0.6, 0.2}}, // z=1,
    {{0.3, 0.4, 0.3}, {0.2, 0.6, 0.2}} // z=2,
  };
  MultiMixtureModel source_model;
  source_model.set_params(lat_probs, cnd_child_probs);
  std::vector<std::vector<int>> data;
  source_model.draw(data, 50000);

  unsigned nvars = 2;
  auto graph = std::make_shared<samogwas::Graph>();
  auto l2g = std::make_shared<std::vector<int>>(nvars, 0);
  Label2Index lab2Idx;
  for (int i = 1; i < nvars; ++i) {
    (*l2g)[i]=i;
    plSymbol v(boost::lexical_cast<std::string>(i), plIntegerType(0, 2));
    auto dataVec = std::make_shared<DataVec>(data.size(), 0);
    for (size_t j = 0; j < data.size(); ++j) {
      dataVec->at(j) = data[j][i+1];
    }
    createObsNode(graph, v, dataVec, 12, lab2Idx);
  }
  std::vector<int> cluster;
  for ( size_t i = 0; i < nvars; ++i ) {
    cluster.push_back(i);
  }
  RandVar var("Z", plIntegerType(0,2));
  Node n = createLatentNode(graph, var, lab2Idx, cluster);

  MultiMixtureEM em;
  auto predicted_model = em.run_preset(*graph, data, lat_probs, cnd_child_probs,
                                        n, 0.00000000001, 250);

}

//////////////////////////////////////////////////////////////
Node& createLatentNode( GraphPtr graph, plSymbol& var,
                        Label2Index& l2i,
                        const std::vector<int>& cluster ) {

 vertex_t vertexId = boost::add_vertex(*graph);
 Node& newNode = (*graph)[vertexId];
 plVariablesConjunction vars;
 for (auto idx: cluster) {
   Node& n = (*graph)[idx];
   vars ^= n.variable;
 }

 newNode.set_index(vertexId).set_graph(graph).set_variable(var)
     .set_local_indexes( vars.begin(), vars.end(), l2i);
 return newNode;
}




BOOST_AUTO_TEST_SUITE_END()  /// Test InfoTheo ends here
