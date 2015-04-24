/*=============================================================================
 * Product        : ProBT 
 * File           : plProbability32.h
 * Author         : Kamel Mekhnacha
 * Creation       : September 2001
 * 
 *=============================================================================
 *     (c) Copyright 2000, Centre National de la Recherche Scientifique,
 *                              all rights reserved
 *=============================================================================
 *
 *------------------------- Description ---------------------------------------
 *   Implements a new arithmetics to handle probabilities.
 *   
 *   
 *-----------------------------------------------------------------------------
 */

#ifndef plProbability_h
#define plProbability_h

#include "plConfig.h"

#include <plDLLExport.h>
#include <plError.h>
#include <iostream>
#include <cmath>
#include <cstdio>
#include <errno.h>
#include <string>
#include <climits>
//#include <cassert>

// isfinite and isnan seem to be macros on android
#undef isfinite
#undef isnan

// Portable integers on 32 and 64 bits architectures
// ATTENTION: this is not portable on 16-bits architectures
typedef int PL_INT32;
typedef unsigned int PL_UINT32;
typedef unsigned long long PL_UINT64;

// A more portable code that needs boost/cstdint.hpp is:
// #include <boost/cstdint.hpp>
// typedef boost::int32_t  PL_INT32;
// typedef boost::uint32_t PL_UINT32;
// typedef boost::uint64_t PL_UINT64;

#define PL_F_SIZE 31
#define PL_MASK_32 0x80000000
#define PL_MASK_31 0x40000000
#define PL_MASK_30 0x20000000
#define PL_MASK_29 0x10000000

#define PL_MAX_U31_BITS 0x7FFFFFFF
#define PL_MAX_31_BITS  0x3FFFFFFF
#define PL_MAX_32_BITS  0x3FFFFFFF
#define PL_MIN_31_BITS (-0x3FFFFFFF)

#define PL_BIT_32(v) ( (v) & PL_MASK_32)
#define PL_BIT_31(v) ( (v) & PL_MASK_31)

#define PL_LOG_BASE2(v) (std::log(v)/0.69314718055994530942)

#ifndef PL_LOG_OF_10_BASE2
#define PL_LOG_OF_10_BASE2 3.321928094887362660
#endif

#ifndef PL_LOG_OF_E_BASE2
#define PL_LOG_OF_E_BASE2 1.442695040888963
#endif

class plProbability;

namespace std
{
  /** Base 2 logarithm function */
  double log2(const plProbability &pr);
  
  /** Natural logarithm function */
  double log(const plProbability &pr);
  
  /** Base 10 logarithm function */
  double log10(const plProbability &pr);
  
//   /** The pow() function  returns the value of pr raised to the power of exp */
//   plProbability pow( const plProbability &pr, PL_INT32 exp );

  bool isfinite(const plProbability &x);

  bool isnan(const plProbability &x);
};
 
 

/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/

SPL_DECL2(plProbability);

// This class implements specialized new arithmetics for probabilities.
class PL_DLL_API plProbability
{
  SPL_ACCESS2(plProbability);

protected:
  PL_UINT32 f_;
  PL_INT32 e_;
public:
  /** Constructors */

  /** The default void constructor */
  inline plProbability();

  /** The conversion constructor from a "long double" */
  inline plProbability(const long double &f);

  /** The conversion constructor from a "double" */
  inline plProbability(const double &f);
  
  /** The conversion constructor from a "float" */
  inline plProbability(const float &f);

  /** The conversion constructor from a "int" */
  inline plProbability(const int &i);

  /** The conversion constructor from a "long int" */
  inline plProbability(const long int &i);

  /** The conversion constructor from a "unsigned int" */
  inline plProbability(const unsigned int &i);

    /** The conversion constructor from a "unsigned long" */
  inline plProbability(const unsigned long &i);

  /** The constructor using the "f" and "e" values. Internal constructor. Never use it!!!*/
  inline plProbability( PL_UINT32 f, PL_INT32 e);

  /** Conversion to "float" operator. */
  inline operator float() const;

  /** Conversion to "double" operator. */
  inline operator double() const;

  /** Conversion to "long double" operator. */
  inline operator long double() const;

  /** The "+" operator */
  inline  plProbability operator+(const plProbability& pc) const;
  inline  plProbability operator+(const long double& fl) const;
  inline  plProbability operator+(const double& fl) const;
  inline  plProbability operator+(const float& fl) const;

  /** The "-" operator */
  inline  plProbability operator-(const plProbability& pc) const;
  inline  plProbability operator-(const long double& fl) const;
  inline  plProbability operator-(const double& fl) const;
  inline  plProbability operator-(const float& fl) const;

  /** The "+=" operator */
  inline  plProbability& operator+=(const plProbability& pc);
  inline  plProbability& operator+=(const long double& fl);
  inline  plProbability& operator+=(const double& fl);
  inline  plProbability& operator+=(const float& fl);

  /** The "-=" operator */
  inline  plProbability& operator-=(const plProbability& pc);
  inline  plProbability& operator-=(const long double& fl);
  inline  plProbability& operator-=(const double& fl);
  inline  plProbability& operator-=(const float& fl);
  
  /** The "*" operator */
  inline  plProbability operator*(const plProbability& pc) const;
  inline  plProbability operator*(const long double& fl) const;
  inline  plProbability operator*(const double& fl) const;
  inline  plProbability operator*(const float& fl) const;
  
