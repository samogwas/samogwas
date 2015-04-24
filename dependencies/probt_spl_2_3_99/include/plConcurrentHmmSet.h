/*=============================================================================
 * Product        : ProBT 
 * File           : plConcurrentHmmSet.h
 * Author         : Kamel Mekhnacha
 * Creation       : Mon Jun 18 15:57:58 2007
 *
 *=============================================================================
 *        (c) Copyright 2006, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */

#ifndef plConcurrentHmmSet_h
#define plConcurrentHmmSet_h

#include "plConfig.h"

#include <vector>
#include <string>


#include <plDLLExport.h>
#include <plMath.h>
#include <plHMM.h>


SPL_DECL2(plConcurrentHmmSet);

/** @class plConcurrentHmmSet
    This class implements a model for recognition and filtering using
    a set of HMMs.
*/
class PL_DLL_API plConcurrentHmmSet  :public plBuiltinModel
{
 public:
  /** @brief Default constructor */
  plConcurrentHmmSet();
    
  /** @brief Destructor */
  ~plConcurrentHmmSet(){}
  

  /** @brief Insert a new HMM model.
      @param[in] model the HMM model to be inserted.
  */
  void add_model(const plHMM &model);

  /** @brief Removes the model at a given index.
      @param[in] index the index of the model to be removed.
  */
  void remove_model(unsigned int index);

  /** @brief Removes the model with a given name.
      @param[in] name the name of the model to be removed.
  */
  void remove_model(const std::string &name);

  /** @brief Clear all HMM models.
  */
  void clear_models();


  /** @brief Set/change transition matrices between models (HMMs).

      @param[in] transition_matrix_final_states transition matrix for final states
      @param[in] transition_matrix_non_final_states transition matrix for non final states
  */
  void set_transition_matrices(const std::vector <std::vector < plProbValue > > &transition_matrix_final_states,
			       const std::vector <std::vector < plProbValue > > &transition_matrix_non_final_states);
  
  /** @brief Set/change an element of the transition matrices between models (HMMs).
  */
  void set_transition_matrices(unsigned int mp, unsigned int m,
			       plProbValue final,
			       plProbValue non_final);

  /** @brief Return the number of the contained HMMs.
      @return the number of the contained HMMs.
  */
  inline unsigned int get_nmodels()const{return models_.size();}
  
  /** @brief Get, for a given observation sequence, the most probable model
      and the corresponding most probable state sequence.
      
      @param[out] state_sequence the most probable state sequence corresponding to the most probable model (HMM)
      @param[in] obs_seq observation sequence
      @param[in] only_final_states a flag to say whether the retained model must be in a final state
      
      @return the index of the most probable model (HMM)
  */
  unsigned int get_best_model_most_probable_state_sequence(plHMM::StateSequence_t &state_sequence, 
                                                           const plHMM::ObservationSequence_t &obs_seq,
                                                           bool only_final_states = false)const;

  /** @brief Get, for a given observation sequence, the most probable model
      and the corresponding most probable state sequence.
      
      @param[out] state_sequence the most probable state sequence corresponding to the most probable model (HMM)
      @param[in] obs_seq observation sequence
      @param[in] only_final_states a flag to say whether the retained model must be in a final state
      
      @return the name of the most probable model (HMM)
  */
  const std::string &get_best_model_most_probable_state_sequence_by_name(plHMM::StateSequence_t &state_sequence, 
                                                                         const plHMM::ObservationSequence_t &obs_seq,
                                                                         bool only_final_states = false)const;
  
  
  /** @brief Get, for a given observation sequence, the most probable model.
      
      @param[in] obs_seq observation sequence
      
      @return the index of the most probable model (HMM)
  */
  unsigned int get_best_model(const plHMM::ObservationSequence_t &obs_seq)const;
  
  /** @brief Get, for a given observation sequence, the most probable model.
      
      @param[in] obs_seq observation sequence
      
      @return the name of the most probable model (HMM)
  */
  const std::string &get_best_model_name(const plHMM::ObservationSequence_t &obs_seq)const;


  /**  @brief Given an observation sequence, get  for each model (HMM), the corresponding log-probability value.
       
       @param[out] models_log_probabilities log probability for each model (HMM) 
       @param[in] observation_sequence observation sequence

       @return the index of the most probable model (HMM)
  */
  unsigned int get_all_models_log_probabilities(std::vector <plFloat> &models_log_probabilities,
                                                const plHMM::ObservationSequence_t &observation_sequence)const;

  /**  @brief Given an observation sequence, get  for each model (HMM), the corresponding log-probability value.
       
       @param[out] models_log_probabilities log probability for each model (HMM) 
       @param[in] observation_sequence observation sequence

       @return the name of the most probable model (HMM)
  */
  const std::string &get_all_models_log_probabilities_name(std::vector <plFloat> &models_log_probabilities,
                                                           const plHMM::ObservationSequence_t &observation_sequence)const;
  
  /**  @brief Given an observation sequence, get  for each model (HMM), the  most probable state sequence
       and the corresponding log-likelihood value.
       
       @param[out] observation_log_likelihoods observation log-likelihood value for each model (HMM) 
       (i.e., P(observation_sequence | HMM_i) )
       @param[out] state_sequences most probable sequence for each model (HMM)
       @param[in] observation_sequence observation sequence
  */
  void get_all_models_most_probable_state_sequences(std::vector <plFloat> &observation_log_likelihoods,
                                                    std::vector < plHMM::StateSequence_t > &state_sequences, 
                                                    const plHMM::ObservationSequence_t &observation_sequence)const;  
  
