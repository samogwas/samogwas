/*=============================================================================
 * Product        : ProBT 
 * File           : plLearnExponential.h
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

#ifndef plLearnExponential_h
#define plLearnExponential_h

#include "plConfig.h"

#include <plNonCndLearnObject.h>
#include <plExponential.h>

SPL_DECL1(plLearnExponential);

/** This class allows to learn exponential distributions.*/
class PL_DLL_API plLearnExponential :public plMLLearnObject
{
public:
  /** Type of the variable (1D). */
  typedef plSymbol VarType;
  
  /** Constructor using a set of variables, the min value, the initial mean, and the corresponding initial weight .*/
  explicit 
  plLearnExponential(const plSymbol &vars, plFloat min_value,
                     plFloat init_mean, plFloat init_weight = PL_ONE)
    : plMLLearnObject(vars)
    , s_(init_weight*init_mean)
    , init_mean_(init_mean)
    , init_weight_(init_weight)
    , mu_(min_value)
  {
    total_weight_ = init_weight_;
  }

  /** Constructor using a set of variables */
  explicit 
  plLearnExponential(const plSymbol &vars)
    : plMLLearnObject(vars)
    , s_(PL_ZERO)
    , init_mean_(PL_ZERO)
    , init_weight_(PL_ZERO)
    , mu_(PL_ZERO)
  {}
  
  /** Default constructor needed for deserialization. */
  plLearnExponential()
    : plMLLearnObject()
    , s_(PL_ZERO)
    , init_mean_(PL_ZERO)
    , init_weight_(PL_ZERO)
    , mu_(PL_ZERO)
  {}
    
  /** Dectructor. */
  virtual ~plLearnExponential(){};
    
  /** Resets learning. */
  void reset();
  
  /** Returns the 'mu' parameter. */
  plFloat get_mu()const;

  /** Returns the 'beta' parameter. */
  plFloat get_beta()const;
    
  /** Returns the exponential distribution corresponding to the learnt
      distribution */
  plDistribution get_distribution() const;
  

  void get_parameters(plValues &params) const;

  unsigned int get_n_parameters()const
  {
    if(all_vars_.is_empty())
      throw plError(75, "when calling 'plLearnExponential::get_n_parameters'");

    // only beta is learned
    return 1;
  }

  virtual plLearnObject* clone()const
  {
    return new plLearnExponential(*this);
  }

protected:
  /** Adds a point \em point with a given weight \em weight and updates 
      the mean and variances statistics. 
      This is an internal method, do not use it directly. Use add_point
      instead.*/
  bool internal_addPoint(const plDataValues &point, plFloat weight);

  virtual void Output(std::ostream &out) const;


    
protected:
  SPL_ACCESS1(plLearnExponential);

  plFloat s_;
 
  plFloat init_mean_;
  plFloat init_weight_;

  plFloat mu_;

  template <class T> friend class plCndLearnObject;
};




#endif //plLearnExponential_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
