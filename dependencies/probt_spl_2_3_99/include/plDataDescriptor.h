/*=============================================================================
 * Product        : ProBT 
 * File           : plDataDescriptor.h
 * Author         : Kamel Mekhnacha
 * Creation       : Tue Feb 20 14:07:49 2007
 *
 *=============================================================================
 *        (c) Copyright 2006, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */

#ifndef plDataDescriptor_h
#define plDataDescriptor_h

#include "plConfig.h"

#include <plObject.h>

#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <limits>

#include <plVariablesConjunction.h>
#include <plValues.h>
#include <plSymbol.h>

// Work around a compilation problem under MSVC10
#undef min
#undef max 

class plVariableDiscretizer;

/** This abstract template class defines a sequential data container.

    This data descriptor is intended to be used with learning functions such as
    plLearnObject::learn(),
    plLearner::learn_model_parameters(), and plEMLearner::run().
*/
class PL_DLL_API plDataDescriptor :public plObject
{
 public:
  /** Constructor */
  plDataDescriptor()
    : plObject()
    , is_ok_(true)
    , n_(-1)
    {}
  
  /** Returns @em true if the initialization (construction) is OK. 

      \return @em true if the initialization (construction) is OK
  */
  bool is_ok()const{return is_ok_;}

  /** Destructor. */
  virtual ~plDataDescriptor(){}

  /** Returns the number of fields (columns).
      
      \return the number of fields (columns/variables).
  */
  virtual unsigned int get_num_fields()const = 0;

  /** Rewinds the container to go back to its begining */
  virtual void rewind() = 0;

  /** Get the next data record (row) and advances by one record in the data
      
      \param[out] data_values an array of values for all variables described in the record.
      
      \param[out] data_definition a vector stating, for each value in \em
      data_values, whether it's defined or not.
      
      \return \em false iif the end of the container is reached.
      
      The returned pointers are managed internally: you should not attempt to
      delete them, or use them after another call to get_data_record(). Also,
      you should not use them when get_data_record() returns \em false.
  */
  virtual bool get_data_record(const plValues*& data_values, 
                               const std::vector<bool>*& data_definition) = 0;

  /** Get the next data record (row) and advances by one record in the data.
      This function make a copy into the two output parameters and is then less
      efficient than those returning pointers. It's intended to be used with 
      the Python, .net, and Java bindings.
      
      \param[out] data_values an array of values for all variables described in the record.
      \param[out] data_definition a vector stating, for each value in \em
      data_values, whether it's defined or not.
      
      \return \em false iif the end of the container is reached.
  */
  bool get_data_record_value(plValues& data_values, 
                             std::vector<bool>& data_definition);

  /** Get the next data record (row) as an STL vector and advances by one
      record in the data.  This function make a copy into the two output
      parameters and is then less efficient than those returning pointers. 
      
      \param[out] data_row a vecor of values for all variables described in the record.
      \param[out] data_row_def a vector stating, for each value in \em
      data_row, whether it's defined or not.
      
      \return \em false iif the end of the container is reached.
  */
  template <typename T>
    bool get_data_row(std::vector<T>  &data_row,
                      std::vector<bool> &data_row_def)
  {
    const plValues *data_values;
    const std::vector<bool> *data_definitions;
    
    const bool not_eof = get_data_record(data_values, data_definitions);
    if(!not_eof) return false;

    
    data_row.resize(data_definitions->size());
    for(unsigned int i = 0; i < data_row.size(); ++i) {
      data_row[i] = (*data_values)[i];
    }
    
    data_row_def = *data_definitions;
    
    return true;
  }

  /** Get the defined values from the next data record (row) and advances by one record in the data
      
      \param[out] data_values an array of values for all variables defined in the record.
      
      \return \em false iif the end of the container is reached.
  */
  bool get_next_defined_values(plValues& data_values);

