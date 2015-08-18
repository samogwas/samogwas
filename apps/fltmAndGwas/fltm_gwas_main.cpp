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
#include <statistics/association_test.hpp>
#include "statistics/permutation_test.hpp"
#include "statistics/test_statistics.hpp"


#include "graph/graph_io.hpp"

#include "fltm_gwas_main.hpp"
#include "dataload.hpp"
#include "algo_properties.hpp"


#ifdef _BOINC
#include <boinc/boinc_api.h>
#endif

using namespace samogwas;
using ms = std::chrono::milliseconds;
using get_time = std::chrono::steady_clock ;
typedef std::shared_ptr<PositionCriteria> PositionCriteriaPtr;

boost::filesystem::path outputDir( Options& progOpt );
char* current_date();

int main( int argc, char** argv ) {
  auto options = get_program_options( argc, argv );
  auto labels = std::make_shared<LabelVec>();
  auto positions = std::make_shared<PosVec>();
  auto ids = std::make_shared<PosVec>();

#ifdef _BOINC
  /* BOINC Initialisation */
  BOINC_OPTIONS b_options;
		
  boinc_options_defaults(b_options);
  b_options.multi_thread = true;

  boinc_init_options(&b_options);  
#endif


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
//   for ( auto cltConf: clustAlgoConfs ) {
     auto graph = init_graph( *mat,  lab2Idx, options.fltm_params.cardinality, *labels, *positions );
     auto l2g = init_index_mapping( mat->size() );
     auto algoClust = read_clustering_algo( *clustAlgoConfs.begin(), graph, l2g, positions, options.fltm_params.maxDist);
#ifdef _BOINC
	 if (boinc_time_to_checkpoint()){
	   boinc_checkpoint_completed();
	 }
#endif

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

//   }
   stats.close();
/////FLTM end//////
/////GWAS begin////

   auto thresholds = read_thresholds(options.thresholdFile);


   Graph& graphRef = *graph;

   printf("Done loading graph of %lu edges and %lu vertices\n", boost::num_edges(graphRef),
          boost::num_vertices(graphRef));


   std::cout << "Loading pheno data from " << options.inputPheno<< std::endl; // todo: logging
   auto pheno = load_phenotype( options.inputPheno );
//   std::cout << "Loading label data from " << options.inputLabelFile << std::endl; // todo: logging

   std::cout << "Loading mapping snp data from " << options.mappingFile << std::endl; // todo: logging
   auto snp2rs = get_snp_mapping(options.mappingFile);

   std::cout << "retrieving the candidates...\n\n";
   auto candidatesByLevel = get_candidates_by_level(graphRef);

   std::cout << "retrieving the cardinalities...\n\n";
   auto cardinalities = get_cardinalities(graphRef);

   auto stat_test =  get_stat_test(options.stat_test);

   std::cout << "start performing test...\n";

   perform_test( graphRef,
                 snp2rs,
                 *pheno,
                 stat_test,
                 candidatesByLevel,
                 cardinalities,
                 options.permutations,
                 options.chromosome,
                 thresholds,
                 options.separator,
                 outputPath
                 );

   std::cout << "bye...\n";

#ifdef _BOINC
   boinc_finish(0);
#endif


}

