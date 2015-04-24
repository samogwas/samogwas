/// \file plHMM.h

/*=============================================================================
 * Product        : ProBT 
 * File           : plHMM.h
 * Author         : Kamel Mekhnacha
 * Creation       : Fri May 18 22:06:27 2007
 *
 *=============================================================================
 *        (c) Copyright 2007, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */


#ifndef PL_HMM_H
#define PL_HMM_H

#include "plConfig.h"

#include <vector>
#include <string>

#include <plCndDistribution.h>
#include <plDistribution.h>
#include <plSymbol.h>
#include <plVariableIndexer.h>

class plLearnObject;

SPL_DECL2(plHMM);

/** \brief Robust computation of the log of a sum of a vector of exponentials.

    It's based on the formula:
    \f{eqnarray*}{
    ln(x+y) &=& \ln(x) + \ln(x+y) - \ln(x) \\
             &=& \ln(x) + \ln\left(\frac{x+y}{x}\right) \\
             &=& \ln(x) + \ln\left(1 + \frac{y}{x}\right) \\
             &=& \ln(x) + \ln\left(1 + e^{ \ln(\frac{y}{x})}\right) \\
             &=& \ln(x) + \ln\left(1 + e^{ \ln(y) - \ln(x)}\right) \\
    \f}


    For numerical stability, it's important to use this formula with \f$x\f$
    larger than \f$y\f$. Indeed, it's important to check the size of the
    logarithms of the summands, because the numerical stability of this
    computation depends on small values of the exponential term in log(1 +
    exp(log(y)-log(x))). Small values can be ensured by always subtracting the
    larger argument from the smaller argument so that the sign of the
    difference of logarithms is negative.  For example, consider the case where
    the summands are exp(60) and exp(-10). When considering decimal logarithms
    and exponentials for simplification, the logarithms of the summands are 60
    and -10, and the logarithm of the sum can be computed either as
    log(exp(60)) + log(1 + exp(-70)), or as log(exp(-10)) + log(1 +
    exp(70)). In the first alternative, the quantity exp(-70) gets underflowed
    to zero, and the result is 60, which is as accurate as most machine
    precisions will allow. In the second alternative, exp(70) will evaluate to
    inf, the floating point constant used to represent a number to large to be
    represented in the machine precision, and the numerical value of the
    computation will be lost. When computing the logarithm of the sum of two
    numbers that have substantially different magnitudes, the exact form of the
    computation employed can make the difference between a usable result and an
    overflow error.
   
    \param[in] logs an array  of logarithm values
    \param[in] len the length of 'logs' array
    \return log ( Sum_i exp( logs[i] ) )
*/
template<typename LogType>
LogType plLogSum(const LogType* logs, const unsigned int len)
{
  const LogType minus_inf = std::log(LogType(0.0));
  const LogType one = 1.0;
  const LogType log_of_2 = std::log(LogType(2.0));

  
  if(len == 0) return minus_inf;
  if(len == 1) return logs[0];
  
  LogType result;
  

  if(minus_inf == logs[0] && minus_inf == logs[1]){
    result = minus_inf;
  }
  else if(minus_inf == logs[0]){
    result = logs[1];
  }
  else if(minus_inf == logs[1]){
    result = logs[0];
  }
  else if(logs[1] < logs[0]){
    result = logs[0] + std::log(one + std::exp(logs[1] - logs[0]));
  }
  else if(logs[1] > logs[0]){
    result = logs[1] + std::log(one + std::exp(logs[0] - logs[1]));
  } 
  else { // logs[1] == logs[0]
    result = logs[1] + log_of_2;
  }
  
  LogType term;
  for(unsigned int i = 2; i < len; ++i){
    term = logs[i];
    
    if(minus_inf == result && minus_inf == term){
      result = minus_inf;
    }
    else if(minus_inf == result){
      result = term;
    }
    else if(minus_inf == term){
      //result = result; nothing to do
    }
    else if(term < result){
      result += std::log(one + std::exp(term - result));
    }
    else if(term > result){
      result = term + std::log(one + std::exp(result - term));
    }
    else{ // term == result
      result += log_of_2;
    }
  }
  
  return result;
}

//=============================================================================
/** \brief Robust computation of the log of a sum of a vector of exponentials.
    
    \param[in] logs a vector  of logarithm values
    \return log ( Sum_i exp( logs[i] ) )
*/
template<typename LogType>
inline LogType plLogSum(const std::vector <LogType> &logs)
{
  return plLogSum( &logs[0], logs.size() );
}

