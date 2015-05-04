/****************************************************************************************
 * File: graph_mut_info.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 23/03/2015

 ***************************************************************************************/
#ifndef NOVO_GRAPH_MUT_INFO_HPP
#define NOVO_GRAPH_MUT_INFO_HPP

#include "graph.hpp"
#include "statistics/entropy.hpp"
#include <pl.h>
#include <cmath>

namespace samogwas
{

typedef std::vector<std::vector<double>> JointProbTab;
typedef std::vector<int> VecT;
typedef std::vector<double> ProbVecT;
typedef std::shared_ptr<VecT> VecPtr;
typedef std::shared_ptr<ProbVecT> ProVecPtr;

struct ComputeNodeJointEntropy {

  static inline double sum_log_count(double &total, double val)
  {
    if (val > 0) {
      return total + val*log(val);
    } else {
      return total;
    }
  }

  static inline double compute_entropy_from_joint_tab( std::vector<double>& jointTab, size_t SIZE ) {
    return log(SIZE) - 1.0/SIZE*std::accumulate( jointTab.begin(), jointTab.end(),
                                        0.0, sum_log_count);
  }

  
  /////////////////////////////////////////////////////////////
  double compute(const Node& nA, const Node& nB) const {
    if (nA.level > nB.level) return compute(nB,nA);
    if (nA.is_leaf() && nB.is_leaf()) {
      assert(nA.dataVec && nB.dataVec && nA.dataVec->size() == nB.dataVec->size());
      return compute_leaf_leaf(nA, nB, *nA.dataVec, *nB.dataVec);
    }
    else if (nA.is_leaf() && !nB.is_leaf()) {
      return compute_leaf_latent(nA,nB, *nA.dataVec, *nB.cndObsDist);
    }
    else {
      return compute_latent_latent(nA, nB, *nA.cndObsDist, *nB.cndObsDist);
    } 
  }

  double compute_leaf_leaf(const Node& nA, const Node& nB,
                           const VecT& dVecA, const VecT& dVecB) const {
    auto jointTab = create_joint_table_leaf_leaf(nA,nB,dVecA,dVecB);
    auto SIZE =  dVecA.size();
    return compute_entropy_from_joint_tab(jointTab, SIZE);
  }

  double compute_leaf_latent(const Node& nA, const Node& nB,
                             const VecT& dVecA, const ProbVecT& dVecB) const {
    auto jt = create_joint_table_leaf_latent(nA,nB,dVecA,dVecB);
    auto SIZE =  dVecA.size();
    return compute_entropy_from_joint_tab(jt, SIZE);
  }

  double compute_latent_latent(const Node& nA, const Node& nB,
                               const ProbVecT& dVecA, const ProbVecT& dVecB) const {
    auto jt = create_joint_table_latent_latent(nA,nB,dVecA,dVecB);
    int CARD_A = nA.cardinality(), CARD_B = nB.cardinality();
    auto SIZE = dVecA.size()/nA.cardinality();
    return compute_entropy_from_joint_tab(jt, SIZE);
  }

  //////////////  
  static double compute_from_joint_tab(const JointProbTab& probTab) {
    assert(probTab.size() > 0);
    const int CARD_X = probTab.size(), CARD_Y = probTab[0].size();

    double rs = 0.0;
    for ( auto &vec: probTab ) {
      for (auto &prob: vec ) {
        if (prob)
          rs += -prob*log(prob);
      }
    }
    return rs;
  }

  static std::vector<double> create_joint_table_leaf_leaf(const Node& nA, const Node& nB,
                                                 const VecT& dVecA, const VecT& dVecB ) {
    assert((nA.level == nB.level) && (nA.level == 0));

    int CARD_A = nA.cardinality(), CARD_B = nB.cardinality();
    int MAX_CARD = std::max(CARD_A, CARD_B);
    std::vector<double> jointTab(MAX_CARD*MAX_CARD, 0);
    auto SIZE =  dVecA.size();
    for (int i = 0; i < SIZE; ++i) {
      auto vA = dVecA.at(i), vB = dVecB.at(i);
      auto commonIdx = MAX_CARD*vA+vB;
      jointTab[commonIdx] += 1.0;
    }
    return jointTab;
  }

