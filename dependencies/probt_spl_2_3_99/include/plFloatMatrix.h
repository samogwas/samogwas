/*=============================================================================
 * Product        : ProBT 
 * File           : plFloatMatrix.h
 * Author         : Christophe Coue
 * Creation       : Mar 12 2001
 *
 *=============================================================================
 *        (c) Copyright 2006, ProBayes SAS - all rights reserved
 *=============================================================================
 */

#ifndef FLOAT_MATRIX_H
#define FLOAT_MATRIX_H

#include "plConfig.h"


#include <plObject.h>
#include <plMath.h>
#include <plFloatVector.h>


class plFloatMatrix;
template<typename T>
PL_DLL_API plFloatMatrix operator*(const T&, const plFloatMatrix&);

SPL_DECL2(plFloatMatrix);

/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/


/** A \em plFloatMatrix is an \em m x \em n matrix of elements of type
    \em plFloat.*/
class PL_DLL_API plFloatMatrix :public plObject
{
  SPL_ACCESS2(plFloatMatrix);

 protected:

  plFloat **m_;

  size_t rz_;
  size_t cz_;

  plFloat elem_check(size_t r, size_t c) const;
  plFloat& elem_check(size_t r, size_t c);
  void destroy();
  void allocate(size_t r, size_t c);
  void resize_without_reset(size_t nr, size_t nc);
  void Output(std::ostream &out) const;

 private:
  template<typename T>
    void set_values( size_t d, T float_vector);

  template<typename T>
    void set_values(const std::vector <std::vector <T> > &stl_matrix);

 public:

  /** Default constructor */
  plFloatMatrix();
  
  /** Constructs a matrix having \em r x \em c  elements */
  plFloatMatrix(size_t r, size_t c);

  /** Constructs a matrix having \em d x \em d  elements */
  explicit plFloatMatrix(size_t d);

  /** Constructs a matrix having \em d x \em d  elements  and fills it using the
      \em float_vector C array of floating-point values so that
      \em m[i][j] = float_vector[i*d+j] for i, j = 0 to d-1.
  */
  plFloatMatrix(size_t d, const double* float_vector);

  plFloatMatrix(size_t d, const float* float_vector);

  plFloatMatrix(size_t d, const long double* float_vector);
   
  /** Constructs a matrix having \em d x \em d  elements  and fills it using the
      \em float_vector STL vector of \em float values so that
      m[i][j] = float_vector[i*d+j] for i, j = 0 to d-1,
      where d = sqrt(float_vector.size()).
  */
  explicit plFloatMatrix(const std::vector <double> &float_vector);
  explicit plFloatMatrix(const std::vector <float> &float_vector);
  explicit plFloatMatrix(const std::vector <long double> &float_vector);
   
  /**  Conversion from an STL float matrix.
   */
  explicit plFloatMatrix(const std::vector <std::vector <double> > &stl_matrix);
  explicit plFloatMatrix(const std::vector <std::vector <float> > &stl_matrix);
  explicit plFloatMatrix(const std::vector <std::vector <long double> > &stl_matrix);
   
  /** Copy constructor */
  plFloatMatrix( const plFloatMatrix &);


  /** Constructs a diagonal square matrix having \em diag_vector as diagonal.
      Previous content is lost. */
  void make_diagonal(const plFloatVector &diag_vector);

  /** Constructs a \em n x \em n identity matrix.
   */
  void make_identity(size_t n);

  /** Sets the size of the matrix to \em nr x \em nc. Note that the previous 
      content  is lost if the new size is different of the current one. */
  void resize(size_t nr, size_t nc);
  
  /** Sets the size of the matrix to \em n x \em n. Note that the previous
      content is lost if the new size is different of the current one. */
  void resize(size_t n);

  /** Destructor */
  virtual ~plFloatMatrix();

  /** Returns the number of rows */
  inline size_t rows() const {return rz_;}

  /** Returns the number of columns */
  inline size_t cols() const {return cz_;}

  /** Returns the (r, c) element of the matrix if \em r and \em c are not
      out of range */
  inline plFloat operator()(size_t r, size_t c) const 
  { 
    return elem_check(r,c); 
  }

  /** Returns a reference to the (r, c) element of the matrix if \em r and
      \em c are not out of range */
  inline plFloat& operator()(size_t r, size_t c) 
  { 
    return elem_check(r,c); 
  }

  /** Returns the (r, c) element of the matrix if \em r and \em c are
      not out of range.  Throws an error otherwise in DEBUG mode */
  inline plFloat at(size_t r, size_t c) const 
  { 
#ifdef DEBUG
    return elem_check(r,c);
#else
    return m_[r][c];
#endif
  }

  /** Returns the (r, c) element of the matrix if \em r and \em c are
      not out of range.  Throws an error otherwise in DEBUG mode */
  inline plFloat& at(size_t r, size_t c) 
  { 
#ifdef DEBUG
    return elem_check(r,c);
#else
    return m_[r][c];
#endif
  }
  


  /* Sets the value v at position r, c */
  void set_value(const size_t &r, const size_t &c, const plFloat &v);

  /* Sets the value v at position r, c */
  plFloat get_value(const size_t &r, const size_t &c) const
  {
    return at(r, c);
  }


  /** Returns a C array containing the ith row of the matrix */
  inline plFloat* operator[](size_t i) const { return m_[i];};
  
  /** Returns a C array containing the ith row of the matrix */
  inline plFloat* operator[](size_t i) { return m_[i];};

  /** Returns a plFloatVector containing the ith row of the matrix */
  plFloatVector row(size_t i) const;

  /** Returns a plFloatVector containing the jth column of the matrix */
  plFloatVector column(size_t j) const;


  /** Assignment operator */
  plFloatMatrix& operator=(const plFloatMatrix &);

