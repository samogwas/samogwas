/*=============================================================================
 * Product        : ProBT 
 * File           : plCndDistribution.h
 * Author         : Juan-Manuel Ahuactzin
 * Creation       : Wed Mar 21 14:08:40 2001
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

#ifndef plCndDistribution_h
#define plCndDistribution_h

#include "plConfig.h"


#include <plMath.h>
#include <plValues.h>
#include <plVariablesConjunction.h>
#include <plDistribution.h>
#include <plJointDistribution.h>
#include <plDataValues.h>


/*****************************************************************************
 *                           External Classes
 *****************************************************************************/
class kplVariable;
class kplConditionalKernel;

/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/

//SPL_DECL2T(plMutable);

SPL_DECL1(plCndDistribution);

/**  plCndDistribution is the base class of all conditional probability (and density) distributions
     like P(A | B).*/
class PL_DLL_API plCndDistribution : public plComputableObject
{
 protected:
  // Friend classes and functions
  friend class plJointDistribution;
  friend class plComputableObject;

 public:

  /** Empty constructor. The resulting object is invalid for most operations,
      except for being assigned to, or used as a return value. */
  plCndDistribution();

  /** Constructor using the left and right variables and also
      the signature.
  */
  plCndDistribution(const plVariablesConjunction& left,
                    const plVariablesConjunction& right,
                    const plComputableObjectType &sig);

  /** Constructs a conditional distribution from another conditional distribution */
  plCndDistribution(const plCndDistribution &cnd_distribution);
  
   
  /** Constructs from a plComputableObject. The object transmitted as parameter
      has to actually point to a conditional distribution, else a plError is
      raised. */
  explicit plCndDistribution(const plComputableObject &);
  
  /** Creates a conditional distribution as a product of a conditional and non
      conditional distributions. Throws an exception if the result of the
      product is not actually a conditional distribution.

      The left variables of the constructed computable object are the concatenation
      (in the same order) of the left variables of all the terms of the list.
      Its right variables are the concatenation of the right variables of
      all the terms and that are not in left one above.
  */
  explicit plCndDistribution(const plComputableObjectList &fun_list);

  /** Creates a conditional distribution as a product of a conditional and non
      conditional distributions. Throws an exception if the result of the
      product is not actually a conditional distribution.  This constructor
      explicits the left and right variables of the constructed distribution.
  */
  plCndDistribution(const plComputableObjectList &func_lis,
                    const plVariablesConjunction &left_variables,
                    const plVariablesConjunction &right_variables);

  /** Construct a plCndDistribution with an array of plProbValues
      implicitly defining a set of plProbTables. ATTENTION: This
      constructor is reserved for discrete/discretized variables. 
      \sa plDistributionTable
  */
  plCndDistribution(const plVariablesConjunction& left,
                    const plVariablesConjunction& right,
                    const plProbValue *table,
                    bool already_normalized = false);

  /** Construct a plCndDistribution with a vector of plProbValues
      implicitly defining a set of plProbTables. ATTENTION: This
      constructor is reserved for discrete variables. 
      \sa plDistributionTable
  */
  plCndDistribution(const plVariablesConjunction& left,
                    const plVariablesConjunction& right,
                    const std::vector<plProbValue> &table,
                    bool already_normalized = false);

  /** Construct a plCndDistribution containing random probability tables. */
  plCndDistribution(const plVariablesConjunction& left,
                    const plVariablesConjunction& right);
  
  /** Destroys the conditional distribution. */
  virtual ~plCndDistribution();



  /**  Produces a new conditional distribution by instantiating a subset of the known
       variables given by a the plValues @em values.*/
  void partial_instantiate(plCndDistribution& distribution_to_instantiate,
			   const plVariablesConjunction& variables_to_instantiate,
			   const plValues& values)const;
  
  
  /**  Produces a new conditional distribution by instantiating a subset of the known
       variables given by a the plValues @em values.
       ie P(A | B C).partial_instantiate(C, c) yields P(A | B c)
       Note that this method returns a freshly created object. If 
       you are calling it in a loop, consider using the version that
       modifies an existing object instead.
  */
  plCndDistribution partial_instantiate(const plVariablesConjunction& variables_to_instantiate,
                                        const plValues& values)const;



