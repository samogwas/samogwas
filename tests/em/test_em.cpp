#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <BoostTestTargetConfig.h>
 
#include "clustering/clustering.hpp"
#include "clustering/dbscan.hpp"
#include "test_tools/data_generation.hpp"
#include "em/multi_em.hpp"
#include "graph/graph.hpp"
#include <pl.h>

using namespace samogwas_test;
using namespace samogwas;


BOOST_AUTO_TEST_SUITE( EM_Test )

typedef plSymbol RandVar;
CndDistPtr create_cndDist(Variable& l, Variable& r, std::vector<double>& tab) {
  return std::make_shared<plCndDistribution>(l, r, tab);
}
Node& createLatentNode( GraphPtr g, plSymbol& var, Label2Index& l2i, const std::vector<int>& cluster );


BOOST_AUTO_TEST_CASE( Test_Log_LLH ) {
  // create Y [0.8, 0.2] --> 800*0, 200*x
  // create X_1|Y  = [0.7 0.3 0.6 0.4] --> [ 560, 240, 120, 80 ]
  // create X_2|Y  = [0.6 0.4 0.7 0.3] --> [ 480, 320, 140, 60 ]
  int N = 1000, CARD = 2;
  auto graph = std::make_shared<Graph>();
  plSymbol vX1("X1", plIntegerType(0, CARD-1)),vX2("X2", plIntegerType(0, CARD-1)), vZ("Z", plIntegerType(0, CARD-1));
  Label2Index lab2Idx;

  auto vec_X1 = std::make_shared<std::vector<int>>(N,0);
  auto vec_X2 = std::make_shared<std::vector<int>>(N,0);
  auto vec_Z = std::make_shared<std::vector<double>>(2*N,0);

  auto vec_Z_wo = std::make_shared<std::vector<int>>(N,0);

  for (int i = 0; i < N; ++i) {
    vec_X1->at(i) = ( (i < 560) || (i > 800 && i < 920) ) ? 0 : 1;
    vec_X2->at(i) = ( (i < 480) || (i > 800 && i < 940) ) ? 0 : 1;
    
    vec_Z->at(2*i) = ( i < 900 ) ? 1.0 : 0.0;
    vec_Z->at(2*i+1) = ( i < 900 ) ? 0.0 : 1.0;
    vec_Z_wo->at(i) = ( i < 900 ) ? 0 : 1; 
  }
    
  Node n_X1 = createObsNode( graph, vX1, vec_X1, 12, lab2Idx);
  Node n_X2 = createObsNode( graph, vX2, vec_X2, 17, lab2Idx);

  std::vector<double> probX1_Z{0.7, 0.3, 0.6, 0.4};
  std::vector<double> probX2_Z{0.6, 0.4, 0.7, 0.3};

  std::vector<CndDistPtr> cndDist {
    create_cndDist(vX1, vZ, probX1_Z),
    create_cndDist(vX2, vZ, probX2_Z),
  };

  auto distPtr = create_emp_distribution( vZ, *vec_Z );
  Node n_Z = createLatentNode( graph, vZ, vec_Z, cndDist, lab2Idx );  // create_latent ( graph, var, cndProb, vectorOfCndDists, position, lab2Idx );
}

BOOST_AUTO_TEST_CASE( Test_DBSCAN ) {
  size_t nclusts = 3, ncols = 25;
  size_t N = 6, CARD = 3, MAX_POS = 5;
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
  auto gdiss = std::make_shared<GraphMutInfoDissimilarity>(graph, l2g);

  DBSCAN dbscan( gdiss, 2, 0.45 );
  Partition result = dbscan();
  for ( int i = 0; i < nrows; ++i ) {
    int expected_cluster = i / N;
    BOOST_CHECK_EQUAL(result.getLabel(i), expected_cluster );
  }

  auto clustering = result.to_clustering();
  for ( auto clt: clustering ) {   
    RandVar var(boost::lexical_cast<std::string>(boost::num_vertices(*graph)), plIntegerType(0,2));
    Node n = createLatentNode(graph, var, lab2Idx, clt);
    MultiEM em(3);
    em.run( graph, n, 0.000001);
    break;
  }

}


Node& createLatentNode( GraphPtr graph, plSymbol& var, Label2Index& l2i, const std::vector<int>& cluster ) {

 vertex_t vertexId = boost::add_vertex(*graph); 
 Node& newNode = (*graph)[vertexId];
 plVariablesConjunction vars;
 for (auto idx: cluster) {
   // auto idx = l2i.at(p.getLabel(e));
   Node& n = (*graph)[idx];
   vars ^= n.variable;  

 }

 newNode.set_index(vertexId).set_graph(graph).set_variable(var)
     .set_local_indexes( vars.begin(), vars.end(), l2i);
 return newNode;  
}



BOOST_AUTO_TEST_SUITE_END()  /// Test InfoTheo ends here