//=============================================================================
/** \class plHMM
    \brief This class implements Hidden Markov Model (HMM).

    An HMM is defined as:
    - a discrete state space \f$S\f$,
    - an emission (observation) space (alphabet) \f$O\f$,
    - an initial state distribution \f$ P(S_0) \f$,
    - a transition distribution \f$ P(S_t | S_{t-1}) \f$,
    - an emission (observation) distribution \f$ P( O_t | S_t) \f$. 

    The state variable \f$S\f$ has to be a discrete (integer) variable
    taking its values in [0,...,nstates-1].

    The emission (observation) variable \f$O\f$ can be discrete or
    continuous. It can be also multidimensional.  Moreover, the
    emission (observation) distribution can be decomposed. Examples for a 2D observation:
    - \f$ P(O_x O_y | S) = P(O_x | S) P(O_y | S) \f$,
    - \f$ P(O_x O_y | S) = P(O_x | S) P(O_y | S O_x) \f$,
    - \f$ P(O_x O_y | S) = P(O_x | S) P(O_y | O_x) \f$.
	
    Using ProBT, such a decomposed emission distribution can be expressed
    using the @em plComputableObject class to define the emission distribution
    as a product.
	
    Given an already constructed HMM, this class implements:
    - Evaluation: Given an observation sequence (or sequences), return the corresponding log-likelihood.
    - Decoding:   Given an observation sequence, return the most probable state sequence.
    - Filtering:  Given an observation as input, online update of the state distribution.
    - Simulation: Generate observation/state sequences according to HMM's parameters.

    It also implements HMM's distributions learning (assuming that the number of states and the emission space are KNOWN):
    - Supervised Learning: Given a set of state/observation sequences, find the HMM's distributions that maximize the sequences likelihood.
    - Unsupervised Learning: Given a set of observation sequences, find the HMM's distributions that maximize the sequences likelihood.

*/
class PL_DLL_API plHMM :public plBuiltinModel
{
public:
  /** \brief Type for an observation element */
  typedef double ObservationElement_t;
  // typedef plFloat ObservationElement_t;

  /** \brief Type for observations */
  typedef std::vector < ObservationElement_t >        Observation_t;

  /** \brief Type for observation sequences */
  typedef std::vector < Observation_t > ObservationSequence_t;

  /** \brief Type for state values */
  typedef unsigned int                  State_t;

  /** \brief Type for state values sequences */
  typedef std::vector < State_t >       StateSequence_t;
public:
  /** Default constructor */
  plHMM();
	
  /** \brief Constructor using model explicit probability tables.
      \param[in] init_state_distribution initial state distribution table.
      \param[in] transition_distribution transition distribution matrix.
      \param[in] emission_distribution emission (observation) distribution matrix.
  */
  plHMM(const std::vector <plProbValue> &init_state_distribution,
	const std::vector < std::vector <plProbValue> > &transition_distribution,
	const std::vector < std::vector <plProbValue> > &emission_distribution);
	
  /** \brief Constructor using model distributions.
      \param[in] init_state_distribution initial state distribution.
      \param[in] transition_distribution transition distribution.
      \param[in] emission_distribution emission (observation) distribution.  
      \param[in] tabulate_emission_distribution a parameter to say if the 
      emission (observation) distribution should be tabulated or not.
      \param[in] tabulate_transition_distribution a parameter to say if the 
      transition distribution should be tabulated or not.
		
      The emission (observation) variable can be discrete or continuous. It can
      be also multidimensional.  Moreover, the emission (observation)
      distribution can be decomposed. Examples for a 2D observation: 
      - \f$ P(O_x O_y | S) = P(O_x | S) P(O_y | S) \f$, 
      - \f$ P(O_x O_y | S) = P(O_x | S) P(O_y | S O_x) \f$, 
      - \f$ P(O_x O_y | S) = P(O_x | S) P(O_y | O_x) \f$.

      Using ProBT, a such decomposed emission distribution can be
      expressed using the @em plComputableObject class to define the
      emission distribution as a product.
  */
  plHMM(const plDistribution &init_state_distribution,
	const plCndDistribution &transition_distribution,
	const plCndDistribution &emission_distribution,
	bool tabulate_emission_distribution = false,
        bool tabulate_transition_distribution = true);
	
  /** Destructor */
  ~plHMM(){};

  /** Return true if the distributions are empty. */
  inline bool is_empty() const { return PS0_.is_empty(); }
			

