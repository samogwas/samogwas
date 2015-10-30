// /****************************************************************************************
//  * File: gwas_fltm.cpp
//  * Description:
//  * @author: siolag161 (thanh.phan@outlook.com)
//  * @date: 09/07/2014
//  ***************************************************************************************/
#include <iostream>
#include <thread>
#include <chrono>
#include <memory>
#include <limits>

#include <boost/graph/visitors.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/graph/adjacency_list.hpp>

#include <statistics/association_test.hpp>
#include "statistics/permutation_test.hpp"
#include "statistics/test_statistics.hpp"

#include <utils/csv_parser.hpp>
#include <graph/graph.hpp>
#include <graph/graph_io.hpp>

#include "gwas_main.hpp"
#include "app_options.hpp"
#include "../utils/dataload.hpp"
#include "../utils/tools.hpp"


#include <pl.h>

#include <vector>
#include <map>
#include <memory>
#include <string>
#include <cmath>

#include <boost/lexical_cast.hpp>

#include <graph/graph.hpp>

using namespace std;


typedef plJointDistribution JointDist;
typedef plVariablesConjunction Vars;
typedef plSymbol Var;


plJointDistribution generate_tree_net( const std::string prefix, const int num_layer,
                                       const int num_children, const int card = 3);
Vars get_joint_vars(const std::string prefix, const int num_layer,
                    const int num_children, const int card = 3);

plJointDistribution construct_tree_joint_dist(const size_t root,
                                              const samogwas::Graph& g);

// plVariablesConjunction get_obs_vars(size_t root,  const samogwas::Graph& g);


typedef std::pair<plJtDistribution , std::vector<size_t>> TreeCouple;
std::vector<TreeCouple> get_separate_trees(const samogwas::Graph& g);

double compute_prob(samogwas::Graph& g, const std::vector<std::shared_ptr<std::vector<int> > >& dat);
double compute_prob(samogwas::Graph& g, const std::vector<std::vector<int>>& dat);

std::vector<size_t> get_root_nodes(const samogwas::Graph& g);
void add_tree(samogwas::Graph& g,
              const std::string prefix,
              const int num_layer,
              const int num_children,
              const int card);


void print_graph(const samogwas::Graph& g);


///////////////////////////////////////////////////////////////////////////////////////
using namespace samogwas;
int main( int argc, char** argv ) {

  auto args = get_gwas_program_options( argc, argv );
  std::cout << "Loading graph data...\n" << std::endl;
  auto g = std::make_shared<samogwas::Graph>();
  //grapL
  BayesGraphLoad graphLoad;
  auto mat = load_data_table(args.inputDataFile);

  graphLoad( g,
             args.inputLabelFile,
             args.bayesVertices,
             args.bayesDist,
             args.cndDataFile,
             args.inputDataFile );

  Graph& graphRef = *g;

  printf("Done loading graph of %lu edges and %lu vertices\n", boost::num_edges(graphRef),
         boost::num_vertices(graphRef));

  auto outputPath = get_output_dir(args);
  std::cout << "Loading pheno data from " << args.inputPheno<< std::endl; // todo: logging
  // auto pheno = load_phenotype( args.inputPheno );
  std::cout << "Loading label data from " << args.inputLabelFile << std::endl; // todo: logging


  // Graph g;
  // add_tree(g, "0-", 3, 3, 3);
  // add_tree(g, "1-", 3, 3, 3);
  // add_tree(g, "2-", 3, 3, 3);

  // samogwas::Matrix mat;
  // for (int i = 0; i < boost::num_vertices(g); ++i) {
  //   mat.push_back(std::vector<int>(4, 1));
  // }
  double r = compute_prob(graphRef, *mat);
  


}

///////////////////////////////////////////////////////////////////////////////////////
ValueMatPtr load_scores(std::string& infile) {
  auto scores = std::make_shared<ValueMat>();
  std::ifstream scoreFile(infile.c_str());
  if (!scoreFile) {
    std::cout << "score file does not exist. program aborted\n";
    exit(-1);
  }
  samogwas::CSVIterator<double> scoreLine(scoreFile);
  for( ; scoreLine != samogwas::CSVIterator<double>(); ++scoreLine ) {
    std::vector<double> line(scoreLine->size(), 0.0);
    for (size_t i = 0; i < scoreLine->size(); ++i)
      line[i] = scoreLine->at(i);
    scores->push_back(line);
  }

  return scores;
}


