/*=============================================================================
 * Product        : ProBT 
 * File           : plEMLearner.h
 * Author         : Kamel Mekhnacha
 * Creation       : Fri Mar  2 14:24:20 2007
 *
 *=============================================================================
 *        (c) Copyright 2006, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */


#ifndef plEMLearner_h
#define plEMLearner_h

#include "plConfig.h"

#include <plDLLExport.h>
#include <plVariablesConjunction.h>
#include <plDistribution.h>
#include <plCndDistribution.h>
#include <plSymbol.h>
#include <plDataDescriptor.h>
#include <plComputableObject.h>
#include <plLearnObject.h>

#include <cassert>
#include <iostream>
#include <limits>

class plLearnDistribVector;
class plJointDistribution;
class plValues;
class plJunctionTree;

/** This class implements a generic EM (Expectation-Maximization) learning
    algorithm (using incomplete data sets). There can be arbitrary missing data
    in the learning observations provided to the EM algorithm by a data
    descriptor.

    An example of estimating a Gaussian mixture using the EM algorithm
    with a BIC (Bayesian Information Criterion) score is as follows:
   
    @code
    #include <pl.h>

    #include <fstream>
    #include <iostream>

    // Output a given mixture
    //========================================================
    void output_mixture(const plJointDistribution &mixture)
    {
    std::cout << mixture.get_computable_object_list()[0] << std::endl;
    std::cout << mixture.get_computable_object_list()[1] << std::endl;
    }

    // Simulate and generate a 2-components (kernels) Gaussian mixture data. This data will be used for learning using EM based on a BIC score 
    //========================================================
    void generate_data(const std::string &file, 
    unsigned int ndata)
    {
    std::ofstream data(file.c_str());
    if(!data) {
    std::cerr << "failed to open '" << file << "' for output" << std::endl;
    exit(-1);
    }

    // Class latent variable (actual number of classes = 2)
    const unsigned int nc = 2;
    const plSymbol C("C", plIntegerType(0, nc-1));

    // X observed variable
    const plSymbol X("X", plRealType(-100.0, 100.0));

    // Actual (PC) table
    const plProbValue pr[] = {0.3, 0.7};
    const plProbTable PC(C, pr);

    // Actual P(X | C) distributions
    plDistributionTable PX(X, C);
    const plFloat mean[] = {-10.0, 10.0};
    const plFloat sd[] = {1.0, 3.0};
    for(unsigned int i = 0; i < nc; ++i) {
    PX.push( plNormal(X, mean[i], sd[i]), int(i));
    }
  
    // Constructing the model
    const plJointDistribution mixture(C^X, PC*PX);

    // Simulating data by drawing from P(X)
    data << "C;X" << std::endl;
    plValues val_CX(C^X);
    for(unsigned int i = 0; i < ndata; ++i) {
    mixture.draw(val_CX);
    data << ";" << val_CX[X] << std::endl;
    }

    data.close();

    std::cout << "Simulation mixture:" << std::endl;
    output_mixture(mixture);
    std::cout << "==================================" << std::endl;
    }

    // Run an EM for a given number nc of mixture components (kernels)
    //========================================================
    void run_em(const std::string &file, unsigned int nc,
    unsigned int &nparams, plFloat &llk,
    plFloat &bic, plJointDistribution &model)
    {
    const plSymbol C("C", plIntegerType(0, nc-1));
    const plSymbol X("X", plRealType(-100.0, 100.0));

    // EM Initial distribution on the class (kernel) variable: P(C)
    const bool random_prob = true;
    const plProbTable pc_init(C, random_prob);

    // EM Initial Gaussians : P(X | C)
    plDistributionTable px_init(X, C);
    for(unsigned int i = 0; i < nc; ++i) {
    px_init.push( plNormal(X, -10 + plRandomFloat(20.), 1.0), 
    int(i));
    }

    // P(C) is learnt as an histogram
    plLearnHistogram LC(C);
    // P(X | C) is learnt as a set of gaussians (a gaussian for each value of C)
    plCndLearnObject <plLearn1dNormal> LX(X, C);
  
    // Creating the EM learner instance
    plCSVFileDataDescriptor myCSVdata(file, C^X);
    std::vector <plLearnObject*> learn_objs(2); learn_objs[0] = &LC; learn_objs[1] = &LX;
    plEMLearner myEM(pc_init*px_init, learn_objs);

    // Run untill convergence
    myEM.run(myCSVdata, 0.0001);

    // Fill the output parameters
    nparams = myEM.get_n_parameters();
    llk = myEM.get_last_computed_loglikelihood();
    bic = llk - 0.5*nparams*std::log(myCSVdata.get_n_records());
    model = myEM.get_joint_distribution();
    }

    // Run an EM for each candidate number of components (kernels) 
    //========================================================
    void run_em(const std::string &file, 
    const std::vector<unsigned int> &n_mixture_candidates,
    std::vector<unsigned int> &nparams,
    std::vector<plFloat> &llk,
    std::vector<plFloat> &bic,
    std::vector<plJointDistribution> &model)
    {  
    nparams.resize( n_mixture_candidates.size() );
    llk.resize( n_mixture_candidates.size() );
    bic.resize( n_mixture_candidates.size() );
    model.resize( n_mixture_candidates.size() );

    for(unsigned int i = 0; i < n_mixture_candidates.size(); ++i) {
    const unsigned int nc = n_mixture_candidates[i];
    run_em(file, nc,
    nparams[i], llk[i], bic[i], model[i]);
    }
    }

    //========================================================
    int main()
    {
    /////////////////////////////////
    //  DATA SIMULATION/GENERATION
    /////////////////////////////////
 
    const std::string file = "gaussian_mixture_data.csv";
    const unsigned int ndata = 5000;
    generate_data(file, ndata);

    /////////////////////////////////
    //  EM-BIC BASED LEARNING
    /////////////////////////////////

    // Number of components (kernels) candidates
    std::vector<unsigned int> n_mixture_candidates;
    n_mixture_candidates.push_back(1); 
    n_mixture_candidates.push_back(2); 
    n_mixture_candidates.push_back(3);
    n_mixture_candidates.push_back(4);

    // Output parameters for each number of components (kernels) candidate
    std::vector<unsigned int> nparams; // The number of parameters
    std::vector<plFloat> llk; // The log-likelihood
    std::vector<plFloat> bic; // The bic score
    std::vector<plJointDistribution> model; // the learnt model
  
    // Run an EM for each number of components (kernels) candidate
    run_em(file, n_mixture_candidates, 
    nparams, llk, bic, model);
  

    /////////////////////////////////
    //  OUTPUT THE LEARNING RESULTS
    /////////////////////////////////

    // Output the estimate results of each number of components (kernels) candidate
    for(unsigned int i = 0; i < n_mixture_candidates.size(); ++i) {
    std::cout << "NC:\t"                     << n_mixture_candidates[i] << std::endl
    << "--BIC:\t"                  << bic[i]                  << std::endl
    << "--Log_likelihood:\t"       << llk[i]                  << std::endl
    << "--Number of parameters:\t" << nparams[i]              << std::endl;
    output_mixture(model[i]);
    std::cout << "==================================" << std::endl << std::endl;
    }
  
    // Get the model with the best BIC score
    unsigned int best_candidate_index = std::max_element(bic.begin(), bic.end()) - bic.begin();

    // Output the best BIC-based estimate
    std::cout << "\n\nBest NC:\t"            << n_mixture_candidates[best_candidate_index] << std::endl
    << "--Best BIC:\t"             << bic[best_candidate_index]                  << std::endl
    << "--Log_llk:\t"              << llk[best_candidate_index]                  << std::endl
    << "--Number of parameters:\t" << nparams[best_candidate_index]              << std::endl
    << std::endl;
    output_mixture(model[best_candidate_index]);

    return 0;
    }
    @endcode

    for which the output will be:
    @verbatim
    Simultation mixture:
    P(C) = 
    C		Probability
    0		0.3
    1		0.7

    P(X|C) = plDistributionTable {
    C = 0: 
    plNormal(X,	-10,	1)

    C = 1: 
    plNormal(X,	10,	3)


    }
    ==================================
    NC:	1
    --BIC:	-18359.7
    --Log_likelihood:	-18351.2
    --Number of parameters:	2
    P(C) = 
    C		Probability
    0		1

    P(X|C) = plCndDistribution {
    C = 0: 
    plNormal(X,	4.12168,	9.50011)


    }
    ==================================

    NC:	2
    --BIC:	-13973.8
    --Log_likelihood:	-13952.5
    --Number of parameters:	5
    P(C) = 
    C		Probability
    0		0.705
    1		0.295

    P(X|C) = plCndDistribution {
    C = 0: 
    plNormal(X,	10.0453,	2.93926)

    C = 1: 
    plNormal(X,	-10.0347,	1.01676)


    }
    ==================================

    NC:	3
    --BIC:	-13987.1
    --Log_likelihood:	-13953
    --Number of parameters:	8
    P(C) = 
    C		Probability
    0		0.295
    1		0.70297
    2		0.00203016

    P(X|C) = plCndDistribution {
    C = 0: 
    plNormal(X,	-10.0347,	1.01676)

    C = 1: 
    plNormal(X,	10.0678,	2.91296)

    C = 2: 
    plNormal(X,	2.23893,	0.90511)


    }
    ==================================

    NC:	4
    --BIC:	-14000.3
    --Log_likelihood:	-13953.5
    --Number of parameters:	11
    P(C) = 
    C		Probability
    0		0.295
    1		0.154698
    2		0.486083
    3		0.0642189

    P(X|C) = plCndDistribution {
    C = 0: 
    plNormal(X,	-10.0347,	1.01676)

    C = 1: 
    plNormal(X,	8.06023,	1.73133)

    C = 2: 
    plNormal(X,	11.2608,	2.40052)

    C = 3: 
    plNormal(X,	5.62642,	1.94988)


    }
    ==================================



    Best NC:	2
    --Best BIC:	-13973.8
    --Log_llk:	-13952.5
    --Number of parameters:	5

    P(C) = 
    C		Probability
    0		0.705
    1		0.295

    P(X|C) = plCndDistribution {
    C = 0: 
    plNormal(X,	10.0453,	2.93926)

    C = 1: 
    plNormal(X,	-10.0347,	1.01676)


    }
    @endverbatim

    \sa plDataDescriptor
    \sa plLearner::learn_model_parameters()
*/
class PL_DLL_API plEMLearner  : public plLearner
{
 public:

  /** Constructor.  

      @param[in] init_distribs initial distributions.
      @param[in] learn_objects a vector of learning
      objects allowing to provide the structure (dependencies), the
      parametrical form of each distribution, and the corresponding
      prior. The constructor clones the passed @em learn_objects and
      stores them internally (i.e. with no side-effect on the passed
      @em learn_objects).
  */
  plEMLearner(const plComputableObjectList &init_distribs,
	      const std::vector <plLearnObject*> &learn_objects);

  /** Constructor. Initial distributions are determined from the initial state
      of the \em learn_objects.

      @param[in] learn_objects a vector of learning objects
      allowing to provide the structure (dependencies), the parametrical form
      of each distribution, and the corresponding prior.  The constructor
      clones the passed @em learn_objects and stores them internally (i.e. with
      no side-effect on the passed @em learn_objects).

      ATTENTION: If the initial state of the \em learn_objects is
      uniform (it's the case for default constructed plLearnObject),
      the EM algorithm can fail to converge because it will stay at
      the initial non-informative state. So, consider using the constructor:
      @code
      plEMLearner(const plComputableObjectList &init_distribs, 
      const std::vector <plLearnObject*> &learn_objects);
      @endcode
      even with random initialized @em  init_distribs.
  */
  explicit plEMLearner(const std::vector <plLearnObject*> &learn_objects);