  /** Sets HMM's name.
      @param name the new name of the HMM.
  */
  void set_name(const std::string &name)
  {
    name_ = name;
  }
	
  /** Get HMM's name.
      @return  the name of the HMM.
  */
  const std::string &get_name()const
  {
    return name_;
  }

  /** \brief Supervised training.
      \param[in] observation_sequences a vector of observation sequences.
      \param[in] state_sequences a vector of state sequences.
      \param[in] emission_distribution_learner an online learning object to choose the form of 
      the emission (observation) distribution to be learned.
      \param[in] init_state_distribution_learner an online learning object to choose the form 
      of the initial state distribution to be learned.
      \param[in] transition_distribution_learner an online learning object to choose the form 
      of the transition distribution to be learned.
      \param[in] tabulate_emission_distribution a parameter to say if the emission 
      (observation) distribution should be tabulated or not.
      \param[in] tabulate_transition_distribution a parameter to say if the 
      transition distribution should be tabulated or not.

      Using this constructor, the observation dimension and type are
      extracted form the @em emission_distribution_learner object.
      If @em init_state_distribution_learner (@em
      transition_distribution_learner) is null, the initial state
      distribution (transition distribution) is learnt as a simple
      histogram (s).

      The emission (observation) variable (O) can be discrete or continuous. It
      can be also multidimensional. Moreover, the emission (observation)
      distribution can be decomposed. Examples for a 2D observation:
      - \f$ P(O_x O_y | S) = P(O_x | S) P(O_y | S) \f$, 
      - \f$ P(O_x O_y | S) = P(O_x | S) P(O_y | S O_x) \f$, 
      - \f$ P(O_x O_y | S) = P(O_x | S) P(O_y | O_x) \f$.

      Using ProBT, a such decomposed emission distribution can be
      expressed using the @em plLearnDistribVector class to define the
      learner of the emission distribution.

      If you want to freeze a given distribution (instead to learn
      it), use special learner @em plLearnFrozenDistribution.
  */
  void train_supervised(const std::vector <ObservationSequence_t>  &observation_sequences,
			const std::vector <StateSequence_t>  &state_sequences,
			plLearnObject* emission_distribution_learner,
			plLearnObject* init_state_distribution_learner  =  0,
			plLearnObject* transition_distribution_learner  =  0,
			bool tabulate_emission_distribution = false,
                        bool tabulate_transition_distribution = true);
	
  /** \brief Supervised training.	
      \param[in] nstates the number of states.
      \param[in] observation_sequences a vector of observation sequences.
      \param[in] state_sequences a vector of state sequences.
      \param[in] obs_min_value the min value for the observation.
      \param[in] obs_max_value the max value for the observation.
      \param[in] tabulate_emission_distribution a parameter to say if the emission 
      (observation) distribution should be tabulated or not.
      \param[in] tabulate_transition_distribution a parameter to say if the 
      transition distribution should be tabulated or not.
		
      Using this method, the observation is supposed to be one-dimensional 
      discrete in the range [obs_min_value,...,obs_max_value]. All distributions
      are learnt as simple histograms.
  */
  void train_supervised(unsigned int nstates,
			const std::vector <ObservationSequence_t>& observation_sequences,
			const std::vector <StateSequence_t> &state_sequences,
			int obs_min_value, int obs_max_value,
                        bool tabulate_emission_distribution = false,
                        bool tabulate_transition_distribution = true);
	
  /** \brief Unsupervised training using iterative viterbi algorithm.
      \param[in] observation_sequences a vector of observation sequences.
      \param[in] emission_distribution_learner an online learning object to choose the form 
      of the emission (observation) distribution to be learned.
      \param[in] init_state_distribution_learner an online learning object to choose the form 
      of the initial state distribution to be learned.
      \param[in] transition_distribution_learner an online learning object to choose the form 
      of the transition distribution to be learned.
      \param[in] tabulate_emission_distribution a parameter to say if the emission (observation) 
      distribution should be tabulated or not.
      \param[in] tabulate_transition_distribution a parameter to say if the 
      transition distribution should be tabulated or not.
      \param[in] left_to_right_init if true, state initialization will be "left to right" instead of random

      \return the loglikelihood

      Using this method, the observation dimension and type are
      extracted form the @em emission_distribution_learner object.
      If @em init_state_distribution_learner (@em
      transition_distribution_learner) is null, the initial state
      distribution (transition distribution) is learned as a simple
      histogram (s).

      The emission (observation) variable can be discrete or continuous. It can
      be also multidimensional.  Moreover, the emission (observation)
      distribution can be decomposed. Examples for a 2D observation:
      - \f$ P(O_x O_y | S) = P(O_x | S) P(O_y | S) \f$, 
      - \f$ P(O_x O_y | S) = P(O_x | S) P(O_y | S O_x) \f$, 
      - \f$ P(O_x O_y | S) = P(O_x | S) P(O_y | O_x) \f$.

      Using ProBT, a such decomposed emission distribution can be
      expressed using the @em plLearnDistribVector class to define the
      learner of the emission distribution.

      If you want to freeze a given distribution (instead to learn
      it), use special learner @em plLearnFrozenDistribution.
  */
  plFloat train_unsupervised_viterbi(const std::vector <ObservationSequence_t>  &observation_sequences,
				     plLearnObject* emission_distribution_learner,
				     plLearnObject* init_state_distribution_learner  =  0,
				     plLearnObject* transition_distribution_learner  =  0,
				     bool tabulate_emission_distribution = false,
                                     bool tabulate_transition_distribution = true,
                                     bool left_to_right_init = false);

