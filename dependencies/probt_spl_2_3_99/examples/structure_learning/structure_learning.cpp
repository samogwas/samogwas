/*=============================================================================
 * Product        : ProBT 
 * File           : structure_learning.cpp
 * Author         : David Raulo
 * Creation       : Wed Jun  3 11:40:25 2009
 *
 *=============================================================================
 *        (c) Copyright 2009, ProBayes SAS  -  all rights reserved
 *=============================================================================
*/

#include "common.h"
#include <plStructureLearner.h>
#include <iostream>

//-----------------------------------------------------------------------------
void save(std::string model_name, const plJointDistribution& joint)
{
  // Save as xfig
  std::string filename = model_name + ".fig";
  joint.draw_graph(filename.c_str());
}

//-----------------------------------------------------------------------------
int main()
{
  {
    // Generate some data
    plJointDistribution orig = make_model();
    generate_data(orig, "model_asia.csv", 10000);
    plCSVDataDescriptor dataset("model_asia.csv", orig.get_variables());
    plNodeScoreBIC node_score(dataset);
    std::cout << "Original model: " << orig << std::endl
              << "BIC score of the original model on the whole dataset: "
              << node_score(orig) << std::endl;
    save("model_asia", orig);
  }

  plSymbol A("A", PL_BINARY_TYPE); // visit to Asia?
  plSymbol S("S", PL_BINARY_TYPE); // Smoker?
  plSymbol T("T", PL_BINARY_TYPE); // has Tuberculosis
  plSymbol L("L", PL_BINARY_TYPE); // has Lung cancer
  plSymbol B("B", PL_BINARY_TYPE); // has Bronchitis
  plSymbol O("O", PL_BINARY_TYPE); // has tuberculosis Or cancer
  plSymbol X("X", PL_BINARY_TYPE); // positive X-Ray
  plSymbol D("D", PL_BINARY_TYPE); // Dyspnoea?

  plVariablesConjunction variables = A^S^T^L^B^O^X^D;
  plCSVDataDescriptor dataset("model_asia.csv", variables);
  plStructureLearner learner(variables);

  // Learn the dependancy structure between our variables from the
  // dataset, using the Directed Maximum Spanning Tree algorithm.
  unsigned int root_index = 0; // using 'A' as the root node.
  std::vector<plSymbol> order;
  plEdgeScoreBIC edge_score(dataset);
  bool result = learner.DMST(edge_score, order, variables[root_index]);
  plJointDistribution result_dmst = learner.get_joint_distribution(dataset);

  // Apply the GS algorithm with BIC score on the same dataset.
  // Use the output of the DMST algo as a starting point.
  plNodeScoreBIC node_score(dataset);
  learner.GS(node_score);
  plJointDistribution result_gs = learner.get_joint_distribution(dataset);

  std::cout << "DMST-BIC obtained the following model: " << result_dmst
            << std::endl
            << "BIC score of the learned model on the whole dataset: "
            << node_score(result_dmst) << std::endl;

  std::cout << "DMST + GS obtained the following model: " << result_gs
            << std::endl
            << "BIC score of the learned model on the whole dataset: "
            << node_score(result_gs) << std::endl;

  save("dmst_bic", result_dmst);
  save("dmst-gs_bic", result_gs);

  // On Windows (Visual C++, MinGW) only.
#if defined(WIN32) || defined(_WIN32)
  std::cout << "Press any key to terminate..." << std::endl;
  getchar();
#endif

  return 0;
}

//-----------------------------------------------------------------------------
// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
