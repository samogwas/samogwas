
#include "pl.h"
#include <iostream>
#include <map>
#include <chrono>
#include <memory>
#include <fstream>
#include <cstdio>
#include <boost/lexical_cast.hpp>
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
#include "inference/inference_engine.hpp"
#include "app_options.hpp"
#include "main.hpp"

using ms = std::chrono::milliseconds;
using get_time = std::chrono::steady_clock ;
using namespace samogwas;

char* time_taken(unsigned long seconds);
void infer_from_graph(Graph& graph, const PtrMatrix& mat, const int inferenceOption, std::ofstream& log);

boost::filesystem::path outputDir( Options& progOpt );
char* current_date();
void saveDatLab( const Graph& g, const std::string& dat, const std::string& lab);


/**
 *
 */
int main(int argc, char* argv[]) {
  auto options = get_program_options( argc, argv );
  auto labels = std::make_shared<LabelVec>();
  auto positions = std::make_shared<PosVec>();
  auto ids = std::make_shared<PosVec>();

  Label2Index lab2Idx;

  load_labels_positions( *labels, *ids, *positions, options.inputLabelFile );

  auto mat = load_data_table(options.inputDataFile, options.matrixType);
  auto clustAlgoConfs = read_clustering_algos( options.clustConf );

  FLTM fltm(options.fltm_params);
  auto cardF = std::make_shared<LinearCardinality>(options.fltm_alpha, options.fltm_beta, options.fltm_maxCard);

  auto start = get_time::now();
  auto outputPath = outputDir(options);
  boost::filesystem::create_directories(outputPath);
  std::ofstream stats( (outputPath / "log.txt").string() );

  for (auto cltConf: clustAlgoConfs) {
    auto graph = init_graph( *mat,  lab2Idx, options.fltm_params.cardinality, *labels, *positions );
    auto l2g = init_index_mapping( mat->size() );
    auto algoClust = read_clustering_algo( cltConf, graph, l2g, positions, options.fltm_params.maxDist);
    FLTM fltm(options.fltm_params);
    fltm.execute( algoClust, cardF, graph);
    auto end = get_time::now();
    auto diff = end - start;

    stats << "Parameter options : " << std::endl;
    for (int i = 0 ; i < argc ; i++ ) { stats << "\t" << argv[i] << std::endl; }
    stats << "Running FLTM for "
          << algoClust->name() << " on " << l2g->size() << " variables"
          << std::endl;
    stats << "Elapsed time is:  "
          << std::chrono::duration_cast<ms>(diff).count()/1000
          << " seconds" << std::endl;

    /** inference **/
    infer_from_graph(*graph, *mat, options.inferenceOption, stats);

    /** saving results **/
    if ( options.outType == 0 ||  options.outType == 2) {
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
         saveDatLab( *graph, outImpDat, outImpLab );
       }
       if ( options.outType == 1 ||  options.outType == 2) {
         std::string outNode;
         char node_fn[256];
         sprintf( node_fn, "fltm_%s_tulip.csv", algoClust->name() );
         outNode = (outputPath  /node_fn ).string();
         TulipGraphSave()( *graph, outNode );
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

char* time_taken(unsigned long total) {
  unsigned minutes, seconds, hours;
  minutes = total / 60;
  seconds = total % 60;
  hours = minutes / 60;
  minutes = minutes % 60;
  char* buffer = new char[120];
  sprintf(buffer, "%d hours, %d minutes, %d seconds", hours, minutes, seconds);
  return buffer;
}


void infer_from_graph(Graph& graph, const PtrMatrix& mat, const int inferenceOption, std::ofstream& stats) {

  auto sample = transpose_data_matrix(mat);
  auto start = get_time::now();
  InferenceFLTM inference_engine(graph);
  inference_engine.compile_evidence(inferenceOption); // compiling...

  auto end = get_time::now();
  auto diff = end - start;
  stats << "model compilation takes: " << time_taken(std::chrono::duration_cast<ms>(diff).count()/1000) << std::endl;

  start = get_time::now();

  double complexity = inference_engine.model_complexity(*sample);
  double log_llh = inference_engine.log_likelihood(*sample);
  double bic_score = -2*log_llh + complexity; // formula of the bic score

  stats << "complexity: " << complexity << std::endl;
  stats << "log llh: " << log_llh << std::endl;
  stats << "bic score: " << bic_score << std::endl;

  end = get_time::now();
  diff = end - start;
  stats << "bic score computation takes: " << time_taken(std::chrono::duration_cast<ms>(diff).count()/1000) << std::endl;
}

boost::filesystem::path outputDir( Options& progOpt ) {
  auto path = boost::filesystem::absolute(progOpt.outputDir);
  path /= current_date();
  boost::filesystem::create_directories(path);
  return path;
}

void saveDatLab( const Graph& g, const std::string& datF, const std::string& labF ) {
  std::ofstream datFile(datF), labFile(labF);
  labFile << "index" << "," << "label" << "," << "cardinality" << "," << "position" << "," << "level" << std::endl;

  for ( auto vp = boost::vertices(g); vp.first != vp.second; ++vp.first) {
    Node n = g[*vp.first];

    //if (!n.is_leaf()) {
      for ( size_t i = 0; i < n.dataVec->size() - 1; ++i ) {
         datFile << n.dataVec->at(i) << ",";
      }
      datFile << n.dataVec->at(n.dataVec->size() - 1) << "\n";
      //}
    labFile << n.index << "," << n.getLabel() << "," << n.cardinality() << "," << n.position << "," << n.level << std::endl;
  }

  datFile.close(); labFile.close();
}