  /** \brief Unsupervised training using iterative viterbi algorithm.
      \param[in] nstates the number of states.
      \param[in] observation_sequences a vector of observation sequences.
      \param[in] obs_min_value the min value for the observation.
      \param[in] obs_max_value the max value for the observation.
      \param[in] tabulate_emission_distribution a parameter to say if the emission 
      (observation) distribution should be tabulated or not.
      \param[in] tabulate_transition_distribution a parameter to say if the 
      transition distribution should be tabulated or not.
      \param[in] left_to_right_init if true, state initialization will be "left to right" instead of random
	
      Using this method, the observation is supposed to be one-dimensional 
      integer in the range [obs_min_value,...,obs_max_value]. All distributions
      are learnt as @em plLearnLaplace (ie, histograms with one sample point initialization).
  */
  plFloat train_unsupervised_viterbi(unsigned int nstates,
				     const std::vector <ObservationSequence_t>  &observation_sequences,
				     int obs_min_value, int obs_max_value,
                                     bool tabulate_emission_distribution = false,
                                     bool tabulate_transition_distribution = true,
                                     bool left_to_right_init = false);
	
  /** \brief Returns the number of states.
      \return the number of states.
  */
  unsigned int get_nstates()const{return nstates_;}
	
  /** \brief Returns the initial state distribution.
      \return the initial state distribution.
  */
  const plDistribution &get_init_state_distribution()const{return PS0_;}

  /** \brief Returns the transition distribution.
      \return the transition distribution.
  */
  const plCndDistribution &get_transition_distribution()const{return PSgivenSp_;}

  /** \brief Returns the emission (observation) distribution.
      \return the emission (observation) distribution.
  */
  const plCndDistribution &get_emission_distribution()const{return POgivenS_;}
	
  /**
     \brief Updates the distribution on the state using a given observation and returns the corresponding log-likelihood.
     \param[in] observation the observation vector.  
     \return the log-likelihood of the observation.

     This method allows to use the HMM as a Bayesian filter to update, after each observation,
     the probability distribution over the states.
     \sa get_state_distribution
  */
  plFloat update_states_probabilities(const Observation_t &observation);

  /**
     \brief Compute the logarithm of the distribution over the states using a
     given observation and returns the corresponding log-likelihood.

     \param[in] observation the observation vector.  
     \param[out] states_probabilities_log the computed log-distribution.

     \return the log-likelihood of the observation.

     This method allows to use the HMM as a Bayesian filter to update, after each observation,
     the probability distribution over the states.
     \sa get_state_distribution()
     \sa update_states_probabilities(const Observation_t &)
  */
  plFloat compute_states_probabilities_log(const Observation_t &observation,
                                           std::vector<plFloat> &states_probabilities_log)const;
	

  /**
     \brief Returns the  probability distribution over the states.
     \param[out] state_probabilities the returned probability table.

     This method is to be used with the @em update_states_probabilities method.

     \sa update_states_probabilities get_state_distribution_log
  */
  void get_state_distribution(std::vector<plProbValue> &state_probabilities)const;


  /**
     \brief Returns the  log of the probability distribution over the states.
     \return log of the probability distribution over the states.
	   
     This method is to be used with the @em update_states_probabilities method.
	   
     \sa update_states_probabilities get_state_distribution
  */
  const std::vector<plFloat> &get_state_distribution_log()const
  {
    return log_PS_;
  }
	
