/*=============================================================================
 * Product        : ProBT 
 * File           : pl.h
 * Author         : Juan-Manuel Ahuactzin
 * Creation       : Wed Jul 25 16:28:52 CEST 2001
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

#ifndef pl_h
#define pl_h

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4251 ) // "needs to have dll-interface to be used by clients of class"
#endif

#include "plConfig.h"

//#define  PL_V1_COMPATIBILITY
//#define  PL_V2_COMPATIBILITY



/*! \mainpage ProBT Documentation

  \image html Probayes.jpg

  \section abstract Abstract  

  ProBT is an advanced modeling, inference, and learning platform. It
  extends the Bayesian Networks framework by providing a structured
  programming language allowing the developers to increase their
  applications capabilities and robustness by integrating Bayesian
  models.

  \section features Major features
  
  - ProBT supports both <b>discrete and continuous</b> variables and provides  <b>many types of distributions</b>:
         - Probability table (aka tabular/multinomial)
         - Deterministic (aka Dirac)
         - Binomial
         - Poisson
         - Noisy-OR
         - Softmax (aka logistic/sigmoid)
         - Beta
         - Dirichlet
         - Exponential
         - Gamma
         - Linear Regression (aka Linear Gaussian)
         - Log-Normal
         - Normal (aka Gaussian) on univariate and multivariate variables
         - Weibull
         - Von Mises

  - ProBT provides <b>parameter learning</b> algorithms for all the supported (builtin) distributions above:
         - <b>Maximum Likelihood</b> estimators
         - <b>Bayesian (Maximum A Posteriori)</b> estimators using conjugate priors when available
         - <b>Expectation-Maximization (EM)</b> allowing to handle both <b>missing values</b> and <b>latent variables</b>. 
	 It uses the distribution estimators above (ML and MAP) for the M step and the 
	 inference algorithms (below) for the E step.

  - ProBT provides <b>structure learning</b> algorithms:
         - Score-based structure leaning (MI, BIC, MDL, AIC, ...)

  - ProBT provides several <b>exact inference</b> algorithms:
         - Junction Tree
         - Successive Restrictions (a sophisticated variant of Variable Elimination) with support for using soft evidences
         - Forwards-backwards/Viterbi for Hidden Markov Models (HMMs)

  - ProBT provides <b>approximate inference</b> functionalities:
         - Direct sampling functions for all the supported (builtin) distributions
         - Forward and MCMC Metropolis sampling methods for the inferred probability expressions
         - Sophisticated Genetic Algorithms for sampling and  maximizing the inferred probability expressions
         - Approximate compact numerical representation of probability distributions

  - ProBT provides a set of ready-to-use <b>dynamic models</b>:
         - Dynamic Bayesian Networks
         - HMM (Hidden Markov Model)
         - Kalman, extended Kalman, and Unscented Kalman filters
         - Particle filters

  - ProBT provides basic <b>linear algebra</b> functionalities such as:
         - Matrix/vector multiplication, addition,...
         - Matrix inversion/pseudo-inversion, eigen-decomposition,...

  - ProBT allows using <b>external user functions</b> as callbacks for computing distribution parameters. It accepts:
         - C++ member and non-member user functions 
         - Python and C# functions when using the corresponding bindings
         - C source code functions compiled using an <b>embedded lightweight C complier (TCC)</b>

  - ProBT provides <b>serialization</b> support of all the used classes:
         - Serialization into a file, a memory buffer, or across a network connection link.

  - ProBT provides flexible and easy-to-use <b>interfaces</b>:
         - C++, C#, Python, and Java APIs
         - Excel plugin
         - C# shell
 
  - ProBT provides basic <b>import/export functionalities </b> from/to several known Bayesian Network formats:
         - Genie (xdsl)
         - Netica (dne)
         - Hugin (net)

  - ProBT accepts several <b>data sources</b>:
         - CSV
         - ODBC
         - MYSQL

  - ProBT is <b>highly portable</b>  and is available on the following platforms:
         - Windows
         - Linux
         - MacOS
         - iOS
         - Android


  \section engine ProBT-Engine

  Probayes provides the ProBT-Engine ensuring the ProBT platform full
  power to programmers and it is made available through an Application
  Programming Interface (API) available in C++, C#, Python, and Java. This
  API allows the developers to increase their application capabilities
  by integrating Bayesian models.


  ProBT-Engine is a set of high-performance algorithm modules 
  developed in the C++ language.

  \subsection learning Learning Algorithms

  ProBT-Engine includes two (2) learning modules: @em Structure @em Learning
  and @em Parameter @em Learning.

  \subsubsection strlearning Structure Learning

  The Structure Learning module (see plStructureLearner) allows you to find the
  dependencies between the variables of the model. That is, it allows you to find
  the Bayesian network or decomposition according to the provided data.
  Structure learning is especially useful to construct models on which only poor
  prior knowledge about variable dependencies is available. In this case, the
  model construction is data-driven and variable dependencies are discovered from
  the data.

  \subsubsection paramlearning Parameter Learning

  For a given structure, learning the parameters of all (or some) of the
  distributions of the model can be achieved using the Parameter Learning
  module. This module (see plLearner) can be used either with complete or
  incomplete data.

  Learning using complete data is adaptive, which allows incremental
  and on-line fitting of the data (see plLearnObject). Depending on
  the used optimal criteria, you can obtain Maximum Likelihood (ML)
  estimates (see plMLLearnObject) or a Maximum A Posteriori (MAP)
  estimates (also known as Bayesian estimation) (see
  plBayesLearnObject).  A set of classes that allow learning the most
  common statistical discrete and continuous distribution is
  implemented for both ML and MAP criteria.

  For incomplete data cases, the ProBT-Engine implements a generic 
  Expectation-Maximization (EM) algorithm (see plEMLearner). Indeed, learning using 
  incomplete data is a central issue: some variables of the model may 
  be unobserved and or/some values are missing for another set of 
  variables.

  The ProBT EM algorithm uses the complete data learning classes 
  (stated above) and ProBT’s inference algorithms (stated below), in 
  order to provide a very flexible tool for parameter learning using 
  incomplete data.

  \subsection inference Inference Algorithms

  ProBT-Engine allows inference either in an exact or an approximate way.

  \subsubsection exinference Exact Inference

  The Exact Inference module consists of two general-purpose algorithms for 
  optimal marginalization sequence computation, namely:
  - The @em Successive @em Restrictions algorithm (a kind of variable elimination) allowing
  choices of optimization for memory size or computation time using plJointDistribution::ask;
  - The @em Junction @em Tree algorithm using the plJunctionTree class.


  \subsubsection approxinference Approximate Inference

  The Approximate Inference module is intended to deal with those 
  problems where exact inference is intractable. Unfortunately, for 
  complicated real-world applications, satisfying the time/memory 
  constraints is seldom possible when using exact calculation.
  This is especially the case for probabilistic problems involving a large 
  number of variables and/or dependencies, and/or defined for variables 
  taking values in a huge (or infinite for continuous variables) set of states.

  In this case, exact inference becomes intractable and approximation 
  methods must be used. This case is known as the @em approximate 
  @em Bayesian @em inference problem.

  The approximate inference module provides four (4) schemes (levels) 
  of approximation:
  - Integrals (sums) estimation.
  - A posteriori distributions sampling using MCMC methods to generate 
  a sample set of points (Gibbs, Metropolis, ...).
  - A posteriori distributions maximization to get the Maximum A 
  Posteriori (MAP) solution.
  - Low-memory cost and efficient numerical representation of 
  distributions by selectively visiting high-probability regions of the target 
  space and computing interpolation for the non-visited regions.

  ProBT-Engine proposes a set of approximations 
  for each level, so that time/memory constraints in an application can be 
  taken into account in a more simplistic manner.

  \section api ProBT API

  This API is a set of native C++ classes that allows the construction of 
  Bayesian models and their use for learning and inference. The C#, Python, and Java
  APIs provide bindings for the C++ native classes. This API comprises:
  - A @em state @em space module (see plSampleSpaceObject) allowing you to specify the possible 
  outcome(s) of a model.
  - A @em probability @em distributions module (see plComputableObject) including the most commonly 
  used discrete and continuous statistical distributions
  - A @em user @em function module (see plUserFunction) allowing the user to provide his own 
  functions to be used in his models as callback functions.
  - A @em Builtin @em models module (see plBuiltinModel) including some of the more common 
  Bayesian models such as filters (Kalman, particle filters,...), HMMs, Policy Networks, ... 
  - A @em learning module (see plLearner) allowing to use a set of data for model structure identification 
  (discovering dependencies between the variables) and/or parameter identification of 
  the commonly used statistical distributions.
  - A @em serialization module (see plSerializer), which is intended for saving or transmitting 
  ProBT objects, for example, into a file, a memory buffer or across a 
  network connection link.
  - A @em data @em descriptor module (see plDataDescriptor) allowing to use several data formats and sources
  (CSV, ODBC, MYSQL, ...)


  \section license License
  
  Please, read the content of the PROBT_LICENSE file.
  
  \section platforms Platforms

  ProBT is available for Linux, Windows, MacOS X, iOS, and Android platforms.  ProBT
  is set-up to be highly portable. As a result, it can be compiled and run on
  most Unix flavors as well.
 
  \section support Support 
  - Mailing list: 
  For all questions about this package, and announcements of new releases, you
  are encouraged to write to the probt-users mailing list (probt-users@inrialpes.fr):
  https://listes-serv.inrialpes.fr/wws/arc/probt-users/
  
  - Individual support:
  If you are looking for individual support, contact @em technical_contact@probayes.com.

*/


