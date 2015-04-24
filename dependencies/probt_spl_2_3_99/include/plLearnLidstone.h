/*==============================================================================
 * Product        : ProBT 
 * File           : plLearnLidstone.h
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


#ifndef plLearnLidstone_h
#define plLearnLidstone_h

#include "plConfig.h"

#include <plBayesLearnProbTable.h>


SPL_DECL1(plLearnLidstone);

/** This class allows to make a Bayesian estimation of multinomial (ProbTable)
    distributions using Lidstone's formula. This is a special case of the
    Multinomial-Dirichlet conjugacy in which all the initial values for the
    parameters of the Dirichlet prior are set to \f$ \alpha \f$. In other
    words, each possible value is supposed to be @em virtually observed \f$
    \alpha \f$ times (before using data).
    
    The a posteriori estimate is: \f[ p_i = \frac {\alpha + n_i} { \sum_i
    \alpha + n_i}.\f] where \f$ n_i \f$ is the number of observations (in the
    data) of the ith value.

    \sa plBayesLearnProbTable 
    \sa plLearnLaplace
*/
class PL_DLL_API plLearnLidstone : public plBayesLearnProbTable
{
public:

  /** Default constructor */
  plLearnLidstone()
    : plBayesLearnProbTable(){}
	
  /** Constructor using a set of variables, with parameter alpha */
  explicit plLearnLidstone(const plVariablesConjunction &vars, plFloat alpha=PL_ONE)
    : plBayesLearnProbTable(vars,
                            std::vector<plFloat>((unsigned int)vars.cardinality(), alpha)) 
  {}
	
  /** Dectructor. */
  ~plLearnLidstone() {}

  virtual plLearnObject* clone()const
  {
    return new plLearnLidstone(*this);
  }

 protected:
  virtual void Output(std::ostream &os) const;
};

#endif //plLearnLidstone_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
