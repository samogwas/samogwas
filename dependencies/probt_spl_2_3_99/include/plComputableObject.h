/// \file plComputableObject.h
/*=============================================================================
 * Product        : ProBT 
 * File           : plComputableObject.h
 * Author         : Juan-Manuel Ahuactzin
 * Creation       : Wed Jun  6 12:01:46 2001
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


#ifndef plComputableObject_h
#define plComputableObject_h

#include "plConfig.h"

#include <plObject.h>
#include <plMath.h>
#include <plSignature.h>
#include <plVariablesConjunction.h>
#include <plValues.h>
#include <plDataValues.h>
#include <plMutable.h>
#include <memory>

/*****************************************************************************
 *                           External Classes
 *****************************************************************************/
class kplComputableObject;
class kplVariable;
class plDistribution;
class plCndDistribution;

/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/

class plComputableObject;

SPL_DECL2(plComputableObjectList);

#if !defined(SWIG) || !defined(SWIGPYTHON)
/*
  plComputableObjectList const iterator.
*/
class PL_DLL_API plComputableObjectList_const_iterator //: public plObject
{
 public:
  typedef plComputableObjectList_const_iterator  Self_;
  typedef std::vector<kplComputableObject *>::const_iterator node_type;

  typedef ptrdiff_t                       difference_type;
  typedef std::bidirectional_iterator_tag iterator_category;
  typedef plComputableObject              value_type;

  plComputableObjectList_const_iterator()
    : node_() { };

  plComputableObjectList_const_iterator(node_type node)
    : node_(node) { };

  virtual ~plComputableObjectList_const_iterator();

  value_type operator*() const;

  std::auto_ptr<value_type> operator->() const;

  /// x.deref() === *x
  value_type deref() const;

  Self_& operator++();

  /// x.incr() === ++x
  Self_& incr();

  Self_ operator++(int);

  Self_& operator--();

  Self_ operator--(int);

  bool operator==(const Self_& other) const;

  bool operator!=(const Self_& other) const;

 private:
  node_type node_;
};
#endif //SWIGPYTHON

/** A @em plComputableObjectList is an STL-like list of @em plComputableObjects. 
    It's provided with iterators and concatenation operators (*, *=).
    A plComputableObjectList is especially used to concatenate a set of plComputableObjects 
    (to be used when creating a @em plJointDistribution for example).
*/
class PL_DLL_API plComputableObjectList : public plObject
{
 public :
  /** Default constructor */
  plComputableObjectList() : internal_list_() {}
  
  /** Constructor using a single computable object */
  explicit plComputableObjectList(const plComputableObject & obj);
  
  /** Copy constructor */
  plComputableObjectList(const plComputableObjectList & l);

  /** Destructor */
  ~plComputableObjectList();

  /** Assignment operator */
  plComputableObjectList &operator=(const plComputableObjectList & l);

  /** Same as operator=() */
  plComputableObjectList &assign_from(const plComputableObjectList & l)
  {
    return operator=(l);
  }
  
  /** Concatenation with a single computable object operator */
  plComputableObjectList operator*(const plComputableObject &f1) const;

  /** Concatenation with a single computable object operator */
  plComputableObjectList concatenate(const plComputableObject &f1) const
  {
    return (*this)*f1;
  }

  /** Concatenation with another computable objects list operator */
  plComputableObjectList operator*(const plComputableObjectList &r_list) const;

  /** Concatenation with another computable objects list operator */
  plComputableObjectList concatenate(const plComputableObjectList &r_list) const
  {
    return (*this)*r_list;
  }

  /** In-place concatenation with a single computable object operator */
  const plComputableObjectList& operator*=(const plComputableObject &f1);

  /** In-place concatenation with a single computable object */
  const plComputableObjectList& push_back(const plComputableObject &f1)
    {
      return this->operator*=(f1);
    }
  
  /** In-place concatenation with another computable objects list operator */
  const plComputableObjectList& operator*=(const plComputableObjectList &r_list);
  
