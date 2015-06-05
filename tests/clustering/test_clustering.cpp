#define BOOST_TEST_DYN_LINK

#include <BoostTestTargetConfig.h>
#include <boost/test/unit_test.hpp>

#include <memory>

#include "test_tools/data_generation.hpp"
#include "clustering/clustering.hpp"
#include "clustering/cast.hpp"
#include "clustering/dbscan.hpp"

#include "clustering/louvain/graph.hpp"
#include "clustering/louvain/community.hpp"
#include "clustering/louvain/louv.hpp"

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

//////////////////////////////////////////////////////////////////////////////
const size_t N = 3, CARD = 3, MAX_POS = 5, NCOLS = 40;

BOOST_AUTO_TEST_CASE( test_Louvain_1 )
{

  int commCard = 5, commCount = 4;
  int nrows = commCard*commCount;
  std::vector<int> positions;
  auto data = GenerateClusteredData( commCount, commCard, CARD, NCOLS )();
  Label2Index lab2Idx;

  auto graph = std::make_shared<samogwas::Graph>();
  auto l2g = std::make_shared<std::vector<int>>(nrows,0);
  for (int i = 0; i < nrows; ++i) {
     positions.push_back(i);
    (*l2g)[i]=i;
    plSymbol v(boost::lexical_cast<std::string>(i), plIntegerType(0, CARD-1));
    auto dataVec = std::make_shared<DataVec>(data->at(i));
    createObsNode(graph, v, dataVec, 12, lab2Idx);
  }

  auto criteria = std::make_shared<PositionCriteria>(std::make_shared<DataVec>(positions), MAX_POS);   
  auto simi = std::make_shared<GraphMutInfoSimilarity>(graph, l2g);
  auto g = std::make_shared<louvain::Graph>( simi, false );
  louvain::Network network(g);
  
  BOOST_CHECK_EQUAL(network.nbrCommunities(), commCount*commCard); // initially
  BOOST_CHECK_EQUAL(network.nbrNodes(), commCount*commCard);

  for (int i = 0; i < commCard*commCount; ++i) {
    BOOST_CHECK_EQUAL( network.getCommunity(i), i ); // initially
    BOOST_CHECK_EQUAL( network.membersOf(i).size(), 1 ); // initially
    BOOST_CHECK_EQUAL( network.membersOf(i)[0], i ); // initially
  }

}

struct Simi: public GraphSimilarity {
  Simi(std::vector< std::vector<double> > d): sim(d) {}

  virtual size_t nbrVariables() const {
    return sim.size();
  }

  virtual size_t nbr_variables() const { return sim.size(); }
  virtual double compute( const int varA, const int varB ) { return sim[varA][varB]; }
  virtual void invalidate_entropy_cache() {}

  virtual void invalidate() {}
  std::vector< std::vector<double> > sim;
};

BOOST_AUTO_TEST_CASE( Test_Louvain_Linked_Weights_Wikipedia ) {
  // http://en.wikipedia.org/wiki/Modularity_%28networks%29
  std::vector< std::vector<double> > sim {
    {0,1,1,0,0,0,0,0,0,1}, // 0
    {1,0,1,0,0,0,0,0,0,0}, // 1
    {1,1,0,0,0,0,0,0,0,0}, // 2 
    {0,0,0,0,1,1,0,0,0,1}, // 3
    {0,0,0,1,0,1,0,0,0,0}, // 4
    {0,0,0,1,1,0,0,0,0,0}, // 5
    {0,0,0,0,0,0,0,1,1,1},
    {0,0,0,0,0,0,1,0,1,0},
    {0,0,0,0,0,0,1,1,0,0},
    {1,0,0,1,0,0,1,0,0,0}    
  };
  
  //std::shared_ptr<SimilarityMatrix> simi(new Simi(sim));
  auto simi = std::make_shared<Simi>(sim);
  auto louv = std::make_shared<louvain::MethodLouvain>(simi);
  louv->first_phase();
  double modul_1st = louv->network->modularity();
  louv->second_phase();
  double modul_2nd = louv->network->modularity();
  auto ntw = louv->network;
  for ( auto comm: ntw->communities() ) {    
    double iw = ntw->in_weights[comm];
    double tl = ntw->tot_linked_weights[comm];
  }  
  
  BOOST_CHECK_EQUAL(modul_1st , modul_2nd);
  auto clustering = louv->run();  
  BOOST_CHECK_EQUAL( clustering.nbrClusters(), 3 );
  BOOST_CHECK_EQUAL( clustering.getLabel(0), clustering.getLabel(1));
  BOOST_CHECK_EQUAL( clustering.getLabel(1), clustering.getLabel(2));
  BOOST_CHECK_EQUAL( clustering.getLabel(2), clustering.getLabel(9));  
  for ( louvain::NodeIndex n = 0; n < clustering.nbrItems(); ++n ) {
    if ( n < 3 ) BOOST_CHECK_EQUAL( clustering.getLabel(n), 0);
    else if ( n < 6 ) BOOST_CHECK_EQUAL( clustering.getLabel(n), 1);
    else if ( n < 9 ) BOOST_CHECK_EQUAL( clustering.getLabel(n), 2);
    else if ( n == 9 ) BOOST_CHECK_EQUAL( clustering.getLabel(n), 0);
  }  
}

