/*==============================================================================
 * Product        : ProBT 
 * File           : plLearnHistogram.h
 * Author         : Manuel Yguel
 * Creation       : Mon Nov 10 08:52:09 2003
 *
 *==============================================================================
 *     (c) Copyright 2000, Centre National de la Recherche Scientifique,
 *                              all rights reserved
 *==============================================================================
 *
 *------------------------- Description ----------------------------------------
 * 
 *   
 *------------------------------------------------------------------------------
 */


#ifndef plLearnHistogram_h
#define plLearnHistogram_h

#include "plConfig.h"

#include <plNonCndLearnObject.h>
#include <plProbTable.h>

#include <plVariableIndexer.h>


SPL_DECL1(plLearnHistogram);

/** The class for learning n dimensionnal histograms.*/
class PL_DLL_API plLearnHistogram : public plMLLearnObject
{
public:
  /** Type of the variable (multi-dim). */
  typedef plVariablesConjunction VarType;
  
  /** Default constructor. */
  plLearnHistogram();
	
  /** Constructor using a set of variables. */
  explicit plLearnHistogram(const plVariablesConjunction &vars);
		
  /** Constructor using a set of variables, initial frequencies, and an initial
      weight. */
  template <class arrayType>
  plLearnHistogram(const plVariablesConjunction &vars,
                   arrayType init_freq, plFloat init_weight=PL_ONE)
    : plMLLearnObject(vars)
    , freq_((unsigned int)vars.cardinality())
    , init_freq_(freq_.size())
    , init_weight_(init_weight)
    , var_index_(vars)
    , has_initialization_(true)
  {		
    total_weight_ = init_weight_;
    for(unsigned int i = 0; i < freq_.size(); i++)
      init_freq_[i] = freq_[i] = init_freq[i];
  }
 	
  /** Dectructor. */
  virtual ~plLearnHistogram();
  
  /** Resets learning. */
  void reset();
  
  /** Returns the probability table corresponding to the learnt distribution */
  plDistribution get_distribution() const;

  /** Returns the probability table corresponding to the learnt distribution in
      the output parameter 'table'.  Supposes that 'table' has the exact
      size. 
  */
  template<typename T>
  void get_probability(T *table)const
  {
    if(all_vars_.is_empty())
      throw plError(75, "when calling 'plLearnHistogram::get_probability'");
    
    if(total_weight_ != PL_ZERO){
      for(unsigned int i = 0; i < freq_.size(); ++i) table[i] = freq_[i]/total_weight_;
    }
    else{
      for(unsigned int i = 0; i < freq_.size(); ++i) table[i] = freq_[i];
    }
  }
  
  /** Returns the probability table corresponding to the learnt distribution in
      the output parameter 'table'. 
  */
  template<typename T>
  void get_probability(std::vector <T> &table)const
  {
    table.resize( freq_.size() );
    get_probability( &(table[0]) );
  }
  
  /** Returns the frequency table corresponding to the learnt distribution in
      the output parameter 'table'.  Supposes that 'table' has the exact
      size. 
  */
  template<typename T>
  void get_frequency(T *table)const
  {
    if(all_vars_.is_empty())
      throw plError(75, "when calling 'plLearnHistogram::get_frequency'");
    
    for(unsigned int i = 0; i < freq_.size(); ++i) table[i] = freq_[i];
  }
  
  /** Returns the frequency table corresponding to the learnt distribution in
      the output parameter 'table'. */
  template<typename T>
  void get_frequency(std::vector <T> &table)const
  {
    table.resize( freq_.size() );
    get_frequency( &(table[0]) );
  }
  
  /** Returns a constant reference to the frequency table corresponding to the
      learnt distribution. */
  const std::vector <plFloat> &get_frequency()const{return freq_;}
	
  /** Returns the actual range of the head variables according to the learnt
      points. */
  void get_actual_min_max(plValues &min, plValues &max) const;
	
  /** Returns the size of the histogram (number of bins). */
  size_t size() const { return freq_.size(); }

	
  void get_parameters(plValues &params) const
  { 
    if(all_vars_.is_empty())
      throw plError(75, "when calling 'plLearnHistogram::get_parameters'");

    for(unsigned int i = 0; i < freq_.size(); ++i){
      params[i] = freq_[i];
    }

  }

  unsigned int get_n_parameters()const
  {
    if(all_vars_.is_empty())
      throw plError(75, "when calling 'plLearnHistogram::get_n_parameters'");
    
    return (unsigned int) (freq_.size()-1);
  }
  
  virtual plLearnObject* clone()const
  {
    return new plLearnHistogram(*this);
  }

protected:
   
  /** Adds a point \em point with a given weight \em weight and updates the
      statistics. This is an internal method, do not use it directly. Use
      add_point instead.
  */
  bool internal_addPoint(const plDataValues &point, plFloat weight=PL_ONE);

  virtual void Output(std::ostream &out) const;

  
protected:
  SPL_ACCESS1(plLearnHistogram);
  
  std::vector <plFloat> freq_;
  std::vector <plFloat> init_freq_;
  plFloat init_weight_;

  plVariableIndexer var_index_;
  
  bool has_initialization_;
  
  template <class T> friend class plCndLearnObject;
};


#endif //plLearnHistogram_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
