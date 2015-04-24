/*=============================================================================
 * Product        : ProBT 
 * File           : plVariableIndexer.h
 * Author         : Kamel Mekhnacha
 * Creation       : Mon Oct 27 14:56:09 2003
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

#ifndef plVariableIndexer_h
#define plVariableIndexer_h

#include "plConfig.h"



#include <plVariablesConjunction.h>
#include <plDataValues.h>
#include <plDLLExport.h>

class plValues;
class kplTableIndex;

SPL_DECL2(plVariableIndexer);


//=============================================================================
/** Helper class for indexing the values of a discrete/discretized variable conjunction */
class PL_DLL_API plVariableIndexer  : public plSampleSpaceObject
{
public:
  
  /** Empty constructor */
  plVariableIndexer();

  /** Constructor using a variables conjunction */
  explicit plVariableIndexer(const plVariablesConjunction &vars);
  
  /** Copy constructor */
  plVariableIndexer(const plVariableIndexer& other);

  /** Assignment operator */
  plVariableIndexer &operator=(const plVariableIndexer& other);

  /** Same as operator=() */
  plVariableIndexer& assign_from(const plVariableIndexer& other)
  {
    return operator=(other);
  }
  
  /** Destructor */
  ~plVariableIndexer();
  
  /** Get the cardinality of the n-th symbol in the conjunction. */
  unsigned int get_symbol_cardinality(unsigned int n) const;
  
  
  /** Get the value corresponding to a given index in the output parameter 'value'.
      Returns 'true' if the index is in the range. */
  bool get_value_from_index(unsigned int index, plValues &value)const;
  
  /** Get the value corresponding to a given index in the output parameter 'value'.
      Returns 'true' if the index is in the range. 
      This template method is instantiated for 'int', 'unsigned int', 'float', 
      'double', and 'long double'.*/
  template <typename vectorT>
  bool get_value_from_index(unsigned int index, std::vector<vectorT> &value)const;
  
  /** Get the value corresponding to a given index in the output parameter 'value'.
      Returns 'true' if the index is in the range.
      This template method is instantiated for 'int', 'unsigned int', 'float', 
      'double', and 'long double'.*/
  template <typename arrayT>
  bool get_value_from_index(unsigned int index, arrayT *value)const;
  
  
  /** Get the index corresponding to a given value in the output parameter 'index'.
      Returns 'true' if the value is in the range. 
  */
  bool get_index_from_value(const plValues &value, unsigned int &index)const;
  
  /** Get the index corresponding to a given value in the output parameter 'index'.
      Returns 'true' if the value is in the range.
      This template method is instantiated for 'int', 'unsigned int', 'float', 
      'double', and 'long double'.*/
  template <typename vectorT>
  bool get_index_from_value(const std::vector <vectorT> &value, unsigned int &index)const;
  
  /** Get the index corresponding to a given value in the output parameter 'index'.
      Returns 'true' if the value is in the range. 
      This template method is instantiated for 'int', 'unsigned int', 'float', 
      'double', and 'long double'. */
  template <typename arrayT>
  bool get_index_from_value(arrayT *value, unsigned int &index)const;

  bool get_index_from_value(const plDataValues &value, unsigned int &index)const;
  bool get_value_from_index(unsigned int index, plDataValues &value)const;
  
 protected:
  virtual void Output(std::ostream &out) const;

 protected:
  SPL_ACCESS2(plVariableIndexer);
  
  kplVariableList variables_;
  kplTableIndex *table_index_;
  mutable plDataValues dval_;
 

  friend class plBayesLearnProbTable;
  template<typename T> friend class plCndLearnObject;
};

#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
