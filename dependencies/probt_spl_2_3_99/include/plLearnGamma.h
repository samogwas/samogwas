/*=============================================================================
 * Product        : ProBT 
 * File           : plLearnGamma.h
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

#ifndef plLearnGamma_h
#define plLearnGamma_h

#include "plConfig.h"

#include <plNonCndLearnObject.h>
#include <plLearn1dNormal.h>
#include <plGamma.h>

SPL_DECL1(plLearnGamma);

/** This class implements Gamma distributions ML learning. */
class PL_DLL_API plLearnGamma :public plMLLearnObject
{

public:
  /** Type of the variable (1D). */
  typedef plSymbol VarType;
  
  /** Constructor using a given variable.*/
  explicit plLearnGamma(const plSymbol &vars, plFloat mu = PL_ZERO);
  
  /** Default constructor. */
  plLearnGamma()
    : plMLLearnObject()
    , mu_(PL_ZERO)
    , normalL_()
  {}
  
  /** Dectructor. */
  virtual ~plLearnGamma() {}
  
  /** Resets learning. */
  void reset();

  /** Returns the 'a' parameter. */
  plFloat get_a()const;
  
  /** Returns the 'b' parameter. */
  plFloat get_b()const;
  
  /** Returns the the learnt Gamma distribution */
  plDistribution get_distribution() const;
  
  void get_parameters(plValues &params) const;

  unsigned int get_n_parameters()const
  {
    return 2;
  }

  virtual plLearnObject* clone()const
  {
    return new plLearnGamma(*this);
  }


protected:
  SPL_ACCESS1(plLearnGamma);

  /** Adds a point \em point with a given weight \em weight and updates the
      statistics.
      This is an internal method, do not use it directly.  Use add_point
      instead.*/
  bool internal_addPoint(const plDataValues &point, plFloat weight);

  virtual void Output(std::ostream &out) const;



protected:
  plFloat mu_;
  plLearn1dNormal normalL_;

  template <class T> friend class plCndLearnObject;
  
};



#endif //plLearnGamma_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
