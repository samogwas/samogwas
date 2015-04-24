/*=============================================================================
 * Product        : ProBT 
 * File           : plBayesianFilter.h
 * Author         : Kamel Mekhnacha
 * Creation       : Fri Apr 16 15:28:11 2004
 *
 *=============================================================================
 *     (c) Copyright 2000, Centre National de la Recherche Scientifique,
 *                              all rights reserved
 *=============================================================================
 *
 *------------------------- Description ---------------------------------------
 *   
 *   
 *-----------------------------------------------------------------------------
 */



#ifndef plBayesianFilter_h
#define plBayesianFilter_h

#include "plConfig.h"

#include <plCndDistribution.h>
#include <plDataValues.h>
#include <plDistribution.h>
#include <plVariablesConjunction.h>
#include <plObject.h>


class plValues;

class kplProbMap;
class kplComputableObject;

SPL_DECL2(plBayesianFilter);

/** @brief This class implements general purpose Bayesian Filters.

    A Bayes filter is defined by:
    - the current state variable \f$ S_t \f$,
    - the previous state variable \f$ S_{t-1} \f$,
    - the current observation variable \f$ O_t \f$,
    - the current applied action (control) variable \f$ A_t \f$,
    - the state initial distribution (at time t = 0) \f$ P(S_0) \f$,
    - the prediction model (dynamic) distribution \f$ P(S_t ~|~ S_{t-1} A_t) \f$,
    - the observation distribution \f$ P( O_t ~|~ S_t A_t) \f$.

    In the general case, the joint distribution representing the Bayes filter
    is (\f$ A_t \f$ is always instantiated):

    \f[ P(S_{t-1} S_t O_t ~|~ A_t) = P(S_{t-1}) P(S_t ~|~ S_{t-1} A_t) P( O_t ~|~ S_t A_t) \f]
 
    If there is no action variable (\f$ A = \emptyset\f$), we have a no-action Bayes filter:
    \f[ P(S_{t-1} S_t O_t) = P(S_{t-1}) P(S_t ~|~ S_{t-1}) P( O_t ~|~ S_t) \f]

    If the prediction model is identity (not dynamic) (\f$ P(S_t ~|~ S_{t-1} A_t) = Dirac(S_t, S_{t-1}) \f$),
    then the filter is defined as:
    \f[ P(S_t O_t ~|~ A_t) = P(S_t) P( O_t ~|~ S_t A_t) \f]
    
    If the system is not dynamic and have no action variables, we have:
    \f[ P(S_t O_t) = P(S_t) P( O_t ~|~ S_t) \f]

    A Bayes filter is provided with @em 'prediction' and @em 'observation' methods.
    The prediction step leads to the calculation:
    \f[ P(S_t ~|~A_t=a) = \sum_{S_{t-1}} P(S_{t-1}) P( S_t ~|~ S_{t-1} A_t=a) \f]

    When the state space \f$ S \f$ is big (or continuous), this calculation may
    be very time consuming. Therefore, this class allows exact and approximate
    computation of the prediction step.
*/
class PL_DLL_API plBayesianFilter :public plBuiltinModel
{
  SPL_ACCESS2(plBayesianFilter);
  
public:

  /** Default constructor used for serialization. */
  plBayesianFilter();

  /** Construction of a filter: 
      \f$ P(S_{t-1} S_t O_t ~|~ A_t) = P(S_{t-1}) P(S_t ~|~ S_{t-1} A_t) P(O_t ~|~ S_t A_t) \f$.
      
      @param[in] state_initial_distribution state initial distribution \f$ P(S_0)\f$
      @param[in] observation_model_distribution observation model \f$ P( O_t ~|~ S_t A_t)\f$
      @param[in] prediction_model prediction (dynamic) model \f$ P(S_t ~|~ S_{t-1} A_t) \f$ (the default value corresponds to identity)
      @param[in] action_variables action (control) variables (the default value corresponds to empty action variables)
  */
  plBayesianFilter(const plDistribution &state_initial_distribution, 
		   const plCndDistribution &observation_model_distribution,
		   const plCndDistribution &prediction_model = plCndDistribution(),
		   const plVariablesConjunction &action_variables = plVariablesConjunction::empty());
                   
  /** Copy constructor */  
  plBayesianFilter(const plBayesianFilter& other);

  /** Assignment operator */
  plBayesianFilter& operator=(const plBayesianFilter& other);

  /** Same as operator=() */
  plBayesianFilter& assign_from(const plBayesianFilter& other)
  {
    return operator=(other);
  }

  /** Destructor */
  ~plBayesianFilter();
    
  /** Apply new command (action) */
  void set_action(const plValues &act);

  /** Get back the last applied command. */
  plValues get_action() const;
    
  // Prediction
  /** Exact prediction step \f$ P(S_t ~|~A_t=a) \f$ (do nothing if no dynamic model) */
  void prediction();

  /** Prediction step \f$ P(S_t ~|~A_t=a) \f$ using 'nsamples' sample points (do nothing if no dynamic model) */
  void n_prediction(unsigned long int nsamples);
    
  /** Prediction step \f$ P(S_t ~|~A_t=a) \f$  using for 'time_in_sec' seconds (do nothing if no dynamic model) */
  void time_prediction(double time_in_sec);
    
    
  // Observation
  /** Incorporate new observation and do the update step \f$ P(S_t | O_t = o A_t = a) \f$ (or \f$ P(S_t | O_t = o) \f$ if no 
      action variables) */
  void observation(const plValues &o);
    

  /** Returns the current estimated distribution \f$ P(S_t) \f$ */
  plDistribution get_estimation()const
  {
    plDistribution PSestim = plDistribution(PoldS_); 
    PSestim.rename(state_variables_);
    return PSestim;
  }
  
  /** Returns the prediction (dynamic, system) model */
  const plCndDistribution &get_prediction_model()const{return PS__A_;}
  
  /** Returns the observation (update) model */
  const plCndDistribution &get_observation_model()const{return PO__SA_;}

protected:
  void init(const plVariablesConjunction &state_vars,
	    const plVariablesConjunction &old_state_vars,
	    const plVariablesConjunction &obs_vars,
	    const plVariablesConjunction &action_vars);

  virtual void Output(std::ostream &out) const;
              
protected:

  // P(S_)
  plMutableDistribution PoldS_;
  
  // Observation model P(O | S A)
  plCndDistribution PO__SA_;
  // P(O | S A) at kernel
  kplComputableObject *PO__SAatKenel_;
  
  // System model P(S | S_ A)
  plCndDistribution PS__oldSA_;
  
  // prediction P(S | A)
  plCndDistribution PS__A_;
  // instantiate prediction P(S | A = a)
  plDistribution PS__a_;
  // P(S | A = a) compiled as map 
  plDistribution PS__a_map_;
  
  kplProbMap *PS__a_map_at_kernel_;
  
  
  // a posteriori P(S | A = a  O = o)
  plDistribution PS_ao_;
  kplProbMap *PS__ao_map_;
  
  
  // P(O = o | S A = a) compiled as a map
  //kplProbMap *Po__Sa_map_; 
  
  plVariablesConjunction state_variables_;
  plVariablesConjunction old_state_variables_;
  kplVariableList action_variables_at_kernel_;
  kplVariableList observation_variables_at_kernel_;
  //kplVariableList _action_obs_vars_;
  
  plDataValues action_;
  plDataValues obs_;
  plDataValues action_obs_values_;
  
  bool has_action_;
  bool system_model_is_id_;

private:
  kplProbMap* get_PS__a_kernel_prob_map()const;
};

#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
