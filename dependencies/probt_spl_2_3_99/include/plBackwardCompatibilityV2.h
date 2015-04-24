/*=============================================================================
 * Product        : ProBT 
 * File           : plBackwardCompatibilityV2.h
 * Author         : Kamel Mekhnacha
 * Creation       : June 2011
 *
 *=============================================================================
 *        (c) Copyright 2011, ProBayes SAS  -  all rights reserved
 *=============================================================================
*/

#ifndef plBackwardCompatibilityV2_h
#define plBackwardCompatibilityV2_h

typedef plNormal plBellShape;
typedef plCndNormal plCndBellShape;
typedef plLearn1dNormal plLearnBellShape;
typedef plBayesLearn1dNormalWithKnownMean plBayesLearnBellShapeWithKnownMean;
typedef plBayesLearn1dNormalWithKnownStandardDeviation plBayesLearnBellShapeWithKnownStandardDeviation;

typedef plUniform plCUniform;
typedef plCndUniform plCndCUniform;

#define plDistributionMap plDistributionTable
#define get_distribution_map get_distribution_table
#define learn_using_data_descriptor learn

typedef plLearnDistributionTable plLearnDistributionMap;

#endif // plBackwardCompatibilityV2_h


// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
