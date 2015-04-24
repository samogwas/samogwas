/// \file plError.h

/*=============================================================================
 * Product        : ProBT 
 * File           : plError.h
 * Author         : Juan-Manuel Ahuactzin
 * Creation       : Thu Apr 12 12:32:08 2001
 *
 *=============================================================================
 *     (c) Copyright 2000, Centre National de la Recherche Scientifique,
 *                              all rights reserved
 *=============================================================================
 *
 *------------------------- Description ---------------------------------------
 *   
 *   
 *-----------------------------------------------------------------------------
*/

#ifndef plError_h
#define plError_h

#include <plConfig.h>

#include <plSPL.h>
#include <plDLLExport.h>
#include <plObject.h>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>

/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/

SPL_DECL1(plError);

/** A @em plError is an exception thrown when a fatal error is encountered
    in ProBT.

    Every kind of @em plError has an integer code accessible with
    get_error_number().  It can be output to a stream or a string by using
    operator<<(); in addition to the error code, this output contains a
    human-readable description of the error.

    When thrown, a @em plError prints itself to the standard error output. This
    behaviour can be disabled globally by using plError::always_display() or for
    a given error number by using plError::ignore_this_message().
*/
class PL_DLL_API plError: public plObject, public std::exception
{
  SPL_ACCESS1(plError);

protected:
  static const char *messages_[];
  static std::vector <bool> ignore_message_;
  static std::vector <bool> treat_this_warning_as_error_;

  /** Whether to abort when displaying a warning. */
  static bool treat_warnings_as_errors_;

  /** Whether to display a plError on cerr at its creation */
  static bool always_display_error_;

  /** Whether to display at warn() time */
  static bool always_display_warning_;

  static std::ostream *output_stream_;

  /** Identifies the error number */
  unsigned int error_number_ ;

  /** Human-readable description of the error.
   */
  std::string full_msg_;

  /** Additional human-readable description of erros context.
   */
  std::string additional_info_;

  /** Return the message for the given error id. */
  static const std::string main_msg(unsigned int error_id);

  static void display_msg(const std::string &msg);

  /** Displays the message error according to the error number in the output
      stream 'out' */
  void Output(std::ostream &out) const;

public:
  /** Default constructor. */
  plError();
  
  /** Creates an error number \em error_id with no additional
      information */
  explicit plError(unsigned int error_id);

  /** Creates an error with number @em error_id, and additional information. */
  plError(unsigned int error_id, const std::string &additional_info);

  /** Erases the object. */
  virtual ~plError() throw();

  /** Returns a human-readable string representing the error. */
  virtual const char* what() const throw();

  /** Same as Output(). Could probably be deprecated, and then removed. */
  void display(std::ostream &out = std::cerr) const { Output(out); }

  /** Returns the error number. */
  unsigned int get_error_number()const;

  /** Set the global display flag for errors. When @em true, all errors will be
      displayed as soon as they are created. When @em false, they will
      not be displayed. In all cases, errors are C++ exceptions: you may
      catch them using @em try and @em catch, and inspect them using what()
      and get_error_number().

      @param display_flag New value of the global display flag.
  */
  static void always_display(bool display_flag);

  /** Inhibits the display of a warning or error, given its number. This is useful
      if you are sure that a given warning is not pertinent in your particular
      setting. This function has no effect if the given message number is invalid.

      @param id_message Number of the warning or error to ignore.

      @param ignoring_state Whether to ignore the message (@em true),
      or display it (@em false).
  */
  static void ignore_this_message(unsigned int id_message, 
				  bool ignoring_state);

  /**
     Tells whether a warning or error is currently ignored.
     This will return @em true unless a warning/error is specifically
     ignored ; calls to always_display_warning() and always_display() have no
     influence on the return of this function. This function
     returns @em false if the given message number is invalid.

     @param id_message Number of the warning or error.
     @return @em true if and only if the warning is currently ignored.
   */
  static bool message_is_ignored(unsigned int id_message);

  //---------------------------------------------------------------------------
  /** Inhibits the display of all warnings.

      @param display_flag Whether to hide all warnings (@em false) or display
      all those that are not ignored (@em true).
  */
  static void always_display_warning(bool display_flag);

  /** Makes all warnings (ignored or not, displayed or not) call abort().

      @param wie_flag Whether to call abort() when the warning is raised (@em true),
      or just display it and go on with execution (@em false).

      This method is obsolete, use treat_warnings_as_errors() instead.
  */
  inline static void abort_in_warning(bool wie_flag)
  {
    warn(112, "calling 'abort_in_warning()'; use 'treat_warnings_as_errors()'"
         " instead");
    treat_warnings_as_errors(wie_flag);
  }

  /** Makes all warnings (ignored or not, displayed or not) raise an exception.

      @param wie_flag Whether to raise an exception when the warning is raised (@em true),
      or just display it and go on with execution (@em false).
  */
  static void treat_warnings_as_errors(bool wie_flag=true);


  /** Makes the warning with id @em id_warning (ignored or not, displayed or not)
      raise an exception.

      @param id_warning Number of the warning.
      @param wae_flag Whether to raise an exception when the warning is raised (@em true),
      or just display it and go on with execution (@em false).
  */
  static void treat_this_warning_as_error(unsigned int id_warning, bool wae_flag=true);

  /** Prints a ProBT warning. Normally, this is not fatal.
      However, warnings can be made fatal globally by calling
      @code plError::abort_in_warning(true); @endcode

      @param error_id Number of the warning to raise.
  */
  static void warn(unsigned int error_id);

  /** Prints a ProBT warning with additional information. Normally, this is not fatal.
      However, @em warnings can be made fatal globally by calling
      @code plError::abort_in_warning(true); @endcode

      @param error_id Number of the warning to raise.
      @param additional_info Additional information that details the warning.
  */
  static void warn(unsigned int error_id, const std::string &additional_info);

  /** Get a human-readable description of the error.
   */
  std::string get_main_message()const
  {
    return main_msg(error_number_);
  }

  /** Get a human-readable description of error's context.
   */
  const std::string& get_additional_info()const
  {
    return additional_info_;
  }

  /** Changes the output stream in which errors and warings are displayed.
      The default one is std::cerr.
  */
  static inline void set_output_stream(std::ostream *outstream)
   {
     if(outstream){
       output_stream_ = outstream;
     }
   }
};

class plIgnoreMessage {
private:
  unsigned int id_;
  bool previous_state_;
public:
  explicit plIgnoreMessage(unsigned int id) :
    id_(id),
    previous_state_(plError::message_is_ignored(id))
  {
    plError::ignore_this_message(id, true);
  }
  ~plIgnoreMessage() {
    plError::ignore_this_message(id_, previous_state_);
  }
};

#define THROW_PL_ERROR(n, msg)                          \
  do {                                                  \
    std::ostringstream pl_error_m;			\
    pl_error_m << msg;					\
    throw plError(n, pl_error_m.str());			\
  } while (0)

#define PL_WARN(n, msg)                                  \
  do {                                                   \
    std::ostringstream pl_error_m;                       \
    pl_error_m << msg;                                   \
    plError::warn(n, pl_error_m.str());                  \
  } while (0)

//=============================================================================
#endif //plError_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
