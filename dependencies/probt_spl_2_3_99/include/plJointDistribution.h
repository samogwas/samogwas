/*=============================================================================
 * Product        : ProBT 
 * File           : plJointDistribution.h
 * Author         : Juan-Manuel Ahuactzin
 * Creation       : Wed Mar 21 13:54:44 2001
 *
 *=============================================================================
 *     (c) Copyright 2000, Centre National de la Recherche Scientifique,
 *                              all rights reserved
 *=============================================================================
 *
 *------------------------- Description ---------------------------------------
 *   
 *   
 *-----------------------------------------------------------------------------
 */

#ifndef plJointDistribution_h
#define plJointDistribution_h

#include "plConfig.h"

#include <plVariablesConjunction.h>
#include <plDistribution.h>
#include <plCndDistribution.h>
#include <plTechniquesTypes.h>
#include <plGraph.h>

#include <plDrawingLanguage.h>

class plSymbol;
class plArray;
class kplDescription;




/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/


//=============================================================================
/** 
    The @em plJointDistribution class is used to describe a probabilistic
    model by providing a decomposition of the joint distribution of the whole
    model variables as a product of elementary conditional and non conditional
    distributions.

    Defining this decomposition is equivalent to the graph construction part in
    standard Bayesian Networks (BN) creation tools, while defining the
    parametric forms of the elementary distributions corresponds to the
    probability assessment part in standard BN creation tools.
    
    Therefore, a @em plJointDistribution object contains both the structural
    information (i.e dependencies between variables) and information on the
    parametric forms of the used elementary distributions (already constructed
    and passed as parameters to the constructor).
    
    Given a @em plJointDistribution object, one can infer any target (query)
    distribution using the @em  ask methods to construct the corresponding
    @em exact or @em approximate (Monte Carlo) expression (see the @em
    ask methods and their parameters) using Bayes rule and some other symbolic
    simplifications.
*/
class PL_DLL_API plJointDistribution : public plDistribution
{
 protected:
  
  void check_variables(const plVariablesConjunction &searched_variables,
                       const plVariablesConjunction &known_variables)const;
                       
  void check_variables(const plVariablesConjunction &searched_variables)const;

  kplDescription *get_kernel_description()const;

 public:

  /** Default constructor used for serialization */
  plJointDistribution() 
    : plDistribution()
    { 
      signature_[PLJointDistribution] = true; 
    }

  /** Creates a joint distribution (probabilistic model) using a single
      distribution */
  plJointDistribution (const plVariablesConjunction &all_params, 
		       const plDistribution  &f);
                       
  /** Creates a joint distribution (probabilistic model) using a single
      distribution */
  explicit plJointDistribution (const plDistribution  &f);

  /** Creates a joint distribution (probabilistic model) as a product of
      distributions */
  plJointDistribution (const plVariablesConjunction &all_params, 
		       const plComputableObjectList &decomposition);
                       
  /** Creates a joint distribution (probabilistic model) as a product of
      distributions.
      The variables of the constructed computable object are the concatenation
      (in the same order) of the left variables of all the terms of the list.
  */
  explicit plJointDistribution (const plComputableObjectList &decomposition);

  /** Copy constructor */
  plJointDistribution(const plJointDistribution &other);

  /** Destroys the probabilistic model */
  virtual ~plJointDistribution ();

  /** Writes the drawing instructions to represent the Graph
      of the joint distribution on the ostream out.
      The current implementation allows using xfig and graphvis dot. 
  **/
  void draw_graph(std::ostream& out,
		  plDrawingLanguage drawing_language=PL_XFIG_GRAPH)const;

  /** Writes the drawing instructions to represent the Graph of the
      joint distribution in @em file_name. The current implementation
      allows using xfig and graphvis dot.
  **/
  void draw_graph(const std::string &file_name, 
		  plDrawingLanguage drawing_language=PL_XFIG_GRAPH)const;

