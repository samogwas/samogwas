// /****************************************************************************************
//  * File: fltm.#include "main.h"
//  * Description: 
//  * @author: siolag161 (thanh.phan@outlook.com)
//  * @date: 09/07/2014
//  ***************************************************************************************/

#include <iostream>
#include <thread>
#include <chrono>
#include <memory>
#include <algorithm>
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

#include "clustering/louvain/graph.hpp"
#include "clustering/louvain/community.hpp"
#include "clustering/louvain/louv.hpp"

#include "graph/graph_io.hpp"

#include "dataload.hpp"
#include "clustering_config.hpp"
#include "application_options.hpp"

#include "clustering/measures/expected_mutual_info.hpp"

#include "main.hpp"

using namespace samogwas;
using ms = std::chrono::milliseconds;
using get_time = std::chrono::steady_clock ;
typedef std::shared_ptr<PositionCriteria> PositionCriteriaPtr;

boost::filesystem::path outputDir( Options& progOpt );
char* current_date();
void saveClustering(std::string cltFN, std::string labFN,
                    Partition& partition, std::string clustAlgoName);

void compute_measures(const Options&);
std::vector<size_t> get_rank(const std::vector<double>& vals);
struct RankItem;

int main( int argc, char** argv ) {
  auto options = get_program_options( argc, argv );

  std::cout << "ref-clustering: " << options.ref_clustering << std::endl;
  if (options.ref_clustering != "") { // compute measures
    compute_measures(options);
  } else {
    auto labels = std::make_shared<LabelVec>();
    auto positions = std::make_shared<PosVec>();
    auto ids = std::make_shared<PosVec>();
    Label2Index lab2Idx;
    load_labels_positions( *labels, *ids, *positions, options.inputLabelFile );

    auto mat = load_data_table(options.inputDataFile, 0);

    auto outputPath = outputDir(options);
    boost::filesystem::create_directories(outputPath);
    std::cout << "out: " << outputPath << std::endl;
    std::string clustON_DBSCAN = (outputPath / "clusterings_DBSCAN.txt").string(),
        clustON_LOUV = (outputPath / "clusterings_LOUV.txt").string(),
        clustON_CAST = (outputPath / "clusterings_CAST.txt").string(),
        labON =  (outputPath / "algo.log").string();

    std::ofstream stats( (outputPath / "log.txt").string() );
    auto start = get_time::now();
    std::cout << "setting up...\n";
    auto graph = init_graph(*mat, lab2Idx, options.fltm_params.cardinality, *labels, *positions );
    auto l2g = init_index_mapping( mat->size() );

    omp_set_num_threads(5);
    std::cout << " now cast's time" << std::endl;

    unsigned count = (100000-50000)/10000+1;
    unsigned step = 1;
    for (unsigned MAX_POS = 50000; MAX_POS <= 200000; MAX_POS += 1000) {
      std::cout << (step++) << " over " << count << std::endl;
      auto criteria = std::make_shared<PositionCriteria>( positions, MAX_POS);
      auto simi = std::make_shared<GraphMutInfoSimilarity>(graph, l2g);
      simi->set_criteria(criteria);
      auto algorithm =std::make_shared<louvain::MethodLouvain>(simi);
      auto partition = algorithm->run();
      saveClustering(clustON_LOUV, labON, partition, std::string(algorithm->name()));
    }
    auto end = get_time::now();
    auto diff = end - start;
    std::cout << "Elapsed time is:  "
              << std::chrono::duration_cast<ms>(diff).count()/1000
              << " seconds."
              << " average: " << std::chrono::duration_cast<ms>(diff).count()/(count*1000)
              << std::endl;
    stats.close();
  }
}

char* current_date() {
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

void saveClustering(std::string cltFN, std::string labFN,
                    Partition& partition, std::string clustAlgoName) {
  std::ofstream cltFile, labFile;
  cltFile.open(cltFN, std::ios_base::app);
  labFile.open(labFN, std::ios_base::app);
  for (size_t i = 0; i < partition.nbrItems()-1; ++i ) {
    cltFile << partition.getLabel(i) << ",";
  }
  cltFile << partition.getLabel(partition.nbrItems()-1);
  cltFile << std::endl;
  labFile << clustAlgoName << std::endl;
  cltFile.close(); labFile.close();
}

//////////////////////////////////////////////////////////////
typedef std::vector<int> MClustering;

MClustering load_ref_clustering(const std::string& infile)  {
  std::ifstream clusteringFile(infile.c_str());
  if (!clusteringFile) {
    printf("file ref_clustering %s not existing\n", infile.c_str());
    exit(-1);
  }
  printf("beginning loading ref_clustering...\n");
  CSVIterator<int> clusteringLine(clusteringFile);// ++clusteringLine;
  // ++clusteringLine;

  MClustering ref_clustering(clusteringLine->size(), 0);
  for (unsigned i = 0; i < clusteringLine->size(); ++i) {
    ref_clustering[i] = clusteringLine->at(i);
  }
  std::cout << "done loading. loaded clustering of " << ref_clustering.size() << " variables.\n";
  return ref_clustering;
}


void compute_measures(const Options& options) {
  auto start = get_time::now();

  auto clusterings = *load_data_table(options.clustering_file);
  auto ref_clustering = load_ref_clustering(options.ref_clustering);
  Partition ref_partititon(ref_clustering);
  double i = 0;
  std::vector<double> measures {};//; measures.reserve(clusterings.size());
  for (auto clusteringPtr: clusterings) {
    std::cout << "doing: " << i++ << " over " << clusterings.size() << std::endl;
    auto clustering = *clusteringPtr;
    Partition partition(clustering);
    auto ami = adjusted_mutual_information(ref_partititon, partition);
    //std::cout << "done: " << ami << std::endl;
    measures.push_back(ami);
  }
  std::cout << "ranking...\n";
  auto rank = get_rank(measures);

  auto path = boost::filesystem::absolute(options.outputDir);
  // auto rank_path = (path / options.clustering_file);
  // auto rank_out_path = (path / options.clustering_file).string();
  boost::filesystem::create_directories(path);
  auto rank_out_file = (path / "rank.csv").string();

  std::cout << "writing to..." << rank_out_file << std::endl;
  std::ofstream rankOutFile(rank_out_file);
  for (size_t r = 0; r < rank.size(); ++r) {
    if (r != 0) {
      rankOutFile << ",";
    }
    rankOutFile << rank[r];
  }
  rankOutFile.close();

  auto end = get_time::now();
  auto diff = end - start;
  std::cout << "Elapsed time is:  "
            << std::chrono::duration_cast<ms>(diff).count()/1000
            << " seconds."
            << " average: " << std::chrono::duration_cast<ms>(diff).count()/(clusterings.size()*1000)
            << std::endl;

}


struct RankItem {
  size_t index;
  double value;
  RankItem(const size_t idx, const double v): index(idx), value(v) {}

  bool operator < (const RankItem& item) const
  {
    return this->value < item.value;
  }
};

std::vector<size_t> get_rank(const std::vector<double>& vals) {
  std::vector<RankItem> ritems; ritems.reserve(vals.size());
  for (size_t i = 0; i < vals.size(); ++i) {
    ritems.push_back(RankItem(i, vals[i]));
  }
  std::sort(ritems.begin(), ritems.end());

  std::vector<size_t> ranks(vals.size(), -1);
  for (size_t i = 0; i < ritems.size(); ++i) {
    auto ri = ritems[i];
    ranks[ri.index] = i;
  }

  return ranks;
}
