/*=============================================================================
 * Product        : ProBT
 * File           : plData.h
 * Author         : Juan-Manuel Ahuactzin
 * Creation       : Fri Mar 16 12:56:22 2001
 *
 *=============================================================================
 *     (c) Copyright 2000, Centre National de la Recherche Scientifique,
 *                              all rights reserved
 *=============================================================================
 */

#ifndef plData_h
#define plData_h

#include "plConfig.h"

#include <plMath.h>
#include <plError.h>
#include <plVariableType.h>


#ifdef PL_USE_PLPROBA
#include <plProbability32.h>
#endif


class kplLabelType;
class kplVariable;

SPL_DECL2(plData);

#define PL_CHECK_DATA_ARITHMETIC_OPERATIONS


//=============================================================================
// A plData  holds a single data 
class PL_DLL_API plData 
{

public:

//=============================================================================
/* Type of information stored in a plData */
  enum plData_type {kplNIL,           // no type defined
                    kplINTEGER,       // integer type
                    kplFLOAT
  };

/** Type of couples of plData, used to define operators like *,+,- etc */
  enum plCoupleDataType {
    INT_INT =      0,  // 00 integer and integer
    INT_FLOAT =    1,  // 01 integer and float
    FLOAT_INT =    2,  // 10 float and integer
    FLOAT_FLOAT =  3   // 11 float and float
  };


public:
  
  /** Creates a void plData */
  inline plData();
  /** Creates a plData with a given type */
  inline explicit plData(plData::plData_type type);

  /** Copy constructor */
  inline plData(const plData&other);

  /** Destructor */
  ~plData();

  /** Sets the content to \em other. This could change the internal type
      of this data.
  */
  inline void set(const plData &other);

  /** Assignment operator from an plData. This does not change its internal type 
      unless it's NIL (void default-constructed value). 
  */
  inline plData& operator=(const plData &data);
  /** Assignment operator from a number. This does not change the internal type,
      unless it's NIL (void default-constructed value).
  */
  template <typename T>
  inline plData& operator=(const T& number);
  /** Assignment operator from a string. 
      Assumes that the corresponding plSymbol is of type plLabelType.
      Throws an exception otherwise.
  */
  plData& operator=(const std::string &label);

  /** Assignment operator from a string. 
      Assumes that the corresponding plSymbol is of type plLabelType.
      Throws an exception otherwise.
  */
  plData& operator=(const char *label);

  
  /** "Equal to"  operator with another plData */
  inline bool operator==(const plData &data) const;
  
  /** "Equal to" another plData */
  inline bool equal(const plData &data) const
  {
    return operator==(data);
  }


  /** "Equal to"  operator with a number */
  template <typename T>
  inline bool operator==(const T&number) const;
 
  /** "Different from"  operator */
  template <typename T> 
    bool operator!=(const T&other)const;

  /** "Different from" another plData */
  inline bool not_equal(const plData &data) const
  {
    return operator!=(data);
  }

  /** "Greater than" operator with another plData*/ 
  inline bool operator>(const plData &data) const;

  /** "Greater than" another plData*/ 
  inline bool greater(const plData &data) const
  {
    return operator>(data);
  }

  /** "Greater than" operator with a number*/ 
  template <typename T>
  inline bool operator>(const T&number) const;
 
  /** "Less than or equal" operator */
  template <typename T>
  inline bool operator<=(const T &data) const;

  /** "Less than or equal"  */
  bool less_or_equal(const plData &data) const
  {
    return operator<=(data);
  }

  /** "Less than" operator with another plData*/ 
  inline bool operator<(const plData &data) const;

  /** "Less than" another plData */ 
  inline bool less(const plData &data) const
  {
    return operator<(data);
  }

  /** "Less than" operator with a number */ 
  template <typename T>
  inline bool operator<(const T& number) const;

  /** "Greater than or equal" operator */
  template<typename T>
  inline bool operator>=(const T &other) const;

  /** "Greater than or equal" */
  inline bool greater_or_equal(const plData &other) const
  {
    return operator>=(other);
  }
 

  /** Multiplication operator with another plData results in a plData */
  inline plData operator*(const plData &data) const;
  /** Multiplication operator with a number results in a plData */
  template<typename T>
  inline plData operator*(const T& number) const;
 
  /** Division operator with another plData results in a plData */
  inline plData operator/(const plData &data) const;
  /** Division operator with a number results in a plData */
  template<typename T>
  inline plData operator/(const T& number) const;

  /** Addition operator with another plData results in a plData */
  inline plData operator+(const plData &data) const;
  /** Addition operator with a number results in a plData */
  template <typename T>
  inline plData operator+(const T& number) const;


