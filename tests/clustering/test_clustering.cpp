#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE
#endif

#include <boost/test/included/unit_test.hpp>
#include <iostream>
#include <BoostTestTargetConfig.h>
BOOST_AUTO_TEST_SUITE( Test_Clustering_Test )

BOOST_AUTO_TEST_CASE( test_case1 )
{
  BOOST_CHECK_EQUAL(2,2);
}



BOOST_AUTO_TEST_SUITE_END()