  /**  Produces a new distribution by instantiating the known
       variables given by a the plValues @em values.
       If @em ensure_normalization_on_compute is true, the obtained Distribution 
       will return normalized values on calling the compute() method.
       If @em ensure_normalization_on_compute is false (the default is true), the instantiation will
       be faster.
  */
  void instantiate(plDistribution& distribution_to_instantiate, 
		   const plValues& values, bool ensure_normalization_on_compute=true)const;
  /**  Produces a new distribution by instantiating the known
       variables given by a the plValues @em values.
       If @em ensure_normalization_on_compute is true, the obtained Distribution 
       will return normalized values on calling the compute() method.
       If @em ensure_normalization_on_compute is false (the default is true), the instantiation will
       be faster.\n
       
       Using this method instead of the one that modifies its first argument will considerably
       slow down the following calls to plDistribution::compile(). If speed (as opposed to ease of use)
       is one of your concerns, use the version that modifies an existing object instead.
  */
  plDistribution instantiate(const plValues& values, bool ensure_normalization_on_compute = true) const;


  /**  Produces a new distribution by instantiating the known variables using the
       values given in a vector of values. 
       If @em ensure_normalization_on_compute is true, the obtained Distribution 
       will return normalized values on calling the compute() method.
       If @em ensure_normalization_on_compute is false (the default is true), the instantiation will
       be faster. */
  void instantiate(plDistribution& distribution_to_instantiate, const int *parameter, 
                   bool ensure_normalization_on_compute=true) const;
  /**  Produces a new distribution by instantiating the known variables using the
       values given in a vector of values. 
       If @em ensure_normalization_on_compute is true, the obtained Distribution 
       will return normalized values on calling the compute() method.
       If @em ensure_normalization_on_compute is false (the default is true), the instantiation will
       be faster. */
  plDistribution instantiate(const int *parameter, bool ensure_normalization_on_compute=true) const;


  void instantiate(plDistribution& distribution_to_instantiate, const unsigned int *parameter, 
                   bool ensure_normalization_on_compute=true) const;
  plDistribution instantiate(const unsigned int *parameter, bool ensure_normalization_on_compute=true) const;


  /**  Produces a new distribution by instantiating the known variables using the
       values given in a vector of values. 
       If @em ensure_normalization_on_compute is true, the obtained Distribution 
       will return normalized values on calling the compute() method.
       If @em ensure_normalization_on_compute is false (the default is true), the instantiation will
       be faster. */
  void instantiate(plDistribution& distribution_to_instantiate, const float *parameter, 
                   bool ensure_normalization_on_compute=true) const;
  plDistribution instantiate(const float *parameter, 
                             bool ensure_normalization_on_compute=true) const;



  /**  Produces a new distribution by instantiating the known variables using the
       values given in a vector of values. 
       If @em ensure_normalization_on_compute is true, the obtained Distribution 
       will return normalized values on calling the compute() method.
       If @em ensure_normalization_on_compute is false (the default is true), the instantiation will
       be faster. */
  void instantiate(plDistribution& distribution_to_instantiate, const double *parameter, 
                   bool ensure_normalization_on_compute=true) const;
  plDistribution instantiate(const double *parameter, bool ensure_normalization_on_compute=true) const;


  /**  Produces a new distribution by instantiating the known variables using the
       values given in an STL vector.  
       If @em ensure_normalization_on_compute is true, the obtained Distribution 
       will return normalized values on calling the compute() method.
       If @em ensure_normalization_on_compute is false (the default is true), the instantiation will
       be faster. */
  void instantiate(plDistribution& distribution_to_instantiate,  
                   const std::vector <int> &parameter, 
                   bool ensure_normalization_on_compute=true) const;
  plDistribution instantiate( const std::vector<int>& parameter, 
                              bool ensure_normalization_on_compute=true)const;

