#define BOOST_TEST_DYN_LINK
#include <BoostTestTargetConfig.h>
#include <boost/test/unit_test.hpp>

#include <memory>
#include "clustering/measures/expected_mutual_info.hpp"
#include "clustering/clustering.hpp"

// using namespace samogwas_test;
using namespace samogwas;
using namespace samogwas::_private;

BOOST_AUTO_TEST_SUITE( Measures_Test )

BOOST_AUTO_TEST_CASE( sum_rows_cols )
{
  size_t R = 4, C = 3;
  IntMat contigency(R, IntVec(C,0));
  for (size_t r = 0; r < R; ++r) {
    for (size_t c = 0; c < R; ++c) {
      contigency[r][c] = C*r+c;
    }
  }

  IntVec a,b;
  sum_over_rows(a, contigency);
  sum_over_cols(b, contigency);

  BOOST_CHECK_EQUAL(a.size(), 4);

  BOOST_CHECK_EQUAL(a[0], 3); BOOST_CHECK_EQUAL(a[1], 12);
  BOOST_CHECK_EQUAL(a[2], 21); BOOST_CHECK_EQUAL(a[3], 30);

  BOOST_CHECK_EQUAL(max_element(a), 30);

  BOOST_CHECK_EQUAL(b.size(), 3);
  BOOST_CHECK_EQUAL(b[0], 18); BOOST_CHECK_EQUAL(b[1], 22);  BOOST_CHECK_EQUAL(b[2], 26);
  BOOST_CHECK_EQUAL(max_element(b), 26);

  // log_outer
  RealMat expected_log_outer {
    { 3.98898405,  4.18965474,  4.35670883 },
    { 5.37527841,  5.5759491 ,  5.74300319 },
    { 5.9348942 ,  6.13556489,  6.30261898 },
    { 6.29156914,  6.49223984,  6.65929392 }
  };

  RealMat ab_outer;
  _compute_outer_ab( ab_outer,a,b);
  for ( size_t r = 0; r < R; ++r) {
    for (size_t c = 0; c < C; ++c) {
      //printf("%f, ", ab_outer[r][c]);
      BOOST_CHECK_CLOSE_FRACTION( ab_outer[r][c], expected_log_outer[r][c], 0.001);
    }
  }
}

BOOST_AUTO_TEST_CASE( gamma_log)
{
  size_t R = 4, C = 3;
  IntMat contigency(R, IntVec(C,0));
  for (size_t r = 0; r < R; ++r) {
    for (size_t c = 0; c < R; ++c) {
      contigency[r][c] = C*r+c;
    }
  }
  IntVec a,b;
  sum_over_rows(a, contigency);
  sum_over_cols(b, contigency);

  RealVec gln_a, gln_b;
  _compute_gamma_lg( gln_a, a, 1, 1);
  RealVec expected_gln_a {1.79175947,  19.9872145 ,  45.3801389 ,  74.65823635};
  for ( size_t r = 0; r < R; ++r) {
    BOOST_CHECK_CLOSE_FRACTION( gln_a[r], expected_gln_a[r], 0.001);
  }

}

BOOST_AUTO_TEST_CASE( start_end )
{
  size_t R = 4, C = 3;
  IntMat contigency(R, IntVec(C,0));
  for (size_t r = 0; r < R; ++r) {
    for (size_t c = 0; c < R; ++c) {
      contigency[r][c] = C*r+c;
    }
  }
  IntVec a,b;
  sum_over_rows(a, contigency);
  sum_over_cols(b, contigency);

  IntMat start, end;
  _compute_start_end( start, end, a, b, 78);
  IntMat expected_start {
    { 1,1,1},
    { 1,1,1},
    { 1,1,1},
    { 1,1,1},
  }, expected_end {
    { 4, 4, 4},
    {13, 13, 13},
    {19, 22, 22},
    {19, 23, 27}
  };

  for ( size_t r = 0; r < R; ++r ) {
    for ( size_t c = 0; c < C; ++c ) {
      BOOST_CHECK_EQUAL(start[r][c], expected_start[r][c]);
      BOOST_CHECK_EQUAL(end[r][c], expected_end[r][c]);
    }
  }
}

BOOST_AUTO_TEST_CASE( emi )
{
  IntMat contigency{{5, 1, 0},
    {1, 4, 1},
    {0, 2, 3}};
  size_t R = contigency.size(), C = contigency[0].size();

  IntVec a,b;
  sum_over_rows(a, contigency);
  sum_over_cols(b, contigency);

  IntMat start, end;
  _compute_start_end( start, end, a, b, 17);
  IntMat expected_start {
    { 1,1,1 },
    { 1,1,1},
    { 1,1,1 },
  }, expected_end {
    {7,7,5},
    {7,7,5},
    {6,6,5}
  };

  for ( size_t r = 0; r < R; ++r ) {
    for ( size_t c = 0; c < C; ++c ) {
      BOOST_CHECK_EQUAL(start[r][c], expected_start[r][c]);
      BOOST_CHECK_EQUAL(end[r][c], expected_end[r][c]);
    }
  }

  double emi = expected_mutual_information(contigency);
  BOOST_CHECK_CLOSE_FRACTION(0.15042, emi, 0.001);
}

BOOST_AUTO_TEST_CASE( test_contingency_matrix ) {
  std::vector<int> labA { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2 };
  std::vector<int> labB { 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 2, 0, 2, 2, 2, 1, 1 };
  Partition pA(labA), pB(labB);

  ContigencyMat expected_contigency { {5, 1, 0}, { 1, 4, 1 }, { 0, 2, 3 } };
  ContigencyMat contigency;
  make_contigency( contigency, pA, pB );
  BOOST_CHECK_EQUAL( contigency.size(), 3 );
  BOOST_CHECK_EQUAL( contigency[0].size(), 3 );

  for ( size_t i = 0; i < 3; ++i ) {
    for (int j = 0; j < 3; ++j ) {
      BOOST_CHECK_EQUAL( contigency[i][j], expected_contigency[i][j] );
    }
  }
  
}

BOOST_AUTO_TEST_CASE( adjusted_info ) {
  std::vector<int> labA { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2 };
  std::vector<int> labB { 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 2, 0, 2, 2, 2, 1, 1 };
  Partition pA(labA), pB(labB);

  ContigencyMat expected_contigency { {5, 1, 0}, { 1, 4, 1 }, { 0, 2, 3 } };
  ContigencyMat contigency;
  make_contigency( contigency, pA, pB );
  BOOST_CHECK_EQUAL( contigency.size(), 3 );
  BOOST_CHECK_EQUAL( contigency[0].size(), 3 );

  auto emi =  expected_mutual_information(pA, pB);
  BOOST_CHECK_CLOSE_FRACTION( emi, 0.1504247933626175, 0.001 );
}

BOOST_AUTO_TEST_CASE( adjusted_mi ) {
  std::vector<int> labA { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2 };
  std::vector<int> labB { 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 2, 0, 2, 2, 2, 1, 1 };
  Partition pA(labA), pB(labB);

  ContigencyMat expected_contigency { {5, 1, 0}, { 1, 4, 1 }, { 0, 2, 3 } };
  ContigencyMat contigency;
  make_contigency( contigency, pA, pB );
  BOOST_CHECK_EQUAL( contigency.size(), 3 );
  BOOST_CHECK_EQUAL( contigency[0].size(), 3 );

  auto ami = adjusted_mutual_information(pA, pB);
  BOOST_CHECK_CLOSE_FRACTION( ami, 0.27502, 0.001 );
}


BOOST_AUTO_TEST_SUITE_END()
















