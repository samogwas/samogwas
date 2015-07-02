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
Clustering compareMe(Clustering clusteringA, Clustering clusteringB) ;


int main() {
    auto labels = std::make_shared<LabelVec>(); auto positions = std::make_shared<PosVec>();  auto ids = std::make_shared<PosVec>();
    Label2Index lab2Idx;

    load_labels_positions( *labels, *ids, *positions, "/home/jules/SAMOGWAS/Data/chr2/lab.csv");


    printf("first SNP id = %d\n", ids->front());
    printf("last SNP id = %d\n", ids->back());
    printf("number of SNPs = %zu\n", ids->size());

    auto mat = load_data_table("/home/jules/SAMOGWAS/Data/chr2/dat.csv");
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

        auto dissimi = std::make_shared<GraphMutInfoDissimilarity>(graph, l2g);
        dissimi->set_criteria(criteria);
        DBSCAN dbscan(dissimi,2,0.38);
        result = dbscan();




        Clustering clusteringDBSCAN = result.to_clustering();
//        info = getClusteringInformations(clusteringDBSCAN,dissimi);

//        std::cout<< "\tnbCluster = " << clusteringDBSCAN.size()
//                 << "nbSingletons = " << info.nbSingletons
//                 << "\tclusterScore = "<< info.clusterScore
//                 << "\tclusterScore = "<< info.mutualInformation
//                 << std::endl;

        end = get_time::now();
        diff = (end-start);
        start = end;
        //we print the duration of the clustering
        std::cout << diff.count()/1000000000 << " secs\n";

    std::cout << "[";
      for (auto i: clusteringCAST) {
        std::cout << "[";
        for (auto j: i)
            std::cout << j << ',';
        std::cout << "],";
      }
    std::cout << "]"<< std::endl<< std::endl;

    std::cout << "[";
      for (auto i: clusteringDBSCAN) {
        std::cout << "[";
        for (auto j: i)
            std::cout << j << ',';
        std::cout << "],";
      }
    std::cout << "]"<< std::endl<< std::endl;
        Clustering clusteringRES;

        clusteringRES = compareMe(clusteringCAST, clusteringDBSCAN);
        std::cout << "[";
          for (auto i: clusteringRES) {
            std::cout << "[";
            for (auto j: i)
                std::cout << j << ',';
            std::cout << "],";
          }
        std::cout << "]"<< std::endl;

        end = get_time::now();
        diff = (end-start);
        //we print the duration of the clustering
        std::cout << diff.count()/1000000000 << " secs\n";

//        auto end = get_time::now();
//        auto diff = (end-start);
        //we print the duration of the clustering
//        std::cout << diff.count()/1000000000 << " secs\n";


//    }


//    for (ClusterInformation info : clusterVector) {
//        std::cout<< "nbSingletons = " << info.nbSingletons << std::endl<< " clusterScore = "<< info.clusterScore<< std::endl;
//    }

//        writeResultsForTulip(clusteringDBSCAN, positions, ids, labels, "DBSCAN");
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


Clustering compareMe(Clustering clusteringA, Clustering clusteringB) {
    std::vector<Cluster>::iterator itA = clusteringA.begin() ;
    std::vector<Cluster>::iterator itB = clusteringB.begin() ;
    Clustering res;
    int firstA, lastA, firstB, lastB;
//    int res =0;
//    while (itA != clusteringA.end() && itB != clusteringB.end()) {
//        if (*itA->begin() > *itB->begin()) {
//            itB++;
//        } else if (*itA->begin() < *itB->begin()) {
//            itA++;
//        } else {
//            if (itA->size() == itB->size()) {
//                bool b = true;
//                for (int i = 0 ; i < itA->size() ; i++) {
//                    if ((*itA)[i] != (*itB)[i]) {
//                        b = false;
//                        break;
//                    }
//                }
//                if (b) {
//                    res++;
//                } else {
//                    for (auto j: *itA)
//                      std::cout << j << ' ';
//                    std::cout<<std::endl;
//                    for (auto j: *itB)
//                      std::cout << j << ' ';
//                    std::cout<<std::endl;
//                }
//            } else {
//                for (auto j: *itA)
//                  std::cout << j << ' ';
//                std::cout<<std::endl;
//                for (auto j: *itB)
//                  std::cout << j << ' ';
//                std::cout<<std::endl;
//            }
//            itA++;
//            itB++;
//        }
//    }
    std::vector<Index>::iterator it;
    Cluster intersectCluster(50);
    while (itA != clusteringA.end() && itB != clusteringB.end()) {
        firstA = itA->front();
        lastA  = itA->back();
        firstB = itB->front();
        lastB  = itB->back();
        if (lastA== firstA || lastA <= firstB) { //if A contains 1 element or if the last element of A is lowest than the biggest element of B
            itA++;
        } else if (lastB== firstB || lastB <= firstA) { //idem with B
            itB++;
        } else { //otherwise A and B can share elements
            //we find the intersection between A and B
            it=std::set_intersection (itA->begin(), itA->end(), itB->begin(), itB->end(), intersectCluster.begin());
            intersectCluster.resize(it-intersectCluster.begin());

            it=std::set_difference (itA->begin(), itA->end(), intersectCluster.begin(), intersectCluster.end(), itA->begin());
            if (it == itA->begin()) {
                itA++;
            } else {
                itA->resize(it-itA->begin());
            }

            it=std::set_difference (itB->begin(), itB->end(), intersectCluster.begin(), intersectCluster.end(), itB->begin());
            if (it == itB->begin()) {
                itB++;
            } else {
                itB->resize(it-itB->begin());
            }
            if (intersectCluster.size() > 1)
                res.push_back(intersectCluster);
            if (intersectCluster.empty())
                (firstA < firstB) ? itA->erase(itA->begin()) : itB->erase(itB->begin());
            intersectCluster.resize(50);

            if (std::next(itA) != clusteringA.end() && std::next(itA)->front() < itA->front()) {
                itA->swap(*std::next(itA));
            }
            if (std::next(itB) != clusteringB.end() && std::next(itB)->front() < itB->front()) {
                itB->swap(*std::next(itB));
            }
        }
    }
    return res;

}
