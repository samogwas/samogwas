
#include <map>
#include <string>
#include <iomanip>

#include <boost/lexical_cast.hpp>
#include <boost/log/trivial.hpp>

#include "graph/graph_io.hpp"
#include "utils/csv_parser.hpp"

namespace samogwas {

/** BN vertex file: see SINGLE format.
 *  BN distribution file: see above.
 */
void BayesGraphLoad::operator()( std::shared_ptr<Graph> graph,
                                 const std::string labPosFileName,
                                 const std::string vertexFileName,
                                 const std::string distributionFileName,
                                 const std::string cndDataFileName,
                                 const std::string dataFileName
                                ) const {

  BOOST_LOG_TRIVIAL(trace) << "begin loading label..." << std::endl;
  LabPosMap labPosMap = readLabPos(labPosFileName);
  BOOST_LOG_TRIVIAL(trace) << "end loading label..." << std::endl;
  std::ifstream vertexFile(vertexFileName.c_str()), distributionFile(distributionFileName.c_str()), dataFile(dataFileName.c_str());
  CSVIterator<std::string> vertexLine(vertexFile); ++vertexLine; // skips header.
  BOOST_LOG_TRIVIAL(trace) << "begin loading vertices\n";

  for( ; vertexLine != CSVIterator<std::string>(); ++vertexLine ) {
    size_t id = boost::lexical_cast<size_t>( (*vertexLine)[TULIP_ID] );
    // bool isLeaf = !(boost::lexical_cast<bool>( (*vertexLine)[TULIP_LATENT]));
    int level =  boost::lexical_cast<size_t>( (*vertexLine)[TULIP_LEVEL] );
    size_t cardinality = boost::lexical_cast<size_t>( (*vertexLine)[TULIP_CARDINALITY] );
    std::string label = labPosMap[id].first;
    int position = labPosMap[id].second;
    create_latent_node( graph, cardinality, label, position, level );
  }

  Graph& graphRef = *graph;
  BOOST_LOG_TRIVIAL(trace)  << "end loading vertices: " << boost::num_vertices(graphRef) <<  "\n\n";
  BOOST_LOG_TRIVIAL(trace)  << "begin loading dist\n";

  CSVIterator<std::string> distributionLine(distributionFile);
  while ( distributionLine != CSVIterator<std::string>() ) {
    BOOST_LOG_TRIVIAL(trace) << "begin loading var - 0" << std::endl;
    plVariablesConjunction variables; // holds child variables
    plComputableObjectList jointDistri;
    size_t latentId =  boost::lexical_cast<size_t>( (*distributionLine)[BN_LATENT_ID] );
    size_t nbrChildren = boost::lexical_cast<size_t>( (*distributionLine)[NBR_CHILDREN] );
    Node& latentNode = graphRef[ latentId ]; ++distributionLine; // reads next line.

    std::vector< plProbValue > probValuesZ;
    for ( size_t latentVal = 0; latentVal < latentNode.variable.cardinality(); ++latentVal) { // loads probability table for the latent var
      probValuesZ.push_back( boost::lexical_cast<plProbValue>( (*distributionLine)[latentVal] ) );
    }
    const plProbTable probTabZ(latentNode.variable, probValuesZ); ++distributionLine;
    for ( size_t child = 0; child < nbrChildren; ++child ) {
      size_t childId = boost::lexical_cast<size_t>( (*distributionLine)[BN_LATENT_ID] ); ++distributionLine;
      Node& childNode = graphRef[ childId ]; variables ^= childNode.variable;
      plDistributionTable distTab_Xi_Z ( childNode.variable, latentNode.variable );

      for ( size_t latentVal = 0; latentVal < latentNode.variable.cardinality(); ++latentVal ) {
        std::vector< plProbValue > probValuesXiZ_vals;
        for ( size_t childVal = 0; childVal < childNode.variable.cardinality(); ++childVal ) {
          probValuesXiZ_vals.push_back( boost::lexical_cast<plProbValue>( (*distributionLine)[childVal] ) );
        }
        distTab_Xi_Z.push( plProbTable( childNode.variable, probValuesXiZ_vals), (int)latentVal );
        ++distributionLine;
      }
      jointDistri *= distTab_Xi_Z; // adds the conditional table to result
      boost::add_edge( latentId, childId, graphRef );
    }

    auto jd = ( probTabZ * jointDistri );

    ++distributionLine;
    latentNode.set_joint_distribution(
        plJointDistribution(latentNode.variable ^ variables, probTabZ * jointDistri) );
  }
  distributionFile.close();
  vertexFile.close();

  set_data(*graph, cndDataFileName, dataFileName);
}

void BayesGraphLoad::set_data( Graph& graph,
                               const std::string cndFileName,
                               const std::string obsFileName ) const {

  std::ifstream obsFile(obsFileName.c_str());
  assert(obsFile); // todo: throws instead
  samogwas::CSVIterator<int> obsLine(obsFile);
  size_t index = 0;
  for( ; obsLine != samogwas::CSVIterator<int>(); ++obsLine ) {
    auto row = std::make_shared<Vec>(obsLine->size(), 0);
    for (unsigned i = 0; i < obsLine->size(); ++i) {
      (*row)[i] = obsLine->at(i);
    }
    graph[index++].set_data_vec(row, true);
  }
  obsFile.close();

  std::ifstream cndFile(cndFileName.c_str());
  assert(cndFile); // todo: throws instead
  samogwas::CSVIterator<double> cndLine(cndFile);
  for( ; cndLine != samogwas::CSVIterator<double>(); ++cndLine ) {
    auto row = std::make_shared<RealVec>(cndLine->size(), 0);
    for (unsigned i = 0; i < cndLine->size(); ++i) {
      (*row)[i] = cndLine->at(i);
    }
    graph[index++].set_cnd_obs_vec(row, false);
  }
  cndFile.close();

}


/** vertex file: see SINGLE format.
*  distribution file: see above.
*/
void BayesGraphSave::operator()( const Graph& graph,
                                 const std::string vertexFileName,
                                 const std::string distFileName ) const {

  std::ofstream distFile(distFileName.c_str()), vertexFile(vertexFileName.c_str());
  vertex_iterator vi, vi_end;
  Label2Index label2Index;
  vertexFile << ID << GRAPH_SEPARATOR << LATENT << GRAPH_SEPARATOR << LEVEL << GRAPH_SEPARATOR << CARDINALITY << "\n";  // writes header
 BOOST_LOG_TRIVIAL(trace) << "saving vertices...\n";
  for ( boost::tie(vi, vi_end) = boost::vertices(graph); vi != vi_end; ++vi ) {
    int vertex = *vi;
    vertexFile << graph[vertex].index
               << GRAPH_SEPARATOR
               << !(graph[vertex].is_leaf())
               << GRAPH_SEPARATOR
               << graph[vertex].level
               << GRAPH_SEPARATOR
               << graph[vertex].variable.cardinality() << std::endl;
    label2Index[ graph[*vi].label ] = graph[*vi].index;
  }
  vertexFile.close();
  BOOST_LOG_TRIVIAL(trace) << "saving joint distribution...\n";

  for ( boost::tie(vi, vi_end) = boost::vertices(graph); vi != vi_end; ++vi ) {
    const Node& node = graph[*vi];
    if ( !node.is_leaf() ) {
      auto latentVar = node.variable;
      // plJointDistribution distribution = node.jointDistribution;
      // plVariablesConjunction all_variables = distribution.get_variables(); // all variables (latent variable and its children)
      plVariablesConjunction childVars = node.get_children_variables(); // child childVars
      // for (size_t i = 1; i <  all_variables.size(); ++i)
      //   childVars ^=  all_variables[i]; // initializes child conjunction.
      // plSymbol latentVar =  all_variables[0]; // latent variable
      distFile << node.index << GRAPH_SEPARATOR <<  childVars.size() << std::endl;

      // plComputableObjectList objLists = distribution.get_computable_object_list();
      // plComputableObject probTableZ = objLists.get_distribution_over(latentVar); // distribution table for the latent variable
      auto probTableZ = node.marginalDist; int val;

      for ( val = 0; val < latentVar.cardinality() - 1 ; ++val ) {
        distFile << std::fixed << std::setprecision(30)
                 << probTableZ->compute( plValues().add(latentVar, val) )
                 << GRAPH_SEPARATOR; // P(latentVar = val)
      }

      distFile << std::fixed << std::setprecision(15)
               << probTableZ->compute( plValues().add(latentVar, val) )
               << std::endl; // writes last probability value

      for ( size_t i = 0; i < childVars.size(); ++i ) {

        plSymbol varX = childVars[ i ]; // retrieves the child variable
        distFile << label2Index[varX.name()] << std::endl; // writes child variable's id.
        auto distTableXZ = node.cndChildrenDists.at(i);  //objLists.get_distribution_over(varX); // conditional distribution P(X_i | Z)
        // plDistributionTable& distTableXZ =
        //     static_cast<plDistributionTable&>( compTableXZ ); // casting P(X_i | Z) to derived class

        for ( val = 0; val < latentVar.cardinality(); ++val ) {
          int childVal;
          for ( childVal = 0; childVal < varX.cardinality() - 1; ++childVal ) { // for each value x of the child variable            
            distFile << std::fixed << std::setprecision(15)
                     << distTableXZ->compute( plValues().add(latentVar, val).add(varX, childVal) )
                     << GRAPH_SEPARATOR; // p(X_i = childVal | Z = val)
          }
          distFile << std::fixed << std::setprecision(15)
                   << distTableXZ->compute( plValues().add(latentVar, val).add(varX, childVal) ) << std::endl;
       }
      }
      distFile << std::endl; // breaks the line, moves to the next latent variable.
    }
  }

  distFile.close();
}