  /** Subtraction operator with another plData results in a plData */
  inline plData operator-(const plData &data) const;
  /** Subtraction operator with a number results in a plData */
  template <typename T> 
  inline plData operator-(const T& number) const;
 
  /** Operator % for plData */
  template<typename T>
  inline long int operator%(const T& int_number)const; 
  
  /** Conversion to float operator */
  inline operator float() const;

  /** Conversion to double operator */
  inline operator double() const;

  /** Conversion to double operator */
  inline operator long double() const;

  /** Conversion to bool  operator */
  inline operator bool() const;

  /** Conversion to int  operator */
  inline operator int() const;

  /** Conversion to unsigned int */
  inline operator unsigned int() const;

  /** Conversion to long int */
  inline operator long int() const;

  /** Conversion to long unsigned int */
  inline operator long unsigned int() const;

  /** Read the data from an input stream */
  void Input(std::istream& in);

  /** Read from an input stream */
  friend inline std::istream& operator>>(std::istream& in, plData &data) 
  {
    data.Input(in);
    return in;
  }

  /** Write the data to an output stream */
  void print(std::ostream &out) const;

  /** Write the data to an output stream */
  friend inline std::ostream &operator<<(std::ostream &out, const plData &data)
  {
    data.print(out);
    return out;
  }

  bool is_label() const {
    return (label_type_ != 0);
  }

  /** Needed by the Python bindings, but not part of the user API.  Please do
   not use. */
  inline plData_type get_data_type() const { return data_type_; }

  /** Get the value as int */
  inline int as_int() const
  {
    return int(*this);
  }

  /** Get the value as double */
  inline double as_double() const
  {
    return double(*this);
  }

  /** Get the value as a label string 
      Throw an exception if the data is not of plLabelType
  */
  std::string as_label() const;

protected:
  SPL_ACCESS2(plData);

  void  maybe_set_label_type(const kplVariable* var);


  /* This anonymous union permits to use \em float_value or \em
      int_value_ according to the value of "data_type_" */
  union {
    /* Stores an integer value */
    long int int_value_;
    
    /* Stores a float value */
    plFloat float_value_;
  };
  
  /* The type of the stored data */
  plData_type data_type_;

  /* Pointer the  kplLabelType if data corresponds to a plLabelType.
     0 otherwise.
  */
  kplLabelType* label_type_;


 private:
  inline plCoupleDataType couple_type(const plData& data) const;
  void set_label_type(kplLabelType* ltype);

  template<typename T>
    inline void mul_int(plData &result, const T& val)const;

  
  template<typename T>
    inline void mul_float(plData &result, const T& val)const;

 template<typename T>
    inline void div_int(plData &result, const T& val)const;

  
  template<typename T>
    inline void div_float(plData &result, const T& val)const;

  template<typename T>
    inline void add_int(plData &result, const T& val)const;

  
  template<typename T>
    inline void add_float(plData &result, const T& val)const;

  template<typename T>
    inline void sub_int(plData &result, const T& val)const;

  
  template<typename T>
    inline void sub_float(plData &result, const T& val)const;


  template <typename T>
  inline long int mod(const T&int_val)const;

  template<typename T>
  inline void to_int_number(T& result)const;

  template<typename T>
  inline void to_float_number(T& result)const;

  template <typename T>
    inline plData &set_from_int(const T& int_number);

  template <typename T>
    inline plData &set_from_float(const T& float_number);

  template <typename T>
    friend inline void int_div(plData &result, const T&int_number, const plData &data);

  template <typename T>
    friend inline void float_div(plData &result, const T&int_number, const plData &data);

  template <typename T>
    friend inline void int_sub(plData &result, const T&int_number, const plData &data);

  template <typename T>
    friend inline void float_sub(plData &result, const T&int_number, const plData &data);

 
 

  friend struct kplDataAccess;
  friend class plDataValues;
};


//=============================================================================
inline plData::plCoupleDataType plData::couple_type(const plData& data) const
{
  plCoupleDataType type1, type2, result;

  // first data is integer
  if (data_type_ == plData::kplINTEGER)
    type1 = plData::INT_INT;
  // first data is float
  else //if (data_type_ == plData::kplFLOAT)
    type1 = plData::FLOAT_INT;
 
  // second data is integer
  if (data.data_type_ == plData::kplINTEGER)
    type2 = plData::INT_INT;
  // second data is float
  else //if (data.data_type_ == plData::kplFLOAT)
    type2 = plData::INT_FLOAT;
    
  // return the data type of the couple by combining the two types
  // (note that it is an or operator)
  result = plCoupleDataType (type1 | type2);
    
  return result;
}

