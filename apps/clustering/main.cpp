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

#include "graph/graph_io.hpp"

#include "dataload.hpp"
#include "clustering_config.hpp"
#include "application_options.hpp"

#include "main.hpp"

using namespace samogwas;
using ms = std::chrono::milliseconds;
using get_time = std::chrono::steady_clock ;
typedef std::shared_ptr<PositionCriteria> PositionCriteriaPtr;

boost::filesystem::path outputDir( Options& progOpt );
char* current_date();
void saveDatLab( const Graph& g, const std::string& dat, const std::string& lab);

int main( int argc, char** argv ) {
  auto options = get_program_options( argc, argv );
  auto labels = std::make_shared<LabelVec>();
  auto positions = std::make_shared<PosVec>();
  auto ids = std::make_shared<PosVec>();

  Label2Index lab2Idx;

  load_labels_positions( *labels, *ids, *positions, options.inputLabelFile );


  auto mat = load_data_table(options.inputDataFile, options.matrixType);
  if (options.matrixType == 1) {
    //*mat = Transpose(*mat)
  }
  auto clustAlgoConfs = read_clustering_algos( options.clustConf );

  auto outputPath = outputDir(options);
  std::ofstream stats( (outputPath / "log.txt").string() );
  auto start = get_time::now();
  for ( auto cltConf: clustAlgoConfs ) {
    auto graph = init_graph( *mat,  lab2Idx, options.fltm_params.cardinality, *labels, *positions );
    auto l2g = init_index_mapping( mat->size() );
    auto algoClust = read_clustering_algo( cltConf, graph, l2g, positions, options.fltm_params.maxDist);

    auto end = get_time::now();
    auto diff = end - start;
  }
  boost::filesystem::create_directories(outputPath);
  stats.close();
}

char* current_date()
{
  time_t rawtime;
  struct tm * timeinfo;
  char* buffer = new char[80];
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  strftime (buffer, 80, "%Y_%m_%d_%H_%M_%S", timeinfo);
  return buffer;
}

boost::filesystem::path outputDir( Options& progOpt ) {
  auto path = boost::filesystem::absolute(progOpt.outputDir);
  path /= current_date();
  boost::filesystem::create_directories(path);
  return path;
}

