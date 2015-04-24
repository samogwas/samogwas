/*=============================================================================
 * Product        : ProBT 
 * File           : plSIRParticleFilter.h
 * Author         : Kamel Mekhnacha
 * Creation       : Tue Mar  3 17:13:35 2009
 *
 *=============================================================================
 *        (c) Copyright 2008, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */

#ifndef plSIRParticleFilter_h
#define plSIRParticleFilter_h

#include <plConfig.h>
#include <plParticleFilter.h>
/**
   This class implements the Sampling Importance Resampling (SIR) particle
   filtering algorithm.  The SIR algorithm can be easily derived from the
   generic particle filtering algorithm \em plParticleFilter by an appropriate
   choice of i) the proposal (importance) distribution, where is chosen to be
   the prior transition distribution, 
   @f$ \pi(x_k~|~x_{0:k-1}, z_{0:k}) = p(x_k~|~x_{k-1}) @f$,
   and ii) the resampling step, which is to be
   applied at every time index.

   The term Sequential Importance Resampling is also sometimes used when
   referring to SIR filters.
*/
class PL_DLL_API plSIRParticleFilter :public plParticleFilter
{
 private:
  // Forbid using another proposal distribution
  bool  prediction_observation(const plValues &obs,
                               const plCndDistribution &proposal_distribution);
  template <typename T>
    bool  prediction_observation(T *obs,
                                 const plCndDistribution &proposal_distribution);
  
  template <typename T>
    bool  prediction_observation(const std::vector<T> &obs,
                                 const plCndDistribution &proposal_distribution);
 public:

  using plParticleFilter::prediction_observation;
  /** Constructor.
      @param[in] observation_model the @f$ p(z_k~|~x_k) @f$ distribution
      @param[in] transition_model the @f$ p(x_k~|~x_{k-1}) @f$ distribution (empty plCndDistribution for identity transition)
  */
  explicit plSIRParticleFilter(const plCndDistribution &observation_model,
                               const plCndDistribution &transition_model = plCndDistribution())
    : plParticleFilter(observation_model, transition_model, -1.0)
  {}

  /** Default constructor used for serialization */
  plSIRParticleFilter()
    : plParticleFilter()
    {}
   
 protected:

  /** Resampling criterion: returns \em true to require resampling at every time index */
  virtual bool needs_resampling()const
  {
    return true;
  }
  
};

#endif // plSIRParticleFilter_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
