/*=============================================================================
 * Product        : ProBT 
 * File           : plParticleFilter.h
 * Author         : Kamel Mekhnacha
 * Creation       : Wed Mar 21 15:17:04 2008
 *
 *=============================================================================
 *        (c) Copyright 2008, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */
#ifndef plParticleFilter_h
#define plParticleFilter_h

#include <plConfig.h>

#include <plObject.h>
#include <plMath.h>
#include <plCndDistribution.h>
#include <plDistribution.h>

#include <vector>


SPL_DECL2(plParticleFilter);

class kplConditionalKernel;

class plNonCndLearnObject;

/**
   This class implements a generic particle filtering algorithm.

   It approximates the filtering a posteriori
   distribution 
   @f[ p(x_k~|~z_0, \cdots, z_k), @f] 
   by a weighted set of particles
   @f[ \left\{ \left(w_k^{(L)}, x_k^{(L)}  \right) : L = 1,\cdots, P  \right\}. @f]
   The importance weights @f$ w_k^{(L)} @f$ are approximations to the relative
   posterior probabilities (or densities) of the particles such that 
   @f$ \sum_{L=1}^{P} w_k^{(L)} = 1 @f$.  Particle filtering is
   a sequential (i.e., recursive) version of importance sampling.

   For a finite set of particles, the algorithm performance is dependent on the
   choice of the proposal distribution 
   @f[ \pi(x_k~|~x_{0:k-1}, z_{0:k}). @f]
   
   The optimal proposal distribution is given as the target distribution 
   @f[\pi(x_k~|~x_{0:k-1}, z_{0:k}) = p(x_k~|~x_{k-1}, z_k). @f]
   
   However, the transition prior is often used as importance function, since it
   is easier to draw particles (or samples) and perform subsequent importance
   weight calculations:
   @f[\pi(x_k~|~x_{0:k-1}, z_{0:k}) = p(x_k~|~x_{k-1}). @f]

   Particle filters with transition prior as importance (proposal) function are
   commonly known as bootstrap filter and condensation algorithm. This is the
   default proposal distribution used in this implementation. However the user
   can provide another proposal distribution by calling the 2 parameters @em
   prediction_observation method.

   Resampling is used to avoid the problem of degeneracy of the
   algorithm, that is, avoiding the situation that all but one of the
   importance weights are close to zero. The performance of the algorithm can
   be also affected by proper choice of resampling method. The stratified
   resampling proposed by Kitagawa (1996) is optimal in terms of variance.
   
   After initialization, a single step of the Sampling Importance Resampling algorithm is as follows: 

   1) For @f$ L = 1,\cdots, P @f$ draw samples from the proposal distribution:
   @f[ x_k^{(L)} \sim \pi(x_k~|~x_{0:k-1}^{(L)}, z_{0:k}). @f]

   2) For @f$ L = 1,\cdots, P @f$ update the importance weights up to a normalizing constant:
   @f[ \hat w_k^{(L)} = w_{k-1}^{(L)} 
   \frac 
   {p(x_k^{(L)}~|~x_{k-1}^{(L)})~p(z_k~|~x_k^{(L)})} 
   {\pi(x_k^{(L)}~|~x_{0:k-1}^{(L)}, z_{0:k})}. @f]
   
   3) For @f$ L = 1,\cdots, P @f$ compute the normalized importance weights:
   @f[ w_k^{(L)} = 
   \frac 
   {\hat w_k^{(L)}}
   {\sum_{J=1}^P \hat w_k^{(J)}}.
   @f]
   
   4) Compute an estimate of the effective number of particles as:
   @f[ \hat N_{eff} = \frac 1 {\sum_{L=1}^P \left( w_k^{(L)} \right)^2}. @f]

   5) If the effective number of particles is less than a given threshold @f$ \hat N_{eff} < N_{thr} @f$, then perform resampling:

   - Draw @f$ P @f$ particles from the current particle set with probabilities proportional to their weights. 
   Replace the current particle set with this new one.
   - For  @f$ L = 1,\cdots, P @f$ set @f$ w_k^{(L)} = 1/P @f$.
   
   Steps 4 and 5 defines the default resampling strategy implemented int this
   class. It corresponds to the stratified resampling algorithm proposed by
   Kitagawa (1996). This resampling criterion may be cutomized by reimplementing
   the plParticleFilter::needs_resampling() method.
*/
class PL_DLL_API plParticleFilter :public plBuiltinModel
{
 private:
  // Particle (state) @f$ x_k @f$ type 
  typedef plDataValues particle_t;