  /** In-place concatenation with another computable objects list */
  const plComputableObjectList& push_back(const plComputableObjectList &r_list)
  {
    return this->operator*=(r_list);
  }
  
  /** Clear the list */
  void clear();

  // Insert a new element at the end of the list */


  /** Return the number of computable objects in the list */
  size_t size() const {return internal_list_.size();}

  /** Access the i-th element as a plComputableObject. */
  plComputableObject operator[](size_t i) const;

  /** Access the i-th element as a plComputableObject. */
  plComputableObject at(size_t i) const;

  /** Return @em true if and only if the list contains zero elements */
  bool empty()const{return internal_list_.empty();}

  /** Associated const_iterator. **/
  typedef plComputableObjectList_const_iterator const_iterator;

  /** 
      Returns a const_iterator to the first object in this list.
  */
  const_iterator begin() const
  { return const_iterator(get_internal_list().begin()); }

  /** 
      Returns a const_iterator pointing one item after the last object
      in this list.
  */
  const_iterator end() const
  { return const_iterator(get_internal_list().end()); }

  /** Returns the first distribution over (i.e., having as left variable) the
      variable @em variable if any.  Raises an exception if no distribution
      over @em variable is found
  */
  plComputableObject get_distribution_over(const plVariablesConjunction &variable)const;
                                              
 protected:
  virtual void Output(std::ostream &out) const;
  void push_back_impl(const plComputableObject & o); 
 protected :
  SPL_ACCESS2(plComputableObjectList);
  friend class plComputableObject;
  friend class plJointDistribution;
  friend class plJunctionTree;
  friend class plJunctionTree_impl;

  typedef std::vector <kplComputableObject *> kplCOL;
  kplCOL internal_list_;

  /* Gets the internal list of objects */
  const  plComputableObjectList::kplCOL & get_internal_list() const;
  
  /* sets the internal list of objects*/
  void set_internal_list(const plComputableObjectList::kplCOL  & list); 

  explicit plComputableObjectList(const std::vector<kplComputableObject*> &);
};


/**********************************************************************
                        plComputableObject
**********************************************************************/

SPL_DECL2(plComputableObject);
/**A @em  computable @em object on \f$ \omega \f$ is defined as an abstract object
   provided with a probability measure function @em compute(\f$ \omega \f$).  
**/
class PL_DLL_API plComputableObject : public plObject
{
 protected:

  // Friend classes and functions
  SPL_ACCESS2(plComputableObject);
  friend class plJointDistribution;
  friend class plBayesianFilter;
  friend class plComputableObjectList;
  friend class plDistributionTable;
  friend class plDistribution;
  friend class plCndDistribution;
  friend class plJunctionTree_impl;
  friend class kplJtBayesianNetwork;
  friend class plParticleFilter;
  friend void set_result_root_from_compilation(plDistribution &result, kplKernel *compiled);
  friend void set_result_root_from_instantiation(plDistribution &result, kplKernel *instantiation);
  template<typename T> friend class  plMutable;
  // needs to build from kplComputableObject*
  friend class plComputableObjectList_const_iterator;
  template<class T> friend class plCndLearnObject;
  friend class plLearnDistributionTable;

  plSignature signature_;
  mutable plDataValues head_variables_data_values_;
  mutable plDataValues known_variables_data_values_;
  mutable plDataValues searched_variables_data_values_;

  kplComputableObject* root_distribution_;

  /* Computes the value of the function, according to a list of
     parameters at kernel level.*/
  plProbValue kernel_compute(const plDataValues &params) const;

  plFloat kernel_compute_log(const plDataValues &params) const;

  /* Renames the variables of a function at kernel level */
  void kernel_rename(const std::map <kplVariable*, kplVariable *> &rename_map);
  
  /* Sets the head, left and right variable lists (conditional distribution).

     @param[in] left_vars Left variables. Must not be empty.
     @param[in] right_vars Right variables. Must not be empty.
  */
  void set_variables(const kplVariableList &left_vars, 
                     const kplVariableList &right_vars);