  //#ifndef SWIG
  /** Writes the drawing instructions to represent the Graph of the
      joint distribution in @em file_name. The current implementation
      allows using xfig and graphvis dot.
  **/
  void draw_graph(const char *file_name, 
		  plDrawingLanguage drawing_language=PL_XFIG_GRAPH)const;
  //#endif

  /** Sets the object @em graph so that it contains the
      necessary information for drawing a graphical representation of
      the joint distribution 
  **/
  void draw_graph(plGraph &graph)const;

  /** Writes the graphvis dot drawing instructions to represent the graph
      of the joint distribution on the ostream out.
      
      The parameters dot_node_shape, dot_node_color, dot_node_fill_color,
      dot_background_color, and dot_rankdir correspond to graphiz dot ones (see
      http://www.graphviz.org/pdf/dotguide.pdf). There values will be inserted
      in the generated dot file without checking their validity.
  **/
  void draw_graph_dot(std::ostream& out, 
		      const std::string &dot_node_shape="", 
		      const std::string &dot_node_color="",
		      const std::string &dot_node_fill_color="",
		      const std::string &dot_background_color="",
		      const std::string &dot_rankdir="")const;
  
  /** Writes the graphvis dot drawing instructions to represent the graph
      of the joint distribution in @em file_name.

      The parameters dot_node_shape, dot_node_color, dot_node_fill_color,
      dot_background_color, and dot_rankdir correspond to graphiz dot ones (see
      http://www.graphviz.org/pdf/dotguide.pdf). There values will be inserted
      in the generated dot file without checking their validity.
  **/
  void draw_graph_dot(const std::string&file_name, 
		      const std::string &dot_node_shape="", 
		      const std::string &dot_node_color="",
		      const std::string &dot_node_fill_color="",
		      const std::string &dot_background_color="",
		      const std::string &dot_rankdir="")const;
  


  // Conditional P(searched_variables | known_variables)

  /** Creates a conditional probability expression  
      P(searched_variables | known_variables) using exact
      marginalization over the missing variables. This requires that the
      marginalized variables are discrete (otherwise, use ask_mc_nsamples() or
      ask_mc_convergence()).

      \param[out] CndExpr The conditional probability expression resulting from the query.
      \param[in] searched_variables The searched (left) variables of the question.
      \param[in] known_variables The known (right) variables of the question.
      \param[in] optimization The optimality criterion when constructing the
      evaluation tree.
      \param[in] do_not_build_normalization_expression Set it to
      @em true, if you don't need to compute the normalization any more.  You
      should use the default value (@em false) unless you know what you are
      doing.

      The optimality criterion @em optimization should take a value among:

      1) PL_NO_OPTIMIZATION will not attempt to optimize the expression for any
      particular purpose.\n

      2) PL_OPTIMIZE_COMPILATION_TIME will optimize the expression for the
      first instantiate-compile sequence that will follow (usage pattern:
      ask-instantiate-compile). \n

      3) PL_OPTIMIZE_UPDATE_TIME will optimize the expression for repeated
      instantiate-compile sequences (usage pattern: ask once,
      instantiate-compile many times).\n

      4) PL_OPTIMIZE_MEMORY_USE will optimize the expression for minimal memory
      use.\n
      
      You may inspect the produced evaluation tree using
      plComputableObject::print_evaluation_tree_infos().\n

      Note that to take advantage of these optimizations, you must use the
      forms of plCndDistribution::instantiate() and plDistribution::compile() that give
      their result in their first argument (by reference) and return void, not
      those that return their result directly.  Also, only the exhaustive
      plDistribution::compile() can take advantage of these optimization
      hints. plDistribution::n_compile() and plDistribution::time_compile(), for instance,
      will not be able to use them.
  */
  void ask(plCndDistribution &CndExpr,
	   const plVariablesConjunction &searched_variables,
	   const plVariablesConjunction &known_variables, 
	   plOptimizationCriterion optimization = PL_NO_OPTIMIZATION,
	   bool do_not_build_normalization_expression = false) const;

