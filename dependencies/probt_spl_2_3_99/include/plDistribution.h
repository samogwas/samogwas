/*=============================================================================
 * Product        : ProBT 
 * File           : plDistribution.h
 * Author         : Juan-Manuel Ahuactzin
 * Creation       : Fri Mar 16 16:22:42 2001
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

#ifndef plDistribution_h
#define plDistribution_h

#include "plConfig.h"


#include <plComputableObject.h>
#include <plMath.h>
#include <plDataValues.h>
#include <plVariablesConjunction.h>
#include <plValues.h>
#include <plTechniquesTypes.h>
#include <plPlotData.h>

/*****************************************************************************
 *                           External Classes
 *****************************************************************************/
class kplVariable;
class kplKernel;
class kplComputableObject;
class plComputableObjectList;

/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/


SPL_DECL1(plDistribution);

//=============================================================================
/**  The \em plDistribution class is the base class of all non-conditional
     probability (and density) distributions. */
class PL_DLL_API plDistribution : public plComputableObject
{
 protected:

  // Friend classes and functions
  friend class plDistributionTable;
  friend class kplCGaIndividual;
  friend class plBayesianFilter;
  friend class plJointDistribution;
  friend class plCndDistribution;
  friend class plComputableObject;
  friend class plLearnSparseHistogram;

 public:

  /** Empty constructor. The resulting object is invalid for most operations,
      except for being assigned to, or used as a return value. */
  plDistribution();
  
  /** Constructor with a set of variables and the signature */
  plDistribution(const plVariablesConjunction& variables,
                 const plComputableObjectType &sig);

  /** Constructs a Distribution from another Distribution */
  plDistribution(const plDistribution &);
  
  /** Constructs from a plComputableObject. The object transmitted as parameter
      has to actually point to a non-conditional distribution, else a plError is
      raised. */
  explicit plDistribution(const plComputableObject &);
  
  /** Construct from a plCndDistribution that has an empty list of known
      variables. */
  explicit plDistribution(const plCndDistribution&);

  /** Creates a non-conditional distribution as a product of a conditional and
      non conditional distributions. Throws an exception if the result of the
      product is not actually a non-conditional distribution.
      The variables of the constructed distribution are the concatenation
      (in the same order) of the left variables of all the terms of the list.
  */
  explicit plDistribution(const plComputableObjectList &);

  /** Creates a non-conditional distribution as a product of a conditional and
      non conditional distributions. Throws an exception if the result of the
      product is not actually a non-conditional distribution.  This constructor
      explicits the variables of the constructed distribution.
  */
  plDistribution(const plComputableObjectList &fun_list,
                 const plVariablesConjunction &variables);

  /** Construct a plDistribution with an array of plProbValues
      implicitly defining a set of plProbTable. ATTENTION: This
      constructor is reserved for discrete/discretized variables. 
      \sa plProbTable
  */
  plDistribution(const plVariablesConjunction& variables,
                 const plProbValue *table,
                 bool already_normalized = false);
  
  /** Construct a variables with a vector of plProbValues
      implicitly defining a set of plProbTable. ATTENTION: This
      constructor is reserved for discrete variables. 
      \sa plProbTable
  */
  plDistribution(const plVariablesConjunction& variables,
                 const std::vector<plProbValue> &table,
                 bool already_normalized = false);

  /** Construct a plDistribution (actually a plProbTable) 
      containing random probability table.
      \sa plProbTable
  */
  explicit plDistribution(const plVariablesConjunction& variables);


  /** Destroys the Distribution*/
  virtual ~plDistribution();

  /**  Draw a set of function variables values, according to the function
       distribution and depose them in a plValues. */
  void draw(plValues &res) const;

  /**  Draw a set of function variables values, according to the function
       distribution and depose them in a plValues.  Note that this method
       returns a freshly created object. If you are calling it in a loop,
       consider using the version that modifies an existing object instead. */
  plValues draw() const;

  
  /**  Draw a set of function variables values, according to the function
       distribution and depose them in an array.  
       Implemented for int, unsigned int, float, and double types. */
  void draw(int *resuls) const;
  void draw(unsigned int *resuls) const;
  void draw(float *resuls) const;
  void draw(double *resuls) const;
  void draw(long double *resuls) const;

  /**  Draw a set of function variables values, according to the function
       distribution and depose them in the parameter.
       Attention mainly used with one variable distributions. For two or more
       variables distributions the first variable value will be copied at \em
       parameter while the others will be ignored.  Implemented for int,
       unsigned int, float, and double types. */
  void draw(int &result)const;
  void draw(unsigned int &result)const;
  void draw(float &result)const;
  void draw(double &result)const;
  void draw(long double &result)const;