  /**  Produces a new distribution by instantiating the known variables using the
       values given in an STL vector.  
       If @em ensure_normalization_on_compute is true, the obtained Distribution 
       will return normalized values on calling the compute() method.
       If @em ensure_normalization_on_compute is false (the default is true), the instantiation will
       be faster. */
  void instantiate(plDistribution& distribution_to_instantiate,  
                   const std::vector <unsigned int> &parameter, 
                   bool ensure_normalization_on_compute=true) const;
  plDistribution instantiate( const std::vector<unsigned int>& parameter, 
                              bool ensure_normalization_on_compute=true)const;
  
  /**  Produces a new distribution by instantiating the known variables using the
       values given in an STL vector.  
       If @em ensure_normalization_on_compute is true, the obtained Distribution 
       will return normalized values on calling the compute() method.
       If @em ensure_normalization_on_compute is false (the default is true), the instantiation will
       be faster. */
  void instantiate(plDistribution& distribution_to_instantiate,  
                   const std::vector <float> &parameter, 
                   bool ensure_normalization_on_compute=true) const;
  plDistribution instantiate( const std::vector<float>& parameter, 
                              bool ensure_normalization_on_compute=true)const;

  /**  Produces a new distribution by instantiating the known variables using the
       values given in an STL vector.  
       If @em ensure_normalization_on_compute is true, the obtained Distribution 
       will return normalized values on calling the compute() method.
       If @em ensure_normalization_on_compute is false (the default is true), the instantiation will
       be faster. 
  */
  void instantiate(plDistribution& distribution_to_instantiate,  
                   const std::vector <double> &parameter, 
                   bool ensure_normalization_on_compute=true) const;
  plDistribution instantiate( const std::vector<double>& parameter, 
                              bool ensure_normalization_on_compute=true)const;




  /**  Produce a new distribution by instantiating the known variable using the value
       of a single value.  
       If @em ensure_normalization_on_compute is true, the obtained Distribution 
       will return normalized values on calling the compute() method.
       If @em ensure_normalization_on_compute is false (the default is true), the instantiation will
       be faster. */
  void instantiate(plDistribution& distribution_to_instantiate, int parameter, 
                   bool ensure_normalization_on_compute=true) const;
  /**  Produces a new distribution by instantiating the known
       variables given by a T (single value).
       If @em ensure_normalization_on_compute is true, the obtained Distribution 
       will return normalized values on calling the compute() method.
       If @em ensure_normalization_on_compute is false (the default is true), the instantiation will
       be faster.\n

       Using this method instead of the one that modifies its first argument will considerably
       slow down the following calls to plDistribution::compile(). If speed (as opposed to ease of use)
       is one of your concerns, use the version that modifies an existing object instead.\n

       This is the fonctionnal version of the method above. It permits the
       following syntax :
       pL_R.instantiate( special_Right_values ).compute( special_Left_values ) 
       which is the PL programming form of 
       P([L = special_Left_values]|[R = special_Right_values])
  */
  plDistribution instantiate( int parameter,
                              bool ensure_normalization_on_compute=true) const;


  void instantiate(plDistribution& distribution_to_instantiate, unsigned int parameter, 
                   bool ensure_normalization_on_compute=true) const;
  plDistribution instantiate( unsigned int parameter,
                              bool ensure_normalization_on_compute=true) const;

