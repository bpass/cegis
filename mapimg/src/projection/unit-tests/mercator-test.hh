/* File: mercator-test.cpp
   Programmer: David Mattli

   Copyright 2007 U.S. Geological Survey

   This program is free software; you can redistribute it and/or
   modify if under the terms of the GNU General Public License version
   2 as published by the Free Software Foundation.
*/

#define BOOST_TEST_MODULE mercator test
#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include <memory>

using boost::unit_test_framework::test_suite;
using boost::unit_test_framework::test_case;

#include "../mercator.hh"

class mercator_test
{
public:
    mercator_test();
    void test_forward();
    void test_inverse();
    void test_param();

private:
    trans::mercator<double> test_mer;
};

class mercator_test_suite : public test_suite
{
 public:
  mercator_test_suite() : test_suite("Mercator Test Suite");
};