  /**  Draw a set of function variables values, according to the function
       distribution and depose them in an STL vector.
       Implemented for int, unsigned int, float, and double types. */
  void draw( std::vector <int> &result)const;
  void draw( std::vector <unsigned int> &result)const;
  void draw( std::vector <float> &result)const;
  void draw( std::vector <double> &result)const;
  void draw( std::vector <long double> &result)const;

  /** Optimize the function and returns the function variables values that
      executes that optimization in a plValues.*/
  void best(plValues &res)const;

  /** Optimize the function and returns the function variables values that
      executes that optimization in a plValues.  Note that this method returns
      a freshly created object. If you are calling it in a loop, consider using
      the version that modifies an existing object instead.
  */
  plValues best()const;


  /** Optimize the function and returns the function variables values that
      executes that optimization in a array. 
      The size of \em parameter must be at least the number of parameters.
      Implemented for int, unsigned int, float, and double types. */
  void best(int *result)const;
  void best(unsigned int *result)const;
  void best(float *result)const;
  void best(double *result)const;
  void best(long double *result)const;

  
  
  /** Optimize the function and returns the function variables values that
      executes that optimization in a scalar variable. This function is mainly used
      with one variable distributions. For two or more variables distributions the first
      variable value will be copied at \em parameter while the others will be
      ignored.  Implemented for int, unsigned int, float, and double types. */
  void best(int &result)const;
  void best(unsigned int &result)const;
  void best(float &result)const;
  void best(double &result)const;
  void best(long double &result)const;
  
  /** Optimize the function and returns the function variables values that
      executes that optimization in an STL vector.
      Implemented for int, unsigned int, float, and double types.
  */
  void best( std::vector <int> &result)const;
  void best( std::vector <unsigned int> &result)const;
  void best( std::vector <float> &result)const;
  void best( std::vector <double> &result)const;
  void best( std::vector <long double> &result)const;


  /** Optimizes the function by running an optimization algorithm for \em
      time_in_seconds seconds and returns the function variables values that
      executes that optimization in an array. The size of \em parameter must
      be at least the number of parameters.  Implemented for int, unsigned int,
      float, and double types. */
  void time_best(int *result, double time_in_seconds)const;
  void time_best(unsigned int *result, double time_in_seconds)const;
  void time_best(float *result, double time_in_seconds)const;
  void time_best(double *result, double time_in_seconds)const;
  void time_best(long double *result, double time_in_seconds)const;
  
  /** Optimizes the function by running an optimization algorithm for \em
      time_in_seconds seconds and returns the function variables values that
      executes that optimization in a scalar variable.  Attention mainly used
      with one variable distributions. For two or more variables distributions the first
      variable value will be copied at \em parameter while the others will be
      ignored.  Implemented for int, unsigned int, float, and double types. */
  void time_best(int &result, double time_in_seconds)const;
  void time_best(unsigned int &result, double time_in_seconds)const;
  void time_best(float &result, double time_in_seconds)const;
  void time_best(double &result, double time_in_seconds)const;
  void time_best(long double &result, double time_in_seconds)const;
  
  
  /** Optimizes the function by running an optimization algorithm for \em
      time_in_seconds seconds and returns the function variables values that
      executes that optimization in an STL vector Attention mainly used with
      one variable distributions. For two or more variables distributions the first
      variable value will be copied at \em parameter while the others will be
      ignored.  Implemented for int, unsigned int, float, and double types. */
  void time_best( std::vector <int> &result, double time_in_seconds)const;
  void time_best( std::vector <unsigned int> &result, double time_in_seconds)const;
  void time_best( std::vector <float> &result, double time_in_seconds)const;
  void time_best( std::vector <double> &result, double time_in_seconds)const;
  void time_best( std::vector <long double> &result, double time_in_seconds)const;

  
  /** Compiles the distribution using an approximation, hopefully faster than
      the plain compile(). This approximation is performed by sampling the
      distribution. This function uses the number of sampling points as stop
      criterion.

      @param[out] result The resulting compiled distribution
      @param[in] n The number of sample points to be used
      @param[in] generator_type The method to be used for generating the
      sample points. The allowed values are:  \n
      1) PL_EXHAUSTIVE_GENERATOR: Use all points of the discrete or discretized
      variables space (exhaustive exploration) \n
      2) PL_RANDOM_GENERATOR: Uniform random exploration of the variable space. \n
      3) PL_MC_GENERATOR: Random (non uniform) exploration of the variable
      space by drawing according to the sampled distribution.\n
      4) PL_GA_GENERATOR: Exploration of the variable space using a genetic
      algorithm.\n
      5) PL_CHOOSE_GENERATOR_FOR_ME: Let ProBT choose the most appropriate
      generation method (default value).
      @param[in] compiled_distrib_type The internal data structure to be used
      for storing the resulting distribution. The allowed values are: \n
      1) PL_TABLE: a probability table. \n
      2) PL_MRBT: a Multi-Resolution Binary Tree representation. \n
      3) PL_MAP: a map for non-null values. \n
      4) PL_CHOOSE_COMP_TYPE_FOR_ME: Let ProBT choose the most appropriate
      structure (default value).\n

      \sa time_compile()
  */
  void n_compile(plDistribution& result,
		 unsigned int n,
		 plGeneratorType generator_type = PL_CHOOSE_GENERATOR_FOR_ME,
		 plCompiledDistributionType compiled_distrib_type 
                 = PL_CHOOSE_COMP_TYPE_FOR_ME) const;


