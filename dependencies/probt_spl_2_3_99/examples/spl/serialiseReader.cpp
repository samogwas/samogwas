/*=======================================================================
 * Product        :  
 * File           : serialiseReader.cpp
 * Author         : Juan-Manuel Ahuactzin-Larios
 * Creation       : Mon Aug 27 15:56:08 2007
 *
 *=======================================================================
 *                      (c) Copyright 2007, Probayes SAS,
 *                              all rights reserved
 *=======================================================================
 *
 *------------------------- Description ---------------------------------
 *   
 *   
 *-----------------------------------------------------------------------
*/


#include <pl.h> 
#include <plDebug.h>
#include <plSerializer.h>
#include <plLearnLaplace.h>
#include <plLearn1dNormal.h>
#include <iostream>
#include <fstream>

#define N_INTERVALS 180

using namespace std;

int main(int argc, char * argv[])
{
#if defined(WIN32) || defined(_WIN32)
  std::string s11n_file("../examples/spl/my_objects.xml");
  std::string data_file("../examples/spl/my_data2.data");
#else
  std::string s11n_file("my_objects.xml");
  std::string data_file("my_data2.data");
#endif

  if (argc > 1) {
    s11n_file = argv[1];
  }
  if (argc > 2) {
    data_file = argv[2];
  }

  // These ProBT objects are constructed by default. They
  // will be filled in by the serialization load().
  plRealType my_type;
  plSymbol A;
  plDistribution my_distribution;
  plLearnHistogram my_learner;

  // A plSerializer stores ProBT objects.
  plSerializer pgm;
  pgm.load(s11n_file);

  pgm.get_object("the type",my_type);

  pgm.get_object("A",A);

  pgm.get_object("distribution",my_distribution);

  pgm.get_object("learn object",my_learner);
  

  // Open the file containig the data
  ifstream fin(data_file.c_str());

  // If the file does not exist then exit 
  if (! fin){
    std::cerr << "Cannot open data file " << data_file << std::endl;
    return 1;
  }

  // Read data from file
  plValues values(A);  
  fin>>values[0];
  while (fin) {
    // Add the read data into the learning my_learner
    my_learner.add_point(values);
    fin >> values[0];
  }

  fin.close();

  plDistribution probas = my_learner.get_distribution();

#if defined(WIN32) || defined(_WIN32)
  probas.plot("../examples/spl/my_learn2.gnu");
#else
  probas.plot("my_learn2.gnu");
#endif


  // On Windows (Visual C++, MinGW) only.
#if defined(WIN32) || defined(_WIN32)
  cout << "Press any key to terminate..." << endl;
  getchar();
#endif

  return 0;
}
