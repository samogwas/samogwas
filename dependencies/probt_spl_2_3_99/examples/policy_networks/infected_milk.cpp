/*=============================================================================
 * Product        : ProBT 
 * File           : infected_milk.cpp
 * Author         : David Raulo
 * Creation       : Wed Sep  9 16:33:27 2009
 *
 *=============================================================================
 *        (c) Copyright 2009, ProBayes SAS  -  all rights reserved
 *=============================================================================
*/

#include <pl.h>
#include <iostream>

using namespace std;

#define GAIN_PER_COW 200
#define INIT_COWS     49
#define COST_TEST      4

// #define WOW

//-----------------------------------------------------------------------------
void f_gain(plValues& output, const plValues& input)
{
  const long int infected = input[0];
  const long int action = input[1];
#ifdef WOW
  const long int ncows = INIT_COWS;
#else
  const long int ncows = input[2];
#endif
  if (action) {
    if (infected) output[0] = 0;
    else          output[0] = (ncows+1) * GAIN_PER_COW;
  }
  else
    output[0] = ncows * GAIN_PER_COW;
}

//-----------------------------------------------------------------------------
void f_ct(plValues& output, const plValues& input)
{
  output[0] = input[0] ? -COST_TEST : 0;
}

//-----------------------------------------------------------------------------
int main()
{
  //---------------------------------------------------------------------------
  // CONSTRUCT MODEL

  // Random variable: state of the milk (infected or not)
  plSymbol Inf("Inf", PL_BINARY_TYPE);
  // Action: whether to test the milk or not
  plSymbol T("T", PL_BINARY_TYPE);
  // Random variable: result of the test (infected or not)
  plSymbol Test("Test", PL_BINARY_TYPE);
  // Utility of the test in cents
  plSymbol CT("CT", plIntegerType(-COST_TEST,0));
  // Action: whether to pour the milk or not
  plSymbol Action("Action", PL_BINARY_TYPE);
  // Gain of the action
  plSymbol Gain("Gain", plIntegerType(0, GAIN_PER_COW*(INIT_COWS+1)));
  // Number of cows already poured
  plSymbol Ncows("Ncows", plIntegerType(INIT_COWS, INIT_COWS));

  // prior P(Ncows)
  plUniform p_Ncows(Ncows);

  // prior P(Inf)
  plProbValue t_Inf[] = {.9993, .0007};
  plProbTable p_Inf(Inf, t_Inf);

  // dependencies of test-decision T (to perform the test or not): Ncows
  plCndUnknown p_T(T, Ncows);

  // utility of the test
  plExternalFunction func_ct(CT, T, &f_ct, "f_ct");
  plCndDeterministic p_CT(CT, T, func_ct);

  // result of the test
  plProbValue t_Test[] = { 0.5, 0.5, //Inf=no T=no
                           1.0, 0.0, //Inf=no T=yes
                           0.5, 0.5, //Inf=yes T=no
                           0.0, 1.0  //Inf=yes T=yes
  };
  plCndDistribution p_Test(Test, Inf^T, t_Test);

  // utility of the action, depending on the milk being infected or not
  plVariablesConjunction p_Gain_right;
#ifdef WOW
  p_Gain_right = Inf^Action;
#else
  p_Gain_right = Inf^Action^Ncows;
#endif
  plExternalFunction func_gain(Gain, p_Gain_right, &f_gain, "f_gain");
  plCndDeterministic p_Gain(Gain, p_Gain_right, func_gain);

  // dependencies of Action: whether we performed the test, and its result
  plCndUnknown p_Action(Action, T^Test);

  //---------------------------------------------------------------------------
  // RUN SOLVER AND PRINT RESULTING POLICIES

  plComputableObjectList chance_dist = p_Ncows * p_Inf * p_Test;
  plComputableObjectList action_dist = p_T * p_Action;
  plComputableObjectList utility_dist = p_CT * p_Gain;

  plPolicyNetwork net(chance_dist, action_dist, utility_dist);

  plJointDistribution joint = net.get_joint_distribution();
  cout << "Joint distribution for solving the policy problem: " << endl
       << joint << endl;

  std::ofstream fout("infected_milk.dot");
  net.output_network_dot(fout);

  plComputableObjectList policies = net.run_optimization();
  cout << "Final policies:" << endl << endl;
  net.output_policies(std::cout);
//   cout << endl << "Detail:" << endl << policies << endl;

  //---------------------------------------------------------------------------
  // RUN SIMULATIONS

  cout << "-------------------------------------------------------------------"
       << endl;
  plSymbol SumC = net.get_sum_utilities();
  const size_t n_questions = 4;
  plCndDistribution question[n_questions];
  plVariablesConjunction cond = Ncows^Inf^T^Action;
  plDistribution answer[n_questions], compiled[n_questions];
  plError::ignore_this_message(50, true);
  joint.ask(question[0], CT,   cond, PL_OPTIMIZE_UPDATE_TIME);
  joint.ask(question[1], Test, cond, PL_OPTIMIZE_UPDATE_TIME);
  joint.ask(question[2], Gain, cond, PL_OPTIMIZE_UPDATE_TIME);
  joint.ask(question[3], SumC, cond, PL_OPTIMIZE_UPDATE_TIME);

  cout << question[0] << endl
       << question[1] << endl
       << question[2] << endl
       << question[3] << endl << endl
       << "Initial conditions\t\t\tCT\tTest\tGain\tSumC" << endl;
  plValues val_right(cond);
  do {
    cout << val_right;
    for (unsigned int i=0; i<n_questions; ++i) {
      std::vector<plFloat> val_left(1);
      question[i].instantiate(answer[i], val_right);
      answer[i].compile(compiled[i]);
      compiled[i].compute_expectation(val_left);
      cout << "\t" << val_left[0];
//       cout << "\t" << plRound(val_left[0]);
    }//endfor
    cout << endl;
  } while (val_right.next());

  // On Windows (Visual C++, MinGW) only.
#if defined(WIN32) || defined(_WIN32)
  cout << "Press any key to terminate..." << endl;
  getchar();
#endif

  return 0;
}


// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