PhenoVecPtr load_phenotype(std::string& phenoFile) {
  auto phenoVec = std::make_shared<PhenoVec>();
  std::ifstream labPosFile(phenoFile.c_str());
  if (!labPosFile) {
    std::cout << "phenotype file does not exist. program aborted\n";
    exit(-1);
  }
  samogwas::CSVIterator<std::string> labPosLine(labPosFile);// ++labPosLine;
  for( ; labPosLine != samogwas::CSVIterator<std::string>(); ++labPosLine ) {
    int pheno = boost::lexical_cast<int>( (*labPosLine)[0]);
    phenoVec->push_back(pheno);
  }

  return phenoVec;
}

void assure_graph_positions( samogwas::Graph& g ) {

}

/////////////////////////////////////////////////////////////////////////////////////////

std::vector<int> get_variable_parent( const samogwas::Graph& graph ) {
  std::vector<int> parent( boost::num_vertices(graph), -1 );
  for ( auto ei = boost::edges(graph); ei.first != ei.second; ++ei.first ) {
    auto source = boost::source(*ei.first, graph);
    auto target = boost::target(*ei.first, graph);
    parent[target] = source;
  }

  return parent;
}

std::vector<int> count_cluster_siblings( samogwas::Graph& graph ) {
  std::vector<int> sibling_count( boost::num_vertices(graph) , 0);
  for ( auto vi = boost::vertices(graph); vi.first != vi.second; ++vi.first ) {
    auto v = *vi.first;
    Node& node = graph[v];
    auto ei = boost::out_edges(v, graph);
    int count = std::distance(ei.first, ei.second);
    for ( ; ei.first != ei.second; ++ei.first ) {
      auto child_idx = boost::target(*ei.first, graph);
      sibling_count[child_idx] = count-1;
    }
  }
  return sibling_count;
}

/////////////////////////////////////////////////////////////////////////////////////////

std::map<std::string, std::string> get_snp_mapping(std::string& infile) {
  std::map<std::string, std::string> snp2rs;
  std::ifstream mapFile(infile.c_str());
  if (!mapFile) {
    printf("file mapping %s not existing\n", infile.c_str());
    exit(-1);
  }

  samogwas::CSVIterator<std::string> mapLine(mapFile);// ++labPosLine;
  for( ; mapLine != samogwas::CSVIterator<std::string>(); ++mapLine ) {
    auto snp = (*mapLine)[0];
    auto rs = (*mapLine)[1];
    snp2rs[snp] = rs;
  }

  mapFile.close();
  return snp2rs;
}

//////////////////////////////////////////////////////////////
std::shared_ptr<StatTest> get_stat_test( const int test ) {
  std::shared_ptr<StatTest> statTest;

  switch(test) {
    case 0:
      statTest = std::make_shared<ChiSq>();
      break;
    default:
      statTest = std::make_shared<ChiSq>();
      break;
  }

  return statTest;
}

CandidatesByLevel get_candidates_by_level( const samogwas::Graph& graph ) {
  int max_level = -1;
  for ( auto vp = boost::vertices(graph); vp.first != vp.second; ++vp.first ) {
    auto vertex = *vp.first;
    auto &node = graph[vertex];
    max_level = std::max( node.level, max_level );
  }

  CandidatesByLevel candidates(max_level+1, Candidates());
  for ( auto vp = boost::vertices(graph); vp.first != vp.second; ++vp.first ) {
    auto vertex = *vp.first;
    auto &node = graph[vertex];
    int level = node.level;
    candidates[level].push_back(vertex);
  }

  return candidates;
}


