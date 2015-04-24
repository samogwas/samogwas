/*=============================================================================
 * Product        : ProBT 
 * File           : plBayesLearnProbTable.h
 * Author         : David Raulo
 * Creation       : Wed Dec 13 16:17:14 2006
 *
 *=============================================================================
 *        (c) Copyright 2006, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */

#ifndef plBayesLearnProbTable_h
#define plBayesLearnProbTable_h

#include "plConfig.h"

#include <plNonCndLearnObject.h>
#include <plDirichlet.h>
#include <plProbTable.h>

#include <plVariableIndexer.h>


SPL_DECL1(plBayesLearnProbTable);

//=============================================================================
/** This class allows to make a Bayesian estimation of multinomial (ProbTable)
    distributions using Multinomial-Dirichlet conjugacy.  To take advantage of
    the conjugacy property, the prior distribution is supposed to be a
    Dirichlet distribution and the a posteriori distribution is a Dirichlet
    distribution too.
*/
class PL_DLL_API plBayesLearnProbTable: public plBayesLearnObject
{
public:
  /** Type of the variable (multi-dim). */
  typedef plVariablesConjunction VarType;
  
  /** Prepare to make a Bayesian estimation of a multinomial (ProbTable)
      distribution on variables @em vars.

      The @em init_alpha array gives the initial values for the parameters of
      the Dirichlet prior used to estimate the multinomial (ProbTable). It
      represents the initial @em virtual numbers of observations for each
      possible value.

      The a posteriori estimate is: \f[ p_i = \frac {init\_alpha_i + n_i}
      {\sum_i init\_alpha_i + n_i}.\f] where \f$ n_i \f$ is the number of
      observations (in the data) of the ith value.

      Beware the dimension of @em init_alpha! See comments about Constructor2
      of plProbTable.

      \sa plLearnLaplace
      \sa plLearnLidstone
  */
  plBayesLearnProbTable(const plVariablesConjunction &vars, 
			const std::vector<plFloat>& init_alpha);
  


  /** Default constructor */
  plBayesLearnProbTable();
  
  /** Destructor */
  ~plBayesLearnProbTable();
    
  /** Resets learning. */
  void reset();
 
  /** Returns the 'alpha' parameters. */
  const std::vector<plFloat>& get_alpha()const 
  { 
    if(all_vars_.is_empty())
      throw plError(75, "when calling 'plBayesLearnPoisson::get_alpha");

    return alpha_;
  }

  /** Returns the a posteriori Dirichlet distribution. 
      The \em params_variable dimension should be equal to this distribution cardinality.
  */
  plDistribution get_aposteriori_distribution(const plVariablesConjunction &params_variable) const;
  
  /** Returns the learnt ProbTable distribution. */
  plDistribution get_distribution() const;


  /** Returns the probability table corresponding to the learnt distribution in
      the output parameter 'table'. Supposes that 'table' has the exact
      size. 
  */
  template<typename T>
  void get_probability(T *table)const
  {
    if(all_vars_.is_empty())
      throw plError(75, "when calling 'plBayesLearnProbTable::get_probability'");
    
    plProbValue sum = PL_ZERO_PROB;
    for(unsigned int i = 0; i < alpha_.size(); ++i) sum += alpha_[i]; 
    
    if(sum > PL_ZERO_PROB) { 
      for(unsigned int i = 0; i < alpha_.size(); ++i) table[i] = alpha_[i]/sum;
    }
    else {
      for(unsigned int i = 0; i < alpha_.size(); ++i) table[i] = PL_ZERO_PROB;
    }
  }


  /** Returns the probability table corresponding to the learnt distribution in
      the output parameter 'table'. 
  */
  template<typename T>
  void get_probability(std::vector <T> &table)const
  {
    if(!alpha_.empty()){
      table.resize(alpha_.size());
      get_probability(&(table[0]));
    }
    else
      throw plError(75, "when calling 'plBayesLearnProbTable::get_probability'");
  }

  void get_parameters(plValues &params) const
  {
    std::vector <plProbValue> table;
    get_probability(table);
    
    for(unsigned int i = 0; i < table.size(); ++i){
      params[i] = plFloat(table[i]);
    }
  }

  unsigned int get_n_parameters()const
  {
    if(all_vars_.is_empty())
      throw plError(75, "when calling 'plBayesLearnProbTable::get_n_parameters");

    return alpha_.size()-1;
  }

  virtual plLearnObject* clone()const
  {
    return new plBayesLearnProbTable(*this);
  }


protected:

  bool internal_addPoint(const plDataValues &point, plFloat weight);
 
  explicit plBayesLearnProbTable(const plVariablesConjunction &vars);

  virtual void Output(std::ostream &out) const;
    
protected:  
  SPL_ACCESS1(plBayesLearnProbTable);

  plVariableIndexer indexer_;
  std::vector<plFloat> init_alpha_;
  std::vector<plFloat> alpha_;

  template <class T> friend class plCndLearnObject;
};

//=============================================================================
#endif //plBayesLearnProbTable_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
