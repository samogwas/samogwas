/*===========================================================================
 * Product        : ProBT
 * File           : plCSVDataDescriptor.h
 * Author         : Ronan Le Hy <ronan.lehy@probayes.com>
 *
 *===========================================================================
 *        (c) Copyright 2008, ProBayes SAS  -  all rights reserved
 *===========================================================================*/

#ifndef PLCSVDATADESCRIPTOR_H
#define PLCSVDATADESCRIPTOR_H

#include "plConfig.h"
#include <plValues.h>
#include <plDataDescriptor.h>
#include <plVariablesConjunction.h>
#include <string>
#include <vector>
#include <iostream>

class kplCSVDataDescriptorInterface;

/** A data descriptor for data in CSV (comma-separated values) format. It can parse
    the data from a file or a file descriptor. It is slower than plCSVFileDataDescriptor.
    However, it is safer (it will detect and report parse errors), and may be easier to use:
    - by default, it allows several common CSV syntaxes
    - it allows to specify the variables and order of the columns inside the file
    - it allows using an input stream instead of a file.

    It is based on RFC 4180: http://tools.ietf.org/html/rfc4180, with the following extensions:
    - records are ended/separated by either CR, LF, CRLF, or any sequence of mixed CR and LF
    - empty lines are skipped
    - leading and trailing spaces and tabulations around a quoted field are ignored
    - each field can be quoted with either &quot; (double quote) or ' (single quote)
    (customisable using set_quotes())
    - fields can be separated with either , (comma) or ; (semicolon)
    (customisable using set_field_separators())
    - an empty field is considered undefined
    - additionally, a field equal to " " (one space) is also considered undefined
    (customisable using set_undefined_field()).
*/
class PL_DLL_API plCSVDataDescriptor : public plDataDescriptor, virtual public plStringDataDescriptor
{
 private:
  kplCSVDataDescriptorInterface * impl_;

  // Disable copying.
  plCSVDataDescriptor(plCSVDataDescriptor const &);
  plCSVDataDescriptor & operator=(plCSVDataDescriptor const &);

 public:
  /** Constructs a CSV data descriptor over a file containing CSV data.

      @param[in] file The name of the file containing the CSV data. The file must exist and be readable.
      
      @param[in] variables A conjunction describing the variables described in the file.
      - If header_specifies_variables is false, this should be the exact ordered list of
      variables present in the file. get_data_record() will return values for all these variables.
      - If header_specifies_variables is true, by default, this should be a superset of the variables
      announced in the file header. get_data_record() will return values for the variables present in
      the file header.
      - If header_specifies_variables is true and you called ignore_unknown_variables(), get_data_record()
      will return values for variables that are both in this parameter and in the file header (that is to
      say, the intersection of these two sets).
      
      @param[in] cache_data Whether this descriptor should cache the data, in order to avoid
      parsing after the first rewind(). If memory allows it, this provides a significant speed
      boost when several passes are made over the data.
      
      @param[in] header_specifies_variables Whether the first line of the file contains a header
      listing the names of the variables present in the file (in order). This header
      allows having superfluous values in the CSV data, and reordering the variables in the data.
  */
  plCSVDataDescriptor(std::string const & file, plVariablesConjunction const & variables = plVariablesConjunction(),
		      bool cache_data = true, bool header_specifies_variables = true);

  /** Constructs a CSV data descriptor over an input stream containing CSV data.

      @param input_stream The input stream containing the CSV data.
      @param variables A conjunction containing a superset of the variables described in the stream.
      All variables in this conjunction must have different names. If header_specifies_variables is false,
      this must correspond exactly to the variables specified in the CSV data.
      @param cache_data Whether this descriptor should cache the data, in order to avoid
      parsing after the first rewind(). If memory allows it, this provides a significant speed
      boost when several passes are made over the data.
      @param header_specifies_variables Whether the first line of the file contains a header
      listing the names of the variables present in the file (in order). This header
      allows having superfluous values in the CSV data, and reordering the variables in the data.
  */
  plCSVDataDescriptor(std::istream & input_stream, plVariablesConjunction const & variables,
		      bool cache_data = true, bool header_specifies_variables = true);

  /** Sets the field separators allowed in the CSV data. Any character of the passed string
      will be accepted as a field separator. Note that a field separator cannot be made of several
      consecutive characters.

      The default value is ",;".
  */
  void set_field_separators(std::string const & field_separators);

