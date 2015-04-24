/*=============================================================================
 * Product        : ProBT 
 * File           : plLearnPoisson.h
 * Author         : Kamel Mekhnacha
 * Creation       : Mon Sept 13 14:57:12 2005
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

#ifndef plLearnPoisson_h
#define plLearnPoisson_h

#include "plConfig.h"

#include <plNonCndLearnObject.h>
#include <plPoisson.h>

SPL_DECL1(plLearnPoisson);


/** This class allows to learn Poisson distributions.*/
class PL_DLL_API plLearnPoisson :public plMLLearnObject
{
public:
  /** Type of the variable (1D). */
  typedef plSymbol VarType;
  
  /** Constructor using a given variable.*/
  explicit plLearnPoisson(const plSymbol &vars) 
    : plMLLearnObject(vars)
    , s_(PL_ZERO)
    , init_mean_(PL_ZERO)
    , init_weight_(PL_ZERO)
  {}
    
  /** Constructor using a set of variables, an initial mean and an initial weight .*/
  plLearnPoisson(const plSymbol &vars, plFloat init_mean,
                 plFloat init_weight = PL_ONE)
    : plMLLearnObject(vars)
    , s_(init_weight*init_mean)
    , init_mean_(init_mean)
    , init_weight_(init_weight)
  {
    total_weight_ = init_weight_;
  }
  
  /** Void default constructor. */
  plLearnPoisson()
    : plMLLearnObject()
    , s_(PL_ZERO)
    , init_mean_(PL_ZERO)
    , init_weight_(PL_ZERO)
  {}
    
  /** Dectructor. */
  virtual ~plLearnPoisson(){};
    
  /** Resets learning. */
  void reset();

  
  /** Returns the 'mu' parameter. */
  plFloat get_mu()const;
  
  /** Returns the Poisson distribution corresponding to the learnt
    distribution.
  */
  plDistribution get_distribution() const;
  
  void get_parameters(plValues &params) const;

  unsigned int get_n_parameters()const
  {
    if(all_vars_.is_empty())
      throw plError(75, "when calling 'plLearnPoisson::get_n_parameters'");

    return 1;
  }

  virtual plLearnObject* clone()const
  {
    return new plLearnPoisson(*this);
  }
  
protected:
  /** Adds a point \em point with a given weight \em weight and updates 
      the mean statistics. 
      This is an internal method, do not use it directly. Use add_point
      instead.
  */
  bool internal_addPoint(const plDataValues &point, plFloat weight);

  virtual void Output(std::ostream &out) const;

protected:
  SPL_ACCESS1(plLearnPoisson);

  plFloat s_;  
  
  plFloat init_mean_;
  plFloat init_weight_;

  template <class T> friend class plCndLearnObject;
};




#endif //plLearnPoisson_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
