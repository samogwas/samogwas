
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


#include "clustering/measures/expected_mutual_info.hpp"
#include "clustering/clustering.hpp"

#include "dataload.hpp"
#include "app_options.hpp"
#include "main.hpp"

using ms = std::chrono::milliseconds;
using get_time = std::chrono::steady_clock ;
using namespace samogwas;

char* time_taken(unsigned long seconds);

boost::filesystem::path outputDir( Options& progOpt );
char* current_date();
void saveResult( const Graph& g, const std::string& dat, const std::string& lab);

void read_clusterings(std::vector<std::vector<int>>& clusterings, std::string clustsFile);
void read_ref_clustering(std::vector<int>& clustering, std::string refClustFile);

std::vector<size_t> rank_data(std::vector<double>& vals);
/**
 *
 */
int main(int argc, char* argv[]) {
  auto options = get_program_options( argc, argv );

  std::vector<std::vector<int>> clusterings;
  std::vector<int> refClustering;
  std::cout << "reading clustering...\n";
  read_clusterings(clusterings, options.inputClusterings);
  std::cout << "reading ref clustering...\n";
  read_ref_clustering(refClustering, options.inputRefClustering);


  auto start = get_time::now();
  auto outputPath = outputDir(options);
  boost::filesystem::create_directories(outputPath);
  std::ofstream resultOutput( (outputPath / "log.txt").string() );

  std::vector<double> vals;

  Partition refPartition(refClustering);
  for (size_t i = 0; i < clusterings.size(); ++i) {
    Partition p(clusterings[i]);
    vals.push_back(adjusted_mutual_information(p, refPartition));
    std::cout << "computing: " << i << "...\n";
  }
  std::ofstream stats( (outputPath / "log.txt").string() );
  stats << "Parameter options : " << std::endl;
  for (int i = 0 ; i < argc ; i++ ) { stats << "\t" << argv[i] << std::endl; }

  auto end = get_time::now();
  auto diff = end - start;
  stats << "Elapsed time is:  "
        << time_taken(std::chrono::duration_cast<ms>(diff).count()/1000)
        << std::endl;

  auto ranks = rank_data(vals);

  std::ofstream valOut( (outputPath / "vals.txt").string() );
  std::ofstream rankOut( (outputPath / "ranks.txt").string() );
  for (size_t i = 0; i < clusterings.size()-1; ++i) {
    valOut << vals[i] << ",";
    rankOut << ranks[i] << ",";
  }
  valOut << vals[clusterings.size()-1];
  rankOut << ranks[clusterings.size()-1];
  stats.close(); rankOut.close(); valOut.close();
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


boost::filesystem::path outputDir( Options& progOpt ) {
  auto path = boost::filesystem::absolute(progOpt.outputDir);
  path /= current_date();
  boost::filesystem::create_directories(path);
  return path;
}


struct RankObject {
  double val;
  size_t index;
  RankObject(const double v, const size_t idx): val(v), index(idx) {}

  bool operator < (const RankObject& other) const {
    return this->val < other.val;
  }
};

std::vector<size_t> rank_data(std::vector<double>& vals) {
  std::vector<size_t> ranks(vals.size(), -1);

  std::vector<RankObject> objects; objects.reserve(vals.size());
  for (size_t i = 0; i < vals.size(); ++i) {
    objects.push_back(RankObject(vals[i], i));
  }
  std::sort(objects.begin(), objects.end());

  for (size_t i = 0; i < vals.size(); ++i) {
    auto index = objects[i].index;
    ranks[index] = i + 1;
  }

  return ranks;
}


void read_clusterings(std::vector<std::vector<int>>& dt, std::string clustsFile) {
  std::ifstream matrixFile(clustsFile.c_str());
  if (!matrixFile) {
    printf("clustsFile data %s not existing\n", clustsFile.c_str());
    exit(-1);
  }

  samogwas::CSVIterator<int> matrixLine(matrixFile);

  for( ; matrixLine != samogwas::CSVIterator<int>(); ++matrixLine ) {
    Vec row(matrixLine->size(), 0);
    for (unsigned i = 0; i < matrixLine->size(); ++i) {
      row[i] = matrixLine->at(i);
    }
    dt.push_back(row);
  }
  dt.resize(dt.size());
  matrixFile.close();
}

void read_ref_clustering(std::vector<int>& clustering, std::string refClustFile) {
  std::ifstream clusteringFile(refClustFile.c_str());
  if (!clusteringFile) {
    printf("refClustFile data %s not existing\n", refClustFile.c_str());
    exit(-1);
  }

  samogwas::CSVIterator<int> line(clusteringFile);
  clustering.resize(line->size(), 0);
  for (unsigned i = 0; i < line->size(); ++i) {
    clustering[i] = line->at(i);
  }
  clusteringFile.close();
}
