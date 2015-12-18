
#include "graph/graph.hpp"

namespace samogwas {

/**
 */
Node& Node::set_children_distributions(plCompIte beg, plCompIte end ) {
  for (auto it = beg; it != end; ++it) {
    auto cGivenZ = std::make_shared<plCndDistribution>(*it);
    this->cndChildrenDists.push_back(cGivenZ);
  }
    return *this;
}

/**
 */
Node& Node::set_local_indexes( const Label2Index &label2Index ) {
  auto vars = get_children_variables();
  int i = 0;
  for ( auto it = vars.begin(); it != vars.end(); ++it ) {
    auto var = *it;
    auto lab = var.name();
    auto idx = label2Index.at(lab);
    global2localIdx[idx] = i++;
  }
  return *this;
}

/**
 */
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
Node& Node::set_marginal_distribution( plCompIte it) {
  auto dist = std::make_shared<plDistribution>(*it);
  return set_marginal_distribution(dist);
}

/**
 */
Node& Node::set_marginal_distribution( DistPtr dist) {
  this->marginalDist = dist;
  return *this;
}


/**
 */
Variables Node::get_children_variables() const {
  Variables vars;
  if (!cndChildrenDists.empty()) {
    for (auto &d: cndChildrenDists) {
      vars ^= d->get_left_variables()[0]; //
    }
  } else {
    for (int i = 0; i < nbr_children(); ++i) {
      auto idx = local2GlobalIdx(i);
      vars ^= (*graph)[idx].variable;
    }
  }
  return vars;
}


/**
 */
std::vector<int> Node::get_children_global_indexes() const {
  std::vector<int> res(this->nbr_children(), 0);
  for (int i = 0; i < nbr_children(); ++i) {
    res[i] = local2GlobalIdx(i);
  }
  return res;
}

/**
 */
IndexType Node::getIndex() const {
  return index;
}

/**
 */
LabelType Node::getLabel() const {
  return this->variable.name();
}

/**
 */
int Node::nbr_children() const {
  return global2localIdx.size();
}


/**
 */
int Node::cardinality() const {
  return this->variable.cardinality();
}


/**
 */
int Node::local2GlobalIdx(const int local) const {
  int global = -1;
  for (auto it = global2localIdx.begin(); it != global2localIdx.end(); ++it ) {
    if (it->second == local) {
      global = it->first;
      break;
    }
  }
  return global;
}


/**
 */
Node& Node::set_joint_distribution( const JointDist& jointDist) {
  auto compList = jointDist.get_computable_object_list();
  return set_joint_distribution(compList);
}

Node& Node::set_joint_distribution( const plComputableObjectList& compList) {
  auto beg = compList.begin();
  this->set_marginal_distribution(beg);
  this->set_children_distributions(++beg, compList.end());
  return *this;
}

Node& Node::set_position() {
  auto children_indexes = this->get_children_global_indexes();
  unsigned nbrChildren = this->nbr_children();
  if (nbrChildren > 0) {
    unsigned totalPos = 0;
    for (auto cidx: children_indexes) {
      Node& n = (*graph)[cidx];
      totalPos += n.position / nbrChildren;
    }
    this->position = totalPos;
  }
  return *this;
}

Node& Node::update_level() {
  auto children_indexes = this->get_children_global_indexes();
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

/**
 *
 */
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


/**
 */
double Node::compute_prob(const int a) const {
  return this->marginalDist->compute(a);
}

/** 
 *
 */
double Node::compute_cond_prob_obs(const int val, const int obs) const {
  // [ (0,1,..card-1), ..., [0,1,..card-1] ]
  if (!this->is_leaf()) { // if latent
    return cndObsDist->at(obs*this->cardinality()+val);
  } else {
    return dataVec->at(obs) == val; // 1 if equal otherwise 0
  }
}

/**
 */
double Node::compute_cond_prob( const Node& cNode, const int childVal, const int parentVal ) const {
  plValues pVl = plValues().add(this->variable, parentVal);  
  plValues cVl = plValues().add(cNode.variable, childVal);
  auto childIdx = global2localIdx.at(cNode.index);  
  return this->compute_cond_prob(childIdx, cVl, pVl);
}

/**
 */
double Node::compute_cond_prob( const int childIdx, const int childVal, const int parentVal ) const {
  plValues pVl = plValues().add( this->variable, parentVal );
  int globaIdx = local2GlobalIdx(childIdx);
  plValues cVl = plValues().add( (*graph)[globaIdx].variable, childVal );
  return this->compute_cond_prob(childIdx, cVl, pVl);
}

/**
 */
double Node::compute_cond_prob( const int childIdx, const plValues& cV, const plValues& pV ) const {
  return this->cndChildrenDists[childIdx]->compute(cV,pV);
}


/**
 */
bool Node::is_parent_of( const Node& other ) const {
  if (other.level <= this->level)
    return false;
  auto other_idx = other.index;
  return global2localIdx.find(other_idx) != global2localIdx.end();
}

/**
 */
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
