/*=============================================================================
 * Product        : ProBT 
 * File           : plVersion.h
 * Author         : David Raulo
 * Creation       : Thu Feb 22 16:17:05 2007
 *
 *=============================================================================
 *        (c) Copyright 2006, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */

#ifndef PL_VERSION_H
#define PL_VERSION_H

#include <plConfig.h>
#include <plDLLExport.h>
#include <string>

// ProBT release version. Do not edit!
#define PROBT_MAJOR 2
#define PROBT_MINOR 3
#define PROBT_REVISION 99
#define PROBT_VERSION (PROBT_MAJOR * 10000 \
                     + PROBT_MINOR * 100 \
                     + PROBT_REVISION)

//=============================================================================
/** Holds a ProBT version triplet (major,minor,revision). */
struct PL_DLL_API plVersion
{
  /// Major version
  unsigned int major_;

  /// Minor version
  unsigned int minor_;

  /// Revision
  unsigned int revision_;

  /// Serialization compatibility number
  unsigned int s11n_compat_;

  /// Default constructor
  plVersion() : major_(0), minor_(0), revision_(0), s11n_compat_(0) {}

  /// Constructor
  plVersion(unsigned int maj, unsigned int min, unsigned int rev, 
            unsigned int compat=0)
    : major_(maj), minor_(min), revision_(rev), s11n_compat_(compat) {}

  /// Parse from a "major.minor.revision" string.
  explicit plVersion(const std::string& version_string);

  /// Print as "major.minor.revision".
  std::string print() const;

  /// "less than" operator
  bool operator<(const plVersion& o) const
  {
    return major_<o.major_ 
      || (major_==o.major_
          && (minor_<o.minor_ 
              || (minor_==o.minor_ 
                  && (revision_<o.revision_
                      || (revision_==o.revision_ 
                          && s11n_compat_<o.s11n_compat_)))));
  }

  /// "less than"
  bool less(const plVersion& o) const
  {
    return operator<(o);
  }

  /** Return the ProBT release version. */
  static plVersion get_version();

  /** Get ProBT's expiration date in yyyy-mm-dd format.  Returns "0000-00-00"
      if it is not a time-limited version.
  */
  static std::string get_expiration_date();
  
  /** Returns true if and only if this ProBT package has expired. Once ProBT
      has reached its expiration date, it will raise a plExpirationError
      when trying to create a ProBT object.
  */
  static bool has_expired();
};

//=============================================================================
#endif//PL_VERSION_H

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
