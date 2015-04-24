/*=============================================================================
 * Product        : ProBT 
 * File           : plLearnMixtureModel.h
 * Author         : Kamel Mekhnacha
 * Creation       : February 2014
 *
 *=============================================================================
 *     (c) Copyright 2014, Probayes SAS
 *=============================================================================
 *
 *------------------------- Description ---------------------------------------
 *   
 *   
 *-----------------------------------------------------------------------------
 */

#ifndef plLearnMixtureModel_h
#define plLearnMixtureModel_h

#include <vector>
#include <iostream>

#include "plConfig.h"

#include <plNonCndLearnObject.h>
#include <plDistribution.h>
#include <plJointDistribution.h>

class plLearnCndObject;
class plDistributionTable;

SPL_DECL1(plLearnMixtureModel);

/** This class implements the Mixture Model unsupervised learning.
   
    Children classes have to implement the pure virtual functions 
    get_initialization_cluster() and create_cluster_learn_object().

    It uses the EM learning algorithm and is based on the plEMLearner class.

    \sa plLearnGMM
*/
class PL_DLL_API plLearnMixtureModel : public plNonCndLearnObject
{

 public:

  /** Type for optimality score allowing to choose the best number of components 
      (clusters).
      
      \sa set_optimality_score()
  */
  enum OptimalityCriterion {
    AIC=0, /*!< Akaike's Information Criterion. */
    BIC,   /*!< Bayesian Information Criterion. */
    N_SCORES // keep N_SCORES as the last value
  };
  static const char* OptimalityCriterionNames[N_SCORES];

  /** Default constructor */
  plLearnMixtureModel();

  /** Constructor 
      \param[in] ObsVars The variables on which the mixture model will be learned
      \param[in] n_mixture The number of components (kernels) of the mixture
  */
  plLearnMixtureModel(const plVariablesConjunction &ObsVars, size_t n_mixture);
  
  /** Constructor 
      \param[in] ObsVars The variables on which the mixture model will be learned
      
      \param[in] n_mixture_candidates The candidate number of components
      (kernels) of the mixture. Choosing among this candidates will be based on 
      the optimality score (as defined by set_optimality_score() among BIC, AIC, ...)
  */
  plLearnMixtureModel(const plVariablesConjunction &ObsVars,
                      const std::vector<size_t> &n_mixture_candidates);

  /** Destructor */
  ~plLearnMixtureModel(){};

  /** Return the mixture corresponding to the best optimality score */
  virtual plDistribution get_distribution() const;

 
  virtual void get_parameters(plValues &params) const;

  virtual void reset();

  virtual unsigned int get_n_parameters()const
  {
    return n_parameters_;
  }

  /** Get the number of parameters to be learned of the model 
      learned using the ith candidate for the number of components
  */
  size_t get_model_n_learn_parameters(size_t i) const;

  /** Get the log-likelihood value of the model 
      learned using the ith candidate for the number of components
  */  
  plFloat get_model_log_likelihood(size_t i) const;

  /** Get the optimality score among BIC, AIC, ...
      of the model learned using the ith candidate for the number of components
  */
  plFloat get_model_optimality_score(OptimalityCriterion score, size_t i) const;

  /** Get the joint distribution 'P(X C) = P(C) P(X | C)' corresponding to the model 
      learned using the ith candidate for the number of components
  */
  const plJointDistribution &get_model_joint(size_t i) const;


  /** Get the best candidate index, based on the the optimality score among BIC, AIC, ...
  */
  size_t get_best_model_index(OptimalityCriterion score) const;

  /** Get the list of candidates of the number of components
      passed to the constructor
  */
  const std::vector<size_t> & get_n_mixture_candidates() const
  {
    return n_mixture_candidate_;
  }

  /** Get the ith  candidate of the number of components
      passed to the constructor
  */
  size_t get_n_mixture_candidate(size_t i) const;


  /** Set the number of EM trials with random starting points (the default is 1) */
  void set_em_n_trials(size_t ntrials)
  {
    n_trials_ = ntrials;
  }

  /* Return the used optimality score (among BIC and AIC) */
  OptimalityCriterion optimality_score() const
  {
    return optimality_score_;
  }

  /** Set the optimality score allowing to choose the best number of components (clusters) among BIC and AIC */
  void set_optimality_score(OptimalityCriterion score);

  /** Print all mixture solutions corresponding to the provided list of components number */
  void print_mixture_solutions(std::ostream &os=std::cout) const;
  
  
 protected:
  virtual bool internal_addPoint(const plDataValues & point, 
                                 plFloat weight = PL_ONE);


  /** Initialize P(C) (weights) and P(X | C ) (clusters) */
  virtual void initialize_distributions(const plVariablesConjunction& X,
                                        const plSymbol &C,
                                        size_t trial,
                                        plDistribution &init_pc,
                                        plDistributionTable &init_px) const;
  
 protected:

  virtual void Output(std::ostream &out) const;
  
  void run_em() const;

  /** Get an initialization cluster distribution P(X | C=c) */
  virtual plDistribution get_initialization_cluster(const plVariablesConjunction &X,
                                                    size_t c, size_t trial) const = 0;

  
  /** Create the cluster learner for P(X | C ) according to the type of the mixture components  */
  virtual plLearnObject *create_cluster_learn_object(const plVariablesConjunction &X,
                                                     const plSymbol &C) const = 0;

  void run_em(size_t i) const;

 private:
  void replace_opt_results() const;

 protected:
  mutable bool is_up_to_date_;
  std::vector< std::vector<plFloat> > points_;
  std::vector<size_t> n_mixture_candidate_;
  mutable plDistribution mixture_;
  mutable unsigned int n_parameters_;
  mutable std::vector<unsigned int> nparams_;
  mutable std::vector<plFloat> llk_;
  mutable std::vector<std::vector<plFloat> > optimality_score_value_;
  mutable std::vector<plJointDistribution> model_;
  size_t n_trials_;
  OptimalityCriterion optimality_score_;

  SPL_ACCESS1(plLearnMixtureModel);
};



#endif // plLearnMixtureModel_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
