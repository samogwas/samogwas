#define BOOST_TEST_DYN_LINK

#include <BoostTestTargetConfig.h>
#include <boost/test/unit_test.hpp>

#include <fstream>
#include <map>
#include <math.h>

#include <boost/test/unit_test.hpp>
#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>

#include "graph/graph.hpp"
#include "graph/graph_mut_info.hpp"
#include "graph/graph_io.hpp"

#include "test_graph.hpp"

#include "test_tools/dataload.hpp"

#include "fltm/latent_var_criteria.hpp"

using namespace samogwas;
using namespace samogwas_test;
namespace fs = boost::filesystem;

typedef plSymbol RandVar;

BOOST_AUTO_TEST_SUITE( Graph_Suite )

// inline Node& createLatentNode( std::shared_ptr<Graph> graph,
//                                const unsigned cardinality,
//                                const std::string label = "",
//                                const int position = -1, const int level = 1 ) {
//   assert(level >= 1);
//   return createNode(graph, cardinality, label, position, level );
// }


// BOOST_AUTO_TEST_CASE( Test_Node_Build ) {
//   const int CARD = 3, N = 1000;
//   std::vector<double> prob { 0.3, 0.5, 0.2 };
//   DataVecPtr dataVec = std::make_shared<DataVec>(N, 0);
//   for (int i = 0; i < N; ++i) {
//     if ( i < N*prob[0] ) {
//       (*dataVec)[i] = 0;
//     }
//     else if ( i < N*(prob[0]+prob[1]) ) {
//       (*dataVec)[i] = 1;
//     } else {
//       (*dataVec)[i] = 2;
//     }
//   }
//   Label2Index lab2Idx;
//   auto graph = std::make_shared<Graph>();
//   plSymbol var("node1", plIntegerType(0, CARD-1));
//   Node& node = createObsNode( graph, var, dataVec, 12, lab2Idx);

//   for ( int i = 0; i < CARD; ++i ) {
//     BOOST_CHECK_EQUAL(prob[i], node.compute_prob(i));
//   }
// }

// BOOST_AUTO_TEST_CASE( Test_Node_Entropy ) {
//   const int CARD = 4, N = 1000;
//   std::vector<int> dataVec {0,0,1,1,2,2,3};
//   auto graph = std::make_shared<Graph>();
//   Node node = createNode( graph, dataVec, CARD, "aNode", 12, 0);
//   ComputeNodeEntropy compute_node_entropy;
//   BOOST_CHECK_CLOSE( compute_node_entropy.compute(node), 1.351784, 0.00001);
// }

// BOOST_AUTO_TEST_CASE( Test_Node_Joint_Entropy ) {
//   const int CARD = 4, N = 1000;
//   std::vector<int> datA {0,0,0,1,2,2,3};
//   std::vector<int> datB {0,0,1,1,2,2,3};

//   auto graph = std::make_shared<Graph>();
//   Node nB = createNode( graph, datB, CARD, "B", 12, 0);
//   Node nA = createNode( graph, datA, CARD, "A", 12, 0);
//   ComputeNodeEntropy compute_node_entropy;

//   double jEntropy = ComputeNodeJointEntropy().compute_leaf_leaf(nA,nB,datA,datB);
//   BOOST_CHECK_CLOSE_FRACTION( compute_node_entropy.compute(nB), 1.351784, 0.00001);
//   BOOST_CHECK_CLOSE_FRACTION( compute_node_entropy.compute(nA), 1.277034, 0.00001);
//   BOOST_CHECK_CLOSE_FRACTION( jEntropy, 1.549826, 0.00001);
// }

// BOOST_AUTO_TEST_CASE( Test_Node_Parent_Child_Entropy ) {
//   const int CARD_X=3, CARD_Z=2;
//   std::vector<int> dat_X {0, 0, 0, 1, 1, 1, 1, 1, 2, 2};
//   std::vector<int> dat_Z {0, 0, 1, 1, 0, 0, 1, 1, 1, 0};