  /** Copy constructor */
  plEMLearner(const plEMLearner &other);

  /** Assignment */
  plEMLearner& operator=(const plEMLearner &other);

  /** Same as operator=() */
  plEMLearner& assign_from(const plEMLearner& other)
  {
    return operator=(other);
  }

  /** Deprecated constructor, kept temporarily for compatibility
      purposes. Identical in function to the above plEMLearner(const
      std::vector <plLearnObject*> &learn_objects).

      Initial distributions are determined from the initial state of
      the \em learn_objects.

      @param[in] data_descriptor_vars the data descriptor variables. It
      represents the order in which the data is arranged in a given row of the
      data descriptor used for learning. It should be a subset of all the
      variables found in the \em learn_objects learning objects.

      @param[in] learn_objects a vector of learning objects
      allowing to provide the structure (dependencies), the parametrical form
      of each distribution, and the corresponding prior. The constructor clones
      the passed @em learn_objects and stores them internally (i.e. with no
      side-effect on the passed @em learn_objects).

      ATTENTION: If the initial state of the \em learn_objects is
      uniform (it's the case for default constructed plLearnObject),
      the EM algorithm can fail to converge because it will stay at
      the initial non-informative state. So, consider using the constructor:
      @code
      plEMLearner(const plComputableObjectList &init_distribs, 
      const std::vector <plLearnObject*> &learn_objects);
      @endcode
      even with random initialized @em  init_distribs.
  */
  plEMLearner(const plVariablesConjunction &data_descriptor_vars,
	      const std::vector <plLearnObject*> &learn_objects);





