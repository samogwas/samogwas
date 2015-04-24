#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE
#endif

#include <BoostTestTargetConfig.h>
#include <boost/test/unit_test.hpp>

#include <iostream>
#include "statistics/fisher.hpp"
#include "statistics/test_statistics.hpp"
BOOST_AUTO_TEST_SUITE( Statistics_Suite )

BOOST_AUTO_TEST_CASE( Test_Fisher ) {
  std::vector<int> geno;
  for ( int i = 0; i < 12; ++i ) geno.push_back(0);
  for ( int i = 0; i < 5; ++i ) geno.push_back(1);
  for ( int i = 0; i < 7; ++i ) geno.push_back(0);
  for ( int i = 0; i < 7; ++i ) geno.push_back(1);
  std::vector<int> pheno;
  for ( int i = 0; i < 17; ++i ) pheno.push_back(0);
  for ( int i = 0; i < 14; ++i ) pheno.push_back(1);  
  stats::StatisticTest<stats::FISHER> fisher;
  BOOST_CHECK_CLOSE( fisher(geno, pheno, 2, 2), 0.2883081, 0.0001 );
}


BOOST_AUTO_TEST_CASE( Test_G2 )
{
  std::vector<int> geno;
  for ( int i = 0; i < 12; ++i ) geno.push_back(0);
  for ( int i = 0; i < 5; ++i ) geno.push_back(1);
  for ( int i = 0; i < 7; ++i ) geno.push_back(0);
  for ( int i = 0; i < 7; ++i ) geno.push_back(1);
  std::vector<int> pheno;
  for ( int i = 0; i < 17; ++i ) pheno.push_back(0);
  for ( int i = 0; i < 14; ++i ) pheno.push_back(1);
  
  stats::StatisticTest<stats::G2> g2;
  BOOST_CHECK_CLOSE( g2(geno, pheno, 2, 2), 0.240864, 0.0001 );
}

BOOST_AUTO_TEST_CASE( Test_G2_Corrected )
{
  std::vector<int> geno;
  for ( int i = 0; i < 12; ++i ) geno.push_back(0);
  for ( int i = 0; i < 5; ++i ) geno.push_back(1);
  for ( int i = 0; i < 7; ++i ) geno.push_back(0);
  for ( int i = 0; i < 7; ++i ) geno.push_back(1);
  
  std::vector<int> pheno;
  for ( int i = 0; i < 17; ++i ) pheno.push_back(0);
  for ( int i = 0; i < 14; ++i ) pheno.push_back(1);
  
  stats::StatisticTest<stats::G2_YATES> g2;
   BOOST_CHECK_CLOSE( g2(geno, pheno, 2, 2), 0.4233159, 0.0001 );
}

BOOST_AUTO_TEST_CASE( Test_ChiSqrt )
{
  std::vector<int> geno;
  for ( int i = 0; i < 12; ++i ) geno.push_back(0);
  for ( int i = 0; i < 5; ++i ) geno.push_back(1);
  for ( int i = 0; i < 7; ++i ) geno.push_back(0);
  for ( int i = 0; i < 7; ++i ) geno.push_back(1);
  std::vector<int> pheno;
  for ( int i = 0; i < 17; ++i ) pheno.push_back(0);
  for ( int i = 0; i < 14; ++i ) pheno.push_back(1);
  
  stats::StatisticTest<stats::CHISQ> chisq;
  BOOST_CHECK_CLOSE( chisq(geno, pheno, 2, 2), 0.2415293, 0.0001 );
}

BOOST_AUTO_TEST_CASE( Test_ChiSqrt_Corrected )
{
  std::vector<int> geno;
  for ( int i = 0; i < 12; ++i ) geno.push_back(0);
  for ( int i = 0; i < 5; ++i ) geno.push_back(1);
  for ( int i = 0; i < 7; ++i ) geno.push_back(0);
  for ( int i = 0; i < 7; ++i ) geno.push_back(1);
  
  std::vector<int> pheno;
  for ( int i = 0; i < 17; ++i ) pheno.push_back(0);
  for ( int i = 0; i < 14; ++i ) pheno.push_back(1);
  
  stats::StatisticTest<stats::CHISQ_YATES> chisq;
  BOOST_CHECK_CLOSE( chisq(geno, pheno, 2, 2), 0.4233054, 0.0001 );
}

BOOST_AUTO_TEST_SUITE_END()
