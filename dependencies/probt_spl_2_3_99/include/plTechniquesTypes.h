/// \file plTechniquesTypes.h
/*=============================================================================
 * Product        : ProBT 
 * File           : plTechniquesTypes.h
 * Author         : Juan-Manuel Ahuactzin
 * Creation       : Mon Jan 28 19:06:37 2002
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

#ifndef plTechniquesTypes_h
#define plTechniquesTypes_h

#include "plConfig.h"

/* Doxygen bug: a reference like "plJointDistribution::ask()" creates a broken link.
   That is why for now we say "%plJointDistribution::ask()" instead.
 */

/** Generator technique to produce points of a distribution space (used in
    %plDistribution::time_compile() and %plDistribution::n_compile()). */
enum plGeneratorType {PL_CHOOSE_GENERATOR_FOR_ME, /*!< Let ProBT choose for you. */
                      PL_EXHAUSTIVE_GENERATOR, /*!< Systematic exhaustive exploration. */
                      PL_GA_GENERATOR, /*!< Exploration using a genetic algorithm. */
                      PL_RANDOM_GENERATOR, /*!< Uniform random exploration. */
                      PL_MC_GENERATOR /*!< Random (non uniform) exploration by
                                        drawing according to the sampled
                                        distribution. */
 };

/** Compiled distributions technique to represent a distribution (used in
    %plDistribution::time_compile() and %plDistribution::n_compile()). */
enum plCompiledDistributionType {PL_CHOOSE_COMP_TYPE_FOR_ME, /*!< Let ProBT choose for you. */
                                 PL_MRBT, /*!< Multi Resolution Binary Tree representation. */
                                 PL_TABLE, /*!< Table representation. */
                                 PL_MAP /*!< Non-null values map representation. */
};

/** Optimization criterion when building an expression (used in %plJointDistribution::ask()).*/
enum plOptimizationCriterion {PL_OPTIMIZE_COMPILATION_TIME, /*!< optimize the
                                                              expression for the first instantiate-compile sequence that will follow
                                                              (usage pattern: ask-instantiate-compile) */
                              PL_OPTIMIZE_UPDATE_TIME, /*!< optimize the expression for repeated
                                                         instantiate-compile sequences (usage pattern: ask once,
                                                         instantiate-compile many times). */
                              PL_OPTIMIZE_MEMORY_USE, /*!< optimize the expression for minimal memory
                                                        use. */
                              PL_NO_OPTIMIZATION /*!< Not attempt to optimize
                                                   the expression for any particular purpose. */
};

#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