  /** Compiles the distribution using an approximation, hopefully faster than
      the plain compile(). This approximation is performed by sampling the
      distribution. This function uses the running time as stop criterion.

      @param[out] result The resulting compiled distribution 
      @param[in] time_in_seconds The time (in seconds) to be spent in
      generating sample points.
      @param[in] generator_type The method to be used for generating the
      sample points. The allowed values are:  \n
      1) PL_EXHAUSTIVE_GENERATOR: Use all points of the discrete or discretized
      variables space (exhaustive exploration) \n
      2) PL_RANDOM_GENERATOR: Uniform random exploration of the variable space. \n
      3) PL_MC_GENERATOR: Random (non uniform) exploration of the variable
      space by drawing according to the sampled distribution.\n
      4) PL_GA_GENERATOR: Exploration of the variable space using a genetic
      algorithm.\n
      5) PL_CHOOSE_GENERATOR_FOR_ME: Let ProBT choose the most appropriate
      generation method (default value).
      @param[in] compiled_distrib_type The internal data structure to be used
      for storing the resulting distribution. The allowed values are: \n
      1) PL_TABLE: a probability table. \n
      2) PL_MRBT: a Multi-Resolution Binary Tree representation. \n
      3) PL_MAP: a map for non-null values. \n
      4) PL_CHOOSE_COMP_TYPE_FOR_ME: Let ProBT choose the most appropriate
      structure (default value).\n

      \sa n_compile()
  */
  void time_compile(plDistribution& result,
		    double time_in_seconds,
		    plGeneratorType generator_type 
                    = PL_CHOOSE_GENERATOR_FOR_ME, 
		    plCompiledDistributionType compiled_distrib_type 
                    = PL_CHOOSE_COMP_TYPE_FOR_ME)const;

  /**
     Does a time_compile(), and returns the compiled distribution. See time_compile()
     above for details.
     Note that this method returns a freshly created object. If you are calling
     it in a loop, consider using the version that modifies an existing object
     instead.
  */
  plDistribution time_compile(double time_in_seconds,
                              plGeneratorType generator_type 
                              = PL_CHOOSE_GENERATOR_FOR_ME, 
                              plCompiledDistributionType compliled_distrib_type 
                              = PL_CHOOSE_COMP_TYPE_FOR_ME)const;


  /** Compiles the distribution using an exhaustive generator
      (PL_EXHAUSTIVE_GENERATOR) (i.e. by generating all points of the discrete
      or discretized variables space) and stores the result as a table
      (PL_TABLE) */
  void compile(plDistribution& result)const;

  /** Compiles the distribution using an exhaustive generator
      (PL_EXHAUSTIVE_GENERATOR) (i.e. by generating all points of the discrete
      or discretized variables space) and stores the result as a table
      (PL_TABLE).  Note that this method returns a freshly created object. If
      you are calling it in a loop, consider using the version that modifies an
      existing object instead.
  */
  plDistribution compile()const;


  /** Compiles the distribution using an exhaustive generator
      (PL_EXHAUSTIVE_GENERATOR) (i.e. by generating all points of the discrete
      or discretized variables space) and stores the result as a table
      (PL_TABLE) */
  void compile_low_memory_use(plDistribution& result)const;

  
  /** Compiles the distribution using an exhaustive generator
      (PL_EXHAUSTIVE_GENERATOR) (i.e. by generating all points of the discrete
      or discretized variables space) and stores the result as a \em
      compiled_distrib_type */
  void compile(plDistribution& result,
	       plCompiledDistributionType compiled_distrib_type)const;

  
  /** Like n_compile() with the possibility to incrementally compile the
      distribution.  It assumes to be called on the same result distribution "result" to
      update it. It also assumed that this result distribution is empty for the
      first call. The first call is equivalent to a call to n_compile(). The
      parameters "generatorType" and "distrib_type" are taken into account only
      in the first call.
  */
  void incremental_n_compile(plDistribution& result,
			     unsigned int n_iterations,
			     plGeneratorType generatorType 
                             = PL_CHOOSE_GENERATOR_FOR_ME,
			     plCompiledDistributionType distrib_type 
                             = PL_CHOOSE_COMP_TYPE_FOR_ME)const;
  
