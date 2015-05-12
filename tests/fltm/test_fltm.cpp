#define BOOST_TEST_DYN_LINK

#include <memory>
#include <iostream>
#include <thread>
#include <chrono>
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

#include "fltm/fltm.hpp"
#include "fltm/latent_var_criteria.hpp"

#include "dataload.hpp"
#include "algo_properties.hpp"
#include "application_options.hpp"
#include "fltm_main.hpp"

void init()
{
  boost::log::core::get()->set_filter
  (
      boost::log::trivial::severity >= boost::log::trivial::info
  );
}


using namespace samogwas_test;
using namespace samogwas;

using ms = std::chrono::milliseconds;
using get_time = std::chrono::steady_clock ;
typedef std::shared_ptr<PositionCriteria> PositionCriteriaPtr;

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


Options get_test_program_options(std::string test);
BOOST_AUTO_TEST_CASE( Test_FLTM_Simu ) {
  auto options = get_test_program_options("Test_FLTM_Simu");
  auto labels = std::make_shared<LabelVec>();
  auto positions = std::make_shared<PosVec>();
  auto ids = std::make_shared<PosVec>();
  Label2Index lab2Idx;

  load_labels_positions( *labels, *ids, *positions, options.inputLabelFile );

  auto mat = load_data_table(options.inputDataFile);
  auto l2g = init_index_mapping( mat->size() );
  auto graph = init_graph( *mat,  lab2Idx, options.fltm_params.cardinality, *labels, *positions );
  auto clustAlgos = read_clustering_algos( graph, l2g, positions, options );

  FLTM fltm(options.fltm_params);
  auto cardF = std::make_shared<LinearCardinality>(options.fltm_alpha, options.fltm_beta, options.fltm_maxCard);
   std::cout << "\nrunning test FLTM" << std::endl;    
  auto start = get_time::now();
  for ( auto clustAlgo: clustAlgos ) {    
    std::cout << "running FLTM for " << clustAlgo->name() << std::endl;    
    fltm.execute(clustAlgo, cardF, graph);
    auto rs = clustAlgo->run();
    auto end = get_time::now();
    auto diff = end - start;
    std::cout<<"Elapsed time is:  "
             << std::chrono::duration_cast<ms>(diff).count()/1000
             << " seconds" <<std::endl <<std::endl <<std::endl;

  }  

}

Options get_test_program_options(std::string test) {

  Options options;
  options.inputDataFile = "../tests/data/fltm/dat_50.csv";
  options.inputLabelFile = "../tests/data/fltm/lab_50.csv";
  options.outputDir = "../tests/data/fltm/out";
  options.clustConf = "../tests/data/fltm/clustering.cfg";
  options.fltm_alpha = 1;
  options.fltm_beta = 1;
  options.fltm_maxCard = 12;
  options.fltm_params.cardinality = 3;
  options.fltm_params.nbrSteps = 5;
  options.fltm_params.nbrRestarts = 1;
  options.fltm_params.emThres = 0.01;
  options.fltm_params.latentVarQualityThres = 0.0;
  
  return options;

}


BOOST_AUTO_TEST_SUITE_END()  //
