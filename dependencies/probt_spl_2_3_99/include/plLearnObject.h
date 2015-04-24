/*=============================================================================
 * Product        : ProBT 
 * File           : plLearnObject.h
 * Author         : Kamel Mekhnacha
 * Creation       : Fri Feb 23 11:49:41 2007
 *
 *=============================================================================
 *        (c) Copyright 2006, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */

#ifndef plLearnObject_h
#define plLearnObject_h

#include "plConfig.h"

#include <limits>

#include <plDataValues.h>
#include <plValues.h>
#include <plVariablesConjunction.h>
#include <plComputableObject.h>
#include <plDLLExport.h>
#include <plDataDescriptor.h>

class plJointDistribution;
class plLearnObject;

SPL_DECL1(plLearner);

//=============================================================================
/** This is the base class of all learning objects. */
class PL_DLL_API plLearner : public plObject
{
 public: 

  /**
     Learns the parameters of a model. There can be arbitrary missing data in
     the learning observations provided by \em data_descriptor. An EM
     (Expectation-Maximization) algorithm is used. This EM algorithm is
     implemented using plEMLearner. The EM algorithm is initialized using the
     initial parameters from \em learn_objects and is run until convergence.

     @param[in] learn_objects a vector of incremental learning
     objects allowing to provide the structure (dependencies), the
     parametrical form of each distribution, and the corresponding priors.
     @param[in] data_descriptor the complete/incomplete data set on which the EM algorithm will iterate.
     
     @return the joint distribution corresponding to the model.
  */
  static plJointDistribution learn_model_parameters(const std::vector <plLearnObject*> &learn_objects,
                                                    plDataDescriptor &data_descriptor);

  /**
     Learns the parameters of a model. There can be arbitrary missing data in
     the learning observations provided by \em data_descriptor. An EM
     (Expectation-Maximization) algorithm is used. This EM algorithm is
     implemented using plEMLearner and is run until convergence.

     @param[in] learn_objects a vector of incremental learning
     objects allowing to provide the structure (dependencies), the
     parametrical form of each distribution, and the corresponding priors.
     @param[in] init_distribs initial distributions of the EM algorithm. This
     parameter is useful only when the data set is incomplete (ie, contains
     missing values).
     @param[in] data_descriptor the complete/incomplete data set on which the EM algorithm will iterate.

     @return the joint distribution corresponding to the model.
  */
  static plJointDistribution learn_model_parameters(const std::vector <plLearnObject*> &learn_objects,
                                                    const plComputableObjectList &init_distribs,
                                                    plDataDescriptor &data_descriptor);
};

//=============================================================================
SPL_DECL2(plLearnObject);

/** This is the base class of all incremental (adaptive) learning objects. */
class PL_DLL_API plLearnObject : public plLearner
{
 public:
  /** Constructor using a set of variables.*/
  explicit plLearnObject(const plVariablesConjunction &variables);
    
  /** Default constructor. */
  plLearnObject()
    : plLearner(), nsamples_(0), total_weight_(PL_ZERO), all_vars_(), values_() {}
	
  /** Copy constructor */
  plLearnObject(const plLearnObject &obj);
  
  /** Assignment operator */
  plLearnObject &operator=(const plLearnObject &obj);
    
  /** Destructor */
  virtual ~plLearnObject();
    
  /** Returns the number  of the points used 
      in learning (i.e. added using the \em add_point methods). */
  unsigned int get_nsamples() const {return nsamples_;}

  
  virtual void get_nsamples(std::vector <unsigned int > &n)const
  {
    n.resize(1);
    n[0] = get_nsamples();
  }
  

  /** Returns the  sum of weights of the points used 
      in learning (i.e. added using add_point()). */
  plFloat get_total_weight() const {return total_weight_;}
  
  /** Returns the  sum of weights of the points used 
      in learning (i.e. added using the \em add_point methods). */
  virtual void get_total_weight(std::vector <plFloat> &w)const
  {
    w.resize(1);
    w[0] = get_total_weight();
  }
  
  /** Resets learning (forgets all learned data). 
   */
  virtual void reset() 
  {
    nsamples_ = 0;
    total_weight_ = PL_ZERO;
  }
 
