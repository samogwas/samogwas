/*=============================================================================
 * Product        : ProBT 
 * File           : oil_wildcatter.cpp
 * Author         : Nicolas Teypaz
 * Creation       : Wed Sep  9 16:33:27 2009
 *
 *=============================================================================
 *        (c) Copyright 2009, ProBayes SAS  -  all rights reserved
 *=============================================================================
*/

#include <pl.h>
#include <iostream>

using namespace std;

#define GAIN_FOR_DRY -70
#define GAIN_FOR_WET 50
#define GAIN_FOR_SOAK 200
#define COST_TEST      10
#define DUMMY      0
//-----------------------------------------------------------------------------
void f_gain(plValues& output, const plValues& input)
{
  const long int oil = input[0];
  const long int action = input[1];
  if (action) {
    if (oil == 0) output[0] = GAIN_FOR_DRY;
    else if (oil == 1) output[0] = GAIN_FOR_WET;
    	else if (oil == 2) output[0] = GAIN_FOR_SOAK;
  }
  else
    output[0] = 0;
}

//-----------------------------------------------------------------------------
void f_ct(plValues& output, const plValues& input)
{
  output[0] = (input[0] == 1)? -COST_TEST : 0;
}

//-----------------------------------------------------------------------------
int main()
{
  // Random variable: state of the fieds (dry = 0, wet = 1, soak = 2)
  plSymbol Oil("Oil", plIntegerType(0, 2));
  // Action: whether to test the milk or not
  plSymbol T("T", PL_BINARY_TYPE);

  // Random variable: result of the test (closed = 0, open = 1, diff = 2)
  plSymbol Seismic("Seismic", plIntegerType(0, 2));

  // Utility of the test in dollars
  plSymbol CT("CT", plIntegerType(-COST_TEST,0));

  // Action: whether to drill for oil
  plSymbol Action("Drill", PL_BINARY_TYPE);

  // Gain of the action
  plSymbol Gain("Gain", plIntegerType(GAIN_FOR_DRY-COST_TEST, GAIN_FOR_SOAK));

  // Dummy node for the test
  plSymbol Dummy("Dummy", plIntegerType(DUMMY, DUMMY));
  // prior P(Ncows)
  plUniform p_Dummy(Dummy);
  //---------------------------------------------------------------------------

  // prior P(Inf)
  plProbValue t_Oil[] = {.5, .3, .2};
  plProbTable p_Oil(Oil, t_Oil);

  // dependencies of test-decision T (to perform the test or not): Ncows
  plCndUnknown p_T(T, Dummy);

  // utility of the test
  plExternalFunction func_ct(CT, T, &f_ct, "f_ct");
  plCndDeterministic p_CT(CT, T, func_ct);

  // result of the test
  plProbValue t_Seismic[] = { 1.00/3.00, 1.00/3.00, 1.00/3.00, 0.1, 0.3, 0.6,   
                           1.00/3.00, 1.00/3.00, 1.00/3.00, 0.3, 0.4, 0.3,  
                           1.00/3.00, 1.00/3.00, 1.00/3.00, 0.5, 0.4, 0.1
  };
  plCndDistribution p_Seismic(Seismic, Oil^T, t_Seismic);

  // utility of the action, depending on the milk being infected or not
  plExternalFunction func_gain(Gain, Oil^Action, &f_gain, "f_gain");
  plCndDeterministic p_Gain(Gain, Oil^Action, func_gain);

  // dependencies of Action: whether we performed the test, and its result
  plCndUnknown p_Action(Action, T^Seismic);

  //---------------------------------------------------------------------------
  plComputableObjectList chance_dist = p_Dummy * p_Oil * p_Seismic;
  plComputableObjectList action_dist = p_T * p_Action;
  plComputableObjectList utility_dist = p_CT * p_Gain;

  plPolicyNetwork net(chance_dist, action_dist, utility_dist);
  std::ofstream fout("oil_wildcatter.dot");
  net.output_network_dot(fout);
  plComputableObjectList policies = net.run_optimization();
  net.output_policies(std::cout);


  plSymbol SumC = net.get_sum_utilities();
  plJointDistribution joint = net.get_joint_distribution();
  plCndDistribution question[3];
  plDistribution answer[3], compiled[3];
  joint.ask(question[0], CT, Action^T^Oil, PL_OPTIMIZE_UPDATE_TIME);
  joint.ask(question[1], Gain, Action^T^Oil, PL_OPTIMIZE_UPDATE_TIME);
  joint.ask(question[2], SumC, Action^T^Oil, PL_OPTIMIZE_UPDATE_TIME);
  std::vector<plFloat> val_left(1);
  plValues val_right(Action^T^Oil);
  cout << endl << "-------------------------------------------------" << endl
       << "Initial conditions\tCT\tGain\tSumC" << endl;
  do {
    cout << val_right;
    for (unsigned int i=0; i<3; ++i) {
      question[i].instantiate(answer[i], val_right);
      answer[i].compile(compiled[i]);
      compiled[i].compute_expectation(val_left);
      cout << "\t" << plRound(val_left[0]);
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
