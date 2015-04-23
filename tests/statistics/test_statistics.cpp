#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE
#endif
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_SUITE( Test_Statistics_Test )

BOOST_AUTO_TEST_CASE( test_case1 )
{
  BOOST_CHECK_EQUAL(1,1);
}

BOOST_AUTO_TEST_SUITE_END()
