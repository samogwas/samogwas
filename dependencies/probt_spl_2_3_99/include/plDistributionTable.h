/*=============================================================================
 * Product        : ProBT 
 * File           : plDistributionTable.h
 * Author         : Emmanuel Mazer and Juan-Manuel Ahuactzin
 * Creation       : Mon Jan  7 15:27:26 2002
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


#ifndef plDistributionTable_h
#define plDistributionTable_h

#include "plConfig.h"

#include <plCndDistribution.h>
#include <plProbTable.h>
#include <plVariablesConjunction.h>

class kplKernelTable;

/*****************************************************************************
 *                           External Classes
 *****************************************************************************/
class kplVariable;


/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/


/**  A plDistributionTable is a way to define a conditional distribution from a
     set of Computable Objects (Distributions and/or Conditional Distributions)
     having the same left variables than the building blocks. It can be use to
     switch from one distribution to another according to the value of a subset
     of the right variables.  This subset of variables have to be discrete or
     discretized.  The set of Computable objects is stored in a table.
*/
class PL_DLL_API plDistributionTable : public plCndDistribution
{ 
 private:
  
  void check_variables(const plVariablesConjunction& left,
		       const plVariablesConjunction& right,
		       const plVariablesConjunction& right_index)const;
  
  void fill_using_table(const plProbValue *table,
                        bool already_normalized);

  void fill_using_function(plProbValue (*f) (const plValues &left_values,
                                             const plValues& right_values),
                           bool already_normalized);

  void initialize(const plVariablesConjunction& left,
                  const plVariablesConjunction& right,
                  const plVariablesConjunction& index,
                  bool force_use_map);

  template <typename T>
    void push_template(const plComputableObject &compObj, T value);

  void copy_data(const kplKernelTable* table, plDataValues &index_values, const plValues & values)const;

  template<typename T>
    void copy_data(const kplKernelTable* table, plDataValues &index_values, const T &val)const;

 public:

  /** Default constructor */
  plDistributionTable();

  /** A plDistributionTable indexed by the set of variables \em right.
      \param left  a set of left variables
      \param right a set of right variables used to select the appropriate distribution
      \param force_use_map if set to true, the internal storage will use a map instead of vector. 
      If set to false (the default) the storage will use a vector unless the index variables are continuous or too large
  */
  plDistributionTable (const plVariablesConjunction& left,
		       const plVariablesConjunction& right,
                       bool force_use_map = false);

  
  /** Construct a plDistributionTable conditional distribution.
      \param left  a set of left variables
      \param right a set of right variables
      \param right_index a subset of the right variables used to select the appropriate distribution
      \param force_use_map if set to true, the internal storage will use a map instead of vector. 
      If set to false (the default) the storage will use a vector unless the index variables are continuous or too large
  */
  plDistributionTable (const plVariablesConjunction& left,
		       const plVariablesConjunction& right,
		       const plVariablesConjunction& right_index,
                       bool force_use_map = false);


  /** Construct a plDistributionTable with an array of plProbValues
      implicitly defining a set of plProbTables. ATTENTION: This
      constructor is reserved for discrete left variables. 
  */
  plDistributionTable (const plVariablesConjunction& left,
		       const plVariablesConjunction& right,
		       const plProbValue *table,
		       bool already_normalized = false);

  template<typename T>
    plDistributionTable (const plVariablesConjunction& left,
			 const plVariablesConjunction& right,
			 const std::vector <T> &table,
			 bool already_normalized = false)
    : plCndDistribution(left, right, PLDistributionTable)
  {
    initialize(left, right, right, false);

    unsigned int the_size = table.size();
    plProbValue *p_table = new plProbValue[the_size];
    for(unsigned int i=0; i<the_size; i++) {
      p_table[i] = table[i];
    }
    fill_using_table(p_table, already_normalized);
    
    delete [] p_table;
  }

  /**
     Constructs a fully tabulated discrete distribution table, with static 
     initialisation using a provided function.
     If you want dynamic use of externally provided data, use a
     plExternalFunction instead.\n
     If you're building P(A B | C D), f(left, right) should return
     the probability of P(A=left[A], B=left[B] | C=right[C],D=right[D]).
  */
  plDistributionTable(const plVariablesConjunction& left,
		      const plVariablesConjunction& right,
		      plProbValue (*f) (const plValues &left_values, 
					const plValues& right_values),
		      bool already_normalized = false);
  