  /** Like time_compile() with the possibility to incrementally compile the
      distribution.  It assumes to be called on the same result distribution "result" to
      update it. It also assumed that this result distribution is empty for the
      first call.  The first call is equivalent to a call to time_compile(). The
      parameters "generatorType" and "distrib_type" are taken into account only
      in the first call.
  */
  void incremental_time_compile(plDistribution& result,
				double time_in_seconds,
				plGeneratorType generatorType 
                                = PL_CHOOSE_GENERATOR_FOR_ME,
				plCompiledDistributionType distrib_type 
                                = PL_CHOOSE_COMP_TYPE_FOR_ME)const;

  
  using plComputableObject::tabulate;
  /** Tabulates the distribution in "out" stream. Each of the possible values of the
      distribution variables set are printed together with the result of
      "compute". Function not allowed for non-discretized continuous
      variables.\n If the parameter \em print_on_zero is set to \em false,
      cases having O as probability value are not written */
  void tabulate(std::ostream& out = std::cout, bool print_on_zero=true)const;
  

  /** Tabulates the distribution in "output" plProbValue vector and fills the
      "values" with the corresponding variable values.  The value P(X) is
      computed for each possible value of the distribution variables and put in the
      output vector "output".  Function not allowed for distributions with
      non-discretized continuous variables.\n Same as the method above for
      multi-dimensional cases*/
  void tabulate(std::vector <plValues> &values, std::vector <plProbValue> &probabilities)const;

  /** Tabulates the distribution in "output" vector <pair <plDataValues, plProbValue>
      >. The The value P(X) is computed for each possible value of the distribution
      variables and put, sorted by probability in the output map
      "output". Function not allowed for Distributions with non-discretized
      continuous variables.*/
  void sorted_tabulate( std::vector <std::pair <plValues, plProbValue> >& output )const;

  /** Generates a set of gnuplot instructions allowing to plot this
      distribution. This function is allowed for 1D and 2D distributions only.
      
      \param[in] file_name The name of the gnuplot instructions file to be generated
      \param[in] plottype  The 'terminal' type to be used by gnuplot (see plPlotType definition)
      \param[in] n_samples The 'samples' parameter to be used by gnuplot (only used for continuous distributions)

      ATTENTION: The function does not call the 'gnuplot' command directly. You
      have to call the 'gnuplot' command using the generated file in order to
      get the plot (on your screen or in a image file according to the value of
      the parameter 'plottype').
  */
  void plot(const std::string &file_name, plPlotType plottype = PL_DEFAULT_PLOT,
            int n_samples = 100)const;

  void plot(const char *file_name, plPlotType plottype = PL_DEFAULT_PLOT,
            int n_samples = 100)const;

  /** Optimizes the function by running an optimization algorithm for \em
      time_in_seconds seconds and returns the function variables values that
      executes that optimization in a plValues.*/
  void time_best(plValues &res, double time_in_seconds)const;

  /** Optimizes the function. Note that this method returns a freshly created
      object. If you are calling it in a loop, consider using the version that
      modifies an existing object instead.
  */
  plValues time_best(double time_in_seconds) const;
  


  /** Optimizes the function by running an optimization algorithm for \em
      ns iterations and returns the function variables values that
      executes that optimization in a plValues.*/
  void n_best(plValues &res, unsigned int n)const;

  /** Optimizes the function. Note that this method returns a freshly created
      object. If you are calling it in a loop, consider using the version that
      modifies an existing object instead.
  */
  plValues n_best(unsigned int n) const;
  
  /** Optimizes the function by running an optimization algorithm for \em
      n iterations and returns the function variables values that
      executes that optimization in an array. The size of \em parameter must
      be at least the number of parameters.  Implemented for int, unsigned int,
      float, and double types. */
  void n_best(int *result, unsigned int n)const;
  void n_best(unsigned int *result, unsigned int n)const;
  void n_best(float *result, unsigned int n)const;
  void n_best(double *result, unsigned int n)const;
  void n_best(long double *result, unsigned int n)const;
  