  /** The "*=" operator */
  inline  plProbability& operator*=(const plProbability& pc);
  inline  plProbability& operator*=(const long double& fl);
  inline  plProbability& operator*=(const double& fl);
  inline  plProbability& operator*=(const float& fl);

  /** The "/" operator */
  inline  plProbability operator/(const plProbability& pc) const;
  inline  plProbability operator/(const long double& fl) const;
  inline  plProbability operator/(const double& fl) const;
  inline  plProbability operator/(const float& fl) const;

  /** The "/=" operator */
  inline  plProbability& operator/=(const plProbability& pc);
  inline  plProbability& operator/=(const long double& fl);
  inline  plProbability& operator/=(const double& fl);
  inline  plProbability& operator/=(const float& fl);
	
  /** Comparison operators */

  inline bool operator==(const plProbability& p) const; 
  inline bool operator==(const long double&) const;
  inline bool operator==(const double&) const;
  inline bool operator==(const float&) const;
  
  inline bool operator!=(const plProbability& p) const; 
  inline bool operator!=(const long double&) const;
  inline bool operator!=(const double&) const;
  inline bool operator!=(const float&) const;
  

  inline bool operator<=(const plProbability& p) const;
  inline bool operator<=(const long double&) const;
  inline bool operator<=(const double&) const;
  inline bool operator<=(const float&) const;
  
  inline bool operator>=(const plProbability& p) const;
  inline bool operator>=(const long double&) const;
  inline bool operator>=(const double&) const;
  inline bool operator>=(const float&) const;

  inline bool operator<(const plProbability& p) const;
  inline bool operator<(const long double&) const;
  inline bool operator<(const double&) const;
  inline bool operator<(const float&) const;
  
  inline bool operator>(const plProbability& p) const;
  inline bool operator>(const long double&) const;
  inline bool operator>(const double&) const;
  inline bool operator>(const float&) const;

  /** Returns "true" is the value is "NaN" and "false" otherwise. */
  inline bool isNaN() const;

  /** Returns "true" is the value is "Infinity" and "false" otherwise. */
  inline bool isInfinity() const;
  
  /** Base 2 logarithm function */
  friend inline double std::log2(const plProbability &pr);
  
  /** Natural logarithm function */
  friend inline double std::log(const plProbability &pr);
  
  /** Base 10 logarithm function */
  friend inline double std::log10(const plProbability &pr);
  
//   /** The pow() function  returns the value of pr raised to the power of exp */
//   friend inline plProbability std::pow( const plProbability &pr, PL_INT32 exp );
  
 
  // Friend oprerators
  
  /** Streaming operators "<<" */
  friend inline std::ostream& operator<<(std::ostream& out, const plProbability& p);
  friend inline std::istream& operator>>(std::istream& out, plProbability& p);
  
  
  friend inline long double operator*(const long double& theFloat, const plProbability& p);
  friend inline double operator*(const double& theFloat, const plProbability& p);
  friend inline float operator*(const float& theFloat, const plProbability& p);

  friend inline long double operator/(const long double& theFloat, const plProbability& p);
  friend inline double operator/(const double& theFloat, const plProbability& p);
  friend inline float operator/(const float& theFloat, const plProbability& p);
  

  // Defining some constants
  static const plProbability plZeroProbaConstant;
  static const plProbability plOneProbaConstant;
  static const plProbability plTwoProbaConstant;
  static const plProbability plHalfProbaConstant;

  static const plProbability plInfinityProbaConstant;
  static const plProbability plNaNProbaConstant;
};

//=============================================================================
// Returns "true" is the value is "NaN" and "false" otherwise.
inline bool plProbability::isNaN() const
{
  return *this == plNaNProbaConstant;
}

//=============================================================================
// Returns "true" is the value is "Infinity" and "false" otherwise.
bool plProbability::isInfinity() const
{
  return *this == plInfinityProbaConstant;
}

/** Constructors */

/** Conversion  constructors  */

#ifdef PL_NOT_IEEE754
// We use a generic (but slow) method!!!
//=============================================================================
inline plProbability::plProbability(const long double &fl)
{
  if(fl < 0.0L) {
    *this = plNaNProbaConstant;
    return;
  }

  if(fl == 0.0L){
    *this = plZeroProbaConstant;
  }
  else{
    e_ = (PL_INT32)(PL_LOG_BASE2(fl))+1;
    f_ = (PL_UINT32) (fl*std::pow(2.0, PL_F_SIZE - (double)e_ ));

    if(PL_BIT_32(f_)){
      f_ >>=1; e_++;
    }
    else{
      if( (f_!=0) && (!PL_BIT_31(f_)) ) {
	while(!PL_BIT_31(f_)) {
	  f_ <<=1; e_--;
	}
      }
    }
  }
}

//=============================================================================
inline plProbability::plProbability(const double &fl)
{
  if(fl < 0.0) {
    *this = plNaNProbaConstant;
    return;
  }
  
  if(fl == 0.0){
    *this = plZeroProbaConstant;
  }
  else{
    e_ = (PL_INT32)(PL_LOG_BASE2(fl))+1;
    f_ = (PL_UINT32) (fl*std::pow(2.0, PL_F_SIZE - (double)e_ ));

    if(PL_BIT_32(f_)){
      f_ >>=1; e_++;
    }
    else{
      if( (f_!=0) && (!PL_BIT_31(f_)) ) {
	while(!PL_BIT_31(f_)) {
	  f_ <<=1; e_--;
	}
      }
    }
  }
}