  /** Deprecated constructor, kept temporarily for compatibility
      purposes. Identical in function to the above plEMLearner(const
      plComputableObjectList &init_distribs, const std::vector
      <plLearnObject*> &learn_objects).

      @param[in] data_descriptor_vars the data descriptor variables. It
      represents the order in which the data is arranged in a given row of the
      data descriptor used for learning. It should be a subset of all the
      variables found in the \em learn_objects learning objects. 

      @param[in] init_distribs initial distributions.

      @param[in] learn_objects a vector of learning objects
      allowing to provide the structure (dependencies), the parametrical form
      of each distribution, and the corresponding prior. The constructor clones
      the passed @em learn_objects and stores them internally (i.e. with no
      side-effect on the passed @em learn_objects).
  */
  plEMLearner(const plVariablesConjunction &data_descriptor_vars,
	      const plComputableObjectList &init_distribs,
	      const std::vector <plLearnObject*> &learn_objects);

  /** Default constructor.
      The resulting object is invalid for most operations, except for being
      assigned to.
  */
  plEMLearner();
  
  /** Destructor. */
  virtual ~plEMLearner();

  /** Resets the learner to its initial state */
  void reset();

 
  /** Run the EM algorithm for a given number of iterations. 

      @param[in] data_descriptor the data set on which the EM algorithm will
      iterate. It can contain arbitrary missing data.

      @param[in] nit the number of iterations for which the EM algorithm will
      be run.
  */
  void run(plDataDescriptor &data_descriptor, unsigned int nit);
  
  /** Run the EM algorithm for a given number of iterations.
      
      @param[in] data_descriptor the data set on which the EM
      algorithm will iterate.  It can contain arbitrary missing data.
      @param[in] nit the number of iterations for which the EM algorithm will
      be run.
  */
  void run(plDataDescriptor &data_descriptor, int nit)
    {
      assert(nit >= 0);
      run(data_descriptor,  (unsigned int)nit);
    }

  /** Run the EM algorithm until convergence:\n 
      | log-likelihood(t) - log-likelihood(t-1) | / | (log-likelihood(t) + log-likelihood(t-1)/2.0 | < convergence_loglikelihood_threshold.

      @param[in] data_descriptor the data set on which the EM
      algorithm will iterate.  It can contain arbitrary missing data.
      @param[in] convergence_loglikelihood_threshold convergence threshold corresponding to data
      log-likelihood change.
      @param[in] max_it the maximal number of iterations.


      \return the number of iterations needed to reach the convergence
  */
  unsigned int run(plDataDescriptor &data_descriptor,
                   double convergence_loglikelihood_threshold=0.0001, unsigned int max_it = std::numeric_limits<unsigned int>::max());

