/*=============================================================================
 * Product        : ProBT 
 * File           : plODBCDataDescriptor.h
 * Author         : Samir OUIFI
 * Creation       : Mon Jul 30 14:50:54 2007
 *
 *=============================================================================
 *        (c) Copyright 2007, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */

#ifndef plODBCDataDescriptor_h
#define plODBCDataDescriptor_h

#include "plConfig.h"

#include <vector>
#include <string>


#include <plDataDescriptor.h>
#include <plODBCDataConnector.h>
#include <plError.h>
#include <plLabelType.h>

#include <plDatabaseInitError.h>

#include <plRealType.h>
#include <plArray.h>

/** This class implements a data descriptor for ODBC data sources. */
class plODBCDataDescriptor :public plDataDescriptor
{

  plODBCDataDescriptor(const plODBCDataDescriptor&);
  plODBCDataDescriptor& operator=(const plODBCDataDescriptor&);

public:

  /** Constructor using database parameters and associating it to a set of variables.

      @param[in] variables The set of variables to be associated to the descriptor.
      @param[in] host Host name (address)
      @param[in] user Database user name
      @param[in] passwd User's password
      @param[in] db Database name
      @param[in] query The query to execute
      @param[in] port Port number
      @param[in] driver_name Driver name
  */
  inline plODBCDataDescriptor(const plVariablesConjunction& variables,
                       const std::string &host,	
		       const std::string &user, 
		       const std::string &passwd, 
		       const std::string &db, 
		       const std::string &query,
		       const std::string &port ,
		       const std::string &driver_name);

  /** Constructor using database parameters without
      associating it to a set of variables.  The actual association to a set of
      variables could be done later by calling set_variables().

      @param[in] host Host name (address)
      @param[in] user Database user name
      @param[in] passwd User's password
      @param[in] db Database name
      @param[in] query The query to execute
      @param[in] port Port number
      @param[in] driver_name Driver name
  */
  inline plODBCDataDescriptor(const std::string &host,	
		       const std::string &user, 
		       const std::string &passwd, 
		       const std::string &db, 
		       const std::string &query,
		       const std::string &port ,
		       const std::string &driver_name);

  /** Constructor using an already open connection and associating it to a set of variables.

      @param[in] variables The set of variables to be associated to the descriptor.
      @param[in] odbc An already open connection.
      @param[in] query The query to execute.
      @param[in] port Port number.
      @param[in] driver_name Driver name.
  */
  inline plODBCDataDescriptor(const plVariablesConjunction& variables,
                       plODBCDataConnector *odbc,
		       const std::string &query,
		       const std::string &port ,
		       const std::string &driver_name);

  /** Constructor using an already open connection without
      associating it to a set of variables.  The actual association to a set of
      variables could be done later by calling set_variables().

      @param[in] odbc An already open connection.
      @param[in] query The query to execute.
      @param[in] port Port number.
      @param[in] driver_name Driver name.
  */
  inline plODBCDataDescriptor(plODBCDataConnector *odbc,
		       const std::string &query,
		       const std::string &port ,
		       const std::string &driver_name);

  /** Destructor */
  inline ~plODBCDataDescriptor();

  inline void rewind();


  //=============================================================================
  inline void set_data_type(unsigned int var)
  {
    if(is_label_[var]){
      ODBC_->set_column_value_type<std::string> (var);
    }
    else {
      ODBC_->set_column_value_type<plFloat> (var);
    }
  }

  //=============================================================================
  inline plFloat get_data_value(unsigned int var)
  {
    if(is_label_[var]){
      const std::string label = ODBC_->get_column_value<std::string> (var);
      return label_[var].label_to_index(label);
    }
    else {
      return ODBC_->get_column_value<plFloat> (var);
    }
  }
  
  //=============================================================================
  inline bool get_data_record(const plValues* &data_values,
                       const std::vector<bool>*& data_definition)
  {    
    if (ODBC_->next ()==0) { //no error in accessing datasource
      for(unsigned int i= 0; i < current_row_.size(); ++i){
        current_row_def_[i] = ODBC_->get_column_def(i);
        if(current_row_def_[i])
          current_row_[i] = get_data_value(i);
      }
      data_values = &current_row_;
      data_definition = &current_row_def_;
      return true;
    }
    else{
      data_values = 0;
      data_definition = 0;
      return false;
    }
  }
  