  /** Constructs the computable object corresponding to the learnt
      distribution */
  /** Constructs and returns the computable object (conditional or non
      conditional distribution) corresponding to the current state of
      this learner */
  virtual plComputableObject get_computable_object()const = 0;
    
        
  /** Adds a data point with a given weight.

      @param[in] point Data point to add.
      @param[in] weight Weight of the added point. If @em weight is an integer,
      add_point(x, weight) is equivalent to doing @em weight times add_point(x).

      @returns True if and only if the point could be added.
  */
  bool add_point(const plValues &point, plFloat weight = PL_ONE)
    {
      // point.to_data_values(all_vars_.get_variable_list(), &values_);
      // return internal_addPoint(values_, weight);

      std::vector<bool> defs;
      point.to_data_values(all_vars_.get_variable_list(), &values_, defs);
      return internal_addPointWithDef(values_, defs, weight);
    }
    
 private:
  template <class arrayT>
    bool add_point_ptr(arrayT* point, plFloat weight = PL_ONE)
  {
    for(size_t i = 0; i < values_.size(); ++i){
      values_[i] = point[i];
    }
    return internal_addPoint(values_, weight);
  }

 public:
  /** Adds a multidimensional data point with a given weight.

      @param[in] point Data point to add, as a C array.
      @param[in] weight Weight of the added point. If @em weight is an integer,
      add_point(x, weight) is equivalent to doing @em weight times add_point(x).

      @returns True if and only if the point could be added.
  */
  bool add_point(const int* point, plFloat weight = PL_ONE)
    {
      return add_point_ptr(point, weight);
    }
  bool add_point(const unsigned int* point, plFloat weight = PL_ONE)
    {
      return add_point_ptr(point, weight);
    }
  bool add_point(const float* point, plFloat weight = PL_ONE)
    {
      return add_point_ptr(point, weight);
    }
  bool add_point(const double* point, plFloat weight = PL_ONE)
    {
      return add_point_ptr(point, weight);
    }
  bool add_point(const long double* point, plFloat weight = PL_ONE)
    {
      return add_point_ptr(point, weight);
    }

 private:
  template <class arrayT>
    bool add_point_vect(const std::vector<arrayT> & point, plFloat weight = PL_ONE)
  {
    assert(point.size() == values_.size());
    for(size_t i = 0; i < values_.size(); ++i){
      values_[i] = point[i];
    }
    return internal_addPoint(values_, weight);
  }

 public:

  /** Adds a multidimensional data point with a given weight.

      @param[in] point Data point to add, as an STL vector.
      @param[in] weight Weight of the added point. If @em weight is an integer,
      add_point(x, weight) is equivalent to doing @em weight times add_point(x).

      @returns True if and only if the point could be added.
  */
  bool add_point(const std::vector<int> & point, plFloat weight = PL_ONE)
    {
      return add_point_vect(point, weight);
    }
  bool add_point(const std::vector<unsigned int> & point, plFloat weight = PL_ONE)
    {
      return add_point_vect(point, weight);
    }
  bool add_point(const std::vector<double> & point, plFloat weight = PL_ONE)
    {
      return add_point_vect(point, weight);
    }
  bool add_point(const std::vector<float> & point, plFloat weight = PL_ONE)
    {
      return add_point_vect(point, weight);
    }
  bool add_point(const std::vector<long double> & point, plFloat weight = PL_ONE)
    {
      return add_point_vect(point, weight);
    }

 private:
  template <class singleT>
    bool add_point_single(singleT point, plFloat weight = PL_ONE)
  {
    if(values_.size() != 1)
      throw plError(8, "plLearnObject::add_point(T &, bool): variables dimension has to be 1");

    values_[0] = point;
    return internal_addPoint(values_, weight);
  }

 public:

  /** Adds a unidimensional data point with a given weight.

      @param[in] point Data point to add. It must be convertible to the type of
      data of the variable.
      @param[in] weight Weight of the added point. If @em weight is an integer,
      add_point(x, weight) is equivalent to doing @em weight times add_point(x).

      @returns True if and only if the point could be added.
  */
  bool add_point(int point, plFloat weight = PL_ONE)
    {
      return add_point_single(point, weight);
    }
  bool add_point(unsigned int point, plFloat weight = PL_ONE)
    {
      return add_point_single(point, weight);
    }
  bool add_point(double point, plFloat weight = PL_ONE)
    {
      return add_point_single(point, weight);
    }
  bool add_point(float point, plFloat weight = PL_ONE)
    {
      return add_point_single(point, weight);
    }
  bool add_point(long double point, plFloat weight = PL_ONE)
    {
      return add_point_single(point, weight);
    }

