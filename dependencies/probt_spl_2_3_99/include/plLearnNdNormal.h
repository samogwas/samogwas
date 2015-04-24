/*=============================================================================
 * Product        : ProBT 
 * File           : plLearnNdNormal.h
 * Author         : Kamel Mekhnacha
 * Creation       : Mon Oct 27 14:57:12 2003
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

#ifndef plLearNdNormal_h
#define plLearNdNormal_h

#include "plConfig.h"

#include <plNonCndLearnObject.h>
#include <plFloatVector.h>
#include <plFloatMatrix.h>
#include <plNormal.h>

SPL_DECL1(plLearnNdNormal);

//-----------------------------------------------------------------------------
/** This class allows to learn multi-dimensional Normal (Gaussian)
    distributions. */
class PL_DLL_API plLearnNdNormal :public plMLLearnObject
{
public:
  /** Type of the variable (multi-dim). */
  typedef plVariablesConjunction VarType;

  /** Constructor using a set of variables.*/
  explicit plLearnNdNormal(const plVariablesConjunction &vars);
  
  /** Constructor using a set of variables, an initial mean, initial
   * covariance, and an initial weight .*/
  plLearnNdNormal(const plVariablesConjunction &vars,
                  const plFloatVector &init_mean,
                  const plFloatMatrix &init_matrix, plFloat init_weight = PL_ONE);
  
  /** Void default constructor. */
  plLearnNdNormal();
    
  /** Destructor. */
  virtual ~plLearnNdNormal(){};
    
  /** Resets learning. */
  void reset();

  
  /** Returns the mean vector in the output parameter \em mean. */
  void get_mean(plFloatVector &mean) const;
    
  /** Returns the mean vector. */
  plFloatVector get_mean() const;
  
  /** Returns the covariance matrix in the output parameter \em covariance. */
  void get_covariance(plFloatMatrix &covariance) const;
    

  /** Returns the covariance matrix. */
  plFloatMatrix get_covariance() const;
    
  /** Returns, for each dimension, the [min, max[ range allowing to cover a
      total cumulated probability of @em cumul_prob (i.e., leaving outside the
      [min, max[ range a total cumulated probability of 1 - @em cumul_prob)
      according to the current estimate of the mean vector and variance matrix.
  */
  void get_actual_min_max(plFloatVector &min, plFloatVector &max,
                          plProbValue cumul_prob = 0.95) const;
  
  /** Returns the normal corresponding to the learnt distribution */
  plDistribution get_distribution() const;


  using plLearnObject::add_point;
  inline bool add_point(const plFloatVector &point, plFloat weight = PL_ONE)
  {
    return add_point(point.data(), weight);
  }
  void get_parameters(plValues &params) const;

  unsigned int get_n_parameters()const
  {
    if(all_vars_.is_empty())
      throw plError(75, "when calling 'plLearnNdNormal::get_n_parameters'");

    return dimension_ + dimension_*(dimension_+1)/2;
  }

  virtual plLearnObject* clone()const
  {
    return new plLearnNdNormal(*this);
  }
  
 protected:
  SPL_ACCESS1(plLearnNdNormal);

  bool internal_addPoint(const plDataValues &point, plFloat weight = PL_ONE);
  
  bool internal_addPointWithDef(const plDataValues &point, 
                                const std::vector <bool> &left_defined,
                                plFloat weight = PL_ONE);

  virtual void Output(std::ostream &out) const;

  void get_covariance_internal(plFloatMatrix &covariance) const;
  plFloatMatrix get_covariance_internal() const;
  void get_mean_internal(plFloatVector &mean) const;
  plFloatVector get_mean_internal() const;

protected:
  unsigned int dimension_;
  std::vector <plFloat> s_;
  std::vector <plFloat> sij_;
  

  plFloatVector init_mean_;
  plFloatMatrix init_covariance_;
  plFloat init_weight_;
  
  bool has_initialization_;

  template<typename T> friend class plCndLearnObject;
};


#endif //plLearNdNormal_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
