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
typedef std::map<std::string, std::string> AlgoConf;

inline std::vector<AlgoConf> read_clustering_algos( std::string confFile );

} // namespace samogwas ends here.

/*------------------------------------------------------------------------------------------------------*/

namespace samogwas {


std::vector<AlgoConf> read_clustering_algos( 
                                             std::string cPath ) {

  auto configPath = cPath; 
  std::ifstream confFile(configPath);
  using boost::property_tree::ptree;  ptree pt;
  read_xml(confFile, pt);

  std::vector<AlgoConf> rs;
  for( const ptree::value_type &v: pt.get_child("clustering") ) {
    AlgoConf conf;    
    if( v.first == "algorithm" ) {
      auto algo_cfg = v.second;
      conf["name"] = algo_cfg.get<std::string>("name");       

      for( const ptree::value_type &pam: algo_cfg.get_child("parameters") ) {
        auto dat = pam.second.data();
        auto key = pam.second.get<std::string>("<xmlattr>.name");
        conf[key] = dat;
      }   
    }
    rs.push_back(conf);
  }

  return rs;
  confFile.close();
}


inline ClustAlgoPtr read_clustering_algo( AlgoConf& conf,
                                          GraphPtr graph,
                                          Local2GlobalPtr l2g,
                                          PosVecPtr positions,                                            
                                          const unsigned MAX_POS
                                          ) {
  ClustAlgoPtr algorithm;
  if ( conf["name"] == "DBSCAN" ) {
    int minPts; double eps;
    minPts = boost::lexical_cast<int>(conf["minPts"]);
    eps = boost::lexical_cast<double>(conf["eps"]);
    auto criteria = std::make_shared<PositionCriteria>( positions, MAX_POS);         
    auto diss = std::make_shared<GraphMutInfoDissimilarity>(graph, l2g);
    diss->set_criteria(criteria);
    algorithm = std::make_shared<DBSCAN>(diss, minPts, eps);

  } else if ( conf["name"] == "CAST") {

    double cast = boost::lexical_cast<double>(conf["t"]);        
    auto criteria = std::make_shared<PositionCriteria>( positions, MAX_POS);
    auto simi = std::make_shared<GraphMutInfoSimilarity>(graph, l2g);
    simi->set_criteria(criteria);
    algorithm = std::make_shared<CAST>(simi, cast);
    
  } else if ( conf["name"] == "LOUVAIN" ) {

  } 

  return algorithm;
  
}



}

/****************************************************************************************/
#endif // SAMOGWAS_CLUSTERING_PARAMETERS_HPP