  /** Adds a multidimensional data point with a given weight; the components of
      the point are specified in non-contiguously.

      @param[in] point Data point to add, as a @em plValues.
      @param[in] is_defined Indicates which components should be used for
      learning.  is_defined[i] should be @em true if and only if point[i] is to
      be used.
      @param[in] weight Weight of the added point. If @em weight is an integer,
      add_point(x, weight) is equivalent to doing @em weight times
      add_point(x).

      @returns True if and only if the point could be added.
  */
  bool add_point(const plValues &point, 
                 const std::vector <bool> &is_defined, plFloat weight = PL_ONE)
    {
      assert(is_defined.size() == values_.size());
      point.to_data_values(all_vars_.get_variable_list(), &values_);
      return internal_addPointWithDef(values_, is_defined, weight);
    }
  
 private:
  template <class arrayT>
    bool add_point_def_ptr(arrayT* point, 
                           const std::vector <bool> &is_defined, plFloat weight = PL_ONE)
  {
    assert(values_.size() == is_defined.size());

    for(size_t i = 0; i < values_.size(); ++i){
      values_[i] = point[i];
    }
    return internal_addPointWithDef(values_, is_defined, weight);
  }
 public:
  /** Adds a multidimensional data point with a given weight; the components of
      the point are specified in order but non-contiguously.

      @param[in] point Data point to add, as a C array.
      @param[in] is_defined Indicates which components should be used for
      learning.  is_defined[i] should be @em true if and only if point[i] is to
      be used.
      @param[in] weight Weight of the added point. If @em weight is an integer,
      add_point(x, weight) is equivalent to doing @em weight times
      add_point(x).

      @returns True if and only if the point could be added.
  */
  bool add_point(const int* point, 
                 const std::vector <bool> &is_defined, plFloat weight = PL_ONE)
    {
      return add_point_def_ptr(point, is_defined, weight);
    }
  bool add_point(const unsigned int* point, 
                 const std::vector <bool> &is_defined, plFloat weight = PL_ONE)
    {
      return add_point_def_ptr(point, is_defined, weight);
    }
  bool add_point(const double* point, 
                 const std::vector <bool> &is_defined, plFloat weight = PL_ONE)
    {
      return add_point_def_ptr(point, is_defined, weight);
    }
  bool add_point(const float* point, 
                 const std::vector <bool> &is_defined, plFloat weight = PL_ONE)
    {
      return add_point_def_ptr(point, is_defined, weight);
    }
  bool add_point(const long double* point, 
                 const std::vector <bool> &is_defined, plFloat weight = PL_ONE)
    {
      return add_point_def_ptr(point, is_defined, weight);
    }
  
 private:
  template <class arrayT>
    bool add_point_def_vect(const std::vector <arrayT> &point,
                            const std::vector <bool> &is_defined, plFloat weight = PL_ONE)
  {
    assert(point.size() == values_.size());
    assert(point.size() == is_defined.size());

    for(size_t i = 0; i < values_.size(); ++i){
      values_[i] = point[i];
    }
    return internal_addPointWithDef(values_, is_defined, weight);
  }

 public:
  /** Adds a multidimensional data point with a given weight; the components of
      the point are specified in order but non-contiguously.

      @param[in] point Data point to add, as an STL vector.
      @param[in] is_defined Indicates which components should be used for
      learning.  is_defined[i] should be @em true if and only if point[i] is
      to be used.
      @param[in] weight Weight of the added point. If @em weight is an
      integer, add_point(x, weight) is equivalent to doing @em weight times
      add_point(x).

      @returns True if and only if the point could be added.
  */
  bool add_point(const std::vector <int> &point,
                 const std::vector <bool> &is_defined, plFloat weight = PL_ONE)
    {
      return add_point_def_vect(point, is_defined, weight);
    }
  bool add_point(const std::vector <unsigned int> &point,
                 const std::vector <bool> &is_defined, plFloat weight = PL_ONE)
    {
      return add_point_def_vect(point, is_defined, weight);
    }
  bool add_point(const std::vector <double> &point,
                 const std::vector <bool> &is_defined, plFloat weight = PL_ONE)
    {
      return add_point_def_vect(point, is_defined, weight);
    }
  bool add_point(const std::vector <float> &point,
                 const std::vector <bool> &is_defined, plFloat weight = PL_ONE)
    {
      return add_point_def_vect(point, is_defined, weight);
    }
  bool add_point(const std::vector <long double> &point,
                 const std::vector <bool> &is_defined, plFloat weight = PL_ONE)
    {
      return add_point_def_vect(point, is_defined, weight);
    }


