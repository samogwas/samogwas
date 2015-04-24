/*=============================================================================
 * Product        : ProBT 
 * File           : plCndPoisson.h
 * Author         : David Raulo
 * Creation       : Sun Sep 16 18:41:41 2007
 *
 *=============================================================================
 *        (c) Copyright 2007, ProBayes SAS  -  all rights reserved
 *=============================================================================
*/

#ifndef PL_CND_POISSON_H
#define PL_CND_POISSON_H

#include "plConfig.h"
#include <plCndDistribution.h>
#include <plExternalFunction.h>

/** Conditional Poisson distribution on one-dimensional space.
    A \em plCndPoisson is a \em plPoisson familly for which the \em mu
    parameter is not constant, but either a known \em plSymbol or a
    \em plExternalFunction. When instantiated, a \em plPoisson is obtained. */
class PL_DLL_API plCndPoisson: public plCndDistribution
{
public:
  /** Default constructor. */
  plCndPoisson();

  
  /** Constructs a conditional poisson distribution on variable @em left in
      which mean @em mu is a variable. The variable @em left must be of type @em
      plIntegerType.
  */
  plCndPoisson(const plSymbol& left,
               const plSymbol& mu);

  /** Constructs a conditional poisson distribution on variable @em left in
      which mean is fixed using an external function applied to variables @em
      right.  The variable @em left must be of type @em plIntegerType.
  */
  plCndPoisson(const plSymbol& left,
               const plVariablesConjunction& right,
               const plExternalFunction& fmu);

  /** Destructor. */
  virtual ~plCndPoisson() {}

  /** Promote from a plCndDistribution. If the object being copied is
      actually of the wrong type, an exception is raised. */
  explicit plCndPoisson(const plCndDistribution&);

  ///@name Parameters accesors.
  ///@{

  /** Returns true if the mu parameter is a variable. */
  bool mu_is_variable() const;
  /** Returns the mu variable. Do not call this method if mu_is_variable()
      returned false. */
  plVariablesConjunction get_mu_variable() const;

  /** Returns true if the mu parameter is a function. */
  bool mu_is_function() const;
  /** Returns the mu function. Do not call this method if mu_is_function()
      returned false. */
  plExternalFunction get_mu_function() const;

  ///@}

  virtual bool is_base_class()const
  {
    return false;
  }

};

#endif //PL_CND_POISSON_H

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