//=============================================================================
inline plProbability::plProbability(const float &fl)
{
  if(fl < 0.0f) {
    *this = plNaNProbaConstant;
    return;
  }
  
  if(fl == 0.0f){
    *this = plZeroProbaConstant;
  }
  else{
    e_ = (PL_INT32)(PL_LOG_BASE2(fl))+1;
    f_ = (PL_UINT32) (fl*std::pow(2.0, PL_F_SIZE - (double)e_ ));

    if(PL_BIT_32(f_)){
      f_ >>=1; e_++;
    }
    else{
      if( (f_!=0) && (!PL_BIT_31(f_)) ) {
	while(!PL_BIT_31(f_)) {
	  f_ <<=1; e_--;
	}
      }
    }
  }
}


// Conversion to float
//=============================================================================
inline plProbability::operator float() const
{
  if (*this == plZeroProbaConstant) return 0.0f;

  return  f_ * std::pow(2.0, ((double)e_ - PL_F_SIZE));
}

// Conversion to double
//=============================================================================
inline plProbability::operator double() const
{
  if (*this == plZeroProbaConstant) return 0.0;

  return  f_ * std::pow(2.0, ((double)e_ - PL_F_SIZE));
}

// Conversion to long double
//=============================================================================
inline plProbability::operator long double() const
{
  if (*this == plZeroProbaConstant) return 0.0L;

  return  f_ * std::pow(2.0, ((double)e_ - PL_F_SIZE));
}

#else // PL_NOT_IEEE754 is defined
// We use a precific method for IEEE754 machines 


// Masks and macros for long double conversion
#define PL_MASK_E_LD  0x7FFF
#define PL_GET_E_LD(f) (f & PL_MASK_E_LD)

// Masks and macros for double conversion
#define PL_MASK_E_D  0x7FF00000
#define PL_MASK_FH_D 0x000FFFFF
#define PL_GET_E_D(f) ((f & PL_MASK_E_D) >> 20)
#define PL_GET_FH_D(f)(f & PL_MASK_FH_D)

// Masks and macros for float conversion
#define PL_MASK_E_S  0x7F800000
#define PL_MASK_FH_S  0x007FFFFF
#define PL_GET_E_S(f) ((f & PL_MASK_E_S) >> 23)
#define PL_GET_FH_S(f)(f & PL_MASK_FH_S)

typedef struct {PL_UINT32 w1, w2;} TwoWords;
typedef union {TwoWords longPart; double doublePart;} DoubleUnion;


typedef union {	PL_UINT32 longPart;	float floatPart;} FloatUnion;

// Converting a "long double" to a plProbability
//=============================================================================
inline plProbability::plProbability(const long double &ld)
{
  if(ld < 0.0L) {
    *this = plNaNProbaConstant;
    return;
  }

  if(ld == 0.0L){
    *this = plZeroProbaConstant;
  }
  else{
    
    DoubleUnion dCopy;
    dCopy.doublePart = double(ld);
    PL_UINT32 dh, dl;
#ifdef __BIG_ENDIAN__
    dh = dCopy.longPart.w1;
    dl = dCopy.longPart.w2;
#else
    dh = dCopy.longPart.w2;
    dl = dCopy.longPart.w1;
#endif
    f_ = ( (PL_GET_FH_D(dh)  | 0x00100000) << 10) | (dl >> 22) ;
    e_ = PL_GET_E_D(dh) - 1022;
  }
}

// Converting a "double" to a plProbability
// This is a hidden-bit representation!!!
//=============================================================================
inline plProbability::plProbability(const double &fl)
{
  if(fl < 0.0) {
    *this = plNaNProbaConstant;
    return;
  }

  if(fl == 0.0){
    *this = plZeroProbaConstant;
  }
  else{
    DoubleUnion dCopy;
    dCopy.doublePart = fl;
    PL_UINT32 dh, dl;
#ifdef __BIG_ENDIAN__ 
    dh = dCopy.longPart.w1;
    dl = dCopy.longPart.w2;
#else
    dh = dCopy.longPart.w2;
    dl = dCopy.longPart.w1;
#endif
    f_ = ( (PL_GET_FH_D(dh)  | 0x00100000) << 10) | (dl >> 22) ;
    e_ = PL_GET_E_D(dh) - 1022;
  }
}

// Converting a "float" to a plProbability
// This is a hidden-bit representation!!!
//=============================================================================
inline plProbability::plProbability(const float &fl)
{
  if(fl < 0.0f) {
    *this = plNaNProbaConstant;
    return;
  }

  if(fl == 0.0f){
    *this = plZeroProbaConstant;
  }
  else{
    FloatUnion flCopy;
    flCopy.floatPart = fl;
    PL_UINT32 af = flCopy.longPart;
    f_ =  ((PL_GET_FH_S(af) | 0x00800000) << 7);
    e_ = PL_GET_E_S(af)- 126;
  }
}