  /** Learn using the data set defined by the container 'data_descriptor'.

      @param[in] data_descriptor the data set to be used.
      @param[in] n the number of data rows to be used.

      @return the number of the inserted points on success. Returns a negative value
      if the data descriptor is not OK (eg, CSV file does not exist, mysql
      server is not responding,...).

      It is the responsibility of the caller to setup the data descriptor properly
      before calling this method, for instance by calling rewind() on it if
      necessary. Also, this method does not attempt to rewind() the data descriptor
      before returning.
  */
  int learn(plDataDescriptor &data_descriptor, 
            unsigned int n = std::numeric_limits<unsigned int>::max());

  int learn_dynamic(plDataDescriptor &data_descriptor, 
                    const std::vector< std::pair<plSymbol, plSymbol> > &ts_edges,
                    unsigned int n = std::numeric_limits<unsigned int>::max());


  /** Deprecated method, kept temporarily for compatibility purposes. Identical in function to the above
      learn().

      @param[in] data_descriptor The data set to use for learning.
      @param[in] variables Variables in the data set. This should be equal to data_descriptor.observed_variables().
      @param[in] rewind_before_use Whether the method should rewind the data descriptor before using it.
  */
  int learn(plDataDescriptor &data_descriptor,
            plVariablesConjunction const & variables,
            bool rewind_before_use = true) {
    plError::warn(112, "plLearnObject::learn(data_descriptor, variables, rewind_before_use) -- use learn_data_descriptor(data_descriptor) and data_descriptor.rewind() instead");
    if (rewind_before_use) {
      data_descriptor.rewind();
    }
    if (data_descriptor.observed_variables() != variables) {
      THROW_PL_ERROR(6, "in deprecated plLearnObject::learn(data_descriptor, variables): passed variables "
                     << variables << " are not equal to data_descriptor variables " << data_descriptor.observed_variables());
    }
    return learn(data_descriptor, std::numeric_limits<unsigned int>::max());
  }

 private:
  template <typename T>
    void learn_impl(const std::vector<T> &vals)
  {
    if(values_.size() != 1)
      throw plError(8, "learn(const std::vector<T> &): variables dimension has to be 1");

    for(size_t i = 0; i < vals.size(); ++i) {
      add_point(vals[i]);
    } 
  }
 public:
  /** Learn using a set of one-dimensional values */
  void learn(const std::vector<int> &vals)
    {
      learn_impl(vals);
    }
  void learn(const std::vector<unsigned int> &vals)
    {
      learn_impl(vals);
    }
  void learn(const std::vector<double> &vals)
    {
      learn_impl(vals);
    }
  void learn(const std::vector<float> &vals)
    {
      learn_impl(vals);
    }
  void learn(const std::vector<long double> &vals)
    {
      learn_impl(vals);
    }
    
 private:
  template <typename T>
    void learn_def_impl(const std::vector<T> &vals, const std::vector<bool> &def)
  {
    if(values_.size() != 1)
      throw plError(8, "learn(const std::vector<T> &, const std::vector<bool> &): variables dimension has to be 1");

    for(size_t i = 0; i < vals.size(); ++i) {
      if(def[i]) add_point(vals[i]);
    }
  }

 public:
  /** Learn using a set of one-dimensional values with their definition information */
  void learn(const std::vector<int> &vals, const std::vector<bool> &def)
    {
      learn_def_impl(vals, def);
    }
  void learn(const std::vector<unsigned int> &vals, const std::vector<bool> &def)
    {
      learn_def_impl(vals, def);
    }
  void learn(const std::vector<double> &vals, const std::vector<bool> &def)
    {
      learn_def_impl(vals, def);
    }
  void learn(const std::vector<float> &vals, const std::vector<bool> &def)
    {
      learn_def_impl(vals, def);
    }
  void learn(const std::vector<long double> &vals, const std::vector<bool> &def)
    {
      learn_def_impl(vals, def);
    }


