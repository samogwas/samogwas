/// \file plSignature.h
/*=============================================================================
 * Product        : ProBT 
 * File           : plSignature.h
 * Author         : Juan-Manuel Ahuactzin
 * Creation       : Mon Jul 30 14:55:42 2001
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




#ifndef PL_SIGNATURE_H_
#define PL_SIGNATURE_H_

#include "plConfig.h"

#ifdef PL_WARN_EFFCPP
#pragma GCC diagnostic ignored "-Weffc++"
#endif //PL_WARN_EFFCPP

#include <bitset>

#ifdef PL_WARN_EFFCPP
#pragma GCC diagnostic warning "-Weffc++"
#endif //PL_WARN_EFFCPP

#include <plDLLExport.h>

class kplKernel;

/*****************************************************************************
 *                           Type Definitions
 *****************************************************************************/

/* NOTE: when you add or remove a label from this enum, update
   the code in plSignature.cpp accordingly.
 */

/** Types of Computable Objects */
enum plComputableObjectType {
  PLCndNormal, /*!< plCndNormal. */
  PLComputableObject, /*!< plComputableObject.  */
  PLCndDistribution, /*!< plCndDistribution. */
  PLCndUnknown, /*!< plCndUnknown. */
  PLJointDistribution, /*!< plJointDistribution. */
  PLDeterministic, /*!< plDeterministic. */
  PLCndDeterministic, /*!< plCndDeterministic. */
  PLGamma, /*!< plGamma. */
  PLNormal, /*!< plNormal. */
  PLDistribution, /*!< plDistribution. */
  PLDistributionTable, /*!< plDistributionTable. */
  PLPoisson, /*!< plPoisson. */
  PLProbTable, /*!< plProbTable. */
  PLUniform, /*!< plUniform. */
  PLUnknown, /*!< plUnknown. */
  PLAnonymousDistribution, /*!< plAnonymousDistribution. */
  PLCndAnonymousDistribution, /*!< plCndAnonymousDistribution. */
  PLIneqConstraint, /*!< plIneqConstraint. */
  PLLogNormal, /*!< plLogNormal. */
  PLCndLogNormal, /*!< plCndLogNormal. */
  PLExponential, /*!< plExponential. */
  PLBeta, /*!< plBeta. */
  PLBinomial, /*!< plBinomial. */
  PLDirichlet, /*!< plDirichlet. */
  PLCndUniform, /*!< plCndUniform. */
  PLCndPoisson, /*!< plCndPoisson. */
  PLJtDistribution, /*!< plJtDistribution. */
  PLCndJtDistribution, /*!< plCndJtDistribution. */
  PLCndBinomial, /*!< plCndBinomial. */
  PLWeibull, /*!< plWeibull. */
  PLNoisyOR, /*!< plNoisyOR. */
  PLLinearRegression, /*!< plLinearRegression. */
  PLSoftmax, /*!< plSoftmax. */
  PLCndBeta, /*!< plCndBeta. */
  PLCndExponential, /*!< plCndExponential. */
  PLCndGamma, /*!< plCndGamma. */
  PLCndWeibull, /*!< plCndWeibull. */
  PLVonMises, /*!< plVonMises. */
  PLCndVonMises, /*!< plCndVonMises. */
  NUMBER_OF_PL_COMPUTABLE_OBJECTS //Keep this in last position
};

/** Defines the set of derived Computable Objects of a particular
    Computable Object (e.g. a plUniform is a plDistribution and a
    plComputableObject)*/
typedef std::bitset <NUMBER_OF_PL_COMPUTABLE_OBJECTS> plSignature;


/*****************************************************************************
 *                           Function Definitions
 *****************************************************************************/

/** Get the printable name of a plComputableObjectType. */
PL_DLL_API 
std::string pl_get_computable_object_type_name(plComputableObjectType);

/** Get a plComputableObjectType value from its printable name. */
PL_DLL_API
plComputableObjectType pl_get_computable_object_type(const std::string &name);

/** Get the actual (most-derived) type of a plComputableObject given
    its signature. */
PL_DLL_API
plComputableObjectType pl_get_computable_object_type(const plSignature &signature);

/** Build the full PL signature from a plComputableObjectType. This is
    the reverse operation of pl_get_computable_object_type(const plSignature&). */
PL_DLL_API
plSignature pl_get_signature(plComputableObjectType type);

#endif //PL_SIGNATURE_H_

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
