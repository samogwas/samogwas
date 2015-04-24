/*=============================================================================
 * Product        : ProBT 
 * File           : plCndBinomial.h
 * Author         : David Raulo
 * Creation       : Fri Aug  7 16:28:59 2009
 *
 *=============================================================================
 *        (c) Copyright 2009, ProBayes SAS  -  all rights reserved
 *=============================================================================
*/

#ifndef PL_CND_BINOMIAL_H
#define PL_CND_BINOMIAL_H

#include "plConfig.h"
#include <plCndDistribution.h>
#include <plExternalFunction.h>

/** Conditional Binomial distribution on one-dimensional space.
    A \em plCndBinomial is a \em plBinomial familly for which the \em p
    parameter is not constant, but either a known \em plSymbol or a
    \em plExternalFunction. When instantiated, a \em plBinomial is obtained. */
class PL_DLL_API plCndBinomial: public plCndDistribution
{
public:
  /** Default constructor. */
  plCndBinomial();
  
  /** Constructs a conditional binomial distribution on variable @em left in
      which mean @em p is a variable. The variable @em left must be of type @em
      plIntegerType.
  */
  plCndBinomial(const plSymbol& left,
                const plSymbol& p);

  /** Constructs a conditional binomial distribution on variable @em left in
      which p is fixed using an external function applied to variables @em
      right.  The variable @em left must be of type @em plIntegerType.

      An example of how to create and use plExternalFunction as conditional distribution
      parameters can be found in @ref param_func_example "'using external functions example'".
  */
  plCndBinomial(const plSymbol& left,
                const plVariablesConjunction& right,
                const plExternalFunction& fp);

  /** Destructor. */
  virtual ~plCndBinomial() {}

  /** Promote from a plCndDistribution. If the object being copied is
      actually of the wrong type, an exception is raised. */
  explicit plCndBinomial(const plCndDistribution&);

  ///@name Parameters accesors.
  ///@{

  /** Returns true if the p parameter is a variable. */
  bool p_is_variable() const;
  /** Returns the p variable. Do not call this method if p_is_variable()
      returned false. */
  plVariablesConjunction get_p_variable() const;

  /** Returns true if the p parameter is a function. */
  bool p_is_function() const;
  /** Returns the p function. Do not call this method if p_is_function()
      returned false. */
  plExternalFunction get_p_function() const;

  ///@}

  virtual bool is_base_class()const
  {
    return false;
  }

};

#endif //PL_CND_BINOMIAL_H

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
