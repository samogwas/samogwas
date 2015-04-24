/*=============================================================================
 * Product        : ProBT 
 * File           : plUserFunction.h
 * Author         : Kamel Mekhnacha
 * Creation       : Wed Feb  6 14:47:01 2002
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

#ifndef plUserFunction_h
#define plUserFunction_h

#include "plConfig.h"
#include "plValues.h"
#include "plSharedObject.h"
#include <plDLLExport.h>

//-----------------------------------------------------------------------------
class PL_DLL_API kplUserFunction: public plSharedObject
{
protected:
  /** Function print name */
  std::string name_;

  /** Variable of the search parameters. */
  kplVariableList all_variables_;

  /** Variable of the search parameters. */
  kplVariableList search_variables_;
  
  /** Variable of the known parameters. */
  kplVariableList known_variables_;

  /** Input parameters */
  plValues *input_values_;

  /** Output parameters */
  plValues *output_values_;

  bool has_side_effect_;

  bool initialized_;


  
private:
  kplUserFunction(const kplUserFunction&);
  kplUserFunction& operator=(const kplUserFunction&);

public:
  kplUserFunction();
  
  kplUserFunction(const kplVariableList& known_params,
                  const std::string& func_name);
  
  kplUserFunction(const kplVariableList& search_params, 
                  const kplVariableList& known_params,
                  const std::string& func_name);
  
  virtual ~kplUserFunction();

  const kplVariableList& get_all_variables() const;
  
  const kplVariableList& get_search_variables() const;
  
  const kplVariableList& get_known_variables() const;
    
  plDataValues operator()(const plValues& values) const;

  virtual void evaluate(plDataValues &output_values,
                        const plDataValues& values) const = 0;


  /** Sets the all, search and known variables lists */
  void set_variables(const kplVariableList &search_vars, 
                     const kplVariableList &known_vars);

  void write(std::ostream &out) const;

  void write_using_map(std::ostream &out, 
                       const std::map <kplVariable*, kplVariable *> &rename_map) const;


  const std::string& get_name() const { return name_; }

  void set_has_side_effect(bool hse){has_side_effect_ = hse;}
  
  bool has_side_effect()const{return has_side_effect_;}

  /** Called by conditional distributions using external functions. */
  virtual void init(size_t output_size, size_t input_size);

  friend std::ostream& operator<<( std::ostream &out,
                                   const kplUserFunction &function);
};

//-----------------------------------------------------------------------------
#endif//plUserFunction_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