  /** Run the EM algorithm for a given number of iterations and return the last iteration info.
      
      @param[in] data_descriptor the data set on which the EM algorithm will
      iterate.  It can contain arbitrary missing data.
      @param[in] nit the number of iterations for which the EM algorithm will
      be run.
      @param[out] missing_most_probable_value the values with the highest
      probability for the missing variables.
      @param[out] missing_probability_table the current probability table on
      the missing variables.
  */
  void run(plDataDescriptor &data_descriptor, 
           unsigned int nit, 
           std::vector <plValues> &missing_most_probable_value,
           std::vector < std::vector <plProbValue> > &missing_probability_table);
 
    
  /** Return the current estimation of the @em nth distribution.

      @param[in] n the position of the distribution to be returned.
      @return the current estimation of the @em nth distribution.
  */
  const plComputableObject &get_distribution(unsigned int n)const;

  /** Set the @em nth distribution. 

      @param[in] n the position of the distribution to be changed.
      @param[in] new_distrib the new distribution.
  */
  void set_distribution(unsigned int n, const plComputableObject &new_distrib);
  
  /** Return the joint distribution (model) with the current learnt parameters.
      @return the joint distribution with the current learnt parameters.
  */
  const plJointDistribution &get_joint_distribution()const;

  /** If 'same' = true, the learning data will contain missing values
      for the same variables in all data rows (unobserved variables).
      If 'same' = false, missing values may concern different variables.

      This method is used just to speed up the algorithm.
  */
  void set_same_missing_variables(bool same = true){same_undef_var_ = same;}
    
  /** Outputs the current learnt distributions on the output stream 'out'. */
  void output_distributions(std::ostream &out=std::cout)const;
    
  /** Sets distributions displaying, after each iteration, to ON or OFF. */
  void set_distributions_display(bool display = true);

  /** Sets distributions displaying stream to 'out'. */
  void set_distributions_display_stream(std::ostream &out);
 
  /** Freezes the nth distribution to its current state. */
  void distribution_set_frozen(unsigned int n, bool freeze = true);

  /** Sets or unsets the use of a junction tree for inference. By default,
      inference is done using the ordinary successive restrictions algorithm
      as accessed by plJointDistribution.
  */
  void use_junction_tree(bool use_jt = true);
  
  /** This method allows to set the marginalization mode to 'monte carlo' using
      'npoints' sample points. It sets the integration mode to 'exact' if
      'npoints' is null.
      
      This is only used for computing the likelihood of the data (given the
      model) P(Observed) because this computation requires the marginalization
      over the missing variables. Calling this function is mondatory if at
      least one of the missing variables is continuous.
  */
  void set_mc_integration_npoints(unsigned int npoints)
  {
    if(npoints > 0){
      mc_integ_mode_ = true;
      mc_npoints_ = npoints;
    }
    else {
      mc_integ_mode_ = false;
    }
  }
  
  /** This method allows to set the compilation mode to 'approximate' using a
      maximal time of 'compilation_time_in_seconds' (in seconds) when
      constructing the distribution P(Missing | Observed) in the E step. It
      sets the compilation mode to 'exact' if 'compilation_time_in_seconds' is
      null.

      Calling this function (or set_approximate_compilation_nsamples()) is
      mondatory if at least one of the missing variables is continuous.

      Calling this function changes the stop criterion for constructing the
      distribution P(Missing | Observed) to 'time'.

      \sa set_approximate_compilation_nsamples
  */
  void set_approximate_compilation_time(double compilation_time_in_seconds)
  { 
    if(compilation_time_in_seconds > 0.0){
      approx_compilation_mode_ = true;
      approx_compilation_n_ = 0;
      approx_compilation_time_ = compilation_time_in_seconds;
    }
    else {
      approx_compilation_mode_ = false;
    }
  }
  
