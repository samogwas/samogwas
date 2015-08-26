/****************************************************************************************
 * File: DataGeneration.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 08/04/2014

 ***************************************************************************************/
#ifndef DATA_GEN_DATAGENERATION_HPP
#define DATA_GEN_DATAGENERATION_HPP

#include "pl.h"

#include <boost/random.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_01.hpp>
#include <queue>
#include <map>

#include "clustering/clustering.hpp"
#include "clustering/dbscan.hpp"
#include "clustering/cast.hpp"
#include "graph/graph.hpp"

#include "dataload.hpp"

#include <vector>
#include <string>
#include <memory>

typedef std::vector<int> Vec;
typedef std::vector< Vec > Matrix;
typedef std::shared_ptr<Vec> VecPtr;
typedef std::vector<VecPtr> NovoMatrix;
typedef std::shared_ptr<NovoMatrix> NovoMatrixPtr;

typedef plSymbol Variable;
typedef plVariablesConjunction Variables;

typedef boost::uniform_real<> RealDistribution; // choose a distribution
typedef boost::uniform_int<> NumberDistribution; // choose a distribution
typedef boost::mt19937 RandomNumberGenerator;    // pick the random number generator method,
typedef boost::variate_generator< RandomNumberGenerator&, NumberDistribution > Generator;  // link the generator to the distribution

typedef std::vector<float> DistValues ;
typedef std::vector<DistValues> DistValueVec;
typedef std::vector<DistValueVec> DistValueMat;
static RandomNumberGenerator generator(std::time(0));

inline Generator getUniformGenerator(const float rangeMin, const float rangeMax);
inline plJointDistribution createNBJointDist(Variables& variables, Variable& latentVar);
inline const DistValues createNBUniVarProbTab(const size_t card);
inline DistValueMat createNBProbTables( const Variables& variables,
                                        const Variable& latentVar);
inline plComputableObjectList createNBCndTable( Variables& variables, Variable& latentVar);
///////////////////////////////////////////////////////////////////////////////////////////////
// inline plComputableObjectList createClusterJointDist( const Variables& variables, const Cluster& cluster );
// inline DistValueMat createClusterProbTables( const Variables& variables, const Cluster& cluster );

inline void normalizeDistValues(DistValues& distValues);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline plJointDistribution generate_models(const unsigned ntrees, // std::string rootName,
                                           unsigned levels,
                                           unsigned children, unsigned card,
                                           Variables& observedVars);

inline Variables create_joint_variables(const Variable& root, const unsigned levels,
                                        const unsigned children, const unsigned card,
                                        std::map<std::string,Variables>& mapper,
                                        Variables& observedVars);

inline plComputableObjectList create_random_marginal(const Variable& root);
inline plComputableObjectList create_LCM(Variables& variables, Variable& latentVar);
inline Variables get_observed_vars( const std::map<std::string,Variables>& mapper);

//inline std::vector<std::vector<

///////////////////////////////////////////////////////////////////////////////////////////////////////////////


inline Variables create_joint_variables(const Variable& root, const unsigned levels,
                                        const unsigned nchildren, const unsigned card,
                                        std::map<std::string,Variables>& mapper,
                                        Variables& observedVars) {
  Variables variables = root;
  std::queue<plSymbol> q;
  q.push(root);

  for (int level = levels - 1; level >= 0; level--) {
    std::queue<plSymbol> tempQ;
    while(!q.empty()) {
      auto r = q.front();
      q.pop();
      Variables children;
      for (unsigned c = 0; c < nchildren; ++c) {
        plSymbol var(r.name() + "-" + boost::lexical_cast<std::string>(c),
                     // + "-" + boost::lexical_cast<std::string>(level),
                     plIntegerType(0, card-1));
        tempQ.push(var);
        variables ^= var;
        children ^= var;
      }
      mapper[r.name()] = children;
      if (level == 0) {
        observedVars ^= children;
      }
    }
    q = tempQ;
  }
  return variables;
}



plComputableObjectList createNBCndTable(Variables& variables, Variable& latentVar) {
  plComputableObjectList dataCndTable;

  DistValueMat probTableXZ = createNBProbTables(variables, latentVar);

  for (size_t x = 0; x < variables.size(); ++x) {
    plDistributionTable distTab_Xi_Z(variables[x], latentVar);

    for (size_t h = 0; h < latentVar.cardinality(); ++h) {
      distTab_Xi_Z.push( plProbTable(variables[x], probTableXZ[x][h]), (int)h );
    }
    dataCndTable *= distTab_Xi_Z; // adds the conditional table to result
  }

  return dataCndTable;
}

plComputableObjectList create_LCM(Variables& variables, Variable& latentVar) {

  auto probValZ = createNBUniVarProbTab(latentVar.cardinality()) ;
  const plProbTable margDist(latentVar, probValZ, true);

  auto cndDist = createNBCndTable(variables, latentVar);

  return margDist*cndDist;

}

DistValueMat createNBProbTables( const Variables& variables,  const Variable& latentVar) {
  DistValueMat probTableX;
  for (size_t var = 0; var < variables.size(); ++var) {
    DistValueVec X_Z;
    for (size_t i = 0; i < latentVar.cardinality(); ++i) {
      const DistValues X_Z_i = createNBUniVarProbTab( variables[var].cardinality() );
      X_Z.push_back(X_Z_i);
    }
    probTableX.push_back(X_Z);
  }
  return probTableX;
}

const DistValues createNBUniVarProbTab(const size_t cardinality) {

  Generator generator = getUniformGenerator(2,5);
  float total = 0.0;

  DistValues probTable( cardinality );
  for (size_t val = 0; val < cardinality; ++val) {
    probTable[val] = generator();
    total += probTable[val];
  }

  for (size_t val = 0; val < cardinality; ++val) {
    probTable[val] /= total;
  }

  return probTable;
}

Generator getUniformGenerator(const float rangeMin, const float rangeMax) {
  NumberDistribution distribution( rangeMin, rangeMax );
  Generator numberGenerator(generator, distribution);
  return numberGenerator;
}

// inline Variables get_observed_vars(const std::map<std::string,Variables>& mapper) {
//   Variables vars;
//   for (auto v: mapper) {
//     // if (v.first )
    
//   }
//   return vars;
// }

inline samogwas::Local2GlobalPtr init_index_mapping(const size_t sz) {
  auto l2g = std::make_shared<samogwas::Local2Global>(sz, -1);
  for (size_t i = 0; i < sz; ++i) {
    l2g->at(i) = i;
  }
  return l2g;
}

inline samogwas::GraphPtr init_graph( samogwas::PtrMatrix& mat, samogwas::Label2Index& lab2Idx,
                            const int CARD, const samogwas::LabelVec& labels,
                            const samogwas::PosVec& positions ) {
  auto graph = std::make_shared<samogwas::Graph>();

  auto nrows = positions.size();
  for (int i = 0; i < nrows; ++i) {
    plSymbol v(labels.at(i), plIntegerType(0, CARD-1) );
    auto dataVec = mat.at(i); //std::make_shared<DataVec>(data->at(i));
    createObsNode(graph, v, dataVec, positions.at(i), lab2Idx);
  }

  return graph;
}



/****************************************************************************************/
#endif // DATA_GEN_DATAGENERATION_HPP
