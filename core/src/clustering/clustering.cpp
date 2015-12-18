

#include "clustering/clustering.hpp"

namespace samogwas {

Partition::Partition() {

}

Partition::Partition( const Clustering& clustering) {
  auto nbrClusters = clustering.size();
  for ( size_t clt = 0; clt < nbrClusters; ++clt ) {
    for (auto& item: clustering.at(clt)) {
      setLabel(item, clt);
    }
  }
}


Partition::Partition( const std::vector<Label>& clustering) {
  for ( size_t i = 0; i < clustering.size(); ++i ) {
    setLabel(i, clustering.at(i));
  }
}

Partition::~Partition() {}

/**
 *
 */
size_t Partition::nbr_clusters() const {
  return m_labelSet.size();
}

/**
 *
 */
size_t Partition::nbr_items() const {
  return m_index2Label.size();
}

/**
 *
 */
Clustering Partition::to_clustering() const {
  Clustering clustering( this->nbr_clusters(), Cluster() );
  for ( const auto& indexLabelPair: m_index2Label ) {
    clustering[ indexLabelPair.second ].push_back( indexLabelPair.first );
  }
  return clustering;
}

/**
 *
 */
Partition::Label Partition::get_label(Index itemIdx) const {
  return m_index2Label.at(itemIdx);
}

/**
 *
 */
AbstractPartition& Partition::setLabel(Index itemIdx, Label clusterIdx) {
  auto it = m_index2Label.find(itemIdx);
  if ( it != m_index2Label.end() && it->second == clusterIdx )
    return *this;

  if ( m_cluster_member_counts.find(clusterIdx) == m_cluster_member_counts.end() )
    m_cluster_member_counts[clusterIdx] = 0; // creates new cluster

  if ( it !=  m_index2Label.end() ) // switching cluster labels for this item
    m_cluster_member_counts[it->second]--;
  if (m_cluster_member_counts[it->second] == 0)
    removeLabel(it->second);

  m_labelSet.insert(clusterIdx);
  m_index2Label[itemIdx] = clusterIdx;
  m_cluster_member_counts[clusterIdx]++;

  return *this;
}

/**
 *
 */
void Partition::removeLabel(Index clusterIdx) {
  auto iter = m_labelSet.find(clusterIdx);
  if (iter != m_labelSet.end()) {
    m_labelSet.erase(iter);
  }
}

/**
 *
 */
const Partition::Index2Label& Partition::get_mapping() const {
  return m_index2Label;
}


} // namespace samogwas ends here

