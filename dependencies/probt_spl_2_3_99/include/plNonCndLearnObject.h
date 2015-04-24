/*=============================================================================
 * Product        : ProBT 
 * File           : plNonCndLearnObject.h
 * Author         : Kamel Mekhnacha
 * Creation       : Fri Feb 23 11:57:18 2007
 *
 *=============================================================================
 *        (c) Copyright 2006, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */

#ifndef plNonCndLearnObject_h
#define plNonCndLearnObject_h

#include "plConfig.h"



#include <plLearnObject.h>
#include <plDistribution.h>

//-----------------------------------------------------------------------------
SPL_DECL1(plNonCndLearnObject);

/** This is the base class of all Non-conditional incremental (adaptive)
 * learning objects.
 */
class PL_DLL_API plNonCndLearnObject :public plLearnObject
{
 public:
  /** Void default constructor. */
  plNonCndLearnObject()
    : plLearnObject()
    {}
  
  /** Constructor using a set of variables (i.e for learning \em P(variables). */
  explicit plNonCndLearnObject(const plVariablesConjunction &variables)
    :plLearnObject(variables)
  {}
                      

  /** Destructor. */
  virtual ~plNonCndLearnObject(){}
  
  /** Constructs the distribution corresponding to the learnt distribution. */
  virtual plDistribution get_distribution() const = 0;

  /** Constructs the computable object corresponding to the learnt distribution. */
  plComputableObject get_computable_object()const
  {return get_distribution();}

  
  /** Returns the left variables */
  const plVariablesConjunction& get_left_variables()const{return all_vars_;}

  /** Returns the right variables */
  const plVariablesConjunction& get_right_variables()const
  {
    return plVariablesConjunction::empty();
  }

  
  /** Returns all learnt parameters of the object (parameters passed to the
      distribution constructors when calling 'get_distribution'). This virtual
      method has to be implemented in all derived classes.
  */
  virtual void get_parameters(plValues &params) const = 0;
  
 protected:
  SPL_ACCESS1(plNonCndLearnObject);
};

//-----------------------------------------------------------------------------
SPL_DECL1(plBayesLearnObject);

/** This is the base class of all non-conditional incremental (adaptive)
    learning objects using a Bayesian criterion, by taking
    advantage of the conjugacy property.

    We note:
    - a variable @em X over which we want to learn a distribution,
    - a set of parameters @em Theta for the distribution to learn over X,
    - a set of experimental data \f[ \delta = x^0 .. x^n \f].
   
    We are interested in learning a distribution over X. Therefore we write:
    - P(X Theta) = P(Theta) P(delta | Theta) = P(Theta) P(x^0 | Theta) ... P(x^n | Theta)
    - P(Theta) : prior over parameters
    - P(x^i | Theta) = P(X | Theta) : likelihood function.
    
    Given a likelihood function with a given set of parameters, there sometimes
    exist a particular form of P(Theta) that will yield a posterior 
    P(Theta | X) of the same particular form. This particular form is called
    the conjugate prior of the likelihood function P(X | Theta).

    We can then evaluate analytically P(Theta | delta). This allows to compute
    the set of parameters @em theta* that either maximise P(theta | delta), or
    that correspond to the expectation of distribution P(Theta | delta).

    @sa plBayesLearnBinomial
*/
class PL_DLL_API plBayesLearnObject : public plNonCndLearnObject
{
 public:
  /** Empty Constructor. */
  plBayesLearnObject()
    : plNonCndLearnObject()
    {}; 

  /** Contructor using a set of variables. */
  explicit plBayesLearnObject(const plVariablesConjunction &vars) 
    : plNonCndLearnObject(vars){};
  
  /** Returns the posterior distribution on the learnt parameters. */
  virtual plDistribution get_aposteriori_distribution(const plVariablesConjunction &param_variables) const = 0;
	
};


//-----------------------------------------------------------------------------
SPL_DECL1(plMLLearnObject);

/** This is the base class of all Non-conditional incremental (adaptive)
    learning objects using the Maximum Likelihood criterion.
*/
class PL_DLL_API  plMLLearnObject: public plNonCndLearnObject
{ 
 public:
  /** Empty Constructor */
  plMLLearnObject()
    : plNonCndLearnObject()
    {};

  /** Contructor using a set of variables */
  explicit plMLLearnObject(const plVariablesConjunction &vars) 
    : plNonCndLearnObject(vars){};

};

//-----------------------------------------------------------------------------
#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
