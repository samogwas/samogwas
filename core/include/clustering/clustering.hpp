/****************************************************************************************
 * File: n_clustering.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 01/04/2015

 ***************************************************************************************/
#ifndef SAMOGWAS_N_CLUSTERING_HPP
#define SAMOGWAS_N_CLUSTERING_HPP

#include <memory>
#include <set>
#include <vector>
#include <map>
#include <iostream>

#include "distance/simi.hpp"
namespace samogwas
{

/** An Index depicts an Item index, relative to its position in the dataset.
 */
typedef int Index;

/** A cluster is simply a collection of indexes.
 *
 */
typedef std::vector<Index> Cluster;

/** A clustering is simply a collection of Clusters
 *
 */
typedef std::vector<Cluster> Clustering; 

////////////////////////////////////////////////////////////////////
struct Partition {
  
  /** A Label represents a cluster index, relative to its position in the clustering.
   */
  typedef int Label;
  
  /** An Index depicts an Item index, relative to its position in the dataset.
   */
  typedef std::map<Index,Label> Index2Label;

  Partition() {}
  Partition( const Clustering& clustering) {
    auto nbrClusters = clustering.size();
    for ( size_t clt = 0; clt < nbrClusters; ++clt ) {
      for (auto& item: clustering.at(clt)) {
        setLabel(item, clt);
      }
    }
  }

 
  virtual size_t nbrClusters() const { return m_labelSet.size(); }
  virtual size_t nbrItems() const { return m_index2Label.size(); } 

  /// Converts itself to a clustering
  virtual Clustering to_clustering() const {
    Clustering clustering( nbrClusters(), Cluster() );
    for ( const auto& indexLabelPair: m_index2Label ) {  
      clustering[ indexLabelPair.second ].push_back( indexLabelPair.first );
    }  
    return clustering;
  } 

  virtual Label getLabel(Index itemIdx) const { return m_index2Label.at(itemIdx); } //@todo: getLabel +change Cluster -> Label
  
  virtual Partition& setLabel(Index itemIdx, Label clusterIdx) {  //@doto: setLabel

     auto it = m_index2Label.find(itemIdx);
     if ( it != m_index2Label.end() && it->second == clusterIdx )
       return *this;

     if ( m_cluster_member_counts.find(clusterIdx) == m_cluster_member_counts.end() )
       m_cluster_member_counts[clusterIdx] = 0;
     
     if ( it !=  m_index2Label.end() )
       m_cluster_member_counts[it->second]--;
     if (m_cluster_member_counts[it->second]==0)
       removeLabel(it->second);
        
     m_labelSet.insert(clusterIdx);
     m_index2Label[itemIdx] = clusterIdx;
     m_cluster_member_counts[clusterIdx]++;

     return *this;
  }

  virtual void removeLabel(Index clusterIdx) {
    for( auto iter = m_labelSet.begin(); iter != m_labelSet.end(); ){
      if(*iter==clusterIdx) {
        iter = m_labelSet.erase(iter);
      }
      else {
        ++iter;
      }
    }
  }
 protected:
  std::set<Label> m_labelSet; // A label is a cluster identifier. 
  Index2Label m_index2Label;
  std::map<Index,int> m_cluster_member_counts; // keeps track of the nbr of items per cluster


  // std::vector<int> label2indexes;
  // bool is_cached = false;
  // bool is_cache_valid = false;

};

/** Convenient way to output the content of a clustering
 *
 */
inline std::ostream& operator<<( std::ostream& os, const Clustering& clt ) {
  os << "There are: " << clt.size() << " clusters\n";  
  for ( size_t c = 0; c < clt.size(); ++c ) {
    os << "cluster: " << c << std::endl;
    for ( size_t i = 0; i < clt.at(c).size() - 1; ++i ) {
      os << clt.at(c).at(i) << ", ";
    }
    os << clt.at(c).at(clt.at(c).size() - 1) << std::endl;
  }
  return os;  
}

/// Convenient way to output the content of a partition
inline std::ostream& operator<<( std::ostream& os, const Partition& p ) {
  return os << p.to_clustering();

}
//////////////////////////////////////////////////////////

/** Any clustering algorithm is derived from this base class.
 */
class ClustAlgo
{ 
 public:
  virtual Partition operator()() { return run(); }
  virtual Partition run() = 0;
  virtual char* name() const = 0;
  virtual void invalidate() = 0; //@todo: invalidateCache
  virtual void set_measure(GraphPtr g, Local2GlobalPtr& l2gp, CriteriaPtr criteria = nullptr) {}
  virtual CriteriaPtr get_criteria() { return nullptr; }
};

typedef std::shared_ptr<ClustAlgo> ClustAlgoPtr;

} // namespace samogwas ends here. 

/****************************************************************************************/
#endif // SAMOGWAS_N_CLUSTERING_HPP