BOOST_AUTO_TEST_CASE( TEST_Louvain_GENERATE ) {
  int commCard = 5, commCount = 4;
  int nrows = commCard*commCount;
  std::vector<int> positions; for ( int i = 0; i < nrows; ++i ) positions.push_back(i);
  auto data = GenerateClusteredData( commCount, commCard, CARD, NCOLS )();
  // std::shared_ptr<SimilarityMatrix> simi( new MutInfoSimi(data, positions, MAX_POS, -1) );
  // std::shared_ptr<Graph> g(new Graph(simi));  // 
  Label2Index lab2Idx;

  auto graph = std::make_shared<samogwas::Graph>();
  auto l2g = std::make_shared<std::vector<int>>(nrows,0);
  for (int i = 0; i < nrows; ++i) {
     positions.push_back(i);
    (*l2g)[i]=i;
    plSymbol v(boost::lexical_cast<std::string>(i), plIntegerType(0, CARD-1));
    auto dataVec = std::make_shared<DataVec>(data->at(i));
    createObsNode(graph, v, dataVec, 12, lab2Idx);
  }
  
  auto criteria = std::make_shared<PositionCriteria>(std::make_shared<DataVec>(positions), MAX_POS);   
  auto simi = std::make_shared<GraphMutInfoSimilarity>(graph, l2g);
  auto g = std::make_shared<louvain::Graph>( simi, false );
  louvain::Network network(g);
  
  std::ofstream wg("../tests/data/clustering/graph.txt");
  auto sz = data->size();
  for ( int r = 0; r < sz; ++r) {
    for ( int c = r+1; c < sz; ++ c) {
      double w = simi->compute(c,r);
      if ( w > 0)
        wg << r << " " << c << " " << w << std::endl;
    }
  }

  double expected_modularity = 0.0;
  double tw2 = 80;
  for (int i = 0; i < commCard*commCount; ++i) {
    double iw = network.in_weights[i];
    double itw = network.tot_linked_weights[i];
    BOOST_CHECK_EQUAL( iw, 0.0 );
    BOOST_CHECK_EQUAL( iw, 0.0 );
    expected_modularity += ( iw - (itw/tw2)*( itw/tw2) );
  }
  BOOST_CHECK_CLOSE_FRACTION(network.modularity(), -0.05, 0.1);
  auto louv1 = std::make_shared<louvain::MethodLouvain>(simi);
  double modul_1st = louv1->network->modularity();
  louv1->second_phase();
  double modul_2nd = louv1->network->modularity();
  BOOST_CHECK_EQUAL(modul_1st , modul_2nd);

  auto louv = std::make_shared<louvain::MethodLouvain>(simi);
  auto clustering = louv->run();  
  for ( louvain::NodeIndex n = 0; n < clustering.nbrItems(); ++n ) {
    BOOST_CHECK_EQUAL( clustering.getLabel(n), n/5 ); // initially
  }
}

BOOST_AUTO_TEST_CASE( Test_Total_Modulariy_n ) {
  std::vector< std::vector<double> > sim {
    {0,0,0,0,0},
    {0,0,0,0,0},
    {0,0,0,0,0},
    {0,0,0,0,0}, 
    {0,0,0,0,0}       
  };
  
  // SimilarityMatrix* simi = new Simi(sim);
  // std::shared_ptr<SimilarityMatrix> simi(new Simi(sim));
  auto simi = std::make_shared<Simi>(sim);
  // auto louv = std::make_shared<louvain::MethodLouvain>(simi);
  auto g = std::make_shared<louvain::Graph>( simi, false );

  // std::shared_ptr<Graph> g(new Graph(simi));
  louvain::Network network(g);
  BOOST_CHECK_EQUAL( network.modularity(), -2.0);
}



BOOST_AUTO_TEST_CASE( Test_Linked_Weights ) {
  std::vector< std::vector<double> > sim {
    {0,1,2,0,0},
    {1,0,0,3,0},
    {2,0,0,3,4},
    {0,3,3,0,0}, 
    {0,0,4,0,0}       
  };
  
  // SimilarityMatrix* simi = new Simi(sim);
  // std::shared_ptr<SimilarityMatrix> simi(new Simi(sim));
  // std::shared_ptr<Graph> g(new Graph(simi));
  // Network network(g);

  auto simi = std::make_shared<Simi>(sim);
  auto g = std::make_shared<louvain::Graph>( simi, false );
  louvain::Network network(g);
  
  BOOST_CHECK_EQUAL( g->linkedWeights(0), 3.0);
  BOOST_CHECK_EQUAL( g->linkedWeights(1), 4.0);
  BOOST_CHECK_EQUAL( g->linkedWeights(2), 9.0);
  BOOST_CHECK_EQUAL( g->linkedWeights(3), 6.0);
  BOOST_CHECK_EQUAL( g->linkedWeights(4), 4.0);

  BOOST_CHECK_EQUAL( network.totalWeights(), 13);

  double total_w_r = 0.0;
  for ( int i = 0; i < network.nbrNodes(); ++i ) total_w_r += g->linkedWeights(i);
  BOOST_CHECK_EQUAL( 2*network.totalWeights(), total_w_r); // becasue 

  BOOST_CHECK( network.modularity() > -0.5  );

}


BOOST_AUTO_TEST_SUITE_END()