//   std::vector<std::vector<double>> cndProbTab { {.2, .2, .1}, {.1, .3, .1} };

//   std::vector<double> probX { .3, .5, .2 };
//   std::vector<double> probZ { .5, 0.5 };
//   auto graph = std::make_shared<Graph>();
//   std::vector<double> zGivenObs { 0.8,0.2, 0.8,0.2, 0.8,0.2, 0.8,0.2,
//         0.8,0.2, 0.8,0.2, 0.8,0.2, 0.8,0.2, 0.8,0.2 , 0.8,0.2 };

//   Label2Index lab2idx;
//   RandVar X("X", plIntegerType(0,CARD_X-1));
//   RandVar Z("Z", plIntegerType(0,CARD_Z-1));
//   RandVar J("J", plIntegerType(0,5));
//   std::vector<double> cndProb { .2, .2, .1, .1, .3, .1 };

//   DistPtr distX = std::make_shared<plProbTable>(X, probX);
//   DistPtr distZ = std::make_shared<plProbTable>(Z, probZ);
//   DistPtr distJ = std::make_shared<plProbTable>(J, cndProb);

//   plCndDistribution cndDist(X, Z, cndProb);
//   plComputableObjectList jointTable;
//   auto compList = (*distZ)*(cndDist);

//   JointDist jd(Z^X, compList);
//   Node nodeX = createObsNode( graph, X, 0, distX, lab2idx);
//   Node nodeZ = createLatentNode( graph, Z, jd, lab2idx);

//   ComputeNodeJointEntropy jEntropy;
//   double jE = jEntropy.compute_leaf_latent(nodeX,nodeZ,dat_X,zGivenObs);

//   BOOST_CHECK_CLOSE_FRACTION(1.69574, jE , 0.0001);
//   BOOST_CHECK_CLOSE(distJ->compute_shannon_entropy(),
//                     ComputeNodeJointEntropy::compute_from_joint_tab(cndProbTab), 0.0001);

// }

// ///////////////////////////////////////////////////////////////////

// inline double sum_joint_tab(std::vector<std::vector<double>>& t) {
//   double sum = 0.0;
//   for (auto& v: t) {
//     for (auto p: v) {
//       sum += p;
//     }
//   }
//   return sum;
// }

// void check_joint_tabs(const std::vector<std::vector<double>>&l,
//                       std::vector<std::vector<double>>& r) {
//   BOOST_CHECK_EQUAL(l.size(), r.size());
//   for (int i=0;i<l.size();++i) {
//     BOOST_CHECK_EQUAL( l[i].size(), r[i].size() );
//     for (int j=0;j<l[i].size();++j) {
//       BOOST_CHECK_CLOSE( l[i][j], r[i][j], 0.00001 );
//     }
//   }
// }

// BOOST_AUTO_TEST_CASE( Test_Create_Joint_Tab_Obs_Obs ) {
//   const int CARD = 2, N = 1000;

//   std::vector<int> datA {0,1,0, 1,1,1,0};
//   std::vector<int> datB {0,0,1, 1,0,0,1};

//   auto graph = std::make_shared<Graph>();
//   Node nB = createNode( graph, datB, CARD, "B", 12, 0);
//   Node nA = createNode( graph, datA, CARD, "A", 12, 0);

//   BOOST_CHECK(nA.is_leaf());
//   BOOST_CHECK(nB.is_leaf());

//   // auto jointTab = ComputeNodeJointEntropy::create_joint_tab_leaf_leaf(nA,nB,datA,datB);
//   // BOOST_CHECK_EQUAL(jointTab.size(), 2);
//   // BOOST_CHECK_EQUAL(jointTab[0].size(), 2);

//   // auto sum = sum_joint_tab(jointTab);