  /** Fills the data matrix corresponding to the data container. 
      \param[out] data_matrix the returned data matrix.
      \param[in] n the number of rows (data records) to be copied
  */
  template <typename T>
    void get_data_matrix(std::vector < std::vector<T> > &data_matrix, 
                         unsigned int n = std::numeric_limits<unsigned int>::max())
  {
    data_matrix.clear();
	
    rewind();

    const plValues *data_values;
    const std::vector<bool> *data_definitions;
    unsigned int i = 0;
    std::vector<T> row;
    while( i < n && get_data_record(data_values, data_definitions) ) {
      row.resize(data_values->size());
      for(unsigned int j = 0; j < data_values->size(); ++j) {
        if( (*data_definitions)[j]) row[j] = (*data_values)[j];
      }
      data_matrix.push_back(row);
      ++i;
    }

    rewind();
  }

  /** Fills the data matrix (and the corresponding definition matrix) 
      corresponding to the data container. 
      
      \param[out] data_matrix the returned data matrix.
      \param[out] def_matrix the corresponding definition matrix.
      \param[in] n the number of rows (data records) to be copied
  */
  template <typename T>
    void get_data_matrix(std::vector <std::vector<T> > &data_matrix,
                         std::vector <std::vector<bool> > &def_matrix, 
                         unsigned int n = std::numeric_limits<unsigned int>::max())
  {
    data_matrix.clear();
    def_matrix.clear();
	
	
    rewind();

    const plValues *data_values;
    const std::vector<bool> *data_definitions;
    unsigned int i = 0;
    std::vector<T> row;
    while( i < n && get_data_record(data_values, data_definitions) ) {
      row.resize(data_values->size());
      for(unsigned int j = 0; j < data_values->size(); ++j) {
        if( (*data_definitions)[j]) row[j] = (*data_values)[j];
      }
      data_matrix.push_back(row);
      def_matrix.push_back(*data_definitions);
      ++i;
    }

    rewind();
  }

  
  /** Returns the range of the data.

      \param[out] min_vals the minimal value found in the data container for each data field (column).
      \param[out] max_vals the maximal value found in the data container for each data field (column).
  */
  virtual void get_data_range(std::vector<plFloat> &min_vals, std::vector<plFloat> &max_vals);

  /** Computes some simple stats on the data.
      \param[out] min_vals the minimal value for each data field (column).
      \param[out] max_vals the maximal value for each data field (column).
      \param[out] mean_vals the mean value for each data field (column).
      \param[out] stdev_vals the standard deviation for each data field (column).
      \param[out] n the number of defined records (rows) for each data field (column).
      \param[out] ncomplete_rows the number of complete records (rows) (with no missing values)

      \returns the number of data records (rows)
  */
  virtual unsigned int get_simple_stats(std::vector<plFloat> &min_vals, std::vector<plFloat> &max_vals,
                                        std::vector<plFloat> &mean_vals, std::vector<plFloat> &stdev_vals,
                                        std::vector<unsigned int> &n,
                                        unsigned int &ncomplete_rows);


  /** Get a column vector.
      @param[in] n column number.
      @param[out] column_vals the returned column vector.
  */
  template <typename T>
    void get_column_values(unsigned int n, std::vector<T> &column_vals)
  {
    column_vals.clear();

    if(n >= get_num_fields() ) return;
        
    rewind();
    
    const plValues *data_values;
    const std::vector<bool> *data_definitions;
    while( get_data_record(data_values, data_definitions) ) {
      column_vals.push_back( (*data_values)[n] );
    }
    rewind();
  }


  /** Get a column vector and the corresponding definition values.
      @param[in] n column number.
      @param[out] column_vals the returned column vector.
      @param[out] column_defs the corresponding definition values.
  */
  template <typename T>
    void get_column_values(unsigned int n, 
                           std::vector<T> &column_vals,
                           std::vector<bool> &column_defs)
  {
    column_vals.clear();
    column_defs.clear();

    if(n >= get_num_fields() ) return;
    
    rewind();
    
    const plValues *data_values;
    const std::vector<bool> *data_definitions;
    while( get_data_record(data_values, data_definitions) ) {
      column_vals.push_back( (*data_values)[n] );
      column_defs.push_back( (*data_definitions)[n] );
    }
    rewind();
  }