  /**  Produce a new distribution by instantiating the known variable using the value
       of a single value.  
       If @em ensure_normalization_on_compute is true, the obtained Distribution 
       will return normalized values on calling the compute() method.
       If @em ensure_normalization_on_compute is false (the default is true), the instantiation will
       be faster. */
  void instantiate(plDistribution& distribution_to_instantiate, float parameter, 
                   bool ensure_normalization_on_compute=true) const;
  /**  Produces a new distribution by instantiating the known
       variables given by a T (single value).
       If @em ensure_normalization_on_compute is true, the obtained Distribution 
       will return normalized values on calling the compute() method.
       If @em ensure_normalization_on_compute is false (the default is true), the instantiation will
       be faster.\n

       Using this method instead of the one that modifies its first argument will considerably
       slow down the following calls to plDistribution::compile(). If speed (as opposed to ease of use)
       is one of your concerns, use the version that modifies an existing object instead.\n

       This is the fonctionnal version of the method above. It permits the
       following syntax :
       pL_R.instantiate( special_Right_values ).compute( special_Left_values ) 
       which is the PL programming form of 
       P([L = special_Left_values]|[R = special_Right_values])
  */
  plDistribution instantiate( float parameter,
                              bool ensure_normalization_on_compute=true) const;

  
  /**  Produce a new distribution by instantiating the known variable using the value
       of a single value.  
       If @em ensure_normalization_on_compute is true, the obtained Distribution 
       will return normalized values on calling the compute() method.
       If @em ensure_normalization_on_compute is false (the default is true), the instantiation will
       be faster. */
  void instantiate(plDistribution& distribution_to_instantiate, double parameter, 
                   bool ensure_normalization_on_compute=true) const;
  /**  Produces a new distribution by instantiating the known
       variables given by a T (single value).
       If @em ensure_normalization_on_compute is true, the obtained Distribution 
       will return normalized values on calling the compute() method.
       If @em ensure_normalization_on_compute is false (the default is true), the instantiation will
       be faster.\n

       Using this method instead of the one that modifies its first argument will considerably
       slow down the following calls to plDistribution::compile(). If speed (as opposed to ease of use)
       is one of your concerns, use the version that modifies an existing object instead.\n

       This is the fonctionnal version of the method above. It permits the
       following syntax :
       pL_R.instantiate( special_Right_values ).compute( special_Left_values ) 
       which is the PL programming form of 
       P([L = special_Left_values]|[R = special_Right_values])
  */
  plDistribution instantiate( double parameter,
                              bool ensure_normalization_on_compute=true) const;


  /**  Produce a new distribution by instantiating the known variable using the value
       of a single value.  
       If @em ensure_normalization_on_compute is true, the obtained Distribution 
       will return normalized values on calling the compute() method.
       If @em ensure_normalization_on_compute is false (the default is true), the instantiation will
       be faster. */
  void instantiate(plDistribution& distribution_to_instantiate, 
                   const std::string &parameter, 
                   bool ensure_normalization_on_compute=true) const;
  plDistribution instantiate(const std::string &parameter, 
                             bool ensure_normalization_on_compute=true) const;
  
  void instantiate(plDistribution& distribution_to_instantiate,  
                   const char* parameter, 
                   bool ensure_normalization_on_compute=true) const;
  plDistribution instantiate(const char*parameter,
                             bool ensure_normalization_on_compute=true) const;



  /** Compiles the conditional distribution P(left_vars | right_vars) by
      constructing a DistributionTable of ProbTable.  The index variables of
      the DistributionTable are the discrete or discretized variables @em right_vars.
      For each value of @em right_vars, a ProbTable is
      constructed by exhaustively compiling the corresponding distribution
      (i.e. by visiting all points of the discrete or discretized @em left_vars
      space.
  */
  void compile(plCndDistribution& result) const;
  
  /** Compiles the conditional distribution P(left_vars | right_vars) by
      constructing a DistributionTable of ProbTable.  The index variables of
      the DistributionTable are the discrete or discretized variables @em right_vars.
      For each value of @em right_vars, a ProbTable is
      constructed by exhaustively compiling the corresponding distribution
      (i.e. by visiting all points of the discrete or discretized
      @em left_vars space.
  */
  void compile_low_memory_use(plCndDistribution& result)const;
  
  /** Compiles the conditional distribution P(left_vars | right_vars) by
      constructing a DistributionTable of ProbTable.  The index variables of
      the DistributionTable are the discrete or discretized variables
      @em right_vars. For each value of @em right_vars, a ProbTable is
      constructed by exhaustively compiling the corresponding distribution
      (i.e. by visiting all points of the discrete or discretized
      @em left_vars space.
      Note that this method returns a freshly created object. If 
      you are calling it in a loop, consider using the version that
      modifies an existing object instead.
  */
  plCndDistribution compile() const;

  /** Compiles the conditional distribution P(left_vars | right_vars) by constructing a DistributionTable.
      The index variables of the DistributionTable are the discrete or discretized variables @em right_vars. 
      For each value of @em right_vars, a compiled distribution of type @em compiled_distrib_type 
      is constructed by exhaustively compiling the corresponding distribution 
      (i.e. by visiting all points of the discrete or discretized @em left_vars space.) 
  */
  void compile(plCndDistribution& result,
  	       plCompiledDistributionType compiled_distrib_type)const;

