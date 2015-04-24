/*=============================================================================
 * Product        : ProBT 
 * File           : plBackwardCompatibilityV1.h
 * Author         : Kamel Mekhnacha
 * Creation       : Monday Oct  6 18:12:33 2008
 *
 *=============================================================================
 *        (c) Copyright 2008, ProBayes SAS  -  all rights reserved
 *=============================================================================
*/

#ifndef plBackwardCompatibilityV1_h
#define plBackwardCompatibilityV1_h

typedef plCndDistribution plCndKernel;
typedef plCndAnonymousDistribution plCndAnonymousKernel;
typedef plCndJtDistribution plCndJtKernel; 
//typedef plDistributionMap plKernelMap;
typedef plDistributionTable plKernelTable;
//typedef plDistributionDictionary plKernelDictionary;

typedef plDistribution plKernel;
typedef plAnonymousDistribution plAnonymousKernel;
typedef plJtDistribution plJtKernel;

typedef plMutableDistribution plMutableKernel;
typedef plMutableCndDistribution plMutableCndKernel;
typedef plLearnDistributionMap plLearnKernelMap;

typedef plVariablesConjunction plVariable;
typedef plConcurrentHmmSet plMetaHMM;

#define get_kernel_map get_distribution_map
#define get_pushed_kernel get_pushed_distribution

#define plKernelMap plDistributionMap
#define plKernelDictionary plDistributionDictionary

#endif // plBackwardCompatibilityV1_h


// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