//   // BOOST_CHECK_EQUAL(sum, 1.0);
//   // std::vector<std::vector<double>> expected_tab {
//   //   {1.0/7, 2.0/7},{3.0/7, 1.0/7 }
//   // };
//   // for (int i=0;i<2;++i) {
//   //   for (int j=0;j<2;++j) {
//   //     BOOST_CHECK_EQUAL( jointTab[i][j], expected_tab[i][j] );
//   //   }
//   //

// }


// /////////////////////////////////////////////////////////////////////////////////
// BOOST_AUTO_TEST_CASE( Test_Create_Joint_Tab_Child_Parent ) {
//   const int CARD_X=3, CARD_Z=2;
//   std::vector<int> dat_X {0, 0, 0, 1, 1, 1, 1, 1, 2, 2};
//   std::vector<int> dat_Z {0, 0, 1, 1, 0, 0, 1, 1, 1, 0};

//   std::vector<double> cndProb { .2, .2, .1,   .1, .3, .1 };
//   std::vector<double> zGivenObs { 0.8,0.2, 0.8,0.2, 0.8,0.2,
//         0.8,0.2, 0.8,0.2, 0.8,0.2, 0.8,0.2, 0.8,0.2, 0.8,0.2 , 0.8,0.2 };

//   std::vector<double> probX { .3, .5, .2 };
//   std::vector<double> probZ { .5, 0.5 };
//   auto graph = std::make_shared<Graph>();

//   Label2Index lab2idx;
//   RandVar X("X", plIntegerType(0,CARD_X-1));
//   RandVar Z("Z", plIntegerType(0,CARD_Z-1));

//   DistPtr distX = std::make_shared<plProbTable>(X, probX);
//   DistPtr distZ = std::make_shared<plProbTable>(Z, probZ);
//   plCndDistribution cndDist(X, Z, cndProb);
//   plComputableObjectList jointTable;
//   auto compList = (*distZ)*(cndDist);
//   JointDist jd(Z^X, compList);

//   Node nodeX = createObsNode( graph, X, 0, distX, lab2idx);
//   Node nodeZ = createLatentNode( graph, Z, jd, lab2idx);

//   std::vector<std::vector<double>>  expected_tab{ {.2, .1}, {0.2, 0.3}, {0.1,0.1} };
//   auto jointTab = ComputeNodeJointEntropy::create_joint_table_leaf_latent(nodeX,nodeZ,dat_X, zGivenObs);
//   BOOST_CHECK(nodeZ.is_parent_of(nodeX));
//   double sum = 0.0;

//   for (auto& p: jointTab) {
//     sum += p;
//   }

//   BOOST_CHECK_CLOSE( dat_X.size(), sum, 0.0000001);
//   for (int i=0;i<3;++i) {
//     for (int j=0;j<2;++j) {
//       auto commonIdx = 3*i + j;
//       BOOST_CHECK_CLOSE_FRACTION( jointTab[commonIdx]/dat_X.size(), expected_tab[i][j], 0.01 );
//     }
//   }
// }

// BOOST_AUTO_TEST_CASE( Test_Create_Joint_Tab_Lat_Lat ) {
//   const int CARD_X=3, CARD_Y=3, CARD_A=2,CARD_B=2;
//   std::vector<int> dat_X {0, 0, 0, 1, 1, 1, 1, 1, 2, 2};
//   std::vector<int> dat_Y {0, 0, 0, 1, 1, 1, 1, 1, 2, 2};

//   std::vector<double> cndX_A{ .2, .2, .1,   .1, .3, .1 };
//   std::vector<double> cndY_B{ .2, .2, .1,   .1, .3, .1 };

//   std::vector<double> probX { .3, .5, .2 };
//   std::vector<double> probY { .3, .5, .2 };

//   std::vector<double> probA { .5, 0.5 };
//   std::vector<double> probB { .5, 0.5 };

//   auto graph = std::make_shared<Graph>();

