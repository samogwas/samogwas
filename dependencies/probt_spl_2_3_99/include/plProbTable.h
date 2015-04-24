/*=============================================================================
 * Product        : ProBT 
 * File           : plProbTable.h
 * Author         : Kamel Mekhnacha
 * Creation       : Tue Oct 23 10:50:44 2001
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


#ifndef plProbTable_h
#define plProbTable_h

#include "plConfig.h"


#include <plDistribution.h>

/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/


/** A plProbTable represents a table of probability values on discrete and/or
 continuous-by-interval variables. This includes both the one-dimensional case
 and the multi-dimensional case. */
class PL_DLL_API plProbTable :public plDistribution
{
 protected:
  
  template <typename Titerator>
  void set(const plVariablesConjunction& V, 
           Titerator begin,
           Titerator end, 
           bool is_already_normalized);
 
 public:

  /** Default constructor, needed by the serialization code. */
  inline plProbTable() 
    : plDistribution()
  { 
    signature_.set(PLProbTable); 
  }

  /** Constructor 1: Constructs a probability table over the variable
      (s) \em V. If @em random_prob_values is @em true, the table is
      filled at random. If If @em random_prob_values is @em false (the default),
      the probability table is set to zero.
  */
  explicit plProbTable(const plVariablesConjunction& V, bool random_prob_values=false);
  
  /** Constructor2: Constructs a probability table on the variable (s) \em V
      and fills it using the values contained in the C array \em values. You
      should set is_already_normalized to true if values is a normalized table
      of numbers.\n
      
      In multi-dimentional cases (H = H1^H2), where \em H1 and \em H2
      can take n1 and n2 values respectively, it constructs the probability
      table on P(H) using the values \em values as follows:\n

      - P([H1=1, H2=1]) = values[0] \n
      - P([H1=1, H2=2]) = values[1] \n
      . \n
      . \n
      . \n
      - P([H1=1, H2=n2]) = values[n2-1] \n\n
      - P([H1=2, H2=1]) = values[n2]) \n
      - P([H1=2, H2=2]) = values[n2+1] \n
      . \n
      . \n
      . \n
      - P([H1=2, H2=n2]) = values[2*(n2-1)] \n
      . \n
      . \n
      . \n
      - P([H1=n, H2=m]) = values[(n1-1)*(n2-1)] \n
  */
  template<typename T>
  plProbTable( const plVariablesConjunction& V, const T *values,
               bool is_already_normalized = false )
    : plDistribution(V, PLProbTable)
  {
    unsigned int n_elems = static_cast<unsigned int>(V.cardinality());
    set(V,
        values, values+n_elems,
        is_already_normalized);
  }
  plProbTable( const plVariablesConjunction& V, const double *values,
               bool is_already_normalized = false )
    : plDistribution(V, PLProbTable)
  {
    unsigned int n_elems = static_cast<unsigned int>(V.cardinality());
    set(V,
        values, values+n_elems,
        is_already_normalized);
  }
  plProbTable( const plVariablesConjunction& V, const float *values,
               bool is_already_normalized = false )
    : plDistribution(V, PLProbTable)
  {
    unsigned int n_elems = static_cast<unsigned int>(V.cardinality());
    set(V,
        values, values+n_elems,
        is_already_normalized);
  }


  /** Constructor 3: Constructs a probability table on the variable (s) \em V
      and fills it using the values contained in the STL vector \em
      values. Same as Constructor2 for multi dimensional cases. */

  template<typename T>
  plProbTable( const plVariablesConjunction& V, const std::vector<T> &values,
               bool is_already_normalized = false)
    : plDistribution(V, PLProbTable)
  {
    set(V,
        values.begin(), values.end(),
        is_already_normalized);
  }
  plProbTable( const plVariablesConjunction& V, const std::vector<double> &values,
               bool is_already_normalized = false)
    : plDistribution(V, PLProbTable)
  {
    set(V,
        values.begin(), values.end(),
        is_already_normalized);
  }
  plProbTable( const plVariablesConjunction& V, const std::vector<float> &values,
               bool is_already_normalized = false)
    : plDistribution(V, PLProbTable)
  {
    set(V,
        values.begin(), values.end(),
        is_already_normalized);
  }


  /** Constructor 4: Constructs a probability table on the variable (s) \em V
      and fills it using the values defined by the range begin [begin, end). 
      Same as Constructor2 for multi dimensional cases. */
  template <typename Titerator>
  plProbTable( const plVariablesConjunction& V, 
               Titerator begin,
               Titerator end,
               bool is_already_normalized = false)
    : plDistribution(V, PLProbTable)
  {
    set(V,
        begin, end,
        is_already_normalized);
  }

  /** Destructor */
  virtual ~plProbTable(){};

  /** Get the parameters used for constructing this object. */
  void get_parameters(std::vector<plProbValue>& values) const;

  /** Promote from a plDistribution. If the object being copied is
      actually of the wrong type, an exception is raised.
  */
  explicit plProbTable(const plDistribution&);

  /** Get a const reference to the probability table */
  const std::vector<plProbValue>& table()const;

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