 private:
  bool observation(const plDataValues &obs);
  bool prediction_observation_internal(const plCndDistribution &proposal_distribution);
  const plParticleFilter::particle_t &get_best_particle(plProbValue &weight)const;
  bool finalize_observation(plProbValue norm);  
  static kplConditionalKernel *get_root_from(const plCndDistribution &distrib);
  static kplKernel *get_root_from(const plDistribution &distrib);

public:
  
  /** Constructor.
      @param[in] observation_model the @f$ p(z_k~|~x_k) @f$ distribution
      @param[in] transition_model the @f$ p(x_k~|~x_{k-1}) @f$ distribution (empty plCndDistribution for identity transition)
      @param[in] Neff_resampling_threshold the @f$ N_{thr} @f$ threshold
  */
  explicit plParticleFilter(const plCndDistribution &observation_model,
                            const plCndDistribution &transition_model = plCndDistribution(),
                            plFloat Neff_resampling_threshold = 1000.0);

  /** Default constructor used for serialization */
  plParticleFilter();
  
  /** Copy constructor */
  plParticleFilter(const plParticleFilter  &other);
  
  /** Assignement operator */
  plParticleFilter& operator=(const plParticleFilter &other);

  /** Same as operator=() */
  plParticleFilter& assign_from(const plParticleFilter& other)
  {
    return operator=(other);
  }

  /** Particle set initialization.
      @param[in] init_distribution the distribution from which the initial particle set is drawn
      @param[in] nparticles the size of the particle set
  */
  void initialize(const plDistribution &init_distribution,
                  unsigned int nparticles);

  /** Prediction step.
      Apply the transition model @f$ p(x_k~|~x_{k-1}) @f$ to all particles.
  */
  void prediction();

  /** Observation step assuming that the proposal distribution 
      @f$ \pi(x_k~|~x_{0:k-1}, z_{0:k}) @f$ is the transition distribution @f$ p(x_k~|~x_{k-1}) @f$.
      
      @param[in] obs the observation @f$ z_k @f$
      @return false if the filter is degenerated (all weights are null).
  */
  bool observation(const plValues &obs);
  
  /** Observation step assuming that the proposal distribution 
      @f$ \pi(x_k~|~x_{0:k-1}, z_{0:k}) @f$ is the transition distribution @f$ p(x_k~|~x_{k-1}) @f$.

      @param[in] obs the observation @f$ z_k @f$
      @return false if the filter is degenerated (all weights are null).
  */
  template <typename T>
    bool observation(T *obs);

  /** Observation step assuming that the proposal distribution 
      @f$ \pi(x_k~|~x_{0:k-1}, z_{0:k}) @f$ is the transition distribution @f$ p(x_k~|~x_{k-1}) @f$.
      
      @param[in] obs the observation @f$ z_k @f$
      @return false if the filter is degenerated (all weights are null).
  */
  template <typename T>
    bool observation(const std::vector<T> &obs);


  /** Prediction/Observation step assuming that the proposal distribution 
      @f$ \pi(x_k~|~x_{0:k-1}, z_{0:k}) @f$ is the transition distribution @f$ p(x_k~|~x_{k-1}) @f$.

      @param[in] obs the observation @f$ z_k @f$

      @return false if the filter is degenerated (all weights are null).
  */
  bool prediction_observation(const plValues &obs)
  {
    prediction();
    return observation(obs);
  }
  /** Prediction/Observation step assuming that the proposal distribution 
      @f$ \pi(x_k~|~x_{0:k-1}, z_{0:k}) @f$ is the transition distribution @f$ p(x_k~|~x_{k-1}) @f$.

      @param[in] obs the observation @f$ z_k @f$

      @return false if the filter is degenerated (all weights are null).
  */
  template <typename T>
    bool prediction_observation(T *obs)
  {
    prediction();
    return observation(obs);
  }