//   Label2Index lab2idx;
//   RandVar X("X", plIntegerType(0,CARD_X-1));
//   RandVar Y("Y", plIntegerType(0,CARD_Y-1));

//   RandVar A("A", plIntegerType(0,CARD_A-1));
//   RandVar B("B", plIntegerType(0,CARD_B-1));

//   std::vector<double> cndA_Obs { 0.8,0.2, 0.8,0.2, 0.8,0.2,
//         0.8,0.2, 0.8,0.2, 0.8,0.2, 0.8,0.2, 0.8,0.2, 0.8,0.2 , 0.8,0.2 };
//   std::vector<double> cndB_Obs { 0.8,0.2, 0.8,0.2, 0.8,0.2,
//         0.8,0.2, 0.8,0.2, 0.8,0.2, 0.8,0.2, 0.8,0.2, 0.8,0.2 , 0.8,0.2 };

//   DistPtr distX = std::make_shared<plProbTable>(X, probX);
//   DistPtr distY = std::make_shared<plProbTable>(Y, probY);
//   DistPtr distA = std::make_shared<plProbTable>(A, probA);
//   DistPtr distB = std::make_shared<plProbTable>(B, probB);

//   plCndDistribution cnd_distX_A(X, A, cndX_A);
//   plCndDistribution cnd_distY_B(Y, B, cndY_B);

//   plComputableObjectList jointTable;
//   auto compList_xa = (*distA)*(cnd_distX_A);
//   JointDist jd_xa(A^X, compList_xa);

//   auto compList_yb = (*distB)*(cnd_distY_B);
//   JointDist jd_yb(B^Y, compList_yb);

//   Node nodeX = createObsNode( graph, X, 0, distX, lab2idx);
//   Node nodeY = createObsNode( graph, Y, 0, distY, lab2idx);

//   Node nodeA = createLatentNode( graph, A, jd_xa, lab2idx);
//   Node nodeB = createLatentNode( graph, B, jd_yb, lab2idx);

//   std::vector<std::vector<double>>  expected_tab{ {.2, .1}, {0.2, 0.3}, {0.1,0.1} };

//   // auto jointTab_XY = ComputeNodeJointEntropy::create_joint_tab_leaf_leaf(nodeX,nodeY,dat_X, dat_Y);
//   // auto jointTab_AB = ComputeNodeJointEntropy::create_joint_tab_latent_latent(nodeA,nodeB, cndA_Obs, cndB_Obs);
//   // auto jointTab_XA = ComputeNodeJointEntropy::create_joint_tab_leaf_latent(nodeX,nodeA,dat_X, cndA_Obs);
//   // auto jointTab_XB = ComputeNodeJointEntropy::create_joint_tab_leaf_latent(nodeX,nodeB,dat_X, cndB_Obs);
//   // auto jointTab_YA = ComputeNodeJointEntropy::create_joint_tab_leaf_latent(nodeY,nodeA,dat_Y, cndA_Obs);
//   // auto jointTab_YB = ComputeNodeJointEntropy::create_joint_tab_leaf_latent(nodeY,nodeB,dat_Y, cndB_Obs);


//   // // samogwas::JointEntropy<samogwas::EMP> jointEntropy;

//   // double ll_xy = ComputeNodeJointEntropy::compute_from_joint_tab( jointTab_XY );
//   // BOOST_CHECK_CLOSE_FRACTION( 1.02965, ll_xy, 0.00001);

//   // BOOST_CHECK_EQUAL(sum_joint_tab(jointTab_XY), 1.0);
//   // BOOST_CHECK_CLOSE(sum_joint_tab(jointTab_AB), 1.0, 0.00001);
//   // BOOST_CHECK_EQUAL(sum_joint_tab(jointTab_XA), 1.0);
//   // BOOST_CHECK_EQUAL(sum_joint_tab(jointTab_XB), 1.0);
//   // BOOST_CHECK_EQUAL(sum_joint_tab(jointTab_YA), 1.0);
//   // BOOST_CHECK_EQUAL(sum_joint_tab(jointTab_YB), 1.0);

