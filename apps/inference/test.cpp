
// #include <pl.h>

// #include <vector>
// #include <map>
// #include <memory>
// #include <string>
// #include <cmath>

// #include <boost/lexical_cast.hpp>

// #include <graph/graph.hpp>

// using namespace std;


// typedef plJointDistribution JointDist;
// typedef plVariablesConjunction Vars;
// typedef plSymbol Var;


// plJointDistribution generate_tree_net( const std::string prefix, const int num_layer,
//                                        const int num_children, const int card = 3);
// Vars get_joint_vars(const std::string prefix, const int num_layer,
//                     const int num_children, const int card = 3);

// plJointDistribution construct_tree_joint_dist(const size_t root,
//                                               const samogwas::Graph& g);

// plVariablesConjunction get_obs_vars(size_t root,  const samogwas::Graph& g);


// std::vector<plJointDistribution> get_sparate_trees(const samogwas::Graph& g);

// double compute_prob(samogwas::Graph& g);
// std::vector<size_t> get_root_nodes(const samogwas::Graph& g);

// void add_tree(samogwas::Graph& g,
//               const std::string prefix,
//               const int num_layer,
//               const int num_children,
//               const int card);


// void print_graph(const samogwas::Graph& g);

// // /**
// //  *
// //  */
// // int main() {
  

// // }



// Vars get_joint_vars(const std::string prefix, const int num_layer,
//                     const int num_children, const int card ) {
//   // 0 --> 2^(num_layer-1) - 2
//   Vars vars;
//   unsigned num_latent = pow(2, num_layer) - 1;
//   for (size_t i = 0; i < num_latent; ++i) {
//     vars ^= Var(prefix + "-latent-" + boost::lexical_cast<std::string>(i), plIntegerType(0, card-1));
//   }

//   int num_block = pow(2, num_layer-1);
//   for (size_t i = num_latent; i < num_latent + num_block*num_children; ++i) {
//     vars ^= Var(prefix + "-" + boost::lexical_cast<std::string>(i), plIntegerType(0, card-1));
//   }

//   return vars;
// }

// void add_tree(samogwas::Graph& g,
//               const std::string prefix,
//               const int num_layer,
//               const int num_children,
//               const int card)  {
//   std::vector<size_t> node_ids((pow(num_children, num_layer+1)- 1)/2, 1);
//   Vars vars;
//   size_t id = 0;
//   for (unsigned i = 0; i < num_layer; ++i) {
//     for (int j = 0; j < pow(num_children, i); ++j) {
//       auto node_id = boost::add_vertex(g);
//       plSymbol var = plSymbol(prefix + boost::lexical_cast<std::string>(node_id),
//                               plIntegerType(0, card-1));
//       g[node_id].set_level(num_layer-1-i).set_index(node_id).set_variable(var);
//       node_ids[id] = node_id;
//       ++id;
//       vars ^= var;
//     }
//   }
//   id = 0;
//   std::vector<double> pRoot {0.3}; for (int i = 1; i < card; ++i)
//                                    { pRoot.push_back( (1.0-0.3)/(card-1)); }
//   g[node_ids.front()].marginalDist = std::make_shared<plProbTable>(vars[0], pRoot);
//   for (unsigned i = 1; i < num_layer; ++i) { // skip root
//     for (int j = 0; j < pow(num_children, i); ++j) {
//       ++id;
//       size_t parent = (id-1)/num_children;
//       boost::add_edge(node_ids[parent], node_ids[id], g);
//       auto cndDist = std::make_shared<plDistributionTable>(vars[id], vars[parent]);
//       for (size_t h = 0; h < card; ++h) {
//         double base = 0.3*(id)/(id+2);
//         std::vector<double> probs{base};
//         for (int i = 1; i < card; ++i) { probs.push_back( (1.0 - base)/(card-1)); }
//         cndDist->push( plProbTable(vars[id], probs), (int)h );
//       }
//       g[node_ids[parent]].cndChildrenDists.push_back(cndDist);
//     }
//   }

// }

// plJointDistribution generate_tree_net(const std::string prefix,
//                                       const int num_layer,
//                                       const int num_children,
//                                       const int card) {

//   auto vars = get_joint_vars(prefix, num_layer, num_children, card);
//   plComputableObjectList dist;

