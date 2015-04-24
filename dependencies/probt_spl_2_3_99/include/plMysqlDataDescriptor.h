/*=============================================================================
 * Product        : ProBT 
 * File           : plMysqlDataDescriptor.h
 * Author         : Kamel Mekhnacha
 * Creation       : Tue Feb 20 17:11:54 2007
 *
 *=============================================================================
 *        (c) Copyright 2006, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */

#ifndef plMysqlDataDescriptor_h
#define plMysqlDataDescriptor_h

#include "plConfig.h"

#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#endif

#include <mysql.h>

#include <vector>
#include <string>
#include <iostream>
#include <sstream>

#include <plDataDescriptor.h>
#include <plError.h>
#include <plLabelType.h>
#include <plDatabaseInitError.h>
#include <plRealType.h>
#include <plArray.h>

/** This class implements a data descriptor for Mysql databases.*/
class plMysqlDataDescriptor :public plDataDescriptor
{
  plMysqlDataDescriptor(const plMysqlDataDescriptor&);
  plMysqlDataDescriptor& operator=(const plMysqlDataDescriptor&);

public:

  /** Constructor using database parameters and
      associating it to a set of variables. 

      @param[in] variables The set of variables to be associated to the descriptor.
      @param[in] host Host name (address).
      @param[in] user Mysql user name.
      @param[in] passwd User's password.
      @param[in] db Database name.
      @param[in] query The query to execute.
      @param[in] cache_data @em true if we want to store the data in client's memory.
      @param[in] port Port number (use the default parameter's value when possible).
      @param[in] unix_socket The path to the used unix socket (use the default parameter's value when possible).
      @param[in] client_flag Connection client flag (use the default parameter's value when possible).
  */
  inline plMysqlDataDescriptor(const plVariablesConjunction& variables,
                        const std::string &host, 
			const std::string &user, 
			const std::string &passwd, 
			const std::string &db, 
			const std::string &query,
			bool cache_data = true,
			unsigned int port = 0,
			const std::string &unix_socket = "",
			unsigned long client_flag = 0);

  /** Constructor using database parameters without
      associating it to a set of variables.  The actual association to a set of
      variables could be done later by calling set_variables().

      @param[in] host Host name (address).
      @param[in] user Mysql user name.
      @param[in] passwd User's password.
      @param[in] db Database name.
      @param[in] query The query to execute.
      @param[in] cache_data @em true if we want to store the data in client's memory.
      @param[in] port Port number (use the default parameter's value when possible).
      @param[in] unix_socket The path to the used unix socket (use the default parameter's value when possible).
      @param[in] client_flag Connection client flag (use the default parameter's value when possible).
  */
  inline plMysqlDataDescriptor(const std::string &host, 
			const std::string &user, 
			const std::string &passwd, 
			const std::string &db, 
			const std::string &query,
			bool cache_data = true,
			unsigned int port = 0,
			const std::string &unix_socket = "",
			unsigned long client_flag = 0);

  /** Constructor using an already open connection and associating it to a set
      of variables.
    
      @param[in] variables The set of variables to be associated to the descriptor.
      @param[in,out] mysql_db_connection An already open connection.
      @param[in] query The query to execute.
      @param[in] cache_data @em true if we want to store the data in client's memory.
  */
  inline plMysqlDataDescriptor(const plVariablesConjunction& variables,
                        MYSQL *mysql_db_connection,
			const std::string &query,
			bool cache_data = true);

  /** Constructor using an already open connection without
      associating it to a set of variables.  The actual association to a set of
      variables could be done later by calling set_variables().
    
      @param[in,out] mysql_db_connection An already open connection.
      @param[in] query The query to execute.
      @param[in] cache_data @em true if we want to store the data in client's memory.
  */
  inline plMysqlDataDescriptor(MYSQL *mysql_db_connection,
			const std::string &query,
			bool cache_data = true);
  
  /** Destructor */
  inline ~plMysqlDataDescriptor();

  inline void rewind();


  inline plFloat get_value(unsigned int var)const
  {
    assert( current_char_row_[var] );

    if(is_label_[var]){
      const std::string label(current_char_row_[var]);
      return label_[var].label_to_index(label);
    }

    // !is_label_[var]
       
    std::stringstream s;
    plFloat res;
    s << current_char_row_[var];
    s >> res;
    return res;
  }


  //=============================================================================
  inline bool get_data_record(const plValues* &data_values,
                       const std::vector<bool>*& data_definition)
  {
    if(!result_) {
      data_values = 0;
      data_definition = 0;
      return false;
    }

    is_rewinded_ = false;

    current_char_row_ = mysql_fetch_row(result_);
    if(current_char_row_){
      for(unsigned int i = 0; i < current_row_.size(); ++i){
	if(current_char_row_[i]){
	  //std::stringstream s;
	  //s << current_char_row_[i];
	  //s >> current_row_[i];
          current_row_[i] = get_value(i);
	  current_row_def_[i] = true;
	}
	else current_row_def_[i] = false;
      }
      data_values = &current_row_;
      data_definition = &current_row_def_;
      return true;
    }
  
    data_values = 0;
    data_definition = 0;
    return false;
  }

