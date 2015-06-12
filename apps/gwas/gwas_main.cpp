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

#include <statistics/association_test.hpp>
#include "statistics/permutation_test.hpp"
#include "statistics/test_statistics.hpp"

#include <utils/csv_parser.hpp>
#include <graph/graph.hpp>
#include <graph/graph_io.hpp>

#include "gwas_main.hpp"
#include "app_options.hpp"
#include "../utils/dataload.hpp"
#include "../utils/tools.hpp"

///////////////////////////////////////////////////////////////////////////////////////
using namespace samogwas;
int main( int argc, char** argv ) {

  auto args = get_gwas_program_options( argc, argv );
  std::cout << "Loading graph data...\n" << std::endl;
  GraphPtr graph;
  BayesGraphLoad graphLoad;
  graphLoad( graph,
             args.inputLabelFile,
             args.bayesVertices,
             args.bayesDist,
             args.cndDataFile,
             args.dataFile );

  Graph& graphRef = *graph;

  printf("Done loading graph of %lu edges and %lu vertices\n", boost::num_edges(graphRef),
         boost::num_vertices(graphRef));

  auto outputPath = get_output_dir(args);
  std::cout << "Loading pheno data from " << args.inputPheno<< std::endl; // todo: logging
  auto pheno = load_phenotype( args.inputPheno );
  std::cout << "Loading label data from " << args.inputLabelFile << std::endl; // todo: logging

  auto labels = std::make_shared<LabelVec>();
  auto positions = std::make_shared<PosVec>();
  auto ids = std::make_shared<PosVec>();
  Label2Index lab2Idx;
  load_labels_positions( *labels, *ids, *positions, args.inputLabelFile );

  std::cout << "Loading mapping snp data from " << args.mappingFile << std::endl; // todo: logging
  auto snp2rs = get_snp_mapping(args.mappingFile);

  std::cout << "retrieving the candidates...\n\n";
  auto candidatesByLevel = get_candidates(graphRef);

  std::cout << "retrieving the cardinalities...\n\n";
  auto cardinalities = get_cardinalities(graphRef);

  // // auto outFileName = ( outputPath / "gwas_p_values_result.txt").string();
  auto stat_test =  get_stat_test(args.stat_test);

  // auto chisq = std::make_shared<ChiSq>();

  std::cout << "start performing test...\n";

  std::vector<double> thresholds {
    0.5,1,1, 1, 1, 1, 1, 1
  };

  perform_test( graphRef,
                snp2rs,
                *pheno,
                stat_test,
                candidatesByLevel,
                cardinalities,
                args.permutations,
                args.chromosome,
                thresholds,
                args.separator,
                outputPath
                );
  // perform_test
  //     ( raph, snp2rs,
  //       *pheno, stat_test,
  //       candidatesByLevel,
  //       cardinalities,
  //       pos.permutations,
  //       pos.chromosome,
  //       thresholds,
  //       outputPath );

}

///////////////////////////////////////////////////////////////////////////////////////
ValueMatPtr load_scores(std::string& infile) {
  auto scores = std::make_shared<ValueMat>();
  std::ifstream scoreFile(infile.c_str());
  if (!scoreFile) {
    std::cout << "score file does not exist. program aborted\n";
    exit(-1);
  }
  samogwas::CSVIterator<double> scoreLine(scoreFile);
  for( ; scoreLine != samogwas::CSVIterator<double>(); ++scoreLine ) {
    std::vector<double> line(scoreLine->size(), 0.0);
    for (size_t i = 0; i < scoreLine->size(); ++i)
      line[i] = scoreLine->at(i);
    scores->push_back(line);
  }

  return scores;
}


PhenoVecPtr load_phenotype(std::string& phenoFile) {
  auto phenoVec = std::make_shared<PhenoVec>();
  std::ifstream labPosFile(phenoFile.c_str());
  if (!labPosFile) {
    std::cout << "phenotype file does not exist. program aborted\n";
    exit(-1);
  }
  samogwas::CSVIterator<std::string> labPosLine(labPosFile);// ++labPosLine;
  for( ; labPosLine != samogwas::CSVIterator<std::string>(); ++labPosLine ) {
    int pheno = boost::lexical_cast<int>( (*labPosLine)[0]);
    phenoVec->push_back(pheno);
  }

  return phenoVec;
}

void assure_graph_positions( samogwas::Graph& g ) {

}

/////////////////////////////////////////////////////////////////////////////////////////

std::vector<int> get_variable_parent( const samogwas::Graph& graph ) {
  std::vector<int> parent( boost::num_vertices(graph), -1 );
  for ( auto ei = boost::edges(graph); ei.first != ei.second; ++ei.first ) {
    auto source = boost::source(*ei.first, graph);
    auto target = boost::target(*ei.first, graph);
    parent[target] = source;
  }

  return parent;
}