  static std::vector<double> create_joint_table_leaf_latent( const Node& nA, const Node& nB,
                                                    const VecT& dVecA, const ProbVecT& dVecB) {
    assert(nA.level < nB.level);
    auto SIZE =  dVecA.size();
    int CARD_A = nA.cardinality(), CARD_B = nB.cardinality();
    int MAX_CARD = std::max(CARD_A, CARD_B);
    std::vector<double> jointTab(MAX_CARD*MAX_CARD, 0);
    
    // JointProbTab jointTab(CARD_A, std::vector<double>(CARD_B,0.0));
    // auto sz = dVecA.size();
    if (nB.is_parent_of(nA)) {
      for (int a = 0; a < CARD_A; ++a) {
        for (int b = 0; b < CARD_B; ++b) {
          auto commonIdx = MAX_CARD*a+b;
          jointTab[commonIdx] = nB.compute_cond_prob(nA,a,b)*nB.compute_prob(b)*SIZE;
        }
      }
    } else {
      for (int i=0;i< SIZE ;++i) {
        int a = dVecA[i];         
        for (int b = 0; b < CARD_B; ++b) {
          auto commonIdx = MAX_CARD*a+b;
          jointTab[commonIdx] += dVecB[i*CARD_B+b];
        }
      }     
      // for (int a = 0; a < CARD_A; ++a) {
      //   for (int b = 0; b < CARD_B; ++b) {
          
      //     auto commonIdx = MAX_CARD*a+b;
      //     jointTab[commonIdx] /= SIZE;
      //     // jointTab[commonIdx] += dVecB[i*CARD_B+b];
      //   }
      // }
    } // for

    return jointTab;
  }  

  static std::vector<double> create_joint_table_latent_latent( const Node& nA, const Node& nB,
                                                    const ProbVecT& dVecA, const ProbVecT& dVecB) {
    //assert(nA.level < nB.level);
    if (nA.level < nB.level) return create_joint_table_latent_latent(nB,nA,dVecB,dVecA);
    int CARD_A = nA.cardinality(), CARD_B = nB.cardinality();
    int MAX_CARD = std::max(CARD_A, CARD_B);
    std::vector<double> jointTab(MAX_CARD*MAX_CARD, 0);
    auto SIZE = dVecA.size()/nA.cardinality();
    if (nB.is_parent_of(nA)) {
      for (int a = 0; a < CARD_A; ++a) {
        for (int b = 0; b < CARD_B; ++b) {
          auto commonIdx = MAX_CARD*a+b;
          jointTab[commonIdx] = nB.compute_cond_prob(nA,a,b)*nB.compute_prob(b)*SIZE;
        }
      }

    } else {
      for (int i=0;i<SIZE;++i) {
        for (int a = 0; a < CARD_A; ++a) {
          for (int b = 0; b < CARD_B; ++b) {
            auto commonIdx = MAX_CARD*a+b;
            jointTab[commonIdx] += dVecB[i*CARD_B+b]*dVecA[i*CARD_A+a];
          }
        }
      } // for observation

      // for (int a = 0; a < CARD_A; ++a) {
      //   for (int b = 0; b < CARD_B; ++b) { 
      //     jointTab[a][b] /= sz;
      //   }
      // }
    } // no relationship


    return jointTab;
  }
  
  
  static JointProbTab create_joint_tab_leaf_leaf(const Node& nA, const Node& nB,
                                                 const VecT& dVecA, const VecT& dVecB ) {
 
    int CARD_A = nA.cardinality(), CARD_B = nB.cardinality();
    JointProbTab jointTab(CARD_A, std::vector<double>(CARD_B,0.0));
    auto sz = dVecA.size();    
    for (int i=0;i<sz;++i) {
      int a = dVecA[i];
      int b = dVecB[i];
      jointTab[a][b] += 1;
    }     
    for (int a = 0; a < CARD_A; ++a)  
      for (int b = 0; b < CARD_B; ++b)
        jointTab[a][b] /= sz;
    return jointTab;
  }

