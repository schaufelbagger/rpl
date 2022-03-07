/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Authors: Alexander Baranyai
 */


// Include a header file from your module to test.
#include "ns3/rpl.h"

// An essential include is test.h
#include "ns3/test.h"

#include "rpl-hello-regression-test.h"


using namespace ns3;
using namespace rpl;

/**
 * \ingroup rpl-test
 * \ingroup tests
 *
 * Various rpl regression tests
 */
class RegressionTestSuite : public TestSuite
{
public:
  RegressionTestSuite();
};

RegressionTestSuite::RegressionTestSuite () : TestSuite ("rpl-reg", SYSTEM)
{
  // TestDuration for TestCase can be QUICK, EXTENSIVE or TAKES_FOREVER
  AddTestCase (new RplHelloRegressionTest, TestCase::QUICK);
}

// Do not forget to allocate an instance of this TestSuite
static RegressionTestSuite srplRegressionTestSuite;