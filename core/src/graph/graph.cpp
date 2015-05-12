
#include "graph/graph.hpp"

namespace samogwas {

Node& Node::set_children_distributions(plCompIte beg, plCompIte end ) {
  for (auto it = beg; it != end; ++it) {
    auto cGivenZ = std::make_shared<plCndDistribution>(*it);
    this->cndChildrenDists.push_back(cGivenZ);
  }
    return *this;
}

Node& Node::set_local_indexes( const Label2Index &label2Index ) {
  auto vars = get_children_variables();
  // set_local_indexes( vars.begin(), vars.end(), label2Index );

  int i = 0;  
  for ( auto it = vars.begin(); it != vars.end(); ++it ) {
    auto var = *it;
    auto lab = var.name();
    auto idx = label2Index.at(lab);
    global2localIdx[idx] = i++;
  }
  return *this;
    
}


Node& Node::set_local_indexes( VarIter beg, VarIter end, const Label2Index &label2Index ) {  
  int i = 0;  
  for ( auto it = beg; it != end; ++it ) {
    auto var = *it;
    auto lab = var.name();
    auto idx = label2Index.at(lab);
    global2localIdx[idx] = i++;
  }
  return *this;
}

/**
 */
Node& Node::set_joint_distribution( const JointDist& jointDist) {

  auto compList = jointDist.get_computable_object_list();
  // auto beg = compList.begin();
  // // set_marginal_dist
  // this->set_marginal_distribution(beg);
  // // set_children_dist
  // this->set_children_distributions(++beg, compList.end());

  return set_joint_distribution(compList);
}

Node& Node::set_joint_distribution( const plComputableObjectList& compList) {
  // auto compList = jointDist.get_computable_object_list();
  auto beg = compList.begin();
  // set_marginal_dist
  this->set_marginal_distribution(beg);
  // set_children_dist
  this->set_children_distributions(++beg, compList.end());
  return *this;
}

// /**
//  */
// Node& Node::set_joint_( const plJointDistribution& compList ) {

//   auto beg = compList.begin();
//   // set_marginal_dist
//   this->set_marginal_distribution(beg);
//   // set_children_dist
//   this->set_children_distributions(++beg, compList.end());

//   return *this;
// }



Node& Node::set_position() {
  auto children_indexes = this->get_children_global_indexes();
  unsigned nbrChildren = this->nbr_children();  
  if (nbrChildren > 0) {    
    unsigned totalPos = 0;
    for (auto cidx: children_indexes) {      
      Node& n = (*graph)[cidx];
      totalPos += n.position;
    }       
    this->position = totalPos / nbrChildren;
  }
  return *this;
}

Node& Node::update_level() {
  auto children_indexes = this->get_children_global_indexes();
  unsigned nbrChildren = this->nbr_children();
  int maxChildrenLevel = 0;
  for (auto cidx: children_indexes) {      
    Node& n = (*graph)[cidx];
    if (maxChildrenLevel <  n.level) {
      maxChildrenLevel = n.level;
    }
  }        
  this->level = maxChildrenLevel + 1;
  return *this;
}

Node& Node::set_data_vec(DataVecPtr dtv, const bool computeEmpDist) {
    this->dataVec = dtv;
    if (computeEmpDist) {    
      auto marginalDist = create_emp_distribution(this->variable, *dtv);
      return set_marginal_distribution(marginalDist);
    }
    return *this;
}

Node& Node::set_cnd_obs_vec(CondObsDistPtr cndObsDist, const bool computeEmpDist) {
  this->cndObsDist = cndObsDist;
  if (computeEmpDist) {    
    auto marginalDist = create_emp_distribution(this->variable, *cndObsDist);
    return set_marginal_distribution(marginalDist);
  }
  return *this;
}


////////////////////////////////

double Node::compute_prob(const int a) const {
  return this->marginalDist->compute(a);
}

double Node::compute_cond_prob_obs(const int val, const int obs) const {
  // [ (0,1,..card-1), ..., [0,1,..card-1] ]
  if (!this->is_leaf()) { // if latent
    return cndObsDist->at(obs*this->cardinality()+val);
  } else {
    return dataVec->at(obs) == val; // 1 if equal otherwise...
  }
}

double Node::compute_cond_prob( const Node& cNode, const int childVal, const int parentVal ) const {
  plValues pVl = plValues().add(this->variable, parentVal);  
  plValues cVl = plValues().add(cNode.variable, childVal);
  auto childIdx = global2localIdx.at(cNode.index);  
  return this->compute_cond_prob(childIdx, cVl, pVl);
}

// node.compute_cond_prob( (int)i,  plValues().add(varX, childVal), plValues().add(latentVar, val) )

double Node::compute_cond_prob( const int cIdx, const int childVal, const int parentVal ) const {
  plValues pVl = plValues().add( this->variable, parentVal );
  int globaIdx = local2GlobalIdx(cIdx);
  plValues cVl = plValues().add( (*graph)[globaIdx].variable, childVal );
  return this->compute_cond_prob(cIdx,cVl,pVl);
}

double Node::compute_cond_prob( const int cIdx, const plValues& cV, const plValues& pV ) const {
  return this->cndChildrenDists[cIdx]->compute(cV,pV);
}

bool Node::is_parent_of( const Node& other ) const {
  if (other.level <= this->level);
  auto other_idx = other.index;
  return global2localIdx.find(other_idx) != global2localIdx.end();
}


Node& Node::copy_data( const Node& n ) {
  this->graph = n.graph;
  this->variable = n.variable;
  this->marginalDist = n.marginalDist;
  this->dataVec = n.dataVec;
  this->cndObsDist = n.cndObsDist;
  this->cndChildrenDists = n.cndChildrenDists;
  this->global2localIdx = n.global2localIdx;
  this->position = n.position;
  this->level = n.level;
  
  return *this;
  
}


} // namespace samogwas







