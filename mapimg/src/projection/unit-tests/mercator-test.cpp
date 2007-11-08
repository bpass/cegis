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
#define DIFF 50.0

void constructors_test ()
{
  mercator<double> m1(MAJ, MIN); 
}

void forward_test ()
{
  mercator<double> m1(MAJ, MIN); 
  
  BOOST_CHECK_CLOSE(m1.forward_y(0.0, 0.0)/MAJ, 1e-20, 1e10);
  //BOOST_CHECK_EQUAL(m1.forward_y(50, 50), 3773);

}


test_suite*
init_unit_test_suite( int argc, char* argv[] )
{
  test_suite* test = BOOST_TEST_SUITE( "mercator test" );

  test->add( BOOST_TEST_CASE( &constructors_test ));
  test->add( BOOST_TEST_CASE( &forward_test ));
  
  return test;
}