  //=============================================================================
  inline unsigned int get_num_fields()const
  {
    return num_fields_;
  }


  /** Get the number of records (rows) in the data set.
   
      ATTENTION: if not cached, the first call scans the data
      set and thus has a complexity of O(n) where \em n is the size.
  
      @return the size of the data set.
  */
  virtual unsigned int get_n_records()
  {
    if(!this->is_ok_) return 0;
    
    if(cache_data_ && result_){
      return mysql_num_rows(result_);
    }
    
    return plDataDescriptor::get_n_records();
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
  }
  
  virtual plVariablesConjunction observed_variables()
  {
    if(variables_.is_empty())
      throw plError(115);

    return variables_;
  }

protected:

  plValues create_row_values() const
  {
    return (!variables_.is_empty() ?
            plValues(variables_) :
            plValues(plArray("X", plRealType(-std::numeric_limits<plFloat>::max(), std::numeric_limits<plFloat>::max()),
                             1, num_fields_)));
  }

  plDatabaseInitError init();
  void handle_error(plDatabaseInitError errorId);

  virtual void Output(std::ostream &os)const
  {
    os << "plMysqlDataDescriptor: " << std::endl
       << " Host: " << host_  << std::endl
       << " User: " << user_ << std::endl
       << " Query: '" << query_ << "'" << std::endl
       << " Variables: " << variables_;
  }
 

protected:
  const std::string host_;
  const std::string user_; 
  const std::string passwd_;
  const std::string db_;
  const std::string query_;
  const unsigned int port_;
  const std::string unix_socket_;
  const unsigned long client_flag_;

  const bool cache_data_;


  MYSQL *mysql_db_connection_;
  MYSQL_RES *result_;

  plValues current_row_;
  std::vector<bool> current_row_def_;

  bool connection_open_by_descriptor_;

  unsigned int num_fields_;

  bool is_rewinded_;

  MYSQL_ROW current_char_row_;

  plVariablesConjunction variables_;
  std::vector<bool> is_label_;
  std::vector< plLabelType > label_;
};


//===============================================================================================
inline plMysqlDataDescriptor::plMysqlDataDescriptor(const std::string &host, const std::string &user, const std::string &passwd, 
                                             const std::string &db, const std::string &query,
                                             bool cache_data,
                                             unsigned int port,
                                             const std::string &unix_socket,
                                             unsigned long client_flag)
  : plDataDescriptor()
  , host_(host)
  , user_(user)
  , passwd_(passwd)
  , db_(db)
  , query_(query)
  , port_(port)
  , unix_socket_(unix_socket)
  , client_flag_(client_flag)
  , cache_data_(cache_data)
  , mysql_db_connection_(0)
  , result_(0)
  , current_row_()
  , current_row_def_()
  , connection_open_by_descriptor_(true)
  , num_fields_(0)
  , is_rewinded_(true)
  , current_char_row_(0)
  , variables_()
  , is_label_()
  , label_()
{
  plDatabaseInitError errorId = init();
  this->is_ok_ = (errorId == PL_NO_ERROR);
  
  handle_error(errorId);
}

//===============================================================================================
inline plMysqlDataDescriptor::plMysqlDataDescriptor(const plVariablesConjunction& variables,
                                             const std::string &host, const std::string &user, const std::string &passwd, 
                                             const std::string &db, const std::string &query,
                                             bool cache_data,
                                             unsigned int port,
                                             const std::string &unix_socket,
                                             unsigned long client_flag)
  : plDataDescriptor()
  , host_(host)
  , user_(user)
  , passwd_(passwd)
  , db_(db)
  , query_(query)
  , port_(port)
  , unix_socket_(unix_socket)
  , client_flag_(client_flag)
  , cache_data_(cache_data)
  , mysql_db_connection_(0)
  , result_(0)
  , current_row_()
  , current_row_def_()
  , connection_open_by_descriptor_(true)
  , num_fields_(variables.dim())
  , is_rewinded_(true)
  , current_char_row_(0)
  , variables_()
  , is_label_(num_fields_,false)
  , label_(num_fields_)
{
  plDatabaseInitError errorId = init();
  this->is_ok_ = (errorId == PL_NO_ERROR);
  
  handle_error(errorId);

  set_variables(variables);
}


