/*=============================================================================
 * Product        : ProBT 
 * File           : plVariableDiscretizer.h
 * Author         : Kamel Mekhnacha
 * Creation       : Oct  2013
 *
 *=============================================================================
 *        (c) Copyright 2013, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */

#ifndef plVariableDicretizer_h
#define plVariableDicretizer_h

#include "plConfig.h"

#include <plMath.h>

#include <vector>
#include <algorithm>


/** Base class for all variable discretization algorithms 
    
    \sa plDataDescriptor::use_for_discretization()
 */
class PL_DLL_API  plVariableDiscretizer
{
 public:
  /** Default constructor */
  plVariableDiscretizer()
    {}
  
  /** Destructor */
  virtual ~plVariableDiscretizer()
    {}

  /** Insert a point */
  virtual void add_point(plFloat val) = 0;

  /** Return the discretization interval values based on the  points inserted using add_point() */
  virtual std::vector<plFloat> get_intervals() const = 0;

  /** Forget all the points inserted using add_point() */
  virtual void reset() = 0;

  /** Use the data set @em data to discretize the variable corresponding to column @em variable_column in @em data */
  std::vector<plFloat> run(plDataDescriptor &data, size_t variable_column)
  {
    reset();
    return data.use_for_discretization(this, variable_column);
  }

  /** Use the data values @em data_values to discretize the corresponding variable */
  std::vector<plFloat> run(const std::vector<plFloat> &data_values)
  {
    reset();
    for(size_t i = 0; i < data_values.size(); ++i) {
      add_point(data_values[i]);
    }
    return get_intervals();
  }

};

/** Simple discretization algorithm providing n intervals with the same number of data points

    \sa plDataDescriptor::use_for_discretization()
 */
class PL_DLL_API  plEqualFrequenciesVariableDiscretizer :public plVariableDiscretizer
{
 public:

  /** Constructor using the requested number of intervals */
  explicit plEqualFrequenciesVariableDiscretizer(size_t n=0)
    : plVariableDiscretizer()
    , n_(n)
    , vals_()
    , is_up_to_date_(false)
    {}
  
  /** Set the number of intervals */
  inline void set_n_intervals(size_t n)
  {
    n_ = n;
  }
  
  virtual void add_point(plFloat val) 
  {
    is_up_to_date_ = false;
    vals_.push_back(val);
  }
  
  virtual std::vector<plFloat> get_intervals() const 
  {    
    if(vals_.empty()) return std::vector<plFloat>();
    
    if(!is_up_to_date_) {
      std::sort(vals_.begin(), vals_.end());
      is_up_to_date_ = true;
    }

    const size_t n_vals = vals_.size();
    const size_t n = std::min(n_, n_vals);

    std::vector<plFloat> intervals;
    size_t vals_index = 0;
    while(vals_index < n_vals) {
      intervals.push_back(vals_[vals_index]);
      assert(n_vals >= vals_index);
      assert(n+1 >= intervals.size());
      vals_index += ((n_vals-vals_index)/(n-intervals.size()+1));
      while( (vals_index < n_vals) && (intervals.back() == vals_[vals_index]) ) {
        ++vals_index;
      }
    }
    
    if( intervals.size() == 1 ) {
      intervals.push_back(vals_.front() + 0.01);
    }
    else {
      if( intervals.back() != vals_.back() ) {
        intervals.push_back(vals_.back());
      }
      intervals.back() += (0.01*(intervals.back() - vals_.front())); // add 1% of the range
    }

    return intervals;
  }

  virtual void reset() 
  {
    vals_.clear();
    is_up_to_date_ = false;
  }

  /** Set the values of the discretizer
      Equivalent to adding each point of @em points using add_point()
  */
  void set_values(const std::vector<plFloat> &points) 
  {
    vals_ = points;
    is_up_to_date_ = false;
  }

 private:
  size_t n_;
  mutable std::vector<plFloat> vals_;
  mutable bool is_up_to_date_;
};

#endif // plVariableDicretizer_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