  /* Sets the head, left and right variable lists (conditional distribution).

     @param[in] left_vars Left variables. Must not be empty.
     @param[in] right_vars Right variables. Must not be empty.
  */
  void set_variables(plVariablesConjunction const & left_vars, 
                     plVariablesConjunction const & right_vars) {
    set_variables(left_vars.get_variable_list(), right_vars.get_variable_list());
  }
  void set_variables(plVariablesConjunction const & left_vars, 
                     kplVariableList const & right_vars) {
    set_variables(left_vars.get_variable_list(), right_vars);
  }
  void set_variables(kplVariableList const & left_vars, 
                     plVariablesConjunction const & right_vars) {
    set_variables(left_vars, right_vars.get_variable_list());
  }

  /* Sets the head, left and right variable lists. Assumes that the list of
     right variables is empty (non-conditional distribution).

     @param[in] left_vars Left variables. Must not be empty.
  */
  void set_variables(const kplVariableList &left_vars);

  /* Sets the head, left and right variable lists. Assumes that the list of
     right variables is empty (non-conditional distribution).

     @param[in] left_vars Left variables. Must not be empty.
  */
  void set_variables(plVariablesConjunction const & left_vars) {
    set_variables(left_vars.get_variable_list());
  }

  void set_signature( const plSignature &sig) 
  {
    signature_ = sig;
  }
  
  const kplVariableList &get_head_variables() const;
  const kplVariableList &get_search_variables() const;
  const kplVariableList &get_known_variables() const;

  /* Returns the pointer to itself if the distribution is not a @em builtin @em distribution
     @em (function). Otherwise, it returns a pointer to the distribution at kernel
     level */
  kplComputableObject * get_root_distribution() const;

  /* Sets the distribution at kernel level to @em new_distribution. The previous one
     is automatically dereferenced if needed. */
  void set_root_distribution(kplComputableObject* new_distribution);

  /* Deallocate the KPL object pointed-to by member root_distribution. */
  void dispose_root_distribution();

  /* Reconstruct from a KPL object. */
  explicit plComputableObject(kplComputableObject*);

  virtual bool is_base_class()const
  {
    return true;
  }

  static plComputableObject as_builtin(const plComputableObject &co);

  void create_kernel(const std::vector <kplComputableObject*> &funcs,
                     const kplVariableList &thisLeft, 
                     const kplVariableList &thisRight);

 public:

  /** Empty constructor. The resulting object is invalid for most operations,
      except for being assigned to, or used as a return value. */
  plComputableObject();


  /** Creates a conditional or non conditional distribution as a product of a
      conditional and non conditional distributions.

      The left variables of the constructed computable object are the concatenation
      (in the same order) of the left variables of all the terms of the list.
      Its right variables are the concatenation of the right variables of
      all the terms and that are not in left one above.
  */
  plComputableObject(const plComputableObjectList &func_lis);

  /* Creates a conditional or non conditional distribution as a product of a
     conditional and non conditional distributions. This constructor
     explicits the left and right variables of the constructed distribution.
  */
  plComputableObject(const plComputableObjectList &func_lis,
                     const plVariablesConjunction &left_variables,
                     const plVariablesConjunction &right_variables);
  
  /** Destroys the function */
  virtual ~plComputableObject();

  /** Copy constructor */
  plComputableObject(const plComputableObject&);

  /** Assignment operator */
  plComputableObject& operator=(const plComputableObject&);

  /** Same as operator=() */
  plComputableObject &assign_from(const plComputableObject &other)
  {
    return operator=(other);
  }

  /** Return the distribution print name */
  std::string get_print_name() const;
  
  /** Returns @em true if the plComputableObject is of type @em type */
  bool is(const plComputableObjectType& type) const;

  /** Returns @em true if the plComputableObjectType is at least one
      of the types described by @em prototype_signature */
  bool is_some_on_signature(const plSignature& prototype_signature) const;

  /** Returns true if the distribution really is conditional. */
  bool is_conditional() const;

  /** Return true if the object is default-constructed (as opposed to
      full-initialized). */
  bool is_empty() const { return (0==root_distribution_); }