  /** Get a column vector.
      @param[in] variable the variable of interest.
      @param[out] column_vals the returned column vector.
  */
  template <typename T>
    void get_variable_values(const plSymbol& variable, 
                             std::vector<T> &column_vals)
  {
    int n = observed_variables().get_symbol_position(variable);

    if(n > 0) {
      get_column_values(n, column_vals);
    }
  }

  /** Get a column vector and the corresponding definition values.
      @param[in] variable the variable of interest.
      @param[out] column_vals the returned column vector.
      @param[out] column_defs the corresponding definition values.
  */
  template <typename T>
    void get_variable_values(const plSymbol& variable, 
                             std::vector<T> &column_vals,
                             std::vector<bool> &column_defs)
  {
    int n = observed_variables().get_symbol_position(variable);

    if(n > 0) {
      get_column_values(n, column_vals, column_defs);
    }
  }


  /** Validate the data set by checking that the provided
      values are compatible with the types of the given variable
      conjunction associaded to the data descriptor.

      @param[out] erroneous_lines line numbers (starting from 1) in which
      incompatible values have been found.
      
      @return true iff no incompatible values have been found.
  */
  virtual bool validate(std::vector<unsigned int> &erroneous_lines);

  /** Get the number of records (rows) in the data set.  

      ATTENTION: For the first call, the default implementation scans the data
      set and thus has a complexity of O(n) where \em n is the size.
  
      @return the size of the data set.
  */
  virtual unsigned int get_n_records();

  /** Get the variables associated to the descriptor */
  virtual plVariablesConjunction observed_variables()=0;

  /** Prints the header and the 'n' first rows in the output stream 'sout'.
      This function has as side effect the rewinding of this data descriptor
  */
  virtual std::ostream& print_head(unsigned int n = 20, std::ostream& sout = std::cout, char sep = '\t');

  /** Prints the header and the 'n' first rows in the file 'file'.
      This function has as side effect the rewinding of this data descriptor
  */
  virtual void print_head(const std::string &file, unsigned int n = 20, char sep = '\t');

  
  /** Prints some simple statistic of the data.
      This function has as side effect the rewinding of this data descriptor
  */
  virtual std::ostream& print_simple_stats(std::ostream& sout = std::cout, char sep = '\t');

  /** Prints some simple statistic of the data.
      This function has as side effect the rewinding of this data descriptor
  */
  virtual void print_simple_stats(const std::string &file, char sep = '\t');

  
  /** Return the discretization intervals for the variables corresponding to
      columns @em variable_columns according to this data set
  */
  std::vector<std::vector<plFloat> >
    use_for_discretization(const std::vector<plVariableDiscretizer*> &discretizers,
                           const std::vector<size_t>& variable_columns);
  
  /** Return the discretization intervals for the variable corresponding to
      column @em variable_column according to this data set
  */
  std::vector<plFloat> use_for_discretization(plVariableDiscretizer* discretizer,
                                              size_t variable_column);
  
 protected:
  virtual void Output(std::ostream &os) const;

 protected:
  bool is_ok_;
  int n_;
};

/** Interface for a data descriptor able to return lines of data as uninterpreted raw string vectors.
 */
class PL_DLL_API plStringDataDescriptor {
 public:
  virtual ~plStringDataDescriptor() {}

  /** Returns the raw, uninterpreted data record as a vector of strings. This is a virtual
      method that should be filled in by implementations.

      @param[out] data_values Line of data split in elements, resized as needed and filled in by this method.

      @return false iff the end of the container is reached.

      This method consumes one line of data descriptor input.
  */
  virtual bool get_data_record(std::vector<std::string> & data_values) = 0;

  /** Tries to guess the variables described by a string data descriptor, by examining its data.
      In the current implementation, one pass is made over all data of the descriptor.

      @param[in] header Whether the first line of data is a header describing the variable names.
      @param[in] real_discretization Discretization to apply to real variables (treated as plContinuousIntervalType). A negative value prescribes no discretization: plRealType is used.
      @return A conjunction of variables suitable to describe the variables in the data descriptor.
  */
  plVariablesConjunction guess_variables(bool header, int real_discretization = -1);
};


#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
