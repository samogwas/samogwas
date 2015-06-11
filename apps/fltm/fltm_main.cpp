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
#include "algo_properties.hpp"
#include "fltm_main.hpp"

using namespace samogwas;
using ms = std::chrono::milliseconds;
using get_time = std::chrono::steady_clock ;
typedef std::shared_ptr<PositionCriteria> PositionCriteriaPtr;

boost::filesystem::path outputDir( Options& progOpt );
char* current_date();
void saveImputedDataLabels( const Graph& g, const std::string& dat, const std::string& lab, const char sep=' ');
using namespace samogwas;

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

   FLTM fltm(options.fltm_params);
   auto cardF = std::make_shared<LinearCardinality>(options.fltm_alpha, options.fltm_beta, options.fltm_maxCard);

   auto outputPath = outputDir(options);
   std::ofstream stats( (outputPath / "log.txt").string() );
   auto start = get_time::now();
   for ( auto cltConf: clustAlgoConfs ) {
     auto graph = init_graph( *mat,  lab2Idx, options.fltm_params.cardinality, *labels, *positions );
     auto l2g = init_index_mapping( mat->size() );
     auto algoClust = read_clustering_algo( cltConf, graph, l2g, positions, options.fltm_params.maxDist);
     FLTM fltm(options.fltm_params);
  
     fltm.execute( algoClust, cardF, graph);
      auto end = get_time::now();
      auto diff = end - start;

      stats << "Parameter options : " << std::endl;
      for (int i = 0 ; i < argc ; i++ )
          stats << "\t" << argv[i] << std::endl;

     stats << "Running FLTM for "
           << algoClust->name() << " on " << l2g->size() << " variables"
           << std::endl;

     stats << "Elapsed time is:  "
           << std::chrono::duration_cast<ms>(diff).count()/1000
           << " seconds" << std::endl;
     boost::filesystem::create_directories(outputPath);

     switch( options.outType) {
       case 0: {
         std::string outBayesVertex, outBayesDist, outImpDat, outImpLab, outGraph;
         char bayesVertex_fn[256], bayesDist_fn[256], imputedDat_fn[256], imputedLab_fn[256], graph_fn[256];
         sprintf(bayesVertex_fn, "fltm_%s_bayes.vertex", algoClust->name() );
         sprintf(bayesDist_fn, "fltm_%s_bayes.dist", algoClust->name() );
         sprintf(imputedDat_fn, "fltm_%s_imputed.dat", algoClust->name() );
         sprintf(imputedLab_fn, "fltm_%s_imputed.lab", algoClust->name() );
         sprintf(graph_fn, "fltm_%s.graph", algoClust->name() );

         outBayesVertex = (outputPath / bayesVertex_fn).string(),
             outBayesDist = (outputPath / bayesDist_fn).string(),
             outImpDat = (outputPath / imputedDat_fn).string(),
             outImpLab = (outputPath / imputedLab_fn).string(),
             outGraph = (outputPath / graph_fn).string();
         BayesGraphSave()( *graph, outBayesVertex, outBayesDist );
         // saveDatLab( *graph, outImpDat, outImpLab );
         saveImputedDataLabels(*graph, outImpDat, outImpLab );
         break;
       }
       case 1: {
         std::string outNode, outEdge;
         char edge_fn[256], node_fn[256];
         sprintf( node_fn, "fltm_%s_tulip_vertex.csv", algoClust->name() );
         sprintf( edge_fn, "fltm_%s_tulip_edge.csv", algoClust->name() );
         outNode = (outputPath  /node_fn ).string(),
             outEdge = (outputPath / edge_fn).string();
         TulipGraphSave()( *graph, outNode, outEdge );
         break;
       }

       default:
         break;
     }
   }
   stats.close();
}



char* current_date()
{
  time_t rawtime;
  struct tm * timeinfo;
  char* buffer = new char[80];
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  strftime (buffer,80,"%Y_%m_%d_%H_%M_%S",timeinfo);

  return buffer;
}

boost::filesystem::path outputDir( Options& progOpt ) {
  auto path = boost::filesystem::absolute(progOpt.outputDir);
  path /= current_date();
  boost::filesystem::create_directories(path);
  return path;
}

void saveImputedDataLabels( const Graph& g, const std::string& datF, const std::string& labF, const char sep ) {
  std::ofstream datFile(datF), labFile(labF);

  std::vector<std::string> labels;
  std::vector<DataVecPtr> dataMat;

  labFile << "id" << sep << "label" << sep
            << "cardinality" << sep
            << "position" << sep
            << "level" << std::endl;
  
  for ( auto vp = boost::vertices(g); vp.first != vp.second; ++vp.first) {
    const Node& n = g[*vp.first];
    if (!n.is_leaf()) {
      labels.push_back(n.getLabel());
      dataMat.push_back(n.dataVec);
    }
    labFile << n.index << sep << n.getLabel() << sep
            << n.cardinality() << sep
            << n.position << sep
            << n.level << std::endl;
  }

  datFile << "individual-id" << sep;
  for (size_t i = 0; i < labels.size() - 1; ++i) {
    datFile << labels.at(i) << sep;
  }
  datFile << labels.at(labels.size()-1) << std::endl;

  auto COLS = labels.size(), ROWS = dataMat[0]->size();
  for ( size_t r = 0; r < ROWS; ++r ) {
    datFile << r << sep;
    for ( size_t c = 0; c < COLS - 1; ++c) {
      datFile << dataMat.at(c)->at(r) << sep;
    }
    datFile << dataMat.at(COLS-1)->at(r) << std::endl;
  }

  datFile.close(); labFile.close();
}
