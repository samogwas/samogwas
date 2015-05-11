/****************************************************************************************
 * File: core_em.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 31/03/2015

 ***************************************************************************************/
#ifndef SAMOGWAS_MULTI_EM_HPP
#define SAMOGWAS_MULTI_EM_HPP

#include <cmath>
#include <pl.h> // ProBT library
#include <vector>
#include <memory>
#include "graph/graph.hpp"
namespace samogwas
{

typedef plSymbol Variable;
typedef plVariablesConjunction Variables;

typedef std::vector< std::vector<bool>> DefTab;
typedef std::shared_ptr<DefTab> DefTabPtr;
typedef size_t Cardinality;
typedef size_t Size; 
typedef plEMLearner EMLearner;
typedef std::vector<plLearnObject*> LearnObjectPtrs;
typedef std::vector<EMLearner> CandidateModels;

/** This structure represents a set of attributes that should be returned by
 *  any EM algorithm dedicated to FLTM construction.
 *  Attributes:
 *  - jointDistribution of type ProBT jointDistribution, containing parameters of model
 *    to be learnt by EM (i.e. conditional distribution of each observed variable given
 *    the latent variable and the distribution of the latent variable itself).
 *  - imputedData: a vector of data imputed for the latent variable: this vector contains a value for each observation.
 */


/** Interface common to by any type of EM underlying algorithm.
 *
 */
struct MultiEM {
  
  typedef std::vector< std::vector<int> > Matrix;
  typedef std::shared_ptr<Matrix> MatrixPtr;
  typedef plMatrixDataDescriptor<int> DataDesc;

  MultiEM(int nbrR): nbrRestarts(nbrR) {}
  
  enum ImputationType { ARGMAX = 0, DRAW };
  typedef size_t Cardinality;
  typedef size_t Size;


  virtual void run( GraphPtr graph,
                    Node& latentNode,
                    const double threshold);


 private:
  LearnObjectPtrs create_learn_objects(const Variable & latentVar, const Variables& variables);
  MatrixPtr create_data_table( GraphPtr graph, const std::vector<int>& indexes );
  DefTabPtr create_def_table( size_t rows, size_t cols ); //
  MatrixPtr transpose(MatrixPtr mat );
  plComputableObjectList create_computable_objects( const Variable& latentVar, const Variables& variables );
  EMLearner get_best_model( CandidateModels& learners,
                            DataDesc& dataDesc );
  
  double scoreBIC( EMLearner& learner, plMatrixDataDescriptor<int>& dataDesc );

  void update_parameters ( GraphPtr graph, Node& latentNode, DataDesc& dataDesc, plEMLearner& leaner );
  // void update_parameters( GraphPtr graph, Node& latentNode, const std::vector<plValues>&,
  //                         const std::vector<std::vector<plProbValue>>& ,
  //                         const JointDist& jointDist );
  
  
 private:
  int nbrRestarts;
};



///////////////////////////////////////////////////////////////////


inline double m_log(double v) {
  return v == 0 ? 0 : log(v);
}

inline double log_likelihood_wo( const Node& latentNode, std::vector<int>& obs ) {
  double result = 0.0;
  size_t N = obs.size();// / latentNode.cardinality();
  Graph& graph = *latentNode.graph;
  double rs = 0.0;
  auto cvars = latentNode.get_children_global_indexes();
  for (int i = 0; i < N; ++i) {
    int y = obs[i];
    rs += m_log(latentNode.compute_prob(y));
    for (int j = 0; j < latentNode.nbr_children(); ++j) {
      Node& X = graph[cvars[j]];
      assert(X.dataVec->size() == obs.size());
      int x = X.dataVec->at(i);
      rs += m_log( latentNode.compute_cond_prob(X,x,y) ); 
    }
  }

  return rs;
}
    
inline double log_likelihood( const Node& latentNode )  {
  double result = 0.0;
  size_t N = latentNode.cndObsDist->size() / latentNode.cardinality();

  Graph& graph = *latentNode.graph;
  auto childrenIndexes = latentNode.get_children_global_indexes();
  for (int i = 0; i < N; ++i) {
    for (int y = 0; y < latentNode.cardinality(); ++y) {
      double llh = m_log(latentNode.compute_prob(y));
      for ( auto x_idx: childrenIndexes ) {
        Node& X = graph[x_idx];
        int x = X.dataVec->at(i);
        llh += m_log(latentNode.compute_cond_prob(X,x,y));
      }
      llh *= latentNode.compute_cond_prob_obs(y, i);
      result += llh;
    }
  }
  
  return result;
}


} // namespace  ends here. 

/****************************************************************************************/
#endif // _CORE_EM_HPP