  /** Same as ask(plCndDistribution &,
      const plVariablesConjunction &,
      const plVariablesConjunction &, 
      plOptimizationCriterion,
      bool) 
      above but returns a freshly created plCndDistribution object. If you are calling it in a loop,
      consider using the version that modifies an existing object instead.
  */
  plCndDistribution ask( const plVariablesConjunction &searched_variables,
                         const plVariablesConjunction &known_variables, 
                         plOptimizationCriterion optimization = PL_NO_OPTIMIZATION,
                         bool do_not_build_normalization_expression = false) const;

  /** Creates a conditional probability expression  P(searched_variables | known_variables) using Monte Carlo
      marginalization over the missing variables.

      \param[out] CndExpr The conditional probability expression resulting from the query.
      \param[in] searched_variables The searched (left) variables of the question.
      \param[in] known_variables The known (right) variables of the question.
      \param[in] mc_nsamples The number of Monte Carlo sample points to be used for approximating marginalization 
      (i.e., computation of the sums/integrals)
      \param[in] do_not_build_normalization_expression Set it to
      @em true, if you don't need to compute the normalization any more.  You
      should use the default value (@em false) unless you know what you are
      doing.
  */
  void ask_mc_nsamples(plCndDistribution &CndExpr,
                       const plVariablesConjunction &searched_variables,
                       const plVariablesConjunction &known_variables, 
                       int mc_nsamples,
                       bool do_not_build_normalization_expression = false) const;
  /** Same as 
      ask_mc_nsamples(plCndDistribution &,
      const plVariablesConjunction &,
      const plVariablesConjunction &, 
      int,
      bool)
      above but returns a freshly created plCndDistribution object.
  */
  plCndDistribution ask_mc_nsamples(const plVariablesConjunction &searched_variables,
                                    const plVariablesConjunction &known_variables, 
                                    int mc_nsamples,
                                    bool do_not_build_normalization_expression = false) const;

  void ask_mc_nsamples(plCndDistribution &CndExpr,
                       const plVariablesConjunction &searched_variables,
                       const plVariablesConjunction &known_variables, 
                       unsigned int mc_nsamples,
                       bool do_not_build_normalization_expression = false) const;
  plCndDistribution ask_mc_nsamples(const plVariablesConjunction &searched_variables,
                                    const plVariablesConjunction &known_variables, 
                                    unsigned int mc_nsamples,
                                    bool do_not_build_normalization_expression = false) const;
  
  
  void ask_mc_convergence(plCndDistribution &CndExpr,
                          const plVariablesConjunction &searched_variables,
                          const plVariablesConjunction &known_variables, 
                          float conv_threshold,
                          bool do_not_build_normalization_expression = false) const;
  plCndDistribution ask_mc_convergence(const plVariablesConjunction &searched_variables,
                                       const plVariablesConjunction &known_variables, 
                                       float conv_threshold,
                                       bool do_not_build_normalization_expression = false) const;

  /** Creates a conditional probability expression  P(searched_variables | known_variables) using Monte Carlo
      marginalization over the missing variables.

      \param[out] CndExpr The conditional probability expression resulting from the query.
      \param[in] searched_variables The searched (left) variables of the question.
      \param[in] known_variables The known (right) variables of the question.
      \param[in] conv_threshold The convergence threshold to be used for approximating marginalization. 
      The computation of the sums (integrals) is performed using a
      number of points that allows convergence of the estimation according to
      the threshold value @em conv_threshold:
      | Estim(t) - Estim(t-1) | / (Estim(t)+Estim(t-1)/2.0) < conv_threshold.

      \param[in] do_not_build_normalization_expression Set it to
      @em true, if you don't need to compute the normalization any more.  You
      should use the default value (@em false) unless you know what you are
      doing.
  */
  void ask_mc_convergence(plCndDistribution &CndExpr,
                          const plVariablesConjunction &searched_variables,
                          const plVariablesConjunction &known_variables,
                          double conv_threshold,
                          bool do_not_build_normalization_expression = false) const;
  /** Same as 
      ask_mc_convergence(plCndDistribution &,
      const plVariablesConjunction &,
      const plVariablesConjunction &,
      double,
      bool)
      above but returns a freshly created plCndDistribution object.
  */
  plCndDistribution ask_mc_convergence(const plVariablesConjunction &searched_variables,
                                       const plVariablesConjunction &known_variables, 
                                       double conv_threshold,
                                       bool do_not_build_normalization_expression = false) const;
  


