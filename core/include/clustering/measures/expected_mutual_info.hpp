/****************************************************************************************
 * File: expected_mutual_info.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 17/05/2015

 ***************************************************************************************/
#ifndef SAMOGWAS_EXPECTED_MUTUAL_INFO_HPP
#define SAMOGWAS_EXPECTED_MUTUAL_INFO_HPP

#include <cmath>
#include <vector>
#include <algorithm>

#include "clustering/clustering.hpp"

#include "statistics/entropy.hpp"
#include "statistics/mutual_information.hpp"


namespace samogwas
{

typedef std::vector<std::vector<unsigned>> ContigencyMat;

namespace _private {
typedef std::vector<double> RealVec;
typedef std::vector<unsigned> IntVec;
typedef std::vector<RealVec> RealMat;
typedef std::vector<IntVec> IntMat;
}

inline double expected_mutual_information( const ContigencyMat& contigency );
inline double expected_mutual_information( const Partition& a, const Partition& b);

// inline double adjusted_mutual_information( const ContigencyMat& contigency );
inline double adjusted_mutual_information( const Partition& a, const Partition& b );
////////////////////////
inline void make_contigency( ContigencyMat& contigency, const Partition& a, const Partition& b );

inline void sum_over_rows(_private::IntVec&a, const ContigencyMat& contigency);
inline void sum_over_cols(_private::IntVec&a, const ContigencyMat& contigency);

template<typename CL>
unsigned max_element( const CL& a) {
  return *std::max_element(a.begin(), a.end());
}

template<typename VL>
void arrange( std::vector<VL>& a, const VL beg, const VL end) {
  auto size = (end - beg);
  a.resize(size, (VL)0);
  for (size_t i = 0; i < size; ++i) {
    a[i] = beg + i;
  }
}


inline void _compute_term_1( _private::RealVec& term1,
                             const _private::RealVec& nijs, const double N);

inline void _compute_outer_ab( _private::RealMat& ab_outer,
                               const _private::IntVec& a,
                               const _private::IntVec& b );

template<typename CL>
inline void _compute_log_vec( _private::RealVec& rs, const CL& v, const double op1 = 1, const double op2 = 0) {
  rs.resize(v.size(), 0.0);
  for (size_t r = 0; r < v.size(); ++r) {
    rs[r] = std::log(v[r]*op1 + op2);
  }
}
inline void _compute_start_end( _private::IntMat& beg, _private::IntMat& end,
                                const _private::IntVec& a, const _private::IntVec& b, const double N);

template<typename CL>
inline void _compute_gamma_lg( _private::RealVec& rs, const CL& inCl, double op1, double op2) {
  rs.resize(inCl.size(), 0.0);
  for (size_t i = 0; i < inCl.size(); ++i) {
    rs[i] = lgamma(inCl[i]*op1+op2);
  }
}


} // namespace samogwasends here.