//===============================================================================================
inline plMysqlDataDescriptor::plMysqlDataDescriptor(MYSQL *mysql_db_connection,
                                             const std::string &query,
                                             bool cache_data)
  : plDataDescriptor()
  , host_()
  , user_()
  , passwd_()
  , query_(query)
  , port_()
  , unix_socket_()
  , client_flag_(0)
  , cache_data_(cache_data)
  , mysql_db_connection_(mysql_db_connection)
  , result_(0)
  , current_row_()
  , current_row_def_()
  , connection_open_by_descriptor_(false)
  , num_fields_(0)
  , is_rewinded_(true)
  , current_char_row_(0)
  , variables_()
  , is_label_()
  , label_()
{
  plDatabaseInitError errorId = init();
  this->is_ok_ = (errorId == PL_NO_ERROR);

  handle_error(errorId);
}

//===============================================================================================
inline plMysqlDataDescriptor::plMysqlDataDescriptor(const plVariablesConjunction& variables,
                                             MYSQL *mysql_db_connection,
                                             const std::string &query,
                                             bool cache_data)
  : plDataDescriptor()
  , host_()
  , user_()
  , passwd_()
  , query_(query)
  , port_()
  , unix_socket_()
  , client_flag_(0)
  , cache_data_(cache_data)
  , mysql_db_connection_(mysql_db_connection)
  , result_(0)
  , current_row_()
  , current_row_def_()
  , connection_open_by_descriptor_(false)
  , num_fields_(variables.dim())
  , is_rewinded_(true)
  , current_char_row_(0)
  , variables_()
  , is_label_(num_fields_, false)
  , label_(num_fields_)
{
  plDatabaseInitError errorId = init();
  this->is_ok_ = (errorId == PL_NO_ERROR);

  handle_error(errorId);
  set_variables(variables);
}

//===============================================================================================
inline void plMysqlDataDescriptor::handle_error(plDatabaseInitError errorId)
{
  std::string message;
  switch(errorId){
  case PL_INITIALIZATION_ERROR : message = "Could not initialize the mysql driver"; 
    break;
  case PL_CONNECTION_ERROR : message = "Could not open connection to host '" + host_ 
      + "' using database '" + db_ + "'";
    break;
  case PL_QUERY_ERROR : message = "Executing query '" + query_ + " on host '" + host_ 
      + "' and using database '" + db_ + "' fails";
    break;
  case PL_NO_ERROR : break;
  }

  if(PL_NO_ERROR != errorId) throw plError(82, message);

}

//===============================================================================================
inline plDatabaseInitError plMysqlDataDescriptor::init()
{
  if(connection_open_by_descriptor_){
    mysql_db_connection_ = mysql_init(mysql_db_connection_);
    if(!mysql_db_connection_) return PL_INITIALIZATION_ERROR;
    
    MYSQL *connection_result = mysql_real_connect(mysql_db_connection_, 
						  host_.c_str(),
						  user_.c_str(),
						  passwd_.c_str(),
						  db_.c_str(), 
						  port_, 
						  (unix_socket_ == "") ? 0 : unix_socket_.c_str(),
						  client_flag_);
    if(!connection_result){
      return PL_CONNECTION_ERROR;
    }
  }
  
  if(!mysql_db_connection_) return PL_CONNECTION_ERROR;

  int query_not_ok = mysql_query(mysql_db_connection_, query_.c_str());
  if(query_not_ok){
    return PL_QUERY_ERROR;
  }
  
  if(cache_data_) result_ = mysql_store_result(mysql_db_connection_);
  else result_ = mysql_use_result(mysql_db_connection_);

  unsigned int nf = mysql_num_fields(result_);
  // no variables
  if(num_fields_ == 0){
    num_fields_ = nf;
  }
  else{
    if(num_fields_ != nf) {
      const std::string message = "The query results does not match variables diemension";
      throw plError(82, message);
    }
  }

  is_label_.assign(num_fields_, false);
  label_.resize(num_fields_);

  current_row_ = create_row_values(); //:resize(num_fields_);
  current_row_def_.resize(num_fields_);
  
  return PL_NO_ERROR;
}

//===============================================================================================
inline plMysqlDataDescriptor::~plMysqlDataDescriptor()
{
  if(result_) mysql_free_result(result_);

  if(connection_open_by_descriptor_ && mysql_db_connection_) mysql_close(mysql_db_connection_);
}

//===============================================================================================
inline void plMysqlDataDescriptor::rewind()
{
  if(!mysql_db_connection_) return;

  if(is_rewinded_) return;

  is_rewinded_ = true;
  
  if(cache_data_ && result_) mysql_data_seek(result_, 0);
  else{
    int query_not_ok = mysql_query(mysql_db_connection_, query_.c_str());
    if(!query_not_ok){
      if(result_) mysql_free_result(result_);
      result_ = mysql_use_result(mysql_db_connection_);
    }
    else{
      std::string message = "Executing query '" + query_ + "' on host '" + host_ 
	+ "' and using database '" + db_ + "' fails";
      throw plError(82, message);
    }
  }
}



#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
