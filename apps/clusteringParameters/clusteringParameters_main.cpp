// /****************************************************************************************
//  * File: clusteringParameters_main.cpp
//  * Description: 
//  * @author: jules
//  * @date: 09/07/2014
//  ***************************************************************************************/

#include <iostream>
#include <thread>
#include <chrono>
#include <memory>
#include <algorithm>

#include <fstream>
#include <cstdio>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp> // to obtain the program's name

#include "graph/graph.hpp"
#include "em/multi_em.hpp"
#include "fltm/card_func.hpp"
#include "distance/simi.hpp"
#include "distance/diss.hpp"
#include "clustering/clustering.hpp"
#include "clustering/dbscan.hpp"
#include "clustering/cast.hpp"
#include "clustering/louvain/louv.hpp"

#include "dataload.hpp"
#include "algo_properties.hpp"
#include "application_options.hpp"
#include "clusteringParameters_main.hpp"


using namespace samogwas;

struct ClusterInformation {
  int nbSingletons = 0;
  int clusterScore = 0;
  double mutualInformation = 0;


};
using ms = std::chrono::milliseconds;
using get_time = std::chrono::steady_clock ;
typedef std::shared_ptr<PositionCriteria> PositionCriteriaPtr;

void writeResultsForTulip( Clustering clustering, std::shared_ptr<PosVec> positions, std::shared_ptr<PosVec> ids, std::shared_ptr<LabelVec> labels, std::string clusteringMethod);
void printClusteringInformations( Clustering clustering );
ClusterInformation getClusteringInformations( Clustering clustering, SimiPtr diss );
ClusterInformation getClusteringInformations( Clustering clustering, DissPtr diss );
Clustering compareMe(std::vector<Clustering> clusterings);
double getObservedEntropy( Cluster cluster, PtrMatrixPtr mat);
double getExpectedEntropy( Cluster cluster, PtrMatrixPtr mat);

