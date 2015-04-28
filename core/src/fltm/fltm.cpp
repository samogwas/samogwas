
#include "fltm/fltm.hpp"
#include "fltm/latent_var_criteria.hpp"
#include <boost/log/trivial.hpp>



namespace samogwas {


void FLTM::execute( ClustAlgoPtr clustAlgo, CardFuncPtr cardFunc, GraphPtr graph ) {
  auto lab2Idx = create_index_map(*graph);
  Local2GlobalPtr l2g = create_local_to_global_map(*graph);
    
  for ( int step = 0; step < params.nbrSteps; ++step) {        
    clustAlgo->set_measure( graph, l2g );
    // printf("FLTM step-%d\n", step);
    BOOST_LOG_TRIVIAL(trace) << "\nFLTM - step[" << step << "]\n";
    auto partition = clustAlgo->run();
    auto clustering = partition.to_clustering();
    // printf("running clustering %s on %d variables to obtain: %d clusters\n",
    //        clustAlgo->name(), l2g->size(), clustering.size());
    BOOST_LOG_TRIVIAL(trace) << "running clustering " << clustAlgo->name() 
                             << " on " << l2g->size() << " to obtain: " << clustering.size() << std::endl;
    
    Local2Global().swap(*l2g);

    if ( contains_only_singletons( clustering) ) { return;  }

    int nbrGoodClusters = 0; int numClust = 0;
    for ( auto &cluster: clustering ) {
      if ( cluster.size() > 1 ) {
        RandVar var(boost::lexical_cast<std::string>(boost::num_vertices(*graph)), plIntegerType(0, cardFunc->compute(cluster) - 1 ));
        Node latentNode = create_latent_node( graph, var, lab2Idx, cluster);
         BOOST_LOG_TRIVIAL(trace) << "performing EM...\n";
        MultiEM em(params.nbrRestarts);
        em.run( graph, latentNode, params.emThres);
         BOOST_LOG_TRIVIAL(trace) << "done EM\n";
        if ( accept_latent_variable( *graph, latentNode, params. latentVarQualityThres ) ) {
          nbrGoodClusters++;
          add_latent_node( *graph, latentNode );
          update_index_map( *l2g, latentNode );
        } else {
          update_index_map( *l2g, cluster);
        }
      } else {
        update_index_map( *l2g, cluster);
      }
    }
    if ( nbrGoodClusters == 0 ) {
      return;
    }
    if (l2g->size() <= 1) {
      return;
    }

    BOOST_LOG_TRIVIAL(trace) << std::endl;
  }
}

bool FLTM::contains_only_singletons( const Clustering &clustering ) {
  int singletonCount = 0; // reset ount
  for ( auto clt: clustering ) {      
    if ( clt.size() <= 1) {
      ++singletonCount;
    }
  }
  return (clustering.size() == singletonCount);
}


bool FLTM::accept_latent_variable( Graph& g, Node& node, double qualityThres ) {
  double q = AverageMutInfo::compute( g, node );
  return (q >= qualityThres);
}

Node FLTM::create_latent_node( GraphPtr graph, plSymbol& var, Label2Index& l2i, Cluster& cluster ) {
  Node newNode;
  plVariablesConjunction vars;
  for (auto idx: cluster) {
    Node& n = (*graph)[idx];
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

void FLTM::update_index_map( Local2Global& l2g, const Cluster& cluster) {
  l2g.reserve(l2g.size()+cluster.size());
  for ( auto item: cluster ) {
    l2g.push_back( item );
  }  
}

void FLTM::update_index_map( Local2Global& l2g, const Node& latentNode) {
  l2g.reserve(l2g.size()+1);
  l2g.push_back(latentNode.index);
}

}


