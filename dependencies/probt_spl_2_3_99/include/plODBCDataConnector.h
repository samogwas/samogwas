/*=============================================================================
 * Product        : ProBT
 * File           : plODBCDataConnector.h
 * Author         : Samir Ouifi
 * Creation       : Mon Jul 30 13:42:08 2007
 *
 *=============================================================================
 *        (c) Copyright 2007, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */

#ifndef PLODBCDATACONNECTOR_H
#define PLODBCDATACONNECTOR_H

#include "plConfig.h"

#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#endif

#include <string>
#include <vector>

/* ODBC driver headers */
#include <sql.h>
#include <sqlext.h>


/** This class implements an ODBC data sources connector. */
class plODBCDataConnector 
{
public :

  /** Constructor	*/	
  inline plODBCDataConnector();

  /** Destructor */
  inline ~plODBCDataConnector()
  {
    disconnect();
  }

  /** Define connection parameters.

  @param[in] db_server Host name (address)
  @param[in] db_login Database user name
  @param[in] db_password User's password
  @param[in] db_name Database name
  @param[in] db_port Port number
  @param[in] driver_name Driver name
  */
  inline void set_driver(const std::string &db_server,
                         const std::string &db_login, 
                         const std::string &db_password, 
                         const std::string &db_name,
                         const std::string &db_port,
                         const std::string &driver_name);

  /** Connect to the database. */
  inline bool connect();

  /** Disconnect from the database. */
  inline void disconnect();

  /** Execute a query using the database connection. 

  @param[in] strQuery the query to be executed.

  @return @em true if the query is executed successfully.
  Return @em false otherwise.
  */
  inline bool run_query(const std::string &strQuery);

  /** Obtain the number of fields in the query.

  @return the number of fields in the query
  */
  inline int get_num_fields()const;

  /**
     Return the number of rows returned by the query.

     @return the number of rows returned by the query
  */
  inline int get_num_rows()const;

  /** Goto the first row of query result. */
  inline SQLRETURN first();

  /** Goto the next row of query result. */
  inline SQLRETURN next();

  /** Set a given column type.

  @param[in] numColumn column's number.

  This template method is instantiated for @em int, @em unsigned @em
  int, @em float, @em double, @em long @em double, and std::string
  */
  template<typename T>
  inline void set_column_value_type(unsigned int numColumn);

  /** Obtain the value of a given column.

  @param[in] numColumn column's number

  @return the value of the column

  This template method is instantiated for @em int, @em unsigned @em
  int, @em float, @em double, @em long @em double, and std::string
  */
  template<typename T>
  inline T get_column_value(unsigned int numColumn)const;

  /** Return @em true if the column is defined (not NULL). 
      Return @em false if the column is not defined (NULL).

      @return @em true if the column is defined (not NULL). 
      Return @em false if the column is not defined (NULL).
  */
  inline bool get_column_def(unsigned int numColumn)const;

  /** Return connection status.

  @return @em true if the connection is established.
  Return @em false otherwise.
  */
  inline bool is_connected()const
  {
    return is_connected_;
  }

private :
#define STR_LEN 1024
  struct CHARARRAY
  {
    SQLCHAR data_[STR_LEN];
  };

  std::vector<SQLLEN> columnLength_;
  std::vector<SQLINTEGER>  ivalue_;
  std::vector<SQLUINTEGER> uivalue_;
  std::vector<SQLDOUBLE>   dvalue_;
  std::vector<CHARARRAY> svalue_;
  std::string login_;
  std::string password_;
  std::string server_;
  std::string dbName_;
  std::string driver_;
  std::string sDriver_;
  std::string sString_;
  std::string sReturn_;
  std::string sSQLCommand_;
  std::string port_;
  std::string driver_name_;

  SQLHENV SQLEnvir_;   // environnement handle
  SQLHDBC SQLDbc_;     // connection handle
  SQLHSTMT SQLStmt_;   // statement handle
  mutable SQLRETURN SQLError_; // error code
  SQLCHAR SQLState_[5];
  SQLCHAR SQLMessage_[100];

  bool is_connected_;
};


//===============================================================================================
inline plODBCDataConnector::plODBCDataConnector()
  : ivalue_(0)
  , uivalue_(0)
  , dvalue_(0)
  , svalue_(0)
  , SQLStmt_(0)
  , is_connected_(false)
{
}

//===============================================================================================
inline void plODBCDataConnector::set_driver(const std::string &db_server, const std::string &db_login, 
                                            const std::string &db_password, const std::string &db_name,
                                            const std::string &db_port, const std::string &db_driver_name)
{
  login_   = db_login; 
  password_= db_password;
  server_  = db_server;
  dbName_  = db_name;
  port_    = db_port;
  driver_name_    = "DRIVER=" + db_driver_name;

  sDriver_ = 
    this->driver_name_ + 
    ";Server=" + 
    this->server_ +  
    ";Port=" + 
    this->port_ + 
    ";Database=" + 
    this->dbName_  + 
    ";UID=" + this->login_; 

  if (!this->password_.empty()) 
    sDriver_ += ";PWD=" + this->password_;

  sDriver_ += ";";

  //cout << sDriver_ << endl;
}