  /**
     Constructs a fully tabulated discrete distribution table, with static 
     initialisation using a provided method.
     If you want dynamic use of externally provided data, use a
     plExternalFunction instead.\n
     If you're building P(A B | C D), obj->method(left, right) should return
     the probability of P(A=left[A], B=left[B] | C=right[C],D=right[D]).
  */
  template<typename T>
    plDistributionTable(const plVariablesConjunction& left,
			const plVariablesConjunction& right, 
			T *obj,
			plProbValue (T::*method) (const plValues &left_values,
						  const plValues& right_values),
			bool already_normalized = false)
    : plCndDistribution(left, right, PLDistributionTable)
  {
    initialize(left, right, right, false);
    
    // XXX this code is duplicated from fill_using_function
    // but I don't know how to fix it -- Ronan (it works fine though)
    
    plValues v_left(get_left_variables());
    plValues v_right(get_right_variables());
    
    std::vector<plProbValue> column_values;
    
    do {
      column_values.clear();
      
      v_left.reset();
      do {
        column_values.push_back((obj->*method)(v_left, v_right));
      } while (v_left.next());
      
      plProbTable column(left, column_values, already_normalized);
      
      push(column, v_right);
      
    } while (v_right.next());
  }
    
    
  /** Destructor of the Distribution Table */
  virtual ~plDistributionTable(){};
  
  
  /** Push is used to incrementaly construct the the condtional distribution.
      \param compObj : must have the same left variables than the 
      original Distribution Table.
      If compObj is a Conditional Distribution, then its right variables 
      must be a subset of the right variables of the Distribution Table.
      \param values : this values is relative to the set of variables used as
      index.
  */
  void push(const plComputableObject &compObj, 
	    const plValues & values); 

  void push(const plValues & values, const plComputableObject &compObj)
  {
    push(compObj, values);
  }
  
  /** Inserts a new distribution \em compObj with an specified integer key value
      \em value. */
  void push(const plComputableObject &compObj, int value); 

  void push(int value, const plComputableObject &compObj)
  {
    push(compObj, value);
  }

  /** Inserts a new distribution \em compObj with an specified float key value
      \em value. */
  void push(const plComputableObject &compObj, plFloat value); 

  void push(plFloat value, const plComputableObject &compObj)
  {
    push(compObj, value);
  }
  
  /** Makes a push of a plProbTable implicitly defined by @em probVector */
  void push(const plValues & values, 
	    const std::vector <plProbValue> &probVector); 

  /** Makes a push of a plProbTable implicitly defined by @em probVector */
  void push(const std::vector <plProbValue> &probVector, 
	    const plValues & values); 

  /** push_default is used to push a default distribution that will be used
      for cases that have never been pushed in the DistributionTable */
  void push_default(const plComputableObject &compObj); 

  /** Return true if a default computable object has been set. */
  bool has_default() const;

  /** Get back the default distribution. */
  plComputableObject get_default() const;

  /** Returns \em true if a computable object has been already pushed for the
      case \em value.  Returns \em false otherwise */
  bool find(const plValues & values) const;

  /** Promote from a plCndDistribution. If the object being copied is
      actually of the wrong type, an exception is raised. */
  explicit plDistributionTable(const plCndDistribution&);

  /** Get a previously pushed object. */
  plComputableObject get_pushed_distribution(const plValues& values) const;
  
  /** Get the subset of the right variables used as index. */
  plVariablesConjunction get_index_variables() const;

  virtual bool is_base_class()const
  {
    return false;
  }

  /** Return the values previously inserted in the distribution map. If you want
      to access the distributions corresponding to these values, then use
      instantiate(). */
  std::vector<plValues> get_pushed_values() const;

 protected:
  void check_pushed_object_variables(const plComputableObject &compObj)const;

  // internal function
  void push(unsigned int key, kplComputableObject* obj);

  friend class plLearnDistributionTable;
};

#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