  /** Prediction/Observation step assuming that the proposal distribution 
      @f$ \pi(x_k~|~x_{0:k-1}, z_{0:k}) @f$ is the transition distribution @f$ p(x_k~|~x_{k-1}) @f$.

      @param[in] obs the observation @f$ z_k @f$

      @return false if the filter is degenerated (all weights are null).
  */
  template <typename T>
    bool prediction_observation(const std::vector<T> &obs)
  {
    prediction();
    return observation(obs);
  }

  /** Prediction/Observation step using  @em proposal_distribution
      as the proposal distribution @f$ \pi(x_k~|~x_{0:k-1}, z_{0:k}) @f$.

      @param[in] obs the observation @f$ z_k @f$
      @param[in] proposal_distribution the proposal distribution @f$
      \pi(x_k~|~x_{0:k-1}, z_{0:k}) @f$.  It's supposed to have the signature
      @f$ p(x_k~|~x_{k-1}) @f$ (i.e. having @f$ x_k @f$ as left variables and
      @f$x_{k-1} @f$ as right ones) and depending implicitly of @f$ x_{0:k-2} @f$ and @f$ z_{0:k} @f$.
    
      @return false iff the filter is degenerated (all weights are null).
  */
  bool  prediction_observation(const plValues &obs,
                               const plCndDistribution &proposal_distribution);
  
  
  /** Prediction/Observation step using  @em proposal_distribution
      as the proposal distribution @f$ \pi(x_k~|~x_{0:k-1}, z_{0:k}) @f$.
      
      @param[in] obs the observation @f$ z_k @f$ 
      @param[in] proposal_distribution the proposal distribution @f$
      \pi(x_k~|~x_{0:k-1}, z_{0:k}) @f$.  It's supposed to have the signature
      @f$ p(x_k~|~x_{k-1}) @f$ (i.e. having @f$ x_k @f$ as left variables and
      @f$x_{k-1} @f$ as right ones) and depending implicitly of @f$ x_{0:k-2} @f$ and @f$ z_{0:k} @f$.
      
      @return false iff the filter is degenerated (all weights are null).
  */
  template <typename T>
    bool  prediction_observation(T *obs,
                                 const plCndDistribution &proposal_distribution);
  
  /** Prediction/Observation step using  @em proposal_distribution
      as the proposal distribution @f$ \pi(x_k~|~x_{0:k-1}, z_{0:k}) @f$.
      
      @param[in] obs the observation @f$ z_k @f$
      @param[in] proposal_distribution the proposal distribution @f$
      \pi(x_k~|~x_{0:k-1}, z_{0:k}) @f$.  It's supposed to have the signature
      @f$ p(x_k~|~x_{k-1}) @f$ (i.e. having @f$ x_k @f$ as left variables and
      @f$x_{k-1} @f$ as right ones) and depending implicitly of @f$ x_{0:k-2} @f$ and @f$ z_{0:k} @f$.

      @return false iff the filter is degenerated (all weights are null).
  */
  template <typename T>
    bool  prediction_observation(const std::vector<T> &obs,
                                 const plCndDistribution &proposal_distribution);

  /** Get the posteriori distribution @f$ p(x_k~|~z_0, \cdots, z_k) @f$ by fitting the particle set to the
      parametrical form defined by @em learning_form.

      @param[in] learning_form a learning object representing the parametrical form in which the particle set is fitted
      
      @return the a posteriori distribution @f$ p(x_k~|~z_0, \cdots, z_k) @f$
  */
  plDistribution get_estimation(plNonCndLearnObject &learning_form);

   /** Get the particle having the heighest weight and the corresponding weight.

      @param[out] best_particle the particle having the heighest weight
      @param[out] weight the corresponding weight
  */
    void get_best_particle(plValues &best_particle, plProbValue &weight)const;