//   // root (marginal)
//   std::vector<double> pRoot {0.3}; for (int i = 1; i < card; ++i) { pRoot.push_back( (1.0-0.3)/(card-1)); }
//   const plProbTable PRoot(vars[0], pRoot);
//   dist *= PRoot;
//   // conditional
//   unsigned num_latent = pow(2, num_layer) - 1, normal_latent = pow(2, num_layer-1) - 1;
//   for (size_t latent = 0; latent < num_latent; ++latent) {
//     plComputableObjectList cndProbTab;
//     int offset = (latent-normal_latent)*num_children + pow(2, num_layer) - 1;
//     size_t beg_child = (latent < normal_latent) ? (latent+1)*2 - 1 : offset;
//     size_t end_child = (latent < normal_latent) ? (latent+1)*2 : offset + num_children - 1;
//     for (size_t child = beg_child; child <= end_child; ++child ) {
//       plDistributionTable cndDist(vars[child], vars[latent]);
//       for (size_t h = 0; h < vars[latent].cardinality(); ++h) {
//         double base = 0.3*(child)/(end_child+1);
//         std::vector<double> probs{base};
//         for (int i = 1; i < card; ++i) { probs.push_back( (1.0 - base)/(card-1)); }
//         cndDist.push( plProbTable(vars[child], probs), (int)h );
//       }
//       dist *= cndDist;
//     }
//   }

//   return plJointDistribution(vars, dist);
// }

// /**
//  *
//  */
// void print_graph(const samogwas::Graph& g){

//   std::vector<int> parent(boost::num_vertices(g), -1);
//   for ( auto eit = boost::edges(g); eit.first != eit.second; ++eit.first) {
//     std::cout << boost::source(*eit.first, g) << " -> "
//               << boost::target(*eit.first, g) << std::endl;
//   }

// }

// /**
//  *
//  */
// std::vector<size_t> get_root_nodes(const samogwas::Graph& g){

//   std::vector<int> parent(boost::num_vertices(g), -1);
//   for ( auto eit = boost::edges(g); eit.first != eit.second; ++eit.first) {
//     parent[boost::target(*eit.first, g)] = boost::source(*eit.first, g);
//   }

//   std::vector<size_t> roots;
//   roots.reserve(boost::num_vertices(g));
//   for (auto vp = boost::vertices(g); vp.first != vp.second; ++vp.first) {
//     if (parent[*vp.first] == -1) {
//       roots.push_back(*vp.first);
//     }
//   }
//   roots.resize(roots.size());
//   return roots;
// }


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

// /**
//  *
//  */
// std::vector<plJointDistribution> get_separate_trees(const samogwas::Graph& g) {
//   std::vector<plJointDistribution> tree_dists;
//   // iterate over all the vertices --> find the stand-alones
//   auto roots = get_root_nodes(g);
//   // for each stand alone --> build a Joint Dist
//   for (auto root: roots) {
//     auto tree_dist = construct_tree_joint_dist(root, g);
//     plJunctionTree junc_tree(tree_dist.get_computable_object_list());
//     Vars evidence_vars;
//     for (auto vp = boost::vertices(g); vp.first != vp.second; ++vp.first) {
//       if (g[*vp.first].is_leaf()) {
//        evidence_vars ^= g[*vp.first].variable;
//       }
//     }
//     auto evidence_dist = junc_tree.ask(evidence_vars);
//   }
//   // tests
//   return tree_dists;
// }

// /**
//  */
// plJointDistribution construct_tree_joint_dist(const size_t root, const samogwas::Graph& g) {
//   plComputableObjectList jointDist;
//   std::deque<size_t> queue;
//   queue.push_back(root);
//   typename boost::graph_traits <samogwas::Graph>::out_edge_iterator ei, ei_end;
//   Vars vars;
//   while(!queue.empty()) {
//     auto node_id = queue.front(); queue.pop_front(); //
//     const samogwas::Node& node = g[node_id];
//     vars ^= node.variable;
//     if (!node.is_leaf()) {
//       for (boost::tie(ei, ei_end) = out_edges(node_id, g); ei != ei_end; ++ei) {
//         auto source = boost::source(*ei, g);
//         auto target = boost::target(*ei, g);
//         queue.push_back(target);
//       }
//       if (node_id == root) {
//         jointDist *= *node.marginalDist;
//       }
//       for (auto cndDist: node.cndChildrenDists) {
//         jointDist *= *cndDist;
//       }
//     }
//   }

//   plJointDistribution dist(vars, jointDist);
//   //  return plJointDistribution(vars, jointDist);
//   cout << vars << endl;
//   return dist;
// }

// // double compute_prob(samogwas::Graph& g) {
// //   plJunctionTree junction_tree;
// //   auto trees = get_separate_trees(g);

// //   double r = -0.0;
// //   for (auto& tree_jd: trees) {
// //     junction_tree.set_decomposition(tree_jd);
// //     r += junc_tree.ask()
// //         }

// // }