  /** Optimizes the function by running an optimization algorithm for \em
      n iterations and returns the function variables values that
      executes that optimization in a scalar variable.  Attention mainly used
      with one variable distributions. For two or more variables distributions the first
      variable value will be copied at \em parameter while the others will be
      ignored.  Implemented for int, unsigned int, float, and double types. */
  void n_best(int &result, unsigned int n)const;
  void n_best(unsigned int &result, unsigned int n)const;
  void n_best(float &result, unsigned int n)const;
  void n_best(double &result, unsigned int n)const;
  void n_best(long double &result, unsigned int n)const;
 
  
  /** Optimizes the function by running an optimization algorithm for \em
      n iterations and returns the function variables values that
      executes that optimization in an STL vector Attention mainly used with
      one variable distributions. For two or more variables distributions the first
      variable value will be copied at \em parameter while the others will be
      ignored.  Implemented for int, unsigned int, float, and double types. */

  void n_best( std::vector <int> &parameter, unsigned int n)const;
  void n_best( std::vector <unsigned int> &parameter, unsigned int n)const;
  void n_best( std::vector <float> &parameter, unsigned int n)const;
  void n_best( std::vector <double> &parameter, unsigned int n)const;
  void n_best( std::vector <long double> &parameter, unsigned int n)const;
  

  /**  Draw a set of function variables values, according to the function
       distribution and depose them in a plValues.  Parameters "initial_state",
       "proposal_standard_deviation", and "n" are only used when drawing
       non-compiled distribution (i.e. using Metropolis algorithm). They represent:

       - initial_state: the initial point from which the Metropolis sampler
       will start.\n

       - proposal_standard_deviation: The standard deviations of the normals
       that will be used when generating Metropolis candidate points. \n

       - n: the number of steps that the Metropolis sampler will run before
       returning a drawn point.

       - probability_log: the log-probability (or density value) of the returned
       point. */
  void draw(plValues &res, const plValues &initial_state,
            const std::vector <plFloat> &proposal_standard_deviation,
            unsigned int n, plFloat &probability_log)const;

  /**  Draw a set of function variables values, according to the function
       distribution and depose them in a scalar array.  Parameters
       "initial_state", "proposal_standard_deviation", and "n" are only used
       when drawing non-compiled distribution (i.e. using Metropolis algorithm). They
       represent:

       - initial_state: the initial point from which the Metropolis sampler
       will start.\n

       - proposal_standard_deviation: The standard deviations of the normals
       that will be used when generating Metropolis candidate points. \n

       - n: the number of steps that the Metropolis sampler will run before
       returning a drawn point.

       - probability_log: the log-probability (or density value) of the returned point.
       Implemented for int, unsigned int, float, and double types. */
  void draw(int *parameter, const plValues &initial_state,
            const std::vector <plFloat>  &proposal_standard_deviation, 
            unsigned int n, plFloat &probability_log)const;
  void draw(unsigned int *parameter, const plValues &initial_state,
            const std::vector <plFloat>  &proposal_standard_deviation, 
            unsigned int n, plFloat &probability_log)const;
  void draw(float *parameter, const plValues &initial_state,
            const std::vector <plFloat>  &proposal_standard_deviation, 
            unsigned int n, plFloat &probability_log)const;
  void draw(double *parameter, const plValues &initial_state,
            const std::vector <plFloat>  &proposal_standard_deviation, 
            unsigned int n, plFloat &probability_log)const;
  void draw(long double *parameter, const plValues &initial_state,
            const std::vector <plFloat>  &proposal_standard_deviation, 
            unsigned int n, plFloat &probability_log)const;

  /**  Draw a set of function variables values, according to the function
       distribution and depose them in a scalar variable. Attention mainly used
       with one variable distributions. For two or more variables distributions the first
       variable value will be copied at \em parameter while the others will
       be ignored.  Parameters "initial_state", "proposal_standard_deviation",
       and "n" are only used when drawing non-compiled distribution (i.e. using
       Metropolis algorithm). They represent:

       - initial_state: the initial point from which the Metropolis sampler
       will start.\n

       - proposal_standard_deviation: The standard deviations of the normals
       that will be used when generating Metropolis candidate points. \n

       - n: the number of steps that the Metropolis sampler will run before
       returning a drawn point.

       - probability_log: the log-probability (or density value) of the returned point.
       Implemented for int, unsigned int, float, and double types. */
  void draw(int &parameter, const plValues &initial_state,
            const std::vector <plFloat>  &proposal_standard_deviation,
            unsigned int n, plFloat &probability_log)const;
  void draw(unsigned int &parameter, const plValues &initial_state,
            const std::vector <plFloat>  &proposal_standard_deviation,
            unsigned int n, plFloat &probability_log)const;
  void draw(float &parameter, const plValues &initial_state,
            const std::vector <plFloat>  &proposal_standard_deviation,
            unsigned int n, plFloat &probability_log)const;
  void draw(double &parameter, const plValues &initial_state,
            const std::vector <plFloat>  &proposal_standard_deviation,
            unsigned int n, plFloat &probability_log)const;
  void draw(long double &parameter, const plValues &initial_state,
            const std::vector <plFloat>  &proposal_standard_deviation,
            unsigned int n, plFloat &probability_log)const;

