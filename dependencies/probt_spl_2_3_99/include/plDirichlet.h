/*=============================================================================
 * Product        : ProBT 
 * File           : plDirichlet.h
 * Author         : David Raulo
 * Creation       : Wed Dec 13 23:37:49 2006
 *
 *=============================================================================
 *        (c) Copyright 2006, ProBayes SAS  -  all rights reserved
 *=============================================================================
*/

#ifndef plDirichlet_h
#define plDirichlet_h

#include "plConfig.h"

#include <plDistribution.h>

/** The Dirichlet distribution. */
class PL_DLL_API plDirichlet: public plDistribution
{
public:
  /** Construct an empty Dirichlet distribution. */
  plDirichlet();

  /** Construct a Dirichlet distribution.

      @param[in] variables The N-dimensional variable on which the distribution is defined.
      @param[in] alpha The N-dimensional vector of effectives corresponding to each variable in @em variables

      The Dirichlet distribution has the following density function:
      \f[ p(x; \alpha) =  \frac{1}{\beta(\alpha)} \prod_{i=1}^{N} x_i^{\alpha_i-1}\f]
  */
  plDirichlet(const plVariablesConjunction& variables, const std::vector<plFloat>& alpha);

  ~plDirichlet();

  /** Get the parameters used for constructing this object. */
  void get_parameters(std::vector<plFloat>& alpha) const;

  /** Promote from a plDistribution. If the object being copied is
      actually of the wrong type, an exception is raised.
  */
  explicit plDirichlet(const plDistribution&);

  virtual bool is_base_class()const
  {
    return false;
  }

};

#endif // plDirichlet_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