// Conversion to float
//=============================================================================
inline plProbability::operator float() const
{
  if(f_ == 0) return 0.0f;
  if(e_ < -126) return 0.0f;
  if(e_ > 0x000000FF) 
    return std::numeric_limits<float>::infinity();
  
  FloatUnion flCopy;
  PL_UINT32 fe = ( (e_+126) << 23 );
  PL_UINT32 ff = ( (f_ & 0x3FFFFFFF) >> 7);
  PL_UINT32 fint = ( (fe | ff) & 0x7FFFFFFF);
  flCopy.longPart = fint;
  
  return flCopy.floatPart;
}


// Conversion to double
//=============================================================================
inline plProbability::operator double() const
{
  if(f_ == 0) return 0.0;
  if(e_ < -1022) return 0.0;
  if(e_ > 0x000007FF)
    return std::numeric_limits<double>::infinity();
  
  DoubleUnion dCopy;
  PL_UINT32 dh =  ( ((e_ + 1022) << 20) | ( (f_ & 0x3FFFFC00) >> 10) ) & 0x7FFFFFFF;
  PL_UINT32 dl =  (f_ & 0x000003FF) << 22;
  
  
#ifdef __BIG_ENDIAN__
  dCopy.longPart.w1 = dh;
  dCopy.longPart.w2 = dl;
#else
  dCopy.longPart.w2 = dh;
  dCopy.longPart.w1 = dl;
#endif
  
  return dCopy.doublePart;
}

// Conversion to long double
//=============================================================================
inline plProbability::operator long double() const
{
  return (long double) (double(*this));
}


#endif // PL_NOT_IEEE754

//=============================================================================
inline plProbability::plProbability(const int &i)
{
  *this = double(i);
}

//=============================================================================
inline plProbability::plProbability(const unsigned int &i)
{
  *this = double(i);
}

//=============================================================================
inline plProbability::plProbability(const unsigned long &i)
{
  *this = double(i);
}


//=============================================================================
inline plProbability::plProbability(const long int &i)
{
  *this = double(i);
}


//=============================================================================
inline plProbability::plProbability()
 : f_(0)
 , e_(0)
{
}

//=============================================================================
inline plProbability::plProbability(PL_UINT32 f,  PL_INT32 e)
  : f_(f)
  , e_(e)
{}




// Comparison operators
//=============================================================================
inline bool plProbability::operator==(const plProbability& p) const
{
  return ( (f_==p.f_) && (e_==p.e_) );
}

//=============================================================================
inline bool plProbability::operator==(const long double& fl) const 
{
  return ( (*this) == plProbability(fl) ); 
} 


//=============================================================================
inline bool plProbability::operator==(const double& fl) const 
{
  return ( (*this) == plProbability(fl) ); 
} 


//=============================================================================
inline bool plProbability::operator==(const float& fl) const 
{
  return ( (*this) == plProbability(fl) ); 
} 


//=============================================================================
inline bool plProbability::operator!=(const plProbability& p) const
{
  return (!(*this == p));
}

//=============================================================================
inline bool plProbability::operator!=(const long double& fl) const 
{ 
  return ( (*this) != plProbability(fl) ); 
} 

//=============================================================================
inline bool plProbability::operator!=(const double& fl) const 
{ 
  return ( (*this) != plProbability(fl) ); 
} 

//=============================================================================
inline bool plProbability::operator!=(const float& fl) const 
{ 
  return ( (*this) != plProbability(fl) ); 
} 




//=============================================================================
inline bool plProbability::operator<=(const plProbability& p) const
{
  if(f_ == 0) return true;
  if(p.f_ == 0) return false;

  if(e_ < p.e_) return true;
  if(e_ == p.e_) return (f_ <= p.f_);
  return false;
}

//=============================================================================
inline bool plProbability::operator<=(const long double& fl) const 
{
  if(fl < 0.0L) return false; 
  return ( (*this) <= plProbability(fl) ); 
}


//=============================================================================
inline bool plProbability::operator<=(const double& fl) const 
{
  if(fl < 0.0) return false; 
  return ( (*this) <= plProbability(fl) ); 
}

//=============================================================================
inline bool plProbability::operator<=(const float& fl) const 
{
  if(fl < 0.0f) return false; 
  return ( (*this) <= plProbability(fl) ); 
}


//=============================================================================
inline bool plProbability::operator<(const plProbability& p) const
{
  if(f_ == 0) return (p.f_ != 0);
  if(p.f_ == 0) return false;
  if(e_ < p.e_) return true;
  if(e_ == p.e_) return (f_ < p.f_);
  return false;
}

//=============================================================================
inline bool plProbability::operator<(const long double& fl) const 
{
  if(fl <= 0.0L) return false; 
  return ( (*this) < plProbability(fl) );
} 

//=============================================================================
inline bool plProbability::operator<(const double& fl) const 
{
  if(fl <= 0.0) return false;
  return ( (*this) < plProbability(fl) );
} 

//=============================================================================
inline bool plProbability::operator<(const float& fl) const 
{
  if(fl <= 0.0f) return false; 
  return ( (*this) < plProbability(fl) );
} 



//=============================================================================
inline bool plProbability::operator>=(const plProbability& p) const 
{
  if(p.f_ == 0) return true;
  if(f_ == 0) return false;
  if(e_ > p.e_) return true;
  if(e_ == p.e_) return (f_ >= p.f_);
  return false;
}