//   // std::vector<std::vector<double>> expected_ab { {0.64, 0.16}, {0.16, 0.04} } ;
//   // check_joint_tabs( expected_ab, jointTab_AB);
// }

// //////////////////////////////////////////////////////////////
// BOOST_AUTO_TEST_CASE( Test_Node_Entropy_Obs_Obs_Indp ) {

//   auto mat = *loadDataTable("../tests/data/graph/obs_obs_ind.csv");
//   BOOST_CHECK_EQUAL(mat.size(), 2);
//   BOOST_CHECK_EQUAL(mat[0]->size(), 50000);

//   int CARD = 2;
//   auto graph = std::make_shared<Graph>();

//   plSymbol vA("A", plIntegerType(0, CARD-1)), vB("B", plIntegerType(0, CARD-1));
//   Label2Index lab2Idx;

//   Node nA = createObsNode( graph, vA, mat[0], 12, lab2Idx);
//   Node nB = createObsNode( graph, vB, mat[1], 12, lab2Idx);

//   ComputeNodeEntropy entropy; ComputeNodeJointEntropy jEntropy;  ComputeNodeMutInfo mut_info;
//   double eA = entropy.compute(nA);
//   double eB = entropy.compute(nB);

//   double mi = mut_info.compute(nA, nB);

//   BOOST_CHECK_CLOSE(eA, 0.6110167, 0.0001);
//   BOOST_CHECK_CLOSE(eB, 0.6726287, 0.0001);

//   double je = jEntropy.compute(nA,nB);
//   BOOST_CHECK_CLOSE(je, 1.283645,  0.0001);
// }


// BOOST_AUTO_TEST_CASE( Test_Node_Entropy_Obs_Obs_Inp ) {
//   auto mat = *loadDataTable("../tests/data/graph/obs_obs_dep.csv");
//   BOOST_CHECK_EQUAL(mat.size(), 2);
//   BOOST_CHECK_EQUAL(mat[0]->size(), 50000);

//   int CARD = 2;
//   auto graph = std::make_shared<Graph>();

//   plSymbol vA("A", plIntegerType(0, CARD-1)), vB("B", plIntegerType(0, CARD-1));
//   Label2Index lab2Idx;

//   Node nA = createObsNode( graph, vA, mat[0], 12, lab2Idx);
//   Node nB = createObsNode( graph, vB, mat[1], 12, lab2Idx);

//   ComputeNodeEntropy entropy; ComputeNodeJointEntropy jEntropy; ComputeNodeMutInfo mut_info;
//   double eA = entropy.compute(nA);
//   double eB = entropy.compute(nB);

//   BOOST_CHECK_CLOSE(eA, 0.6925289, 0.0001);
//   BOOST_CHECK_CLOSE(eB, 0.6722338, 0.0001);

//   double je = jEntropy.compute(nA,nB);
//   double mi = mut_info.compute(nA,nB);
//   BOOST_CHECK_CLOSE(je, 1.319324, 0.001);
//   BOOST_CHECK_CLOSE(mi, 0.04543878, 0.001);

// }

// // //############################ create a fake latent
// BOOST_AUTO_TEST_CASE( Test_Node_Entropy_Obs_Latent_Inp ) {
//   int N = 50000;
//   auto dat_X = *loadDataTable("../tests/data/graph/obs_latent_no_relation_X.csv");
//   auto dat_Y = *loadDataTable("../tests/data/graph/obs_latent_no_relation_X.csv");

//   BOOST_CHECK_EQUAL(dat_X.size(), 1);
//   BOOST_CHECK_EQUAL(dat_X[0]->size(), N);
//   auto dat_Z = *loadRealDataTable("../tests/data/graph/obs_latent_no_relation_Y.csv");
//   BOOST_CHECK_EQUAL(dat_Z.size(), 1);
//   BOOST_CHECK_EQUAL(dat_Z[0]->size(), N*2);