void perform_test( const samogwas::Graph& graph,
                   std::map<std::string, std::string>& snp2rs,
                   const PhenoVec& pheno,
                   std::shared_ptr<samogwas::StatTest> statTest,
                   const CandidatesByLevel& candidatesByLevel,
                   const Cardinalities &cardinalities,
                   const int permutations,
                   const int chr,
                   const std::vector<double> thresholds,
                   const char sep,
                   boost::filesystem::path outDir) {

  auto gwas_filtered_outFile = ( outDir / "gwas_filtered.txt").string();
  auto gwas_outFile = ( outDir / "gwas.txt").string();
  auto gwas_tulip_outFile = ( outDir / "gwas_tulip.csv").string();
  std::ofstream gwasFile(gwas_outFile), gwasFilteredFile(gwas_filtered_outFile), gwasTulipFile(gwas_tulip_outFile);
  printf("beginning of testing of %d permutations by test: %s\n", permutations, statTest->name.c_str());
  int levels = candidatesByLevel.size();

  std::vector<double> scores( boost::num_vertices(graph), 0.0);
  auto parent =  get_graph_parent(graph);

  gwasFile << "chr" << chr << sep << "id" << sep
           << "snp" << sep << "snp-id" << sep
           << "level" << sep << "parent" << sep
           << "position"<< sep << "pval" << sep
           << "corrected-pval" << std::endl;

  gwasTulipFile << "id" << sep << "parent_id" << sep
                << "label" << sep << "level" << sep
                << "cardinality" << sep << "position"<< sep
                << "pval" << sep << "corrected-pval" << std::endl;

  for ( int l = 0; l < candidatesByLevel.size(); ++l) {
    auto &candidates = candidatesByLevel[l];
    auto cards = get_cardinalities(graph,candidates);
    std::vector<double> dist;
    std::vector<double> pvals;
    if ( candidates.size() ) {
      printf("\nWe now tests %lu vars - @level: %d over %d\n\n", candidates.size(), l, levels - 1);
      permutation_procedure( dist,
                             pvals,
                             statTest,
                             graph,
                             pheno,
                             candidates, cards,
                             2, permutations);

      for ( int i = 0; i < candidates.size(); ++i ) {
        auto cand = candidates[i];
        std::string rs = "";
        if ( snp2rs.find(graph[cand].label) != snp2rs.end() ) {
          rs =  snp2rs[graph[cand].label];
        } else {
          rs = "NA";
        }
        scores[cand] =  pvals[2*i+1];
        gwasFile << "chr" << chr << sep
                 << graph[cand].index << sep
                 << graph[cand].label << sep
                 << rs << sep
                 << l << sep
                 << parent[cand] << sep
                 << graph[cand].position << sep
                 << pvals[2*i] << sep
                 << pvals[2*i+1] << std::endl;

        gwasTulipFile << graph[cand].index << sep
                 << parent[cand] << sep
                 << graph[cand].getLabel() << sep
                 << l << sep
                 << graph[cand].variable.cardinality() << sep
                 << graph[cand].position << sep
                 << pvals[2*i] << sep
                 << pvals[2*i+1] << std::endl;

        if ( scores[cand] < thresholds[l]) {
          Node node = graph[cand];
          size_t sz_start = std::numeric_limits<int>::max(), sz_end = 0;
          size_t sum = 0, pos = 0, count = 0;
          for ( auto ei = boost::out_edges(cand, graph); ei.first != ei.second; ++ei.first ) {
            auto child = graph[boost::target(*ei.first, graph)];
            sz_start = std::min(sz_start, (size_t)child.position);
            sz_end = std::max(sz_end, (size_t)child.position);
            count++;
          }
          gwasFilteredFile << "chr" << chr << sep
                           << cand << sep
                           << graph[cand].label << sep
                           << l << sep
                           << parent[cand] << sep
                           << graph[cand].position << sep
                           << pvals[2*i] << sep
                           << pvals[2*i+1] << std::endl;

          if ( sz_start != sz_end && sz_start != node.position && sz_end != node.position  && count > 1 ) {
            printf("level: %d, var: %lu, score: %f, thres: %f, start: %lu, end: %lu\n",
                   l, cand, scores[cand], thresholds[l], sz_start, sz_end);
            if (sz_start < 0) {
              printf("level: %d, var: %lu, score: %f, thres: %f, start: %lu, end: %lu\n",
                     l, cand, scores[cand], thresholds[l], sz_start, sz_end);
              exit(-1);
            }
          }
        }
      }
    }
  }
  gwasFile.close(); gwasFilteredFile.close(); gwasTulipFile.close(); // regionFile.close();
  std::cout << "writing to: " << gwas_outFile << std::endl;
  std::cout << "writing to: " << gwas_filtered_outFile << std::endl;
}