  /**  Draw a set of function variables values, according to the function
       distribution and depose them in an STL scalar vector.  Parameters
       "initial_state", "proposal_standard_deviation", and "n" are only used
       when drawing non-compiled distribution (i.e. using Metropolis algorithm). They
       represent:

       - initial_state: the initial point from which the Metropolis sampler
       will start.\n

       - proposal_standard_deviation: The standard deviations of the normals
       that will be used when generating Metropolis candidate points. \n

       - n: the number of steps that the Metropolis sampler will run before
       returning a drawn point.

       - probability_log: the log-probability (or density value) of the returned point.
       Implemented for int, unsigned int, float, and double types. */
  void draw( std::vector <int> &parameter, const plValues &initial_state,
             const std::vector <plFloat> &proposal_standard_deviation,
             unsigned int n, plFloat &probability_log)const;
  void draw( std::vector <unsigned int> &parameter, const plValues &initial_state,
             const std::vector <plFloat> &proposal_standard_deviation,
             unsigned int n, plFloat &probability_log)const;
  void draw( std::vector <float> &parameter, const plValues &initial_state,
             const std::vector <plFloat> &proposal_standard_deviation,
             unsigned int n, plFloat &probability_log)const;
  void draw( std::vector <double> &parameter, const plValues &initial_state,
             const std::vector <plFloat> &proposal_standard_deviation,
             unsigned int n, plFloat &probability_log)const;
  void draw( std::vector <long double> &parameter, const plValues &initial_state,
             const std::vector <plFloat> &proposal_standard_deviation,
             unsigned int n, plFloat &probability_log)const;


  void best(plValues &res, const plValues &initial_state, 
            const  std::vector <plFloat> &proposal_standard_deviation,
            unsigned int n, plFloat &probability_log)const;

  /** Implemented for int, unsigned int, float, and double types. */
  void best(int *parameter, const plValues &initial_state,
            const std::vector <plFloat> &proposal_standard_deviation,
            unsigned int n, plFloat &probability_log)const;
  void best(unsigned int *parameter, const plValues &initial_state,
            const std::vector <plFloat> &proposal_standard_deviation,
            unsigned int n, plFloat &probability_log)const;
  void best(float *parameter, const plValues &initial_state,
            const std::vector <plFloat> &proposal_standard_deviation,
            unsigned int n, plFloat &probability_log)const;
  void best(double *parameter, const plValues &initial_state,
            const std::vector <plFloat> &proposal_standard_deviation,
            unsigned int n, plFloat &probability_log)const;
  void best(long double *parameter, const plValues &initial_state,
            const std::vector <plFloat> &proposal_standard_deviation,
            unsigned int n, plFloat &probability_log)const;

  /** Implemented for int, unsigned int, long int, float, double and long
      double value-types. */
  void best(int &parameter, 
            const plValues &initial_state,
            const std::vector <plFloat> &proposal_standard_deviation,
            unsigned int n, plFloat &probability_log)const;
  void best(unsigned int &parameter, 
            const plValues &initial_state,
            const std::vector <plFloat> &proposal_standard_deviation,
            unsigned int n, plFloat &probability_log)const;
  void best(float &parameter, 
            const plValues &initial_state,
            const std::vector <plFloat> &proposal_standard_deviation,
            unsigned int n, plFloat &probability_log)const;
  void best(double &parameter, 
            const plValues &initial_state,
            const std::vector <plFloat> &proposal_standard_deviation,
            unsigned int n, plFloat &probability_log)const;
  void best(long double &parameter, 
            const plValues &initial_state,
            const std::vector <plFloat> &proposal_standard_deviation,
            unsigned int n, plFloat &probability_log)const;

  /** Implemented for int, unsigned int, float, and double types. */
  void best( std::vector <int> &parameter, 
             const plValues &initial_state,
             const std::vector <plFloat> &proposal_standard_deviation,
             unsigned int n, plFloat &probability_log)const;
  void best( std::vector <unsigned int> &parameter, 
             const plValues &initial_state,
             const std::vector <plFloat> &proposal_standard_deviation,
             unsigned int n, plFloat &probability_log)const;
  void best( std::vector <float> &parameter, 
             const plValues &initial_state,
             const std::vector <plFloat> &proposal_standard_deviation,
             unsigned int n, plFloat &probability_log)const;
  void best( std::vector <double> &parameter, 
             const plValues &initial_state,
             const std::vector <plFloat> &proposal_standard_deviation,
             unsigned int n, plFloat &probability_log)const;
  void best( std::vector <long double> &parameter, 
             const plValues &initial_state,
             const std::vector <plFloat> &proposal_standard_deviation,
             unsigned int n, plFloat &probability_log)const;
  
