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
#include "fltm_main.hpp"

using namespace samogwas;
using ms = std::chrono::milliseconds;
using get_time = std::chrono::steady_clock ;
typedef std::shared_ptr<PositionCriteria> PositionCriteriaPtr;

void pre_compute( PtrMatrixPtr mat, Graph& graph, CriteriaPtr criteria );

int main( int argc, char** argv ) {
  // auto options = get_test_program_options(""); // get_program_options( argc, argv );
  auto options = get_program_options( argc, argv );
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
  
  //use auto keyword to minimize typing strokes :)
  auto start = get_time::now();
  for ( auto clustAlgo: clustAlgos ) {    
    std::cout << "running FLTM for " << clustAlgo->name() << std::endl;    
    fltm.execute(clustAlgo, cardF, graph);
    auto end = get_time::now();
    auto diff = end - start;
    std::cout<<"Elapsed time is:  " << std::chrono::duration_cast<ms>(diff).count()/1000 << " seconds" <<std::endl;

  }  

}