Cardinalities get_cardinalities( const Graph& graph ) {
  Cardinalities cards;
  for ( auto vp = boost::vertices(graph); vp.first != vp.second; ++vp.first ) {
    auto vertex = *vp.first;
    auto &node = graph[vertex];
    cards.push_back(node.variable.cardinality());
  }
  return cards;
}

Cardinalities get_cardinalities( const Graph& g, const Candidates& candidates ) {
  Cardinalities cards;
  for ( auto cand: candidates ) {
    cards.push_back(g[cand].variable.cardinality());
  }
  return cards;
}

std::vector<int> get_graph_parent( const samogwas::Graph& graph ) {
  std::vector<int> parent( boost::num_vertices(graph), -1 );
  for ( auto ei = boost::edges(graph); ei.first != ei.second; ++ei.first ) {
    auto source = boost::source(*ei.first, graph);
    auto target = boost::target(*ei.first, graph);
    parent[target] = source;
  }
  return parent;
}

////////////////////////////////////////////////////////
std::vector<double> read_thresholds(std::string filename) {
  std::ifstream thresholdFile(filename.c_str());
  if (!thresholdFile) {
    printf("file data %s not existing\n", filename.c_str());
    exit(-1);
  }

  samogwas::CSVIterator<double> thresholdLine(thresholdFile);
  //auto row = std::make_shared<Vec>(thresholdLine->size(), 0);
  std::vector<double> thresholds(thresholdLine->size(), 0.0);
  for (unsigned i = 0; i < thresholdLine->size(); ++i) {
    thresholds[i] = thresholdLine->at(i);
  }

  return thresholds;

}

/////////////////////////////////////////////////////////////////////////


Vars get_joint_vars(const std::string prefix, const int num_layer,
                    const int num_children, const int card ) {
  // 0 --> 2^(num_layer-1) - 2
  Vars vars;
  unsigned num_latent = pow(2, num_layer) - 1;
  for (size_t i = 0; i < num_latent; ++i) {
    vars ^= Var(prefix + "-latent-" + boost::lexical_cast<std::string>(i), plIntegerType(0, card-1));
  }

  int num_block = pow(2, num_layer-1);
  for (size_t i = num_latent; i < num_latent + num_block*num_children; ++i) {
    vars ^= Var(prefix + "-" + boost::lexical_cast<std::string>(i), plIntegerType(0, card-1));
  }

  return vars;
}

void add_tree(samogwas::Graph& g,
              const std::string prefix,
              const int num_layer,
              const int num_children,
              const int card)  {
  std::vector<size_t> node_ids((pow(num_children, num_layer+1)- 1)/2, 1);
  Vars vars;
  size_t id = 0;
  for (unsigned i = 0; i < num_layer; ++i) {
    for (int j = 0; j < pow(num_children, i); ++j) {
      auto node_id = boost::add_vertex(g);
      plSymbol var = plSymbol(prefix + boost::lexical_cast<std::string>(node_id),
                              plIntegerType(0, card-1));
      g[node_id].set_level(num_layer-1-i).set_index(node_id).set_variable(var);
      node_ids[id] = node_id;
      ++id;
      vars ^= var;
    }
  }
  id = 0;
  std::vector<double> pRoot {0.3}; for (int i = 1; i < card; ++i)
                                   { pRoot.push_back( (1.0-0.3)/(card-1)); }
  g[node_ids.front()].marginalDist = std::make_shared<plProbTable>(vars[0], pRoot);
  for (unsigned i = 1; i < num_layer; ++i) { // skip root
    for (int j = 0; j < pow(num_children, i); ++j) {
      ++id;
      size_t parent = (id-1)/num_children;
      boost::add_edge(node_ids[parent], node_ids[id], g);
      auto cndDist = std::make_shared<plDistributionTable>(vars[id], vars[parent]);
      for (size_t h = 0; h < card; ++h) {
        double base = 0.3*(id)/(id+2);
        std::vector<double> probs{base};
        for (int i = 1; i < card; ++i) { probs.push_back( (1.0 - base)/(card-1)); }
        cndDist->push( plProbTable(vars[id], probs), (int)h );
      }
      g[node_ids[parent]].cndChildrenDists.push_back(cndDist);
    }
  }

}

