/*=============================================================================
 * Product        : ProBT 
 * File           : plLearn1dNormal.h
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

#ifndef plLear1dNormal_h
#define plLear1dNormal_h

#include "plConfig.h"

#include <plNonCndLearnObject.h>
#include <plSymbol.h>

SPL_DECL1(plLearn1dNormal);

/** This class allows to learn one-dimensional Normal (Gaussian) distributions.*/
class PL_DLL_API plLearn1dNormal :public plMLLearnObject
{
public:
  /** Type of the variable (1D). */
  typedef plSymbol VarType;
  
  /** Constructor using a given variable.*/
  explicit plLearn1dNormal(const plSymbol &vars) 
    : plMLLearnObject(vars)
    , s_(PL_ZERO)
    , s2_(PL_ZERO)
    , init_mean_(PL_ZERO)
    , init_std_dev_(PL_ZERO)
    , init_weight_(PL_ZERO)
  {}
  
  /** Constructor using a set of variables, an initial mean, initial
      covariance, and an initial weight .*/
  plLearn1dNormal(const plSymbol &vars, plFloat init_mean,
                  plFloat init_std_dev, plFloat init_weight = PL_ONE)
    : plMLLearnObject(vars)
    , s_(init_weight*init_mean)
    , s2_(init_weight*(init_mean*init_mean + init_std_dev*init_std_dev))
    , init_mean_(init_mean)
    , init_std_dev_(init_std_dev)
    , init_weight_(init_weight)
  {
    total_weight_ = init_weight_;
  }
  
  /** Void default constructor. */
  plLearn1dNormal()
    : plMLLearnObject()
    , s_(PL_ZERO)
    , s2_(PL_ZERO)
    , init_mean_(PL_ZERO)
    , init_std_dev_(PL_ZERO)
    , init_weight_(PL_ZERO)
  {}
    
  /** Dectructor. */
  virtual ~plLearn1dNormal(){};
    
  /** Resets learning. */
  void reset();

  virtual plLearnObject* clone()const
  {
    return new plLearn1dNormal(*this);
  }

  /** Returns the mean value. */
  plFloat get_mu()const;
  
  /** Returns the variance value. */
  plFloat get_var() const;

  
  /** Returns the sigma (standard deviation) value. */
  plFloat get_sigma()const 
  {        
    return std::sqrt( get_var() );
  }
    
  /** Returns the [min, max[ range allowing to cover a total cumulated
      probability of @em cumul_prob (i.e., leaving outside the [min, max[ range
      a total cumulated probability of 1 - @em cumul_pro) according to the
      current estimate of the mean and variance.
  */
  void get_actual_min_max(plFloat &min, plFloat &max, 
                          plProbValue cumul_prob=0.95) const;
  
  /** Returns the normal corresponding to the learnt distribution */
  plDistribution get_distribution() const;

  void get_parameters(plValues &params) const;

  unsigned int get_n_parameters()const
  {
    return 2;
  }
  
protected:
  /** Adds a Point \em point with a given weight \em weight and updates 
      the mean and variances statistics. 
      This is an internal method, do not use it directly. Use add_point
      instead.*/
  bool internal_addPoint(const plDataValues &point, plFloat weight);

  virtual void Output(std::ostream &out) const;

protected:
  SPL_ACCESS1(plLearn1dNormal);

  plFloat s_; // Sum of x
  plFloat s2_; // Sum of x^2
  
  
  plFloat init_mean_;
  plFloat init_std_dev_;
  plFloat init_weight_;

  template <class T> friend class plCndLearnObject;
};




#endif //plLear1dNormal_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
