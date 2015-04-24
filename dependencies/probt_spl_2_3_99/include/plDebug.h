/*=============================================================================
 * Product        : ProBT 
 * File           : plDebug.h
 * Author         : David Raulo
 * Creation       : Tue Mar 13 11:58:25 2007
 *
 *=============================================================================
 *        (c) Copyright 2006, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */

#ifndef PL_DEBUG_H
#define PL_DEBUG_H

#include "plConfig.h"
#include <plDLLExport.h> // DEBUG

#ifdef __GNUC__
#include <cxxabi.h>
#endif
#include <typeinfo>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>

//-----------------------------------------------------------------------------
// A helper class for tracing function calls while debugging.
// This is exported in the DLL in order to let the testing framework
// use it.
struct PL_DLL_API plTracer
{
private:
  std::string str_;
  static unsigned int count;

public:
  static void message( std::string const & msg );

  static unsigned int depth() { return count; }

  plTracer( const plTracer& other );
  plTracer& operator=( const plTracer& other );
  plTracer( std::string const & str );
  ~plTracer();
};

//-----------------------------------------------------------------------------
/// Macros that print something unconditionnally (in debug and release modes,
/// and whatever the variables in config.mak are).
/// This is intended for temporary debug sessions. Don't leave these macros
/// uncommented in the CVS.

/// A debugging message, with location information.
#define PL_MSG(x)                                               \
  do {                                                          \
    std::ostringstream msg;                                     \
    msg << "(DD) " << __FILE__ << ":" << ((int) __LINE__)       \
        << " " << __FUNCTION__ << " " << x << "\n";             \
    std::cerr << msg.str(); /* std::cerr.flush(); */            \
  } while (0)

/// An indented nested trace. Will print a begin/end block that starts
/// with the macro, and ends with the current code block.
/// Nested calls to this will produce pretty indented traces.
#define PL_TRACE(a)                                             \
  std::ostringstream pl_trace_msg__;                            \
  pl_trace_msg__ << "(DD) " << __FUNCTION__ << ": " << a;       \
  plTracer tracer(pl_trace_msg__.str())

/// A message, nicely indented, for use together with PL_TRACE(),
/// when you don't want a begin/end block, but just a single message.
#define PL_TRACE_MSG(a)						\
  do { std::ostringstream pl_trace_msg__;                       \
    pl_trace_msg__ << "(DD) " << __FUNCTION__ << ": " << a;     \
    plTracer::message(pl_trace_msg__.str()); } while (0)

/// Same as PL_TRACE_MSG(), except that you pass it a function or method that
/// can output something to an ostream.
/// For instance:
///   PL_TRACE_MSG_FUN(my_object.output_data);
#define PL_TRACE_MSG_FUN(f)					\
  do { std::ostringstream pl_trace_msg__;                       \
    pl_trace_msg__ << "(DD) " << __FUNCTION__ << ": ";          \
    f(pl_trace_msg__);                                          \
    pl_trace_msg__ << std::endl;                                \
    plTracer::message(pl_trace_msg__.str()); } while (0)

//-----------------------------------------------------------------------------
/// Macros that can be enabled through config.mak. In all cases, they are
/// all disabled in release mode.
/// They behave the same as their unconditional counterparts above.
/// This is intended for debugging messages for complete subsystems. As the
/// messages they produce will not appear in release builds, you may leave
/// them uncommented in the code without problem.

#if defined(DEBUG) && defined(PL_ENABLE_DEBUG_TRACE)
# define PL_DEBUG_TRACE(msg)     PL_TRACE(msg)
# define PL_DEBUG_TRACE_MSG(msg) PL_TRACE_MSG(msg)
#else
# define PL_DEBUG_TRACE(msg)     /* nothing */
# define PL_DEBUG_TRACE_MSG(msg) /* nothing */
#endif

//-----------------------------------------------------------------------------
// Use the C++ RTTI to extract the class-name.
template< typename T >
std::string pl_rt_type( const T& o )
{
#ifdef __GNUC__
  const std::type_info & ti = typeid(o);
  int status;	
  char* realname = abi::__cxa_demangle( ti.name(), 0, 0, &status );
  std::string result = realname;
  free(realname);
  return result;
#else	
  return "(non-printable type)";
#endif
}

//-----------------------------------------------------------------------------
// Use the C++ RTTI to extract the class-name, and print the object.
template< typename T >
std::ostream& pl_print_object_info( std::ostream& out, const T* o )
{
  out << "Object at " << std::hex << o << std::dec << " is a " 
      << pl_rt_type(*o) << ": " << *o << std::endl;
  return out;
}

//-----------------------------------------------------------------------------
#endif //PL_DEBUG_H

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
