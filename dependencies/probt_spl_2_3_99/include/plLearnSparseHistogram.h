/*==============================================================================
 * Product        : ProBT 
 * File           : plLearnSparseHistogram.h
 * Author         : Ronan Le Hy
 * Creation       : $Author: mekhnach $ $Date: 2010/12/30 12:27:13 $ $Revision: 1.35 $
 *
 *------------------------- Description ----------------------------------------
 *   Learning n dimensional sparse histograms.
 *   
 *------------------------------------------------------------------------------
 */

#ifndef plLearnSparseHistogram_h
#define plLearnSparseHistogram_h

#include "plConfig.h"

#include <plNonCndLearnObject.h>

class kplProbMap;
SPL_DECL1(plLearnSparseHistogram);

//-----------------------------------------------------------------------------
/** A class for learning sparse histograms.*/
class PL_DLL_API plLearnSparseHistogram : public plMLLearnObject
{
 public:
  /** Type of the variable (multi-dim). */
  typedef plVariablesConjunction VarType;

  /** Default constructor. */
  plLearnSparseHistogram();

  /** Copy constructor. */
  plLearnSparseHistogram(const plLearnSparseHistogram &histo);
  
  /** Constructor using a set of variables. */
  explicit plLearnSparseHistogram(const plVariablesConjunction &vars);
  
  /** Destructor. */
  virtual ~plLearnSparseHistogram();
	
  /** Returns the probability table corresponding to the learnt distribution */
  plDistribution get_distribution() const;
  
  /** Resets learning. */
  void reset();

  /** Assignement operator. */
  plLearnSparseHistogram& operator=(const plLearnSparseHistogram &histo);

  /** Returns the probability table corresponding to the learnt distribution in
      the output parameter 'table'. 
  */
  void get_probability(std::vector <plProbValue> &table)const;

  /** Returns the probability table corresponding to the learnt distribution in
      the output parameter 'table'.  Supposes that 'table' has the exact
      size. */
  void get_probability(plProbValue *table)const;

  void get_parameters(plValues &params) const;

  unsigned int get_n_parameters()const
  {
    if(all_vars_.is_empty())
      throw plError(75, "when calling 'plLearnSparseHistogram::get_n_parameters'");

    return (unsigned int)all_vars_.cardinality()-1;
  }

  virtual plLearnObject* clone()const
  {
    return new plLearnSparseHistogram(*this);
  }

 protected:
  SPL_ACCESS1(plLearnSparseHistogram);

  /** Adds a point \em point with a given weight \em weight and updates the
      statistics.  This is an internal method, do not use it directly. Use
      add_point instead. */
  bool internal_addPoint(const plDataValues &point, plFloat weight = PL_ONE);
  
  template<typename arrayT>
    void get_probability_tmp(arrayT table)const;

  // Pretty-prints this object.
  void Output(std::ostream &out) const;

 private:
  kplProbMap *prob_map_;

  template <class T> friend class plCndLearnObject;
};


#endif //plLearnSparseHistogram_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