 // Format LP_ID = 0, LP_LABEL, LP_POSITION
LabPosMap FLTMGraphReader::readLabPos( const std::string labPosFileName ) const {
  LabPosMap lpMap;
  std::ifstream labPosFile(labPosFileName.c_str());
  CSVIterator<std::string> labPosLine(labPosFile);

  size_t id = 0;
  for( ; labPosLine != CSVIterator<std::string>(); ++labPosLine ) {
    int position; std::string label;
    if (labPosLine->size() == 4) {
      // id = boost::lexical_cast<size_t>( (*labPosLine)[LP_ID] );
      label = (*labPosLine)[LP_LABEL] ;
      position = boost::lexical_cast<int>( (*labPosLine)[LP_POSITION] );
    } else {
      // id = boost::lexical_cast<size_t>( (*labPosLine)[LP_ID+1] );
      label = (*labPosLine)[LP_LABEL+1] ;
      position = boost::lexical_cast<int>( (*labPosLine)[LP_POSITION+1] );
    }
    lpMap[id] = std::pair<std::string, int>(label, position);
    id++;
  }
  return lpMap;
}




//////////////////
//static const std::string LABEL = "LABEL";
void TulipGraphSave::operator()(const Graph& graph,
                                const std::string nodeFile,
                                const std::string edgeF) const {
  std::ofstream vertexFile(nodeFile.c_str());
  vertex_iterator vi, vi_end;
  vertexFile << ID << GRAPH_SEPARATOR << LABEL << GRAPH_SEPARATOR << LEVEL << GRAPH_SEPARATOR << CARDINALITY << GRAPH_SEPARATOR <<  POSITION << "\n";  // writes header
  for ( boost::tie(vi, vi_end) = boost::vertices(graph); vi != vi_end; ++vi ) {
    int vertex = *vi;
    vertexFile << graph[vertex].index << GRAPH_SEPARATOR
               << graph[vertex].getLabel() << GRAPH_SEPARATOR
               << graph[vertex].level << GRAPH_SEPARATOR
               << graph[vertex].variable.cardinality() << GRAPH_SEPARATOR
               << graph[vertex].position
               << std::endl;
  }
  vertexFile.close();

  std::ofstream edgeFile(edgeF.c_str());
  edgeFile << PARENT_ID << GRAPH_SEPARATOR << ID << "\n"; // writes header
  edge_iterator ei, ei_end;
  for( boost::tie(ei, ei_end) = boost::edges(graph); ei != ei_end; ++ei ) {
    edgeFile << boost::source(*ei, graph) << GRAPH_SEPARATOR << boost::target(*ei, graph) << std::endl;
  }
  edgeFile.close();
}

}
