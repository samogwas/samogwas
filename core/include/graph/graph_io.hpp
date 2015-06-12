/****************************************************************************************
 * File: graph_io.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 06/05/2015

 ***************************************************************************************/
#ifndef SAMOGWAS_GRAPH_IO_HPP
#define SAMOGWAS_GRAPH_IO_HPP

#include <map>
#include <string>
#include <boost/lexical_cast.hpp>

#include <pl.h>

#include "graph.hpp"


namespace samogwas
{

typedef std::map<int, std::pair<std::string, int> > LabPosMap;

static const char GRAPH_SEPARATOR = ',';
static const std::string ID = "id"; static const std::string LABEL = "label";
static const std::string LATENT = "latent"; static const std::string PARENT = "parent";
static const std::string LEVEL = "level"; static const std::string POSITION = "position";
static const std::string CARDINALITY = "cardinality";
static const std::string PARENT_ID = "parent_id";

enum FULL { FULL_ID = 0, FULL_LABEL, FULL_LATENT, FULL_PARENT, FULL_LEVEL, FULL_POSITION, FULL_CARDINALITY };
enum LAB_POS { LP_ID = 0, LP_LABEL, LP_POSITION };

// format SINGLE (one file, structure)
enum SINGLE { SINGLE_ID = 0, SINGLE_LATENT, SINGLE_PARENT, SINGLE_LEVEL, SINGLE_CARDINALITY };
// by convention, SINGLE_PARENT set to -1 denotes a root.

// format TULIP (two files, structure)
enum TULIP_VERTICES { TULIP_ID = 0, TULIP_LATENT, TULIP_LEVEL, TULIP_CARDINALITY };
enum TULIP_EDGES { /*TULIP_ID = 0,*/ TULIP_PARENT_ID = 1 };
// by convention, TULIP_PARENT_ID set to -1 denotes a root.


// format BN (two files, structure and parameters)
enum BN_VERTICES { BN_LATENT_ID = 0, NBR_CHILDREN };


/*************************************************************************************/
/*
 *  
 */
struct FLTMGraphReader {
  /** Loads the nodes' idendities, labels and positions from a formatted file and returns a
   * map that maps the identity to a pair of label and position.
   */
  LabPosMap readLabPos( const std::string labPosFileName ) const;  
};

/** Loads the Bayian network corresponding to the FLTM model from two files:
 *  - vertexFileName: information regarding vertices of the following (SINGLE) format: ID, LATENT, PARENT, LEVEL, CARDINALITY
 *  - distributionFileName:
 *     == example: Z has 2 children X1 and X2; card(Z) = 3; card(X1) = card(X2) = 2;
 *         Z_id 2 // 2 denotes the number of children.
 *         p(Z=0) p(Z=1) p(Z=2)
 *         X1_id
 *         P(x1=0 | Z=0) p(X1=1 | Z=0)
 *         p(x1 | Z=1)
 *         p(x1 | Z=2)
 *         X2_id
 *         P(x2=0 | Z=0) p(X2=1 | Z=0)
 *         p(x2 | Z=1)
 *         p(x2 | Z=2)
 *
 *         Y_id 7
 *         etc.
 */
struct BayesGraphLoad: public FLTMGraphReader {

  
  void operator()( GraphPtr graph,
                  const std::string labPosFileName,
                   const std::string vertexFileName,
                   const std::string distributionFileName,
                   const std::string cndDataFileName,
                   const std::string dataFileName ) const;
  
  GraphPtr operator()( const std::string labPosFileName,
                       const std::string vertexFileName,
                       const std::string distributionFileName,
                       const std::string cndDataFileName,
                       const std::string dataFileName ) const  {
    
    auto graph = std::make_shared<Graph>();
    (*this)( graph, labPosFileName, vertexFileName, distributionFileName, cndDataFileName, dataFileName);
    return graph;    
  }

 private: 
  typedef std::vector<int> Vec;
  typedef std::vector<double> RealVec;

  typedef std::shared_ptr<Vec> VecPtr;
  typedef std::vector<VecPtr> PtrMatrix;
  typedef std::shared_ptr<PtrMatrix> PtrMatrixPtr;
  
  void set_data( Graph& g,
                 const std::string cndDataFileName,
                 const std::string dataFileName ) const;
};


/** Saves the Bayesian network corresponding to the FLTM model into two files of the following formats:
 *  - vertexOutputFilename: ID, LABEL, LATENT, PARENT, LEVEL, POSITION, CARDINALITY
 *  - distributionFileName: see above.
 */
struct BayesGraphSave {  
  void operator()( const Graph& graph,
                   const std::string vertexFileName,
                   const std::string distributionFileName ) const;
};


struct TulipGraphSave {
  void operator()( const Graph& graph,
                   const std::string nodeFile ) const;
};


} // namespace samogwas ends here. 


/****************************************************************************************/
#endif // SAMOGWAS_GRAPH_IO_HPP
