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

#include <unordered_map>
#include <unordered_set>


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

/**
 *
 */
struct AbstractPartition {
  typedef int Label;

  /**
   *
   */
  AbstractPartition() {}

  /**
   *
   */
  AbstractPartition( const Clustering& clustering) {}

  /**
   *
   */
  AbstractPartition( const std::vector<Label>& clustering) {}

  /**
   *
   */
  virtual ~AbstractPartition() {}

  /**
   *
   */
  virtual size_t nbr_clusters() const = 0;

  /**
   *
   */
  virtual size_t nbr_items() const = 0;

  /**
   *
   */
  virtual Clustering to_clustering() const = 0;

  /**
   *
   */
  virtual Label get_label(Index itemIdx) const = 0;

  /**
   *
   */
  virtual AbstractPartition& setLabel(Index itemIdx, Label clusterIdx) = 0;

};


struct NPartition: public AbstractPartition {
  using AbstractPartition::Label;

  /**
   *
   */
  NPartition() {}

  /**
   *
   */
  NPartition(const Clustering& clustering) {}

  /**
   *
   */
  NPartition(const std::vector<Label>& clustering) {}

  /**
   *
   */
  virtual ~NPartition() {}

  /**
   *
   */
  virtual size_t nbr_clusters() const {
    return labels.size();
  }

  /**
   *
   */
  virtual size_t nbr_items() const {
    return index2label.size();
  }

  /**
   *
   */

  virtual Clustering to_clustering() const {
    Clustering clustering(nbr_clusters(), Cluster());
    for (const auto& indexLabelPair: index2label) {
      clustering[indexLabelPair.second].push_back(indexLabelPair.first);
    }
    return clustering;
  }
  /**
   *
   */
  virtual Label get_label(Index index) const {
    return index2label.at(index);
  }

  /**
   *
   */
  virtual AbstractPartition& setLabel(Index itemIdx, Label clusterIdx) {
    index2label[itemIdx] = clusterIdx;
    labels.insert(clusterIdx);
    return *this;
  }

 private:

 private:
  std::unordered_map<Index,Label> index2label;
  std::unordered_set<Label> labels;

};



/**
 *
 */
struct Partition: public AbstractPartition {

  /** A Label represents a cluster index, relative to its position in the clustering.
   */
  typedef int Label;
  /** An Index depicts an Item index, relative to its position in the dataset.
   */
  typedef std::map<Index,Label> Index2Label;

  Partition();

  Partition( const Clustering& clustering);

  Partition( const std::vector<Label>& clustering);

  virtual ~Partition();

  /**
   *
   */
  virtual size_t nbr_clusters() const;

  /**
   *
   */
  virtual size_t nbr_items() const;

  /**
   * Converts itself to a clustering
   */
  virtual Clustering to_clustering() const;

  /**
   *
   */
  virtual Label get_label(Index itemIdx) const;

  /**
   *
   */
  virtual AbstractPartition& setLabel(Index itemIdx, Label clusterIdx);

  /**
   *
   */
   virtual void removeLabel(Index clusterIdx);

  /**
   *
   */
  const Index2Label& get_mapping() const;

 protected:
  std::set<Label> m_labelSet; // A label is a cluster identifier.
  Index2Label m_index2Label;
  std::map<Label,int> m_cluster_member_counts; // keeps track of the nbr of items per cluster

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
class ClustAlgo {
 public:
  virtual ~ClustAlgo() {}
  virtual std::shared_ptr<AbstractPartition> operator()() { return run(); }
  virtual std::shared_ptr<AbstractPartition> run() = 0;
  virtual char* name() const = 0;
  virtual void invalidate() = 0; //
  virtual void set_measure(GraphPtr g, Local2GlobalPtr& l2gp, CriteriaPtr criteria = nullptr) {}
  virtual CriteriaPtr get_criteria() { return nullptr; }

  virtual double measure(const size_t a, const size_t b) {
    return -1.0;
  }
};

typedef std::shared_ptr<ClustAlgo> ClustAlgoPtr;

} // namespace samogwas ends here. 

/****************************************************************************************/
#endif // SAMOGWAS_N_CLUSTERING_HPP