//=============================================================================
#ifdef PL_WARN_EFFCPP
#pragma GCC diagnostic ignored "-Weffc++"
#endif //PL_WARN_EFFCPP


//=============================================================================
inline plData::plData()
  : float_value_(0), data_type_(plData::kplNIL), label_type_(0)
{
}

//=============================================================================
inline plData::plData(plData::plData_type type)
  : float_value_(0), data_type_(type), label_type_(0)
{
}

//=============================================================================
inline plData::plData(const plData&other)
  : float_value_(0), data_type_(other.data_type_), label_type_(0)
{
  set_label_type(other.label_type_);
  
  switch(data_type_){
  case plData::kplINTEGER:
    int_value_ = other.int_value_;
    break;
    
  case plData::kplFLOAT:
    float_value_ = other.float_value_;
    break;
  default:
    break;
  }
  
}

#ifdef PL_WARN_EFFCPP
#pragma GCC diagnostic warning "-Weffc++"
#endif //PL_WARN_EFFCPP
//=============================================================================
inline void plData::set(const plData &data)
{
  /* In this case we force the type be equal to that of the sent data */
  data_type_  = data.data_type_;

  set_label_type(data.label_type_);

  switch(couple_type(data)){
  case plData::INT_INT:
    int_value_ = data.int_value_;
    break;
  case plData::FLOAT_FLOAT:
    float_value_ = data.float_value_;
    break;
  default:
    throw plError(2," plData set method ");
  }
}

//=============================================================================
inline plData& plData::operator=(const plData &data)
{
  //This is the case when we have a new plData and we copy on it the
  //content of another plData
  if(data_type_ == plData::kplNIL){
    data_type_  = data.data_type_;
    set_label_type(data.label_type_);
  }

  switch(couple_type(data)){
  case plData::INT_INT:
    int_value_ = data.int_value_;
    break;
  case plData::INT_FLOAT:
    {
#ifdef PL_CHECK_DATA_ARITHMETIC_OPERATIONS
      if((long int)data.float_value_ != data.float_value_){
        std::stringstream ss;
        ss << "plData::operator=(const &plData), this = " << *this << " other = " << data; 
        plError::warn(107, ss.str());
      }
#endif
      int_value_ = (long int)data.float_value_;
      break;
    }
  case plData::FLOAT_INT:
    float_value_ = data.int_value_;
    break;
  case plData::FLOAT_FLOAT:
    float_value_ = data.float_value_;
    break;
  default:
    throw plError(2," plData operator = plData ");
  }
    

  return *this;
}

//=============================================================================
template <typename T>
inline plData &plData::set_from_int(const T& int_number)
{
  switch (data_type_){
  case plData::kplINTEGER :
    int_value_ = (long int)int_number;
    break;
  case plData::kplFLOAT:
    float_value_ = plFloat(int_number);
    break;
  case plData::kplNIL:
    data_type_  = plData::kplINTEGER;
    int_value_ = (long int)int_number;
    break;
  default:
    throw plError(2,"plData::operator=int_number");
  }

  return *this;
}

//=============================================================================
template <>
inline plData &plData::operator=(const bool& b)
{
  return set_from_int(b);
}

//=============================================================================
template <>
inline plData &plData::operator=(const int& int_number)
{
  return set_from_int(int_number);
}

//=============================================================================
template <>
inline plData &plData::operator=(const unsigned int& int_number)
{
  return set_from_int(int_number);
}

//=============================================================================
template <>
inline plData &plData::operator=(const long int& int_number)
{
  return set_from_int(int_number);
}

//=============================================================================
template <>
inline plData &plData::operator=(const unsigned long int& int_number)
{
  return set_from_int(int_number);
}


//=============================================================================
template <typename T>
inline plData &plData::set_from_float(const T& float_number)
{
  switch (data_type_){
  case plData::kplINTEGER :
    {
#ifdef PL_CHECK_DATA_ARITHMETIC_OPERATIONS
      if((long int)float_number != float_number){
        std::stringstream ss;
        ss << "plData::operator=(const T&  float_number), this = " << *this << " float_number = " << float_number; 
        plError::warn(107, ss.str());
      }
#endif
      int_value_ = (long int)float_number;
      break;
    }
  case plData::kplFLOAT:
    float_value_ = (plFloat) float_number;
    break;
  case plData::kplNIL:
    data_type_  = plData::kplFLOAT;
    float_value_ = (plFloat) float_number;
    break;
  default:
    throw plError(2,"plData::operator=float_number");
  }

  return *this;
}


//=============================================================================
template <>
inline plData &plData::operator=(const float& float_number)
{
  return set_from_float(float_number);
}

//=============================================================================
template <>
inline plData &plData::operator=(const double& float_number)
{
  return set_from_float(float_number);
}

