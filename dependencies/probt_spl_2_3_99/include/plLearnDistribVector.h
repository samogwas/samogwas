/*=============================================================================
 * Product        : ProBT 
 * File           : plLearnDistribVector.h
 * Author         : Kamel Mekhnacha
 * Creation       : Fri Feb 23 11:59:30 2007
 *
 *=============================================================================
 *        (c) Copyright 2006, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */

#ifndef plLearnDistribVector_h
#define plLearnDistribVector_h

#include "plConfig.h"

#include <plLearnObject.h>
#include <cassert>

SPL_DECL2(plLearnDistribVector);

/** This class allows to learn a set of conditional and non-conditional distributions
    in the same time. */
class PL_DLL_API plLearnDistribVector :public plLearnObject
{
 public:

  /** Default constructor used for serialization */
  plLearnDistribVector();

  /** Construct an object allowing to jointly learn a set of distributions
      corresponding to the learning object vector @em learn_objects.  
      @em vars represents the order of data values when inserted without
      using a plValues container (i.e., using stl vectors and pointers).

     The constructor clones the passed @em learn_objects and stores them
     internally (i.e. with no side-effect on the passed @em learn_objects).
  */
  plLearnDistribVector(const std::vector <plLearnObject*> &learn_objects,                          
		       const plVariablesConjunction &vars);

  /** Construct an object allowing to jointly learn a set of distributions
      corresponding to the learning object vector @em learn_objects. 

      The constructor clones the passed @em learn_objects and stores them 
      internally (i.e. with no side-effect on the passed @em learn_objects)
      unless @em do_not_clone_learners is set to @em true

      ATTENTION: using this constructor, you have to:
      - use @em plValues when using @em add_point, or
      - ensure that the order of the learning data corresponds to
      the order defined by variables apparition in the
      concatenation of the variables of all elements of @em
      learn_objects.
  */
  explicit plLearnDistribVector(const std::vector <plLearnObject*> &learn_objects,
                                bool do_not_clone_learners=false);

  /** Copy constructor */
  plLearnDistribVector(const plLearnDistribVector& other);

  /** Assignment */
  plLearnDistribVector &operator=(const plLearnDistribVector& other);

  /** Destructor. */
  virtual ~plLearnDistribVector();
                    
  /** Resets all learning objects. */
  void reset();

  /** Constructs the computable object corresponding to the learnt
      distribution.  Actually, the returned object is a product
      plComputableObject (that can be conditional or not).

      When no variable order is given to the constructor (ie, using
      plLearnDistribVector(const std::vector <plLearnObject*> &)), the left
      variables of the constructed computable object are the concatenation, in
      the  apparition order, of the left variables of all elements of the vector @em
      learn_objects passed to the constructor.  Its right variables are the
      concatenation of the right variables of all elements of @em learn_objects
      and that are not in left one above.

      When an order is provided (ie, using plLearnDistribVector(const
      std::vector <plLearnObject*> &learn_objects, const plVariablesConjunction
      &vars)), the left variables of the constructed computable object are the
      concatenation, respecting the order 'vars', of the left variables of all
      elements of the vector @em learn_objects passed to the constructor.  Its
      right variables are the concatenation, respecting the order 'vars', of
      the right variables of all elements of @em learn_objects and that are not
      in left one above.

      If the learnt set of distributions contains a given variable on the left
      side several times (ie, not a valid decomposition), calling this method
      will throw an error when creating the plComputableObject instance.
  */
  plComputableObject get_computable_object()const;

  /** Returns the list of all learnt computable objects. */
  plComputableObjectList get_computable_object_list()const;
  
  /** Adds a point @em point (represented as a @em plValues) with a
      given weight @em weight and updates the statistics. 
      @em filter is an array of boolean values to say,
      for each learning object, if it will be updated or not.*/
  template <class filterArrayT>
    bool add_point_filter(const plValues &point, filterArrayT filter, plFloat weight = PL_ONE)
  {
    point.to_data_values(all_vars_.get_variable_list(), &values_);
    return addFilterPoint(values_, filter, weight);
  }
  
  /** Same as above, but @em point is represented as a C array. */
  template <class arrayT, class filterArrayT>
    bool add_point_filter(arrayT* point, filterArrayT filter, plFloat weight = PL_ONE)
  {
    for(unsigned int i = 0; i < values_.size(); i++){
      values_[i] = point[i];
    }
    return addFilterPoint(values_, filter, weight);
  }
     
