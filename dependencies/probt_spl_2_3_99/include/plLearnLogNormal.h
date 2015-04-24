/*=============================================================================
 * Product        : ProBT 
 * File           : plLearnLogNormal.h
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

#ifndef plLearnLogNormal_h
#define plLearnLogNormal_h

#include "plConfig.h"

#include <plNonCndLearnObject.h>
#include <plLearn1dNormal.h>
#include <plLogNormal.h>

SPL_DECL1(plLearnLogNormal);

/** This class allows to learn log-normal distributions.
    Learning concerns exclusively the m and sigma parameters.
    The theta (location) parameter is assumed to be known.
*/
class PL_DLL_API plLearnLogNormal :public plMLLearnObject
{ 
public:
  /** Type of the variable (1D). */
  typedef plSymbol VarType;

  /** Constructor using a given variable and a given theta (location) parameter */
  explicit 
    plLearnLogNormal(const plSymbol &vars, plFloat theta = PL_ZERO);
  
  /** Void default constructor. */
  plLearnLogNormal()
    : plMLLearnObject()
    , theta_(PL_ZERO)
    , normalL_()
  {}
  
  /** Dectructor. */
  virtual ~plLearnLogNormal(){};
  
  /** Resets learning. */
  void reset();
  
  /** Sets the theta parameter. */
  void set_theta(plFloat theta);
  
  /** Returns the 'theta' parameter. */
  plFloat get_theta()const ;
  
  /** Returns the 'm' parameter. */
  plFloat get_m()const;
  
  /** Returns the 'sigma' parameter. */
  plFloat get_sigma()const;
  
  /** Returns the lognormal corresponding to the learnt distribution */
  plDistribution get_distribution() const;
  
  void get_parameters(plValues &params) const;

  unsigned int get_n_parameters()const
  {
    return 2;
  }

  virtual plLearnObject* clone()const
  {
    return new plLearnLogNormal(*this);
  }

  SPL_ACCESS1(plLearnLogNormal);

protected:
  /** Adds a point \em point with a given weight \em weight and updates the
      statistics. This is an internal method, do not use it directly.
      Use add_point instead.*/
  bool internal_addPoint(const plDataValues &point, plFloat weight);

  virtual void Output(std::ostream &out) const;


protected:
  plFloat theta_;
  plLearn1dNormal normalL_;

  template <class T> friend class plCndLearnObject;
};



#endif //plLearnLogNormal_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
