/*=============================================================================
 * Product        : ProBT 
 * File           : common.h
 * Author         : David Raulo
 * Creation       : Wed Jun  3 11:35:50 2009
 *
 *=============================================================================
 *        (c) Copyright 2009, ProBayes SAS  -  all rights reserved
 *=============================================================================
*/

#ifndef SL_TEST_COMMON_H_
#define SL_TEST_COMMON_H_

#include <pl.h>
#include <iostream>
#include <string>
#include <vector>

typedef plCSVDataDescriptor DataDescriptor;

plJointDistribution make_model();
void generate_data(const plJointDistribution &jd,
                   const std::string &dataset_filename,
                   unsigned int nb_samples);
void print_result(const plJointDistribution &joint, const std::string &name,
                  DataDescriptor& data);

//-----------------------------------------------------------------------------
#endif //SL_TEST_COMMON_H_

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