  /** @brief Use an observation to update  the probability table over the contained models (HMMs)
      and return the likelihood of the observation.
      
      @param[in] observation the observation vector
      @return the likelihood of the observation
  */
  plFloat update_models_states_probabilities(const plHMM::Observation_t &observation);
  
  /** @brief Get the probability (belief) for a given model (HMM)
      @param[in] n the index of the model
      @return the probability (belief) for the model with index @em n
  */
  plProbValue get_model_probability(unsigned int n)const;

  /** @brief Get the probability (belief) for a given model (HMM)
      @param[in] name the name of the model
      @return the probability (belief) for the model with name @em name
  */
  plProbValue get_model_probability(const std::string &name)const;


  /** @brief Reset the state distribution to the initial one for each model (HMM). 
      ATTENTION: this function does not reset models probabilities to the initial ones

      \sa reset()
   */
  void reset_models_state_distributions();


  /** @brief Reset the HMM set:
      - reset the state distribution to the initial one for each model (HMM)
      - reset models probabilities to the initial ones
   */
  void reset();


  /** @brief Get the logarithm of the probability (belief) table over all models (HMMs)
      @return the logarithm of the probability (belief) table over all models (HMMs)
      \sa get_models_probabilities()
  */
  inline const std::vector <plFloat> &get_models_probabilities_log()const
  {return models_probabilities_log_;}


  /** @brief Get the probability (belief) table over all models (HMMs)
      @param[out] models_probabilities the probability (belief) table over all models (HMMs)
      \sa get_models_probabilities_log()
  */
  void get_models_probabilities(std::vector <plProbValue> &models_probabilities)const;

  /** @brief Set the current probability (belief) table over all models (HMMs)
      @param[in] models_probabilities the probability (belief) table over all models (HMMs)
  */
  void set_models_probabilities(const std::vector <plProbValue> &models_probabilities);


  /** @brief Set the initial and the current probability (belief) table over all models (HMMs)
      @param[in] init_models_probabilities the initial probability (belief) table over all models (HMMs)
  */
  void set_init_models_probabilities(const std::vector<plProbValue> &init_models_probabilities);

  /** @brief Same as set_init_models_probabilities() above. 
      Kept only for backward compatibility 
  */
  inline void set_models_prior(const std::vector <plProbValue> &models_probabilities)
  {
    set_init_models_probabilities(models_probabilities);
  }
 
  /** @brief Get transition matrices between models (HMMs).

      @param[out] transition_matrix_final_states transition matrix for final states
      @param[out] transition_matrix_non_final_states transition matrix for non final states
  */
  void get_transition_matrices(std::vector <std::vector < plProbValue > > &transition_matrix_final_states,
			       std::vector <std::vector < plProbValue > > &transition_matrix_non_final_states)const;


  /** @brief Get a reference to the nth model (HMM).

      @param[in] n the number of the model (HMM)  
      @return a reference to the nth model (HMM)
  */
  plHMM &get_model(unsigned int n);

  /** @brief Get a reference to the nth model (HMM).

      @param[in] name the name of the model (HMM)  
      @return a reference to the nth model (HMM)
  */
  plHMM &get_model(const std::string &name);
	
  /** @brief Get a const reference to the nth model (HMM) if any.

      @param[in] n the number of the model (HMM)  
      @return a const reference to the nth model (HMM)
  */
  const plHMM &get_model(unsigned int n) const;

  /** @brief Get a const reference to the model (HMM) with a given name if any.

      @param[in] name the name of the model (HMM)  
      @return a const reference to the nth model (HMM)
  */
  const plHMM &get_model(const std::string &name) const;
	
  /**
     \brief Make a pure prediction step.
  */
  void predict_models_states_probabilities();

   /**
     Compute the dissimilarity [Rabiner 89] of HMMi to HMMj.
     \sa compute_symmetric_dissimilarity_matrix
  */
  void compute_dissimilarity_matrix(std::vector< std::vector<plFloat> > &dissimilarity_matrix,
                                    const std::vector< std::vector<plHMM::ObservationSequence_t> > &obs_sequences)const;

  /**
     Compute the symmetric dissimilarity [Rabiner 89] between each pair (HMMi, HMMj).
     \sa compute_dissimilarity_matrix
  */
  void compute_symmetric_dissimilarity_matrix(std::vector< std::vector<plFloat> > &symmetric_dissimilarity_matrix,
                                              const std::vector< std::vector<plHMM::ObservationSequence_t> > &obs_sequences)const;


  /**
     \brief Returns @em true iff the nth HMM's probability to be in a final state
     is greater or equal to @em prob.
  */
  bool is_in_final_state(unsigned int n, plProbValue prob) const;
  
  /** @brief Get the initial probability (belief) table over all models (HMMs)
      @return the initial probability (belief) table over all models (HMMs)
  */
  const std::vector<plProbValue> &get_init_models_probabilities() const
  {
    return init_models_probabilities_;
  }

 protected:
  SPL_ACCESS2(plConcurrentHmmSet);
  
  plFloat compute_prediction_prob_log(unsigned int m)const;
  plFloat compute_transition_prob_log(unsigned int mp, unsigned int m)const;
  void update_size();
  
  void Output(std::ostream& out)const;

 protected:
  std::vector <plHMM> models_;
  std::vector <plFloat> models_probabilities_log_;
  
  std::vector <std::vector<plFloat> > transition_log_final_;
  std::vector <std::vector<plFloat> > transition_log_non_final_;
  
  bool has_default_transition_matrices_;

 protected:
  mutable std::vector <plFloat> log_vals_;
  std::vector <plFloat> new_models_probabilities_log_;

  mutable std::vector< std::vector<plFloat> > state_distribution_log_;
  std::vector<plProbValue> init_models_probabilities_;
};


#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
