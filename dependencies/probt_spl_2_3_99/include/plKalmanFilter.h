/*=============================================================================
 * Product        : ProBT 
 * File           : plKalmanFilter.h
 * Author         : Christophe Coue
 * Creation       : Tue Mar 27 2001
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

#ifndef LINEAR_KALMAN_FILTER_H
#define LINEAR_KALMAN_FILTER_H

#include "plConfig.h"

#include <plObject.h>
#include <plMath.h>
#include <plFloatVector.h>
#include <plFloatMatrix.h>

SPL_DECL2(plKalmanFilter);

/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/

/** This class implements the Linear Kalman Filter.

    The system is defined by the two equations:

    System (prediction) linear equation:
    \f[ x_k = A x_{k-1} + B u_k + w_{k-1} \f]
    with:
    \f[ p(w) = {\cal N}(0, Q) \f]
    
    
    Observation (measurement) linear equation:
    \f[ z_k = H x_{k} + v_{k} \f]
    with:
    \f[ p(v) = {\cal N}(0, R) \f]

*/
class PL_DLL_API plKalmanFilter : public plBuiltinModel
{
  SPL_ACCESS2(plKalmanFilter);
 public:


  /** Default constructor used for serialization. */
  plKalmanFilter();

  /** Constructor.

      @param[in] state_dim state dimension
      @param[in] obs_dim observation dimension
      @param[in] command_dim command dimension 
  */
  plKalmanFilter(unsigned int state_dim, unsigned int obs_dim,  unsigned int command_dim=0);

  /** Construction and initialization.
      @param[in] init_estimation_mean initial estimation mean vector (x0)
      @param[in] init_estimation_cov_matrix initial estimation covariance matrix (P0)
      @param[in] prediction_matrix prediction (system) matrix (A)
      @param[in] prediction_noise_cov_matrix prediction noise covariance matrix (Q)
      @param[in] observation_matrix observation (measurement) matrix (H)
      @param[in] observation_noise_cov_matrix observation noise covariance matrix (R)
      @param[in] control_matrix command (control) matrix (B)
  */
  plKalmanFilter(const plFloatVector &init_estimation_mean, const plFloatMatrix &init_estimation_cov_matrix,
		 const plFloatMatrix &prediction_matrix,  const plFloatMatrix &prediction_noise_cov_matrix,
		 const plFloatMatrix &observation_matrix, const plFloatMatrix &observation_noise_cov_matrix,
		 const plFloatMatrix &control_matrix = plFloatMatrix());
		 
  /** Get the state dimension */
  inline unsigned int get_state_dim() const
  {
    return x_current_estimation.size();
  }

  /** Get the observation dimension */
  inline unsigned int get_observation_dim() const
  {
    return z.size();
  }

  /** Get the command dimension */
  inline unsigned int get_command_dim() const
  {
    return u.size();
  }

  /** Destructor */
  virtual ~plKalmanFilter() {};
  
  /** Set the prediction matrix A */
  inline void set_prediction_matrix(const plFloatMatrix& Ain)
  { 
    assert(Ain.rows() == A.rows());
    assert(Ain.cols() == A.cols());

    A = Ain;
  }
  
  /** Set system noise covariance matrix Q */
  inline void set_prediction_noise_cov_matrix(const plFloatMatrix& QS) { Q = QS; }
  
  /** Set observation matrix H */
  inline void set_observation_matrix(const plFloatMatrix& Hin)  
  { 
    assert(Hin.rows() == H.rows());
    assert(Hin.cols() == H.cols());

    H = Hin;
  } 
  
  
  /** Set observation noise covariance matrix R */
  inline void set_observation_noise_cov_matrix(const plFloatMatrix& Ro) 
  {
    assert(Ro.rows() == R.rows());
    assert(Ro.cols() == R.cols());

    R = Ro; 
  }
  
  /** Set control matrix B */
  inline void set_control_matrix(const plFloatMatrix& Bin) 
  {
    assert(Bin.rows() == B.rows());
    assert(Bin.cols() == B.cols());

    B = Bin; 
  }

  /** Set the current command u */
  inline void set_control(const plFloatVector& Uin)
  {
    assert(Uin.size() == B.cols());
    u = Uin; 
  }
  
  /** Set the initial estimation x0 ans P0*/
  inline void set_initial_estimation(const plFloatVector& Xo, const plFloatMatrix& Po)
  {
    x_current_estimation = Xo;
    P_current_estimation = Po;
  }
  
  /** Set the current observation z */
  inline void set_observation(const plFloatVector& Zin, plFloat observation_weight_in = PL_ONE)
  { 
    z = Zin; 
    observation_weight_ = observation_weight_in;
  }
  
  /** Prediction step.

      \f[ \hat x^{-}_k = A \hat x_{k-1} + B u_k \f]
  
      \f[ P^{-}_k = A P_{k-1} A^T + Q \f]
	
  */
  virtual void prediction();
  
  /** Update step assuming that the observation has been inserted using set_observation() function.

      \f[ K_k = P^{-}_k H^T (H P^{-}_k H^T + R)^{-1} \f]
      
      \f[ \hat x_k = \hat x^{-}_k + K_k (z_k - H_k \hat x^{-}_k ) \f]
      
      \f[ P_k = (I - K_k H) P^{-}_k \f]
  */
  void update();
  
  /**  Update step using observation vector @em zo.
       
       Equivalent to calling (without changing @em z internally however):
       - set_observation(zo, observation_weight)
       - update()
  */
  void observation(const plFloatVector &zo, plFloat observation_weight = PL_ONE);
  
