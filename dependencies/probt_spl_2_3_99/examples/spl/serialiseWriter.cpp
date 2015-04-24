/*=======================================================================
 * Product        :  
 * File           : serialiseWriter.cpp
 * Author         : Juan-Manuel Ahuactzin-Larios
 * Creation       : Tue Aug 21 10:13:29 2007
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

using namespace std;

int main(int argc, char * argv[])
{
#if defined(WIN32) || defined(_WIN32)
  std::string file("../examples/spl/my_data.data");
#else
  std::string file("my_data.data");
#endif


  if (argc > 1) {
    file = argv[1];
  }

  // Open the file containig the data
  ifstream fin(file.c_str());

  // If the file does not exist then exit 
  if (! fin){
    std::cerr << "Error: cannot open file " << file << std::endl;
    return 1;
  }

  // Defining the variables
  plContinuousIntervalType my_type(0.0, 18.0, 180);
  plSymbol A("A",my_type);

  // Create the learning object
  plLearnHistogram my_learner(A);

  // Read data fron file
  plValues values(A);  
  fin >> values[0];
  while (fin) {
    // Add the read data into the learning my_learner
    my_learner.add_point(values);
    fin >> values[0];
  }

  fin.close();

  plDistribution probas = my_learner.get_distribution();

  //This is the object that can store the ProBT objects
  plSerializer pgm( "my_program" );

  //We store the type
  pgm.add_object("the type", my_type);

  //We store the variable
  pgm.add_object("A", A);

  //We store the learned distribution
  pgm.add_object("distribution", probas);

  //We store the object that learns
  pgm.add_object("learn object", my_learner);

  //Save the added objects

#if defined(WIN32) || defined(_WIN32)
  pgm.save("../examples/spl/my_objects.xml");
  probas.plot("../examples/spl/my_learn.gnu");
#else
  pgm.save("my_objects.xml");
  probas.plot("my_learn.gnu");
#endif


  // On Windows (Visual C++, MinGW) only.
#if defined(WIN32) || defined(_WIN32)
  cout << "Press any key to terminate..." << endl;
  getchar();
#endif
}
