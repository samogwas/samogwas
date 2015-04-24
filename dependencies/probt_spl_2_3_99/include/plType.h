/*=============================================================================
 * Product        : ProBT 
 * File           : plType.h
 * Author         : Juan-Manuel Ahuactzin
 * Creation       : Wed Mar 21 12:26:12 2001
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

#ifndef plType_h
#define plType_h

#include "plConfig.h"

#include <plObject.h>
#include <plData.h>
#include <plVariableType.h>

/*****************************************************************************
 *                           External Classes
 *****************************************************************************/

class kplType;

/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/

SPL_DECL2(plType);

//=============================================================================
/**
  \em plType is the base class of all ProBT types.
  */
class PL_DLL_API plType : public plSampleSpaceObject
{
protected:
  /* friend classes, methods and functions */
  friend class plArray;
  friend class plSymbol;
  friend class plVariablesConjunction;
  SPL_ACCESS2(plType);

  /** Pointer to the distribution type */
  kplType *kernel_type_;

   void set_kernel_type(kplType *k_type);
   explicit plType(kplType*);

  void Output(std::ostream &out) const;

public:

  /** Constructor */
  plType();

  /** Copy constructor */
  plType(const plType& other);
  
  /** Errase the type */
  virtual ~plType();

  /** Assignment operator. */
  plType& operator=(const plType& other);

  /** Same as operator=() */
  plType& assign_from(const plType& other)
  {
    return operator=(other);
  }
  
  /** Returns the start value of the type */
  const plData& first_value() const;

  /** Returns the minimum value of the type */
  const plData& get_min() const;

  /** Returns the maximum value of the type */
  const plData& get_max() const;

  /** Returns \em true if \em v has a successor and sets \em v
      to the successor otherwise it return \em false */
  bool next_value(plData &v) const;

  /** Returns \em true if \em v has a successor and sets \em v
      to the successor otherwise it return \em false.
      Implemented for typenames float, double, long double, int. */
  template<typename T>
  bool next_value(T &v) const;

  /** Get the type cardinality. */
  unsigned int cardinality() const;

  /** Get the variable type as a value from
      enum {PL_INTEGER, PL_REAL, PL_CONTINUOUS_INTERVAL, PL_DISCRETE_INTERVAL,
      PL_LABEL, PL_UNKNOWN_TYPE} */
  plVariableType get_var_type() const;

  /** Return true if the value is valid for this type. */
  bool value_is_valid(const plData &v) const;

  /** Return true if the object is default-constructed (as opposed to fully
      initialized). In that case, it is invalid for the creation of
      plSymbols. */
  bool is_empty() const { return (0==kernel_type_); }

  /** For discretized types, get as result the center of the interval
      represented by the given index. Throws if the type is not
      discretized. Return false if the index is out-of-bounds, true otherwise. */
  bool get_value_from_index(plData& result, unsigned int index) const;

  /** For discretized types, draws a value from the interval represented 
      by the given index. Throws if the type is not discretized. Return
      false if the index is out-of-bounds, true otherwise. */
  bool get_random_value_from_index(plData& result, unsigned int index) const;

  /** For discretized types, get the index representing the given value.
      Throws if the type is not discretized. Return false if the value is invalid,
      true otherwise. */
  template <typename T>
  bool get_value_index(unsigned int &result, const T &value) const;

  /** Get the values of the type as strings */
  std::vector<std::string> get_values_as_strings() const;
  
  /** Equality operator */
  bool operator==(const plType& other) const;

  /** Equality  */
  bool equal(const plType& other) const
  {
    return operator==(other);
  }


  /** Inequality operator */
  bool operator!=(const plType& other) const;

  /** Inequality  */
  bool not_equal(const plType& other) const
  {
    return operator!=(other);
  }

};


//=============================================================================
/**
  \em plDiscreteType is the base class of all ProBT discrete types.
  */
class PL_DLL_API plDiscreteType : public plType
{
public:
  /// Default constructor
  plDiscreteType()
    : plType()
  {}

  /** Promote from a plType. This makes sense because all the information
      is actually in parent class plType. If the object being copied is
      actually of the wrong type, an exception is raised.
  */
  plDiscreteType(const plType &other)
    : plType(other)
  {
    if (other.get_var_type() !=  PL_INTEGER 
        && other.get_var_type() != PL_LABEL && other.get_var_type() != PL_DISCRETE_INTERVAL)
      throw plError(8, "promoting a plType to a plDiscreteType");
  }
};

//=============================================================================
/**
  \em plContinuousType is the base class of all ProBT continuous types.
  */
class PL_DLL_API plContinuousType : public plType
{
public:

  /// Default constructor
  plContinuousType()
    : plType()
  {}

  /** Promote from a plType. This makes sense because all the information
      is actually in parent class plType. If the object being copied is
      actually of the wrong type, an exception is raised.
  */
  plContinuousType(const plType &other)
    : plType(other)
  {
    if(other.get_var_type() !=  PL_REAL && other.get_var_type() != PL_CONTINUOUS_INTERVAL)
      throw plError(8, "promoting a plType to a plContinuousType");
  }
};

#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
