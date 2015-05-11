/****************************************************************************************
 * File: clustering_parameters.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 05/12/2014

 ***************************************************************************************/
#ifndef SAMOGWAS_COMMON_CLUSTERING_PARAMETERS_HPP
#define SAMOGWAS_COMMON_CLUSTERING_PARAMETERS_HPP

#include "distance/diss.hpp"
#include "distance/simi.hpp"
#include "distance/distance.hpp"

#include "clustering/clustering.hpp"
#include "clustering/cast.hpp"
#include "clustering/dbscan.hpp"
#include "graph/graph.hpp"

#include <vector>
#include <memory>
#include <stdexcept>

#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/lexical_cast.hpp>

#include "application_options.hpp"

namespace samogwas
{

typedef ClustAlgo Algorithm;
typedef std::shared_ptr<Algorithm> ClustAlgoPtr;

typedef std::vector<int> Local2Global;
typedef std::shared_ptr<Local2Global> Local2GlobalPtr;
typedef std::vector< int > DataVec;

typedef std::shared_ptr<std::vector<int>> PosVecPtr;

typedef std::vector< DataVec> Matrix; // We consider here only vector of int is relevant
typedef std::shared_ptr<Matrix> MatrixPtr;

typedef int Position;

// typedef MutInfoSimilarity<Matrix> MutInfoSimi;
// typedef MutInfoDissimilarity<Matrix> MutInfoDiss;
// typedef std::vector<Position> Positions;

// typedef DBSCAN<MutInfoDiss> DBSCAN_Algo;
// typedef CAST<MutInfoSimi> CAST_Algo;

inline std::vector<ClustAlgoPtr> read_clustering_algos( GraphPtr graph,
                                                        Local2GlobalPtr l2g,
                                                        PosVecPtr positions,
                                                        Options& opt );


} // namespace samogwas ends here.

/*------------------------------------------------------------------------------------------------------*/

namespace samogwas {

std::vector<ClustAlgoPtr> read_clustering_algos( GraphPtr graph,
                                                 Local2GlobalPtr l2g,
                                                 PosVecPtr positions,
                                                 Options& opt ) {

  auto configPath = opt.clustConf; auto MAX_POS = opt.fltm_maxDist;
  std::ifstream confFile(configPath);
  using boost::property_tree::ptree;  ptree pt;
  read_xml(confFile, pt);

  std::vector<ClustAlgoPtr> rs;
  for( const ptree::value_type &v: pt.get_child("clustering") ) {
    if( v.first == "algorithm" ) {
      ClustAlgoPtr algorithm;
      auto algo_cfg = v.second;
      if ( algo_cfg.get<std::string>("name") == "DBSCAN" ) {

        int minPts; double eps;
        for( const ptree::value_type &pam: algo_cfg.get_child("parameters") ) {
          auto dat = pam.second.data();
          if ( pam.second.get<std::string>("<xmlattr>.name") == "minPts") {
            minPts = boost::lexical_cast<int>(dat);
          } else if (pam.second.get<std::string>("<xmlattr>.name") == "eps") {
            eps = boost::lexical_cast<double>(dat);
          }
        }
        
        auto criteria = std::make_shared<PositionCriteria>( positions, MAX_POS);         
        auto diss = std::make_shared<GraphMutInfoDissimilarity>(graph, l2g);
        diss->set_criteria(criteria);
        algorithm = std::make_shared<DBSCAN>(diss, minPts, eps);

      } else if ( algo_cfg.get<std::string>("name") == "CAST" ) {
        double cast;
        for( const ptree::value_type &pam: algo_cfg.get_child("parameters") ) {
          auto dat = pam.second.data();
          if ( pam.second.get<std::string>("<xmlattr>.name") == "t" ) {
            cast = boost::lexical_cast<double>(dat);
          }
        }
        
        auto criteria = std::make_shared<PositionCriteria>( positions, MAX_POS);
        auto simi = std::make_shared<GraphMutInfoSimilarity>(graph, l2g);
        simi->set_criteria(criteria);
        algorithm = std::make_shared<CAST>(simi, cast);
        
      } else if ( algo_cfg.get<std::string>("name") == "LOUVAIN" ) {
                
       
      } else {
        
        throw std::invalid_argument( "unknown algorithm" );
      }
      printf("processing: %s\n", algorithm->name());
      rs.push_back(algorithm);
    }
  }    
  return rs;
  
  confFile.close();
}

}

/****************************************************************************************/
#endif // SAMOGWAS_CLUSTERING_PARAMETERS_HPP
