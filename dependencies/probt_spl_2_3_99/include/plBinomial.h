
/*=============================================================================
 * Product        : ProBT 
 * File           : plBinomial.h
 * Author         : Kamel Mekhnacha
 * Creation       : Jan 2006
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


#ifndef plBinomial_h
#define plBinomial_h

#include "plConfig.h"

#include <plDistribution.h>
#include <plSymbol.h>

/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/

/** This class implements the binomial distribution.
    The binomial distribution is used when there are exactly two mutually
    exclusive outcomes of a trial. These outcomes are appropriately labeled
    @em success and @em failure. The binomial distribution is used to obtain the
    probability of observing @em x successes in @em N trials, with the probability of
    success on a single trial denoted by @em p. The binomial distribution assumes
    that @em p is fixed for all trials.\n
	
    The binomial probability distribution has the following formula (for x = 0, 1, ..., N):\n
    \f[ P(x; p, N) = \frac{ N!~p^x~(1-p)^{N-x} }{x!~(N-x)! } \f]
*/
class PL_DLL_API plBinomial : public plDistribution
{
public:

    /** Default constructor used for serialization. */
    plBinomial() 
      : plDistribution()
  { 
    signature_[PLBinomial] = true; 
  }


  /** Constructs a plBinomial distribution on the domain of the @em variable.
      The parameter @em p is the probability of @em success (the probability that
      the single event happens).\n The parameter @em N is the number of
      @em trials and is just the maximal value of the one-dimensional variable
      @em variable taking its value in 0...N.\n
	  
      The binomial probability distribution has the following formula (for x = 0, 1, ..., N):\n
      \f[ P(x; p, N) = \frac{ N!~p^x~(1-p)^{N-x} }{x!~(N-x)! } \f]
	  
      The parameter @em N in the formula above is extracted from the variable @em variable 
      definition (the maximal value of @em variable).
  */
  plBinomial(const  plSymbol &variable, plProbValue p);
  
  /** Destroys the distribution */
  virtual ~plBinomial(){};
  
  /** Get the parameter used for constructing this object. */
  plProbValue get_p() const;

  /** Promote from a plDistribution. If the object being copied is
      actually of the wrong type, an exception is raised.
  */
  explicit plBinomial(const plDistribution&);

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
