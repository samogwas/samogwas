/*=============================================================================
 * Product        : ProBT 
 * File           : plLearnBeta.h
 * Author         : Kamel Mekhnacha
 * Creation       : March 17 2011
 *
 *=============================================================================
 *     (c) Copyright 2011, ProBayes SAS  -  all rights reserved
 *=============================================================================
 *
 *------------------------- Description ---------------------------------------
 *   
 *   
 *-----------------------------------------------------------------------------
 */

#ifndef plLearnBeta_h
#define plLearnBeta_h

#include "plConfig.h"

#include <plNonCndLearnObject.h>
#include <plLearn1dNormal.h>
#include <plBeta.h>

SPL_DECL1(plLearnBeta);

/** This class allows to learn Beta distributions. 
    
    Learning concerns exclusively the @em p and @em q parameters.  The
    @em a and @em b range parameters are assumed known.
*/
class PL_DLL_API plLearnBeta :public plMLLearnObject
{ 
public:
  /** Type of the variable (1D). */
  typedef plSymbol VarType;
  
  /** Constructor */
  explicit 
    plLearnBeta(const plSymbol &vars, plFloat a = PL_ZERO, plFloat b = PL_ONE);
  
  /** Default constructor. */
  plLearnBeta()
    : plMLLearnObject()
    , a_(PL_ZERO)
    , b_(PL_ZERO)
    , normalL_()
  {}
  
  /** Dectructor. */
  virtual ~plLearnBeta(){};
  
  /** Resets learning. */
  void reset();
  
  /** Sets the range parameters. */
  void set_range(plFloat a, plFloat b);
  
  /** Returns the range parameters. */
  void get_range(plFloat &a, plFloat &b)const;
  
  /** Returns the 'p' parameter. */
  plFloat get_p()const;
  
  /** Returns the 'q' parameter. */
  plFloat get_q()const;
  
  /** Returns the learnt Beta distribution */
  plDistribution get_distribution() const;
  
  void get_parameters(plValues &params) const;

  unsigned int get_n_parameters()const
  {
    return 2;
  }

  virtual plLearnObject* clone()const
  {
    return new plLearnBeta(*this);
  }

  SPL_ACCESS1(plLearnBeta);

protected:
  /** Adds a point \em point with a given weight \em weight and updates the
      statistics. This is an internal method, do not use it directly.
      Use add_point instead.
  */
  bool internal_addPoint(const plDataValues &point, plFloat weight);

  virtual void Output(std::ostream &out) const;


protected:
  plFloat a_;
  plFloat b_;
  plLearn1dNormal normalL_;

  template <class T> friend class plCndLearnObject;
};



#endif //plLearnBeta_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