std::vector<int> count_cluster_siblings( samogwas::Graph& graph ) {
  std::vector<int> sibling_count( boost::num_vertices(graph) , 0);
  for ( auto vi = boost::vertices(graph); vi.first != vi.second; ++vi.first ) {
    auto v = *vi.first;
    Node& node = graph[v];
    auto ei = boost::out_edges(v, graph);
    int count = std::distance(ei.first, ei.second);
    for ( ; ei.first != ei.second; ++ei.first ) {
      auto child_idx = boost::target(*ei.first, graph);
      sibling_count[child_idx] = count-1;
    }
  }
  return sibling_count;
}

/////////////////////////////////////////////////////////////////////////////////////////

std::map<std::string, std::string> get_snp_mapping(std::string infile) {
  std::map<std::string, std::string> snp2rs;
  std::ifstream mapFile(infile.c_str());
  if (!mapFile) {
    printf("file mapping %s not existing\n", infile.c_str());
    exit(-1);
  }

  samogwas::CSVIterator<std::string> mapLine(mapFile);// ++labPosLine;
  for( ; mapLine != samogwas::CSVIterator<std::string>(); ++mapLine ) {
    auto snp = (*mapLine)[0];
    auto rs = (*mapLine)[1];
    snp2rs[snp] = rs;
  }

  mapFile.close();
}

//////////////////////////////////////////////////////////////
std::shared_ptr<StatTest> get_stat_test( const int test ) {
  std::shared_ptr<StatTest> statTest;

  switch(test) {
    case 0:
      statTest = std::make_shared<ChiSq>();
      break;
    default:
      statTest = std::make_shared<ChiSq>();
      break;
  }

  return statTest;
}

// std::string get_gwas_p_values_path( boost::filesystem::path outDir ) {
//   auto gwas_outFile = ( outDir / "gwas.txt").string();
//   auto parent = get_graph_parent(graph);

//   char gwas_filtered_buffer[80];
//   sprintf( gwas_filtered_buffer, "gwas_filtered.txt");

//   return ( outDir / gwas_filtered_buffer).string();
// }

void perform_test( const samogwas::Graph& graph,
                   std::map<std::string, std::string>& snp2rs,
                   const PhenoVec& pheno,
                   std::shared_ptr<samogwas::StatTest> statTest,
                   const CandidatesByLevel& candidatesByLevel,
                   const Cardinalities &cardinalities,
                   const int permutations,
                   const int chr,
                   const std::vector<double> thresholds,
                   const char sep,
                   boost::filesystem::path outDir) {

  auto gwas_filtered_outFile = ( outDir / "gwas_filtered.txt").string();
  auto gwas_outFile = ( outDir / "gwas.txt").string();
  std::ofstream gwasFile(gwas_outFile), gwasFilteredFile(gwas_filtered_outFile);
  printf("beginning of testing of %d permutations by test: %s\n", permutations, statTest->name.c_str());
  int levels = candidatesByLevel.size();

  std::vector<double> scores( boost::num_vertices(graph), 0.0);

  gwasFile << "chr" << chr << sep << "id" << sep
           << "snp" << sep << "snp-id" << sep
           << "level" << sep << "parent" << sep
           << "position"<< sep << "pval" << sep
           << "corrected-pval" << std::endl;

  for ( int l = 0; l < candidatesByLevel.size(); ++l) {
    auto &candidates = candidatesByLevel[l];
    auto cards = get_cardinalities(graph,candidates);
    std::vector<double> dist;
    std::vector<double> pvals;
    if ( candidates.size() ) {
      printf("\nWe now tests %lu vars - @level: %d over %d\n\n", candidates.size(), l, levels - 1);
      // permutationProcedure( dist,
      //                       pvals,
      //                       statTest,
      //                       pheno,
      //                       candidates, cards,
      //                       2, permutations);
    }
  }

  gwasFile.close(); gwasFilteredFile.close(); // regionFile.close();
  std::cout << "writing to: " << gwas_outFile << std::endl;
  std::cout << "writing to: " << gwas_filtered_outFile << std::endl;
  // std::cout << "writing to: " << region_outFile << std::endl;


}


Cardinalities get_cardinalities( const Graph& graph ) {
  Cardinalities cards;
  for ( auto vp = boost::vertices(graph); vp.first != vp.second; ++vp.first ) {
    auto vertex = *vp.first;
    auto &node = graph[vertex];
    cards.push_back(node.variable.cardinality());
  }
  return cards;
}

Cardinalities get_cardinalities( const Graph& g, const Candidates& candidates ) {
  Cardinalities cards;
  for ( auto cand: candidates ) {
    cards.push_back(g[cand].variable.cardinality());
  }
  return cards;
}

/////////////////////////////////////////////////////////////////////////
