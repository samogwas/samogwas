/*===========================================================================
 * Product        : ProBT
 * File           : plJtDistribution.h
 * Author         : Ronan Le Hy <ronan.lehy@probayes.com>
 *
 *===========================================================================
 *        (c) Copyright 2007, ProBayes SAS  -  all rights reserved
 *===========================================================================*/

#ifndef PLJTDISTRIBUTION_H
#define PLJTDISTRIBUTION_H

#include "plConfig.h"
#include <plDistribution.h>
#include <plCndDistribution.h>

class kplJtKernel;
class kplCndJtKernel;

SPL_DECL1(plCndJtDistribution);

/** Conditional distribution as returned by asking a question to a plJunctionTree.
 */
class PL_DLL_API plCndJtDistribution :
  public plCndDistribution
{
 protected:
  kplCndJtKernel & root();
  kplCndJtKernel const & root() const;
  bool is_base_class() const { return false; }

 public:
  /** Empty constructor. */
  explicit plCndJtDistribution();

  explicit plCndJtDistribution(kplCndJtKernel const & vroot);

  /**  Returns a specialised version of this distribution.
       Specialising transforms a summed or non-instantiated variable into an instantiated variable.

       For instance, P(A | D).specialise(B C D) == k * P(A | B C D).
       
       The specialized expression should share as many caches as possible with
       the original expression.
       
       @param[in] added_known Variables to specialise. None of these should be
       already in the right variable set.
  */
  plCndJtDistribution specialise(plVariablesConjunction const & added_known); 

  /** Outputs the formula inside this distribution in dot format. The formula is
      represented as a tree of expressions. Together with other formulas from
      other distributions, it forms a DAG of expressions that helps see the shared
      computations produced by the junction tree algorithm.
  */
  void output_dot(std::ostream & os) const;

  SPL_ACCESS1(plCndJtDistribution);
};

SPL_DECL1(plJtDistribution);

/** Non-conditional distribution as returned by asking a question to a plJunctionTree.
 */
class PL_DLL_API plJtDistribution :
  public plDistribution
{
protected:
  kplJtKernel & root();
  kplJtKernel const & root() const;
  bool is_base_class()const{return false;}

public:
  /** Empty constructor. */
  explicit plJtDistribution();

  explicit plJtDistribution(kplJtKernel const & vroot);

  /**  Returns a specialised version of this distribution.
       Specialising adds or moves a variable to the right variable set.

       For instance, P(A D).specialise(B C D) == P(A | B C D).
       
       The specialized expression shares as many caches as possible with
       the original expression. This is therefore more efficient than
       asking P(A D) and P(A | B C D) separately to a plJunctionTree
       or plJointDistribution.
       
       @param[in] added_known Variables to specialise. None of these should be
       already in the right variable set.
  */
  plCndJtDistribution specialise(plVariablesConjunction const & added_known); 

  /** Outputs the formula inside this distribution in dot format. The formula is
      represented as a tree of expressions. Together with other formulas from
      other distributions, it forms a DAG of expressions that helps see the shared
      computations produced by the junction tree algorithm.
  */
  void output_dot(std::ostream & os) const;

  SPL_ACCESS1(plJtDistribution);
};

#endif /* PLJTDISTRIBUTION_H */


// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
