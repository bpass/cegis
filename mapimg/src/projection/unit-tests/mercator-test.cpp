/* File: mercator-test.cpp
 *  Programmer: David Mattli
 *
 * Copyright 2007 U.S. Geological Survey
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 as published by the Free Software Foundation.
 */

#include <boost/test/included/unit_test_framework.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include "../mercator.hh"

using namespace boost::unit_test_framework;
using namespace trans;

#define MAJ 6378206.4 // Clarke 1866 ellipsoid
#define MIN 6356583.8
#define TOL 3  // maximum percent difference

void constructors_test ()
{
  mercator<double> m1(MAJ, MIN); 
}

void forward_test ()
{
  mercator<long double> m1(MAJ, MIN); 
  
  // Test forward_y
  BOOST_CHECK_CLOSE(m1.forward_y(0.0, 0.0)/MAJ+1, 1.0L, TOL);
  BOOST_CHECK_CLOSE(m1.forward_y(0.0872664626,0.0872664626)/MAJ, 0.08679, TOL); // 5 deg
  BOOST_CHECK_CLOSE(m1.forward_y(0.174532925,0.174532925)/MAJ, 0.17425, TOL); // 10 deg
  BOOST_CHECK_CLOSE(m1.forward_y(0.261799388, 0.26179938)/MAJ, 0.26309, TOL); // 15 deg

  // Test forward_x
  BOOST_CHECK_CLOSE(m1.forward_x(0.0, 0.0)/MAJ+1, 1.0, TOL);
  BOOST_CHECK_CLOSE(m1.forward_x(0.0872664626, 0.0872664626)/MAJ, 0.017453*5, TOL);
  BOOST_CHECK_CLOSE(m1.forward_x(0.174532925,0.174532925)/MAJ, 0.017453*10, TOL);
  BOOST_CHECK_CLOSE(m1.forward_x(0.261799388, 0.26179938)/MAJ, 0.017453*15, TOL);

  return;
}

void inverse_test ()
{
  mercator<double> m1(MAJ, MIN);
  
  BOOST_CHECK_CLOSE(m1.inverse_lon(0.0,0.0)+1.0, 1.0, TOL);

  return;
}

test_suite*
init_unit_test_suite( int argc, char* argv[] )
{
  test_suite* test = BOOST_TEST_SUITE( "mercator test" );

  test->add( BOOST_TEST_CASE( &constructors_test ));
  test->add( BOOST_TEST_CASE( &forward_test ));
  test->add( BOOST_TEST_CASE( &inverse_test ));
  
  return test;
}