int main() {
    auto labels = std::make_shared<LabelVec>(); auto positions = std::make_shared<PosVec>();  auto ids = std::make_shared<PosVec>();
    Label2Index lab2Idx;

    load_labels_positions( *labels, *ids, *positions, "/home/jules/SAMOGWAS/Data/chr2/lab_300.csv");


    printf("first SNP id = %d\n", ids->front());
    printf("last SNP id = %d\n", ids->back());
    printf("number of SNPs = %zu\n", ids->size());

    auto mat = load_data_table("/home/jules/SAMOGWAS/Data/chr2/dat_300.csv");
    auto l2g = init_index_mapping( mat->size() );
    auto graph = init_graph( *mat,  lab2Idx, 3, *labels, *positions );

    auto criteria = std::make_shared<PositionCriteria>( positions, 50000 );


//    std::vector<ClusterInformation> clusterVector;
    ClusterInformation info;

//    for (double epsi = 0.38 ; epsi <= 0.6 ;epsi +=1) {
//        printf("\n*******************************************************************\nMinPt = 2, epsilon = %f\n\n",epsi);

        auto start = get_time::now();
        auto simi = std::make_shared<GraphMutInfoSimilarity>(graph, l2g);
        simi->set_criteria(criteria);
        CAST cast( simi, 0.6 );
        Partition result = cast();


        Clustering clusteringCAST = result.to_clustering();
//        info = getClusteringInformations(clusteringCAST,simi);

//        std::cout<< "nbCluster = " << clusteringCAST.size()
//                 << "\tnbSingletons = " << info.nbSingletons
//                 << "\tclusterScore = "<< info.clusterScore
//                 << "\tclusterScore = "<< info.mutualInformation
//                 << std::endl;

//        writeResultsForTulip(clusteringCAST, positions, ids, labels, "CAST");

        auto end = get_time::now();
        auto diff = (end-start);
        start = end;
        //we print the duration of the clustering
        std::cout << diff.count()/1000000000 << " secs\n";



//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

   /*     auto dissimi = std::make_shared<GraphMutInfoDissimilarity>(graph, l2g);
        dissimi->set_criteria(criteria);
        DBSCAN dbscan(dissimi,2,0.38);
        result = dbscan();




        Clustering clusteringDBSCAN = result.to_clustering();


        end = get_time::now();
        diff = (end-start);
        start = end;
        //we print the duration of the clustering
        std::cout << diff.count()/1000000000 << " secs\n";

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



        auto simi2 = std::make_shared<GraphMutInfoSimilarity>(graph, l2g);
        simi2->set_criteria(criteria);

        louvain::MethodLouvain louv(simi2);
        result = louv();




        Clustering clusteringLOUV = result.to_clustering();

        end = get_time::now();
        diff = (end-start);
        start = end;
        //we print the duration of the clustering
        std::cout << diff.count()/1000000000 << " secs\n";

*/
        Clustering clusteringRES;
//        Clustering , clusteringCAST, clusteringDBSCAN;
//        Cluster c;
//        c.push_back(0);
//        c.push_back(4);
//        c.push_back(7);
//        clusteringCAST.push_back(c);
//        c.clear();
//        c.push_back(0);
//        clusteringDBSCAN.push_back(c);
//        c.clear();
//        c.push_back(1);
//        clusteringDBSCAN.push_back(c);
//        c.clear();
//        c.push_back(1);
//        c.push_back(5);
//        c.push_back(6);
//        clusteringCAST.push_back(c);
//        c.clear();
//        c.push_back(2);
//        c.push_back(3);
//        clusteringCAST.push_back(c);
//        clusteringDBSCAN.push_back(c);
//        c.clear();
//        c.push_back(4);
//        c.push_back(5);
//        c.push_back(6);
//        c.push_back(7);
//        clusteringDBSCAN.push_back(c);

         std::cout << "CAST[";
          for (auto i: clusteringCAST) {
            std::cout << "[";
            for (auto j: i)
                std::cout << j << ',';
            std::cout << "],";
          }
        std::cout << "]"<< std::endl<< std::endl;
/*        std::cout << "DBSCAN[";
          for (auto i: clusteringDBSCAN) {
            std::cout << "[";
            for (auto j: i)
                std::cout << j << ',';
            std::cout << "],";
          }
        std::cout << "]"<< std::endl<< std::endl;

        std::cout << "LOUV[";
          for (auto i: clusteringLOUV) {
            std::cout << "[";
            for (auto j: i)
                std::cout << j << ',';
            std::cout << "],";
          }
        std::cout << "]"<< std::endl<< std::endl;

        */
     /*   std::vector<Clustering> clusterings;
        clusterings.push_back(clusteringCAST);
        clusterings.push_back(clusteringDBSCAN);
        clusterings.push_back(clusteringLOUV);
        clusteringRES = compareMe(clusterings);
        std::cout << "RESU[";
          for (auto i: clusteringRES) {
            std::cout << "[";
            for (auto j: i)
                std::cout << j << ',';
            std::cout << "],";
          }
        std::cout << "]"<< std::endl;*/
        clusteringCAST.begin()->push_back(45);
        clusteringCAST.begin()->push_back(88);
         clusteringCAST.begin()->push_back(108);
          clusteringCAST.begin()->push_back(208);
           clusteringCAST.begin()->push_back(54);

double obs = getObservedEntropy(*clusteringCAST.begin(), mat);
double exp = getExpectedEntropy(*clusteringCAST.begin(), mat);
double epsilon = (exp - obs) /exp;
std::cout << obs <<std::endl;
std::cout << exp <<std::endl;
std::cout << epsilon <<std::endl;
//    for (ClusterInformation info : clusterVector) {
//        std::cout<< "nbSingletons = " << info.nbSingletons << std::endl<< " clusterScore = "<< info.clusterScore<< std::endl;
//    }

//        writeResultsForTulip(clusteringDBSCAN, positions, ids, labels, "DBSCAN");
}