  /** Sets the quotes allowed in the CSV data. Any character of the passed string will
      be accepted as a quoting character. Note that there is no way to use different
      starting and ending characters for a quote, or a multi-character quote.

      The default value is &quot;\\&quot;'&quot;: single and double quotes.
  */
  void set_quotes(std::string const & quotes);

  /** Sets the value that will correspond to an undefined field in the CSV data. Note
      that there is no way to specify several possible values for an undefined field.
      
      In addition to this value, an empty field always corresponds to an undefined field.
      
      The default value is " ": a string of exactly one space.
  */
  void set_undefined_field(std::string const & undefined_field);

 public:
  /** When there is a header describing the file variables (header_specifies_variables = true),
      this lets the parser ignore the unknown variables and their corresponding data columns.
      The "known" variables are those that were passed to the constructor.

      By default, unknown variables trigger an exception when parsing the header.
      
      When the file header is not used (header_specifies_variables = false), this
      has no effect.
      
      Calling this method once the file header has already been parsed has no effect.
  */
  void ignore_unknown_variables(bool ignore = true);

  /** Destructor.
   */
  virtual ~plCSVDataDescriptor();

  /** Returns the number of fields in the CSV data.
   */
  virtual unsigned int get_num_fields() const;

  /** Returns the variables actually present in the CSV file or stream, in order.

      If header_specifies_variables was false at construction time, this returns the
      variables passed to the constructor.

      If header_specifies_variables was true at construction, this corresponds to
      the data header. Therefore, this triggers parsing of the file or stream. In this
      case, you should make sure to call set_quotes(), set_field_separators(),
      set_undefined_field() or ignore_unknown_variables() (if necessary)
      before calling this method.
  */
  plVariablesConjunction observed_variables();

  /** Get the next data record (row) and advances by one record in the data

      \param[out] data_values a pointer to a plValues containing assignments
      for all variables described in the record.

      \param[out] data_definition a vector stating, for each value in \em
      data_values, whether it's defined or not.

      \return \em false iif the end of the container is reached.

      The returned pointers are managed internally: you should not attempt to
      delete them, or use them after another call to get_data_record(). Also,
      you should not use them when get_data_record() returns \em false.

      This method triggers parsing of the file or stream. Therefore,
      you should make sure to call set_quotes(), set_field_separators(),
      set_undefined_field() or ignore_unknown_variables() (if necessary)
      before calling this method.
  */
  virtual bool get_data_record(plValues const * & data_values,
                               const std::vector<bool>* &data_definition);

  /** Returns the raw, uninterpreted data record as a vector of strings.

      @param[out] data_values Record element, resized as needed and filled in by this method.

      @return false iff the end of the container is reached.

      This method consumes one line of CVS input.

      This method triggers parsing of the file or stream. Therefore,
      you should make sure to call set_quotes() or set_field_separators()
      (if necessary) before calling this method. Methods set_undefined_field() and
      ignore_unknown_variables() have no effect on what this methods returns.

      Warning: when the cache is enabled, there are important rules to follow to avoid losing data:
      - after calling this method, you should always call rewind() before calling
      get_data_record(plValues const * & data_values, data_definition)
      - once you have called get_data_record(plValues const * & data_values, data_definition) once, you should
      not call this method anymore, even after doing a rewind().

      There are no such restrictions when the descriptor's cache is disabled.
  */
  virtual bool get_data_record(std::vector<std::string> & data_values);

  /** Rewinds the descriptor, by making it restart from the beginning of the data.
      This is always possible for a descriptor attached to a file. For a descriptor
      attached to a stream, this will work if the descriptor is cached; if it is
      not cached, an error will be raised (since there is no way to rewind an input
      stream).
  */
  virtual void rewind();



  using plStringDataDescriptor::guess_variables;

  /** Tries to guess the variables described in a CSV file, by parsing it.
      @param[in] csv_file Name of the CSV file.
      @param[in] header Whether the CSV file contains a header describing the variable names.
      @param[in] real_discretization Discretization to apply to real variables (treated as plContinuousIntervalType). A negative value prescribes no discretization: plRealType is used.
      @return A conjunction of variables corresponding to the variables in the CSV file.
  */
  static plVariablesConjunction guess_variables(std::string const & csv_file,
                                                bool header = true, int real_discretization = -1) {
    plCSVDataDescriptor desc(csv_file, plVariablesConjunction(), false, false);
    desc.ignore_unknown_variables();

    return desc.guess_variables(header, real_discretization);
  }

 protected:
  /** Output a human-readable representation of the descriptor.
   */
  virtual void Output(std::ostream & os) const;
};


#endif /* PLCSVDATADESCRIPTOR_H */

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
