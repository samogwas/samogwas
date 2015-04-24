/*=============================================================================
 * Product        : ProBT 
 * File           : plArray.h
 * Author         : Juan-Manuel Ahuactzin
 * Creation       : Wed Mar 21 11:52:28 2001
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

#ifndef plArray_h
#define plArray_h

#include "plConfig.h"

#include <plVariablesConjunction.h>
#include <plSymbol.h>
#include <plType.h>

SPL_DECL2(plArray);

/*****************************************************************************
 *                           External Classes
 *****************************************************************************/
class kplArray;

/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/


//============================================================================
/** Implements single-type multidimensional variable arrays.

    For example, to construct a 3-dimentional array of type PL_BINARY_TYPE,
    with @em n entries for the first dimension,
    @em m entries for the second dimension, and @em p for the third one, you can write:
    \code
    plArray my_array("3DVariableArray", PL_BINARY_TYPE, 3, n, m, p);
    \endcode

    \sa pl1DArray
    \sa pl2DArray
 */
class PL_DLL_API plArray : public plVariablesConjunction
{

public:

  /**  Constructs a single-type multidimensional variable array. 
       The array named \em
       print_name contains variables of type \em variable_type,
       has dimensions \em tab_dim. The other parameters \em first_dimension,...
       are the sizes for each dimension. Example :
       \code
      plArray t("matrix",t1,2,3,4) 
      \endcode
      creates a table named \em matrix with
      variables of type \em t1 and of size (2 dimensions) \em 3x4.
  */
  plArray(const std::string &print_name, 
	  const plType &variable_type,
          int tab_dim,...);

  /** Default constructor */
  plArray();
  
  /** Assignment operator */
  plArray &operator=(const plArray &array);
  
  /** Copy constructor */
  plArray(const plArray &array);

  /** Erase the array of variables */
  virtual ~plArray();

  /** Access operator for an array of variables. Example, for a 2x2 table
      \em t you can access \em t(0,0), \em  t(0,1), \em t(1,0), and \em t(1,1). 
 */
  plSymbol operator()(int first_index,...) const;

  /** Gets the array name, given by the user.*/
  virtual std::string name() const;

protected:
  SPL_ACCESS2(plArray);
  kplArray* get_kernel_array() const;
  void set_kernel_array(kplArray* array);

protected:
  /** Pointer to the kernel array */
  kplArray *kernel_array_;

 private:
  plVariablesConjunction &operator^=(const plVariablesConjunction &var);
  
};

//============================================================================
/** Implements single-type one-dimensional variable arrays.

    For example, to construct a one-dimentional array of type PL_BINARY_TYPE,
    with @em n elements, you can write:
    \code
    pl1DArray my_1d_array("1DVariableArray", PL_BINARY_TYPE, n);
    \endcode

    \sa plArray
    \sa pl2DArray
 */
class PL_DLL_API pl1DArray : public plArray
{

public:

  /**  Constructs a Variable Set */
  pl1DArray(const std::string &print_name, 
            const plType &variable_type,
            unsigned int n);

  /** Default constructor */
  pl1DArray();

  /** Access operator */
  plSymbol operator()(int index) const;

  /** Same as operator()(int index) */
  plSymbol at(int index) const;

};

//============================================================================
/** Implements single-type two-dimensional variable arrays.

    For example, to construct \em nxm two-dimentional array (matrix)
    of type PL_BINARY_TYPE, you can write:
    \code
    pl2DArray my_2d_array("2DVariableArray", PL_BINARY_TYPE, n, m);
    \endcode

    \sa plArray
    \sa pl1DArray
 */
class PL_DLL_API pl2DArray : public plArray
{

public:

  /**  Constructs a Variable Set */
  pl2DArray(const std::string &print_name, 
            const plType &variable_type,
            unsigned int n, unsigned int m);

  /** Default constructor */
  pl2DArray();

  /** Access operator */
  plSymbol operator()(int index1, int index2) const;

  /** Same as  operator()(int index1, int index2) */
  plSymbol at(int index1, int index2) const;

};

#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