/****************************** IMLEMENTATION BELOW THIS POINT **************************/
namespace samogwas
{

double expected_mutual_information(const ContigencyMat& contigency) {
  size_t R = contigency.size(), C = contigency[0].size();
  double N = 0.0, gln_N = 0, emi = 0, term2 = 0, term3 = 0, gln = 0;
  _private::RealVec gln_a, gln_b, gln_Na, gln_Nb, gln_nij, log_Nnij;
  _private::RealVec nijs, term1;
  _private::IntMat  start, end;
  _private::RealMat log_ab_outer;
  _private::IntVec a, b;
  sum_over_rows(a, contigency);  sum_over_cols(b, contigency);

  for (auto& v: a) { N += v; }

  auto nijs_sz = std::max(max_element(a), max_element(b));
  arrange(nijs, (double)0, (double)( nijs_sz + 1)); nijs[0] = 1;
  _compute_term_1(term1, nijs, N);
  _compute_outer_ab(log_ab_outer,a,b);
  _compute_log_vec(log_Nnij, nijs, N, 0);

  // _compute_gamma_lg(_private::RealVec &rs, const CL &inCl, double op1, double op2)
  _compute_gamma_lg( gln_a, a, 1, 1);
  _compute_gamma_lg( gln_b, b, 1, 1);
  _compute_gamma_lg( gln_Na, a, -1, N+1);
  _compute_gamma_lg( gln_Nb, b, -1, N+1);
  _compute_gamma_lg( gln_nij, nijs, 1, 1);
  gln_N = lgamma(N+1);

  _compute_start_end( start, end, a, b, N);

  for (size_t i = 0; i < R; ++i) {
    for (size_t j = 0; j < C; ++j) {
      for (size_t nij = start[i][j]; nij < end[i][j]; ++nij) {
        term2 = log_Nnij[nij] - log_ab_outer[i][j];
        gln = (gln_a[i] + gln_b[j] + gln_Na[i] + gln_Nb[j]
               - gln_N - gln_nij[nij] - lgamma(a[i] - nij + 1)
               - lgamma(b[j] - nij + 1)
               - lgamma(N - a[i] - b[j] + nij + 1));
        term3 = exp(gln);
        emi += (term1[nij] * term2 * term3);
      }
    }
  }
  return emi;
}

inline void sum_over_rows(_private::IntVec&a, const ContigencyMat& contigency) {
  size_t R = contigency.size();
  a.resize(R, 0);
  for (size_t r = 0; r < R; ++r) {
    for (auto v: contigency[r]) {
      a[r] += v;
    }
  }      
}

inline void sum_over_cols(_private::IntVec&b, const ContigencyMat& contigency) {
  size_t C = contigency[0].size(), R = contigency.size();
  b.resize(C, 0);
  for (size_t c = 0; c < C; ++c) {
    for (size_t r = 0; r < R; ++r) {
      // auto idx = r*C+c;
      b[c] += contigency[r][c];
    }
  }      
}


inline void _compute_term_1( _private::RealVec& term1, const _private::RealVec& nijs, const double N) {
  term1.resize(nijs.size(), 0.0);
  for (size_t i = 0; i < nijs.size(); ++i) {
    term1[i] = nijs[i]/N;
  }
}

inline void _compute_outer_ab( _private::RealMat& ab_outer,
                               const _private::IntVec& a,
                               const _private::IntVec& b) {
  size_t R = a.size(), C = b.size();
  ab_outer.resize(R, _private::RealVec(C, 0.0));
  for ( size_t r = 0; r < R; ++r) {
    for (size_t c = 0; c < C; ++c) {
      ab_outer[r][c] = std::log(a[r])+std::log(b[c]);
    }
  }
}


inline void _compute_start_end( _private::IntMat& beg, _private::IntMat& end,
                                const _private::IntVec& a, const _private::IntVec& b, const double N) {
  size_t R = a.size(), C = b.size();
  beg.resize(R, _private::IntVec(C, 0));
  end.resize(R, _private::IntVec(C, 0));
  for (size_t r = 0; r < R; ++r) {
    for (size_t c = 0; c < C; ++c) {
             // beg[r][c] = std::max((unsigned)1, (unsigned)(a[r]+b[c]-N));
      beg[r][c] = (1 > a[r]+b[c]-N) ? 1 : a[r]+b[c]-N;
      end[r][c] = std::min(a[r], b[c]) + 1;
    }
  }

}

inline double expected_mutual_information( const Partition& a, const Partition& b) {
  ContigencyMat contigency;
  make_contigency( contigency, a, b );
  return expected_mutual_information( contigency );
}

inline void make_contigency( ContigencyMat& contigency, const Partition& pA, const Partition& pB ) {
  auto max_cluster = std::max( pA.nbr_clusters(), pB.nbr_clusters() );
  contigency.resize(max_cluster, std::vector<unsigned>(max_cluster, 0));

  for ( auto pairA: pA.get_mapping() ) {
    auto item = pairA.first;
    auto labA = pairA.second;
    auto labB = pB.get_label(item);
    ++contigency[labA][labB];
  }
}

//////////////////////////////////////////////////////////////x
inline void partition_to_clustering( std::vector<int>& v, const Partition& a ) {
  v.resize( a.nbr_items(), 0);
  for ( size_t i = 0; i < a.nbr_items(); ++i ) {
    v[i] = a.get_label(i);
  }
}

inline double entropy_clustering( const Partition& a ) {
  std::vector<int> v;
  partition_to_clustering( v, a );

  Entropy<EMP> entropy;
  return entropy(v);
}

inline double mutual_information_clustering( const Partition& a, const Partition& b ) {
  std::vector<int> vA, vB;
  partition_to_clustering(vA, a);
  partition_to_clustering(vB, b);

  MutualInformation<EMP> mi;
  return mi(vA, vB);
}

inline double adjusted_mutual_information( const Partition& a, const Partition& b ) {
  ContigencyMat contigency;
  make_contigency( contigency, a, b );

  Entropy<EMP> entropy;
  MutualInformation<EMP> mut_info;

  std::vector<int> vA, vB;
  partition_to_clustering(vA, a);
  partition_to_clustering(vB, b);

  double eA = entropy(vA), eB = entropy(vB), mi = mut_info(vA,vB);
  double emi = expected_mutual_information( a, b );

  double deno = (std::max(eA, eB) - emi);
  double nomi = (mi - emi);

  if ( deno == 0 ) return 0.0;
  return nomi / deno;

}


} // namespace samogwas ends here.

/****************************************************************************************/
#endif // SAMOGWAS_EXPECTED_MUTUAL_INFO_HPP













