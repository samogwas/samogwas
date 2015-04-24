/*=============================================================================
 * Product        : ProBT 
 * File           : plUserProbFunction.h
 * Author         : Kamel Mekhnacha
 * Creation       : Tue Apr  9 11:09:30 2002
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

#ifndef plUserProbFunction_h
#define plUserProbFunction_h

#include "plConfig.h"

#include <plValues.h>
#include <plDLLExport.h>
#include <plSharedObject.h>

//-----------------------------------------------------------------------------
class PL_DLL_API kplUserProbFunction: public plSharedObject
{

protected:
  /** Function print name */
  std::string name_;

  kplVariableList variables_;

  plValues *input_values_;

  bool has_side_effect_;

  /** Called by conditional distributions using external functions. */
  virtual void init(int /* input_size */) {}

  /** Writes the External Probability Function at the \em out
      stream. */
  virtual void Output(std::ostream &out) const;
  
private:
  kplUserProbFunction(const kplUserProbFunction&);
  kplUserProbFunction& operator=(const kplUserProbFunction&);
  
public:
  kplUserProbFunction();
  
  kplUserProbFunction(const kplVariableList& variables, 
		      const std::string& func_name);
  
  virtual ~kplUserProbFunction();

  const kplVariableList& get_variables() const;
     
  void set_variables(const kplVariableList &variables);

  virtual plProbValue operator()(const plValues& values) const = 0;

  const std::string& get_name() const { return name_; }

  void set_has_side_effect(bool hse){has_side_effect_ = hse;}
  
  bool has_side_effect()const{return has_side_effect_;}


  friend std::ostream& operator<<( std::ostream &out,
                                   const kplUserProbFunction &function);

  friend class kplAnonymousKernel;
  friend class kplCndAnonymousKernel;
};

//-----------------------------------------------------------------------------
#endif //plUserProbFunction_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
