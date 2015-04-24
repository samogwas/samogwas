/*=============================================================================
 * Product        : ProBT 
 * File           : plVariablesConjunction.h
 * Author         : Juan-Manuel Ahuactzin
 * Creation       : Wed Apr 10 10:59:32 2002
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


#ifndef plVariablesConjunction_h
#define plVariablesConjunction_h

#include "plConfig.h"

#include <list>
#include <vector>
#include <memory>
#include <set>
#include <cstddef>

#include <plObject.h>
#include <plMath.h>
#include <plVariableType.h>
#include <plType.h>

/*****************************************************************************
 *                           External Classes
 *****************************************************************************/
class kplVariable;
typedef std::list<kplVariable*> kplVariableList;
class plSymbol;

/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/

class plVariablesConjunction;

#if !defined(SWIG) || !defined(SWIGPYTHON)
/*
	\em plVariablesConjunction const iterator.
 */
class PL_DLL_API plVariablesConjunction_const_iterator
{
public:
    typedef plVariablesConjunction_const_iterator  Self_;
    typedef kplVariableList::const_iterator node_type;

    typedef ptrdiff_t                     difference_type;
    typedef std::bidirectional_iterator_tag    iterator_category;
    typedef plSymbol                      value_type;

    plVariablesConjunction_const_iterator()
	: node_() { };

    plVariablesConjunction_const_iterator(node_type node)
	: node_(node) { };

    virtual ~plVariablesConjunction_const_iterator();

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

//=============================================================================
SPL_DECL2(plVariablesConjunction);

typedef std::set<kplVariable const *> kplVariableSet;
typedef std::vector<kplVariable *> kplModVariableVector;

/** The \em plVariablesConjunction class implements the conjunction of a
    set of variables. */
class PL_DLL_API plVariablesConjunction : public plSampleSpaceObject
{
protected:
  /** Size of the variable list (variable_list), faster that the acces 
      to size() function */
  size_t Dsize_;

  /** List of the function arguments variables */
  kplVariableList variable_list_;

  /** Mirrors variable_list_. variable_set_ may be empty, use variable_set()
      to make sure it is valid and get it. Used to speed up operator^=().
  */
  mutable kplVariableSet variable_set_;
  kplVariableSet & variable_set() const;

  /** Mirrors variable_list_. variable_vector_ may be empty, use variable_vector()
      to make sure it is valid and get it. Used to speed up operator[](size_t).
   */
  mutable kplModVariableVector variable_vector_;
  kplModVariableVector & variable_vector() const;

  /** Constructor using a  list of kplVariables */
  explicit plVariablesConjunction(const kplVariableList &kernel_variable_list);

  /** Constructor using one kplVariable */
  explicit plVariablesConjunction(kplVariable* kernel_variable);

  /** clean up the variable list */
  void cleanup();

  void set(const kplVariableList &kernel_variable_list);

  /** Writes the variable at the output given by "out" */
  virtual void Output(std::ostream &out) const;  

  static const plVariablesConjunction empty_;
  
public:

  /** Needed by the Python bindings, but not part of the user API.  Please do
   not use. */
  inline const kplVariableList& get_variable_list() const
  { return variable_list_; }

  /** Default constructor, creates an empty variable conjuction */
  plVariablesConjunction();

  /** Destructor, if the variable represents a list erase the list but
      not the variables on the list. If empty variable, erase the
      empty variable */
  virtual ~plVariablesConjunction();
  
  /** Return the concatenation of the variable \em var (at right side of the
      operator) with the variable at left side. If a variable is found twice,
      only the first occurrence will be kept and a warning 62 will be raised.
  */
  plVariablesConjunction operator^(const plVariablesConjunction &variable) const;

  /** Return the concatenation of the variable \em var (at right side of the
      operator) with the variable at left side. If a variable is found twice,
      only the first occurrence will be kept and a warning 62 will be raised.
  */
  plVariablesConjunction concatenate(const plVariablesConjunction &variable) const
  {
    return operator^(variable);
  }
  
  /** Returns the number of values taken by the variable
      conjunction. For example, if a variable 'x' takes 10 possible
      values and another variable 'y' takes 3 possible values, then
      the conjunction x^y takes 30 (10 x 3) possible values.  It
      returns zero if the variable is not discretized (ie,
      non-discretized plRealType)
  */
  long double cardinality()const;