  /** This method allows to set the compilation mode to 'approximate' using a
      number of samples of 'n' when constructing the distribution P(Missing |
      Observed) in the E step. It sets the compilation mode to 'exact' if 'n'
      is null.

      Calling this function (or set_approximate_compilation_time()) is
      mondatory if at least one of the missing variables is continuous.

      Calling this function changes the stop criterion for constructing the
      distribution P(Missing | Observed) to 'number of
      samples'.

      \sa set_approximate_compilation_time
  */
  void set_approximate_compilation_nsamples(unsigned int n)
  {
    if(n > 0){
      approx_compilation_mode_ = true;
      approx_compilation_n_ = n;
    }
    else {
      approx_compilation_mode_ = false;
    }
  }
  
  /** This method allows to set the methode to be used for
      constructing the distribution P(Missing | Observed) in the E
      step when using approximate mode (using
      set_approximate_compilation_time or
      set_approximate_compilation_nsamples).  If set to @em true,
      P(Missing | Observed) will be sampled and the corresponding
      points will be directly used in the M_step instead to
      constructing explicitly the corresponding probability table.
      
      The default value of this parameter is @em false. Do not change
      this parameter unless you understand exactly its effect.
  */
  void set_use_sampling_for_compilation(bool use_it =true)
    {
      use_sampling_for_compilation_ = use_it;
    }

  /** Compute for each data row: 
      (i) the values with the highest probability for the
      missing variables argmax P(Missing)

      (ii) the current probability table on the
      missing variables P(Missing).
  */
  void compute_missing_values_infos(plDataDescriptor &data_descriptor,
                                    std::vector <plValues> &missing_most_probable_value,
                                    std::vector < std::vector <plProbValue> > &missing_probability_table);
  
  /** Compute and return, for each data row, the log-likelihood of the data given the current
      parameters estimation.
  */
  plFloat compute_loglikelihood(plDataDescriptor &data_descriptor,
                                std::vector <plFloat> &data_loglikelihood);

  /** Computes the log-likelihood of the data given the current
      parameters estimation.
  */
  plFloat compute_loglikelihood(plDataDescriptor &data_descriptor);
  
  /** Returns the list of distributions (computable object)
      corresponding to the current estimation.
  */
  plComputableObjectList get_computable_object_list()const;

  
  /** Set/unset log-likelihood evolution output */
  void set_trace_loglikelihood(bool trace_it = true)
    {
      trace_loglikelihood_ = trace_it;
    }

  /** Return a pointer to the @em nth internal learn object in the same order
      as passed to the constuctor.

      @param[in] n the position of the distribution to be returned.  @return a
      pointer to the @em nth internal learn object in the same order as passed
      to the constuctor.
  */
  plLearnObject* get_learn_object(unsigned int n)const;

  /** Return the vector of the stored learn objects */
  const std::vector<plLearnObject*> &get_learn_objects()const;

  /** Run the EM algorithm for one iteration. 
      
      @param[in] data_descriptor the data set on which the EM algorithm will
      iterate. It can contain arbitrary missing data.
  */
  void iteration(plDataDescriptor &data_descriptor);
  
  /** Run the EM algorithm for oneiterations and return this last iteration info.
      
      @param[in] data_descriptor the data set on which the EM algorithm will
      iterate.  It can contain arbitrary missing data.

      @param[out] missing_most_probable_value the values with the highest
      probability for the missing variables argmax P(Missing).
      @param[out] missing_probability_table the current probability table on
      the missing variables P(Missing).
  */
  void iteration(plDataDescriptor &data_descriptor,
                 std::vector <plValues> &missing_most_probable_value,
                 std::vector < std::vector <plProbValue> > &missing_probability_table);


  /** Returns the last cached loglikelihood value. 
      Throws an exception if this value is not available or made obsolete.
  */
  plFloat get_last_computed_loglikelihood()const
  {
    if(last_computed_loglikelihood_ == std::numeric_limits<plFloat>::infinity())
      throw plError(124);

    return last_computed_loglikelihood_;
  }

  /** Set/unset observation and inference information for each data record */
  void set_trace_observation_and_inference_info(bool set_it=true)
  {
    trace_observation_and_inference_info_ = set_it;
  }
  
  /** Returns the number of parameters to be learnt */
  unsigned int get_n_parameters()const;
  
