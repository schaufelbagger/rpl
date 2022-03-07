/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

// Include a header file from your module to test.
#include "ns3/rpl.h"
#include "ns3/rpl-helper.h"

// An essential include is test.h
#include "ns3/test.h"

#include "ns3/internet-module.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/internet-apps-module.h"
#include "ns3/simple-net-device-helper.h"
#include "ns3/simulator.h"
#include "ns3/double.h"
#include "ns3/uinteger.h"
#include "ns3/string.h"
#include "rpl-two-nodes-test.h"


// Do not put your test classes in namespace ns3.  You may find it useful
// to use the using directive to access the ns3 namespace directly
namespace ns3 {
NS_LOG_COMPONENT_DEFINE("RPL two nodes test");
namespace rpl {



// Add some help text to this case to describe what it is intended to test
RplTestTwoNodes::RplTestTwoNodes() : TestCase ("Rpl two nodes test case")
{
}

// This destructor does nothing but we include it as a reminder that
// the test case should clean up after itself
RplTestTwoNodes::~RplTestTwoNodes()
{
}

//
// This method is the pure virtual method from class TestCase that every
// TestCase must implement
//
void RplTestTwoNodes::DoRun(void)
{

  NS_LOG_UNCOND("asdaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaasd\n\n");
  //TODO uncomment when rpl helper does not return an empty helper pointer
  //CreateNodes();
  // A wide variety of test macros are available in src/core/test.h
  NS_TEST_ASSERT_MSG_EQ (true, true, "true doesn't equal true for some reason");
  // Use this one for floating point comparisons
  NS_TEST_ASSERT_MSG_EQ_TOL (0.01, 0.01, 0.001, "Numbers are not equal within tolerance");
}

void RplTestTwoNodes::CreateNodes()
{
  // create 2 nodes
  NodeContainer nodes;
  nodes.Create(2);
  // install TCP/IP & RPL
  RplHelper rpl;
  InternetStackHelper internetv6;
  internetv6.SetRoutingHelper(rpl);
  internetv6.Install(nodes);


  // create channel & devices
  SimpleNetDeviceHelper simpleNetHelper;
  simpleNetHelper.SetDeviceAttribute("DataRate", StringValue ("5Mbps"));
  simpleNetHelper.SetChannelAttribute("Delay", StringValue ("2ms"));
  NetDeviceContainer nd = simpleNetHelper.Install (nodes);

  NS_LOG_INFO("Create networks and assign IPv6 Addresses.");
  Ipv6AddressHelper ipv6;
  ipv6.SetBase(Ipv6Address("2001:1::"), Ipv6Prefix (64));
  Ipv6InterfaceContainer i1 = ipv6.Assign(nd);
  //i1.SetForwarding (1, true);
  //i1.SetDefaultRouteInAllNodes (1);

}

}
}