  /** Compiles the conditional distribution P(left_vars | right_vars) by constructing a DistributionTable.
      The index variables of the DistributionTable are the discrete or discretized variables @em right_vars. 
      For each value of @em right_vars, a compiled distribution of type @em compiled_distrib_type 
      is constructed by exhaustively compiling the corresponding distribution 
      (i.e. by visiting all points of the discrete or discretized @em left_vars space.) 
      Note that this method returns a freshly created object. If 
      you are calling it in a loop, consider using the version that
      modifies an existing object instead.
  */
  plCndDistribution compile(plCompiledDistributionType compiled_distrib_type)const;


  /** Compiles the conditional distribution P(left_vars | right_vars) by
      constructing a DistributionTable.  The index variables of the
      DistributionTable are the discrete or discretized variables \em
      right_vars. For each value of @em right_vars, the
      corresponding distribution is compiled by sampling it using a set of
      @em n points in the variables space The used points may be
      generated using 4 possible methods (according to the value of
      @em generator_type): \n 1) Using all points of the discrete or
      discretized variables space (PL_EXHAUSTIVE_GENERATOR) \n 2)
      Generating, for @em time_in_seconds seconds, a set of points
      at random from the variables space (PL_RANDOM_GENERATOR) \n 3)
      Generating, for @em time_in_seconds seconds, a set of points
      using a Monte Carlo generator from the variables space
      (PL_MC_GENERATOR).\n 4) Generating, for @em time_in_seconds
      seconds, a set of points using a Genetic Algorithm generator
      from the variables space (PL_GA_GENERATOR).\n
      If @em generator_type is PL_CHOOSE_GENERATOR_FOR_ME (default value), 
      ProBT will choose the optimal generator for you.

      The result of the compilation is stored, according to the value of @em compiled_distrib_type, as: \n
      1) a table (PL_TABLE). \n
      2) an MRBT (PL_MRBT). \n
      3) a map (PL_MAP) for non-null values.\n
      
      If @em compiled_distrib_type is PL_CHOOSE_COMP_TYPE_FOR_ME (default value), 
      ProBT will choose the optimal data structure for you.
  */
  void n_compile(plCndDistribution& result,
  		 unsigned int n_iterations,
  		 plGeneratorType generator_type = PL_CHOOSE_GENERATOR_FOR_ME,
  		 plCompiledDistributionType compiled_distrib_type = PL_CHOOSE_COMP_TYPE_FOR_ME) const;
  

  /** Compiles the conditional distribution P(left_vars | right_vars) by
      constructing a DistributionTable.  The index variables of the
      DistributionTable are the discrete or discretized variables \em
      right_vars. For each value of @em right_vars, the
      corresponding distribution is compiled by sampling it using a set of
      points in the variables space for @em time_in_seconds. The used
      points may be generated using 4 possible methods (according to
      the value of @em generator_type): \n 1) Using all points of the
      discrete or discretized variables space
      (PL_EXHAUSTIVE_GENERATOR) \n 2) Generating, for \em
      time_in_seconds seconds, a set of points at random from the
      variables space (PL_RANDOM_GENERATOR) \n 3) Generating, for \em
      time_in_seconds seconds, a set of points using a Monte Carlo
      generator from the variables space (PL_MC_GENERATOR).\n 4)
      Generating, for @em time_in_seconds seconds, a set of points
      using a Genetic Algorithm generator from the variables space
      (PL_GA_GENERATOR).\n
      If @em generator_type is PL_CHOOSE_GENERATOR_FOR_ME (default value), 
      ProBT will choose the optimal generator for you.
      
      The result of the compilation is stored, according to the value of @em compiled_distrib_type, as: \n
      1) a table (PL_TABLE). \n
      2) an MRBT (PL_MRBT). \n
      3) a map (PL_MAP) for non-null values.\n
      
      If @em compiled_distrib_type is PL_CHOOSE_COMP_TYPE_FOR_ME (default value), 
      ProBT will choose the optimal data structure for you.
  */
  void time_compile(plCndDistribution& result,
  		    double time_in_seconds,
  		    plGeneratorType generator_type = PL_CHOOSE_GENERATOR_FOR_ME, 
  		    plCompiledDistributionType compiled_distrib_type = PL_CHOOSE_COMP_TYPE_FOR_ME) const;


