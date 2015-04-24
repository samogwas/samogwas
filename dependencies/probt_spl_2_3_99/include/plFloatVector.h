/*=============================================================================
 * Product        : ProBT 
 * File           : plFloatVector.h
 * Author         : Christophe Coue
 * Creation       : Mar 12 2001
 *
 *=============================================================================
 *     (c) Copyright 2000, Centre National de la Recherche Scientifique,
 *                              all rights reserved
 *=============================================================================
*/

#ifndef FLOAT_VECTOR_H
#define FLOAT_VECTOR_H

#include "plConfig.h"


#include <plObject.h>
#include <plMath.h>
#include <plError.h>
#include <plDataValues.h>


SPL_DECL2(plFloatVector);

/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/





/** A \em plFloatVector is a vector of \em n elements of type \em plFloat.*/
class PL_DLL_API plFloatVector : public plObject
{
  SPL_ACCESS2(plFloatVector);

  friend class plFloatMatrix;

protected:

  std::vector<plFloat> v_;

  /** checked access */
  plFloat elem_check(size_t i) const;

  /** checked access */
  plFloat& elem_check(size_t i);

    /** Output operator */
  void Output(std::ostream &out) const;
public:

  /** Returns vector's size */
  inline size_t size() const { return v_.size(); }
 
  // Constructors

  /** Default void constructor */
  plFloatVector();

  /** Constructs a vector having \em r elements initilized to \em val.
  */
  plFloatVector(size_t r, double val);

  /** Constructs a vector having \em r elements initilized to \em val.
  */
  plFloatVector(size_t r, float val);

  /** Constructs a vector having \em r elements initilized to \em val.
  */
  plFloatVector(size_t r, long double val);
  
  /** Constructs a vector having \em r elements */
  explicit plFloatVector(size_t r);

  /** Constructs a vector having \em r elements and fills it using the \em
      float_vector C array of values.
  */
  plFloatVector(size_t r, const double *float_vector);
 
  plFloatVector(size_t r, const float *float_vector);

  plFloatVector(size_t r, const long double *float_vector);

  /** Constructs a vector and fills it using the \em float_vector STL vector
      of values.
  */
  explicit plFloatVector(const std::vector <double> &float_vector);

  explicit plFloatVector(const std::vector <float> &float_vector);

  explicit plFloatVector(const std::vector <long double> &float_vector);

  /** Constructs a vector and fills it using the data values*/
  explicit plFloatVector(const plDataValues &values);

  /** Copy constructor */
  plFloatVector( const plFloatVector &v2);


  /** Destructor */
  virtual ~plFloatVector();

  /** Sets the size of the vector to \em r. Note that the previous content is
      lost. */
  void resize(size_t r);

  // operators

  /** Assignement operator */
  plFloatVector& operator=(const plFloatVector &);

  /** Same as operator=() */
  plFloatVector& assign_from(const plFloatVector& other)
  {
    return operator=(other);
  }

  /** Addition operator */
  plFloatVector operator+(const plFloatVector&) const;

  /** Addition */
  plFloatVector add(const plFloatVector& other) const
  {
    return operator+(other);
  }

  
  /** In-place addition operator */
  plFloatVector &operator+=(const plFloatVector&);

  /** In-place Addition */
  plFloatVector &add_in_place(const plFloatVector& other)
  {
    return operator+=(other);
  }


  /** Subtraction operator */
  plFloatVector operator-(const plFloatVector&) const;

  /** Subtraction  */
  plFloatVector subtract(const plFloatVector& other) const
  {
    return operator-(other);
  }
  
  /** In-place subtraction operator */
  plFloatVector &operator-=(const plFloatVector&);

  /** In-place subtraction  */
  plFloatVector &subtract_in_place(const plFloatVector& other)
  {
    return operator-=(other);
  }

  /** Multiplication with a scalar */
  plFloatVector operator*(plFloat) const;

    /** Multiplication with a scalar */
  plFloatVector multiply(plFloat f) const
  {
    return operator*(f);
  }

  
  /** In-place multiplication with a scalar */
  plFloatVector& operator*=(plFloat f);

  /** In-place multiplication with a scalar */
  plFloatVector& multiply_in_place(plFloat f)
  {
    return operator*=(f);
  }

  /** Division with a scalar */
  plFloatVector operator/(plFloat) const;
  
  /** Division with a scalar */
  plFloatVector divide(plFloat f) const
  {
    return operator/(f);
  }

  /** In-place division with a scalar */
  plFloatVector& operator/=(plFloat f);

  /** In-place  division with a scalar */
  plFloatVector& divide_in_place(plFloat f)
  {
    return operator/=(f);
  }

  /** Multiplication with a scalar operator */
  friend PL_DLL_API plFloatVector operator*(plFloat ,const plFloatVector&);

  /** Returns the ith element of the vector */
  inline plFloat operator[](size_t i) const { return v_[i]; }

  /** Returns a reference to the ith element of the vector */
  inline plFloat& operator[](size_t i) { return v_[i]; }

  /** Returns the ith element if \em i is less than vector's size. Generates
      an error otherwise */
  inline plFloat operator()(size_t i) const 
  { 
    return elem_check(i); 
  }
  
  /** Returns the ith element if \em i is less than vector's size. Generates
      an error otherwise in DEBUG mode */
  inline plFloat at(size_t i) const 
  { 
#ifdef DEBUG
    return elem_check(i); 
#else
    return v_[i];
#endif
  }

  /** Returns a reference to the ith element if \em i is less than
      vector's size. Generates an error otherwise */
  inline plFloat& at(size_t i) 
  { 
#ifdef DEBUG
    return elem_check(i); 
#else
    return v_[i];
#endif
  }
  

  /** Returns a reference to the ith element if \em i is less than vector's
      size. Generates an error otherwise */
  inline plFloat& operator()(size_t i) { return elem_check(i); }

  /** Sets the value at position i */
  void set_value(const size_t &i, const plFloat & val);

  /** Returns the ith element if \em i is less than vector's size. Generates
      an error otherwise */
  plFloat get_value(size_t i) const
  {
    return elem_check(i); 
  }

  /** Equality test operator */
  bool operator==(const plFloatVector&) const;	

  /** Equality test */
  bool equal(const plFloatVector& other) const
  {
    return operator==(other);
  }

  /** Computes the dot (inner) product of two vectors */
  plFloat dot_product(const plFloatVector &a) const ;
  
   
  /** Computes the cross product of two vectors. This method is only allowed
   * for 3-dimensional vectors */
  plFloatVector cross_product(const plFloatVector &a) const;

  /** Normalizes the vector, stores the result in \em result vector and
   * returns the norm of the vector*/
  plFloat normalize(plFloatVector &result) const;

  /** Returns the product of all elements */
  plFloat product() const;

  /** Resets the vector to val (0 by default).
  */
  void reset(plFloat val=0.);

  /** Return @em true iff it contains zero elements */
  inline bool is_empty()const{return v_.empty();}

  /** Return a const reference to the raw data vector */
  inline const std::vector<plFloat> &data()const{return v_;}

 private:
template<typename T>
  void set_values(size_t r, T float_vector);

};


#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