  /** Compute Shannon's entropy of the distribution */
  plFloat compute_shannon_entropy() const;
  
  /** Compute the expectation of the distribution.
      
      ATTENTION: as plValues are typed (integer, float,...), using this function could
      truncate the result. For example, the following code will truncate the mean value of
      \em PX by returning 50 instead of 50.5:
      @code
      
      plSymbol X("X", plIntegerType(0, 100) );
      plNormal PX(X, 50.5, 2.0);
      plValues X_val(X);
      PX.compute_expectation(X_val);
      std::cout << X_val << std::endl;
      @endcode
  */
  void compute_expectation(plValues & res) const;
  
  /** Compute the expectation of the distribution. */
  void compute_expectation(std::vector<plFloat> & res) const;

  /** Computes the Kullback-Leibler divergence from this distribution to distribution q,
      using a natural logarithm.
      The two distributions must have the same cardinality.

      The divergence from p to q Dkl(p,q) is a measure of dissimilarity
      between two distributions.
      It has the following properties:
      - non-negative
      - null if and only if p = q
      - infinity iff there exists x such that p(X=x)!=0 and q(X=x)==0
      - NOT symmetric
      - does NOT satisfy triangle inequality.
  **/
  plFloat kullback_leibler_divergence_to(plDistribution const & q) const;
  

  /** Return 'true' if the distribution has all its probability values to zero. */
  bool is_null() const;

  static plDistribution as_builtin(const plDistribution &k);


  using plComputableObject::compute;

  using plComputableObject::compute_log;

  /** Computes the value of the function for the one-dimensional input parameter
      @em value.  ATTENTION: the returned value is guaranteed to be
      NORMALIZED for and only for: 
      - builtin distributions (basics
      distributions provided by ProBT), 
      - compiled distributions (obtained by using the @em compile method).
      - distributions obtained with @em instantiate called with parameter 
      @em ensure_normalization_on_compute = @em true.
      
      Calling the @em compute method on distributions resulting from the ask
      and/or instantiate methods is not guaranteed to return a
      normalized value unless the instantiate method is called with parameter 
      @em ensure_normalization_on_compute = @em true. To be sure to obtain normalized values,
      consider compiling these distributions using the @em compile method.
  */
  plProbValue compute(int value)const;

  /** Computes the value of the function for the one-dimensional input parameter
      @em value.  ATTENTION: the returned value is guaranteed to be
      NORMALIZED for and only for: 
      - builtin distributions (basics
      distributions provided by ProBT), 
      - compiled distributions (obtained by using the @em compile method).
      - distributions obtained with @em instantiate called with parameter 
      @em ensure_normalization_on_compute = @em true.
      
      Calling the @em compute method on distributions resulting from the ask
      and/or instantiate methods is not guaranteed to return a
      normalized value unless the instantiate method is called with parameter 
      @em ensure_normalization_on_compute = @em true. To be sure to obtain normalized values,
      consider compiling these distributions using the @em compile method.
  */
  plProbValue compute(unsigned int value)const;

  /** Computes the value of the function for the one-dimensional input parameter
      @em value.  ATTENTION: the returned value is guaranteed to be
      NORMALIZED for and only for: 
      - builtin distributions (basics
      distributions provided by ProBT), 
      - compiled distributions (obtained by using the @em compile method).
      - distributions obtained with @em instantiate called with parameter 
      @em ensure_normalization_on_compute = @em true.
      
      Calling the @em compute method on distributions resulting from the ask
      and/or instantiate methods is not guaranteed to return a
      normalized value unless the instantiate method is called with parameter 
      @em ensure_normalization_on_compute = @em true. To be sure to obtain normalized values,
      consider compiling these distributions using the @em compile method.
  */
  plProbValue compute(float value)const;

  /** Computes the value of the function for the one-dimensional input parameter
      @em value.  ATTENTION: the returned value is guaranteed to be
      NORMALIZED for and only for: 
      - builtin distributions (basics
      distributions provided by ProBT), 
      - compiled distributions (obtained by using the @em compile method).
      - distributions obtained with @em instantiate called with parameter 
      @em ensure_normalization_on_compute = @em true.
      
      Calling the @em compute method on distributions resulting from the ask
      and/or instantiate methods is not guaranteed to return a
      normalized value unless the instantiate method is called with parameter 
      @em ensure_normalization_on_compute = @em true. To be sure to obtain normalized values,
      consider compiling these distributions using the @em compile method.
  */
  plProbValue compute(double value)const;

  plProbValue compute(long double value)const;