  /**
     Does a time_compile(), and returns the compiled distribution. See time_compile() above
     for details.

     Note that this method returns a freshly created object. If 
     you are calling it in a loop, consider using the version that
     modifies an existing object instead.
  */
  plCndDistribution time_compile(double time_in_seconds,
                                 plGeneratorType generator_type = PL_CHOOSE_GENERATOR_FOR_ME, 
                                 plCompiledDistributionType distrib_type = PL_CHOOSE_COMP_TYPE_FOR_ME) const;

  static plCndDistribution as_builtin(const plCndDistribution &ck);

  
  using plComputableObject::compute;
  using plComputableObject::compute_log;
  
  /** Computes the value of the function for the input parameters
      @em  left_params and @em right_params.
  */
  plProbValue compute(const plValues &left_params,
                      const plValues &right_params)const;
  
  /** Computes the logarithm value of the function for the input parameters
      @em  left_params and @em right_params.
  */
  plFloat compute_log(const plValues &left_params,
                      const plValues &right_params)const;
  
  /** Computes the value of the function for the input parameters
      @em  left_params and @em right_params.
  */
  plProbValue compute(const int *left_params,
                      const int *right_params)const;

  /** Computes the value of the function for the input parameters
      @em  left_params and @em right_params.
  */
  plProbValue compute(const unsigned int *left_params,
                      const unsigned int *right_params)const;

  /** Computes the value of the function for the input parameters
      @em  left_params and @em right_params.
  */
  plProbValue compute(const float *left_params,
                      const float *right_params)const;

  /** Computes the value of the function for the input parameters
      @em  left_params and @em right_params.
  */
  plProbValue compute(const double *left_params,
                      const double *right_params)const;

  /** Computes the logarithm value of the function for the array input parameters
      @em  left_params and @em right_params.
  */
  plFloat compute_log(const int *left_params,
                      const int *right_params)const;

  /** Computes the logarithm value of the function for the array input parameters
      @em  left_params and @em right_params.
  */
  plFloat compute_log(const unsigned int *left_params,
                      const unsigned int *right_params)const;

  /** Computes the logarithm value of the function for the array input parameters
      @em  left_params and @em right_params.
  */
  plFloat compute_log(const float *left_params,
                      const float *right_params)const;

  /** Computes the logarithm value of the function for the array input parameters
      @em  left_params and @em right_params.
  */
  plFloat compute_log(const double *left_params,
                      const double *right_params)const;


  /** Computes the value of the function for the one-dimensional input parameters
      @em left_param and @em right_param.
  */
  plProbValue compute(int left_param, int right_param)const;

  /** Computes the value of the function for the one-dimensional input parameters
      @em left_param and @em right_param.
  */
  plProbValue compute(unsigned int left_param, unsigned int right_param)const;

  /** Computes the value of the function for the one-dimensional input parameters
      @em left_param and @em right_param.
  */
  plProbValue compute(float left_param, float right_param)const;

  /** Computes the value of the function for the one-dimensional input parameters
      @em left_param and @em right_param.
  */
  plProbValue compute(double left_param, double right_param)const;


  /** Computes the logarithm value of the function for the one-dimensional input parameters
      @em left_param and @em right_param.
  */
  plFloat compute_log(int left_param, int right_param)const;

  /** Computes the logarithm value of the function for the one-dimensional input parameters
      @em left_param and @em right_param.
  */
  plFloat compute_log(unsigned int left_param, unsigned int right_param)const;

  /** Computes the logarithm value of the function for the one-dimensional input parameters
      @em left_param and @em right_param.
  */
  plFloat compute_log(float left_param, float right_param)const;

  /** Computes the logarithm value of the function for the one-dimensional input parameters
      @em left_param and @em right_param.
  */
  plFloat compute_log(double left_param, double right_param)const;

  /** Computes the value of the function for the STL vector input @em  left_params
      and @em right_params.
  */
  plProbValue compute(const std::vector <int> &left_params,
                      const std::vector <int> &right_params)const;