//=============================================================================
inline bool plProbability::operator>=(const long double& fl) const
{
  if(fl <= 0.0L) return true; 
  return ( (*this) >= plProbability(fl) );
}


//=============================================================================
inline bool plProbability::operator>=(const double& fl) const
{
  if(fl <= 0.0) return true; 
  return ( (*this) >= plProbability(fl) );
}


//=============================================================================
inline bool plProbability::operator>=(const float& fl) const
{
  if(fl <= 0.0f) return true; 
  return ( (*this) >= plProbability(fl) );
}


//=============================================================================
inline bool plProbability::operator>(const plProbability& p) const 
{ 
  if(p.f_ == 0) return (f_ != 0);
  if(f_ == 0) return false;
  if(e_ > p.e_) return true;
  if(e_ == p.e_) return (f_ > p.f_);
  return false;
}

//=============================================================================
inline bool plProbability::operator>(const long double& fl) const 
{
  if(fl < 0.0L) return true; 
  return ( (*this) > plProbability(fl) ); 
}

//=============================================================================
inline bool plProbability::operator>(const double& fl) const 
{
  if(fl < 0.0) return true; 
  return ( (*this) > plProbability(fl) ); 
}

//=============================================================================
inline bool plProbability::operator>(const float& fl) const 
{
  if(fl < 0.0f) return true; 
  return ( (*this) > plProbability(fl) ); 
}


/** The "+" operators */

//=============================================================================
inline  plProbability plProbability::operator+(const plProbability& pc) const
{ 
  if(isNaN()) return plNaNProbaConstant;
  if(pc.isNaN()) return plNaNProbaConstant;
  
  if(isInfinity()) return plInfinityProbaConstant;
  if(pc.isInfinity()) return plInfinityProbaConstant;
  
  PL_INT32 first_exponent_minus_second = e_ - pc.e_;
  if(first_exponent_minus_second >= PL_F_SIZE) return *this;
  if(first_exponent_minus_second <= -PL_F_SIZE) return pc;
   
  plProbability pa;
  
  if (first_exponent_minus_second < 0){
    pa.f_ = (f_ >> (-first_exponent_minus_second )) + pc.f_;
    pa.e_ = pc.e_;
  }
  else{
    pa.f_ = f_ + (pc.f_ >> first_exponent_minus_second);
    pa.e_ = e_;
  }
  
  if(PL_BIT_32(pa.f_)){
    pa.f_ >>= 1;
    pa.e_++;
    if( (pa.e_ > PL_MAX_31_BITS)) return plInfinityProbaConstant;
  }
  
  return pa;
}

//=============================================================================
inline  plProbability plProbability::operator+(const long double& fl) const
{
  if(fl < 0.0L) return (*this) - plProbability(-fl);
  return (*this) + plProbability(fl);
}


//=============================================================================
inline  plProbability plProbability::operator+(const double& fl) const
{
  if(fl < 0.0) return (*this) - plProbability(-fl);
  return (*this) + plProbability(fl);
}

//=============================================================================
inline  plProbability plProbability::operator+(const float& fl) const
{
  if(fl < 0.0f) return (*this) - plProbability(-fl);
  return (*this) + plProbability(fl);
}

//=============================================================================
inline  plProbability& plProbability::operator+=(const plProbability& pc)
{
  if(isNaN()) {*this = plNaNProbaConstant; return *this;}
  if(pc.isNaN()) {*this = plNaNProbaConstant; return *this;}
  
  if(isInfinity()) {*this = plInfinityProbaConstant; return *this;}
  if(pc.isInfinity()) {*this = plInfinityProbaConstant; return *this;}
  
  PL_INT32 first_exponent_minus_second = e_ - pc.e_;

  if(first_exponent_minus_second >= PL_F_SIZE) return *this;

  if(first_exponent_minus_second <= -PL_F_SIZE){
    f_ = pc.f_; 
    e_ = pc.e_;
    return *this;
  }
  
  if (first_exponent_minus_second < 0){ 
    f_ = (f_ >> (-first_exponent_minus_second )) + pc.f_;
    e_ = pc.e_;
  }
  else{
    f_ += (pc.f_ >> first_exponent_minus_second);
  }
  
  if(PL_BIT_32(f_)){
    f_ >>= 1;
    e_++;
    if( (e_ > PL_MAX_31_BITS)) *this = plInfinityProbaConstant;
  }
  return *this;
}

//=============================================================================
inline  plProbability& plProbability::operator+=(const long double& fl)
{
  if(fl < 0.0L) (*this) -= plProbability(-fl);
  return (*this) += (plProbability(fl)); 
}

//=============================================================================
inline  plProbability& plProbability::operator+=(const double& fl)
{
  if(fl < 0.0) (*this) -= plProbability(-fl);
  return (*this) += (plProbability(fl)); 
}

//=============================================================================
inline  plProbability& plProbability::operator+=(const float& fl)
{
  if(fl < 0.0f) (*this) -= plProbability(-fl);
  return (*this) += (plProbability(fl)); 
}