  /** Returns the left variables */
  virtual const plVariablesConjunction& get_left_variables()const = 0;

  /** Returns the right variables */
  virtual const plVariablesConjunction& get_right_variables()const = 0;

  /** Returns all variables */
  const plVariablesConjunction &get_variables()const
  {
    return all_vars_;
  }

  /** Returns the ACTUAL number of parameters. It could be different from the
      number of parameters returned by get_parameters.
      
      For example, the actual number of parameters of a N dimensional normal distribution 
      is N + N*(N+1)/2 because the covariance matrix is symmetric.
  */
  virtual unsigned int get_n_parameters()const = 0;


  /** Clone the learning object */
  virtual plLearnObject* clone()const = 0;
  
 protected:
  plLearnObject(const kplVariableList &variables);

   
  ///
  virtual bool internal_addPointWithDef(const plDataValues &point,
                                        const std::vector <bool> &is_defined,
                                        plFloat weight = PL_ONE);

  /* Adds a point @em point with a given weight @em weight and updates
     statistics.

     This virtual method has to be implemented by all derived classes that have
     to call this default implementation.
      
     The default implementation checks that @em point is in variable range and
     increments @em nsamples_ by 1 and increments @em total_weight_ by the
     value of @em weight.
  */
  virtual bool internal_addPoint(const plDataValues & point, plFloat weight = PL_ONE);
  
  int internal_learn(plDataDescriptor &data_descriptor, unsigned int n);

  int internal_learn(plDataDescriptor &data_descriptor, 
                     const std::vector< std::pair<plSymbol, plSymbol> > &ts_edges,
                     unsigned int n);

 protected:
  SPL_ACCESS2(plLearnObject);

  void set_variables(const plVariablesConjunction& vars);
  
  unsigned int nsamples_;
  plFloat total_weight_;


  plVariablesConjunction all_vars_;
  plDataValues values_;

  friend class plLearnDistribVector;
  friend class plParticleFilter;
  friend class plLearnDistributionTable;
};

//=============================================================================
SPL_DECL1(plLearnFrozenDistribution);

/** This class implements a learning object that learns nothing. It's
    just a helper to provide the possibility to freeze some
    distributions when using learning algorithms.  Actually, this
    leaning object is initialized with a given distribution and simply
    returns the same distribution when the @em get_computable_object
    method is called.
*/
class PL_DLL_API plLearnFrozenDistribution : public plLearnObject
{
 public:
  
  /** Default constructor.
   */
  plLearnFrozenDistribution()
    : frozen_computable_object_(), left_vars_(), right_vars_()
    {}

  /** Constructor.
      @param[in] frozen_computable_object the computable object to freeze.
  */
  explicit plLearnFrozenDistribution(const plComputableObject &frozen_computable_object)
    : plLearnObject(frozen_computable_object.get_variables())
    , frozen_computable_object_(frozen_computable_object)
    , left_vars_(  frozen_computable_object_.get_left_variables() )
    , right_vars_( frozen_computable_object_.get_right_variables() )
    {}
  
  /** Returns the left variables */
  const plVariablesConjunction& get_left_variables()const
  {
    return left_vars_;
  }
  
  /** Returns the right variables */
  const plVariablesConjunction& get_right_variables()const
  {
    return right_vars_;
  }

  /** Returns the frozen computable object */
  plComputableObject get_computable_object()const
  {
    return frozen_computable_object_;
  }

  unsigned int get_n_parameters()const
  {
    return 0;
  }

  virtual plLearnObject* clone()const
  {
    return new plLearnFrozenDistribution(*this);
  }

 protected:
  virtual void Output(std::ostream &out) const
  {
    if(left_vars_.is_empty())
      out << "plLearnFrozenDistribution: Empty";
    else
      out << "plLearnFrozenDistribution: " << get_computable_object();
  }

 protected:  
  SPL_ACCESS1(plLearnFrozenDistribution);

  plComputableObject frozen_computable_object_;
  plVariablesConjunction left_vars_;
  plVariablesConjunction right_vars_;
};




#endif //plLearnObject_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