  /** Get the object full signature (e.g its actual type and parent classes) */
  const plSignature& get_signature() const;

  /** Get the object actual (most derived) type. \sa get_signature() */
  plComputableObjectType get_computable_object_type() const;

  /** Returns all the variables (left and right) of the computable object.
      This is equivalent to get_left_variables() ^ get_right_variables().
  */
  plVariablesConjunction get_variables() const;

  /** Returns the left variables of the computable object. For instance, for
      a distribution representing P(A B | C D), this would return A B.
  */
  plVariablesConjunction get_left_variables() const;

  /** Returns the right variables of the computable
      object. For instance, for a conditional distribution representing P(A B | C D),
      this would return C D. This returns an empty plVariablesConjunction for
      a non-conditional distribution.
  */
  plVariablesConjunction get_right_variables() const;


  /** Computes the value of the function for the input parameter
      @em values.

      ATTENTION: the returned value is guaranteed to be NORMALIZED for and only for: 
      - builtin distributions (basic distributions provided directly by ProBT), 
      - compiled distributions (obtained by using the compile() method).
      - distributions obtained with @em instantiate called with parameter 
      @em ensure_normalization_on_compute = @em true.

      Calling the 'compute' method on distributions resulting from the ask
      and/or instantiate methods is not guaranteed to return a
      normalized value unless the instantiate method is called with
      parameter @em ensure_normalization_on_compute = @em true. To be
      sure to obtain normalized values, consider compiling these
      distributions using the 'compile' method.

      However, this method is useful if you just want to compare the relative magnitudes
      of several probability values.
  */
  plProbValue compute(const plValues &values)const;
  
  /** Computes the logarithm value of the function for the input parameter
      @em values.
  */
  plFloat compute_log(const plValues &values)const;
  

  /** Computes the value of the function for the array input parameter
      @em values.  ATTENTION: the returned value is guaranteed to be
      NORMALIZED for and only for: 
      - builtin distributions (basics
      distributions provided by ProBT), 
      - compiled distributions (obtained by using the @em compile method).
      - distributions obtained with @em instantiate called with parameter 
      @em ensure_normalization_on_compute = @em true.
      
      Calling the @em compute method on distributions resulting from the ask
      and/or instantiate methods is not guaranteed to return a
      normalized value unless the instantiate method is called with
      parameter @em ensure_normalization_on_compute = true. To be sure
      to obtain normalized values, consider compiling these distributions
      using the 'compile' method.
  */
  plProbValue compute(const int *values)const;

  /** Computes the value of the function for the array input parameter
      @em values.  ATTENTION: the returned value is guaranteed to be
      NORMALIZED for and only for: 
      - builtin distributions (basics
      distributions provided by ProBT), 
      - compiled distributions (obtained by using the @em compile method).
      - distributions obtained with @em instantiate called with parameter 
      @em ensure_normalization_on_compute = @em true.
      
      Calling the @em compute method on distributions resulting from the ask
      and/or instantiate methods is not guaranteed to return a
      normalized value unless the instantiate method is called with
      parameter @em ensure_normalization_on_compute = true. To be sure
      to obtain normalized values, consider compiling these distributions
      using the 'compile' method.
  */
  plProbValue compute(const unsigned int *values)const;

  /** Computes the value of the function for the array input parameter
      @em values.  ATTENTION: the returned value is guaranteed to be
      NORMALIZED for and only for: 
      - builtin distributions (basics
      distributions provided by ProBT), 
      - compiled distributions (obtained by using the @em compile method).
      - distributions obtained with @em instantiate called with parameter 
      @em ensure_normalization_on_compute = @em true.
      
      Calling the @em compute method on distributions resulting from the ask
      and/or instantiate methods is not guaranteed to return a
      normalized value unless the instantiate method is called with
      parameter @em ensure_normalization_on_compute = true. To be sure
      to obtain normalized values, consider compiling these distributions
      using the 'compile' method.
  */
  plProbValue compute(const float *values)const;

