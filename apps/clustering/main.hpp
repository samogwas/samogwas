/****************************************************************************************
 * File: fltm_main.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 29/04/2015

 ***************************************************************************************/
#ifndef SAMOGWAS_FLTM_MAIN_HPP
#define SAMOGWAS_FLTM_MAIN_HPP

#include "clustering/clustering.hpp"
#include "clustering/dbscan.hpp"
#include "clustering/cast.hpp"
#include "graph/graph.hpp"

#include "dataload.hpp"
#include <vector>
#include <string>
#include <memory>

namespace samogwas
{

inline Local2GlobalPtr init_index_mapping(const size_t sz) {
  auto l2g = std::make_shared<Local2Global>(sz, -1);
  for (size_t i = 0; i < sz; ++i) {
    l2g->at(i) = i;
  }
  return l2g;
}

inline GraphPtr init_graph( PtrMatrix& mat, Label2Index& lab2Idx,
                            const int CARD, const LabelVec& labels,
                            const PosVec& positions ) {
  auto graph = std::make_shared<Graph>();

  auto nrows = positions.size();
  for (int i = 0; i < nrows; ++i) {
    plSymbol v(labels.at(i), plIntegerType(0, CARD-1) );
    auto dataVec = mat.at(i); //std::make_shared<DataVec>(data->at(i));
    createObsNode(graph, v, dataVec, positions.at(i), lab2Idx);
  }

  return graph;
}


} // namespace samogwas ends here. 

/****************************************************************************************/
#endif // SAMOGWAS_FLTM_MAIN_HPP