//=============================================================================
template <>
inline plData &plData::operator=(const long double& float_number)
{
  return set_from_float(float_number);
}

#ifdef PL_USE_PLPROBA
//=============================================================================
template <>
inline plData &plData::operator=(const plProbability& prob)
{
  return set_from_float(plFloat(prob));
}
#endif

//=============================================================================
inline bool plData::operator==(const plData &data) const 
{
  switch(couple_type(data)){
  case plData::INT_INT:
    return int_value_ == data.int_value_;
 
  case plData::INT_FLOAT:
    return int_value_ == data.float_value_;

  case plData::FLOAT_INT:
    return float_value_ == data.int_value_;
  
  case plData::FLOAT_FLOAT:
    return float_value_ == data.float_value_;

  default:
    throw plError(2," plData operator == plData");
    return false;
  }

}

//=============================================================================
template <typename T>
inline bool plData::operator==(const T&number)const
{
  switch (data_type_){
  case plData::kplINTEGER :
    return int_value_ == number;

  case plData::kplFLOAT:
    return float_value_ == number;
    
  default:
    throw plError(2,"plData operator == number");
    return false;
  }
}


//=============================================================================
template <typename T>
inline bool plData::operator!=(const T&other)const
{
  return !operator==(other);
}

//=============================================================================
inline bool plData::operator>(const plData &data) const 
{
  switch(couple_type(data)){
  case plData::INT_INT:
    return int_value_ > data.int_value_;
 
  case plData::INT_FLOAT:
    return int_value_ > data.float_value_;
  
  case plData::FLOAT_INT:
    return float_value_ > data.int_value_;
 
  case plData::FLOAT_FLOAT:
    return float_value_ > data.float_value_;

  default:
    throw plError(2,"plData operator > plData");
    return false;
  }

}


//=============================================================================
template <typename T>
inline bool plData::operator>(const T&number)const
{
  switch (data_type_){
  case plData::kplINTEGER :
    return int_value_ > number;

  case plData::kplFLOAT:
    return float_value_ > number;
    
  default:
    throw plError(2,"plData operator > number");
    return false;
  }
}


//=============================================================================
template <typename T>
inline bool plData::operator<=(const T &data) const 
{
  return !operator>(data);
}

//=============================================================================
inline bool plData::operator<(const plData &data) const 
{
  switch(couple_type(data)){
  case plData::INT_INT:
    return int_value_ < data.int_value_;
 
  case plData::INT_FLOAT:
    return int_value_ < data.float_value_;
  
  case plData::FLOAT_INT:
    return float_value_ < data.int_value_;

  case plData::FLOAT_FLOAT:
    return float_value_ < data.float_value_;

  default:
    throw plError(2,"plData < plData ");
    return false;
  }

}

//=============================================================================
template <typename T>
inline bool plData::operator<(const T&number)const
{
  switch (data_type_){
  case plData::kplINTEGER :
    return int_value_ < number;
    
  case plData::kplFLOAT:
    return float_value_ < number;
    
  default:
    throw plError(2,"plData operator > number");
    return false;
  }
}

//=============================================================================
template <typename T>
inline bool plData::operator>=(const T &data) const 
{
  return !operator<(data);
}


//=============================================================================
inline plData plData::operator*(const plData &data) const 
{
  plData res;
  switch (couple_type(data)){
  case plData::INT_INT:
    res.data_type_ = plData::kplINTEGER;
    res.int_value_ = int_value_*data.int_value_;
    break;
  case plData::INT_FLOAT:
    res.data_type_ = plData::kplFLOAT;
    res.float_value_ = int_value_*data.float_value_; 
    break;
  case plData::FLOAT_INT:
    res.data_type_ = plData::kplFLOAT;
    res.float_value_ = float_value_*data.int_value_; 
    break;
  case plData::FLOAT_FLOAT:
    res.data_type_ = plData::kplFLOAT;
    res.float_value_ = float_value_*data.float_value_; 
    break;
  default:
    throw plError(2,"plData * plData");
  }
  return res;
}

//=============================================================================
template <typename T>
inline void plData::mul_int(plData &res, const T& val)const
{
  switch (data_type_){
  case plData::kplINTEGER:
    res.data_type_ = plData::kplINTEGER;
    res.int_value_ = int_value_*(long int)val;
    break;
  case plData::kplFLOAT:
    res.data_type_ = plData::kplFLOAT;
    res.float_value_ = float_value_*(plFloat)val; 
    break;
  default:
    throw plError(2,"plData * int");
  }
 
}

//=============================================================================
template<>
inline plData plData::operator*(const bool &b) const 
{
  plData res;
  mul_int(res, b);
  return res;
}