/** The "-" operators */
//=============================================================================
inline  plProbability plProbability::operator-(const plProbability& pc) const
{
  if(isNaN()) return plNaNProbaConstant;
  if(pc.isNaN()) return plNaNProbaConstant;
  
  if(isInfinity() && !pc.isInfinity()) return plInfinityProbaConstant;
  if(pc.isInfinity()) return plNaNProbaConstant;
  
  if(*this == pc) return plZeroProbaConstant;
  
  if( *this < pc){
    plError::warn(34);
    return plNaNProbaConstant;
  }

  
  PL_INT32 first_exponent_minus_second = e_ - pc.e_;
  
  if(first_exponent_minus_second >= PL_F_SIZE) return *this;
  
  plProbability pa;
  pa.f_ = f_ - (pc.f_ >> first_exponent_minus_second);
  pa.e_ = e_;
  
  while(!PL_BIT_31(pa.f_)){
    pa.f_ <<= 1;
    pa.e_--;
  }
  
  return pa;
}

//=============================================================================
inline  plProbability plProbability::operator-(const long double& fl) const 
{
  if(fl < 0.0L) (*this) + plProbability(-fl);
  return (*this) - plProbability(fl);
}

//=============================================================================
inline  plProbability plProbability::operator-(const double& fl) const 
{
  if(fl < 0.0) (*this) + plProbability(-fl);
  return (*this) - plProbability(fl);
}

//=============================================================================
inline  plProbability plProbability::operator-(const float& fl) const 
{
  if(fl < 0.0f) (*this) + plProbability(-fl);
  return (*this) - plProbability(fl);
}

//=============================================================================
inline  plProbability& plProbability::operator-=(const plProbability& pc)
{
  if(isNaN()) return *this;
  if(pc.isNaN()) {*this = pc; return *this;}
   
  if(isInfinity() && !pc.isInfinity() ) {*this = plInfinityProbaConstant; return *this;}
  if(pc.isInfinity()) {*this = plNaNProbaConstant; return *this;}
  
  if(*this == pc) {*this = plZeroProbaConstant; return *this;}
  
  if( *this < pc){
    plError::warn(34);
    *this = plNaNProbaConstant;
  }

  
  PL_INT32 first_exponent_minus_second = e_ - pc.e_;
  
  if(first_exponent_minus_second >= PL_F_SIZE) return *this;
  
  f_ -= (pc.f_ >> first_exponent_minus_second);
  
  while(!PL_BIT_31(f_)){
    f_ <<= 1;
    e_--;
  }
  
  return *this;
}

//=============================================================================
inline  plProbability& plProbability::operator-=(const long double& fl)
{
  if(fl < 0.0L) return (*this) += plProbability(-fl); 

  return (*this)-= (plProbability(fl)); 
}

//=============================================================================
inline  plProbability& plProbability::operator-=(const double& fl)
{
  if(fl < 0.0) return (*this) += plProbability(-fl); 

  return (*this)-= (plProbability(fl)); 
}

//=============================================================================
inline  plProbability& plProbability::operator-=(const float& fl)
{
  if(fl < 0.0f) return (*this) += plProbability(-fl);

  return (*this)-= (plProbability(fl)); 
}


/** The "*" operators */

//=============================================================================
inline  plProbability plProbability::operator*(const plProbability& pc) const
{
  if(isNaN()) return plNaNProbaConstant;
  if(pc.isNaN()) return plNaNProbaConstant;
  
  if(isInfinity()){
    if(pc.f_ == 0) return plNaNProbaConstant;
    else return plInfinityProbaConstant;
  }
  
  if(pc.isInfinity()){
    if(f_ == 0) return plNaNProbaConstant;
    else return plInfinityProbaConstant;
  }
  
  plProbability pa;
  //pa.f_ = (f_ >> 15) * (pc.f_ >> 15);
  pa.f_ = (PL_UINT32)(((PL_UINT64)f_ * (PL_UINT64)pc.f_) >> 30);

  pa.e_ = e_ + pc.e_ - 1 ;

  if(PL_BIT_32(pa.f_)){
    pa.f_ >>= 1;
    pa.e_++;
  }

  if(pa.e_ > PL_MAX_31_BITS) return plInfinityProbaConstant;
  
  if(pa.e_ < PL_MIN_31_BITS) return plZeroProbaConstant;
 
  
  return pa;
}

//=============================================================================
inline  plProbability plProbability::operator*(const long double& fl) const 
{
  return (*this) * plProbability(fl);
}

//=============================================================================
inline  plProbability plProbability::operator*(const double& fl) const 
{
  return (*this) * plProbability(fl);
}

//=============================================================================
inline  plProbability plProbability::operator*(const float& fl) const 
{
  return (*this) * plProbability(fl);
}


//=============================================================================
inline long double operator*(const long double& theFloat, const plProbability& p)
{
  return (theFloat * (long double)p);
}


//=============================================================================
inline double operator*(const double& theFloat, const plProbability& p)
{
  return (theFloat * double(p));
}

//=============================================================================
inline float operator*(const float& theFloat, const plProbability& p)
{
  return (theFloat * float(p));
}

