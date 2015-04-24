/*=============================================================================
 * Product        : ProBT 
 * File           : BayesianNetwork.cpp
 * Author         : David Raulo
 * Creation       : Fri Feb 29 16:09:15 2008
 *
 *=============================================================================
 *        (c) Copyright 2008, ProBayes SAS  -  all rights reserved
 *=============================================================================
*/

#include <pl.h>
#include <iostream>
using namespace std;

int main() 
{
  /**********************************************************************
   VARIABLES SPECIFICATION
  **********************************************************************/
  plSymbol A("A",PL_BINARY_TYPE);
  plSymbol B("B",PL_BINARY_TYPE);
  plSymbol C("C",PL_BINARY_TYPE);
  plSymbol D("D",PL_BINARY_TYPE);
  plSymbol E("E",PL_BINARY_TYPE);

  /**********************************************************************
   PARAMETRIC FORM SPECIFICATION
  **********************************************************************/
  // Specification of P(A)
  plProbValue tableA[] = {0.4, 0.6};
  plDistribution P_A(A, tableA);

  // Specification of P(B)
  plProbValue tableB[] = {0.18, 0.82};
  plDistribution P_B(B, tableB);

  // Specification of P(C)
  plProbValue tableC[] = {0.75, 0.25};
  plDistribution P_C(C, tableC);

  // Specification of P(D | A B)
  plProbValue tableD_knowingA_B[] = {0.6, 0.4,  // P(D | [A=f]^[B=f])
                                     0.3, 0.7,  // P(D | [A=f]^[B=t])
                                     0.1, 0.9,  // P(D | [A=t]^[B=f])
                                     0.5, 0.5}; // P(D | [A=t]^[B=t])
  plCndDistribution P_D(D,A^B,tableD_knowingA_B);

  // Specification of P(E | C D)
  plProbValue tableE_knowingC_D[] = {0.59, 0.41,  // P(E | [C=f]^[D=f])
                                     0.25, 0.75,  // P(E | [C=f]^[D=t])
                                     0.8,  0.2,   // P(E | [C=t]^[D=f])
                                     0.35, 0.65}; // P(E | [C=t]^[D=t])
  plCndDistribution P_E(E,C^D,tableE_knowingC_D);

  /**********************************************************************
   DECOMPOSITION
  **********************************************************************/
  plJointDistribution jd(A^B^C^D^E, P_A*P_B*P_C*P_D*P_E);
  jd.draw_graph("bayesian_network.fig");
  cout<<"OK\n";
  /**********************************************************************
   PROGRAM QUESTION
  **********************************************************************/
  // Get the inferred conditional distribution representing P(C B | E D)
  plCndDistribution CndP_CB;
  jd.ask(CndP_CB, C^B, E^D);

  // Create the value representing the evidence = [E=true]^[D=false]
  plValues evidence(E^D);
  evidence[E] = true;
  evidence[D] = false;
  
  // Get the distribution representing P(C B | [E=true]^[D=false] )
  plDistribution P_CB;
  CndP_CB.instantiate(P_CB,evidence);

  // Get the normalized distribution representing P(C B | [E=true]^[D=false] )
  plDistribution  T_P_CB;
  P_CB.compile(T_P_CB);

  // Display the result
  cout << T_P_CB << endl;

  // On Windows (Visual C++, MinGW) only.
#if defined(WIN32) || defined(_WIN32)
  cout << "Press any key to terminate..." << endl;
  getchar();
#endif

  return 0;
}