  // Non Conditional P(searched_variables)

  /** Creates a non conditional probability expression P(searched_variables) using exact
      marginalization over the missing variables. This requires that the
      marginalized variables are discrete (otherwise, use ask_mc_nsamples() or
      ask_mc_convergence()).

      \param[out] Expr The non conditional probability expression resulting from the query.
      \param[in] searched_variables The searched (left) variables of the question.
      \param[in] optimization The optimality criterion when constructing the
      evaluation tree.

      The optimality criterion @em optimization should take a value among:

      1) PL_NO_OPTIMIZATION will not attempt to optimize the expression for any
      particular purpose.\n

      2) PL_OPTIMIZE_COMPILATION_TIME will optimize the expression for the
      first instantiate-compile sequence that will follow (usage pattern:
      ask-instantiate-compile). \n

      3) PL_OPTIMIZE_UPDATE_TIME will optimize the expression for repeated
      instantiate-compile sequences (usage pattern: ask once,
      instantiate-compile many times).\n

      4) PL_OPTIMIZE_MEMORY_USE will optimize the expression for minimal memory
      use.\n
      
      You may inspect the produced evaluation tree using
      plComputableObject::print_evaluation_tree_infos().\n
  */
  void ask(plDistribution &Expr,
	   const plVariablesConjunction &searched_variables,
	   plOptimizationCriterion optimization = PL_NO_OPTIMIZATION) const;
  /** Same as 
      ask(plDistribution &,
      const plVariablesConjunction &,
      plOptimizationCriterion)
      above but returns a freshly created plDistribution object.
  */
  plDistribution ask( const plVariablesConjunction &searched_variables,
                      plOptimizationCriterion optimization = PL_NO_OPTIMIZATION) const;
  
  /** Creates a non conditional probability expression P(searched_variables) using Monte Carlo
      marginalization over the missing variables.

      \param[out] Expr The non conditional probability expression resulting from the query.
      \param[in] searched_variables The searched (left) variables of the question.
      \param[in] mc_nsamples The number of Monte Carlo sample points to be used for approximating marginalization 
      (i.e., computation of the sums/integrals)
  */
  void ask_mc_nsamples(plDistribution &Expr,
                       const plVariablesConjunction &searched_variables,
                       int mc_nsamples) const;
  /** Same as 
      ask_mc_nsamples(plDistribution &,
      const plVariablesConjunction &,
      int)
      above but returns a freshly created plDistribution object.
  */
  plDistribution ask_mc_nsamples(const plVariablesConjunction &searched_variables,
                                 int mc_nsamples) const;

  void ask_mc_nsamples(plDistribution &Expr,
                       const plVariablesConjunction &searched_variables,
                       unsigned int mc_nsamples) const;
  plDistribution ask_mc_nsamples(const plVariablesConjunction &searched_variables,
                                 unsigned int mc_nsamples) const;
  
  
  void ask_mc_convergence(plDistribution &Expr,
                          const plVariablesConjunction &searched_variables,
                          float conv_threshold) const;
  plDistribution ask_mc_convergence(const plVariablesConjunction &searched_variables,
                                    float conv_threshold) const;


  /** Creates a non conditional probability expression P(searched_variables) using Monte Carlo
      marginalization over the missing variables.

      \param[out] Expr The conditional probability expression resulting from the query.
      \param[in] searched_variables The searched (left) variables of the question.
      \param[in] conv_threshold The convergence threshold to be used for approximating marginalization. 
      The computation of the sums (integrals) is performed using a
      number of points that allows convergence of the estimation according to
      the threshold value @em conv_threshold:
      | Estim(t) - Estim(t-1) | / (Estim(t)+Estim(t-1)/2.0) < conv_threshold.
  */
  void ask_mc_convergence(plDistribution &Expr,
                          const plVariablesConjunction &searched_variables,
                          double conv_threshold) const;