double getObservedEntropy( Cluster cluster, PtrMatrixPtr mat){
    std::map<VecT, int> cluster2Occurrence;

    for (int i = 0 ; i < mat->at(0)->size() ; ++i) {
        VecT newVec;
        for (int label : cluster) {
            newVec.push_back(mat->at(label)->at(i));
        }
        if (cluster2Occurrence.count(newVec) > 0)
            cluster2Occurrence[newVec]++;
        else
            cluster2Occurrence[newVec] = 1;
    }

//    CondObsDistPtr cndObsDist = std::make_shared<CondObsDist>();
//    for (auto a : cluster2Occurrence) {
//        cndObsDist->push_back(((double) a.second)/mat->size());
//    }
//    DistPtr dist = create_emp_distribution(p,*cndObsDist);
//    return dist->compute_shannon_entropy();

    double res = 0;
    for (auto a : cluster2Occurrence) {
        std::cout << std::endl << a.second << " -> ";
        for (int j : a.first) {
            std::cout << j << ",";
        }
        double x = ((double) a.second)/mat->at(0)->size();
        res += -x * log2(x);
    }
    std::cout << std::endl;
    return res;
}

double getExpectedEntropy( Cluster cluster, PtrMatrixPtr mat){
    int distribution[cluster.size()][3] = {0};
    int nbObs = mat->at(0)->size();
    std::vector<double> resVec(nbObs,1);
    for (int j = 0 ; j < cluster.size() ; j++) {
        for (int i = 0 ; i < nbObs ; ++i) {
            distribution[j][mat->at(cluster[j])->at(i)]++;
        }
        for (int i = 0 ; i < nbObs ; ++i) {
            resVec[i] *= ((double) distribution[j][mat->at(cluster[j])->at(i)]) / nbObs;
        }
    }
    for (int i = 0 ; i < 3 ;i++) {
        for (int j =0 ;j < cluster.size() ; j++)
            std::cout << distribution[j][i] << "\t";
        std::cout << std::endl;
    }


    double res =0;
    for (double x : resVec) {
//        if (std::abs(x - 0.903654) >0.001)
//            std::cout << x << ",";
        res += -x * log2(x);
    }
    return res;
}


ClusterInformation getClusteringInformations( Clustering clustering, SimiPtr diss ){
    ClusterInformation info;

    for (std::vector<Cluster>::iterator it = clustering.begin() ; it != clustering.end() ; ++it) {
        if (it->size() < 1)
            printf("bizarre, un cluster vide..");
        else if (it->size() == 1)
             info.nbSingletons++;
        else {
            for (std::vector<Index>::iterator itA = it->begin() ; itA != it->end() ; ++itA) {

                for (std::vector<Index>::iterator itB = std::next(itA) ; itB != it->end() ; ++itB) {
                    info.mutualInformation += diss->compute(*itA,*itB);
                }
            }
            if (it->size() > 20) {
                info.clusterScore += (it->size() - 20) * (it->size() - 20);
                std::cout << it->size() << " ";
            }
        }
    }
    std::cout<<std::endl;
    return info;
}

ClusterInformation getClusteringInformations( Clustering clustering, DissPtr diss ){
    ClusterInformation info;

    for (std::vector<Cluster>::iterator it = clustering.begin() ; it != clustering.end() ; ++it) {
        if (it->size() < 1)
            printf("bizarre, un cluster vide..");
        else if (it->size() == 1)
             info.nbSingletons++;
        else {
            for (std::vector<Index>::iterator itA = it->begin() ; itA != it->end() ; ++itA) {
                for (std::vector<Index>::iterator itB = std::next(itA) ; itB != it->end() ; ++itB) {
                    info.mutualInformation += diss->compute(*itA,*itB);
                }
            }
            if (it->size() > 20) {
                info.clusterScore += (it->size() - 20) * (it->size() - 20);
                std::cout << it->size() << " ";
            }
        }
    }
    std::cout<<std::endl;
    return info;
}


