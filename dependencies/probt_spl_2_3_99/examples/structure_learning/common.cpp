/*=============================================================================
 * Product        : ProBT 
 * File           : common.cpp
 * Author         : David Raulo
 * Creation       : Wed Jun  3 11:14:51 2009
 *
 *=============================================================================
 *        (c) Copyright 2009, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */

#include "common.h"
#include <plStructureLearner.h>

//-----------------------------------------------------------------------------
void generate_data(const plJointDistribution &jd, const std::string &dataset_filename,
                   unsigned int nb_samples)
{
  jd.draw_graph("bayesian_network.fig");
  std::cout << std::endl << "Bayesian network: " 
            << std::endl << jd << std::endl;

  plVariablesConjunction variables = jd.get_variables();
  std::ofstream out(dataset_filename.c_str());
  out << "\"" << variables[0].name() << "\"";
  for (unsigned int j=1; j<variables.dim(); ++j)
    out << "; \"" << variables[j].name() << "\"";
  out << std::endl;
  std::vector<unsigned int> values(variables.dim());
  for (unsigned int i=0; i<nb_samples; ++i) {
    jd.draw(values);
    out << values[0];
    for (unsigned int j=1; j<variables.dim(); ++j)
      out << "; " << values[j];
    out << std::endl;
  }
  std::cout << "Generated dataset" << std::endl << std::endl;
}

//-----------------------------------------------------------------------------
void print_result(const plJointDistribution &joint, const std::string &name,
                  DataDescriptor& data)
{
  std::cout << "Bayesian Network obtained by " 
            << name << " algo on complete dataset:"
            << std::endl << std::endl << joint << std::endl;
#if 0
  // Get back the tables
  plComputableObjectList comp_list(joint.get_computable_object_list());
  for (unsigned int i=0; i<comp_list.size(); ++i) {
    const plComputableObject object(comp_list[i]);
    std::cout << "Table #" << i << ":" << std::endl
              << object << std::endl;
  }
#endif

  // Draw the joint distribution as an xfig file
  std::string filename = name + "_result.fig";
  joint.draw_graph(filename.c_str());

  // Perform some statistics on the dataset.
  plVariablesConjunction variables = joint.get_variables();
  plNodeScoreAIC aic(data);
  plNodeScoreBIC bic(data);
  plNodeScoreBDeu bdeu(data, 3);
  plNodeScoreMDL mdl(data);
  plEdgeScoreMI mi(data);
  
  std::cout << "AIC  score=" << aic(joint) << std::endl;
  std::cout << "BIC  score=" << bic(joint) << std::endl;
  std::cout << "BDeu score=" << bdeu(joint) << std::endl;
  std::cout << "MDL  score=" << mdl(joint) << std::endl;
  std::cout << "MI   score=" << mi(joint) << std::endl
            << std::endl;
}

//-----------------------------------------------------------------------------
// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
