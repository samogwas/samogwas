
#include "fltm/fltm.hpp"
#include "fltm/latent_var_criteria.hpp"
#include <boost/log/trivial.hpp>

#include <fstream>
#include <vector>

namespace samogwas {


void FLTM::execute( ClustAlgoPtr clustAlgo, CardFuncPtr cardFunc, GraphPtr graph ) {
  auto lab2Idx = create_index_map(*graph);
  Local2GlobalPtr l2g = create_local_to_global_map(*graph);  
  auto criteria = clustAlgo->get_criteria();
 
  for ( int step = 0; step < params.nbrSteps; ++step) {
    if (step > 0) {
      criteria = create_current_criteria( *graph, *l2g, params.maxDist, step);
      clustAlgo->set_measure( graph, l2g, criteria );
    }

   
    BOOST_LOG_TRIVIAL(trace) << "FLTM - step[" << step << "] over " << params.nbrSteps;
    BOOST_LOG_TRIVIAL(trace) << "running clustering " << clustAlgo->name()
                             << " on " << l2g->size();
    auto partition = clustAlgo->run();
    auto clustering = partition.to_clustering();
    auto SIZE = l2g->size();
    BOOST_LOG_TRIVIAL(trace) << "to obtain: " << clustering.size();   
    int nonSingletons =  number_non_singletons(clustering);
    BOOST_LOG_TRIVIAL(trace) << "num non-singletons: " << nonSingletons;
    if ( nonSingletons == 0 ) {
      BOOST_LOG_TRIVIAL(trace) << "stop due to only singleton.";
      return;
    }

    std::vector<int> l2gTemp(*l2g);

    Local2Global().swap(*l2g);  
    int nbrGoodClusters = 0; int numClust = 0;
    for ( auto &cluster: clustering ) {      
      if ( cluster.size() > 1 ) {
        numClust++;
        RandVar var("latent-"+boost::lexical_cast<std::string>(boost::num_vertices(*graph)),
                    plIntegerType(0, cardFunc->compute(cluster) - 1 ));
        Node latentNode = create_latent_node( graph, var, l2gTemp, lab2Idx, cluster);
        MultiEM em(params.nbrRestarts, params.seed);
        em.run( graph, latentNode, params.emThres);
        if ( accept_latent_variable( *graph, latentNode, params.latentVarQualityThres) ) {
          nbrGoodClusters++;
          add_latent_node( *graph, latentNode );
          update_index_map( *l2g, l2gTemp, latentNode );
          lab2Idx[ latentNode.getLabel() ] = latentNode.index;

          for ( auto item: cluster ) {
            // l2g.push_back( currentL2G.at(item) );
            boost::add_edge( latentNode.index, l2gTemp.at(item), *graph); 
          }
            
        } else {
          update_index_map( *l2g, l2gTemp, cluster);
        }
      } else {
        update_index_map( *l2g, l2gTemp, cluster);
      }
    }

    BOOST_LOG_TRIVIAL(trace) << "nbrGoodClusters: " << nbrGoodClusters;

    if ( nbrGoodClusters == 0 ) {
      BOOST_LOG_TRIVIAL(trace) << "stop due to zero good clusters.";
      return;
    }
    if (l2g->size() <= 1) {
      BOOST_LOG_TRIVIAL(trace) << "stop due to zero or only one cluster.";
      return;
    }
    BOOST_LOG_TRIVIAL(trace) << std::endl  << std::endl;
  }
}

int FLTM::number_non_singletons( const Clustering &clustering ) {
  int singletonCount = 0, nonSingletons = 0; // reset ount
  for ( auto clt: clustering ) {      
    if ( clt.size() <= 1) {
    } else {
      nonSingletons++;
    }
  }
  return nonSingletons;
}


bool FLTM::accept_latent_variable( Graph& g, Node& node, double qualityThres ) {
  double q = AverageMutInfo::compute( g, node );
  return (q >= qualityThres);
}

Node FLTM::create_latent_node( GraphPtr graph, plSymbol& var, Local2Global& l2g, Label2Index& l2i, Cluster& cluster ) {
  Node newNode;
  plVariablesConjunction vars;
  for (auto idx: cluster) {
    auto globalIdx = l2g[idx];
    Node& n = (*graph)[globalIdx];
    vars ^= n.variable;  
  }
  newNode.set_graph(graph);
  newNode.set_variable(var);

  newNode.set_local_indexes( vars.begin(), vars.end(), l2i);
  return newNode;  
}

Node& FLTM::add_latent_node( Graph& graph, Node& n ) {
  auto vertexId = boost::add_vertex(graph); // adds a new Node to the graph and returns the newly added node's index.
  Node& newNode = graph[vertexId];
  newNode.copy_data(n);
  newNode.index = vertexId;
  n.index = vertexId;
  return newNode;

}

Label2Index FLTM::create_index_map( const Graph& graph ) {
  Label2Index lab2idx;
  typedef boost::graph_traits<Graph>::vertex_iterator vertex_iter;
  std::pair<vertex_iter, vertex_iter> vp;
  for (vp = vertices(graph); vp.first != vp.second; ++vp.first) {
    lab2idx[graph[*vp.first].variable.name()] = graph[*vp.first].index;
  }

  return lab2idx;
}

Local2GlobalPtr FLTM::create_local_to_global_map(const Graph& graph) {
  auto l2g = std::make_shared<Local2Global>( num_vertices(graph),0 );
  for ( size_t i = 0; i < num_vertices(graph); ++i ) {
    l2g->at(i) = graph[i].index;
  }
  return l2g;
}

void FLTM::update_index_map( Local2Global& l2g, const Local2Global& currentL2G, const Cluster& cluster) {
  l2g.reserve(l2g.size()+cluster.size());
  for ( auto item: cluster ) {
    l2g.push_back( currentL2G.at(item) );
  }
}

void FLTM::update_index_map( Local2Global& l2g, const Local2Global& currentL2G, const Node& latentNode) {
  l2g.reserve(l2g.size()+1);
  l2g.push_back(latentNode.index);
}

CriteriaPtr FLTM::create_current_criteria( Graph& graph, Local2Global& l2g, unsigned MAX_POS, int step) {  
  // if (step >= 1) {
  //   auto positions = std::make_shared<std::vector<int>>(l2g.size(), 0);
  //   for ( size_t i = 0; i < l2g.size(); ++i) {
  //     auto idx = l2g.at(i);
  //     positions->at(i) = graph[idx].position;
  //   }                             

  //   return std::make_shared<PositionCriteria>( positions, MAX_POS*2 );         
  // }

  return std::make_shared<GraphPositionCriteria>( graph, l2g, MAX_POS);
}

}



