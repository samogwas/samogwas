/*=============================================================================
 * Product        : ProBT 
 * File           : plTimer.h
 * Author         : Kamel Mekhnacha
 * Creation       : Wed Mar 12 18:18:32 2003
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

#ifndef plTimer_h
#define plTimer_h

#include "plConfig.h"

#include <plDLLExport.h>

#define PL_USE_SYSTIME

//#if defined(WIN32) || defined(_WIN32)
#if defined(_MSC_VER) // MS VC++
#undef PL_USE_SYSTIME
#endif

#include <ctime>
#ifdef PL_USE_SYSTIME
#include <sys/time.h>
#endif

#include <cstdlib>


class PL_DLL_API plTimer 
{
protected:
  double tstart_;
  double tstop_;
#ifdef PL_USE_SYSTIME
  struct timeval tv_;
#endif
  bool activated_;
public:
  inline plTimer();
  inline void start();
  inline void stop();
  inline double get_time_sec() const;
  inline double get_time_msec() const;
  inline void activate_for_time_sec(double time_in_sec);
  inline void activate_for_time_msec(double time_in_sec);
  inline bool is_activated() const;
  inline bool expired() const;
  inline void reset();

  static plTimer timer; 

};


//=============================================================================
inline plTimer::plTimer()
  :tstart_(0.0), tstop_(0.0),
#ifdef PL_USE_SYSTIME
  tv_(),
#endif
  activated_(false)
{}

//=============================================================================
inline void plTimer::reset()
{
  tstart_ = tstop_ = 0.0;
  activated_ = false;
}


//=============================================================================
inline bool plTimer::is_activated() const
{
  return activated_;
}


#ifdef PL_USE_SYSTIME

static inline double plSeconds(timeval const & tv) {
  return ((double) tv.tv_sec) + ((double) tv.tv_usec) * 1e-6;
}

//=============================================================================
inline void plTimer::start()
{
  gettimeofday(&tv_,0);
  tstart_ = plSeconds(tv_);
 }

//=============================================================================
inline void plTimer::stop()
{
  gettimeofday(&tv_,0);
  tstop_ = plSeconds(tv_);
}

//=============================================================================
inline double plTimer::get_time_sec() const
{
  return tstop_ - tstart_;
}
 
//=============================================================================
inline double plTimer::get_time_msec() const
{
  return (tstop_ - tstart_)*1000.0;
}

//=============================================================================
inline void plTimer::activate_for_time_sec(double time_in_sec)
{
  gettimeofday(&tv_,0);
  tstart_ = plSeconds(tv_);

  tstop_ = tstart_ + time_in_sec;
  activated_ = true;
}

//=============================================================================
inline void plTimer::activate_for_time_msec(double time_in_msec)
{
  gettimeofday(&tv_,0);
  tstart_ = plSeconds(tv_);

  tstop_ = tstart_ + time_in_msec*1e-3;
  activated_ = true;
}

//=============================================================================
inline bool plTimer::expired() const
{
  if(!activated_) return false;

  struct timeval ltv;
  gettimeofday(&ltv,0);
  double current_time = plSeconds(ltv);

  return (current_time >= tstop_);
}


#else
//=============================================================================
inline void plTimer::start()
{
  tstart_ = clock();
}

//=============================================================================
inline void plTimer::stop()
{
  tstop_ = clock();
}

//=============================================================================
inline double plTimer::get_time_sec() const
{
  return (tstop_ - tstart_)/CLOCKS_PER_SEC;
}
 
//=============================================================================
inline double plTimer::get_time_msec() const
{
  return (tstop_ - tstart_)*1000.0/CLOCKS_PER_SEC;
}


//=============================================================================
inline void plTimer::activate_for_time_sec(double time_in_sec)
{
  tstart_ = clock();
  tstop_ = tstart_ + time_in_sec*CLOCKS_PER_SEC;
  activated_ = true;
}

//=============================================================================
inline void plTimer::activate_for_time_msec(double time_in_msec)
{
  tstart_ = clock();
  tstop_ = tstart_ + time_in_msec*1e-3*CLOCKS_PER_SEC;
  activated_ = true;
}

//=============================================================================
inline bool plTimer::expired() const
{
  if(!activated_) return false;

  return (clock() >= tstop_);
}

#endif

#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
