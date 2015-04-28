#define BOOST_TEST_DYN_LINK

#include <boost/test/included/unit_test.hpp>
#include <BoostTestTargetConfig.h>

#include <memory>

#include "test_tools/data_generation.hpp"
#include "clustering/clustering.hpp"
#include "clustering/cast.hpp"
#include "clustering/dbscan.hpp"

#include "graph/graph.hpp"

using namespace samogwas_test;
using namespace samogwas;
BOOST_AUTO_TEST_SUITE( Clustering_Test )

BOOST_AUTO_TEST_CASE( test_CAST_1 )
{
  size_t nclusts = 3, ncols = 20;
  size_t N = 5, CARD = 3, MAX_POS = 5;
  int nrows = nclusts*N;
  std::vector<int> positions; for ( int i = 0; i < nrows; ++i ) positions.push_back(i);
  auto data = GenerateClusteredData( nclusts, N, CARD, ncols )();  

  auto graph = std::make_shared<samogwas::Graph>();
  auto l2g = std::make_shared<std::vector<int>>(nrows,0);
  Label2Index lab2Idx;
  for (int i = 0; i < nrows; ++i) {
    (*l2g)[i]=i;
    plSymbol v(boost::lexical_cast<std::string>(i), plIntegerType(0, CARD-1));
    auto dataVec = std::make_shared<DataVec>(data->at(i));
    createObsNode(graph, v, dataVec, 12, lab2Idx);
  }

  auto criteria = std::make_shared<PositionCriteria>(std::make_shared<DataVec>(positions), MAX_POS); 
  auto gsimi = std::make_shared<GraphMutInfoSimilarity>(graph, l2g);
  gsimi->set_criteria(criteria);

  CAST cast( gsimi, 0.5 );  
  Partition result = cast();
  for ( int i = 0; i < nrows; ++i ) {
    int expected_cluster = i / N;
    BOOST_CHECK_EQUAL(result.getLabel(i), expected_cluster );
  }
}

BOOST_AUTO_TEST_CASE( test_DBSCAN_1 )
{
  size_t nclusts = 3, ncols = 20;
  size_t N = 3, CARD = 3, MAX_POS = 5;
  int nrows = nclusts*N;
  std::vector<int> positions; for ( int i = 0; i < nrows; ++i ) positions.push_back(i);
  auto data = GenerateClusteredData( nclusts, N, CARD, ncols )();  

  int lim_nrows = 6;  Label2Index lim_lab2Idx;
  auto graph = std::make_shared<Graph>();
  auto l2g = std::make_shared<std::vector<int>>(nrows,0);
  auto lim_l2g = std::make_shared<std::vector<int>>(lim_nrows,0);

  Label2Index lab2Idx;
  for (int i = 0; i < nrows; ++i) {
    if (i < lim_nrows) {
      (*lim_l2g)[i]=i;
    }      
    (*l2g)[i]=i;
    plSymbol v(boost::lexical_cast<std::string>(i), plIntegerType(0, CARD-1));
    auto dataVec = std::make_shared<DataVec>(data->at(i));
    createObsNode(graph, v, dataVec, 12, lab2Idx);
  }

  auto criteria = std::make_shared<PositionCriteria>(std::make_shared<DataVec>(positions), MAX_POS);
  auto lim_gdiss = std::make_shared<GraphMutInfoDissimilarity>(graph, lim_l2g);

  DBSCAN lim_dbscan( lim_gdiss, 2, 0.45 );
  Partition result = lim_dbscan();
  for ( int i = 0; i < lim_nrows; ++i ) {
    int expected_cluster = i / N;
    BOOST_CHECK_EQUAL(result.getLabel(i), expected_cluster );
  }

  std::vector<int> lim_l2g_dat { 0,1,3,4,5,7 };
  auto lim_l2g_2 = std::make_shared<std::vector<int>>(lim_l2g_dat);
  auto lim_gdiss_2 = std::make_shared<GraphMutInfoDissimilarity>(graph, lim_l2g_2);
  DBSCAN lim_dbscan_2( lim_gdiss_2, 1, 0.45 );
  Partition result_2 = lim_dbscan_2();
  std::vector<int> expected_clusters {0,0,1,1,1,2};
  for ( int i = 0; i < lim_nrows; ++i ) {
     BOOST_CHECK_EQUAL(result_2.getLabel(i), expected_clusters[i] );
  }
}
 

BOOST_AUTO_TEST_SUITE_END()