  /** Same as 
      ask_mc_convergence(plDistribution &,
      const plVariablesConjunction &,
      double)
      above but returns a freshly created plDistribution object.
  */
  plDistribution ask_mc_convergence(const plVariablesConjunction &searched_variables,
                                    double conv_threshold) const;
  
  /** Get back the list of computable objects. */
  plComputableObjectList get_computable_object_list() const;

  /** Return markov blanket for a given variable */
  plVariablesConjunction get_markov_blanket(const plSymbol&s) const;

  /** Set the potential soft evidence variables.
      This ensures that the symbolic simplification does not remove the corresponding 
      evaluation nodes
  */
  void set_soft_evidence_variables(const plVariablesConjunction &variables);

  /** Clear the potential soft evidence variables.
      Equivalent to calling  set_soft_evidence_variables(plVariablesConjunction())
  */
  void clear_soft_evidence_variables()
  {
    set_soft_evidence_variables(plVariablesConjunction());
  }

  virtual bool is_base_class()const
  {
    return false;
  }

#ifdef PL_ENABLE_CONVERTER
  /** Save as Genie xdsl file */
  void save_as_genie(const std::string &file_name) const;

  /** Load from a Genie xdsl file */
  void load_from_genie(const std::string &file_name);

  /** Save as Netica dne file */
  void save_as_netica(const std::string &file_name) const;

  /** Load from a Netica dne file */
  void load_from_netica(const std::string &file_name);

  /** Save as Hugin net file */
  void save_as_hugin(const std::string &file_name) const;

  /** Load from a Hugin net file */
  void load_from_hugin(const std::string &file_name);
#endif

 protected:

  const std::vector<kplComputableObject*> &get_computable_objects_at_kernel() const;

  friend class plJunctionTree; // needs access to get_computable_objects_at_kernel()

 private:

  template<typename COMPUTATION_SCHEME_TYPE>
    void ask_cnd_impl(plCndDistribution &CndExpr,
                      const plVariablesConjunction &searched_variables,
                      const plVariablesConjunction &known_variables, 
                      COMPUTATION_SCHEME_TYPE c,
                      bool do_not_build_normalization_expression) const;
  
  template<typename COMPUTATION_SCHEME_TYPE>
    void ask_impl(plDistribution &Expr,
                  const plVariablesConjunction &searched_variables, 
                  COMPUTATION_SCHEME_TYPE c) const;


  template <typename T>
    plCndDistribution ask_cnd_exact_fresh_impl(const plVariablesConjunction &searched_variables,
                                               const plVariablesConjunction &known_variables, 
                                               T computation_param,
                                               bool do_not_build_normalization_expression) const;
  
  template <typename T>
    plDistribution ask_exact_fresh_impl(const plVariablesConjunction &searched_variables,
                                        T computation_param) const;

  template <typename T>
    plDistribution ask_mc_n_fresh_impl(const plVariablesConjunction &searched_variables,
                                       T computation_param) const;


  template <typename T>
    plDistribution ask_mc_conv_fresh_impl(const plVariablesConjunction &searched_variables,
                                          T computation_param) const;

  template <typename T>
    plCndDistribution ask_cnd_mc_n_fresh_impl(const plVariablesConjunction &searched_variables,
                                              const plVariablesConjunction &known_variables, 
                                              T computation_param,
                                              bool do_not_build_normalization_expression) const;

  template <typename T>
    plCndDistribution ask_cnd_mc_conv_fresh_impl(const plVariablesConjunction &searched_variables,
                                                 const plVariablesConjunction &known_variables, 
                                                 T computation_param,
                                                 bool do_not_build_normalization_expression) const;

};

#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
