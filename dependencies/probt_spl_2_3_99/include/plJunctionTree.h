/*===========================================================================
 * Product        : ProBT
 * File           : plJunctionTree.h
 * Author         : Ronan Le Hy <ronan.lehy@probayes.com>
 *
 *===========================================================================
 *        (c) Copyright 2007, ProBayes SAS  -  all rights reserved
 *===========================================================================*/

#ifndef PLJUNCTIONTREE_H
#define PLJUNCTIONTREE_H

#include <plConfig.h>
#include <plJtDistribution.h>

/**
   Implements exact inference using a junction tree. A junction tree is a
   structure computed from a joint distribution, that allows estabshing
   formulas for answering questions in a way that maximises the amount of
   shared computations between different questions. For instance, a junction
   tree built from a decomposition of P(A B C) will be able to share intermediate
   numerical results used to answer P(A | c=0) and P(B | c=0), or P(A | c=0) and P(A | c=1).

   A junction tree can only be built from a decomposition where all factors have
   exactly one left variable. Moreover, all factors must be discretizable distributions (distributions that
   can be compiled).

   Asking a question to a plJunctionTree triggers transparently the construction of the tree
   (if necessary), and yields a symbolic expression in the form of a plJtDistribution or plCndJtDistribution.
   The resulting distribution can then be compiled, instantiated, as with other distributions.

   In order to maximise sharing between questions having the same left variables, but
   different-but-included sets of right variables, such as:
     P(A), P(A | B), P(A | B C), P(A | B D),
   you should use specialise(), like so:
     - P(A) = junction_tree.ask(A)
     - P(A | B) = P(A).specialise(B)
     - P(A | B C) = P(A | B).specialise(C)
     - P(A | B D) = P(A | B).specialise(D)

   This will result in a better sharing than the one obtained from asking all these
   questions separately to the plJunctionTree.

 */
class PL_DLL_API plJunctionTree
{
private:
  class plJunctionTree_impl * impl_;
  
private:
  /// Disabled copy constructor.
  plJunctionTree(plJunctionTree const &);
  /// Disabled assignment operator.
  plJunctionTree & operator=(plJunctionTree const &);

protected:
  /// Debug method. Returns all ways to compute P(left | right).
  std::vector<plCndJtDistribution> ask_all(plVariablesConjunction const & left,
				     plVariablesConjunction const & right);


public:
  /** Default constructor. Use set_joint_distribution() or set_decomposition() to link a
      decomposition to this junction tree after calling this constructor.
   */
  plJunctionTree();

  /** Destructor. */
  ~plJunctionTree();

  /** Constructs a junction tree object from a decomposition. This is light :
      no symbolic or numeric computations occurs until a question is asked with ask().

      @param[in] decomposition The decomposition this tree will operate with. This must contain
      only discretizable distributions (that is, distributions it is possible to compile()). All factors
      of the decomposition must have exactly one left variable (and zero or more right variables).
  */
  explicit plJunctionTree(plComputableObjectList const & decomposition);

  /** Constructs a junction tree object from a joint distribution. This is light :
      no symbolic or numeric computations occurs until a question is asked with ask().

      This is a convenience constructor that just builds the junction tree from the
      decomposition inside the joint distribution.

      @param[in] joint The joint distribution this tree will operate with. This must contain
      only discretizable distributions (that is, distributions it is possible to compile()). All factors
      of the decomposition must have exactly one left variable (and zero or more right variables).
   */
  explicit plJunctionTree(plJointDistribution const & joint);

  /** Sets the decomposition for this tree.

      @param[in] decomposition The decomposition this tree will operate with. This must contain
      only discretizable distributions (that is, distributions it is possible to compile()). All factors
      of the decomposition must have exactly one left variable (and zero or more right variables).
   */
  void set_decomposition(plComputableObjectList const & decomposition);

  /** Asks a non-conditional question to the tree.

      @param[in] variables Variables for this question.

      @return A symbolic distribution corresponding to P(variables).
   */
  plJtDistribution ask(plVariablesConjunction const & variables);
 
  /** Asks a conditional question to the tree.
      
      @param[in] left Left variables for this question.
      @param[in] right Right variables for this question.
      @param[in] do_not_build_normalization_expression Whether we should not attempt to normalize
      the results of plCndJtDistribution::compute() on the question.

      @return A symbolic conditional distribution corresponding to P(left | right).
  */
  plCndJtDistribution ask(plVariablesConjunction const & left, plVariablesConjunction const & right,
		    bool do_not_build_normalization_expression = false);

  /** Outputs the internal structures of the junction tree in a human-readable format.
   */
  void output_debug(std::ostream & os) const;

  /** Outputs the internal structures of the junction tree in Graphviz (dot) format.
      This will output the Bayesian network, the moralised graph, and the clique graph
      in this junction tree.
   */
  void output_dot(std::ostream & os) const;
};

#endif /* PLJUNCTIONTREE_H */

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