  /** Computes the value of the function for the array input parameter
      @em values.  ATTENTION: the returned value is guaranteed to be
      NORMALIZED for and only for: 
      - builtin distributions (basics
      distributions provided by ProBT), 
      - compiled distributions (obtained by using the @em compile method).
      - distributions obtained with @em instantiate called with parameter 
      @em ensure_normalization_on_compute = @em true.
      
      Calling the @em compute method on distributions resulting from the ask
      and/or instantiate methods is not guaranteed to return a
      normalized value unless the instantiate method is called with
      parameter @em ensure_normalization_on_compute = true. To be sure
      to obtain normalized values, consider compiling these distributions
      using the 'compile' method.
  */
  plProbValue compute(const double *values)const;

  /** Computes the logarithm value of the function for the input parameter
      @em values.
  */
  plFloat compute_log(const int *values)const;

  /** Computes the logarithm value of the function for the input parameter
      @em values.
  */
  plFloat compute_log(const unsigned int *values)const;

  /** Computes the logarithm value of the function for the input parameter
      @em values.
  */
  plFloat compute_log(const float *values)const;

  /** Computes the logarithm value of the function for the input parameter
      @em values.
  */
  plFloat compute_log(const double *values)const;


  /** Computes the value of the function for the STL vector input parameter
      'values'.  ATTENTION: the returned value is guaranteed to be
      NORMALIZED for and only for: 
      - builtin distributions (basics
      distributions provided by ProBT), 
      - the compiled distributions (obtained by using the @em compile method).
      - distributions obtained with @em instantiate called with parameter 
      @em ensure_normalization_on_compute = true.

      Calling the 'compute' method on distributions resulting from the ask
      and/or instantiate methods is not guaranteed to return a
      normalized value unless the instantiate method is called with
      parameter @em ensure_normalization_on_compute = @em true. To be
      sure to obtain normalized values, consider compiling these
      distributions using the @em compile method.
  */
  plProbValue compute(const std::vector <int> &parameter)const;

  /** Computes the value of the function for the STL vector input parameter
      'values'.  ATTENTION: the returned value is guaranteed to be
      NORMALIZED for and only for: 
      - builtin distributions (basics
      distributions provided by ProBT), 
      - the compiled distributions (obtained by using the @em compile method).
      - distributions obtained with @em instantiate called with parameter 
      @em ensure_normalization_on_compute = true.

      Calling the 'compute' method on distributions resulting from the ask
      and/or instantiate methods is not guaranteed to return a
      normalized value unless the instantiate method is called with
      parameter @em ensure_normalization_on_compute = @em true. To be
      sure to obtain normalized values, consider compiling these
      distributions using the @em compile method.
  */
  plProbValue compute(const std::vector <unsigned int> &parameter)const;

  /** Computes the value of the function for the STL vector input parameter
      'values'.  ATTENTION: the returned value is guaranteed to be
      NORMALIZED for and only for: 
      - builtin distributions (basics
      distributions provided by ProBT), 
      - the compiled distributions (obtained by using the @em compile method).
      - distributions obtained with @em instantiate called with parameter 
      @em ensure_normalization_on_compute = true.

      Calling the 'compute' method on distributions resulting from the ask
      and/or instantiate methods is not guaranteed to return a
      normalized value unless the instantiate method is called with
      parameter @em ensure_normalization_on_compute = @em true. To be
      sure to obtain normalized values, consider compiling these
      distributions using the @em compile method.
  */
  plProbValue compute(const std::vector <float> &parameter)const;

  /** Computes the value of the function for the STL vector input parameter
      'values'.  ATTENTION: the returned value is guaranteed to be
      NORMALIZED for and only for: 
      - builtin distributions (basics
      distributions provided by ProBT), 
      - the compiled distributions (obtained by using the @em compile method).
      - distributions obtained with @em instantiate called with parameter 
      @em ensure_normalization_on_compute = true.

      Calling the 'compute' method on distributions resulting from the ask
      and/or instantiate methods is not guaranteed to return a
      normalized value unless the instantiate method is called with
      parameter @em ensure_normalization_on_compute = @em true. To be
      sure to obtain normalized values, consider compiling these
      distributions using the @em compile method.
  */
  plProbValue compute(const std::vector <double> &parameter)const;

