/****************************************************************************************
 * File: fltm_main.hpp
 * Description:
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 29/04/2015

 ***************************************************************************************/
#ifndef SAMOGWAS_FLTM_GWAS_MAIN_HPP
#define SAMOGWAS_FLTM_GWAS_MAIN_HPP

#include "statistics/association_test.hpp"
#include "clustering/clustering.hpp"
#include "clustering/dbscan.hpp"
#include "clustering/cast.hpp"
#include "graph/graph.hpp"


#include "algo_properties.hpp"

#include "dataload.hpp"

#include <vector>
#include <map>
#include <string>
#include <memory>
#include <boost/filesystem/path.hpp>


///FLTM
namespace samogwas
{

inline Local2GlobalPtr init_index_mapping(const size_t sz) {
  auto l2g = std::make_shared<Local2Global>(sz, -1);
  for (size_t i = 0; i < sz; ++i) {
    l2g->at(i) = i;
  }
  return l2g;
}

inline GraphPtr init_graph( PtrMatrix& mat, Label2Index& lab2Idx,
                            const int CARD, const LabelVec& labels,
                            const PosVec& positions ) {
  auto graph = std::make_shared<Graph>();

  auto nrows = positions.size();
  for (int i = 0; i < nrows; ++i) {
    plSymbol v(labels.at(i), plIntegerType(0, CARD-1) );
    auto dataVec = mat.at(i); //std::make_shared<DataVec>(data->at(i));
    createObsNode(graph, v, dataVec, positions.at(i), lab2Idx);
  }

  return graph;
}


} // namespace samogwas ends here.

///GWAS

typedef std::vector<int> PhenoVec;
typedef std::shared_ptr<PhenoVec> PhenoVecPtr;
typedef std::vector<std::vector<double>> ValueMat;
typedef std::shared_ptr<ValueMat> ValueMatPtr;
typedef std::vector<int> Cardinalities;

ValueMatPtr load_thres(std::string& infile);
PhenoVecPtr load_phenotype(std::string& phenoFile);

// void assure_graph_positions( samogwas::Graph& g );

std::vector<int> get_graph_parent( const samogwas::Graph& graph );
std::map<std::string, std::string> get_snp_mapping( std::string& infile);

Cardinalities get_cardinalities( const samogwas::Graph& graph );

typedef std::vector<size_t> Candidates;
typedef std::vector<Candidates> CandidatesByLevel;
typedef std::vector<int> Cardinalities;

CandidatesByLevel get_candidates_by_level( const samogwas::Graph& g );
Cardinalities get_cardinalities( const samogwas::Graph& g );
Cardinalities get_cardinalities( const samogwas::Graph& g, const Candidates& candidates );

std::shared_ptr<samogwas::StatTest> get_stat_test( const int test );

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
                   boost::filesystem::path outDir);

std::vector<double> read_thresholds(std::string filename);
/****************************************************************************************/
#endif // SAMOGWAS_FLTM_GWAS_MAIN_HPP