void printClusteringInformations( Clustering clustering ){
    int singletons = 0;
    int clustersBiggerThan15 = 0;
    int clusters = 0;
    int snpInBigCluster = 0;

    for (std::vector<Cluster>::iterator it = clustering.begin() ; it != clustering.end() ; ++it) {
        if (it->size() < 1)
            printf("bizarre, un cluster vide..");
        else if (it->size() == 1)
             singletons++;
        else {
            clusters++;
            if (it->size() > 15) {
                clustersBiggerThan15++;
                printf("%zu ", it->size());
                snpInBigCluster += it->size();
            }
        }
    }
    printf("\nnumber of clusters : %d\n", clusters);
    printf("number of singletons : %d\n", singletons);
    printf("number of clusters bigger than 15 SNPs : %d\n", clustersBiggerThan15);
    printf("number of SNPs in a cluster bigger than 15 SNPs : %d\n", snpInBigCluster);
    printf("=> number of SNPs not so good : %d\n", singletons + snpInBigCluster);
}

void writeResultsForTulip( Clustering clustering, std::shared_ptr<PosVec> positions, std::shared_ptr<PosVec> ids, std::shared_ptr<LabelVec> labels, std::string clusteringMethod) {

    //Csv files used for Tulip vizualisation
    FILE *nodes, *edges;
    nodes = fopen ((clusteringMethod + "_nodes.csv").c_str(),"w");
    edges = fopen ((clusteringMethod + "_edges.csv").c_str(),"w");
    fprintf (nodes, "id,level,position,label\n");
    fprintf (edges, "id,parent_id\n");

    printf("\nbeginning writing in nodes.csv and edges.csv...\n");
    //initialization with the SNPs
    for (int i = 0 ; i < ids->size() ; i++)
        fprintf (nodes, "%d,0,%d,%s\n", ids->at(i), positions->at(i), labels->at(i).c_str());

    //the next id which is available
    int availableId = ids->back() + 1;

    //we iterate the clustering
    for (std::vector<Cluster>::iterator it = clustering.begin() ; it != clustering.end() ; ++it) {
        if (it->size() > 1){
            //we calculate the position of the latent variable according to its SNPs
            int positionsMean = 0;
            for (std::vector<Index>::iterator it2 = it->begin() ; it2 != it->end() ; ++it2) {
                positionsMean += positions->at(*it2)/it->size();
                fprintf(edges, "%d,%d\n",*it2 + ids->front(),availableId);
            }
            fprintf (nodes, "%d,1,%d,latent_variable\n", availableId++, positionsMean);
        }
    }
    printf("done writing.\n");
}


Clustering compareMe(std::vector<Clustering> clusterings) {
    Cluster intersectedCluster;
    std::map<Index, std::vector<Index>> indexToCluster;
    Clustering res;
    //create for each clustering, for each index, the list of the associated clusters
    for (Clustering clustering : clusterings)
        for (int i = 0 ; i < clustering.size() ; i++)
            for (Index clusterIndex : clustering[i])
                indexToCluster[clusterIndex].push_back(i);
    Cluster::iterator it;
    //we run all the index in the map
    while (!indexToCluster.empty()) {
        //we initialize the intersected cluster with the first clustering
        intersectedCluster = clusterings[0][indexToCluster.begin()->second[0]];
        //we find the intersection between all the clusterings
        for (int i = 1 ; i < clusterings.size() ; i++) {
            Cluster currentCluster = clusterings[i][indexToCluster.begin()->second[i]];
            it=std::set_intersection (currentCluster.begin(), currentCluster.end(), intersectedCluster.begin(), intersectedCluster.end(), intersectedCluster.begin());
            intersectedCluster.resize(it-intersectedCluster.begin());
        }
        //we add the intersected cluster and we remove it from the map
        res.push_back(intersectedCluster);
        for (auto j : intersectedCluster) {
            indexToCluster.erase(j);
        }
        intersectedCluster.resize(50);
    }
    return res;
}
