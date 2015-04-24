/*=============================================================================
 * Product        : ProBT 
 * File           : plUniform.h
 * Author         : Juan-Manuel Ahuactzin
 * Creation       : Mon May 28 10:55:52 2001
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


#ifndef plUniform_h
#define plUniform_h

#include "plConfig.h"

#include <plDistribution.h>

/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/

/** The \em plUniform class implements 1-dimensional uniform distributions
 */
class PL_DLL_API plUniform : public plDistribution
{
public:
  /** Default constructor, needed by the serialization code. */
  plUniform();

  // ONE D

  /** Constructs a uniform distribution over the range of the variable \em variable. */
  explicit plUniform(const plSymbol &variable);

  /**  Creates a uniform distribution over @em variable in the range
      {min,..., max} for discrete variables and in the range [min, max[ for
      continuous ones. 
      This template function is instantiated for int, unsigned int, float, double, and long double.
  */
  template <typename T>
  plUniform(const plSymbol &variable, T min, T max);


  /** Get the parameters used for constructing this object. 
      This template function is instantiated for int, unsigned int, float, double, and long double.
  */
  template<typename T>
    void get_parameters(T& min, T& max) const;


  // N D
  /** Constructs a uniform distribution over the range of the variable \em variable. */
  explicit plUniform(const plVariablesConjunction &variables);

  /**  Creates a uniform distribution over @em variable in the range
      {min,..., max} for discrete variables and in the range [min, max[ for
      continuous ones.
      This template function is instantiated for int, unsigned int, float, double, and long double.
  */
  template <typename T>
  plUniform(const plVariablesConjunction &variable, T min, T max);

  /**  Creates a uniform distribution over @em variable in the range
      {min,..., max} for discrete variables and in the range [min, max[ for
      continuous ones.
      This template function is instantiated for int, unsigned int, float, double, and long double.
  */
  template <typename T>
    plUniform(const plVariablesConjunction &variable, const std::vector<T> &min, const std::vector<T> & max);


  /** Get the parameters used for constructing this object. 
      This template function is instantiated for int, unsigned int, float, double, and long double.
   */
  template<typename T>
    void get_parameters(std::vector<T> & min, std::vector<T> & max) const;

  
  /** Promote from a plDistribution. This makes sense because all the information
      is actually in parent class plDistribution. If the object being copied is
      actually of the wrong type, an exception is raised.
  */
  explicit plUniform(const plDistribution&);

  virtual bool is_base_class()const
  {
    return false;
  }

};


#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