  //=============================================================================
  inline unsigned int get_num_fields()const
  {
    return num_fields_;
  }


  /** Get the number of records (rows) in the data set.
      @return the size of the data set.
  */
  virtual unsigned int get_n_records()
  {
    if(!this->is_ok_) return 0;
    
    if(ODBC_){
      return ODBC_->get_num_rows();
    }
    
    return 0;
  }


  /** Set the variables to be associated to the data descriptor.
      This method is to be called when the descriptor is constructed without providing
      the variables list. After this call, observed_variables() will return 
      the passed 'variables' parameter.
      
      @param[in] variables The variables to be associated to the data descriptor
  */
  inline void set_variables(const plVariablesConjunction &variables)
  {
    assert(variables.dim() == get_num_fields() );
    assert(!variables.is_empty());

    variables_ = variables;

    is_label_.assign(get_num_fields(), false);
    label_.resize(get_num_fields());
    for(unsigned int i = 0; i < get_num_fields(); ++i){
      plSymbol s = variables_[i];
      if( s.get_var_type() == PL_LABEL){
        is_label_[i] = true;
        label_[i] = plLabelType( s.get_type() );
      }
    } 

    //data values binding
    for(unsigned int i= 0; i < current_row_.size(); ++i){          
      set_data_type(i);
    }

  }

  virtual plVariablesConjunction observed_variables()
  {
    if(variables_.is_empty())
      throw plError(115);

    return variables_;
  }

  plValues create_row_values() const
  {
    return (!variables_.is_empty() ?
            plValues(variables_) :
            plValues(plArray("X", plRealType(-std::numeric_limits<plFloat>::max(), std::numeric_limits<plFloat>::max()),
                             1, num_fields_)));
  }

protected:
  plDatabaseInitError init();
  void handle(plDatabaseInitError errorId);

  virtual void Output(std::ostream &os)const
  {
    os << "plODBCDataDescriptor: " << std::endl
       << " Host: " << host_  << std::endl
       << " User: " << user_ << std::endl
       << " Query: '" << query_  << "'" << std::endl
       << " Variables: " << variables_;
  }


protected:
  std::string host_;
  std::string user_; 
  std::string passwd_;
  std::string db_;
  std::string query_;
  std::string port_;
  std::string driver_name_;

  plValues current_row_;
  std::vector<bool> current_row_def_;

  bool connection_open_by_descriptor_;
  unsigned int num_fields_;
  plODBCDataConnector *ODBC_;
  plVariablesConjunction variables_;
  std::vector<bool> is_label_;
  std::vector< plLabelType > label_;
};



//===============================================================================================
inline plODBCDataDescriptor::plODBCDataDescriptor( const std::string &host,  const std::string &user,  const std::string &passwd, 
                                            const std::string &db, const std::string &query,
                                            const std::string  &port, const std::string &driver_name)
  : plDataDescriptor()
  , host_(host)
  , user_(user)
  , passwd_(passwd)
  , db_(db)
  , query_(query)
  , port_(port)
  , driver_name_(driver_name)
  , current_row_()
  , current_row_def_()
  , connection_open_by_descriptor_(true)
  , num_fields_(0)
  , ODBC_(0)
  , variables_()
  , is_label_()
  , label_()
{
  plDatabaseInitError errorId = init();
  this->is_ok_ = (errorId == PL_NO_ERROR);

  handle(errorId);
}

//===============================================================================================
inline plODBCDataDescriptor::plODBCDataDescriptor(const plVariablesConjunction& variables,
                                           const std::string &host,  const std::string &user,  const std::string &passwd, 
                                           const std::string &db, const std::string &query,
                                           const std::string  &port, const std::string &driver_name)
  : plDataDescriptor()
  , host_(host)
  , user_(user)
  , passwd_(passwd)
  , db_(db)
  , query_(query)
  , port_(port)
  , driver_name_(driver_name)
  , current_row_()
  , current_row_def_()
  , connection_open_by_descriptor_(true)
  , num_fields_(variables.dim())
  , ODBC_(0)
  , variables_()
  , is_label_(num_fields_, false)
  , label_(num_fields_)
{
  plDatabaseInitError errorId = init();
  this->is_ok_ = (errorId == PL_NO_ERROR);

  handle(errorId);
  set_variables(variables);
}