//   auto vec_X = dat_X[0];
//   auto vec_Y = dat_Y[0];
//   auto vec_Z = dat_Z[0];

//   Label2Index lab2idx;

//   int CARD = 2;
//   auto graph = std::make_shared<Graph>();
//   Label2Index lab2Idx;
//   plSymbol vX("X", plIntegerType(0, CARD-1)), vZ("Z", plIntegerType(0, CARD-1));
//   Node nX = createObsNode( graph, vX, vec_X, 12, lab2Idx);  
//   plSymbol vY("Y", plIntegerType(0, CARD-1));
//   Node nY = createObsNode( graph, vY, vec_Y, 12, lab2Idx);

//   // Node nZ =  createLatentNode( graph, CARD, "Z", 13, 1);  
//   // //nZ.cndObsDist = vec_Z;
//   // nZ. set_cnd_obs_vec(vec_Z);

//   // for (int i = 0; i < N;++i) {    
//   //   BOOST_CHECK_EQUAL( nX.compute_cond_prob_obs( 0, i ), vec_X->at(i) == 0);
//   //   BOOST_CHECK_EQUAL( nX.compute_cond_prob_obs( 1, i ), vec_X->at(i) == 1);
//   //   BOOST_CHECK_EQUAL( nZ.compute_cond_prob_obs( 0, i ), vec_Z->at(i*2) );
//   //   BOOST_CHECK_EQUAL( nZ.compute_cond_prob_obs( 1, i ), vec_Z->at(i*2+1) );
//   // }

//   // BOOST_CHECK(!nZ.is_parent_of(nX));
//   // ComputeNodeJointEntropy jointEntropy; ComputeNodeMutInfo mut_info;
//   // double je = jointEntropy.compute(nX, nZ);

//   // double mi = mut_info.compute(nX, nZ);
//   // BOOST_CHECK_CLOSE( je, 1.36554 , 0.001);
// }

// BOOST_AUTO_TEST_CASE( Test_Create_Joint_Tab_Child_Parent_2 ) {
//   const int CARD_X=3, CARD_Z=2;
//   auto vec_X = std::make_shared<std::vector<int>>(100,0);// {0, 0, 0, 1, 1, 1, 1, 1, 2, 2};
//   for (int i = 0; i < 100; ++i) {
//     if (i<36) vec_X->at(i) = 0;
//     else if (i<80) vec_X->at(i) = 1;
//     else vec_X->at(i) = 2;
//   }

//   std::vector<double> cndProb { .4, .4, .2,   .2, .6, 2 };
//   std::vector<double> z_GivenObs { 0.8,0.2, 0.8,0.2, 0.8,0.2, 0.8,0.2, 0.8,0.2,
//         0.8,0.2, 0.8,0.2, 0.8,0.2, 0.8,0.2 , 0.8,0.2 };
//   auto zGivenObs = std::make_shared< std::vector<double> > (z_GivenObs );
//   std::vector<double> probX { 0.36, 0.44, 0.2 };
//   std::vector<double> probZ { .8, 0.2 };
//   auto graph = std::make_shared<Graph>();

//   Label2Index lab2idx;
//   RandVar X("X1", plIntegerType(0,CARD_X-1));
//   RandVar Y("Y1", plIntegerType(0,CARD_X-1));
//   RandVar Z("Z1", plIntegerType(0,CARD_Z-1));

//   DistPtr distX = std::make_shared<plProbTable>(X, probX);
//   DistPtr distY = std::make_shared<plProbTable>(Y, probX);
//   DistPtr distZ = std::make_shared<plProbTable>(Z, probZ);

//   plCndDistribution cndDist_XZ(X, Z, cndProb);
//   plCndDistribution cndDist_YZ(Y, Z, cndProb);

