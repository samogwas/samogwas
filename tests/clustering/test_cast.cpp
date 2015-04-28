// // #define BOOST_TEST_DYN_LINK

// // #include <boost/test/included/unit_test.hpp>
// // #include <BoostTestTargetConfig.h>

// // #include <iostream>
// // #include <memory>

// // #include "test_tools/data_generation.hpp"
// // #include "clustering/clustering.hpp"
// // #include "clustering/cast.hpp"

// // #include "graph/graph.hpp"
// // using namespace samogwas_test;

// // BOOST_AUTO_TEST_SUITE( Clustering_CAST_Test )
// // BOOST_AUTO_TEST_CASE( test_CAST_1 )
// // {
//   size_t nclusts = 3, ncols = 20;
//   size_t N = 5, CARD = 3, MAX_POS = 5;
//   int nrows = nclusts*N;
//   std::vector<int> positions; for ( int i = 0; i < nrows; ++i ) positions.push_back(i);
//   auto data = GenerateClusteredData( nclusts, N, CARD, ncols )();  

//   auto graph = std::make_shared<samogwas::Graph>();
//   auto l2g = std::make_shared<std::vector<int>>(nrows,0);
//   Label2Index lab2Idx;
//   for (int i = 0; i < nrows; ++i) {
//     (*l2g)[i]=i;
//     plSymbol v(boost::lexical_cast<std::string>(i), plIntegerType(0, CARD-1));
//     auto dataVec = std::make_shared<DataVec>(data->at(i));
//     createObsNode(graph, v, dataVec, 12, lab2Idx);
//   }

//   auto criteria = std::make_shared<PositionCriteria>(std::make_shared<DataVec>(positions), MAX_POS); 
//   auto gsimi = std::make_shared<GraphMutInfoSimilarity>(graph, l2g);
//   gsimi->set_criteria(criteria);

//   CAST cast( gsimi, 0.5 );  
//   Partition result = cast();
//   for ( int i = 0; i < nrows; ++i ) {
//     int expected_cluster = i / N;
//     BOOST_CHECK_EQUAL(result.getLabel(i), expected_cluster );
//   }  
// // }

// // BOOST_AUTO_TEST_SUITE_END()