  /**
     \brief Returns the  most probable state sequence given an input
     observation sequence and the corresponding log-likelihood.
     \param[out] loglikelihood the log-likelihood of the observation sequence.
     \param[out] state_sequence the most probable state sequence.
     \param[in] observation_sequence the input observation sequence.
  */
  void get_most_probable_state_sequence(plFloat &loglikelihood,
					StateSequence_t &state_sequence, 
					const ObservationSequence_t &observation_sequence)const;	
	
	
  /**
     \brief Returns the log-likelihood for a given input observation sequence.
     \return the log-likelihood for a given input observation sequence.
  */
  plFloat get_observation_sequence_log_likelihood(const ObservationSequence_t &observation_sequence)const;


  /**
     \brief Returns the log-likelihood for a given input set of observation sequences.
     \return the log-likelihood for a given input set of observation sequences.
  */
  plFloat get_observation_sequences_log_likelihood(const std::vector <ObservationSequence_t> &observation_sequences)const;	
	
  /** 
      \brief Generate a sequence in a CSV file by simulating the HMM.
      \param[in] seq_length sequence's length.
      \param[in] csv_file_name the name of the CSV file to be generated.
      \param[in] save_state if @em true the state sequence will be also generated in the file.
      \param[in] separator the separator character to be used in the CSV file.
  */
  bool generate_sequence(unsigned int seq_length, const std::string &csv_file_name,
			 bool save_state = false, char separator = ';')const;
	
	
  /** 
      \brief Generate an observation/state sequence by simulating the HMM.
      \param[in] seq_length sequence's length.
      \param[out] observation_sequence the generated observation sequence.
      \param[out] state_sequence the generated state sequence.
  */
  void generate_sequence(unsigned int seq_length,
			 ObservationSequence_t  &observation_sequence,
			 StateSequence_t &state_sequence)const;
	

  /** 
      \brief Generate an observation sequence by simulating the HMM.
      \param[in] seq_length sequence's length.
      \param[out] observation_sequence the generated observation sequence.
  */	
  void generate_sequence(unsigned int seq_length,
			 ObservationSequence_t  &observation_sequence)const;
	

  /**
     \brief Set/change the final states.
     \param[in] final_states final states vector.
  */
  void set_final_states(const std::vector<State_t> &final_states);


  /**
     \brief Return the final states.
     \return the final states
  */
  const std::vector<State_t> &get_final_states()const{return final_states_;}

  /**
     \brief Return @em true if the state @em s is final.
     \return @em true if the state @em s is final.
  */
  bool is_final_state(State_t s)const;

  /**
     \brief Returns @em true iff the probability to be in a final state
     is greater or equal to @em prob.
  */
  bool is_in_final_state(plProbValue prob) const;


  /** \brief Set/Change state distribution 
      \param[in] state_probabilities distribution over state values.
  */
  void set_state_distribution(const std::vector<plProbValue> &state_probabilities);
	
  /** \brief Set/Change state distribution \param[in] state_probabilities_log
      the logarithm of the distribution over state values.
  */
  void set_state_distribution_log(const std::vector<plFloat> &state_probabilities_log);


  /** \brief Set/Change state distribution 
      \param[in] state_distribution distribution over state values.
  */
  void set_state_distribution(const plDistribution&state_distribution);


  /** \brief Reset the state distribution to the initial one.
   */
  void reset_state_distribution();
	
  /** \brief Set the number of states. 
      \param[in] n the number of states
  */
  void set_nstates(unsigned int n);
	
  /**
     \brief Make a prediction step.
  */
  void predict_states_probabilities();


  void set_init_state_table(const std::vector <plProbValue> &init_state_distribution,
                            bool is_already_normalized = false);
  void set_init_state_distribution(const plDistribution &init_state_distribution);
  
  void set_transition_table(const std::vector < std::vector <plProbValue> > &transition_distribution,
                            bool is_already_normalized = false);
  
  void set_transition_distribution(const plCndDistribution &transition_distribution,
                                   bool tabulate_transition_distribution);
	
  void set_emission_table(const std::vector < std::vector <plProbValue> > &emission_distribution);
  void set_emission_distribution(const plCndDistribution &emission_distribution,
				 bool tabulate_emission_distribution);

  /**
     Compute the dissimilarity [Rabiner 89] to another
     HMM using a set of observation sequences.

     \sa compute_symmetric_dissimilarity
  */
  plFloat compute_dissimilarity_to(const plHMM &other,
                                   const std::vector< ObservationSequence_t> &this_obs_sequences)const;