plJointDistribution generate_tree_net(const std::string prefix,
                                      const int num_layer,
                                      const int num_children,
                                      const int card) {

  auto vars = get_joint_vars(prefix, num_layer, num_children, card);
  plComputableObjectList dist;

  // root (marginal)
  std::vector<double> pRoot {0.3}; for (int i = 1; i < card; ++i) { pRoot.push_back( (1.0-0.3)/(card-1)); }
  const plProbTable PRoot(vars[0], pRoot);
  dist *= PRoot;
  // conditional
  unsigned num_latent = pow(2, num_layer) - 1, normal_latent = pow(2, num_layer-1) - 1;
  for (size_t latent = 0; latent < num_latent; ++latent) {
    plComputableObjectList cndProbTab;
    int offset = (latent-normal_latent)*num_children + pow(2, num_layer) - 1;
    size_t beg_child = (latent < normal_latent) ? (latent+1)*2 - 1 : offset;
    size_t end_child = (latent < normal_latent) ? (latent+1)*2 : offset + num_children - 1;
    for (size_t child = beg_child; child <= end_child; ++child ) {
      plDistributionTable cndDist(vars[child], vars[latent]);
      for (size_t h = 0; h < vars[latent].cardinality(); ++h) {
        double base = 0.3*(child)/(end_child+1);
        std::vector<double> probs{base};
        for (int i = 1; i < card; ++i) { probs.push_back( (1.0 - base)/(card-1)); }
        cndDist.push( plProbTable(vars[child], probs), (int)h );
      }
      dist *= cndDist;
    }
  }

  return plJointDistribution(vars, dist);
}

/**
 *
 */
void print_graph(const samogwas::Graph& g){

  std::vector<int> parent(boost::num_vertices(g), -1);
  for ( auto eit = boost::edges(g); eit.first != eit.second; ++eit.first) {
    std::cout << boost::source(*eit.first, g) << " -> "
              << boost::target(*eit.first, g) << std::endl;
  }

}

/**
 *
 */
std::vector<size_t> get_root_nodes(const samogwas::Graph& g){

  std::vector<int> parent(boost::num_vertices(g), -1);
  for ( auto eit = boost::edges(g); eit.first != eit.second; ++eit.first) {
    parent[boost::target(*eit.first, g)] = boost::source(*eit.first, g);
  }

  std::vector<size_t> roots;
  roots.reserve(boost::num_vertices(g));
  for (auto vp = boost::vertices(g); vp.first != vp.second; ++vp.first) {
    if (parent[*vp.first] == -1) {
      roots.push_back(*vp.first);
    }
  }
  roots.resize(roots.size());
  return roots;
}


// plVariablesConjunction get_obs_vars(size_t root,  const samogwas::Graph& g) {
//   Vars obs_vars;

//   std::deque<size_t> queue;
//   queue.push_back(root);
//   typename boost::graph_traits <samogwas::Graph>::out_edge_iterator ei, ei_end;
//   while(!queue.empty()) {
//     auto node_id = queue.front(); queue.pop_front(); //
//     const samogwas::Node& node = g[node_id];
//     if (!node.is_leaf()) {
//       for (boost::tie(ei, ei_end) = out_edges(node_id, g); ei != ei_end; ++ei) {
//         auto source = boost::source(*ei, g);
//         auto target = boost::target(*ei, g);
//         queue.push_back(target);
//       }
//     } else {
//       obs_vars ^= node.variable;
//     }
//   }


//   return obs_vars;
// }


std::pair<Vars, std::vector<size_t>> get_obs_vars(size_t root,  const samogwas::Graph& g) {
  Vars obs_vars;
  std::vector<size_t> var_indices;
  std::deque<size_t> queue;
  queue.push_back(root);
  typename boost::graph_traits <samogwas::Graph>::out_edge_iterator ei, ei_end;
  while(!queue.empty()) {
    auto node_id = queue.front(); queue.pop_front(); //
    const samogwas::Node& node = g[node_id];
    if (!node.is_leaf()) {
      for (boost::tie(ei, ei_end) = out_edges(node_id, g); ei != ei_end; ++ei) {
        auto source = boost::source(*ei, g);
        auto target = boost::target(*ei, g);
        queue.push_back(target);
      }
    } else {
      obs_vars ^= node.variable;
      var_indices.push_back(node.index);
    }
  }


  return std::make_pair(obs_vars, var_indices);

}


