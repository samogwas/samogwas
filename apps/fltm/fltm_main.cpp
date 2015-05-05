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
using ms = std::chrono::milliseconds;
using get_time = std::chrono::steady_clock ;
typedef std::shared_ptr<PositionCriteria> PositionCriteriaPtr;

void pre_compute( PtrMatrixPtr mat, Graph& graph, CriteriaPtr criteria );

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
  auto criteria = std::make_shared<PositionCriteria>( positions, options.fltm_maxDist ); 
  simi->set_criteria(criteria);
  pre_compute(mat, *graph, criteria );

  auto start = get_time::now(); //use auto keyword to minimize typing strokes :)
  // std::cout << "abt to precompute all ...\n";
  // simi->precompute_all();
  // auto end = get_time::now();
  // auto diff = end - start;
  // std::cout<<"A total of time is :  "<< std::chrono::duration_cast<ms>(diff).count()/1000 << "s "<< std::endl;


}


void pre_compute( PtrMatrixPtr mat, Graph& graph, CriteriaPtr criteria )  {
  // ComputeNodeEntropy entropy;
  ComputeNodeJointEntropy jEntropy;

  Entropy<EMP> entropy;
  // JointEntropy<EMP> jointEntropy;
  auto SIZE = 38730;
      
  std::vector<double> cached_entropies(SIZE, 0.0);
  ///std::vector<double> cached_joint_entropies(SIZE*(SIZE+1)/2, 0.0);
  std::map<int, double> cached_joint_entropies;
  auto start = get_time::now(); //use auto keyword to minimize typing strokes :)
  int count = 0;
  std::cout << "abt to precompute all ..." << cached_entropies.size() << std::endl;
  for ( size_t a = 0; a < SIZE; ++a) {
    cached_entropies[a] = entropy(*mat->at(a));
  }
  
  for ( size_t a = 0; a < SIZE; ++a) {
    for ( size_t b = a+1; b < SIZE; ++b ) {        
      if (criteria && !criteria->valid(a,b)) continue;        
      count++;
      double minEntropyAB = std::min(cached_entropies.at(a), cached_entropies.at(b));
      double norm_mutinfo = 0.0;
      if (minEntropyAB != 0) {
        double jE_AB = jEntropy.compute(graph[a], graph[b]);//jointEntropy( *mat->at(a), *mat->at(b) );
        double mi_AB = cached_entropies.at(a) + cached_entropies.at(b) - jE_AB -1;
        norm_mutinfo = mi_AB / minEntropyAB;
      }
      auto commonIdx = indexOfPair(a,b,SIZE);
      // cached_joint_entropies[commonIdx] = norm_mutinfo;
    }
  }
  
  auto end = get_time::now();
  auto diff = end - start;
  std::cout<<"Elapsed time is :  "<< std::chrono::duration_cast<ms>(diff).count()/1000 << "s (" << count << ")" << std::endl;

}