  static JointProbTab create_joint_tab_latent_latent( const Node& nA, const Node& nB,
                                                    const ProbVecT& dVecA, const ProbVecT& dVecB) {
    if (nA.level > nB.level) return create_joint_tab_latent_latent(nB,nA,dVecB,dVecA);
    int CARD_A = nA.cardinality(), CARD_B = nB.cardinality();
    JointProbTab jointTab(CARD_A, std::vector<double>(CARD_B,0.0));
    auto sz = dVecA.size()/nA.cardinality();
    if (nB.is_parent_of(nA)) {
      for (int a = 0; a < CARD_A; ++a) {
        for (int b = 0; b < CARD_B; ++b) {
          jointTab[a][b] = nB.compute_cond_prob(nA,a,b)*nB.compute_prob(b);
        }
      }

    } else {
      for (int i=0;i<sz;++i) {
        for (int a = 0; a < CARD_A; ++a) {
          for (int b = 0; b < CARD_B; ++b) {
            jointTab[a][b] += dVecB[i*CARD_B+b]*dVecA[i*CARD_A+a];
          }
        }
      } // for observation

      for (int a = 0; a < CARD_A; ++a) {
        for (int b = 0; b < CARD_B; ++b) { 
          jointTab[a][b] /= sz;
        }
      }
    } // no relationship


    return jointTab;
  }

  static JointProbTab create_joint_tab_leaf_latent( const Node& nA, const Node& nB,
                                                    const VecT& dVecA, const ProbVecT& dVecB) {
    int CARD_A = nA.cardinality(), CARD_B = nB.cardinality();
    JointProbTab jointTab(CARD_A, std::vector<double>(CARD_B,0.0));
    auto sz = dVecA.size();
    if (nB.is_parent_of(nA)) {
      for (int a = 0; a < CARD_A; ++a) {
        for (int b = 0; b < CARD_B; ++b) {
          jointTab[a][b] = nB.compute_cond_prob(nA,a,b)*nB.compute_prob(b);
        }
      }
    } else {
      for (int i=0;i<sz;++i) {
        int a = dVecA[i];         
        for (int b = 0; b < CARD_B; ++b) {
          jointTab[a][b] += dVecB[i*CARD_B+b];
        }
      }     
      for (int a = 0; a < CARD_A; ++a) {
        for (int b = 0; b < CARD_B; ++b) {
          jointTab[a][b] /= sz;        
        }
      }
    } // for

    return jointTab;
  }  
  
};

//////////////////////////////////////////////////////////////////
struct ComputeNodeEntropy {
  inline double compute( const Node& node ) const {
    // return 1;
    // assert(node.marginalDist);
   return node.marginalDist->compute_shannon_entropy();
  }
};

//////////////////////////////////////////////////////////////////
struct ComputeNodeMutInfo {
  double compute(const Node& nA, const Node& nB) const {
    if (nA.level > nB.level) return compute(nB,nA);
    if (nA.is_leaf() && nB.is_leaf()) {
      assert(nA.dataVec && nB.dataVec && nA.dataVec->size() == nB.dataVec->size());
      return compute_leaf_leaf(nA, nB, *nA.dataVec, *nB.dataVec);
    }
    else if (nA.is_leaf() && !nB.is_leaf()) {
      return compute_leaf_latent(nA,nB, *nA.dataVec, *nB.cndObsDist);
    }
    else {
      return compute_latent_latent(nA, nB, *nA.cndObsDist, *nB.cndObsDist);
    } 
  }
  double compute_leaf_leaf(const Node& nA, const Node& nB,
                           const VecT& dVecA, const VecT& dVecB) const {
    double je = jointEntropy.compute_leaf_leaf(nA,nB,dVecA, dVecB);
    double eA = entropy.compute(nA);
    double eB = entropy.compute(nB);
    return eA + eB - je;
  }

  double compute_leaf_latent(const Node& nA, const Node& nB,
                             const VecT& dVecA, const ProbVecT& dVecB) const {
    double je = jointEntropy.compute_leaf_latent(nA,nB,dVecA, dVecB);
    double eA = entropy.compute(nA);
    double eB = entropy.compute(nB);
    return eA + eB - je;
  }

  double compute_latent_latent(const Node& nA, const Node& nB,
                               const ProbVecT& dVecA, const ProbVecT& dVecB) const {
    double je = jointEntropy.compute_latent_latent(nA,nB,dVecA, dVecB);
    double eA = entropy.compute(nA);
    double eB = entropy.compute(nB);
    return eA + eB - je;
  }

  ComputeNodeJointEntropy jointEntropy;
  ComputeNodeEntropy entropy;
}; 

} // namespace novo ends here. 

/****************************************************************************************/
#endif // NOVO_GRAPH_MUT_INFO_HPP