//===============================================================================================
inline bool plODBCDataConnector::connect()
{
  // environment handle
  SQLError_ = SQLAllocHandle (SQL_HANDLE_ENV, SQL_NULL_HANDLE, &SQLEnvir_);
  //cout << "environment handle " ;

  if (SQLError_ != SQL_SUCCESS && SQLError_ != SQL_SUCCESS_WITH_INFO)
    {
      goto BDD_SQLCommandSqlExit;
    }

  // environment attribute
  SQLError_ = SQLSetEnvAttr (SQLEnvir_, SQL_ATTR_ODBC_VERSION,
                             (SQLPOINTER) SQL_OV_ODBC3, SQL_IS_INTEGER);

  //cout << "environment attribute " ;

  if (SQLError_ != SQL_SUCCESS && SQLError_ != SQL_SUCCESS_WITH_INFO)
    {
      goto BDD_SQLCommandSqlExit;
    }

  // connection handle

  //cout << "connection handle " ;
  SQLError_ = SQLAllocHandle (SQL_HANDLE_DBC, SQLEnvir_, &SQLDbc_);
  if (SQLError_ != SQL_SUCCESS && SQLError_ != SQL_SUCCESS_WITH_INFO)
    {
      goto BDD_SQLCommandSqlExit;
    }

  //cout << "connection attribute ";
  // connection attribute
  SQLError_ = SQLSetConnectAttr (SQLDbc_, SQL_ATTR_ACCESS_MODE,
                                 (SQLPOINTER) SQL_MODE_READ_WRITE, 0);
  if (SQLError_ != SQL_SUCCESS && SQLError_ != SQL_SUCCESS_WITH_INFO)
    {
      goto BDD_SQLCommandSqlExit;
    }

  //cout << "driver connect : " << sDriver.c_str() << endl;
  SQLError_ = SQLDriverConnect (SQLDbc_, 0,(SQLCHAR*) sDriver_.c_str(), 
                                sDriver_.length (),0, 0, 0, SQL_DRIVER_COMPLETE);

  if (SQLError_ != SQL_SUCCESS && SQLError_ != SQL_SUCCESS_WITH_INFO)
    {
      goto BDD_SQLCommandSqlExit;
    }

  is_connected_ = true;

  return true;

 BDD_SQLCommandSqlExit:
  // close handles
  SQLFreeHandle (SQL_HANDLE_DBC, SQLDbc_);
  SQLFreeHandle (SQL_HANDLE_ENV, SQLEnvir_);
  return false;
}

//===============================================================================================
inline void plODBCDataConnector::disconnect()
{
  if(is_connected_){
    if(SQLStmt_) SQLFreeHandle (SQL_HANDLE_STMT, SQLStmt_);
    SQLDisconnect (SQLDbc_);
    SQLFreeHandle (SQL_HANDLE_DBC, SQLDbc_);
    SQLFreeHandle (SQL_HANDLE_ENV, SQLEnvir_);
    is_connected_ = false;
  }
}

//===============================================================================================
inline bool plODBCDataConnector::run_query(const std::string &strQuery)
{
  if(!is_connected_) return false;

  // Free last query resources
  if(SQLStmt_) SQLFreeHandle (SQL_HANDLE_STMT, SQLStmt_);

  // open statement handle
  SQLError_ = SQLAllocHandle (SQL_HANDLE_STMT, SQLDbc_, &SQLStmt_);
  if (SQLError_ != SQL_SUCCESS && SQLError_ != SQL_SUCCESS_WITH_INFO)
    {
      return true;
    }

  // execute SQL command.
  SQLSetStmtAttr(SQLStmt_,SQL_ATTR_CURSOR_SCROLLABLE,(void*) SQL_SCROLLABLE, 0);
  SQLError_ = SQLExecDirect (SQLStmt_, (SQLCHAR*) strQuery.c_str(),
                             strQuery.length());

  int nb = this->get_num_fields();

  ivalue_.resize(nb);
  uivalue_.resize(nb);
  dvalue_.resize(nb);
  svalue_.resize(nb);
  columnLength_.resize(nb);

  if (SQLError_ != SQL_SUCCESS && SQLError_ != SQL_SUCCESS_WITH_INFO)
    {
      // get description of error
      SQLError_ = SQLGetDiagRec (SQL_HANDLE_STMT, SQLStmt_, 1, SQLState_, 0,
                                 SQLMessage_, sizeof(SQLMessage_), 0);		

      SQLFreeHandle (SQL_HANDLE_STMT, SQLStmt_);
      return false;
    }

  return true;
}

