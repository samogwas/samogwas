/*=============================================================================
 * Product        : ProBT 
 * File           : plUKF.h
 * Author         : Laurent Saroul
 * Creation       : Fri Dec 18 10:00:27 2014
 *
 *=============================================================================
 *        (c) Copyright 2015, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */


#ifndef plUKF_h
#define plUKF_h


#include "plConfig.h"

#include <vector>

#include <plKalmanFilter.h>


/** This class implements the Unscented Kalman Filter (first version: needs to be tested).

    The system is defined by the two equations:

    - System (prediction)  non-linear equation:
    \f[ x_k = f( x_{k-1} ) + w_{k-1}\f]
    with:
    \f[ p(w) = {\cal N}(0, Q) \f]
    
    
    - Observation (measurement)  non-linear equation:
    \f[ z_k = h( x_{k}) + v_{k}  \f]
    with:
    \f[ p(v) = {\cal N}(0, R) \f]
	
*/
class PL_DLL_API plUKF :public plKalmanFilter
{
 public:
  /** Constructor.
      @param[in] state_dim state dimension
      @param[in] obs_dim observation dimension
      @param[in] alpha ukf parameter
      @param[in] beta ukf parameter
      @param[in] k ukf parameter
  */
  plUKF(size_t state_dim, size_t obs_dim,plFloat alpha=0.001, plFloat beta=2.0, plFloat k=0.0);
  
  /** Construction and initialization.
      @param[in] state_dim state dimension
      @param[in] obs_dim observation dimension
      @param[in] init_estimation_mean initial estimation mean vector (x0)
      @param[in] init_estimation_cov_matrix initial estimation covariance matrix (P0)
      @param[in] prediction_noise_cov_matrix prediction noise covariance matrix (Q)
      @param[in] observation_noise_cov_matrix observation noise covariance matrix (R)
      @param[in] augmented_process_noise add the process noise in the augmented state (false by default)
      @param[in] augmented_observation_noise add the observation noise in the augmented state (false by default)
      @param[in] alpha ukf parameter
      @param[in] beta ukf parameter
      @param[in] k ukf parameter
  */
  plUKF(size_t state_dim, size_t obs_dim,
	const plFloatVector &init_estimation_mean, 
	const plFloatMatrix &init_estimation_cov_matrix,
	const plFloatMatrix &prediction_noise_cov_matrix,
	const plFloatMatrix &observation_noise_cov_matrix,
	bool augmented_process_noise=false,
	bool augmented_observation_noise=false,
	plFloat alpha=0.001, plFloat beta=2.0, plFloat k=0.0);
  
  /** Destructor */
  virtual ~plUKF(){}
  
  /** Prediction step.
   */
  virtual void prediction();
  
  using plKalmanFilter::observation;

  /** Observation step.
   */
  virtual void observation(const plFloatVector &zo, const plFloatMatrix &Ro, plFloat observation_weight = PL_ONE);
  
  using plKalmanFilter::get_observation_mahalanobis_distance;
  
  virtual plFloat get_observation_mahalanobis_distance(const plFloatVector &zo, const plFloatMatrix &Ro)const;

  virtual plFloat get_observation_mahalanobis_distance(const plFloatVector &zo, const plFloatMatrix &Ro, plFloat &det)const;
  
  using plKalmanFilter::get_pdf_value;
 
  virtual plProbValue get_pdf_value(const plFloatVector &zo, const plFloatMatrix &Ro)const;


 protected:
  /** Return the predicted state.
      \param[in] state The initial state vector.
      Its size is already set to state dimension.

      \param[out] predicted_state The vector to be filled with the predicted state. 
      Its size is already set to state dimension.

      \f[ predicted\_state = f( state, 0) \f]
  */
  virtual void prediction_model(const plFloatVector &state,plFloatVector &predicted_state) = 0;
  
  /** Return the expected observation.
      \param[in] state The current state vector.
      Its size is already set to state dimension.
      
      \param[out] expected_observation The vector to be filled with the expected observation. 
      Its size is already set to observation dimension.

      \f[ expected\_observation = h( state, 0) \f] 
  */
 
  virtual void observation_model(const plFloatVector &state, plFloatVector &expected_observation)const=0;
  
  
  void compute_observation_parameters(const plFloatMatrix &Ro) const;

  void observation_internal_ukf(const plFloatVector &obs, const plFloatVector &obs_exp, const plFloatMatrix &Ro, plFloat observation_weight);
  
  virtual void  compute_initial_sigma_points();

  void compute_mean(const std::vector<plFloatVector>& points,plFloat W0, plFloat Wi,plFloatVector& mean )const;

  void compute_variance(const plFloatVector& mean, const std::vector<plFloatVector>& points, plFloat W0c, plFloat Wi,plFloatMatrix& covariance_mat )const;

  void compute_crossvariance(const plFloatVector& mean1, const plFloatVector& mean2,const std::vector<plFloatVector>& points1,
                             const std::vector<plFloatVector>& points2, plFloat W0c, plFloat Wi, plFloatMatrix& covariance_mat )const ;

  /** may be used for post process the initial sigma points,
      for instance by adding constraints on the state values.
      For now, do nothing
  */
  virtual void post_process_initial_sigma_points(){}
  
  /** may be used for post process the predicted sigma points,
      for instance by adding constraints on the state values.
      For now, do nothing
  */
  virtual void post_process_predicted_sigma_points(){}

  /** may be used for post process the predicted sigma points,
      for instance by adding constraints on the state values.
      For now, do nothing
  */
  virtual void post_process_predicted_state(){}

  /** may be used for post process the current estimate,
      for instance by adding constraints on the state values.
      For now, do nothing
  */
  virtual void post_process_current_estimate(){}


 protected:

  plFloat m_alpha;
  plFloat m_beta;
  plFloat m_k;
  plFloat m_lambda;

  bool m_augmented_process_noise;
  bool m_augmented_observation_noise;

  size_t m_augmented_state_dim;

  mutable std::vector<plFloatVector> m_X_sigma_points;
  mutable std::vector<plFloatVector> m_Y_sigma_points;
  mutable std::vector<plFloatVector> m_Z_sigma_points;

  mutable plFloat m_W0s,m_W0c,m_Wi;

  /** Current estimate augmented covariance matrix */
  plFloatMatrix m_P_aug;
};


#endif // plUKF_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
