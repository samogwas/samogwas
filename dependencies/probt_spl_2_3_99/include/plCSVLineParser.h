/*=============================================================================
 * Product        : ProBT 
 * File           : plCSVLineParser.h
 * Author         : Kamel Mekhnacha
 * Creation       : Tue Feb 20 17:10:57 2007
 *
 *=============================================================================
 *        (c) Copyright 2006, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */

#ifndef plCSVLineParser_h
#define plCSVLineParser_h

#include "plConfig.h"

#include <plDLLExport.h>

#include <string>
#include <vector>


/** This class implements parsing utilities for a CSV file.*/
class PL_DLL_API plCSVLineParser
{
 private:
  // disable copy and assignment
  plCSVLineParser(const plCSVLineParser&);
  plCSVLineParser & operator=(const plCSVLineParser &);

 public:
  /** Constructs a CSV line parser.
      
      @param[in] field_delimiter the field separator used in the CSV data. It's
      default value is ';' and it can be set later using set_field_delimiter().

      @param[in] line_delimiter The line separator used in the CSV data. It's
      default value is '\n' and it can be set later using set_line_delimiter().

      @param[in] undefined_field The value that will correspond to an undefined
      field in the CSV data.  Its default value is "" and it can be set later using
      set_undef_value_string(). In addition to this value, an empty field always
      corresponds to an undefined field.

      @param[in] quote The character to be used for quoting text fields. The default is
      " and it can be set later using set_quote().
  */
  explicit plCSVLineParser(char field_delimiter = ';',
                           char line_delimiter = '\n',
                           const std::string &undefined_field = "",
                           char quote = '"')
    : field_delimiter_(field_delimiter)
    , line_delimiter_(line_delimiter)
    , quote_(quote)
    , undef_value_string_(undefined_field)
    , current_line_buffer_("")
    , current_text_(0)
    {}
	
  /** Descructor */
  virtual ~plCSVLineParser()
  {}
    
  /** Sets the field delimiter to 'field_delimiter'.*/
  void set_field_delimiter(char field_delimiter)
  { field_delimiter_ = field_delimiter;}

  /** Sets the field delimiter to 'field_delimiter'.*/
  void set_separator(char field_delimiter)
  { field_delimiter_ = field_delimiter;}
	
  /** Sets line end character */
  void set_line_delimiter(char lend)
  { line_delimiter_ = lend;}
	
  /** Defines the string representing undefined values in the CSV file.*/
  void set_undef_value_string(const std::string &undef_str)
  { undef_value_string_ = undef_str;}

  /** Sets the quote character to 'quote' */
  void set_quote(char quote)
  { quote_ = quote; }
  
  /** Read a new line from the input stream.
      Return false iff the eof is reached.
  */
  bool read_line(std::istream &data_stream);


  /** Read an item from the current line. Return false iff the item is not
      defined or the end of the line is reached.  

      This template method is instantiated for @em std::string, @em
      unsigned @em int, @em int, @em float, @em double, and @em long @em double
      types.  
  */
  template <typename T>
    bool read_from_current_line(T &item)const;

  /** Read a line and convert it to a vector of strings.
      Return false iff the eof is reached.
  */
  bool read_line_as_string_vector(std::istream &data_stream,
                                  std::vector<std::string> &string_vector);
	

  /** Get a const reference to the current line text buffer */
  inline const std::string &get_current_line_buffer() const
  {
    return current_line_buffer_;
  }

  /** Get the field delimiter */
  char get_field_delimiter() const
  {
    return field_delimiter_;
  }
 protected:
  
  bool read_from_line(const char * &line, std::string &st) const;
  bool read_from_line(const char * &line, unsigned int &num) const;
  bool read_from_line(const char * &line, int &num) const;
  bool read_from_line(const char * &line, float &num) const;
  bool read_from_line(const char * &line, double &num) const;
  bool read_from_line(const char * &line, long double &num) const;
	

  void set_current_text(const char *text){current_text_ = text;}

  bool get_string_token(const char *line, 
                        unsigned int &b, 
                        unsigned int &l, 
                        unsigned int &p)const;
    
 protected:

  char field_delimiter_;
  char line_delimiter_;
  char quote_;
	
  std::string undef_value_string_;

  std::string current_line_buffer_;
  
  mutable const char *current_text_;
};


#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
