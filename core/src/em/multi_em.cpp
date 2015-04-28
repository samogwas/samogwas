
#include "em/multi_em.hpp"
#include "utils/matrix_utils.hpp"

#define NOT_MISSING 1
#define MISSING 0
#define DEFAULT_VALUE -1
#define MISSING_VALUE -1

namespace samogwas {

using MatrixPtr = MultiEM::MatrixPtr;

void MultiEM::run( GraphPtr graph,
                   Node& latentNode,
                   const double threshold ) {
  
  auto vars = latentNode.get_children_variables();
  // printf("hey, nbr of children: %d\n", vars.size());
  auto indexes = latentNode.get_children_global_indexes();
  auto dataTable = create_data_table( graph, indexes );
  auto defTable = create_def_table(utility::nrows(*dataTable),utility::ncols(*dataTable));
 
  auto learnObjs = create_learn_objects(latentNode.variable, vars);  
  plMatrixDataDescriptor<int> dataDesc( latentNode.variable^vars, dataTable.get(), defTable.get());
  CandidateModels candidateModels;  // vector to store different learners

  for (size_t it = 0; it < 1; ++it) {
    //  joint distribution P(X,Z) = P(Z)*P(X_1 | Z)*...*P(X_n | Z), Z: latent variable
    plComputableObjectList jointDist = create_computable_objects(latentNode.variable, vars); // all distributions
    // randomly initialized
    EMLearner learner(jointDist, learnObjs);
    learner.set_same_missing_variables(false);
    learner.run(dataDesc, threshold); // executes the EM learning process
    candidateModels.push_back(learner); // puts the learnt model into the collection
  }

  plEMLearner bestModel = get_best_model(candidateModels, dataDesc);
   std::vector<plValues> missing_most_probable_value;
   std::vector<std::vector<plProbValue>> missing_probability_table;
   bestModel.compute_missing_values_infos(dataDesc, missing_most_probable_value, missing_probability_table );
  auto jointDist = bestModel.get_joint_distribution();
  update_parameters(graph, latentNode, missing_most_probable_value, missing_probability_table, jointDist );
}


LearnObjectPtrs MultiEM::create_learn_objects(const Variable & latentVar, const Variables& variables) {
  LearnObjectPtrs learnObjects;
  plLearnObject* learnLatent = new plLearnHistogram(latentVar);
  learnObjects.push_back(learnLatent);
  
  for (size_t var = 0; var < variables.size(); ++var) {
    learnObjects.push_back(new plCndLearnObject <plLearnHistogram> (variables[var], latentVar));
  }

  return learnObjects;
}


// @todo: directly convert and not 
MatrixPtr MultiEM::create_data_table( GraphPtr graph, const std::vector<int>& indexes ) {
  auto nbrVars = indexes.size() + 1; // children + (-1)
  auto matrix = std::make_shared<Matrix>();
  matrix->reserve(nbrVars);
  auto nbrInds = (*graph)[0].dataVec->size();
  matrix->push_back(std::vector<int>( nbrInds, -1));

  for ( auto index: indexes ) {
    Node& n = (*graph)[index];
    matrix->push_back(*n.dataVec);
  }
  return transpose(matrix);   
}


DefTabPtr MultiEM::create_def_table( size_t rows, size_t cols ) {      
  auto defTable = std::make_shared<DefTab>( rows, std::vector<bool>(cols, MISSING));
  for (size_t ind = 0; ind < rows; ++ind) { // Each row will contain (FALSE TRUE TRUE ... TRUE).
    for ( size_t v = 1; v < cols; ++v ) {
      defTable->at(ind).at(v) = NOT_MISSING;
    }    
  }
  return defTable;
}


MatrixPtr MultiEM::transpose(MatrixPtr mat ) {
  size_t rows = utility::nrows(*mat), cols = utility::ncols(*mat);
  auto matrix = std::make_shared<Matrix>( cols, std::vector<int>(rows) );

  for (unsigned row = 0; row < rows; row++) {    
    for (unsigned col = 0; col < cols; col++) {
      matrix->at(col).at(row) = mat->at(row).at(col);
    }
  }

  return matrix;
}

plComputableObjectList MultiEM::create_computable_objects( const Variable& latentVar,
                                                           const Variables& variables ) {
  plComputableObjectList jointTable; // joint distribution P(X,Z) = P(Z)*P(X_1 | Z)*...*P(X_n | Z)

  const plProbTable latentProbInit(latentVar, true); // Z denotes the latent variable.
  jointTable *= latentProbInit;
  
  for (auto var: variables) {
    plDistributionTable distTab_Xi_Z(var, latentVar); // Conditional distribution P(Xi | Z)
    for (size_t h = 0; h < latentVar.cardinality(); ++h) {
      distTab_Xi_Z.push( plProbTable(var, true), (int)h );
    }
    jointTable *= distTab_Xi_Z;
  }  
  
  return jointTable;
}

EMLearner MultiEM::get_best_model( CandidateModels& learners,
                                   plMatrixDataDescriptor<int>& dataDesc ) {
  EMLearner bestModel = learners[0];  
  for (size_t i = 1; i < learners.size(); ++i) {
    if ( scoreBIC( learners[i], dataDesc) > scoreBIC(bestModel, dataDesc)) {
      bestModel = learners[i];
    }
  }  
  return bestModel;
}

double MultiEM::scoreBIC( EMLearner& learner, plMatrixDataDescriptor<int>& dataDesc ) {
  double llh = 0.0;
  try {
    llh = learner.get_last_computed_loglikelihood(); // if stored value available
  } catch (plError& e) {
    llh = (double)learner.compute_loglikelihood(dataDesc); // otherwise, computes the value
  }  
  double result = llh - 0.5*learner.get_n_parameters()*std::log(dataDesc.get_n_records());
  return result;
}

// 

void MultiEM::update_parameters(GraphPtr graph, Node& latentNode, const std::vector<plValues>& most_prob_vals,
                                const std::vector<std::vector<plProbValue>>& probTab,
                                const JointDist& jointDist ) {
  auto latentVar = latentNode.variable;

  auto dataVec = std::make_shared<std::vector<int>>(most_prob_vals.size(), 0);
  auto it = dataVec->begin();  
  for (auto v: most_prob_vals) {
    *it = v.get_value_as_int(latentVar);
  }
  latentNode.set_data_vec(dataVec); // marginal + data_vec

  auto cndDist = std::make_shared<std::vector<double>>(latentVar.cardinality()*most_prob_vals.size(), 0.0);
  auto cIt = cndDist->begin();  
  for (auto vec: probTab ) {
    for ( auto v: vec ) {
      *cIt = v;
    }    
  }  
  latentNode.set_cnd_obs_vec(cndDist).set_joint_distribution( jointDist )
      .set_position().set_level();

}

} // namespace
