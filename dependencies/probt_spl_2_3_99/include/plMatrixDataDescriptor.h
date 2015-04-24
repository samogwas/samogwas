/*=============================================================================
 * Product        : ProBT 
 * File           : plMatrixDataDescriptor.h
 * Author         : Kamel Mekhnacha
 * Creation       : Tue Feb 20 17:10:57 2007
 *
 *=============================================================================
 *        (c) Copyright 2006, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */

#ifndef plMatrixDataDescriptor_h
#define plMatrixDataDescriptor_h

#include "plConfig.h"

#include <plDataDescriptor.h>
#include <plError.h>

#include <plRealType.h>
#include <plArray.h>
#include <plValues.h>

#include <vector>
#include <cassert>

/** This class implements a data descriptor for STL data matrices
    with possibly missing values.
*/
template <typename T>
class plMatrixDataDescriptor :public plDataDescriptor
{
private:
  plMatrixDataDescriptor(const plMatrixDataDescriptor&);
  plMatrixDataDescriptor& operator=(const plMatrixDataDescriptor&);
public:
  //=============================================================================
  /** @brief Constructor using a pointer to an STL matrix without
      associating it to a set of variables.  The actual association to a set of
      variables could be done later by calling set_variables().

      @param[in] data a pointer to the STL data matrix to be used.
      @param[in] def a pointer to the STL data definition matrix. 0 is complete data (no missing values).
  */
  explicit 
  plMatrixDataDescriptor(const std::vector < std::vector<T> > *data,
			 const std::vector < std::vector <bool> > *def=0)
    : plDataDescriptor()
    , data_(data)
    , definition_(def)
    , current_row_(0)
    , complete_data_current_row_def_()
    , variables_()
    , current_value_()
  {
    if(!data){
      this->is_ok_ = false;
      throw plError(82, "plMatrixDataDescriptor: data pointer is null");
    }
    unsigned int s = (data_->empty() ?  0 : ((*data_)[0]).size()); 

    if(definition_){
      if(definition_->size() != data_->size())
        throw plError(82, "plMatrixDataDescriptor: data matrix and definition matrix must have the same dimension");
			
      if(!data_->empty()){
        if((*definition_)[0].size() != (*data_)[0].size())
          throw plError(82, "plMatrixDataDescriptor: data matrix and definition matrix must have the same dimension");
      }
    }
    else {
      complete_data_current_row_def_ = std::vector<bool>(s, true);
    }

    current_value_ = plValues(plArray("X", 
                                      plRealType(-std::numeric_limits<plFloat>::max(), std::numeric_limits<plFloat>::max()),
                                      1, s));
  }
    
  //=============================================================================
  /** @brief Constructor using a pointer to an STL matrix.
      @param[in] variables The  variables associated to the descriptor.
      @param[in] data A pointer to the STL data matrix to be used.
      @param[in] def A pointer to the STL data definition matrix. 0 is complete data (no missing values).
  */
  plMatrixDataDescriptor(const plVariablesConjunction& variables,
                         const std::vector < std::vector<T>  > *data,
                         const std::vector < std::vector <bool> > *def=0)
    : plDataDescriptor()
    , data_(data)
    , definition_(def)
    , current_row_(0)
    , complete_data_current_row_def_()
    , variables_()
    , current_value_()
  {
    if(!data){
      this->is_ok_ = false;
      throw plError(82, "plMatrixDataDescriptor: data pointer is null");
    }
	
    if(!data_->empty()){
      if(variables.dim() != (*data_)[0].size()){
        throw plError(82, "plMatrixDataDescriptor: data matrix and the variables must have the same dimension");
      }
    }
    
    this->set_variables(variables);

    if(definition_){
      if(definition_->size() != data_->size())
        throw plError(82, "plMatrixDataDescriptor: data matrix and definition matrix must have the same dimension");
			
      if(!data_->empty()){
        if((*definition_)[0].size() != (*data_)[0].size())
          throw plError(82, "plMatrixDataDescriptor: data matrix and definition matrix must have the same dimension");
      }
    }
    else {
      unsigned int s;
      if(data_->empty()) s = 0;
      else s = ((*data_)[0]).size(); 
      complete_data_current_row_def_ = std::vector<bool>(s, true);
    }
  }

  //=============================================================================
  void rewind()
  {
    current_row_ = 0;
  }
    
  //=============================================================================
  bool get_data_record(const plValues* &data_values,
                       const std::vector<bool>*& data_definition)
  {
    if(!this->is_ok_) {
      data_values = 0;
      data_definition = 0;
      return false;
    }

    if(current_row_ < data_->size()) {
      const std::vector<T> &crow = (*data_)[current_row_];
      for(unsigned int j = 0; j < crow.size(); ++j) {
        current_value_[j] = crow[j];
      }

      data_values = &current_value_;

      if(definition_){
        data_definition = &((*definition_)[current_row_]);
      }
      else{
        data_definition = &complete_data_current_row_def_;
      }
      
      ++current_row_;
      return true;
    }

    data_values = 0;
    data_definition = 0;
    return false;
  }
    
  //=============================================================================
  unsigned int get_num_fields()const
  {
    if(!this->is_ok_) return 0;

    if(data_->empty()) return 0;
      
    return ((*data_)[0]).size();  
  }    

  /** Get the number of records (rows) in the data set.
      @return the size of the data set.
  */
  virtual unsigned int get_n_records()
  {
    if(!this->is_ok_) return 0;
  
    return data_->size();
  }
    
  /** Set the variables to be associated to the data descriptor.
      This method is to be called when the descriptor is constructed without providing
      the variables list. After this call, observed_variables() will return 
      the passed 'variables' parameter.
      
      @param[in] variables The variables to be associated to the data descriptor
  */
  void set_variables(const plVariablesConjunction &variables)
  {
    assert(variables.dim() == get_num_fields() );
    variables_ = variables;
    current_value_ = plValues(variables_);
  }

  virtual plVariablesConjunction observed_variables()
  {
    if(variables_.is_empty())
      throw plError(115);

    return variables_;
  }

protected:
  virtual void Output(std::ostream &os)const
  {
    os << "plMatrixDataDescriptor: " << std::endl
       << " Data records number: " << (data_ ? data_->size() : 0)  << std::endl
       << " Current row: " << current_row_ << std::endl
       << " Variables: " << variables_;
  }

protected:
    
  const std::vector < std::vector<T>  > *data_;
  const std::vector < std::vector <bool> > *definition_;
  unsigned int current_row_;
  std::vector<bool> complete_data_current_row_def_;
  plVariablesConjunction variables_;
  plValues  current_value_;
};


#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
