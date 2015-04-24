/*=============================================================================
 * Product        : ProBT 
 * File           : plPolicyNetwork.h
 * Author         : David Raulo
 * Creation       : Tue Sep  8 09:24:01 2009
 *
 *=============================================================================
 *        (c) Copyright 2009, ProBayes SAS  -  all rights reserved
 *=============================================================================
*/

#ifndef PL_POLICY_NETWORK_H
#define PL_POLICY_NETWORK_H

#include <plConfig.h>
#include <plJointDistribution.h>
#include <plComputableObject.h>
#include <iostream>

//-----------------------------------------------------------------------------
/**
   Class for solving decision problems using policy networks.
   
   A policy network is a kind of influence diagram, containing chance nodes
   (random variables), action nodes (decision) and utility nodes. The problem
   consists in finding an optimal policy for the action nodes that maximizes
   the sum of all utilities.
 */
class PL_DLL_API plPolicyNetwork: public plBuiltinModel
{
public:
  /**
     Construct an initial policy network from the given chance, action and
     utility variables.

     \param[in] chance_dist the product of distributions for the chance
     variables ;

     \param[in] action_dist the product of distributions for the action nodes ;
     These distributions specify only the dependences of the action nodes, and
     thus each one will typically be an instance of class plCndUnknown. The
     method run_optimization() will compute the actual distributions.  Note
     that the order of the distributions in the product is important: it must
     match the temporal dependency of the corresponding action variables.

     \param[in] utility_dist the product of distribution for the utility nodes
     ; Each of these will typically be a plCndDeterministic (deterministic
     utility computed from a formula in an external function), but this is not
     mandatory.

     Note: with the current implementation, all the variables must be
     discrete.

   */
  plPolicyNetwork(const plComputableObjectList& chance_dist,
                  const plComputableObjectList& action_dist,
                  const plComputableObjectList& utility_dist);

  /** Destructor */
  ~plPolicyNetwork();

  /** Copy constructor */
  plPolicyNetwork(const plPolicyNetwork&);

  /** Assignment operator */
  plPolicyNetwork& operator=(const plPolicyNetwork&);

  /**
     Perform the computation of the optimal policies. The optimum found is a
     local one; it is guaranteed to reach a global optimum only if there is
     only one action in the network.

     \return the obtained policies, in the form of a product of conditional
     distributions over the action nodes.
   */
  plComputableObjectList run_optimization();

  /**
     Write the policies in human-readable form to the given output stream.

     The result is only meaningful if this method is called after
     plPolicyNetwork::run_optimization(). If you called it before, you will
     get an exception.
  */
  void output_policies(std::ostream& out) const;

  /**
     Write the policy network in human-readable form to the given output
     stream.
  */
  void output_network(std::ostream& out) const;

  /**
     Write the policy network in Graphviz format to the given output stream.
  */
  void output_network_dot(std::ostream& out) const;

  /**
     Get the joint distribution corresponding to the policy network.

     This joint distribution is defined over the chance, action and utility
     variables given to the class constructor plus one variable representing
     the sum of all utilities.

     The product terms corresponding to the policies (over the decison
     variables) are defined as plMutableCndDistribution objects, initialized to
     uniform distributions.  When you call plPolicyNetwork::run_optimization()
     these terms are automatically replaced by the policies found by the
     algorithm.

     \sa plPolicyNetwork::get_sum_utilities()
     \sa plPolicyNetwork::run_optimization()
  */
  const plJointDistribution& get_joint_distribution();

  /**
     Get the variable representing the sum of all utilities.
     This can be used to interrogate the joint distribution of the computed
     policy network.
     \sa plPolicyNetwork::get_joint_distribution()
  */
  const plSymbol& get_sum_utilities();

 protected:
  /** Writes the object in human-readable form at the \em out stream. */
  virtual void Output(std::ostream &out) const;

private:
  struct Impl;
  Impl* pimpl_;
};

//-----------------------------------------------------------------------------
#endif //PL_POLICY_NETWORK_H

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