//FLTM
boost::filesystem::path outputDir( Options& progOpt ) {
  auto path = boost::filesystem::absolute(progOpt.outputDir);
  path /= current_date();
  boost::filesystem::create_directories(path);
  return path;
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


//GWAS


///////////////////////////////////////////////////////////////////////////////////////


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

std::map<std::string, std::string> get_snp_mapping(std::string& infile) {
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
  return snp2rs;
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

CandidatesByLevel get_candidates_by_level( const samogwas::Graph& graph ) {
  int max_level = -1;
  for ( auto vp = boost::vertices(graph); vp.first != vp.second; ++vp.first ) {
    auto vertex = *vp.first;
    auto &node = graph[vertex];
    max_level = std::max( node.level, max_level );
  }

  CandidatesByLevel candidates(max_level+1, Candidates());
  for ( auto vp = boost::vertices(graph); vp.first != vp.second; ++vp.first ) {
    auto vertex = *vp.first;
    auto &node = graph[vertex];
    int level = node.level;
    candidates[level].push_back(vertex);
  }

  return candidates;
}


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
  auto gwas_tulip_outFile = ( outDir / "gwas_tulip.csv").string();
  std::ofstream gwasFile(gwas_outFile), gwasFilteredFile(gwas_filtered_outFile), gwasTulipFile(gwas_tulip_outFile);
  printf("beginning of testing of %d permutations by test: %s\n", permutations, statTest->name.c_str());
  int levels = candidatesByLevel.size();

  std::vector<double> scores( boost::num_vertices(graph), 0.0);
  auto parent =  get_graph_parent(graph);

  gwasFile << "chr" << chr << sep << "id" << sep
           << "snp" << sep << "snp-id" << sep
           << "level" << sep << "parent" << sep
           << "position"<< sep << "pval" << sep
           << "corrected-pval" << std::endl;

  gwasTulipFile << "id" << sep << "parent_id" << sep
                << "label" << sep << "level" << sep
                << "cardinality" << sep << "position"<< sep
                << "pval" << sep << "corrected-pval" << std::endl;

  for ( int l = 0; l < candidatesByLevel.size(); ++l) {
    auto &candidates = candidatesByLevel[l];
    auto cards = get_cardinalities(graph,candidates);
    std::vector<double> dist;
    std::vector<double> pvals;
    if ( candidates.size() ) {
      printf("\nWe now tests %lu vars - @level: %d over %d\n\n", candidates.size(), l, levels - 1);
      permutation_procedure( dist,
                             pvals,
                             statTest,
                             graph,
                             pheno,
                             candidates, cards,
                             2, permutations);

      for ( int i = 0; i < candidates.size(); ++i ) {
        auto cand = candidates[i];
        std::string rs = "";
        if ( snp2rs.find(graph[cand].label) != snp2rs.end() ) {
          rs =  snp2rs[graph[cand].label];
        } else {
          rs = "NA";
        }
        scores[cand] =  pvals[2*i+1];
        gwasFile << "chr" << chr << sep
                 << graph[cand].index << sep
                 << graph[cand].label << sep
                 << rs << sep
                 << l << sep
                 << parent[cand] << sep
                 << graph[cand].position << sep
                 << pvals[2*i] << sep
                 << pvals[2*i+1] << std::endl;

        gwasTulipFile << graph[cand].index << sep
                 << parent[cand] << sep
                 << graph[cand].getLabel() << sep
                 << l << sep
                 << graph[cand].variable.cardinality() << sep
                 << graph[cand].position << sep
                 << pvals[2*i] << sep
                 << pvals[2*i+1] << std::endl;

        if ( scores[cand] < thresholds[l]) {
          Node node = graph[cand];
          size_t sz_start = std::numeric_limits<int>::max(), sz_end = 0;
          size_t sum = 0, pos = 0, count = 0;
          for ( auto ei = boost::out_edges(cand, graph); ei.first != ei.second; ++ei.first ) {
            auto child = graph[boost::target(*ei.first, graph)];
            sz_start = std::min(sz_start, (size_t)child.position);
            sz_end = std::max(sz_end, (size_t)child.position);
            count++;
          }
          gwasFilteredFile << "chr" << chr << sep
                           << cand << sep
                           << graph[cand].label << sep
                           << l << sep
                           << parent[cand] << sep
                           << graph[cand].position << sep
                           << pvals[2*i] << sep
                           << pvals[2*i+1] << std::endl;

          if ( sz_start != sz_end && sz_start != node.position && sz_end != node.position  && count > 1 ) {
            printf("level: %d, var: %lu, score: %f, thres: %f, start: %lu, end: %lu\n",
                   l, cand, scores[cand], thresholds[l], sz_start, sz_end);
            if (sz_start < 0) {
              printf("level: %d, var: %lu, score: %f, thres: %f, start: %lu, end: %lu\n",
                     l, cand, scores[cand], thresholds[l], sz_start, sz_end);
              exit(-1);
            }
          }
        }
      }
    }
  }
  gwasFile.close(); gwasFilteredFile.close(); gwasTulipFile.close(); // regionFile.close();
  std::cout << "writing to: " << gwas_outFile << std::endl;
  std::cout << "writing to: " << gwas_filtered_outFile << std::endl;
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

std::vector<int> get_graph_parent( const samogwas::Graph& graph ) {
  std::vector<int> parent( boost::num_vertices(graph), -1 );
  for ( auto ei = boost::edges(graph); ei.first != ei.second; ++ei.first ) {
    auto source = boost::source(*ei.first, graph);
    auto target = boost::target(*ei.first, graph);
    parent[target] = source;
  }
  return parent;
}

////////////////////////////////////////////////////////
std::vector<double> read_thresholds(std::string filename) {
  std::ifstream thresholdFile(filename.c_str());
  if (!thresholdFile) {
    printf("file data %s not existing\n", filename.c_str());
    exit(-1);
  }

  samogwas::CSVIterator<double> thresholdLine(thresholdFile);
  //auto row = std::make_shared<Vec>(thresholdLine->size(), 0);
  std::vector<double> thresholds(thresholdLine->size(), 0.0);
  for (unsigned i = 0; i < thresholdLine->size(); ++i) {
    thresholds[i] = thresholdLine->at(i);
  }

  return thresholds;

}

