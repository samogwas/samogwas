// /****************************************************************************************
//  * File: fltm.cpp
//  * Description: 
//  * @author: siolag161 (thanh.phan@outlook.com)
//  * @date: 09/07/2014
//  ***************************************************************************************/

#include <iostream>
#include <thread>
#include <chrono>
#include <memory>

#include <fstream>
#include <cstdio>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp> // to obtain the program's name

#include "graph/graph.hpp"
#include "em/multi_em.hpp"
#include "fltm/card_func.hpp"
#include "distance/simi.hpp"
#include "distance/diss.hpp"
#include "clustering/clustering.hpp"
#include "clustering/dbscan.hpp"
#include "clustering/cast.hpp"

#include "dataload.hpp"
#include "algo_properties.hpp"
#include "application_options.hpp"
#include "fltm_main.hpp"

using namespace samogwas;
using ns = std::chrono::nanoseconds;
using get_time = std::chrono::steady_clock ;
 
int main( int argc, char** argv ) {
  auto options = get_program_options( argc, argv );
  auto labels = std::make_shared<LabelVec>(); auto positions = std::make_shared<PosVec>();  auto ids = std::make_shared<PosVec>();
  Label2Index lab2Idx;

  load_labels_positions( *labels, *ids, *positions, options.inputLabelFile );

  auto mat = load_data_table(options.inputDataFile);
  auto l2g = init_index_mapping( mat->size() );
  auto graph = init_graph( *mat,  lab2Idx, options.fltm_params.cardinality, *labels, *positions );
  // auto clustAlgos = read_clustering_algos( graph, l2g, positions, options );

  // clustering

  // FLTM fltm(options.fltm_params);
  // auto cardF = std::make_shared<LinearCardinality>(options.fltm_alpha, options.fltm_beta, options.fltm_maxCard);

  // for ( auto clustAlgo: clustAlgos ) {
  //   fltm.execute(clustAlgo, cardF, graph);
  // }
  auto simi = std::make_shared<GraphMutInfoSimilarity>(graph, l2g);

  auto start = get_time::now(); //use auto keyword to minimize typing strokes :)
  std::cout << "abt to precompute all ...\n";
  simi->precompute_all();
  auto end = get_time::now();
  auto diff = end - start;
  std::cout<<"Elapsed time is :  "<< std::chrono::duration_cast<ns>(diff).count()/10000000000 << "s "<< std::endl;

  
}

