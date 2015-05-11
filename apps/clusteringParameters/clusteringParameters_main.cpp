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
using ms = std::chrono::milliseconds;
using get_time = std::chrono::steady_clock ;
typedef std::shared_ptr<PositionCriteria> PositionCriteriaPtr;

void writeResultsForTulip( Clustering clustering, std::shared_ptr<PosVec> positions, std::shared_ptr<PosVec> ids, std::shared_ptr<LabelVec> labels);
void printClusteringInformations( Clustering clustering );

int main() {
    auto labels = std::make_shared<LabelVec>(); auto positions = std::make_shared<PosVec>();  auto ids = std::make_shared<PosVec>();
    Label2Index lab2Idx;

    load_labels_positions( *labels, *ids, *positions, "/home/jules/Bureau/donnees/input/lab_3000.csv");


    printf("first SNP id = %d\n", ids->front());
    printf("last SNP id = %d\n", ids->back());
    printf("number of SNPs = %zu\n", ids->size());

    auto mat = load_data_table("/home/jules/Bureau/donnees/input/dat_3000.csv");
    auto l2g = init_index_mapping( mat->size() );
    auto graph = init_graph( *mat,  lab2Idx, 3, *labels, *positions );

    auto criteria = std::make_shared<PositionCriteria>( positions, 50000 );


    Clustering clustering;


    for (double epsi = 0.63 ; epsi < 0.64 ;epsi +=0.01) {
        printf("\n*******************************************************************\nMinPt = 2, epsilon = %f\n\n",epsi);

        auto start = get_time::now();
//        auto simi = std::make_shared<GraphMutInfoSimilarity>(graph, l2g);
//        simi->set_criteria(criteria);
//        CAST cast( simi, 0.5 );
//        Partition result = cast();
        auto dissimi = std::make_shared<GraphMutInfoDissimilarity>(graph, l2g);
        dissimi->set_criteria(criteria);
        DBSCAN dbscan(dissimi,2,epsi);
        Partition result = dbscan();

        auto end = get_time::now();
        auto diff = (end-start);
        //we print the duration of the clustering
        std::cout << diff.count()/1000000000 << " secs\n";

        clustering = result.to_clustering();
        printClusteringInformations(clustering);

    }
    writeResultsForTulip(clustering, positions, ids, labels);
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

void writeResultsForTulip( Clustering clustering, std::shared_ptr<PosVec> positions, std::shared_ptr<PosVec> ids, std::shared_ptr<LabelVec> labels) {

    //Csv files used for Tulip vizualisation
    FILE *nodes, *edges;
    nodes = fopen ("nodes.csv","w");
    edges = fopen ("edges.csv","w");
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