//=============================================================================
template<>
inline plData plData::operator*(const int &data) const 
{
  plData res;
  mul_int(res, data);
  return res;
}

//=============================================================================
template<>
inline plData plData::operator*(const unsigned int &data) const 
{
  plData res;
  mul_int(res, data);
  return res; 
}

//=============================================================================
template<>
inline plData plData::operator*(const long int &data) const 
{
  plData res;
  mul_int(res, data);
  return res;
}

//=============================================================================
template<>
inline plData plData::operator*(const unsigned long int &data) const 
{ 
  plData res;
  mul_int(res, data);
  return res;
}

//=============================================================================
template <typename T>
inline  void plData::mul_float(plData &res, const T& val)const
{
  res.data_type_ = plData::kplFLOAT;

  switch (data_type_){
  case plData::kplINTEGER:
    res.float_value_ = (plFloat)int_value_ * (plFloat) val;
    break;
  case plData::kplFLOAT:
    res.float_value_ = float_value_ * (plFloat) val;
    break;
  default:
    throw plError(2,"plData * float");
  }

}

//=============================================================================
template<>
inline plData plData::operator*(const float &data) const 
{ 
  plData res;
  mul_float(res, data);
  return res;
}

//=============================================================================
template<>
inline plData plData::operator*(const double &data) const 
{  
  plData res;
  mul_float(res, data);
  return res;
}

//=============================================================================
template<>
inline plData plData::operator*(const long double &data) const 
{  
  plData res;
  mul_float(res, data);
  return res;
}

//=============================================================================
inline plData plData::operator/(const plData &data) const 
{
#ifdef PL_CHECK_DATA_ARITHMETIC_OPERATIONS
  if( (data.data_type_ == plData::kplINTEGER && data.int_value_ == 0)
      ||
      (data.data_type_ == plData::kplFLOAT && data.float_value_ == PL_ZERO)){
    std::stringstream ss;
    ss << "Data = " << *this;
    plError::warn(108, ss.str());
  }
#endif

  plData res;
  switch (couple_type(data)){
  case plData::INT_INT:
    res.data_type_ = plData::kplINTEGER;
    res.int_value_ = int_value_/data.int_value_;
    break;
  case plData::INT_FLOAT:
    res.data_type_ = plData::kplFLOAT;
    res.float_value_ = int_value_/data.float_value_; 
    break;
  case plData::FLOAT_INT:
    res.data_type_ = plData::kplFLOAT;
    res.float_value_ = float_value_/data.int_value_; 
    break;
  case plData::FLOAT_FLOAT:
    res.data_type_ = plData::kplFLOAT;
    res.float_value_ = float_value_/data.float_value_; 
    break;
  default:
    throw plError(2,"plData / plData");
  }
  return res;
}


//=============================================================================
template <typename T>
inline void plData::div_int(plData &res, const T& int_val)const
{
#ifdef PL_CHECK_DATA_ARITHMETIC_OPERATIONS
  if(int_val == 0){
    std::stringstream ss;
    ss << "Data = " << *this;
    plError::warn(108, ss.str());
  }
#endif

  switch (data_type_){
  case plData::kplINTEGER:
      res.data_type_ = plData::kplINTEGER;
      res.int_value_ = int_value_/(long int)int_val;
    break;
  case plData::kplFLOAT:
    res.data_type_ = plData::kplFLOAT;
    res.float_value_ = float_value_/plFloat(int_val); 
    break;
  default:
    throw plError(2,"plData / int");
  }
}


//=============================================================================
template<>
inline plData plData::operator/(const int &data) const 
{ 
  plData res;
  div_int(res, data);
  return res;
}

//=============================================================================
template<>
inline plData plData::operator/(const unsigned int &data) const 
{ 
  plData res;
  div_int(res, data);
  return res;
}

//=============================================================================
template<>
inline plData plData::operator/(const long int &data) const 
{ 
  plData res;
  div_int(res, data);
  return res;
}

//=============================================================================
template<>
inline plData plData::operator/(const unsigned long int &data) const 
{ 
  plData res;
  div_int(res, data);
  return res;
}

//=============================================================================
template <typename T>
inline  void plData::div_float(plData &res, const T& float_val)const
{
#ifdef PL_CHECK_DATA_ARITHMETIC_OPERATIONS
  if(float_val == T(0)){
    std::stringstream ss;
    ss << "Data = " << *this;
    plError::warn(108, ss.str());
  }
#endif

  res.data_type_ = plData::kplFLOAT;

  switch (data_type_){
  case plData::kplINTEGER:
    res.float_value_ =  (plFloat)int_value_ / (plFloat) float_val;
    break;
  case plData::kplFLOAT:
    res.float_value_ = float_value_ / (plFloat) float_val; 
    break;
  default:
    throw plError(2,"plData / float");
  }

}