  /** Computes the logarithm value of the function for the input parameter
      @em values.
  */
  plFloat compute_log(const std::vector <int> &parameter)const;

  /** Computes the logarithm value of the function for the input parameter
      @em values.
  */
  plFloat compute_log(const std::vector <unsigned int> &parameter)const;

  /** Computes the logarithm value of the function for the input parameter
      @em values.
  */
  plFloat compute_log(const std::vector <float> &parameter)const;

  /** Computes the logarithm value of the function for the input parameter
      @em values.
  */
  plFloat compute_log(const std::vector <double> &parameter)const;

  /** Renames the head variables of a function and returns a
      reference to it .*/
  plComputableObject& rename(const plVariablesConjunction &new_variables);

  /** Writes the the distribution head at the output stream @em out */
  std::ostream& write_head(std::ostream &out=std::cout) const;

  /** Get the distribution head as a string */
  std::string get_head_string() const;

  /** Writes the distribution body at the output stream @em out */
  std::ostream& write_body(std::ostream &out=std::cout) const;

  /** Get the distribution body as a string */
  std::string get_body_string() const;


  /** Concatenation with another computable object operator */
  plComputableObjectList operator*(const plComputableObject &f2)const ;

   /** Concatenation with another computable object operator */
  plComputableObjectList concatenate(const plComputableObject &f2)const
  {
    return this->operator*(f2);
  }

  /** Concatenation with a computable objects list operator */
  plComputableObjectList operator*(const plComputableObjectList &v_list)const;
  
  /** Concatenation with a computable objects list operator */
  plComputableObjectList concatenate(const plComputableObjectList &v_list)const
  {
    return this->operator*(v_list);
  }

  /** Replace the referenced  distribution P(left_vars |
      right_vars) by another  distribution @em
      new_co. @em new_cnd_distribution must have the same left and
      right variables.

      This method is intended  to be used especially on non-compiled
      computable objects such as:
      
      - Expressions obtained using the @em ask method (see
      @em plJointDistribution class) and non-conditional expressions
      obtained using the @em instantiate method on the conditional
      expressions above.
      
      - Joint distribution objects (@em plJointDistribution).

      Using @em replace affects only the computable object on which is
      called. To get a global effect, use @em mutate instead.

      \sa plMutableComputableObject::mutate
  */
  void replace(const plVariablesConjunction &left_vars, 
               const plVariablesConjunction &right_vars, 
               const plComputableObject &new_co);
  
  /** Replace the distribution P(left_vars ) by another distribution
      @em new_distribution. @em new_distribution must have the same left
      variables.
      
      This method is intended  to be used especially on non-compiled
      computable objects such as:
      
      - Expressions obtained using the @em ask method (see
      @em plJointDistribution class) and non-conditional expressions
      obtained using the @em instantiate method on the conditional
      expressions above.
      
      - Joint distribution objects (@em plJointDistribution).
      
      Using @em replace affects only the computable object on which is
      called. To get a global effect, use @em mutate instead.  

      \sa plMutableComputableObject::mutate
  */
  void replace(const plVariablesConjunction &left_vars, 
               const plDistribution &new_distribution);

  /** Return @em true if this and the other object are the same. */
  bool operator==(const plComputableObject& other) const;

  /** Return @em true if this and the other object are the same. */
  bool equal(const plComputableObject& other) const
  {
    return this->operator==(other);
  }

  /** Return @em true if this and the other object are not the same. */
  inline bool operator!=(const plComputableObject& other) const
  { return ! this->operator==(other); }

  /** Return @em true if this and the other object are not the same. */
  bool not_equal(const plComputableObject& other) const
  {
    return this->operator!=(other);
  }

