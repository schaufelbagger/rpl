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

#include "rpl-hello-regression-test.h"
#include "rpl-header-regression-test.h"
#include "rpl-header-option-regression-test.h"


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
  ns3::PacketMetadata::Enable ();
  // TestDuration for TestCase can be QUICK, EXTENSIVE or TAKES_FOREVER
  AddTestCase (new RplHeaderRegressionTest, TestCase::QUICK);
  AddTestCase (new RplHeaderOptionRegressionTest, TestCase::QUICK);
  //AddTestCase (new RplHelloRegressionTest, TestCase::QUICK);
}

// Do not forget to allocate an instance of this TestSuite
static RegressionTestSuite srplRegressionTestSuite;