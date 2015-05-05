/****************************************************************************************
 * File: distance.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 01/04/2015

 ***************************************************************************************/
#ifndef SAMOGWAS_DISTANCE_HPP
#define SAMOGWAS_DISTANCE_HPP

#include <memory>
#include <vector>
#include <cmath>
namespace samogwas
{

typedef std::shared_ptr<std::vector<int>> PosVecPtr;
struct ConstraintCriteria {
  virtual bool valid( const int vA, const int vB) const = 0;
};
  
typedef std::shared_ptr<ConstraintCriteria> CriteriaPtr;

struct AcceptAllCriteria: public ConstraintCriteria {
  virtual bool valid( const int vA, const int vB) const {
    return true;
  }
};

struct PositionCriteria: public ConstraintCriteria {
  PositionCriteria(PosVecPtr posV, int max_d): posVec(posV), max_dist(max_d) {}

  virtual bool valid( const int vA, const int vB) const {
    return abs(posVec->at(vA) - posVec->at(vB)) < max_dist;
  }
 private:
  PosVecPtr posVec;
  int max_dist;
};

} // namespace samogwas ends here. 

/****************************************************************************************/
#endif // SAMOGWAS_DISTANCE_HPP
