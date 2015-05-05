/****************************************************************************************
 * File: n_dbscan.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 01/04/2015

 ***************************************************************************************/
#ifndef SAMOGWAS_N_DBSCAN_HPP
#define SAMOGWAS_N_DBSCAN_HPP

#include <memory>
#include "clustering.hpp" // AlgoClust
#include "distance/diss.hpp"
namespace samogwas
{


/** DBSCAN is a functor that implements AlgoClust interface for data clustering.
*   The DBSCAN method returns a partition of the dataset, much like other types of AlgoClust.
*/
 
struct DBSCAN: public ClustAlgo {
  enum { UNASSIGNED_LABEL = -1 };
  typedef int Index;
  typedef std::vector<Index> Neighbors; // An important concept of DBSCAN involves Neighbors, a set of nearby points.typedef int Index;
    
  typedef int Label;
  typedef std::vector<Index> LabelSet; // A Label maps a point Index to its cluster Index.
                                     // A  for instance, may be instanciated as MutInfoSimilarityMatrix label indicates that the point is not yet assigned to any cluster.
  /** The constructor takes three parameters: 
   * a distance matrix that holds the dissimilarities between pairs of points, 
   * the minimum number of points to form a dense neighborhood, 
   * and the maximum radius (epsilon) of the neighborhood to be considered, which determines whether two points are neighbors.
   */

  DBSCAN () {}
  DBSCAN( DissPtr d, const int minPoints, const double epsi ):
      diss(d), minPts(minPoints), epsilon(epsi) { 
  }

  virtual void invalidate() {
    if (diss) {
      diss->invalidate_entropy_cache();
    }
  }

  virtual DBSCAN& setDiss( DissPtr diss ) {
    this->invalidate();
    this->diss = diss;
    return *this;
  }
  

  /** The main method that executes the algorithm
   **/
  inline virtual Partition run();

  /// The method that returns the name of this algorithm, along with its parameters
  virtual char* name() const {
    char* name = new char[81];
    sprintf( name, "DBSCAN_%d_%.3f", minPts, epsilon);
    return name;
  }
  virtual CriteriaPtr get_criteria() { return diss->get_criteria(); }

 protected:
  /// Internal method that returns the set of neighbors for a given point.
  inline Neighbors find_neighbors( const Index pid ) const; 

  /// Converts current clustering to the partition type
  inline static Partition toPartition( const LabelSet& LabelSet );

  inline virtual void set_measure(GraphPtr g, Local2GlobalPtr& l2gp, CriteriaPtr criteria = nullptr);

    
 protected:
  int minPts;
  double epsilon;

  DissPtr diss;
  
};



/** The main method that executes the algorithm. The idea is based on the `density-reachability' concept. 
 * We visit every non-visited point in the data set and from there try to reach other non-visited points 
 * that are close to this one.
 * A point is reachable from another one when the dissimilarity between them is less than the radius 
 * parameter (epsilon).
 * For this purpose, for each non-visited point, we first identify its direct neighbors. If the number of such neigbors
 * is below a given threshold minPts, we consider it as noisy point and move on to the next non-visited point.
 * Otherwise, we proceed to grow this neighborhood by trying to add to it other points that could be reached from other 
 * members of the neighborhood.
 * We continue until we cannot reach any more point outside of this group. A new cluster is then formed.
 */
Partition DBSCAN::run() {

  size_t nvars = this->diss->nbr_variables();
  LabelSet m_LabelSet( nvars,  UNASSIGNED_LABEL); 
  std::vector<size_t> visited( nvars, 0 ); // to keep track of visiting state for each point
  int cluster_id = 0; // Initially there is no cluster formed.
  for (int pid = 0; pid < nvars; ++pid) { // We visit every non-visited point. 
                                         
    if ( !visited[pid] ) {
      visited[pid] = 1;
      Neighbors neighbors  = find_neighbors(pid); 
      if ( neighbors.size() >= minPts ) { // If the neighborhood is dense,
        m_LabelSet[pid] = cluster_id; // we form a new cluster.
        for ( int i = 0; i < neighbors.size(); ++i) { // We grow this cluster by trying to reach other points
                                                     // from each of its members.
          int nPid = neighbors[i]; // 
          if ( !visited[nPid] ) { 
            visited[nPid] = 1;
            Neighbors subNeighbors = find_neighbors(nPid); // trying to find a new dense neighborhood
            if ( subNeighbors.size() >= minPts ) {
              for (const auto & neighbor : subNeighbors) { 
                neighbors.push_back(neighbor); // adds all the newly found points to the current cluster
              }
            }
          }
          if ( m_LabelSet[nPid] ==  UNASSIGNED_LABEL ) { // to avoid overriding a possible previous cluster assignment
            m_LabelSet[nPid] = cluster_id; 
          }
        }
        ++cluster_id; // increments the current cluster id
      }
    }
  }
  return toPartition(m_LabelSet); 
}

/////////////////////////////////////////
/** A neighborhood of a given point is defined as all the points that are within a certain given radius (epsilon).
 */
typename DBSCAN::Neighbors DBSCAN::find_neighbors( const Index pid ) const {
  Neighbors ne;  
  size_t nvars = this->diss->nbr_variables(); // @todo: remove direct access to compMatrix
  for ( Index i = 0; i < nvars; ++i ) {
    if ( this->diss->compute( i, pid ) <= epsilon ) { // @todo: remove direct access to compMatrix      
      ne.push_back(i);
    }
  }

  return ne;
}

/////////////////////////////////////////
Partition DBSCAN::toPartition( const LabelSet& LabelSet ) {
  Partition partition;
  std::set<Label> labs;
  std::vector<int> singletons;
  for ( size_t i = 0; i < LabelSet.size(); ++i ) {
    if ( LabelSet.at(i) != UNASSIGNED_LABEL ) {
      partition.setLabel( i, LabelSet.at(i) ); 
      labs.insert( LabelSet.at(i) );
    } else {
      singletons.push_back(i);
    }
  }

  for ( auto& i: singletons ) {
    size_t curCluster = labs.size();
    partition.setLabel(i, curCluster);
    labs.insert(curCluster);
  }
  
  return partition;
}

void DBSCAN::set_measure(GraphPtr graph, Local2GlobalPtr& l2gp, CriteriaPtr criteria ) {
  this->invalidate();
  this->diss = std::make_shared<GraphMutInfoDissimilarity>(graph, l2gp);
  this->diss->set_criteria(criteria);
}



} // namespace samogwas ends here. 

/****************************************************************************************/
#endif // SAMOGWAS_N_DBSCAN_HPP
