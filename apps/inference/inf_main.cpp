// /****************************************************************************************
//  * File: gwas_fltm.cpp
//  * Description:
//  * @author: siolag161 (thanh.phan@outlook.com)
//  * @date: 09/07/2014
//  ***************************************************************************************/
#include <iostream>
#include <thread>
#include <chrono>
#include <memory>
#include <limits>

#include <boost/graph/visitors.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/graph/adjacency_list.hpp>

#include <statistics/association_test.hpp>
#include "statistics/permutation_test.hpp"
#include "statistics/test_statistics.hpp"

#include <utils/csv_parser.hpp>
#include <graph/graph.hpp>
#include <graph/graph_io.hpp>

#include "inf_main.hpp"
#include "app_options.hpp"
#include "../utils/dataload.hpp"
#include "../utils/tools.hpp"


#include <pl.h>

#include <vector>
#include <map>
#include <memory>
#include <string>
#include <cmath>

#include <boost/lexical_cast.hpp>
#include <graph/graph.hpp>
#include "inference/inference.hpp"

using namespace std;



///////////////////////////////////////////////////////////////////////////////////////
using namespace samogwas;
int main( int argc, char** argv ) {
  auto args = get_program_options( argc, argv );
  std::cout << "Loading graph data...\n" << std::endl;
  auto g = std::make_shared<samogwas::Graph>();
  BayesGraphLoad graphLoad;
  graphLoad( g,
             args.inputLabelFile,
             args.bayesVertices,
             args.bayesDist,
             args.cndDataFile,
             args.inputDataFile );
  auto mat = load_data_table(args.inputDataFile);

  Graph& graphRef = *g;
  printf("Done loading graph of %lu edges and %lu vertices\n", boost::num_edges(graphRef),
         boost::num_vertices(graphRef));
  auto outputPath = get_output_dir(args);
  std::cout << "Loading pheno data from " << args.inputPheno<< std::endl; // todo: logging
  std::cout << "Loading label data from " << args.inputLabelFile << std::endl; // todo: logging

  auto engine = std::make_shared<InferenceFLTM>(graphRef);
  //double r = engine.log_likelihood(*mat);
  std::cout << "param_num: " << engine->number_of_free_parameters() << std::endl;
  std::cout << "complexity: " << engine->model_complexity(*mat) << std::endl;
  std::cout << "bic: " << engine->bic_score(*mat) << std::endl;
  std::cout << "log_likelihood: " << engine->log_likelihood(*mat) << std::endl;

}
