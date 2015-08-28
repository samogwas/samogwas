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


using ms = std::chrono::milliseconds;
using get_time = std::chrono::steady_clock ;
typedef std::shared_ptr<PositionCriteria> PositionCriteriaPtr;



int main() {
    auto labels = std::make_shared<LabelVec>();
    auto positions = std::make_shared<PosVec>();
    auto ids = std::make_shared<PosVec>();
    Label2Index lab2Idx;

    load_labels_positions( *labels, *ids, *positions, "/home/jules/SAMOGWAS/Data/chr2/lab_300.csv");
    printf("Selected SNPs: %d -> %d\n\n", ids->front(), ids->back());


    auto mat = load_data_table("/home/jules/SAMOGWAS/Data/chr2/dat_300.csv");
    auto l2g = init_index_mapping( mat->size() );
    auto graph = init_graph( *mat,  lab2Idx, 3, *labels, *positions );

    auto criteria = std::make_shared<PositionCriteria>( positions, 50000 );

    Clustering clusteringCAST = runClustering(graph, l2g, criteria,1);
    Clustering clusteringDBSCAN = runClustering(graph, l2g, criteria,2);
//    Clustering clusteringLOUV = runClustering(graph, l2g, criteria,3);
    Clustering clusteringRES;

//        clusteringCAST.erase(clusteringCAST.begin() + 53);

    std::vector<Clustering> clusterings;
    clusterings.push_back(clusteringCAST);
    clusterings.push_back(clusteringDBSCAN);
//    clusterings.push_back(clusteringLOUV);
    clusteringRES = computeConsensusCluster(clusterings);
//    std::cout << "RESU[";
//      for (auto i: clusteringRES) {
//        std::cout << "[";
//        for (auto j: i)
//            std::cout << j << ',';
//        std::cout << "],";
//      }
//    std::cout << "]"<< std::endl;

//    clusteringRES.push_back(Cluster());
//    for (int i=0;i < 300 ; i++) {
//    clusteringRES.begin()->push_back(i);
//    }
    auto simi = std::make_shared<GraphMutInfoSimilarity>(graph, l2g);
    simi->set_criteria(criteria);
    std::cout << "gaston " << getRedundancyMeasure(clusteringRES,simi);

//    std::cout<< std::endl;
//    double obs = getObservedEntropy(*(clusteringRES.begin()), mat);
//    std::cout << obs <<std::endl;
//    double exp = getExpectedEntropy(*(clusteringRES.begin()), mat);
//    std::cout << exp <<std::endl;
//    std::cout << (exp - obs) /exp <<std::endl;

    double epsilon = getEpsilon(clusteringCAST, mat);
    std::cout << std::endl << "clusteringCAST : " << epsilon <<std::endl;
    epsilon = getEpsilon(clusteringDBSCAN, mat);
    std::cout << std::endl << "clusteringDBSCAN : " << epsilon <<std::endl;
//        epsilon = getEpsilon(clusteringLOUV, mat);
//        std::cout << std::endl << "clusteringLOUV : " << epsilon <<std::endl;
        epsilon = getEpsilon(clusteringRES, mat);
        std::cout << std::endl << "clusteringRES : " << epsilon <<std::endl;


//        writeResultsForTulip(clusteringDBSCAN, positions, ids, labels, "DBSCAN");
}

Clustering runClustering(GraphPtr graph, Local2GlobalPtr l2g, PositionCriteriaPtr criteria, int clusteringChoice) {
    auto start = get_time::now();
    Partition result;
    switch (clusteringChoice) {
        case 1: {
            std::cout << "CAST clustering = ";
            auto simi = std::make_shared<GraphMutInfoSimilarity>(graph, l2g);
            simi->set_criteria(criteria);
            CAST cast( simi, 0.6 );
            result = cast();
            std::cout << getRedundancyMeasure(result.to_clustering(),simi);
            break;
        }
        case 2: {
            std::cout << "DBSCAN clustering = ";
            auto dissimi = std::make_shared<GraphMutInfoDissimilarity>(graph, l2g);
            dissimi->set_criteria(criteria);
            DBSCAN dbscan(dissimi,2,0.38);
            result = dbscan();
            std::cout << getRedundancyMeasure(result.to_clustering(),dissimi);
            break;
        }
        case 3: {
            std::cout << "LOUVAIN clustering = ";
            auto simi2 = std::make_shared<GraphMutInfoSimilarity>(graph, l2g);
            simi2->set_criteria(criteria);
            louvain::MethodLouvain louv(simi2);
            result = louv();
            std::cout << getRedundancyMeasure(result.to_clustering(),simi2);
            break;
        }
    }
    auto diff = (get_time::now()-start);
    //we print the duration of the clustering
    std::cout <<" runs in " << diff.count()/1000000000 << " seconds"<< std::endl;

    Clustering clustering = result.to_clustering();
    std::cout <<" [";
     for (auto i: clustering) {
       std::cout << "[";
       for (auto j: i)
           std::cout << j << ',';
       std::cout << "],";
     }
    std::cout << "]"<< std::endl << std::endl;

    return clustering;
}

