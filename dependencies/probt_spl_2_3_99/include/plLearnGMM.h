/*=============================================================================
 * Product        : ProBT 
 * File           : plLearnGMM.h
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

#ifndef plLearnGMM_h
#define plLearnGMM_h

#include <vector>
#include <iostream>

#include "plConfig.h"

#include <plLearnMixtureModel.h>

SPL_DECL1(plLearnGMM);

/** This class implements the Gaussian Mixture Model unsupervised learning.
   
    It uses the EM learning algorithm and is based on the plEMLearner class.

    \sa plLearnMixtureModel
*/
class PL_DLL_API plLearnGMM :public plLearnMixtureModel
{
 public:

  /** Enum for initialization methods 
      
      \sa set_initialization_method()
  */
  enum InitializationMethod {
    GMM_RANDOM_INIT, /*!< Random initialization in variable ranges. */
    GMM_KMEANS_FORGY_INIT, /*!< k-means Forgy method. */
    GMM_KMEANS_RANDOM_PARTITION_INIT /*!< k-means random partition method. */
  };

  /** Default constructor */
  plLearnGMM();
  
  /** Constructor 
      \param[in] ObsVars The variables on which the GMM will be learned
      \param[in] n_mixture The number of components (kernels) of the mixture
      \param[in] independent_dimensions If 'true', ObsVars are assumed to be independent (i.e., diagnonal conarianace matrix)  
  */
  plLearnGMM(const plVariablesConjunction &ObsVars, size_t n_mixture, bool independent_dimensions=false);
  
  /** Constructor 
      \param[in] ObsVars The variables on which the GMM will be learned
      
      \param[in] n_mixture_candidates The candidate number of components
      (kernels) of the mixture. Choosing among this candidates will be based on 
      the optimality score (as defined by set_optimality_score() among BIC, AIC, ...)

      \param[in] independent_dimensions If 'true', ObsVars are assumed to be independent (i.e., diagnonal conarianace matrix) 
  */
  plLearnGMM(const plVariablesConjunction &ObsVars,
             const std::vector<size_t> &n_mixture_candidates, 
             bool independent_dimensions=false);

  virtual plLearnObject* clone()const
  {
    return new plLearnGMM(*this);
  }

  /** Set components (clusters) initialization parameters for each dimension.

      \param[in] variable the variable (dimension) for which the parameters are set
      \param[in] min_val The minimal value of the range 
      \param[in] max_val The maximal value of the range 
      \param[in] variance The variance

      These parameters will be used by the default get_initialization_cluster() function 
      \sa get_initialization_cluster()
  */
  void set_initialization_parameters(size_t variable, 
                                     plFloat min_val, 
                                     plFloat max_val, 
                                     plFloat variance)
  {
    range_[variable].first  = min_val;
    range_[variable].second = max_val;
    variance_[variable]     = variance;
  }

  /** Set/unset independent variables hypothesis (i.e., diagonal covariance matrix)
      
      Has no effect in one-dimensional case
  */
  void set_diagonal_covariance(bool diagonal_covariance=true)
  {
    independent_dimensions_ = diagonal_covariance;
  }

  /** Change initialization method **/
  void set_initialization_method(InitializationMethod init_method)
  {
    initialization_method_ = init_method;
  }

 protected:

  /** Get an initialization Normal distribution (cluster) P(X | C=c).
     
      The default implementation returns a random Normal (Gaussian) distribution constructed as follows:
      @code
      plDistribution plLearnGMM::get_initialization_cluster(const plVariablesConjunction &X,
      size_t nc, size_t trial)
      {
      if(X.dim() == 1) {
      return plNormal(plSymbol(X), 
      range_[0].first + plRandomFloat(range_[0].second - range_[0].first),
      std::sqrt(variance_[0]));
      }
      else {
      plFloatVector mean(X.dim());
      plFloatMatrix Sigma(X.dim(), X.dim());
      for(size_t i = 0; i < X.dim(); ++i) {
      mean[i] = range_[i].first + plRandomFloat(range_[i].second - range_[i].first);
      Sigma[i][i] = variance_[i];
      }
      return plNormal(X, mean, Sigma);
      }
      }
      @endcode
  */
  virtual plDistribution get_initialization_cluster(const plVariablesConjunction &X,
                                                    size_t c, size_t trial) const;

  virtual plLearnObject *create_cluster_learn_object(const plVariablesConjunction &X,
                                                     const plSymbol &C) const;

  virtual void initialize_distributions(const plVariablesConjunction& X,
                                        const plSymbol &C,
                                        size_t trial,
                                        plDistribution &init_pc,
                                        plDistributionTable &init_px) const;
  
 private:
  void init_initialization_parameters();

 protected:
  std::vector<std::pair<plFloat, plFloat> > range_;
  std::vector<plFloat> variance_;
  bool independent_dimensions_;
  InitializationMethod initialization_method_;

  SPL_ACCESS1(plLearnGMM);
};


#endif // plLearnGMM_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
