#define BOOST_TEST_DYN_LINK

#include <memory>

#include <boost/test/unit_test.hpp>
#include <BoostTestTargetConfig.h>

#include <boost/test/unit_test.hpp>
#include <BoostTestTargetConfig.h>
 
#include "clustering/clustering.hpp"
#include "clustering/dbscan.hpp"
#include "fltm/fltm.hpp"

#include "test_tools/data_generation.hpp"
#include "em/multi_em.hpp"
#include "graph/graph.hpp"
#include <pl.h>

#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

void init()
{
  boost::log::core::get()->set_filter
      (
          boost::log::trivial::severity >= boost::log::trivial::info
       );
}


using namespace samogwas_test;
using namespace samogwas;

BOOST_AUTO_TEST_SUITE( EM_Test )

typedef plSymbol RandVar;
// Node& createLatentNode( GraphPtr g, plSymbol& var, Label2Index& l2i, const std::vector<int>& cluster );

BOOST_AUTO_TEST_CASE( Test_FLTM_DBSCAN ) {
  init();
  size_t nclusts = 3, ncols = 25;
  size_t N = 6, CARD = 3, MAX_POS = 5;
  int nrows = nclusts*N;
  std::vector<int> positions; for ( int i = 0; i < nrows; ++i ) positions.push_back(i);
  auto data = GenerateClusteredData( nclusts, N, CARD, ncols )();  
  auto graph = std::make_shared<Graph>();
  auto l2g = std::make_shared<std::vector<int>>(nrows,0);
  Label2Index lab2Idx;
  for (int i = 0; i < nrows; ++i) {
    (*l2g)[i]=i;
    plSymbol v(boost::lexical_cast<std::string>(i), plIntegerType(0, CARD-1));
    auto dataVec = std::make_shared<DataVec>(data->at(i));
    createObsNode(graph, v, dataVec, 12, lab2Idx);
  }

  auto criteria = std::make_shared<PositionCriteria>(std::make_shared<DataVec>(positions), MAX_POS);
  auto gdiss = std::make_shared<GraphMutInfoDissimilarity>(graph, l2g);
  auto dbscan = std::make_shared<DBSCAN>( gdiss, 2, 0.45 );

  FLTM_Params params; params.nbrSteps = 5; params.nbrRestarts = 2; params.emThres = 0.01; params.latentVarQualityThres = 0.2;
  FLTM fltm(params);
  auto cardF = std::make_shared<LinearCardinality>(1,1,2);
  fltm.execute(dbscan, cardF, graph);
}

BOOST_AUTO_TEST_SUITE_END()  //
