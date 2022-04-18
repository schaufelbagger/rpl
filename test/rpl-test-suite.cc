/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2022 COPYRIGHTHOLDER
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Alexander Baranyai <e1525251@student.tuwien.ac.at>
 */
// Include a header file from your module to test.
#include "ns3/rpl.h"

// An essential include is test.h
#include "ns3/test.h"

#include "rpl-two-nodes-test.h"

// Do not put your test classes in namespace ns3.  You may find it useful
// to use the using directive to access the ns3 namespace directly
using namespace ns3;
using namespace rpl;

// This is an example TestCase.
class RplTestCase1 : public TestCase
{
public:
    RplTestCase1();
    virtual ~RplTestCase1();

private:
    virtual void DoRun (void);
};

// Add some help text to this case to describe what it is intended to test
RplTestCase1::RplTestCase1() : TestCase ("Rpl test case (does nothing)")
{
}

// This destructor does nothing but we include it as a reminder that
// the test case should clean up after itself
RplTestCase1::~RplTestCase1()
{
}

//
// This method is the pure virtual method from class TestCase that every
// TestCase must implement
//
void RplTestCase1::DoRun(void)
{
    // A wide variety of test macros are available in src/core/test.h
    NS_TEST_ASSERT_MSG_EQ (true, true, "true doesn't equal true for some reason");
    // Use this one for floating point comparisons
    NS_TEST_ASSERT_MSG_EQ_TOL (0.01, 0.01, 0.001, "Numbers are not equal within tolerance");
}

// The TestSuite class names the TestSuite, identifies what type of TestSuite,
// and enables the TestCases to be run.  Typically, only the constructor for
// this class must be defined
//
class RplTestSuite : public TestSuite
{
public:
    RplTestSuite();
};

RplTestSuite::RplTestSuite() : TestSuite ("rpl", UNIT)
{
    // TestDuration for TestCase can be QUICK, EXTENSIVE or TAKES_FOREVER
    AddTestCase (new RplTestCase1, TestCase::QUICK);
    AddTestCase (new RplTestTwoNodes, TestCase::QUICK);
}

// Do not forget to allocate an instance of this TestSuite
static RplTestSuite srplTestSuite;

