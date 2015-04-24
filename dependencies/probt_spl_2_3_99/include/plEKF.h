/*=============================================================================
 * Product        : ProBT 
 * File           : plEKF.h
 * Author         : Kamel Mekhnacha
 * Creation       : Wed Mar 27 11:41:27 2002
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

#ifndef plEKF_h
#define plEKF_h

#include "plConfig.h"

#include <plKalmanFilter.h>


/** This class implements the Extended Kalman Filter.

    The system is defined by the two equations:

    - System (prediction)  non-linear equation:
    \f[ x_k = f( x_{k-1}, w_{k-1} ) \f]
    with:
    \f[ p(w) = {\cal N}(0, Q) \f]
    
    
    - Observation (measurement)  non-linear equation:
    \f[ z_k = h( x_{k}, v_{k} ) \f]
    with:
    \f[ p(v) = {\cal N}(0, R) \f]
	
*/
class PL_DLL_API plEKF :public plKalmanFilter
{
 public:
  /** Constructor.

      @param[in] state_dim state dimension
      @param[in] obs_dim observation dimension
  */
  plEKF(unsigned int state_dim, unsigned int obs_dim);
  
  /** Construction and initialization.
      @param[in] init_estimation_mean initial estimation mean vector (x0)
      @param[in] init_estimation_cov_matrix initial estimation covariance matrix (P0)
      @param[in] prediction_noise_cov_matrix prediction noise covariance matrix (Q)
      @param[in] observation_noise_cov_matrix observation noise covariance matrix (R)
  */
  plEKF(const plFloatVector &init_estimation_mean, const plFloatMatrix &init_estimation_cov_matrix,
	const plFloatMatrix &prediction_noise_cov_matrix,
	const plFloatMatrix &observation_noise_cov_matrix);
  
  /** Destructor */
  virtual ~plEKF(){};
  
  /** Prediction step.

      \f[ \hat x^{-}_k = f(\hat x_{k-1}, 0) \f]
  
      \f[ P^{-}_k = A_k P_{k-1} A_k^T + W_k Q W_k^T\f]
  */
  virtual void prediction();
  
  using plKalmanFilter::observation;

  /** Observation step.

      \f[ K_k = P^{-}_k H_k^T (H_k P^{-}_k H_k^T + V_k Ro V_k^T)^{-1} \f]
  
      \f[ \hat x_k = \hat x^{-}_k + K_k (zo - h( \hat x^{-}_k, 0 ) ) \f]
      
      \f[ P_k = (I - K_k H_k) P^{-}_k \f]
  */
  virtual void observation(const plFloatVector &zo, const plFloatMatrix &Ro, plFloat observation_weight = PL_ONE);
  
  using plKalmanFilter::get_observation_mahalanobis_distance;
  
  virtual plFloat get_observation_mahalanobis_distance(const plFloatVector &zo, const plFloatMatrix &Ro)const;

  virtual plFloat get_observation_mahalanobis_distance(const plFloatVector &zo, const plFloatMatrix &Ro, plFloat &det)const;
  
  using plKalmanFilter::get_pdf_value;
 
  virtual plProbValue get_pdf_value(const plFloatVector &zo, const plFloatMatrix &Ro)const;

 protected:

  /** Return the predicted state.
      
      \param[out] predicted_state The vector to be filled with the predicted mean. 
      Its size is already set to state dimension.

      \f[ predicted\_state = f( x\_current\_estimation, 0) \f]
  */
  virtual void prediction_model(plFloatVector &predicted_state)const=0;
  
  /** Return the expected observation.

      \param[out] expected_observation The vector to be filled with the expected observation. 
      Its size is already set to observation dimension.

      \f[ expected\_observation = h( x\_current\_estimation, 0) \f] 
  */
  virtual void observation_model(plFloatVector &expected_observation)const=0;
  
  /** Return the partial derivative (Jacobian) \f$ A_k \f$ of the system (prediction) function 
      @em f with respect to \f$ x_{k-1} \f$ evaluated using the current estimate 
      @em x_current_estimation.

      \param[out] A_k The Jacobian square matrix to be filled. 
      It has already the right dimension (state dimension x state dimension)
	  
      \f[  A_k = \frac {\partial f(.)}{\partial x_{k-1} }(x\_current\_estimation) \f]
  */
  virtual void jacob_prediction_model(plFloatMatrix &A_k)const=0;
  
  /** Return the partial derivative \f$ W_k \f$ of the system (prediction) function 
      @em f with respect to \f$ w_{k-1} \f$ evaluated using the current estimate 
      @em x_current_estimation.

      \param[out] W_k The Jacobian square matrix to be filled. 
      It has already the right dimension (state dimension x state dimension)
	  
      \f[  W_k = \frac {\partial f(.)}{\partial w_{k-1} } (x\_current\_estimation) \f]
	 
      The default implementation returns an identity matrix (assuming that w is
      an additive noise). Do not reimplement this function unless you know
      what you are doing.
  */
  virtual void jacob_prediction_noise(plFloatMatrix &W_k)const;
  
  /** Return the partial derivative \f$ H_k \f$ of the observation (measurement) 
      function @em h with respect to \f$ x_{k} \f$ evaluated using the current estimate 
      @em x_current_estimation.

      \param[out] H_k The Jacobian matrix to be filled. 
      It has already the right dimension (observation dimension x state dimension)
	  
      \f[  H_k = \frac {\partial h(.)}{\partial x_{k} } (x\_current\_estimation) \f]
  */
  virtual void jacob_observation_model(plFloatMatrix &H_k)const=0;
  
  /** Return the partial derivative \f$ V_k \f$ of the observation (measurements) 
      function @em h with respect to \f$ v_{k} \f$ evaluated using the current estimate 
      @em x_current_estimation.

      \param[out] V_k The Jacobian matrix to be filled. 
      It has already the right dimension (observation dimension x observation dimension)
	  
      \f[ V_k = \frac {\partial h(.)}{\partial v_{k} } (x\_current\_estimation) \f]
	 
      The default implementation returns an identity matrix (assuming that v is
      an additive noise). Do not reimplement this function unless you know what
      you are doing.
  */
  virtual void jacob_observation_noise(plFloatMatrix &V_k)const;

 private:
  void compute_observation_parameters(const plFloatMatrix &Ro) const;
  
 protected:
  mutable bool reimplemented_jacob_observation_noise_;
  mutable bool reimplemented_jacob_prediction_noise_;

  mutable plFloatMatrix temp_mat_n_n_3_;
  mutable plFloatMatrix temp_mat_n_n_4_;


};






#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
