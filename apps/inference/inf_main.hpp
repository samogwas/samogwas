/****************************************************************************************
 * File: gwas_common.hpp
 * Description:
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 10/12/2014

 ***************************************************************************************/
#ifndef _GWAS_MAIN_HPP
#define _GWAS_MAIN_HPP

#include "graph/graph.hpp"
#include "graph/graph_io.hpp"

#include <vector>
#include <map>
#include <memory>
#include <string>
#include <boost/filesystem/path.hpp>
#include <statistics/association_test.hpp>

////////////////////////////////////////////////////////////////////
typedef std::vector<int> PhenoVec;
typedef std::shared_ptr<PhenoVec> PhenoVecPtr;
typedef std::vector<std::vector<double>> ValueMat;
typedef std::shared_ptr<ValueMat> ValueMatPtr;
typedef std::vector<int> Cardinalities;

ValueMatPtr load_scores(std::string& infile);
ValueMatPtr load_thres(std::string& infile);
PhenoVecPtr load_phenotype(std::string& phenoFile);

// void assure_graph_positions( samogwas::Graph& g );

std::vector<int> get_graph_parent( const samogwas::Graph& graph );
std::vector<int> count_cluster_siblings( samogwas::Graph& graph );
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
#endif // _GWAS_COMMON_HPP