//=============================================================================
template <>
inline plData plData::operator/(const float &data) const 
{ 
  plData res;
  div_float(res, data);
  return res;
}

//=============================================================================
template <>
inline plData plData::operator/(const double &data) const 
{
  plData res;
  div_float(res, data);
  return res;
}

//=============================================================================
template <>
inline plData plData::operator/(const long double &data) const 
{
  plData res;
  div_float(res, data);
  return res;
}


//=============================================================================
inline plData plData::operator+(const plData &data) const 
{ 
  plData res;
  switch (couple_type(data)){
  case plData::INT_INT:
    res.data_type_ = plData::kplINTEGER;
    res.int_value_ = int_value_+data.int_value_;
    break;
  case plData::INT_FLOAT:
    res.data_type_ = plData::kplFLOAT;
    res.float_value_ = int_value_+data.float_value_; 
    break;
  case plData::FLOAT_INT:
    res.data_type_ = plData::kplFLOAT;
    res.float_value_ = float_value_+data.int_value_; 
    break;
  case plData::FLOAT_FLOAT:
    res.data_type_ = plData::kplFLOAT;
    res.float_value_ = float_value_+data.float_value_; 
    break;
  default:
    throw plError(2,"plData + plData");
  }
  return res;
}

//=============================================================================
template <typename T>
inline void plData::add_int(plData &res, const T& val)const
{
  switch (data_type_){
  case plData::kplINTEGER:
    res.data_type_ = plData::kplINTEGER;
    res.int_value_ = int_value_+(long int)val;
    break;
  case plData::kplFLOAT:
    res.data_type_ = plData::kplFLOAT;
    res.float_value_ = float_value_+(plFloat)val; 
    break;
  default:
    throw plError(2,"plData + int");
  }
}

//=============================================================================
template<>
inline plData plData::operator+(const bool &data) const 
{
  plData res;
  add_int(res, data);
  return res;
}

//=============================================================================
template<>
inline plData plData::operator+(const int &data) const 
{
  plData res;
  add_int(res, data);
  return res;
}

//=============================================================================
template<>
inline plData plData::operator+(const unsigned int &data) const 
{
  plData res;
  add_int(res, data);
  return res;
}
//=============================================================================
template<>
inline plData plData::operator+(const long int &data) const 
{ 
  plData res;
  add_int(res, data);
  return res;
}

//=============================================================================
template<>
inline plData plData::operator+(const unsigned long int &data) const 
{
  plData res;
  add_int(res, data);
  return res;
}

//=============================================================================
template <typename T>
inline  void plData::add_float(plData &res, const T& val)const
{
  res.data_type_ = plData::kplFLOAT;

  switch (data_type_){
  case plData::kplINTEGER:
    res.float_value_ = (plFloat) int_value_ + (plFloat) val;
    break;
  case plData::kplFLOAT:
    res.float_value_ = float_value_ + (plFloat) val;
    break;
  default:
    throw plError(2,"plData + float");
  }

}

//=============================================================================
template<>
inline plData plData::operator+(const float &data) const 
{ 
  plData res;
  add_float(res, data);
  return res;
}

//=============================================================================
template<>
inline plData plData::operator+(const double &data) const 
{ 
  plData res;
  add_float(res, data);
  return res;
}

//=============================================================================
template<>
inline plData plData::operator+(const long double &data) const 
{
  plData res;
  add_float(res, data);
  return res;
}

//=============================================================================
inline plData plData::operator-(const plData &data) const
{
  plData res;
  switch (couple_type(data)){
  case plData::INT_INT:
    res.data_type_ = plData::kplINTEGER;
    res.int_value_ = int_value_-data.int_value_;
    break;
  case plData::INT_FLOAT:
    res.data_type_ = plData::kplFLOAT;
    res.float_value_ = int_value_-data.float_value_; 
    break;
  case plData::FLOAT_INT:
    res.data_type_ = plData::kplFLOAT;
    res.float_value_ = float_value_-data.int_value_; 
    break;
  case plData::FLOAT_FLOAT:
    res.data_type_ = plData::kplFLOAT;
    res.float_value_ = float_value_-data.float_value_; 
    break;
  default:
    throw plError(2,"plData - plData");
  }
  return res;
}

//=============================================================================
template<typename T>
inline plData plData::operator-(const T&number) const 
{
  return operator+(-number);
}


