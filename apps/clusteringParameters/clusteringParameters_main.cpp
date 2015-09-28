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

    load_labels_positions( *labels, *ids, *positions, "/home/jules/SAMOGWAS/Data/chr2/lab.csv");
    printf("Selected SNPs: %d -> %d\n\n", ids->front(), ids->back());


    auto mat = load_data_table("/home/jules/SAMOGWAS/Data/chr2/dat.csv");
    auto l2g = init_index_mapping( mat->size() );
    auto graph = init_graph( *mat,  lab2Idx, 3, *labels, *positions );

    auto criteria = std::make_shared<PositionCriteria>( positions, 50000 );
Clustering clusteringDBSCAN, clusteringCAST;
//    Clustering clusteringCAST = runClustering(graph, l2g, criteria,1);
//    Clustering clusteringDBSCAN = runClustering(graph, l2g, criteria,2);
//    Clustering clusteringLOUV = runClustering(graph, l2g, criteria,3);
    Clustering clusteringRES;

//        clusteringCAST.erase(clusteringCAST.begin() + 53);

//    std::vector<Clustering> clusterings;
//    clusterings.push_back(clusteringCAST);
//    clusterings.push_back(clusteringDBSCAN);
//    clusterings.push_back(clusteringLOUV);
//    clusteringRES = computeConsensusCluster(clusterings);

    auto simi = std::make_shared<GraphMutInfoSimilarity>(graph, l2g);
    simi->set_criteria(criteria);

    std::cout<< "clusteringDBSCAN " << "\n";
//    for (double epsilon = 0.1 ; epsilon <= 0.9 ; epsilon += 0.4) {
//        std::cout<< epsilon << "  \t-> ";
//        clusteringDBSCAN = runClustering(graph, l2g, criteria,2, epsilon);
////        std::cout << "redundancy = " << getRedundancyMeasure(clusteringDBSCAN,simi) ;
//        std::cout << "NED = " << getEpsilon(clusteringDBSCAN, mat,3);
//        std::cout << std::endl;
//    }
    std::cout << "+++++++++++++++++++++++++++++++++++++" << std::endl;
    std::string modeName;
    for (int mode = 1 ; mode < 5 ; mode++) {
        if (mode == 1 || mode == 2) {
            modeName = "nbSNPs_";
        } else {
            modeName = "nbClusters_";
        }
        if (mode == 1 || mode == 3) {
            modeName += "avecSingletons";
        } else {
            modeName += "sansSingletons";
        }
        std::cout << modeName << std::endl;
        for (double epsilon = 0.05 ; epsilon <= 0.95 ; epsilon += 0.05) {
            std::cout << epsilon << std::endl;
            clusteringDBSCAN = runClustering(graph, l2g, criteria,2, epsilon);
            for (int it = 1 ; it < 101 ; it++) {
                std::string name = "resultats/it";
                name += std::to_string(it);
                name += "_DBSCAN_NED_";
                name += modeName;
                std::ofstream fichier(name, std::ios::out | std::ios::app);  // ouverture en écriture avec effacement du fichier ouvert
                if(fichier) {
                        fichier << getEpsilon(randomSubClustering(clusteringDBSCAN), mat,mode) << std::endl;
                        fichier.close();
                } else
                        std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;
            }
        }

    }

//    std::cout<< epsilon << "  \t-> ";
//    clusteringDBSCAN = runClustering(graph, l2g, criteria,2, epsilon);
//    clusteringAux = randomSubClustering(clusteringDBSCAN);
//    std::cout << "NED = " << getEpsilon(clusteringDBSCAN, mat,mode);
//    std::cout << std::endl;



//    std::cout<< "clusteringCAST " << "\n";
//    for (double epsilon = 0.2 ; epsilon < 1 ; epsilon += 1) {
//        std::cout<< epsilon << "\t-> ";
//        clusteringCAST = runClustering(graph, l2g, criteria,1, epsilon);
//        std::cout << "redundancy = " << getRedundancyMeasure(clusteringCAST,simi) ;
//        std::cout << "\tepsilon NED = " << getEpsilon(clusteringCAST, mat);
//        std::cout << std::endl;
//    }

