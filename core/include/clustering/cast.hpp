/****************************************************************************************
 * File: n_cast.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 01/04/2015

 ***************************************************************************************/
#ifndef SAMOGWAS_N_CAST_HPP
#define SAMOGWAS_N_CAST_HPP

#include <memory>
#include "clustering.hpp" // AlgoClust
#include "distance/simi.hpp"

namespace samogwas
{

/** The CAST method returns a partition of the dataset, much like other types of AlgoClust.
 * Under the hood, during the computation, the private class CAST_Item is used to represent an 
 * item in the cluster. CAST_Item is not supposed to be used by an outside class.
 * CAST_Item provides a convenient way for holding, for each item, its current affinity relative to 
 * the current candidate Opencluster, and its (global) index relative to the original dataset.
 */
struct CAST_Item {
  int globalIndex;
  double affinity;
  CAST_Item( const int matIdx, const double aff): globalIndex(matIdx), affinity(aff) {}
};

/** CAST is a functor that implements AlgoClust interface for data clustering.
 *  It takes as input a similarity matrix and a 
 *  threshold parameter thres (the same as t described above) in the constructor.
 */
struct CAST: public ClustAlgo {
  typedef std::vector<CAST_Item> CAST_Cluster;

  /** Constructor takes a similarity matrix and a threshold criterion for removing and adding an item from the current 
   * candidate Opencluster.
   *
   */
  CAST ( SimiPtr s, const double& thres): simi(s), thresCAST(thres) {}


  virtual double measure(const size_t a, const size_t b) {
    return simi->compute(a,b);
  }

  virtual void invalidate() {
    if (simi) {
      simi->invalidate_entropy_cache();
    }
  }

  /** Executes the algorithm given the current input (SimiMatrix).
   *
   */
  virtual std::shared_ptr<AbstractPartition> run() {
    /// Initializes the cluster unAssignedCluster, grouping all the unassigned items.
    CAST_Cluster unAssignedCluster;
    auto nvars = simi->nbr_variables();
    for ( unsigned i = 0; i < nvars; ++i ) { // SimilarityMatrix has type SimiMatrix (for instance
      // MutInfoSimilarityMatrix)
      // @todo: remove direct access, and change the name
      unAssignedCluster.push_back(CAST_Item(i, 0.0) );
    }
    /// Delegates the job to the method taking the newly initialized unassignedCluster as parameter.
    return run( unAssignedCluster );
  }

  /** Provides the name of the algorithm along with its parameters *
   */
  virtual char* name() const {
    char* name = new char[80];
    sprintf( name, "CAST_%.3f", thresCAST);
    return name;
  }

  virtual CriteriaPtr get_criteria() { return simi->get_criteria(); }

  virtual inline void set_measure(GraphPtr g, Local2GlobalPtr& l2gp, CriteriaPtr criteria = nullptr);

 protected:
  /** Performs on an unAssignedCluster, setup by the run() method above
   */
  inline std::shared_ptr<AbstractPartition> run( CAST_Cluster& unassignedCluster );

  /** Resets the affinities of the items in unAssignedCluster */
  inline void resetAffinities( CAST_Cluster& unAssignedCluster );


  // Adds good item to OpenCluster and removes it from unassignedCluster
  inline void addGoodItem( CAST_Cluster& unassignedCluster, CAST_Cluster& openCluster,
                           GraphSimilarity& simMatrix, const int itemIdx );

  /** Removes bad item from openCluster and moves it back to unassignedCluster  */
  inline void removeBadItem( CAST_Cluster& unassignedCluster, CAST_Cluster& openCluster,
                             GraphSimilarity& simMatrix, const int itemIdx );

  /** Moves the item indexed by itemIdx from sourceCluster, puts into targetCluster and then updates the affinity. */
  inline void updateClustersAffinity( CAST_Cluster& sourceCluster, CAST_Cluster& targetCluster,
                                      GraphSimilarity& simMatrix, const int itemIdx );

  /** Moves item indexed by itemIdx from sourceCluster to targetCluster  */
  inline void moveItemBetweenClusters( CAST_Cluster& source, CAST_Cluster& target, const int itemIdx );