/**
 *
 */
std::vector<TreeCouple> get_separate_trees(const samogwas::Graph& g) {
  //plJunctionTree junc_tree;
  std::vector<TreeCouple> tree_dists;
  // iterate over all the vertices --> find the stand-alones
  auto roots = get_root_nodes(g);
  // for each stand alone --> build a Joint Dist
  
  int count = 0;
  for (auto root: roots) {
    //cout << "chappie-0-0\n";
     auto tree_dist = construct_tree_joint_dist(root, g);
    // cout << "chappie-0-1\n";
    // junc_tree.set_decomposition()
    plJunctionTree junc_tree(tree_dist.get_computable_object_list());
    // cout << "chappie-0-2\n";
    auto obs_vars = get_obs_vars(root, g);
    //  cout << "chappie-0-3\n";

    if (count==0) {
      //cout << tree_dist.get_computable_object_list() << std::endl;
      //  cout << obs_vars.first << std::endl << std::endl;
      count++;
    }
    //   cout << "chappie-0-4\n";
    auto evidence_dist = junc_tree.ask(obs_vars.first);
    tree_dists.push_back(std::make_pair(evidence_dist, obs_vars.second));

  }
  // tests
  return tree_dists;
}



/**
 */
plJointDistribution construct_tree_joint_dist(const size_t root, const samogwas::Graph& g) {
  //  cout << "construct_td_0                   \
  //cout << "chappie em chappie ut\n";
  // printf("---construt-td-0-%d---\n", root);
  plComputableObjectList jointDist;
  std::deque<size_t> queue;
  queue.push_back(root);
  typename boost::graph_traits <samogwas::Graph>::out_edge_iterator ei, ei_end;
  Vars vars;
  while(!queue.empty()) {
    auto node_id = queue.front(); queue.pop_front(); //
    const samogwas::Node& node = g[node_id];
    // cout << "ti-chuot-" << node_id  << " - " << node.variable << endl;
    vars ^= node.variable;

    if (node_id == root) {
      jointDist *= *node.marginalDist;
    }
    if (!node.is_leaf()) {
      for (boost::tie(ei, ei_end) = out_edges(node_id, g); ei != ei_end; ++ei) {
        auto source = boost::source(*ei, g);
        auto target = boost::target(*ei, g);
        queue.push_back(target);
      }
      for (auto cndDist: node.cndChildrenDists) {
        jointDist *= *cndDist;
      }
    }
  }
  // // cout << "construct_td_1\n";
      // cout << vars << endl;
  // cout << jointDist << endl;
  plJointDistribution dist(vars, jointDist);
  
  // cout << "construct_td_2\n";
  //  return plJointDistribution(vars, jointDist);
  //  cout << vars << endl;
  return dist;
}



double compute_prob(samogwas::Graph& g, const std::vector<std::shared_ptr<std::vector<int> > >& mat) {
  cout << "chapie-0\n";
  auto trees = get_separate_trees(g);
  double r = 0.0;
  for (auto& tree_jd: trees) {
    //cout << "chapie-[1]-" << mat.size() << endl;
    std::vector<int> vec;
    for (auto idx: tree_jd.second) {
      //cout << mat[idx]->size() << endl;
      //cout << idx << " " <<  mat[idx]->size() << " " << mat[idx]->at(0) << endl;
      vec.push_back(mat[idx]->at(0));
    }
    r += tree_jd.first.compute(vec);
  }

  return r;
}


double compute_prob(samogwas::Graph& g, const std::vector<std::vector<int>>& mat) {
  auto trees = get_separate_trees(g);
  double r = 0.0;
  // for (auto& tree_jd: trees) {
  //   std::vector<int> vec;
  //   for (auto idx: tree_jd.second) {
  //     vec.push_back(mat[idx][0]);
  //   }
  //   //  r += tree_jd.first.compute(vec);
  // }

  return r;
}