  /** Returns true if the conjunction contains only discretizable symbols
      (ie, all types excluding non-discretized plRealType), false
      otherwise. */
  bool is_discretized() const;

  /** Returns true if the conjunction contains only discrete symbols, false otherwise. */
  bool is_discrete() const;

  /** Returns true if the conjunction contains only continuous symbols
      (ie, plRealType or plContinuousIntervalType), false otherwise. */
  bool is_continuous() const;

  /** Returns the number of symbols that compose the conjunction */
  inline size_t dim() const { return Dsize_; }

  /** Returns the number of symbols that compose the conjunction */
  inline size_t size() const { return Dsize_; }

  /** Returns true if the conjunction is empty. */
  inline bool is_empty() const { return (Dsize_==0); }
  
  /** Returns the variable's type as a 'plVariableType' value, where
      plVariableType is defined as an enum:   
      enum {PL_INTEGER, PL_REAL, PL_CONINUOUS_INTERVAL, 
      PL_DISCRETE_INTERVAL, PL_LABEL, PL_UNKNOWN_TYPE}
      
      In the case of a multi-dimensional conjunction, the type of the
      first variable is returned.
  */
  plVariableType get_var_type() const;

  /** Returns the variable's type as a 'plType' object. In the case of a
      conjunction, the type of the first variable is returned. The returned
      object can in turn be converted into the appropriate derived class
      (plIntegerType, plRealType, plContinuousIntervalType, ...) so you can
      use its specialized methods and query its characteristics. In order to
      know into which class the object has to be converted, use the
      get_var_type() method.
  */
  plType get_type() const;

  /** Gets the variable name, given by the user.
      In the case of a conjunction, the type of the first variable is returned. 
  */
  virtual std::string name() const;

  /** Get the names of all the variables in the conjunction. */
  std::vector<std::string> get_names() const;

  /** Associated const_iterator. **/
  typedef plVariablesConjunction_const_iterator const_iterator;

  /** 
      Returns a const_iterator to the first variable in this conjunction.
  */
  const_iterator begin() const
  { return const_iterator(variable_list_.begin()); }

  /** 
      Returns a const_iterator pointing one item after the last variable
      in this conjunction.
   */
  const_iterator end() const
  { return const_iterator(variable_list_.end()); }

  /** 
      Tests if this conjunction is a superset of 'other'.
      (a ^ b).contains(a) == true
      (a ^ b).contains(b) == true
      (a ^ b).contains(c) == false
      (a ^ b ^ c).contains(a ^ b) == true
      (a ^ b ^ c).contains(a ^ b ^ d) == false
   */
  bool contains(const plVariablesConjunction& other) const;
  
  /**
     Tests if this conjunction has the same symbols as 'other'.
     (a ^ b).has_same_symbols_as(b ^ a) == true;
     (a ^ b).has_same_symbols_as(b ^ a ^ c) == false;
     (a ^ b).has_same_symbols_as(b) == false;
  */
  bool has_same_symbols_as(const plVariablesConjunction& other)const;

  /** Assignment operator */
  plVariablesConjunction& operator=(const plVariablesConjunction& other);

  /** Same as operator=() */
  plVariablesConjunction& assign_from(const plVariablesConjunction& other)
  {
    return operator=(other);
  }

  /** Copy constructor*/
  plVariablesConjunction(const plVariablesConjunction& other);

  /** Convert the plVariablesConjunction into a vector of plSymbol objects. */
  void get_all_symbols(std::vector<plSymbol> &symbols) const;
  /** Same as get_all_symbols() but returns the vector by value */
  std::vector<plSymbol> as_symbol_vector() const;

  /** Get the n^th symbol of the conjunction. */
  plSymbol operator[](size_t n) const;

  /** Get the n^th symbol of the conjunction. */
  plSymbol at(size_t n) const;

  /** Return true if this and the other object are the same. */
  bool operator==(const plVariablesConjunction& other) const;

  /** Return true if this and the other object are the same. */
  bool equal(const plVariablesConjunction& other) const
  {
    return operator==(other);
  }


  /** Return true if this and the other object are not the same. */
  inline bool operator!=(const plVariablesConjunction& other) const
  { return ! this->operator==(other); }

  /** Return true if this and the other object are not the same. */
  bool not_equal(const plVariablesConjunction& other) const
  {
    return operator!=(other);
  }

