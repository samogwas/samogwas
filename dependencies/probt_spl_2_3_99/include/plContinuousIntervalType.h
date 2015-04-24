/*=============================================================================
 * Product        : ProBT 
 * File           : plContinuousIntervalType.h
 * Author         : kamel Mekhnacha
 * Creation       : September 2006
 *
 *=============================================================================
 *        (c) Copyright 2006, ProBayes SAS  -  all rights reserved
 *=============================================================================
 *
 *------------------------- Description ---------------------------------------
 *   
 *   
 *-----------------------------------------------------------------------------
*/

#ifndef plContinuousIntervalType_h
#define plContinuousIntervalType_h

#include "plConfig.h"

#include <plMath.h>
#include <plType.h>

/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/


//=============================================================================
/**
   This class implements continuous types defined as intervals.
 */
class PL_DLL_API plContinuousIntervalType: public plContinuousType
{
 private:
  template<typename T>
    void init_with_vals(unsigned int n, const T *interval_values);
  
public:

  /** Default constructor, needed by the serialization code. */
  plContinuousIntervalType();
  
  /** Constructor using the min/max values of the intervals.  A set of n
      intervals is defined using (n+1)-length interval_values as follows:
      { [interval_values(0), interval_values(1) [,
        [interval_values(1), interval_values(2) [, 
      ..., [interval_values(n-1), interval_values(n) [ }.
   */ 
  explicit plContinuousIntervalType(const std::vector <double> &interval_values);

  explicit plContinuousIntervalType(const std::vector <float> &interval_values);

  explicit plContinuousIntervalType(const std::vector <long double> &interval_values);
  
  
  /** Constructor using the min/max values of the intervals. A set of n
      intervals is defined using (n+1)-length interval_values as follows:
      { [interval_values(0), interval_values(1) [,
        [interval_values(1), interval_values(2) [, 
      ..., [interval_values(n-1), interval_values(n) [ }.
  */ 
  plContinuousIntervalType(unsigned int n_plus_one, const double *interval_values);

  plContinuousIntervalType(unsigned int n_plus_one, const float *interval_values);

  plContinuousIntervalType(unsigned int n_plus_one, const long double *interval_values);


  /** Creates a continuous-by-interval type. The interval \em [min,max[ is
      dicretized into \em n_intervals subintervals.*/ 
  plContinuousIntervalType(plFloat min, plFloat max,  unsigned int n_intervals);
  

  /** Destructor of the type */
  virtual ~plContinuousIntervalType();


  /** Returns the number (begining from zero) of the interval corresponding to
      the value 'val'.
      Returns -1 when 'val' is greater or equal to the maximum value.
  */
  int get_interval_number(plFloat val)const;


  /** Promote from a plType. This makes sense because all the information
      is actually in parent class plType. If the object being copied is
      actually of the wrong type, an exception is raised.
  */
  explicit plContinuousIntervalType(const plType&);

  
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