 protected:
  // main parameter of CAST
  SimiPtr simi;
  double thresCAST;

};

//////////////////////////////////////////////////////////////////////////////

/** Functor to find the index of the item of extremal affinity (mimimal or maximal).
 *
 */
struct ExtremumIndexAffinity {
  template<typename Compare>
  const int operator()(const std::vector<CAST_Item>& cluster, Compare comparat) const;
};



/** Implements the heuristic CAST algorithm following the original paper mentioned in the header.
 *  This heuristic requires as input a similarity matrix ( or any method that returns a similarity between
 *  a pair of item indexes).
 .  It also requires a parameter t as the affinity threshold which determines whether we remove
 *  or add an item to the current candidate Opencluster. This parameter t also determines when a cluster is stabilized.
 *  This method returns a partition of the dataset.
 */
std::shared_ptr<AbstractPartition> CAST::run( CAST_Cluster& unAssignedCluster ) {
  auto result = std::make_shared<NPartition>();
  while ( unAssignedCluster.size() ) {  // as long as there still remains an object to be classified (i.e. unassigned)
    CAST_Cluster openCluster; // creates a new cluster
    resetAffinities( unAssignedCluster ); // reset the affinities of the unassigned objects
    bool changesOccurred = true;
    while (changesOccurred && unAssignedCluster.size()) { // starts assigning objects to openCluster
      changesOccurred = false;
      ExtremumIndexAffinity maxCompute;
      ExtremumIndexAffinity minCompute;
      while ( unAssignedCluster.size() ) { // while there is still an object to be assigned,
        // successively tries to assign the objects with the best affinities to openCluster
        int maxAffIdx = maxCompute( unAssignedCluster, std::greater<double>() );
        if ( unAssignedCluster.at(maxAffIdx).affinity >= thresCAST * openCluster.size() ) {
          changesOccurred = true;
          addGoodItem( unAssignedCluster, openCluster, *this->simi, maxAffIdx );
        } else {
          break;
        }
      }
      while( unAssignedCluster.size() ) { // while there is still an object to be assigned,
        // successively tries to remove the objects with the worst affinities from openCluster
        int minAffIdx = minCompute( openCluster, std::less<double>() );
        if ( openCluster.at(minAffIdx).affinity < thresCAST * openCluster.size() ) {
          changesOccurred = true;
          removeBadItem( unAssignedCluster, openCluster, *this->simi, minAffIdx );
        } else {
          break;
        }
      }
    }  // until stabilized
    // puts the newly created openCluster into the current partition and continues
    int cluster_id =  result->nbr_clusters();
    for ( auto& item: openCluster ) {
      result->setLabel( item.globalIndex, cluster_id );
    }
  }
  return result;
}

//////////////////////////////////////////////////////////////////////////////////////
// Sets all the affinity values to zeroes.
void CAST::resetAffinities( CAST_Cluster& cluster ) {
  for ( CAST_Cluster::iterator it = cluster.begin(); it != cluster.end(); ++it ) {
    it->affinity = 0.0;
  }
}

//////////////////////////////////////////////////////////////////////////////////////
// Delegates the task to updateClusterAffinity which moves the item indexed by itemIdx from unassignedCluster
// to openCluster and update the affinity correspondingly.
void CAST::addGoodItem( CAST_Cluster& unAssignedCluster, CAST_Cluster& openCluster,
                        GraphSimilarity& simiCompute, const int itemIdx ) {
  updateClustersAffinity( unAssignedCluster, openCluster,
                          simiCompute, itemIdx );
}

//////////////////////////////////////////////////////////////////////////////////////
// Delegates the task to updateClusterAffinity which moves back the item indexed by itemIdx from openCluster to
// unassignedCluster and update the affinity correspondingly
void CAST::removeBadItem( CAST_Cluster& unAssignedCluster, CAST_Cluster& openCluster,
                          GraphSimilarity& simiCompute, const int itemIdx ) {
  updateClustersAffinity( openCluster, unAssignedCluster,
                          simiCompute, itemIdx);
}

//////////////////////////////////////////////////////////////////////////////////////
// Updates the affinity values in sourceCluster and targetCluster after the move of indeXItem.
void CAST::updateClustersAffinity( CAST_Cluster& sourceCluster, CAST_Cluster& targetCluster,
                                   GraphSimilarity& simiCompute,
                                   const int itemIdx )
{
  const int itemGlobalIndex = sourceCluster.at(itemIdx).globalIndex;
  moveItemBetweenClusters( sourceCluster, targetCluster, itemIdx );
  for (unsigned i = 0; i < sourceCluster.size(); i++) {
    sourceCluster[i].affinity += simiCompute.compute( sourceCluster[i].globalIndex, itemGlobalIndex );
  }

  for (unsigned i = 0; i < targetCluster.size(); i++) {
    targetCluster[i].affinity += simiCompute.compute( targetCluster[i].globalIndex, itemGlobalIndex );
  } 

}

//////////////////////////////////////////////////////////////////////////////////////
// Moves item from source cluster to target cluster.
void CAST::moveItemBetweenClusters( CAST_Cluster& source,
                                    CAST_Cluster& target,
                                    const int itemIdx ) {
  target.push_back( source.at(itemIdx) );
  source.erase( source.begin() + itemIdx );
}

////////////////////////////////////////////////////////////////////////////////////////
/** Searches for the object in given cluster which has the optimal (best/worst) affinity.
 *
 */
template<typename Compare>
const int ExtremumIndexAffinity::operator()( const std::vector<CAST_Item>& cluster, Compare comparat) const
{
  int result = 0;
  for (unsigned idx = 1; idx < cluster.size(); idx++) {
    if ( comparat(cluster.at(idx).affinity, cluster.at(result).affinity) ) {
      result = idx;
    }
  }

  return result;
}

void CAST::set_measure(GraphPtr graph, Local2GlobalPtr& l2gp, CriteriaPtr criteria ) {
  this->invalidate();
  this->simi = std::make_shared<GraphMutInfoSimilarity>(graph, l2gp);
  this->simi->set_criteria(criteria);
}

} // namespace samogwas ends here.

/****************************************************************************************/
#endif // SAMOGWAS_N_CAST_HPP