//=============================================================================
inline  plProbability& plProbability::operator*=(const plProbability& pc)
{
  if(isNaN()) {*this = plNaNProbaConstant; return *this;}
  if(pc.isNaN()) {*this = plNaNProbaConstant; return *this;}
  
  if(isInfinity()){
    if(pc.f_ == 0) {*this = plNaNProbaConstant; return *this;}
    else {*this = plInfinityProbaConstant; return *this;}
  }
  
  if(pc.isInfinity()){
    if(f_ == 0) {*this = plNaNProbaConstant; return *this;}
    else {*this = plInfinityProbaConstant; return *this;}
  }
  
  
  //f_>>=15;
  //f_ *= (pc.f_>>15);
  f_ = (PL_UINT32)(((PL_UINT64)f_ * (PL_UINT64)pc.f_) >> 30);

  e_ += (pc.e_ - 1) ;
  
  if(PL_BIT_32(f_)){
    f_ >>= 1;
    e_++;
  }

  if(e_ > PL_MAX_31_BITS){
    *this = plInfinityProbaConstant;
  }
  else{
    if(e_ < PL_MIN_31_BITS){
      *this = plZeroProbaConstant;
    }
  }
  
  return *this;
}

//=============================================================================
inline  plProbability& plProbability::operator*=(const long double& fl)
{ 
  return (*this)*= plProbability(fl);
}


//=============================================================================
inline  plProbability& plProbability::operator*=(const double& fl)
{ 
  return (*this)*= plProbability(fl);
}

//=============================================================================
inline  plProbability& plProbability::operator*=(const float& fl)
{ 
  return (*this)*= plProbability(fl);
}


/** The "/" operator */

//=============================================================================
inline  plProbability plProbability::operator/(const plProbability& pc) const
{
  if(isNaN()) return plNaNProbaConstant;
  if(pc.isNaN()) return plNaNProbaConstant;
  
  if(isInfinity()) {
    if(pc.isInfinity()) return plNaNProbaConstant;
    else return plInfinityProbaConstant;
  }
  
  if(pc.isInfinity()) return plZeroProbaConstant;

  if(pc.f_ == 0){
    plError::warn(33);
    if(f_ == 0) return plNaNProbaConstant;
    return plInfinityProbaConstant;
  }
  if(f_ == 0) return plZeroProbaConstant;
  
  plProbability pa;
  pa.e_ = e_ - pc.e_ + 3 ;
  long double f_over_pcf = (long double)f_/(long double)pc.f_;
  PL_UINT32 int_f_over_pcf = (PL_UINT32) (PL_MASK_29 * f_over_pcf);
  pa.f_ = int_f_over_pcf;
  while(pa.f_ < PL_MASK_31){
    pa.f_ <<= 1;
    pa.e_--;
    if(pa.e_ < PL_MIN_31_BITS){
      return plZeroProbaConstant;
    }
  }
 
  return pa;
}

//=============================================================================
inline long double operator/(const long double& theFloat, const plProbability& p)
{
  return theFloat/(long double)p;
}

//=============================================================================
inline double operator/(const double& theFloat, const plProbability& p)
{
  return theFloat/double(p);
}

//=============================================================================
inline float operator/(const float& theFloat, const plProbability& p)
{
  return theFloat/float(p);
}

//=============================================================================
inline  plProbability plProbability::operator/(const long double& fl) const 
{ 
  return (*this)/plProbability(fl);
}

//=============================================================================
inline  plProbability plProbability::operator/(const double& fl) const 
{ 
  return (*this)/plProbability(fl);
}

//=============================================================================
inline  plProbability plProbability::operator/(const float& fl) const 
{ 
  return (*this)/plProbability(fl);
}

/** The "/=" operator */
//=============================================================================
inline  plProbability& plProbability::operator/=(const plProbability& pc)
{
  
  *this =  *this / pc;
  
  return *this;
}

//=============================================================================
inline  plProbability& plProbability::operator/=(const long double& fl)
{
  return (*this) /= plProbability(fl);
}

//=============================================================================
inline  plProbability& plProbability::operator/=(const double& fl)
{
  return (*this) /= plProbability(fl);
}

//=============================================================================
inline  plProbability& plProbability::operator/=(const float& fl)
{
  return (*this) /= plProbability(fl);
}

//=============================================================================
inline bool isNANstr(const std::string &s)
{
  return (s.size() == 3) && (toupper(s[0]) == 'N') && (toupper(s[1]) == 'A') && (toupper(s[2]) == 'N');
}

//=============================================================================
inline bool isINFstr(const std::string &s)
{
  return (s.size() == 3) && (toupper(s[0]) == 'I') && (toupper(s[1]) == 'N') && (toupper(s[2]) == 'F');
}

//=============================================================================
inline std::istream& operator>>(std::istream& in, plProbability& p)
{
  std::string st;
  in >> st;
	
  if(isNANstr(st)) { p = plProbability::plNaNProbaConstant; return in;}
  if(isINFstr(st)) { p = plProbability::plInfinityProbaConstant; return in;}
	
  PL_INT32 df = 0;
  PL_INT32 ide = 0;
  std::string dfStr, ideStr;
	
  unsigned int i = 0;
  unsigned int ndec = 0;
  bool afterdot = false;

  while( (st[i] != 'e') && (st[i] != 'E') && (i < st.size()) ) {
    if(afterdot) ndec++;
		
    if(st[i] == '.') afterdot = true;
    else dfStr += st[i];
		
    i++;
  }
  i++;
  while(i < st.size() ) ideStr += st[i++];

  std::sscanf(dfStr.c_str(), "%d", &df);
  std::sscanf(ideStr.c_str(), "%d", &ide);
	
  if(df == 0){ p = plProbability::plZeroProbaConstant; return in;}
  if(df < 0){ p = plProbability::plNaNProbaConstant; return in;}

  ide -= ndec;
	
  long double fideBin = ide*3.321928094887362660L;
  PL_INT32 ideBin = PL_INT32(fideBin);
  long double diff = fideBin - ideBin;
	
  long double fdf = df * std::pow(2.0L, diff);
	
  while(fdf < PL_MASK_31) {
    fdf *= 2.0L;
    ideBin--;
  }
	
  p.e_ = ideBin + PL_F_SIZE;
  p.f_ = (PL_UINT32)fdf;
	
  return in;
}

