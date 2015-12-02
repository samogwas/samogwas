/****************************************************************************************
 * File: card_func.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 14/04/2015

 ***************************************************************************************/
#ifndef SAMOGWAS_CARD_FUNC_HPP
#define SAMOGWAS_CARD_FUNC_HPP

namespace samogwas
{

struct CardFunc {
  virtual int operator()(const std::vector<int> &observedVariables) {
    return compute(observedVariables);
  }

  virtual int compute(const std::vector<int> &observedVariables) = 0;
};

/** Computes the cardinality according to the following formula:
 * card = min(alpha * nbrChildren + beta, maxCardinality)
 * where alpha, beta and maxCard are given.
 */
struct LinearCardinality: public CardFunc
{
  LinearCardinality(const double a, const double b, const int card):
                                 alpha(a), beta(b), maxCard(card) {}

  virtual int compute(const std::vector<int> &observedVariables) {
    int nbrVariables = observedVariables.size();
    return std::min( int(alpha * nbrVariables + beta), maxCard);
  }

 protected:
  double alpha;
  double beta;
  int maxCard;
};

} // namespace samogwas ends here. 

/****************************************************************************************/
#endif // SAMOGWAS_CARD_FUNC_HPP