//    std::cout<< std::endl;
//    double obs = getObservedEntropy(*(clusteringRES.begin()), mat);
//    std::cout << obs <<std::endl;
//    double exp = getExpectedEntropy(*(clusteringRES.begin()), mat);
//    std::cout << exp <<std::endl;
//    std::cout << (exp - obs) /exp <<std::endl;

//    double epsilon = getEpsilon(clusteringCAST, mat);
//    std::cout << std::endl << "clusteringCAST : " << epsilon <<std::endl;
//    epsilon = getEpsilon(clusteringDBSCAN, mat);
//    std::cout << std::endl << "clusteringDBSCAN : " << epsilon <<std::endl;
//        epsilon = getEpsilon(clusteringLOUV, mat);
//        std::cout << std::endl << "clusteringLOUV : " << epsilon <<std::endl;
//        epsilon = getEpsilon(clusteringRES, mat);
//        std::cout << std::endl << "clusteringRES : " << epsilon <<std::endl;


//        writeResultsForTulip(clusteringDBSCAN, positions, ids, labels, "DBSCAN");
}

Clustering randomSubClustering(Clustering clustering) {
    int random = rand() % (38730 - 3010);
//    int random = rand() % (3000 - 310);
    Clustering randomSubClustering;
    for (auto a : clustering) {
        Cluster c;
        for (int b : a) {
            if ( b > random && b < random +3000) {
                c.push_back(b);
            }
        }
        if (!c.empty())
            randomSubClustering.push_back(c);
    }
    return randomSubClustering;
}
Clustering runClustering(GraphPtr graph, Local2GlobalPtr l2g, PositionCriteriaPtr criteria, int clusteringChoice, double epsilon) {
    auto start = get_time::now();
    Partition result;
    switch (clusteringChoice) {
        case 1: {
//            std::cout << "CAST clustering = ";
            auto simi = std::make_shared<GraphMutInfoSimilarity>(graph, l2g);
            simi->set_criteria(criteria);
            CAST cast( simi, epsilon );
            result = cast();
//            std::cout << getRedundancyMeasure(result.to_clustering(),simi);
            break;
        }
        case 2: {
//            std::cout << "DBSCAN clustering = ";
            auto dissimi = std::make_shared<GraphMutInfoDissimilarity>(graph, l2g);
            dissimi->set_criteria(criteria);
            DBSCAN dbscan(dissimi, 2, epsilon);
            result = dbscan();
//            std::cout << getRedundancyMeasure(result.to_clustering(),dissimi);
            break;
        }
        case 3: {
            std::cout << "LOUVAIN clustering = ";
            auto simi2 = std::make_shared<GraphMutInfoSimilarity>(graph, l2g);
            simi2->set_criteria(criteria);
            louvain::MethodLouvain louv(simi2);
            result = louv();
//            std::cout << getRedundancyMeasure(result.to_clustering(),simi2);
            break;
        }
    }
    auto diff = (get_time::now()-start);
    //we print the duration of the clustering
//    std::cout <<" runs in " << diff.count()/1000000000 << " seconds"<< std::endl;

    Clustering clustering = result.to_clustering();

//     std::cout <<" [";
//     for (auto i: clustering) {
//       std::cout << "[";
//       for (auto j: i)
//           std::cout << j << ',';
//       std::cout << "],";
//     }
//    std::cout << "]"<< std::endl << std::endl;

    return clustering;
}

//mode 1: nbSNPs_avecSingletons, 2: nbSNPs_sansSingletons, 3: nbClusters_avecSingletons, 4: nbClusters_sansSingletons
double getEpsilon(Clustering clustering, PtrMatrixPtr mat, int mode) {
    double epsilon = 0;
    int nb = 0;


    for (auto cluster : clustering ) {
        if (cluster.size() > 1 && cluster.size() < 10) {
            std::map<VecT, double> toto = getClusterToFrequencyMap(cluster, mat);
            double obs = getObservedEntropy(toto);
//           std::cout << "obs = " << obs ;
            double exp = getExpectedEntropy(toto);
//           std::cout << "\texp = " << exp;
//            for (auto a : cluster) {
//                std::cout<<a<<" ";
//            }

            if (exp == 0) {
               epsilon += cluster.size();
            } else {
                 if (mode == 1 || mode == 2)
                    epsilon +=  cluster.size() * (exp - obs) / exp;
                 else if (mode == 3 || mode == 4)
                     epsilon +=  (exp - obs) / exp;
            }
            if (mode == 4)
                nb++;
            else if (mode == 2)
                nb += cluster.size();

//           }
//           std::cout << "\tepsilon = " << cluster.size() * (exp - obs) / exp << std::endl;
        }
        if (mode == 3)
            nb++;
        else if (mode == 1)
            nb += cluster.size();

    }
    return epsilon/nb;
}



