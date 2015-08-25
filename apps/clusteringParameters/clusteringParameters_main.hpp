/****************************************************************************************
 * File: clusteringParameters_main.hpp
 * Description: 
 * @author: jules
 * @date: 29/04/2015

 ***************************************************************************************/
#ifndef SAMOGWAS_CLUSTERINGPARAMETERS_MAIN_HPP
#define SAMOGWAS_CLUSTERINGPARAMETERS_MAIN_HPP

#include "distance/simi.hpp"
#include "distance/diss.hpp"
#include "clustering/clustering.hpp"
#include "clustering/dbscan.hpp"
#include "clustering/cast.hpp"
#include "clustering/louvain/louv.hpp"
#include "graph/graph.hpp"

#include "algo_properties.hpp"
#include "application_options.hpp"
#include "dataload.hpp"

#include <vector>
#include <string>
#include <memory>


namespace samogwas
{

inline Local2GlobalPtr init_index_mapping(const size_t sz) {
  auto l2g = std::make_shared<Local2Global>(sz, -1);
  for (size_t i = 0; i < sz; ++i) {
    l2g->at(i) = i;
  }
  return l2g;
}

inline GraphPtr init_graph( PtrMatrix& mat, Label2Index& lab2Idx, const int CARD, const LabelVec& labels, const PosVec& positions ) {
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

typedef std::shared_ptr<samogwas::PositionCriteria> PositionCriteriaPtr;

struct ClusterInformation {
  int nbSingletons = 0;
  int clusterScore = 0;
  double mutualInformation = 0;


};

void writeResultsForTulip( samogwas::Clustering clustering, std::shared_ptr<samogwas::PosVec> positions, std::shared_ptr<samogwas::PosVec> ids, std::shared_ptr<samogwas::LabelVec> labels, std::string clusteringMethod);
void printClusteringInformations( samogwas::Clustering clustering );
ClusterInformation getClusteringInformations( samogwas::Clustering clustering, samogwas::SimiPtr diss );
ClusterInformation getClusteringInformations( samogwas::Clustering clustering, samogwas::DissPtr diss );
samogwas::Clustering computeConsensusCluster(std::vector<samogwas::Clustering> clusterings);
double getObservedEntropy( samogwas::Cluster cluster, samogwas::PtrMatrixPtr mat);
double getExpectedEntropy( samogwas::Cluster cluster, samogwas::PtrMatrixPtr mat);
double getEpsilon(samogwas::Clustering clustering, samogwas::PtrMatrixPtr mat);
samogwas::Clustering runClustering(samogwas::GraphPtr graph, samogwas::Local2GlobalPtr l2g, PositionCriteriaPtr criteria, int clusteringChoice);

#endif // SAMOGWAS_CLUSTERINGPARAMETERS_MAIN_HPP