  /** Return the empty variable conjunction. */
  static const plVariablesConjunction& empty();

  /** Return the position of a given symbol in the conjunction.
      Return -1 if the symbol is not found.
  */
  int get_symbol_position(const plSymbol&s)const;
  
  /**  In place concatenation of the variable \em var (at right side of the
       operator) with the variable at left side.  If a variable is found twice,
      only the first occurrence will be kept and a warning 62 will be raised.

      WARNING: This operator has effect only on base plVariablesConjunction
      objects. For all subclasses this operator does not have any effect
      (e.g. You can't use a plSymbol at the left side of the operator).
  */
  plVariablesConjunction &operator^=(const plVariablesConjunction &var);

  /**  In place concatenation of the variable \em var (at right side of the
       operator) with the variable at left side.  If a variable is found twice,
       only the first occurrence will be kept and a warning 62 will be raised.
       
      WARNING: This operator has effect only on base plVariablesConjunction
      objects. For all subclasses this operator does not have any effect
      (e.g. You can't use a plSymbol at the left side of the operator).
  */
  plVariablesConjunction &push_back(const plVariablesConjunction &var)
  {
    return operator^=(var);
  }

  /** Remove one symbol from the variable conjunction and return a reference to the conjunction */
  plVariablesConjunction& remove(const plSymbol& symbol);


  /** Clear the conjunction */
  void clear()
  {
    cleanup();
  }

  /** Returns the first symbol having @em name as print name. 
      Raises an exception if no symbol with this print name is found.
  */
  plSymbol get_symbol_with_name(const std::string &name)const;

  /** Difference operator */
  plVariablesConjunction operator-(const plVariablesConjunction& other)const;

  /** Difference */
  plVariablesConjunction subtract(const plVariablesConjunction& other)const
  {
    return operator-(other);
  }

  /** Get a sorted-by-name copy of the conjunction */
  plVariablesConjunction sort_by_name() const;

  /** Get the discrete variables of the conjunction */
  plVariablesConjunction  get_discrete_variables() const;

  /** "Less than" operator */
  bool operator<(const plVariablesConjunction& other) const;

  /** "Less than"  */
  bool less(const plVariablesConjunction& other) const
  {
    return operator<(other);
  }
  
  friend class plData;
  friend class plArray;
  friend class plCndDistribution;
  friend class plCndNormal;
  friend class plCndLogNormal;
  friend class plCndUnknown;
  friend class plComputableObject;
  friend class plDeterministic;
  friend class plCndDeterministic;
  friend class plGamma; 
  friend class plJointDistribution;
  friend class plKalmanFilter;
  friend class plDistribution;
  friend class plDistributionTable;
  friend class plNormal;
  friend class plPoisson;
  friend class plProbTable;
  friend class plSymbol;
  friend class plUniform;
  friend class plUnknown;
  friend class plValues;
  friend class plLogNormal;
  friend class plVariablesConjunction_const_iterator;
  friend class plExponential;
  friend class plBeta;
  friend class plBinomial;
  friend class plExternalFunction;
  friend class plExternalProbFunction;
  friend class plVariableIndexer;
  friend class plLearnSparseHistogram;
  friend class plLearnObject;
  friend class plLearnDistribVector;
  friend class plDirichlet;
  friend class plBayesLearnProbTable;
  friend class plLearnNdNormal;
  friend class plCndAnonymousDistribution;
  friend class plBayesianFilter;
  friend class plAnonymousDistribution;
  friend class plIneqConstraint;
  friend class plExternalFunctionFromC;
  friend class plExternalProbFunctionFromC;
  friend class plLearnHistogram;
  friend class plHMM;
  friend class plCndUniform; 
  friend class plCndPoisson;
  friend class plCndBinomial;
  friend class plJtDistribution;
  friend class plCndJtDistribution;
  friend class plJunctionTree_impl;
  friend class plParticleFilter;
  friend class plLearnDistributionTable;
  friend class plEMLearner;
  friend class plWeibull;
  friend class plComputableObjectList;
  friend class plCndBeta;
  friend class plCndExponential;
  friend class plCndGamma;
  friend class plCndWeibull;
  friend class plVonMises;
  friend class plCndVonMises;

  template <class T> friend class plCndLearnObject;

  SPL_ACCESS2(plVariablesConjunction);
};


#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