  /**
     Compute the symmetric dissimilarity [Rabiner 89] to another
     HMM using a set of observation sequences.

     \sa compute_dissimilarity_to
  */
  plFloat compute_symmetric_dissimilarity(const plHMM &other,
                                          const std::vector< ObservationSequence_t> &this_obs_sequences,
                                          const std::vector< ObservationSequence_t> &other_obs_sequences)const
  {
    return PL_HALF*(this->compute_dissimilarity_to(other, this_obs_sequences) +
                    other.compute_dissimilarity_to(*this, other_obs_sequences));
  }

  /** \brief Get back the value of the parameter tabulate_transition_distribution
      that was given at construction time.
   */
  bool did_tabulate_transition_distribution() const;

  /** \brief Get back the value of the parameter tabulate_emission_distribution
      that was given at construction time.
   */
  bool did_tabulate_emission_distribution() const;


protected:
  SPL_ACCESS2(plHMM);
  void set_observation_dimension(unsigned int observation_dimension)const;

  void set_observation_sequences_length(unsigned int observation_sequences_length)const;
	
  void create_internal_variables();

  unsigned int set_variables_from_distributions(const plDistribution &init_state_distribution,
						const plCndDistribution &transition_distribution,
						const plCndDistribution &emission_distribution);

 
	
  void construct_distributions(const std::vector <plProbValue> &init_state_distribution,
			       const std::vector < std::vector <plProbValue> > &transition_distribution,
			       const std::vector < std::vector <plProbValue> > &emission_distribution);
	
  inline plFloat emission_log_prob(const Observation_t &obs, State_t s)const;
  
  inline plFloat transition_log_prob(State_t sp, State_t s)const;
	
  void set_emission_discretization_parameters(unsigned int emission_variable_ncases);
	
  void insert_sequence_statistics(const ObservationSequence_t  &observation_sequence,
				  const StateSequence_t &state_sequences,
				  plLearnObject* emission_distribution_learner,
				  plLearnObject* init_state_distribution_learner,
				  plLearnObject* transition_distribution_learner,
				  std::vector<State_t> &S_Sp_val,
				  std::vector<ObservationElement_t> &O_S_val)const;
	
  void init_state_sequences(std::vector <StateSequence_t>  &state_sequences,
                            const std::vector <ObservationSequence_t> &obs_seqs,
                            bool left_to_right_init)const;
  
  plFloat train_unsupervised_viterbi_iteration(const std::vector <ObservationSequence_t>  &observation_sequences,
					       std::vector <StateSequence_t>  &state_sequences,
					       plLearnObject* emission_distribution_learner,
					       plLearnObject* init_state_distribution_learner,
					       plLearnObject* transition_distribution_learner,
					       bool tabulate_emission_distribution,
                                               bool tabulate_transition_distribution);
  
  void learn_distribs(const std::vector <ObservationSequence_t>  &observation_sequences,
		      const std::vector <StateSequence_t>  &state_sequences,
		      plLearnObject* emission_distribution_learner,
		      plLearnObject* init_state_distribution_learner,
		      plLearnObject* transition_distribution_learner,
		      bool tabulate_emission_distribution,
                      bool tabulate_transition_distribution);

  void Output(std::ostream& out)const;

protected:
	
  unsigned int nstates_;
	
  std::vector <plFloat> log_PS0_;
  plDistribution PS0_;

  std::vector < std::vector <plFloat> > log_PSgivenSp_;
  plCndDistribution PSgivenSp_;
	
  std::vector < std::vector <plFloat> > log_POgivenS_;
  plCndDistribution POgivenS_;
	
  plSymbol Sp_;
  plSymbol S_;
  plVariablesConjunction O_;
	
  mutable unsigned int observation_dimension_;
  mutable unsigned int observation_sequences_length_;

  bool tabulated_emission_distribution_;
  unsigned int emission_variable_ncases_;

  std::vector <plFloat> log_PS_;

  std::vector<State_t> final_states_;
  std::vector<bool> is_final_state_;

  mutable std::vector <plFloat> logrho_;
  mutable std::vector <plFloat> new_log_rho_;
	
  mutable std::vector <plFloat> log_alpha_;
  mutable std::vector <plFloat> new_log_alpha_;
  mutable std::vector <plFloat> log_alpha_tmp_;

  mutable std::vector < std::vector <State_t > > viterbi_;

  std::string name_;

  bool tabulated_transition_distribution_;

  plVariableIndexer observation_indexer_;

  mutable std::vector <plFloat> log_PS_tmp_;
};




#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