  /** Same as above, but @em point is represented as an STL vector. */
  template <class arrayT, class filterArrayT>
    bool add_point_filter(const std::vector <arrayT> &point, filterArrayT filter, plFloat weight = PL_ONE)
  {
    assert(values_.size() == point.size());
    for(unsigned int i = 0; i < values_.size(); i++){
      values_[i] = point[i];
    }
    return addFilterPoint(values_, filter, weight);
  }
    
  /** Same as above, but @em point is an unidimensional value. 
      ATTENTION: this method can only be used for one-dimensional cases. */
  template <class singleT, class filterArrayT>
    bool add_point_filter(singleT point, filterArrayT filter, plFloat weight = PL_ONE)
  {
    assert(values_.size() == 1);
    values_[0] = point;
    return addFilterPoint(values_, filter, weight);
  }
  
  /** Returns the left variables.  

      ATTENTION: when using the constructor plLearnDistribVector(const
      std::vector <plLearnObject*> &learn_objects, const plVariablesConjunction
      &vars), get_variables() is not necessary equivalent to the concatenation
      of get_left_variables() and get_right_variables().
  */
  const plVariablesConjunction& get_left_variables()const
  {
    return left_variables_;
  }
  
  /** Returns the right variables.
      
      ATTENTION: when using the constructor plLearnDistribVector(const
      std::vector <plLearnObject*> &learn_objects, const plVariablesConjunction
      &vars), get_variables() is not necessary equivalent to the concatenation
      of get_left_variables() and get_right_variables().
  */
  const plVariablesConjunction& get_right_variables()const
  {
    return right_variables_;
  }

   /** Returns the ACTUAL number of parameters. It could be different from the
      number of parameters returned by get_parameters. For example, the actual
      number of parameters of a N dimensional normal distribution is N +
      N*(N+1)/2 because the covariance matrix is symmetric.

      The number of parameters of \em plLearnDistribVector instance is the sum
      of the number of parameters of each containd \em plLearnObject.
  */
  unsigned int get_n_parameters()const;

  /** Return \em nsamples for each learned distribution */
  virtual void get_nsamples(std::vector <unsigned int > &n)const;

  /** Return  \em total_weight for each learned distribution */
  virtual void get_total_weight(std::vector <plFloat> &w)const;

  virtual plLearnObject* clone()const
  {
    return new plLearnDistribVector(*this);
  }

  /** Return the vector of the stored learn objects */
  const std::vector <plLearnObject*> &get_learn_objects()const
  {
    return learn_objects_;
  }

  
 protected:
  SPL_ACCESS2(plLearnDistribVector);

  void init(const std::vector <plLearnObject*> &learn_objects);
  bool internal_addPoint(const plDataValues &point, plFloat weight = PL_ONE);
 

  template <class filterArrayT>
    bool addFilterPoint(const plDataValues &point,  filterArrayT distrib_to_update, plFloat weight = PL_ONE)
  {
    unsigned int i, j;
    bool res = true;

    nsamples_++;
    total_weight_ += weight;
      
    for( i = 0; i < learn_objects_.size(); i++){
	
      if(distrib_to_update[i]){
        for(j = 0; j < point_to_children_points_[i].size(); j++){
          children_points_[i][j] = point[  point_to_children_points_[i][j] ];
        }
        res = res && learn_objects_[i]->internal_addPoint(children_points_[i], weight);
      }
    }
	
    return res;
  }

  bool internal_addPointWithDef(const plDataValues &point, 
                                const std::vector <bool> &is_defined, 
                                plFloat weight = PL_ONE);

  void clean();

  void finalize_copy();

  // private constructor used for deserialization
  plLearnDistribVector(const std::vector <plLearnObject*> &learn_objects,                          
		       const plVariablesConjunction &vars,
                       bool clone_passed_learn_objects);

  virtual void Output(std::ostream &out) const;

 protected:
  std::vector <plLearnObject*> learn_objects_;
  std::vector <plDataValues> children_points_;
  std::vector < std::vector < unsigned int> > point_to_children_points_;
  plVariablesConjunction left_variables_;
  plVariablesConjunction right_variables_;
  bool delete_learn_objects_;
    
  template <class T> friend class plCndLearnObject;
};


#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