//===============================================================================================
inline plODBCDataDescriptor::plODBCDataDescriptor( plODBCDataConnector *odbc, const std::string &query,
                                            const std::string  &port, const std::string &driver_name)
  : plDataDescriptor()
  , host_()
  , user_()
  , passwd_()
  , db_()
  , query_(query)
  , port_(port)
  , driver_name_(driver_name)
  , current_row_()
  , current_row_def_()
  , connection_open_by_descriptor_(false)
  , num_fields_(0)
  , ODBC_(odbc)
  , variables_()
  , is_label_()
  , label_()
{
  plDatabaseInitError errorId = init();
  this->is_ok_ = (errorId == PL_NO_ERROR);

  handle(errorId);
}

//===============================================================================================
inline plODBCDataDescriptor::plODBCDataDescriptor( const plVariablesConjunction& variables,
                                            plODBCDataConnector *odbc, const std::string &query,
                                            const std::string  &port, const std::string &driver_name)
  : plDataDescriptor()
  , host_()
  , user_()
  , passwd_()
  , db_()
  , query_(query)
  , port_(port)
  , driver_name_(driver_name)
  , current_row_()
  , current_row_def_()
  , connection_open_by_descriptor_(false)
  , num_fields_(variables.dim())
  , ODBC_(odbc)
  , variables_()
  , is_label_(num_fields_, false)
  , label_(num_fields_)
{
  plDatabaseInitError errorId = init();
  this->is_ok_ = (errorId == PL_NO_ERROR);

  handle(errorId);
  set_variables(variables);
}


//===============================================================================================
inline plDatabaseInitError plODBCDataDescriptor::init()
{
  if(connection_open_by_descriptor_){
    ODBC_ = new plODBCDataConnector;
		
    ODBC_->set_driver(host_, user_, passwd_, db_,port_, driver_name_);
    ODBC_->connect();
  }

  if(!ODBC_->is_connected()) return PL_CONNECTION_ERROR;

  const bool query_ok = ODBC_->run_query(query_);
  if(!query_ok) return PL_QUERY_ERROR;


  const unsigned int nf = ODBC_->get_num_fields();
  // no variables
  if(num_fields_ == 0){
    num_fields_ = nf;
  }
  else{
    if(num_fields_ != nf) {
      const std::string message = "The query results does not match the variables dimension";
      throw plError(82, message);
    }
  }

  is_label_.assign(num_fields_, false);
  label_.resize(num_fields_);

  current_row_=  create_row_values(); //.resize(num_fields_);
  current_row_def_.resize(num_fields_);

  //data values binding
  for(unsigned int i= 0; i < current_row_.size(); ++i){          
    set_data_type(i);
  }

  return PL_NO_ERROR;
}

//===============================================================================================
inline plODBCDataDescriptor::~plODBCDataDescriptor()
{
  if(connection_open_by_descriptor_){
    // Deleting the connector will also disconnect it;
    delete ODBC_;
  }
} 

//===============================================================================================
inline void plODBCDataDescriptor::rewind() 
{
  ODBC_->first();
}

//===============================================================================================
inline void plODBCDataDescriptor::handle( plDatabaseInitError errorId)
{
  std::string message;

  switch(errorId){
  case PL_INITIALIZATION_ERROR : message = "Could not initialize the ODBC driver"; 
    break;
  case PL_CONNECTION_ERROR : message = "Could not open connection to host '" + host_ 
      + "' using database '" + db_ + "'";
    break;
  case PL_QUERY_ERROR : message = "Executing query '" + query_ + "' on host '" + host_ 
      + "' and using database '" + db_ + "' fails";
    break;
  case PL_NO_ERROR : break;
  }

  if(PL_NO_ERROR != errorId) throw plError(82, message);
}


#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
