/*=============================================================================
 * Product        : ProBT 
 * File           : asia.cpp
 * Author         : David Raulo
 * Creation       : Fri Jul 25 14:49:21 2008
 *
 *=============================================================================
 *        (c) Copyright 2008, ProBayes SAS  -  all rights reserved
 *=============================================================================
*/

// "Asia" model

//  |   A       S
//  |    \     / \ 
//  |     T   L   B
//  |      \ /   /
//  |       O   /
//  |      / \ /
//  V     X   D

//   ProbA = [0.01]
//   ProbS = [0.5]
//   ProbT = [0.05, 0.01]
//   ProbL = [0.1, 0.01]
//   ProbB = [0.6, 0.3]
//   ProbO = [1, 1, 1, 0]
//   ProbX = [0.98, 0.05]
//   ProbD = [0.9, 0.8, 0.7, 0.1]

#include <pl.h>

//-----------------------------------------------------------------------------
plJointDistribution make_model()
{
  // VARIABLES SPECIFICATION
  plSymbol A("A", PL_BINARY_TYPE); // visit to Asia?
  plSymbol S("S", PL_BINARY_TYPE); // Smoker?
  plSymbol T("T", PL_BINARY_TYPE); // has Tuberculosis
  plSymbol L("L", PL_BINARY_TYPE); // has Lung cancer
  plSymbol B("B", PL_BINARY_TYPE); // has Bronchitis
  plSymbol O("O", PL_BINARY_TYPE); // has tuberculosis Or cancer
  plSymbol X("X", PL_BINARY_TYPE); // positive X-Ray
  plSymbol D("D", PL_BINARY_TYPE); // Dyspnoea?

  // PARAMETRIC FORM SPECIFICATION
  plProbValue tableA[] = { 0.99, 0.01 };
  plProbValue tableS[] = { 0.5,  0.5  };
  plProbValue tableT[] = { 0.99, 0.01,    // P(T | [A=f])
                           0.95, 0.05 };  // P(T | [A=t])
  plProbValue tableL[] = { 0.99,  0.01,
                           0.9,   0.1  };
  plProbValue tableB[] = { 0.7,  0.3,
                           0.4,  0.6  };
  plProbValue tableO[] = { 1,    0,      // P(O | [T=f]^[L=f])
                           0,    1,      // P(O | [T=f]^[L=t])
                           0,    1,      // P(O | [T=t]^[L=f])
                           0,    1    }; // P(O | [T=t]^[L=t])
  plProbValue tableX[] = { 0.95, 0.05,
                           0.02, 0.98 };
  plProbValue tableD[] = { 0.9,  0.1,
                           0.3,  0.7,
                           0.2,  0.8,
                           0.1,  0.9  };

  // DECOMPOSITION
  plDistribution P_A(A, tableA);
  plDistribution P_S(S, tableS);
  plCndDistribution P_T_k_A(T, A, tableT);
  plCndDistribution P_L_k_S(L, S, tableL);
  plCndDistribution P_B_k_S(B, S, tableB);
  plCndDistribution P_O_k_T_L(O, T^L, tableO);
  plCndDistribution P_X_k_O(X, O, tableX);
  plCndDistribution P_D_k_O_B(D, O^B, tableD);
  plVariablesConjunction variables = A^S^T^L^B^O^X^D;
  plJointDistribution jd(variables, P_A * P_S * P_T_k_A * P_L_k_S *
                         P_B_k_S * P_O_k_T_L * P_X_k_O * P_D_k_O_B);

  return jd;
}

//-----------------------------------------------------------------------------
// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