#include <plAnonymousDistribution.h>
#include <plArray.h>
#include <plBayesLearn1dNormalWithKnownMean.h>
#include <plBayesLearn1dNormalWithKnownStandardDeviation.h>
#include <plBayesLearnBinomial.h>
#include <plBayesLearnExponential.h>
#include <plBayesLearnGamma.h>
#include <plBayesLearnLogNormalWithKnownShape.h>
#include <plBayesLearnNdNormalWithKnownVariance.h>
#include <plBayesLearnPoisson.h>
#include <plBayesLearnProbTable.h>
#include <plBayesianFilter.h>
#include <plBayesianNetwork.h>
#include <plBeta.h>
#include <plBinomial.h>
#include <plCSVDataDescriptor.h>
#include <plCSVFileDataDescriptor.h>
#include <plCSVLineParser.h>
#include <plCndAnonymousDistribution.h>
#include <plCndBinomial.h>
#include <plCndDistribution.h>
#include <plCndLearnObject.h>
#include <plCndLogNormal.h>
#include <plCndNormal.h>
#include <plCndPoisson.h>
#include <plCndUniform.h>
#include <plCndUnknown.h>
#include <plComputableObject.h>
#include <plConcurrentHmmSet.h>
#include <plDataDescriptor.h>
#include <plDataValues.h>
#include <plDeterministic.h>
#include <plDirichlet.h>
#include <plDistribution.h>
#include <plDistributionTable.h>
#include <plEKF.h>
#include <plEMLearner.h>
#include <plErf.h>
#include <plError.h>
#include <plExponential.h>
#include <plExternalFunction.h>
#include <plExternalProbFunction.h>
#include <plFactorial.h>
#include <plFloatMatrix.h>
#include <plFloatVector.h>
#include <plCndDeterministic.h>
#include <plGamma.h> 
#include <plGraph.h>
#include <plHMM.h>
#include <plIneqConstraint.h>
#include <plIntegerType.h>
#include <plContinuousIntervalType.h>
#include <plJointDistribution.h>
#include <plJtDistribution.h>
#include <plJunctionTree.h>
#include <plKalmanFilter.h>
#include <plLabelType.h>
#include <plLearn1dNormal.h>
#include <plLearnDistribVector.h>
#include <plLearnDistributionTable.h>
#include <plLearnExponential.h>
#include <plLearnGamma.h>
#include <plLearnHistogram.h>
#include <plLearnLaplace.h>
#include <plLearnLidstone.h>
#include <plLearnLogNormal.h>
#include <plLearnNdNormal.h>
#include <plLearnObject.h>
#include <plLearnPoisson.h>
#include <plLearnSparseHistogram.h>
#include <plLogNormal.h>
#include <plMatrixDataDescriptor.h>
#include <plMutable.h>
#include <plNonCndLearnObject.h>
#include <plNormal.h>
#include <plParticleFilter.h>
#include <plPhi.h>
#include <plPoisson.h>
#include <plPolicyNetwork.h>
#include <plProbTable.h>
#include <plRandom.h>
#include <plRealType.h>
#include <plSIRParticleFilter.h>
#include <plStatFunctions.h>
#include <plSymbol.h>
#include <plType.h>
#include <plUKF.h>
#include <plUniform.h>
#include <plUnknown.h>
#include <plValues.h>
#include <plVariableIndexer.h>
#include <plVariablesConjunction.h>
#include <plVersion.h>
#include <plWeibull.h>
#include <plLearnNoisyOR.h>
#include <plNoisyOR.h>
#include <plLearnLinearRegression.h>
#include <plLinearRegression.h>
#include <plLearnLinearRegressionFamily.h>

#include <plLearnSoftmax.h>
#include <plLearnSoftmaxFamily.h>
#include <plSoftmax.h>
#include <plLearnCndObject.h>

#include <plLearnBeta.h>

#include <plDiscreteIntervalType.h>

#include <plCndBeta.h>
#include <plCndExponential.h>
#include <plCndGamma.h>
#include <plCndWeibull.h>

#include <plVonMises.h>
#include <plCndVonMises.h>

#include <plLearnGMM.h>
#include <plLearnMixtureModel.h>

#include <plVariableDiscretizer.h>


//#include <plODBCDataDescriptor.h>
//#include <plMysqlDataDescriptor.h>

#ifdef PL_V1_COMPATIBILITY
#include <plBackwardCompatibilityV1.h>
#endif

#ifdef PL_V2_COMPATIBILITY
#include <plBackwardCompatibilityV2.h>
#endif

#ifdef _MSC_VER
#pragma warning( pop )
#endif


#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