//===============================================================================================
inline int plODBCDataConnector::get_num_fields()const
{
  if(!is_connected_) return 0;

  SQLSMALLINT numFields=0;
  SQLError_ = SQLNumResultCols (SQLStmt_,&numFields);
  if (SQLError_ != SQL_SUCCESS && SQLError_ != SQL_SUCCESS_WITH_INFO)
    {
      return -1;
    }

  return numFields;
}

//===============================================================================================
inline int plODBCDataConnector::get_num_rows()const
{
 if(!is_connected_) return 0;

  SQLLEN numRows=0;
  
  SQLError_ = SQLRowCount (SQLStmt_,&numRows);
  if (SQLError_ != SQL_SUCCESS && SQLError_ != SQL_SUCCESS_WITH_INFO)
    {
      return 0;
    }

  return numRows;
}

//===============================================================================================
inline bool plODBCDataConnector::get_column_def(unsigned int numColumn)const
{
  return ( columnLength_[numColumn] != SQL_NULL_DATA);
}

//===============================================================================================
inline SQLRETURN plODBCDataConnector::next()
{
  SQLError_ = SQLFetchScroll (SQLStmt_,SQL_FETCH_NEXT,1);

  return SQLError_;
}

//===============================================================================================
inline SQLRETURN plODBCDataConnector::first()
{
  SQLError_ = SQLFetchScroll (SQLStmt_,SQL_FETCH_ABSOLUTE,0);
  return SQLError_;
}

//===============================================================================================  
template<>
inline void plODBCDataConnector::set_column_value_type<std::string>(unsigned int numColumn)
{	
  //svalue_[numColumn]="";columnLength_[numColumn]=0;
  SQLError_ = SQLBindCol (SQLStmt_,
                          numColumn+1,
                          SQL_C_CHAR,
                          &svalue_[numColumn].data_,
                          sizeof(svalue_[numColumn].data_),
                          &columnLength_[numColumn]);
}

//===============================================================================================  
template<>
inline void plODBCDataConnector::set_column_value_type<double>(unsigned int numColumn)
{	
  //dvalue_[numColumn]=0.0;columnLength_[numColumn]=0;
  SQLError_ = SQLBindCol (SQLStmt_,
                          numColumn+1,
                          SQL_C_DOUBLE,
                          &dvalue_[numColumn],
                          sizeof(dvalue_[numColumn]),
                          &columnLength_[numColumn]);
}

//===============================================================================================	
template<>
inline void plODBCDataConnector::set_column_value_type<long double>(unsigned int numColumn)
{
  set_column_value_type<double>(numColumn);
}

//===============================================================================================	
template<>
inline void plODBCDataConnector::set_column_value_type<float>(unsigned int numColumn)
{
  set_column_value_type<double>(numColumn);
}

//===============================================================================================	
template<>
inline void plODBCDataConnector::set_column_value_type<int>(unsigned int numColumn)
{
  //ivalue_[numColumn]=0;columnLength_[numColumn]=0;
  SQLError_ = SQLBindCol (SQLStmt_,
                          numColumn+1,
                          SQL_C_LONG,
                          &ivalue_[numColumn],
                          sizeof(ivalue_[numColumn]),
                          &columnLength_[numColumn]);
}

//===============================================================================================
template<>
inline void plODBCDataConnector::set_column_value_type<unsigned int>(unsigned int numColumn)
{
  //uivalue_[numColumn]=0;columnLength_[numColumn]=0;
  SQLError_ = SQLBindCol (SQLStmt_,
                          numColumn+1,
                          SQL_C_ULONG,
                          &uivalue_[numColumn],
                          sizeof(uivalue_[numColumn]),
                          &columnLength_[numColumn]);
}

//===============================================================================================
template<>
inline std::string plODBCDataConnector::get_column_value<std::string>(unsigned int numColumn)const
{
  std::string res;
  for(int i = 0; i < columnLength_[numColumn];++i){
    res += svalue_[numColumn].data_[i];
  }

  return res;
}

//===============================================================================================
template<>
inline double plODBCDataConnector::get_column_value<double>(unsigned int numColumn)const
{
  return dvalue_[numColumn];
}

//===============================================================================================
template<>
inline long double plODBCDataConnector::get_column_value<long double>(unsigned int numColumn)const
{
  return get_column_value <double>(numColumn);
}

//===============================================================================================
template<>
inline float plODBCDataConnector::get_column_value<float>(unsigned int numColumn)const
{
  return get_column_value<double>(numColumn);
}

//===============================================================================================
template<>
inline int plODBCDataConnector::get_column_value<int>(unsigned int numColumn)const
{
  return ivalue_[numColumn];
}

//===============================================================================================
template<>
inline unsigned int plODBCDataConnector::get_column_value<unsigned int>(unsigned int numColumn)const
{
  return uivalue_[numColumn];
}


#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