  /** Get the particle having the heighest weight and the corresponding weight.

      @param[out] best_particle the particle having the heighest weight
      @param[out] weight the corresponding weight
  */
  template <typename T>
    void get_best_particle(T *best_particle, plProbValue &weight)const;


  /** Get the particle having the heighest weight and the corresponding weight.

      @param[out] best_particle the particle having the heighest weight
      @param[out] weight the corresponding weight
  */
  template <typename T>
    void get_best_particle(std::vector<T> &best_particle, plProbValue &weight)const;

  /** Return the number of particles.

      @return the number of particles
  */
  inline unsigned int get_n_particles()const
  {
    return particles1_.size();
  }

   /** Get the nth particle and its weight.
      
      @param[in] n particle's number
      @param[out] particle particle value
      @param[out] weight the corresponding weight
  */
  void get_particle(unsigned  int n, plValues &particle, plProbValue &weight)const;
  
  /** Get the nth particle and its weight.
      
      @param[in] n particle's number
      @param[out] particle particle value
      @param[out] weight the corresponding weight
  */
  template <typename T>
    void get_particle(unsigned  int n, T *particle, plProbValue &weight)const
  {
    if(n < get_n_particles()){

      const plDataValues &p = (*current_particles_)[n];
      for(unsigned int i = 0; i < state_dim_; ++i){
        particle[i] = p[i];
      }
      weight = weights_[n];
    }
  }

  /** 
      Get the nth particle and its weight.

      @param[in] n particle's number.
      @param[out] particle particle value
      @param[out] weight the corresponding weight
  */
  template <typename T>
    void get_particle(unsigned  int n, std::vector<T> &particle, plProbValue &weight)const
  {
    particle.resize(state_dim_);
    get_particle(n, &particle[0], weight);
  }
  
  
  /** Get the set of weights.
      @return the set of weights
  */
  const std::vector<plProbValue> &get_weights()const
  {
    return weights_;
  }

  /** Print the particles and their weights in an output stream.
      @param[out] out the output stream in which the particles are printed
      @return the output stream
  */
  std::ostream &print_particles(std::ostream& out = std::cout)const;

  /** Plot the particles and their weights in a gnuplot file.
      @param[in]  file_name the output file name
  */
  void plot_particles(const std::string &file_name)const;



 protected:
  /**
     This method implements the resampling criterion and is called at the end
     of each observation to decide whether a resampling is needed or not. The
     default implementation corresponds to the stratified resampling algorithm
     proposed by Kitagawa (1996).
  */
  virtual bool needs_resampling()const;

  /**
     Resampling procedure.
   */
  void resample();

  /** Equivalent to print_particles.
      @param[out] out the output stream in which the particles are printed
  */
  void Output(std::ostream& out)const
  {
    out << "plParticleFilter:" << std::endl
	<< "Observation model: " << PO_given_S_ << std::endl
	<< "Transition (prediction) model: " << PS_given_oldS_  << std::endl
	<< "Current particle set: " << std::endl;
    print_particles(out);
  }

 protected:
  SPL_ACCESS2(plParticleFilter);

  // System (dynamic/transition) model P(S | S_)
  plCndDistribution PS_given_oldS_;
  kplConditionalKernel *PS_given_oldS_at_kernel_;

  
  // Observation model P(O | S)
  plCndDistribution PO_given_S_;
  kplConditionalKernel *PO_given_S_at_kernel_;

 
  std::vector <plProbValue> weights_;

  plFloat Neff_resampling_threshold_;
  bool no_dynamic_model_;
  bool is_null_;

  unsigned int observation_dim_;
  unsigned int state_dim_;
  bool just_resampled_;

  mutable plDataValues tmp_observation_;
  //mutable plDataValues tmp_Sp_observation_;

  std::vector <particle_t> particles1_;
  std::vector <particle_t> particles2_;

  std::vector <particle_t> *current_particles_;
  std::vector <particle_t> *tmp_particles_;

  particle_t best_particle_before_samping_;
  plProbValue best_weight_before_samping_;
};

#endif // plParticleFilter_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