 protected:
  void E_step(const plValues  *descriptor_current_observation_point,
              const std::vector<bool> *descriptor_current_observation_point_def);

  void M_step();

  bool parse_current_observation(const plValues  *descriptor_current_observation_point,
                                 const std::vector<bool> *descriptor_current_observation_point_def);

  void update_distributions();
  
  void compute_current_missing_value_infos(const plValues  *descriptor_current_observation_point,
                                           const std::vector<bool> *descriptor_current_observation_point_def,
                                           plValues &best_value, 
                                           std::vector <plProbValue> &probability_table);
  
  plFloat compute_current_observation_loglikelihood(const plValues  *descriptor_current_observation_point,
                                                    const std::vector<bool> *descriptor_current_observation_point_def);

  void copy_desc_data_to_complete_data(const plValues  *descriptor_current_observation_point);

  void check_initialization(const plVariablesConjunction &descriptor_variables)const;

  void ask(plCndDistribution & result,
           plVariablesConjunction const & left,
           plVariablesConjunction const & right)const;

  void ask(plDistribution & result,
           plVariablesConjunction const & left)const;

  void M_step_using_samples();

  void M_step_using_samples_n();

  void M_step_using_samples_time();

  void M_step_using_prob_table();
  
  void construct_junction_tree()const;

  void init_distributions();

  void set_observed_variables(const plVariablesConjunction &descriptor_variables);

  //void error_missing_continuous_marginalization()const;
  //void error_missing_continuous_compilation()const;
  void approx_compile();
  void cleanup();

  virtual void Output(std::ostream &out) const;

  void iteration_internal(plDataDescriptor&data_descriptor);
  
  
 
  void iteration_internal(plDataDescriptor &data_descriptor,
                          std::vector <plValues> &missing_most_probable_value,
                          std::vector < std::vector <plProbValue> > &missing_probability_table);

  void finalize_copy();

  void print_data_record(const plValues  *descriptor_current_observation_point,
                         const std::vector<bool> *descriptor_current_observation_point_def)const;

 private:
     
  std::vector <plSymbol> descriptor_symbols_;
  plVariablesConjunction current_def_vars_;
  plVariablesConjunction current_undef_vars_;
  std::vector <unsigned int> descriptor_current_def_symbols_;
  std::vector <unsigned int> descriptor_old_def_symbols_;
  std::vector <unsigned int> current_undef_symbols_; 
  std::vector <unsigned int> static_undef_symbols_; 
  plLearnDistribVector *learn_distrib_vector_;
  plVariablesConjunction all_variables_;
  std::vector<plFloat> all_variables_data_;
  std::vector<unsigned int> descriptor_vars_2_all_vars_;
  plVariablesConjunction static_undef_variables_;
  bool same_undef_var_;
  std::vector <plMutableComputableObject*> distributions_;
  plCndDistribution current_pUndefCnd_;
  plDistribution current_pUndef_;
  plDistribution current_pUndefComp_;
  std::vector <plProbValue> current_undef_prob_table_;
  plValues *current_defined_values_;
  plValues *current_undefined_values_;
  plJointDistribution *joint_;
  mutable plJunctionTree * junction_tree_;
  bool output_distribution_after_each_iteration_;
  bool there_is_missing_values_;
  std::vector <bool> distrib_is_not_frozen_;
  std::ostream *display_stream_;
  bool mc_integ_mode_;
  unsigned int mc_npoints_;
  bool approx_compilation_mode_;
  double approx_compilation_time_;
  plDistribution current_pDef_;
  bool first_undef_question_;
  bool first_def_question_;
  plComputableObjectList init_distribs_;
  bool trace_loglikelihood_;
  unsigned int approx_compilation_n_;
  plTimer timer_;
  bool jt_to_be_used_;
  plVariablesConjunction current_descriptor_variables_;
  bool use_sampling_for_compilation_;
  plFloat last_computed_loglikelihood_;
  bool trace_observation_and_inference_info_;
  int current_data_record_; 
  bool data_have_missing_values_;
};

#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