  /**  
       Update step using observation vector @em zo and its corresponding covariance matrix @em Ro.
       
       Equivalent to calling (without changing @em z and @em R internally however):
       - set_observation_noise_cov_matrix(Ro)
       - set_observation(zo, observation_weight)
       - update() 
  */
  virtual void observation(const plFloatVector &zo, const plFloatMatrix &Ro, plFloat observation_weight = PL_ONE);
  
  /** Prediction and update step using the observation vector @em zo.

      It performs a prediction step followed by an observation (update) step.
  */
  inline void prediction_observation(const plFloatVector& zo, plFloat observation_weight = PL_ONE )
  {
    prediction();
    observation(zo, observation_weight);
  }
  
  /** Prediction and update step using the observation vector @em zo and its corresponding covariance matrix @em Ro.

      It performs a prediction step followed by an observation (update) step.
  */
  inline void prediction_observation(const plFloatVector& zo, const plFloatMatrix &Ro, 
				     plFloat observation_weight = PL_ONE )
  {
    prediction();
    observation(zo, Ro, observation_weight);
  }
  
  /** Return the current estimation mean vector. */
  const plFloatVector& get_estimation_mean()const {return x_current_estimation;}
  
  /** Return the current estimation covariance matrix. */
  const plFloatMatrix& get_estimation_covariance()const {return P_current_estimation;}
  
  /** Return the Mahalanobis distance of a given observation.*/
  plFloat get_observation_mahalanobis_distance(const plFloatVector &zo)const;

  /** Return the Mahalanobis distance of a given observation and the determinant of the convariance matrix */
  plFloat get_observation_mahalanobis_distance(const plFloatVector &zo, plFloat &det)const;
  
  /** Return the Mahalanobis distance of a given observation.*/
  virtual plFloat get_observation_mahalanobis_distance(const plFloatVector &zo, const plFloatMatrix &Ro)const;

  /** Return the Mahalanobis distance of a given observation and the determinant of the convariance matrix.*/
  virtual plFloat get_observation_mahalanobis_distance(const plFloatVector &zo, const plFloatMatrix &Ro, plFloat &det)const;

  /** Return, for the observation @em zo, the value of the density function
      value corresponding to the current gaussian estimate. It assumes that the variance
      matrix associated to the observation is the default @em R.
  */
  plProbValue get_pdf_value(const plFloatVector &zo)const;
  
  /** Return, for the observation @em zo and the corresponding variance
      matrix @em Ro, the value of the density function value according to
      the current gaussian estimate.
  */
  virtual plProbValue get_pdf_value(const plFloatVector &zo, const plFloatMatrix &Ro)const;

  /** Set/unset matrix inversion method using eigen decomposition.
      If set to 'false' (the default), Greville's algorithm will be used.

      Inversion using eigen decomposition is more robust than using Greville's
      algorithm (the default). Greville's algorithm is slightly faster.

      \sa set_greville_inversion_threshold()
  */
  void use_eigen_decomposition_matrix_inversion(bool eigen_decomposition_inversion)
  {
    eigen_decomposition_inversion_ = eigen_decomposition_inversion;
  }

  /** Set/change Greville's inversion threshold. This parameter is used only
      when eigen_decomposition_inversion is set to 'false'. The default value is
      std::numeric_limits<plFloat>::min().

      \sa use_eigen_decomposition_matrix_inversion()
      \sa plFloatMatrix::inverse_using_greville()
  */
  void set_greville_inversion_threshold(plFloat threshold)
  {
    greville_inversion_threshold_ = threshold;
  }
  
 public:

  /** System (prediction) matrix.
      
      Store the system (prediction) model Jacobian for the non-linear case.
  */
  plFloatMatrix A;
  
  // mutable only for EKF ?
  /** Observation (measurement) matrix.
      
      Store the observation (measurement) model Jacobian for the non-linear case.
  */
  mutable plFloatMatrix H;

  /** Control (command) matrix */
  plFloatMatrix B;
  
  /** Current observation */
  plFloatVector z;

  /** Current command */
  plFloatVector u;

  /** System (prediction) noise covariance matrix */
  plFloatMatrix Q;
  
  /** Observation (measurement) noise covariance matrix */
  plFloatMatrix R;
 
  /** Current estimate mean vector */
  plFloatVector x_current_estimation;

  /** Current estimate covariance matrix */
  plFloatMatrix P_current_estimation;
  
 protected:
  void observation_internal(const plFloatVector &obs, const plFloatVector &obs_exp, const plFloatMatrix &Ro, plFloat observation_weight);
  plFloat get_observation_mahalanobis_distance_internal(const plFloatVector &Zo, const plFloatVector &obs_exp, const plFloatMatrix &Ro,
                                                        plFloat &det)const;

  plProbValue get_pdf_value_internal(const plFloatVector &Zo, const plFloatVector &obs_exp, const plFloatMatrix &Ro)const;
 
 protected:
  virtual void Output(std::ostream &out) const;

 protected:
 
  plFloat observation_weight_;
  mutable plFloatMatrix temp_mat_n_n_;
  mutable plFloatMatrix temp_mat_n_n_2_;
  mutable plFloatMatrix temp_mat_m_m_;
  mutable plFloatMatrix temp_mat_m_m_2_;
  mutable plFloatMatrix temp_mat_m_m_3_;
  mutable plFloatMatrix temp_mat_n_m_;
  mutable plFloatMatrix temp_mat_n_m_2_;
  mutable plFloatVector temp_vect_n_;
  mutable plFloatVector temp_vect_m_;
  mutable plFloatVector temp_vect_m_2_;
  mutable plFloatVector temp_vect_m_3_;
  bool eigen_decomposition_inversion_;
  plFloat greville_inversion_threshold_;
};

#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
