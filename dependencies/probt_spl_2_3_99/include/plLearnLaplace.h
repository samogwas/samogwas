/*==============================================================================
 * Product        : ProBT 
 * File           : plLearnLaplace.h
 * Author         : Manuel Yguel
 * Creation       : Mon Nov 10 08:52:09 2003
 *
 *==============================================================================
 *     (c) Copyright 2000, Centre National de la Recherche Scientifique,
 *                              all rights reserved
 *==============================================================================
 *
 *------------------------- Description ----------------------------------------
 *   
 *------------------------------------------------------------------------------
*/


#ifndef plLearnLaplace_h
#define plLearnLaplace_h

#include "plConfig.h"

#include <plLearnLidstone.h>

SPL_DECL1(plLearnLaplace);

/** This class allows to make a Bayesian estimation of multinomial (ProbTable)
    distributions using Laplace's formula. This is a special case of the
    Multinomial-Dirichlet conjugacy in which all the initial values for the
    parameters of the Dirichlet prior are set to 1. In other words, each
    possible value is supposed to be @em virtually observed once (before using
    data).
    
    The a posteriori estimate is: \f[ p_i = \frac {1 + n_i} {\sum_i 1 +
    n_i}.\f] where \f$ n_i \f$ is the number of observations (in the data) of
    the ith value.

    \sa plBayesLearnProbTable 
    \sa plLearnLidstone
*/
class PL_DLL_API plLearnLaplace : public plLearnLidstone
{
public:
  
  /** Default constructor */
  plLearnLaplace()
    : plLearnLidstone(){}
	
  /** Constructor using a set of variables. */
  explicit plLearnLaplace(const plVariablesConjunction &vars) 
    : plLearnLidstone(vars, PL_ONE)
  {}
	
  /** Dectructor. */
  ~plLearnLaplace(){}

  virtual plLearnObject* clone()const
  {
    return new plLearnLaplace(*this);
  }

 protected:
  virtual void Output(std::ostream &os) const;

};


#endif //plLearnLaplace_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