  /** Computes the value of the function for the one-dimensional input parameter
      @em value.  ATTENTION: the returned value is guaranteed to be
      NORMALIZED for and only for: 
      - builtin distributions (basics
      distributions provided by ProBT), 
      - compiled distributions (obtained by using the @em compile method).
      - distributions obtained with @em instantiate called with parameter 
      @em ensure_normalization_on_compute = @em true.
      
      Calling the @em compute method on distributions resulting from the ask
      and/or instantiate methods is not guaranteed to return a
      normalized value unless the instantiate method is called with parameter 
      @em ensure_normalization_on_compute = @em true. To be sure to obtain normalized values,
      consider compiling these distributions using the @em compile method.
  */
  plProbValue compute(const std::string &value)const;

  /** Computes the logarithm value of the function for the input parameter
      @em values.
  */
  plFloat compute_log(int value)const;

  /** Computes the logarithm value of the function for the input parameter
      @em values.
  */
  plFloat compute_log(unsigned int value)const;

  /** Computes the logarithm value of the function for the input parameter
      @em values.
  */
  plFloat compute_log(float value)const;

  /** Computes the logarithm value of the function for the input parameter
      @em values.
  */
  plFloat compute_log(double value)const;

  plFloat compute_log(long double value)const;

  plFloat compute_log(const std::string &value)const;


  /** Set a soft evidence on the expression. 
      Has no effect on non-expression distributions
  */
  void set_soft_evidence(const plDistribution &soft_evidence);

  /** Set a set of soft evidence on the expression. 
      Has no effect on non-expression distributions
  */
  void set_soft_evidences(const plComputableObjectList &soft_evidences);

  /** Remove soft evidence on a given variable */
  void remove_soft_evidence(const plSymbol &variable);
  
  /** Remove all soft evidences */
  void clear_soft_evidences();

 private:
  template<typename T>
    void incremental_compile(plDistribution& result,
                             T param,
                             plGeneratorType generatorType,
                             plCompiledDistributionType distrib_type)const;

  template<typename T>
    void compile_approx(plDistribution& result,
                        T param,
                        plGeneratorType generatorType,
                        plCompiledDistributionType distrib_type)const;

  void fill_result_from_head_values(plValues &res, const std::string &message)const;


  template <typename T>
    plProbValue compute_single_impl(T value)const;

  template <typename T>
    plFloat compute_log_single_impl(T value)const;

  unsigned int label_2_index(const std::string &label, const std::string &function) const;

  template <typename T>
    void draw_ptr_impl(T *parameter)const;

  template <typename T>
    void draw_single_impl(T &parameter)const;

  template <typename T>
    void draw_vector_impl( std::vector <T> &parameter)const;

  template <typename T>
    void best_ptr_impl(T *parameter)const;

  template <typename T>
    void best_single_impl(T &parameter)const;

  template <typename T>
    void best_vector_impl( std::vector <T> &parameter)const;

  template <typename T>
    void time_best_ptr_impl(T *parameter, double time_in_seconds)const;

  template <typename T>
    void time_best_single_impl(T &parameter, double time_in_seconds)const;

  template <typename T>
    void time_best_vector_impl( std::vector <T> &parameter, double time_in_seconds)const;

  template <typename T>
    void n_best_ptr_impl(T *parameter, unsigned int n)const;

  template <typename T>
    void n_best_single_impl(T &parameter, unsigned int n)const;

  template <typename T>
    void n_best_vector_impl( std::vector <T> &parameter, unsigned int n)const;

  template <typename T>
    void draw_ptr_impl(T *parameter, const plValues &initial_state,
                       const std::vector <plFloat>  &proposal_standard_deviation, 
                       unsigned int n, plFloat &probability_log)const;

  template <typename T>
    void draw_single_impl(T &parameter, const plValues &initial_state,
                          const std::vector <plFloat>  &proposal_standard_deviation,
                          unsigned int n, plFloat &probability_log)const;

  template <typename T>
    void draw_vector_impl( std::vector <T> &parameter, const plValues &initial_state,
                           const std::vector <plFloat> &proposal_standard_deviation,
                           unsigned int n, plFloat &probability_log)const;

  template <typename T>
    void best_ptr_impl(T *parameter, const plValues &initial_state,
                       const std::vector <plFloat> &proposal_standard_deviation,
                       unsigned int n, plFloat &probability_log)const;

  template <typename T>
    void best_single_impl(T &parameter, 
                          const plValues &initial_state,
                          const std::vector <plFloat> &proposal_standard_deviation,
                          unsigned int n, plFloat &probability_log)const;

  template <typename T>
    void best_vector_impl( std::vector <T> &parameter, 
                           const plValues &initial_state,
                           const std::vector <plFloat> &proposal_standard_deviation,
                           unsigned int n, plFloat &probability_log)const;

};

#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