//   plComputableObjectList jointTable;
//   auto compList = (*distZ)*(cndDist_XZ)*(cndDist_YZ);
//   JointDist jd(Z^X^Y, compList);

//   Node nodeX = createObsNode( graph, X, vec_X, 12, lab2idx);
//   Node nodeY = createObsNode( graph, Y, vec_X, 12, lab2idx);
//   Node nodeZ = createLatentNode( graph, Z, jd, lab2idx);
//   nodeZ.set_cnd_obs_vec(zGivenObs);

//   ComputeNodeMutInfo mut_info;
//   double mi_XZ = mut_info.compute(nodeX,nodeZ);
//   double mi_YZ = mut_info.compute(nodeY,nodeZ);
//   AverageMutInfo averageMutInfo;
//   double expected_avi = (mi_XZ+mi_YZ)/2.0;
// }

// ////////////////////////////////////////////////////////////////////////////

// BOOST_AUTO_TEST_CASE(Test_Save_Load_1) {
//   const int CARD_X=3, CARD_Z=2;
//   std::vector<int> dat_X {0, 0, 0, 1, 1, 1, 1, 1, 2, 2};
//   std::vector<int> dat_Z {0, 0, 1, 1, 0, 0, 1, 1, 1, 0};

//   std::vector<double> cndProb { .2, .2, .1, .1, .3, .1 };
//   std::vector<double> zGivenObs { 0.8,0.2, 0.8,0.2, 0.8,0.2,
//         0.8,0.2, 0.8,0.2, 0.8,0.2, 0.8,0.2, 0.8,0.2, 0.8,0.2 , 0.8,0.2 };

//   std::vector<double> probX { .3, .5, .2 };
//   std::vector<double> probZ { .5, 0.5 };
//   auto graph = std::make_shared<Graph>();

//   Label2Index lab2idx;
//   RandVar X("X", plIntegerType(0,CARD_X-1));
//   RandVar Z("Z", plIntegerType(0,CARD_Z-1));

//   lab2idx["X"] = 0; lab2idx["Z"] = 1;

//   DistPtr distX = std::make_shared<plProbTable>(X, probX);
//   DistPtr distZ = std::make_shared<plProbTable>(Z, probZ);
//   plCndDistribution cndDist(X, Z, cndProb);
//   plComputableObjectList jointTable;
//   auto compList = (*distZ)*(cndDist);
//   JointDist jd(Z^X, compList);

//   Node nodeX = createObsNode( graph, X, 0, distX, lab2idx);
//   Node nodeZ = createLatentNode( graph, Z, jd, lab2idx);
//   nodeZ.set_local_indexes( lab2idx );

//   BayesGraphSave graphSave;
//   auto graphRef = *graph;
//   graphSave( graphRef,
//              "../tests/data/graph/io/saved_graph_1_vertex.csv",
//              "../tests/data/graph/io/saved_graph_1_dist.csv" );

//   BayesGraphLoad graphLoad;
//   auto loadedGraph = std::make_shared<Graph>();
//   graphLoad( loadedGraph,
//              "../tests/data/graph/io/saved_graph_1_label.csv",
//              "../tests/data/graph/io/saved_graph_1_vertex.csv",
//              "../tests/data/graph/io/saved_graph_1_dist.csv",
//              "../tests/data/graph/io/saved_graph_1_cnd_data.csv",
//              "../tests/data/graph/io/saved_graph_1_data.csv" );

//   auto loadedGraphRef = *loadedGraph;
//   auto nX = loadedGraphRef[0], nZ = loadedGraphRef[1];

//   graphSave( loadedGraphRef,
//              "../tests/data/graph/io/saved_graph_1a_vertex.csv",
//              "../tests/data/graph/io/saved_graph_1a_dist.csv" );
// }


BOOST_AUTO_TEST_SUITE_END()