double getEpsilon(Clustering clustering, PtrMatrixPtr mat) {
    double epsilon = 0;
    int nb = 0;
    for (auto cluster : clustering ) {
        if (cluster.size() > 1) {
           double obs = getObservedEntropy(cluster, mat);
           double exp = getExpectedEntropy(cluster, mat);
//            for (auto a : cluster) {
//                std::cout<<a<<" ";
//            }

//            std::cout << obs << " + ";
//            std::cout << exp <<" = ";
//            std::cout << " -> " << (exp - obs) / exp  <<std::endl;
            epsilon +=  cluster.size() * (exp - obs) / exp;
            nb += cluster.size();
        }
    }

    return epsilon/nb;
}


double getObservedEntropy( Cluster cluster, PtrMatrixPtr mat){
    //this map lists all possible configurations with their occurrences
    std::map<VecT, int> cluster2Occurrence;
    int nbObs = mat->at(0)->size();
    for (int i = 0 ; i < nbObs ; ++i) {
        //this vector is a possible configuration of the cluster
        VecT newVec;
        for (int label : cluster) {
            newVec.push_back(mat->at(label)->at(i));
        }
        if (cluster2Occurrence.count(newVec) > 0)
            cluster2Occurrence[newVec]++;
        else
            cluster2Occurrence[newVec] = 1;
    }
//    for (auto a:cluster2Occurrence) {
//        std::cout<<"[";
//        for (auto b : a.first) {
//            std::cout << b << " ";
//        }
//        std::cout<<"] -> "<< a.second<<std::endl;
//    }

    double res = 0;
    for (auto a : cluster2Occurrence) {
        double x = ((double) a.second)/nbObs;
        res += -x * log2(x);
    }
    return res;
}


double getExpectedEntropy( Cluster cluster, PtrMatrixPtr mat){
    //this array gives the probabilities for  each value and each variable
    int distribution[cluster.size()][3];
    for (int i = 0 ; i < cluster.size() ; i++) {
        for (int j = 0 ; j < 3 ; ++j) {
            distribution[i][j] = 0;
        }
    }
    int nbObs = mat->at(0)->size();
    for (int i = 0 ; i < cluster.size() ; i++) {
        for (int j = 0 ; j < nbObs ; ++j) {
            distribution[i][mat->at(cluster[i])->at(j)]++;
        }
    }
//    for (int i = 0 ; i < cluster.size() ; i++) {
//        for (int j = 0 ; j < 3 ; ++j) {
//            std::cout<<distribution[i][j]<<" ";
//        }
//        std::cout<<std::endl;
//    }

    //we just have to sum all the distribution (cf writen proof)
    double res =0;
    for (int i = 0 ; i < cluster.size() ; i++) {
        for (int j = 0 ; j < 3 ; ++j) {
            double x = ((double) distribution[i][j]) / nbObs;
            if (x !=0)
                res += - x * log2(x);
        }
    }
    return res;
}

template <class T>
double getRedundancyMeasure( Clustering clustering, T distance ){
    double score = 0;
    int nbClusters = 0;
    for (std::vector<Cluster>::iterator it = clustering.begin() ; it != clustering.end() ; ++it) {
        if (it->size() > 1) {
            double scoreCluster = 0;
            for (std::vector<Index>::iterator itA = it->begin() ; itA != it->end() ; ++itA) {
                for (std::vector<Index>::iterator itB = std::next(itA) ; itB != it->end() ; ++itB) {
                    if (((std::string) typeid(T).name()).find("Diss") != std::string::npos)
                        scoreCluster += 1.0 - distance->compute(*itA,*itB);
                    else
                        scoreCluster += distance->compute(*itA,*itB);
                }
            }
            scoreCluster *= 2;
            scoreCluster /= it->size() * (it->size() -1);
            score += scoreCluster;

        }
        nbClusters++;
    }
    return score / nbClusters;
}


template <class T>
ClusterInformation getClusteringInformations( Clustering clustering, T diss ){
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


Clustering computeConsensusCluster(std::vector<Clustering> clusterings) {
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