  /** Same as operator=() */
  plFloatMatrix& assign_from(const plFloatMatrix& other)
  {
    return operator=(other);
  }

  /** Addition operator */
  plFloatMatrix operator+(const plFloatMatrix&) const; 


  /** Addition */
  plFloatMatrix add(const plFloatMatrix& other) const
  {
    return operator+(other);
  } 
  
  
  /** In-place addition operator */
  plFloatMatrix &operator+=(const plFloatMatrix&);

  /** In-place addition  */
  plFloatMatrix &add_in_place(const plFloatMatrix& other)
  {
    return operator+=(other);
  }

  /** Subtraction operator */
  plFloatMatrix operator-(const plFloatMatrix&) const;

  /** Subtraction  */
  plFloatMatrix subtract(const plFloatMatrix&other) const
  {
    return operator-(other);
  }
  
  /** In-place subtraction operator */
  plFloatMatrix &operator-=(const plFloatMatrix&);

  /** In-place subtraction operator */
  plFloatMatrix &subtract_in_place(const plFloatMatrix& other)
  {
    return operator-=(other);
  }


  /** Multiplication with another matrix operator */
  plFloatMatrix operator*(const plFloatMatrix&) const;

  /** Multiplication with a scalar operator.
      This template operator is instantiated for float, double, long-double,
      int and size_t operands.
  */
  template<typename T>
    friend PL_DLL_API plFloatMatrix operator*(const T&,const plFloatMatrix&);
  
  /** In-place multiplication  with a scalar operator.
      This template operator is instantiated for float, double, long-double,
      int and size_t operands.
  */
  template<typename T>
    plFloatMatrix& operator*=(const T&);

  /** Multiplication with a vector operator */
  plFloatVector operator*(const plFloatVector&) const;

  /** Equality test operator */
  bool operator==(const plFloatMatrix&) const;

  /** Equality test */
  bool equal(const plFloatMatrix& other) const
  {
    return operator==(other);
  }

  /**  Returns the transpose of the matrix */
  plFloatMatrix transpose() const;

  /**  Returns the transpose of the matrix in the \em result matrix */
  void transpose(plFloatMatrix & result) const;

  /** Returns the trace of the matrix */
  plFloat trace() const;

  /** Inverses the matrix if inversible and returns the pseudoinverse if the matrix is not
      inversible.
      \param[out] inverse  The resulting inverse or pseudoinverse matrix 
      \param[in] threshold The threshold to be used by the \em Greville's  algorithm.

      \returns the rank of the matrix. 

      The pseudoinverse allows computing a 'best fit' (least squares) solution
      to a system of linear equations that lacks a unique solution
      (over-determined systems: nrows > ncolmns). Another use is to find the
      minimum (Euclidean) norm solution to a system of linear equations with
      multiple solutions (under-determined systems: nrows < ncolmns).
  */
  unsigned int inverse_using_greville(plFloatMatrix& inverse, plFloat threshold) const;

  /** Inverses the matrix and returns the result in the \em inverse 
      matrix using eigen decomposition.

      ATTENTION: The algorithm is ONLY valid for
      symmetric positive definite matrices (such as variance/covariance matrices)

      \return the determinant of this matrix
  */
  plFloat inverse_using_eigen_decomposition(plFloatMatrix &inverse) const;


  /** Inverses the matrix and returns the result in the \em inverse 
      matrix using eigen decomposition.
      This version allows avoiding internal allocations :
      parameters mat_n_n_tmp1, mat_n_n_tmp2, and 
      vect_n_tmp buffers will be used internally.

      ATTENTION: The algorithm is ONLY valid for
      symmetric positive definite matrices (such as variance/covariance matrices)

      \return the determinant of this matrix
  */
  plFloat inverse_using_eigen_decomposition(plFloatMatrix &inverse,
                                            plFloatMatrix &mat_n_n_tmp1,
                                            plFloatMatrix &mat_n_n_tmp2,
                                            plFloatVector &vect_n_tmp) const;
  
  /** Compute the eigen-decomposition the matrix and returns: \n
      - Eigen vectors in the result matrix \em Eigen_Matrix \n
      - Eigen values in the result vector \em Eigen_Vector \n
      ATTENTION: The algorithm used for eigen-decomposition is ONLY valid for
      symmetric positive definite matrices (such as variance/covariance matrices)
  */    
  void compute_eigen_decomposition( plFloatMatrix& Eigen_Matrix, plFloatVector& Eigen_Vector) const;
  

  /** Resets the matrix to val.
   */
  void reset(plFloat val=0.);
  
  /** Return @em true iff it contains zero elements */
  inline bool is_empty()const{return !m_;}

  /** Multiply this matrix with the 'other' one and stores the result in 'result' */
  void multiply(const plFloatMatrix &other, plFloatMatrix &result)const;

  /** Multiply this matrix with the transpose of 'other' and stores the result in 'result' */
  void multiply_with_transpose(const plFloatMatrix &other, plFloatMatrix &result)const;

  /** Multiply this matrix with the transpose of 'other' and stores the result in 'result' when the result is known to be symmetric */
  void multiply_with_transpose_symmetric(const plFloatMatrix &other, plFloatMatrix &result)const;

  /** Multiply the transpose of this matrix with 'other' and stores the result in 'result' */
  void multiply_transpose_with(const plFloatMatrix &other, plFloatMatrix &result)const;


  /** Multiply this matrix with a vector and stores the result in 'result' */
  void multiply(const plFloatVector &vect, plFloatVector &result)const;

  /** Multiply the transpose of this matrix with a vector and stores the result in 'result' */
  void multiply_transpose_with(const plFloatVector &vect, plFloatVector &result)const;

};


#endif // FLOAT_MATRIX_H

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