  /** Computes the value of the function for the STL vector input @em  left_params
      and @em right_params.
  */
  plProbValue compute(const std::vector <unsigned int> &left_params,
                      const std::vector <unsigned int> &right_params)const;

  /** Computes the value of the function for the STL vector input @em  left_params
      and @em right_params.
  */
  plProbValue compute(const std::vector <float> &left_params,
                      const std::vector <float> &right_params)const;

  /** Computes the value of the function for the STL vector input @em  left_params
      and @em right_params.
  */
  plProbValue compute(const std::vector <double> &left_params,
                      const std::vector <double> &right_params)const;


  /** Computes the value of the function for the string input @em  left_params
      and @em right_params.
  */
  plProbValue compute(const std::string &left_param,
                     const std::string &right_param)const;

  /** Computes the value of the function for the string input @em  left_params
      and @em right_params.
  */
  plProbValue compute(const char *left_param,
                     const char *right_param)const;

  /** Computes the logarithm value of the function for the STL vector input @em  left_params
      and @em right_params.
  */
  plFloat compute_log(const std::vector <int> &left_params,
                      const std::vector <int> &right_params)const;

  /** Computes the logarithm value of the function for the STL vector input @em  left_params
      and @em right_params.
  */
  plFloat compute_log(const std::vector <unsigned int> &left_params,
                      const std::vector <unsigned int> &right_params)const;

  /** Computes the logarithm value of the function for the STL vector input @em  left_params
      and @em right_params.
  */
  plFloat compute_log(const std::vector <float> &left_params,
                      const std::vector <float> &right_params)const;

  /** Computes the logarithm value of the function for the STL vector input @em  left_params
      and @em right_params.
  */
  plFloat compute_log(const std::vector <double> &left_params,
                      const std::vector <double> &right_params)const;


  /** Computes the logarithm value of the function for the string input @em  left_params
      and @em right_params.
  */
  plFloat compute_log(const std::string &left_param,
                      const std::string &right_param)const;

  /** Computes the logarithm value of the function for the string input @em  left_params
      and @em right_params.
  */
  plFloat compute_log(const char *left_param,
                      const char *right_param)const;
  

 private:

  plProbValue kernel_compute(const plDataValues &left_param,
                             const plDataValues &right_param)const;

  plFloat kernel_compute_log(const plDataValues &left_param,
                             const plDataValues &right_param)const;

  void init(const plVariablesConjunction& left,
            const plVariablesConjunction& right,
            const plProbValue *table,
            bool already_normalized);


  template<typename T> 
    plDistribution instantiate_impl_fresh( T parameter,
                                           bool ensure_normalization_on_compute) const;

  template<typename T>
    void instantiate_template_all(plDistribution& distribution_to_instantiate,
                                  const T &values,
                                  bool ensure_normalization_on_compute) const;

  template<typename T>
    void instantiate_impl_vector(plDistribution& distribution_to_instantiate, 
                                 const std::vector <T> &parameter,
                                 bool ensure_normalization_on_compute)const;
 
  template<typename T>
    void instantiate_impl_single(plDistribution& distribution_to_instantiate, T Value,
                                 bool ensure_normalization_on_compute) const;


  template <typename T>
    plProbValue compute_impl_ptr(T *left_params,
                                 T *right_params)const;
 

  template <typename T>
    plFloat compute_log_impl_ptr(T *left_params,
                                 T *right_params)const;

  template <typename T>
    plProbValue compute_impl_single(T left_param, T right_param)const;
   

  template <typename T>
    plFloat compute_log_impl_single(T left_param, T right_param)const;


  template <typename T>
    plProbValue compute_impl_vector(const std::vector <T> &left_params,
                                    const std::vector <T> &right_params)const;

  template <typename T>
    plFloat compute_log_impl_vector(const std::vector <T> &left_params,
                                    const std::vector <T> &right_params)const;


  template<typename T>
    plProbValue compute_on_array(T left_params, T right_params) const;
  
  template<typename T>
    plFloat compute_log_on_array(T left_params, T right_params) const;

  template<typename T> 
    void copy_left_right(T left_params, T right_params) const;

  unsigned int label_2_index(const std::string &label, bool leftp, const std::string &function) const;

};

#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
