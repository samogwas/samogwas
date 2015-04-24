/*=============================================================================
 * Product        : ProBT 
 * File           : plLearnDistributionTable.h
 * Author         : Kamel Mekhnacha
 * Creation       : Thu Oct  2 13:45:57 2008
 *
 *=============================================================================
 *        (c) Copyright 2008, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */

#ifndef plLearnDistributionTable_h
#define plLearnDistributionTable_h
 
#include "plConfig.h"

#include <plLearnCndObject.h>
#include <plVariablesConjunction.h>
#include <plDistributionTable.h>
#include <plVariableIndexer.h>

class plNonCndLearnObject;


SPL_DECL1(plLearnDistributionTable);

/** This class implements conditional learning objects as a map. Learning a
    conditional distribution P(X | Y) consists in using a map of
    non-conditional learning objects. This map contains, for each possible
    value of Y a non-conditional learning object on X. These non-conditional
    learning objects are added using the plLearnDistributionTable::push and
    plLearnDistributionTable::push_default methods.

    \sa plCndLearnObject
*/
class PL_DLL_API plLearnDistributionTable : public plLearnCndObject
{
 public:
  /** Constructor */
  plLearnDistributionTable(const plVariablesConjunction& left_vars,
                           const plVariablesConjunction& right_vars);

  /** Constructor when using the same type for non contional learning
      objects for all the values of the right variables.  @em
      init_object will be used as initilization even after calling
      reset() function
  */
  plLearnDistributionTable(const plVariablesConjunction& left_vars,
                           const plVariablesConjunction& right_vars,
                           const plNonCndLearnObject &init_object);
  
  /** Default constructor to be used for serialisation */
  plLearnDistributionTable();

  /** Copy constructor */
  plLearnDistributionTable(const plLearnDistributionTable& other);

  /** Assignment */
  plLearnDistributionTable& operator=(const plLearnDistributionTable& other);
  
  /** Destructor */
  virtual ~plLearnDistributionTable();
  
  /** Inserts a new non-conditional learning object with a given right value.
      A clone of the passed @em learn_object is created and stored internally
      (i.e. with no side-effect on the passed @em learn_object).
  */
  void push(const plNonCndLearnObject& learn_object, const plValues &value);
  
  /** Inserts a new non-conditional learning object with a given right value.
      A clone of the passed @em learn_object is created and stored internally
      (i.e. with no side-effect on the passed @em learn_object).
  */
  void push(const plNonCndLearnObject& learn_object, int value);
  
  /** Inserts a new non-conditional learning object with a given right value.
      A clone of the passed @em learn_object is created and stored internally
      (i.e. with no side-effect on the passed @em learn_object).
  */
  void push(const plNonCndLearnObject& learn_object, plFloat value);
  
  /** Inserts a new non-conditional learning object as a default one for
      learning distributions that correspond to right values that have not been
      inserted using push(). A clone of the passed @em default_learn_object is
      created and stored internally (i.e. with no side-effect on the passed @em
      default_learn_object).
  */
  void push_default(const plNonCndLearnObject &default_learn_object);
  
  /** Resets learning to its initial state. */
  void reset();
  
  /** Creates the distribution map corresponding to the learnt conditional
      distribution. If no default learning object is provided
      using \em push_default, then the returned distribution map does not contain a
      default function for the right values that have never been
      inserted. Consider calling \em push_default on the returned distribution
      map in order to customize it.
  */
  plDistributionTable get_distribution_table()const;

  /** Constructs the corresponding learnt conditional
      distribution. It's equivalent to calling \em
      get_distribution_table but the returned distribution is read-only
      and thus is more efficient. Moreover, if no default learning
      object is inserted (no call to \em push_default), then the
      returned conditional distribution will contain a uniform
      distribution for the right values that have never been inserted.
  */
  plCndDistribution get_cnd_distribution()const;

  /** Returns, for each right value, the total weight of the inserted points. */
  void get_total_weights(std::vector <plFloat> &w)const;
  
  /** Returns, for each right value, the corresponding number of the inserted
      points.
  */
  void get_nsamples(std::vector <unsigned int> &n)const;

  
  /** Returns a pointer to the non conditional learning object corresponding to
      the @em right_value value of the right variables.  Returns 0 if @em
      right_value has never been seen.
  */
  const plNonCndLearnObject* get_learnt_object_for_value(const plValues &right_value)const;
  
  /** Returns in the output parameter \em params, for a given value \em right_value of the right variables,
      all learnt parameters. 
  */
  void get_parameters(plValues &params, const plValues &right_value) const;

  unsigned int get_n_parameters()const;

  virtual plLearnObject* clone()const
  {
    return new plLearnDistributionTable(*this);
  }


 protected:
  bool internal_addPoint(const plDataValues &point, plFloat weight = PL_ONE);
  bool internal_addPointWithDef(const plDataValues &point, 
				const std::vector <bool> &is_defined, 
				plFloat weight = PL_ONE);

  virtual void Output(std::ostream &out) const;

  virtual std::string get_name()const
  {
    return "plLearnDistributionTable";
  }

 private:
  void copy_data_internally(const plValues & values);
  
  template<typename T>
    void copy_data_internally(const T &val);
  
  template <typename T>
    void push_template(const plNonCndLearnObject& learn_object, T value);

  void clean();
  void finalize_copy();

  plDistribution create_uniform()const;

 protected:
  plLearnDistributionTable(const plVariablesConjunction& left_vars,
                           const plVariablesConjunction& right_vars,
                           const plNonCndLearnObject &init_object,
                           bool has_default_initializer);
  

 protected:
  SPL_ACCESS1(plLearnDistributionTable);

  std::map<unsigned int, plNonCndLearnObject*> NCndLObjects_map_;
  
  plNonCndLearnObject *default_learn_object_;
  
  mutable plDataValues left_value_;
  mutable plDataValues right_value_;


  plVariableIndexer right_vars_indexer_;
  plNonCndLearnObject *init_object_;
  bool has_default_initializer_;
};

#endif // plLearnDistributionTable_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
