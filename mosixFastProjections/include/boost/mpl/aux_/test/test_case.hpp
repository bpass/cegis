
#ifndef BOOST_MPL_AUX_TEST_TEST_CASE_HPP_INCLUDED
#define BOOST_MPL_AUX_TEST_TEST_CASE_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2002-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /home/dmattli/cvs/mosixFastProjections/include/boost/mpl/aux_/test/test_case.hpp,v $
// $Date: 2005/03/11 23:59:18 $
// $Revision: 1.1 $

#include <boost/preprocessor/cat.hpp>

#define MPL_TEST_CASE() void BOOST_PP_CAT(test,__LINE__)()

#endif // BOOST_MPL_AUX_TEST_TEST_CASE_HPP_INCLUDED