//=============================================================================
inline std::ostream& operator<<(std::ostream& out,  const plProbability& p)
{
  if(p.isNaN()) {out << "NaN"; return out;}
  if(p.isInfinity()) {out << "Inf"; return out;}
  
  if(p.f_ ==  0){
    out << "0";
    return out;
  }
  
  long double de = p.e_/3.321928094887362660L;
  PL_INT32 ide = PL_INT32(de);
  long double diff =  de - ide;
  long double df = p.f_/(0x80000000 * std::pow(10.0L, -diff));

  // do not use out directly (possible customised precision and format)
  std::ostringstream ss;
  ss << df;
  //out << df;
  if(ide) {
    //out << "e";
    //if( ide > 0) out << "+";
    //out << ide;
    
    ss << "e";
    if( ide > 0) ss << "+";
    ss << ide;
  }
  
  out << ss.str();
  
  return out;
}

//=============================================================================
inline double std::log2(const plProbability &pr)
{
  if(pr.f_ == 0){
    errno = ERANGE;
    return -std::numeric_limits<double>::infinity();
  }
  if(pr.isNaN()){
#if defined(_MSC_VER) // MS VC++
    const double zero = 0.0;
    return zero/zero;
#else     
    return NAN;
#endif
  }
  if(pr.isInfinity()) 
    return std::numeric_limits<double>::infinity();
    
  return PL_LOG_BASE2((double)pr.f_) + pr.e_ - PL_F_SIZE;
}

//=============================================================================
inline double std::log(const plProbability &pr)
{
  return (double)(std::log2(pr)/PL_LOG_OF_E_BASE2);
}
  
//=============================================================================
inline double std::log10(const plProbability &pr)
{
  return (double)(std::log2(pr)/PL_LOG_OF_10_BASE2);
}

//=============================================================================
inline bool std::isfinite(const plProbability &x) 
{
  return x != plProbability::plInfinityProbaConstant && x != plProbability::plNaNProbaConstant;
}

//=============================================================================
inline bool std::isnan(const plProbability &x)
{
  return x == plProbability::plNaNProbaConstant;
}

// //=============================================================================
// inline plProbability std::pow( const plProbability &pr, PL_INT32 exp )
// {
//   if( pr.isNaN() )
//     {
//       return pr;
//     }
//   else
//     {
//       if( 0 == exp )
// 	{
// 	  return plProbability::plOneProbaConstant;
// 	}
//       if( 1 == exp )
// 	{
// 	  return pr;
// 	}

//       plProbability pr_aux;
//       if( exp < 0 )
// 	{
// 	  pr_aux = plProbability(1)/pr;
// 	  exp = -exp;
// 	}
//       else
// 	pr_aux = pr;

//       /** Mantiss calculus - new mantiss : interm.f */
//       double mantisse = std::pow( (double)pr_aux.f_, (double)exp );
//       plProbability interm( mantisse );
//       if( interm.isNaN() 
// 	  || interm.isInfinity() )
// 	{
// 	  return plProbability::plNaNProbaConstant;
// 	}

//       /** Exponent calculus */
//       PL_INT32 exposant = interm.e_ + (pr_aux.e_ - PL_F_SIZE)*exp;
//       /** Tests of calcul validity on PL_INT32 */
//       if( !(PL_INT32_MIN + PL_F_SIZE <= pr_aux.e_ ) ///< assert that  pr_aux.e - PL_F_SIZE >= PL_INT32_MIN
// 	  || !( (pr_aux.e_ - PL_F_SIZE)<= PL_INT32_MAX/exp  )///< assert that  ( pr_aux.e - PL_F_SIZE )*exp <= PL_INT32_MAX
// 	  || !( PL_INT32_MIN/exp <= (pr_aux.e_ - PL_F_SIZE) )///< assert that  PL_INT32_MIN <= ( pr_aux.e - PL_F_SIZE )*exp
// 	  )
// 	return plProbability::plNaNProbaConstant;

//       if( interm.e_ > 0 )
// 	{
// 	  /** assert that interm.e + ( pr_aux.e - PL_F_SIZE )*exp <= PL_INT32_MAX */
//       	  if( !( ( pr_aux.e_ - PL_F_SIZE )*exp <= PL_INT32_MAX - interm.e_ ) )
// 	    {
// 	      return plProbability::plNaNProbaConstant;
// 	    }
// 	}
//       else
// 	if( interm.e_ < 0 )
// 	  {
// 	    /** assert that PL_INT32_MIN <= interm.e + ( pr_aux.e - PL_F_SIZE )*exp */
// 	    if( !(PL_INT32_MIN - interm.e_ <= ( pr_aux.e_ - PL_F_SIZE )*exp ) )
// 	      {
// 		return plProbability::plNaNProbaConstant;
// 	      }
// 	  }

//       return plProbability( interm.f_, exposant );
//     }
// }

#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
