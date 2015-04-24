/*=============================================================================
 * Product        : ProBT 
 * File           : plDiscreteIntervalType.h
 * Author         : Kamel Mekhnacha
 * Creation       : May 2011
 *
 *=============================================================================
 *        (c) Copyright 2010, ProBayes SAS  -  all rights reserved
 *=============================================================================
 *
 *------------------------- Description ---------------------------------------
 *   
 *   
 *-----------------------------------------------------------------------------
 */

#ifndef plDiscreteIntervalType_h
#define plDiscreteIntervalType_h

#include "plConfig.h"

#include <plMath.h>
#include <plType.h>

//=============================================================================
/**
   This class implements discretized continuous types defined as intervals.  

   Calculation using this type assumes that the variable is discrete taking a
   finite set of values corresponding to the middle of the provided
   intervals. Thus, tabulation and marginalization over this variable will be
   exact (exhaustive) by using this finite set of values. If you need to assume
   that the variable is continuous by interval then consider using the type @em
   plContinuousIntervalType.
   
   \sa plContinuousIntervalType
*/
class PL_DLL_API plDiscreteIntervalType: public plDiscreteType
{
 private:
  template<typename T>
    void init_with_vals(unsigned int n, const T *interval_values);
  
 public:

  /** Default constructor, needed by the serialization code. */
  plDiscreteIntervalType();
  
  /** Constructor using the min/max values of the intervals.  A set of n
      intervals is defined using (n+1)-length interval_values as follows:
      { [interval_values(0), interval_values(1) [,
      [interval_values(1), interval_values(2) [, 
      ..., [interval_values(n-1), interval_values(n) [ }.
  */
  explicit plDiscreteIntervalType(const std::vector <double> &interval_values);

  explicit plDiscreteIntervalType(const std::vector <float> &interval_values);
  
  explicit plDiscreteIntervalType(const std::vector <long double> &interval_values);
  
  
  /** Constructor using the min/max values of the intervals. A set of n
      intervals is defined using (n+1)-length interval_values as follows:
      { [interval_values(0), interval_values(1) [,
      [interval_values(1), interval_values(2) [, 
      ..., [interval_values(n-1), interval_values(n) [ }.
  */
  plDiscreteIntervalType(unsigned int n_plus_one, const double *interval_values);

  plDiscreteIntervalType(unsigned int n_plus_one, const float *interval_values);

  plDiscreteIntervalType(unsigned int n_plus_one, const long double *interval_values);


  /** Creates a continuous-by-interval type. The interval \em [min,max[ is
      dicretized into \em n_intervals subintervals.*/ 
  plDiscreteIntervalType(plFloat min, plFloat max,  unsigned int n_intervals);
  

  /** Destructor of the type */
  virtual ~plDiscreteIntervalType();


  /** Returns the number (begining from zero) of the interval corresponding to
      the value 'val'.
      Returns -1 when 'val' is greater or equal to the maximum value.
  */
  int get_interval_number(plFloat val)const;


  /** Promote from a plType. This makes sense because all the information
      is actually in parent class plType. If the object being copied is
      actually of the wrong type, an exception is raised.
  */
  explicit plDiscreteIntervalType(const plType&);

  
  /** Get the interval boundaries. */
  std::vector<plFloat> get_values() const;
    
};


#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