//=============================================================================
template <typename T>
inline long int plData::mod(const T&int_v)const
{
#ifdef PL_CHECK_DATA_ARITHMETIC_OPERATIONS
  if(int_v == T(0)){
    std::stringstream ss;
    ss << "Data = " << *this;
    plError::warn(108, ss.str());
  }
#endif

  long int result;
  switch (data_type_){
  case plData::kplINTEGER:
    result = int_value_ % (long int)int_v;
    break;
  case plData::kplFLOAT:
    result = (long int) float_value_ % (long int)int_v;
    break;
  default:
    throw plError(2,"operator plData % int");
  }
  return result;
}

//=============================================================================
template<>
inline long int plData::operator%(const int &number)const
{
  return mod(number);
}

//=============================================================================
template<>
inline long int plData::operator%(const unsigned int &number)const
{
  return mod(number);
}

//=============================================================================
template<>
inline long int plData::operator%(const long int &number)const
{
  return mod(number);
}

//=============================================================================
template<>
inline long int plData::operator%(const unsigned long int &number)const
{
  return mod(number);
}


//=============================================================================
template<typename T>
inline void plData::to_int_number(T& int_result)const
{  
  switch (data_type_){
  case plData::kplINTEGER:
    int_result = T(int_value_);
    break;
  case plData::kplFLOAT:
    {
#ifdef PL_CHECK_DATA_ARITHMETIC_OPERATIONS
      if( T(float_value_) != float_value_){
        std::stringstream ss;
        ss << "int(const &plData), this = " << *this; 
        plError::warn(107, ss.str());
      }
#endif
      int_result = T(float_value_);
    break;
    }
  default:
    throw plError(2,"Conversion to number");
  }
}

//=============================================================================
inline plData::operator bool() const
{
#ifdef PL_CHECK_DATA_ARITHMETIC_OPERATIONS
  if(*this != 0 && *this != 1){
    std::stringstream ss;
    ss << "bool(const &plData), this = " << *this; 
    plError::warn(109, ss.str());
  }
#endif

  bool res;
  
  switch (data_type_){
  case plData::kplINTEGER:
    res = (int_value_ != 0);
    break;
  case plData::kplFLOAT:
    res = (float_value_ != PL_ZERO);
    break;
  default:
    throw plError(2,"Conversion to bool");
  }
  return res;
}

//=============================================================================
inline plData::operator int() const
{
  int res;
  to_int_number(res);
  return res;
}

//=============================================================================
inline plData::operator unsigned int() const
{
  unsigned int res;
  to_int_number(res);
  return res;
}

//=============================================================================
inline plData::operator long int() const
{
  long int res;
  to_int_number(res);
  return res;
}

//=============================================================================
inline plData::operator long unsigned int() const
{
  long unsigned int res;
  to_int_number(res);
  return res;
}

//=============================================================================
template<typename T>
inline void plData::to_float_number(T& result)const
{  
  switch (data_type_){
  case plData::kplINTEGER:
    result = T(int_value_);
    break;
  case plData::kplFLOAT:
    result = T(float_value_);
    break;
  default:
    throw plError(2,"Conversion to number");
  }
}

//=============================================================================
inline plData::operator float() const 
{
  float res;
  to_float_number(res);
  return res;
}

//=============================================================================
inline plData::operator double() const 
{
  double res;
  to_float_number(res);
  return res;
}

//=============================================================================
inline plData::operator long double() const 
{
  long double res;
  to_float_number(res);
  return res;
}


//=============================================================================
/** Multiplication of a number with a plData results in a plData*/
template <typename T>
inline plData operator*(const T&number, const plData &data)
{
  return data*number;
}

//=============================================================================
/** Division of a number by a plData results in a plData*/
template <typename T>
inline plData operator/(const T&number, const plData &data);

//=============================================================================
template <typename T>
inline void int_div(plData &result, const T&int_number, const plData &data)
{
  switch (data.data_type_){
  case plData::kplINTEGER:
    {
#ifdef PL_CHECK_DATA_ARITHMETIC_OPERATIONS
      if(data.int_value_ == 0){
        std::stringstream ss;
        ss << "number = " << int_number;
        plError::warn(108, ss.str());
      }
#endif

    result.data_type_ = plData::kplINTEGER;
    result.int_value_ = (long int)int_number/data.int_value_;
    break;
    }
  case plData::kplFLOAT:
    {
#ifdef PL_CHECK_DATA_ARITHMETIC_OPERATIONS
      if(data.float_value_ == PL_ZERO){
        std::stringstream ss;
        ss << "number = " << int_number;
        plError::warn(108, ss.str());
      }
#endif
    result.data_type_ = plData::kplFLOAT;
    result.float_value_ = plFloat(int_number)/data.float_value_; 
    break;
    }
  default:
    throw plError(2,"int / plData");
  }
}