std::map<VecT, double> getClusterToFrequencyMap( Cluster cluster, PtrMatrixPtr mat ) {
    //this map lists all possible configurations with their occurrences
    std::map<VecT, double> clusterToFrequencyMap;
    int nbObs = mat->at(0)->size();
    for (int i = 0 ; i < nbObs ; ++i) {
        //this vector is a possible configuration of the cluster
        VecT newVec;
        for (int label : cluster) {
            newVec.push_back(mat->at(label)->at(i));
        }
        if (clusterToFrequencyMap.count(newVec) > 0)
            clusterToFrequencyMap[newVec] += 1;
        else
            clusterToFrequencyMap[newVec] = 1;
    }

    //à mettre pour le mode nbSNPs_avecSingletons_bis
//    int nbObsAux = nbObs;
//    for (auto &clusterMap : clusterToFrequencyMap) {
//        if (clusterMap.second < nbObsAux * (-0.056 * log(cluster.size()) + 0.13)) {
//            nbObs -= clusterMap.second;
////            clusterMap.second = 0;
//            clusterToFrequencyMap.erase(clusterMap.first);
//        }
//    }

    //est ce que la couverture est bonne...
        //    if (((double) nbObs) / nbObsAux < 0.7) { //si la couverture est inférieure à 70%, elle est mauvaise..
    //       std::cout <<std::endl<< "coverage = " << ((double) nbObs) / nbObsAux <<std::endl;
    //       std::cout << "[";
    //       for (auto j: cluster)
    //           std::cout << j << ',';
    //       std::cout << "]"<<std::endl;
    //    }
    //    for (auto a:clusterToFrequencyMap) {
    //        std::cout<<"[";
    //        for (auto b : a.first) {
    //            std::cout << b << " ";
    //        }
    //        std::cout<<"] -> "<< a.second<<std::endl;
    //    }

    if (nbObs > 0) {
        for (auto &cluster : clusterToFrequencyMap) {
            cluster.second /= nbObs;
        }
    }
    return clusterToFrequencyMap;
}



double getObservedEntropy( std::map<VecT, double> clusterToFrequencyMap ){

    double res = 0;
    for (auto clusterMap : clusterToFrequencyMap) {
        if (clusterMap.second !=0) {
            res += - clusterMap.second * log2(clusterMap.second);
        }
    }
    return res;
}

double getExpectedEntropy( std::map<VecT, double> clusterToFrequencyMap ){
    int clusterSize = clusterToFrequencyMap.begin()->first.size();

    //this array gives the probabilities for  each value and each variable
    double distribution[clusterSize][3];
    for (int i = 0 ; i < clusterSize ; i++) {
        for (int j = 0 ; j < 3 ; j++) {
            distribution[i][j]=0;
        }
    }

    for (auto clusterMap : clusterToFrequencyMap) {
        for (int i = 0; i < clusterSize ; i++) {
            distribution[i][clusterMap.first[i]] += clusterMap.second;
        }
    }

    //we just have to sum all the distribution (cf writen proof)
    double res =0;
    for (int i = 0 ; i < clusterSize ; i++) {
        for (int j = 0 ; j < 3 ; ++j) {
            if (distribution[i][j] !=0)
                res += - distribution[i][j] * log2(distribution[i][j]);
        }
    }

    return res;
}

double getExpectedEntropy( Cluster cluster, PtrMatrixPtr mat ){

    //this array gives the probabilities for  each value and each variable
    double distribution[cluster.size()][3];
    int nbObs = mat->at(0)->size();
    for (int i = 0 ; i < cluster.size() ; i++) {
        for (int j = 0 ; j < 3 ; ++j) {
            distribution[i][j]=0;
        }
    }

    for (int i = 0 ; i < cluster.size() ; i++) {
        for (int j = 0 ; j < nbObs ; ++j) {
            distribution[i][mat->at(cluster[i])->at(j)]++;
        }
    }

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
            nbClusters++;
        }
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
