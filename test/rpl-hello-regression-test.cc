/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Authors: Alexander Baranyai
 */

#include <vector>
#include "rpl-hello-regression-test.h"
#include "ns3/simulator.h"
#include "ns3/rpl.h"
#include "ns3/rpl-helper.h"

namespace ns3 {
NS_LOG_COMPONENT_DEFINE("RPL hello regression test");
namespace rpl {

RplHelloRegressionTest::RplHelloRegressionTest () :
  TestCase ("Test RPL Hello messages generation"),
  m_time (Seconds (5))
{
}

RplHelloRegressionTest::~RplHelloRegressionTest ()
{
}

void RplHelloRegressionTest::DoRun ()
{
  CreateNodes ();

  Simulator::Stop (m_time);
  Simulator::Run ();
  Simulator::Destroy ();
}

void RplHelloRegressionTest::CreateNodes ()
{

}


}   //rpl
}   //ns3