  /** Returns @em true if and only if the computable object is a ProBT builtin one.
   */
  bool is_builtin()const;
  
  
  /** Displays the evaluation tree corresponding to the computable object.
      \sa get_exhaustive_compilation_complexity get_exhaustive_update_complexity
  */
  std::ostream &print_evaluation_tree_infos(std::ostream &out)const;

  
  /** Get the sum of the number of sums and the number of products required to
      compile the corresponding expression.
      \sa get_exhaustive_update_complexity print_evaluation_tree_infos
  */
  long double get_exhaustive_compilation_complexity() const;

  /** Get the sum of the number of sums and the number of products required to
      update the corresponding expression for a new given evidence value.
      
      \sa get_exhaustive_compilation_complexity print_evaluation_tree_infos
  */
  long double get_exhaustive_update_complexity() const;

  /** Get the mutability property.
      \return 'true' if the computable object is mutable.
  */
  bool is_mutable()const;


  /** Tabulates the computable object in the 'output' vector.  The value \f$
      P(X) \f$ is computed for each possible value of the \f$ X \f$ variables
      and stored in the output vector 'output. Calling this method on
      computable objects with non-discretized continuous variables throws a
      plError(27) exception.
      
      In multi-dimensional cases \f$ X = X_1 X_2 \f$, where \f$ X_1 \f$ and \f$
      X_2 \f$ can take, for example, \f$ 1 \cdots n_1 \f$ and \f$ 1 \cdots n_2
      \f$ values respectively, it constructs the probability table on \f$
      P(X)\f$ as follows:\n
    
      - output[0] = \f$ P([X_1=1, X_2=1]) \f$  \n 
      - output[1] = \f$ P([X_1=1, X_2=2]) \f$  \n
      . \n 
      . \n 
      . \n 
      - output[\f$ n_2-1\f$] = \f$ P([X_1=1, X_2=n_2]) \f$ \n\n 
      - output[\f$ n_2   \f$] = \f$ P([X_1=2, X_2=1])   \f$ \n 
      - output[\f$ n_2+1 \f$] = \f$ P([X_1=2, X_2=2])   \f$ \n 
      . \n 
      . \n 
      . \n 
      - output[\f$ 2 n_2 - 1 \f$] = \f$ P([X_1=2, X_2=n_2]) \f$ \n 
      . \n 
      . \n 
      . \n 
      - output[\f$ n_1 n_2-1 \f$] = \f$ P([X_1=n_1, X_2=n_2]) \f$ \n

      The same tabulation scheme is used for conditional distributions 
      (eg., \f$ P(X_1 X_2 | Y_1 Y_2)\f$).
  */
  void tabulate(std::vector <plProbValue> &output)const;

  /** Same as tabulate(vector <plProbValue> &output) but using limited 
      sampling iterations \em n */
  void n_tabulate(std::vector <plProbValue> &output, long unsigned int n) const;

  /** Same as tabulate(vector <plProbValue> &output) but using limited 
      sampling time \em time_in_seconds */
  void time_tabulate(std::vector <plProbValue> &output, double time_in_seconds) const;

  /** Get the independant factors of a product computable object */
  plComputableObjectList get_factors()const;

  /** Get expression children */
  plComputableObjectList get_children() const;

  /** Get expression marginalized variables */
  plVariablesConjunction get_marginalized_variables() const;

 protected:
  /** Writes the object at the output given by "out" */
  virtual void Output(std::ostream &out) const;

 private:
  template <typename T>
    plProbValue compute_ptr_impl(T *values)const;
  
  template <typename T>
    plFloat compute_log_ptr_impl(T *values)const;

  template <typename T>
    plProbValue compute_vector_impl(const std::vector <T> &parameter)const;

  template <typename T>
    plFloat compute_log_vector_impl(const std::vector <T> &parameter)const;


};


/** Type for mutable plComputableObject */
typedef plMutable<plComputableObject> plMutableComputableObject;

/** Type for mutable plDistribution */
typedef plMutable<plDistribution> plMutableDistribution;

/** Type for mutable plCndDistribution */
typedef plMutable<plCndDistribution> plMutableCndDistribution;




#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