//=============================================================================
template <>
inline plData operator/(const int&number, const plData &data)
{
  plData res;
  int_div(res, number, data);
  return res;
}

//=============================================================================
template <>
inline plData operator/(const unsigned int&number, const plData &data)
{
  plData res;
  int_div(res, number, data);
  return res;
}

//=============================================================================
template <>
inline plData operator/(const long int&number, const plData &data)
{
  plData res;
  int_div(res, number, data);
  return res;
}

//=============================================================================
template <>
inline plData operator/(const unsigned long int&number, const plData &data)
{
  plData res;
  int_div(res, number, data);
  return res;
}

//=============================================================================
template <typename T>
inline void float_div(plData &result, const T&float_number, const plData &data)
{
  result.data_type_ = plData::kplFLOAT;
  switch (data.data_type_){
  case plData::kplINTEGER:
    {
#ifdef PL_CHECK_DATA_ARITHMETIC_OPERATIONS
      if(data.int_value_ == 0){
        std::stringstream ss;
        ss << "number = " << float_number;
        plError::warn(108, ss.str());
      }
#endif
      result.float_value_ =  (plFloat)float_number / (plFloat)data.int_value_;
    break;
    }
  case plData::kplFLOAT:
    {
#ifdef PL_CHECK_DATA_ARITHMETIC_OPERATIONS
      if(data.float_value_ == PL_ZERO){
        std::stringstream ss;
        ss << "number = " << float_number;
        plError::warn(108, ss.str());
      }
#endif
      result.float_value_ = (plFloat)float_number / data.float_value_;
    break;
    }
  default:
    throw plError(2,"float / plData");
  }
}

//=============================================================================
template <>
inline plData operator/(const float&number, const plData &data)
{
  plData res;
  float_div(res, number, data);
  return res;
}

//=============================================================================
template <>
inline plData operator/(const double&number, const plData &data)
{
  plData res;
  float_div(res, number, data);
  return res;
}

//=============================================================================
template <>
inline plData operator/(const long double&number, const plData &data)
{
  plData res;
  float_div(res, number, data);
  return res;
}

//=============================================================================
//Addition of a number to a plData results in a plData*/
template <typename T>
inline plData operator+(const T&number, const plData &data)
{
  return data+number;
}

//=============================================================================
//Subtraction of a plData form a number results in a plData*/
template <typename T>
inline plData operator-(const T&number, const plData &data);

//=============================================================================
template <typename T>
inline void int_sub(plData &result, const T&int_number, const plData &data)
{
  switch (data.data_type_){
  case plData::kplINTEGER:
    result.data_type_ = plData::kplINTEGER;
    result.int_value_ = (long int)int_number-data.int_value_;
    break;
  case plData::kplFLOAT:
    result.data_type_ = plData::kplFLOAT;
    result.float_value_ = plFloat(int_number)-data.float_value_; 
    break;
  default:
    throw plError(2,"int - plData");
  }
}

//=============================================================================
template <>
inline plData operator-(const bool&b, const plData &data)
{
  plData res;
  int_sub(res, b, data);
  return res;
}

//=============================================================================
template <>
inline plData operator-(const int&number, const plData &data)
{
  plData res;
  int_sub(res, number, data);
  return res;
}

//=============================================================================
template <>
inline plData operator-(const unsigned int&number, const plData &data)
{
  plData res;
  int_sub(res, number, data);
  return res;
}

//=============================================================================
template <>
inline plData operator-(const long int&number, const plData &data)
{
  plData res;
  int_sub(res, number, data);
  return res;
}

//=============================================================================
template <>
inline plData operator-(const unsigned long int&number, const plData &data)
{
  plData res;
  int_sub(res, number, data);
  return res;
}

//=============================================================================
template <typename T>
inline void float_sub(plData &result, const T&float_number, const plData &data)
{
  result.data_type_ = plData::kplFLOAT;
  switch (data.data_type_){
  case plData::kplINTEGER:
    result.float_value_ = (plFloat)float_number - (plFloat)data.int_value_;
    break;
  case plData::kplFLOAT:
    result.float_value_ = (plFloat)float_number - data.float_value_; 
    break;
  default:
    throw plError(2,"float - plData");
  }
}

//=============================================================================
template <>
inline plData operator-(const float&number, const plData &data)
{
  plData res;
  float_sub(res, number, data);
  return res;
}

//=============================================================================
template <>
inline plData operator-(const double&number, const plData &data)
{
  plData res;
  float_sub(res, number, data);
  return res;
}

//=============================================================================
template <>
inline plData operator-(const long double&number, const plData &data)
{
  plData res;
  float_sub(res, number, data);
  return res;
}


#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
