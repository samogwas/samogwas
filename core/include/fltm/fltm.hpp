/****************************************************************************************
 * File: fltm.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 10/04/2015

 ***************************************************************************************/
#ifndef SAMOGWAS_FLTM_HPP
#define SAMOGWAS_FLTM_HPP

#include <vector>
#include <memory>
#include <boost/lexical_cast.hpp>

#include "graph/graph.hpp"
#include "em/multi_em.hpp"
#include "card_func.hpp"
#include "distance/simi.hpp"
#include "distance/diss.hpp"
#include "clustering/clustering.hpp"
#include "clustering/dbscan.hpp"
#include "clustering/cast.hpp"
#include "fltm/latent_var_criteria.hpp"

namespace samogwas
{

struct FLTM_Params {
  int cardinality;
  int nbrSteps;
  int nbrRestarts;
  double emThres; // controls EM algorithm convergence.
  double latentVarQualityThres;
  unsigned maxDist;
  unsigned seed;
  #ifdef _OPENMP
    int jobsNumber;
  #endif
};

struct FLTM {
  typedef std::vector< std::vector<int> > Matrix;
  typedef std::shared_ptr<Matrix> MatrixPtr;
  typedef std::shared_ptr<ClustAlgo> ClustAlgoPtr;
  typedef std::shared_ptr<CardFunc> CardFuncPtr;
  typedef plSymbol RandVar;
  typedef std::vector<int> Local2Global;
  typedef std::shared_ptr<Local2Global> Local2GlobalPtr;

  FLTM( FLTM_Params &p ): params(p) { }
  void execute( ClustAlgoPtr clt, CardFuncPtr cardF, GraphPtr graph );
 
 private:
  int number_non_singletons( const Clustering &clustering );
  bool accept_latent_variable(const Graph&, Node& node, double qualityThres );
  Node create_latent_node( GraphPtr graph, plSymbol& var, Local2Global& l2g, Label2Index& l2i, Cluster& cluster );
  Node& add_latent_node( Graph& graph, Node& n );
  Label2Index create_index_map( const Graph& graph );
  Local2GlobalPtr create_local_to_global_map(const Graph& graph);
  void update_index_map( Local2Global& l2g, const Local2Global& currentL2G, const Cluster& cluster);
  void update_index_map( Local2Global& l2g, const Local2Global& currentL2G, const Node& latentNode);
  CriteriaPtr create_current_criteria(  Graph& graph, Local2Global& l2g, unsigned MAX_POS, int step);

 private:
  FLTM_Params params;
};


}
/****************************************************************************************/
#endif // SAMOGWAS_FLTM_HPP
