
#include "em/multi_em.hpp"
#include "utils/matrix_utils.hpp"

#define NOT_MISSING 1
#define MISSING 0
#define DEFAULT_VALUE -1
#define MISSING_VALUE -1

namespace samogwas {

using MatrixPtr = MultiEM::MatrixPtr;
typedef plMatrixDataDescriptor<int> DataDesc;

double MultiEM::run( const Graph& graph,
                   Node& latentNode,
                   const double threshold ) {

  auto vars = latentNode.get_children_variables();
  auto indexes = latentNode.get_children_global_indexes();
  auto dataTable = create_data_table( graph, indexes );
  auto defTable = create_def_table(samogwas::nrows(*dataTable),samogwas::ncols(*dataTable));
 
  auto learnObjs = create_learn_objects(latentNode.variable, vars);  
  DataDesc dataDesc( latentNode.variable^vars, dataTable.get(), defTable.get());
  CandidateModels candidateModels;  // vector to store different learners

  nbrRestarts = nbrRestarts <= 0 ? 1 : nbrRestarts;
  for (size_t it = 0; it < nbrRestarts; ++it) {
    //  joint distribution P(X,Z) = P(Z)*P(X_1 | Z)*...*P(X_n | Z), Z: latent variable
    plComputableObjectList jointDist = create_computable_objects(latentNode.variable, vars); // all distributions
    // randomly initialized
    EMLearner learner(jointDist, learnObjs);
    learner.set_same_missing_variables(false);
    learner.run(dataDesc, threshold); // executes the EM learning process
    candidateModels.push_back(learner); // puts the learnt model into the collection
  }

  plEMLearner bestModel = get_best_model(candidateModels, dataDesc);
  update_parameters( latentNode, dataDesc, bestModel );
  //std::cout << "max_llh: " << bestModel.get_last_computed_loglikelihood() << std::endl;
  //std::cout << "best model: " << bestModel.get_joint_distribution().get_computable_object_list() << std::endl;
  return bestModel.get_last_computed_loglikelihood();
}

void MultiEM::update_parameters ( Node& latentNode,
                                  DataDesc& dataDesc,
                                  plEMLearner& model ) {

  std::vector<plValues> missing_vals;
  std::vector<std::vector<plProbValue>> prob_tabs;
  model.compute_missing_values_infos(dataDesc, missing_vals, prob_tabs );
  auto jointDistCompList = model.get_joint_distribution().get_computable_object_list();

  auto latentVar = latentNode.variable;
  auto dataVec = std::make_shared<std::vector<int>>( missing_vals.size(), 0 );

  auto it = dataVec->begin();
  for (size_t ind = 0; ind < missing_vals.size(); ++ind) {
    plProbTable probTab(latentVar, prob_tabs[ind], true);
    dataVec->at(ind) = probTab.draw()[0];
  }

  auto SIZE = latentVar.cardinality()*missing_vals.size();
  auto cndDist = std::make_shared<std::vector<double>>( SIZE, 0.0 );
  auto cIt = cndDist->begin();
  for (auto vec: prob_tabs ) {
    for ( auto v: vec ) {
      *cIt = v;
      cIt++;
    }
  }

  latentNode.set_cnd_obs_vec( cndDist, false ).set_data_vec(dataVec, true)
      .set_children_distributions( ++jointDistCompList.begin(), jointDistCompList.end() )
      .set_position()
      .update_level();
}

LearnObjectPtrs MultiEM::create_learn_objects( const Variable & latentVar,
                                               const Variables& variables ) {
  LearnObjectPtrs learnObjects;
  plLearnObject* learnLatent = new plLearnHistogram(latentVar);
  learnObjects.push_back(learnLatent);
  for (size_t var = 0; var < variables.size(); ++var) {
    learnObjects.push_back(new plCndLearnObject <plLearnHistogram> (variables[var], latentVar));
  }
  return learnObjects;
}


// @todo: directly convert and not
MatrixPtr MultiEM::create_data_table( const Graph& graph, const std::vector<int>& indexes ) {
  auto nbrVars = indexes.size() + 1; // chgildren + (-1)
  auto matrix = std::make_shared<Matrix>();
  matrix->reserve(nbrVars);
  auto nbrInds = graph[0].dataVec->size();
  matrix->push_back(std::vector<int>( nbrInds, -1));

  for ( auto index: indexes ) {
    const Node& n = graph[index];
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
  size_t rows = samogwas::nrows(*mat), cols = samogwas::ncols(*mat);
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
      std::vector<double> probs(var.cardinality(), 0.0);
      fill_vector_with_random_floats( probs, this->generator, 0.0, 1.0 );
      distTab_Xi_Z.push( plProbTable(var, probs, false), (int)h );
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


} // namespace
