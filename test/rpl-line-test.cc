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

// n0 (root) --- n1 --- n2
#include "ns3/core-module.h"
#include "ns3/rpl-helper.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/internet-apps-module.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/mobility-module.h"
#include "ns3/propagation-module.h"
#include "ns3/sixlowpan-module.h"
#include "ns3/applications-module.h"
#include "ns3/trace-helper.h"

#include "ns3/lr-wpan-module.h"
#include "ns3/yans-wifi-helper.h"


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
#include "rpl-line-test.h"

#define USE_SIXLOWPAN
#define USE_APPLICATION


// Do not put your test classes in namespace ns3.  You may find it useful
// to use the using directive to access the ns3 namespace directly
namespace ns3 {
NS_LOG_COMPONENT_DEFINE("RPL line test");
namespace rpl {

Ptr<rpl::RoutingProtocol> RplLineTest::GetRpl(Ptr <Node> node)
{
  Ptr<Ipv6> ipv6 = node->GetObject<Ipv6> ();
  NS_ASSERT_MSG (ipv6, "Ipv6 not installed on node");
  Ptr<Ipv6RoutingProtocol> proto = ipv6->GetRoutingProtocol ();
  NS_ASSERT_MSG (proto, "Ipv6 routing not installed on node");
  Ptr<rpl::RoutingProtocol> rpl = DynamicCast<rpl::RoutingProtocol> (proto);
  if (rpl)
  {
    return rpl;
  }else
  {
    return nullptr;
  }
}

void RplLineTest::RouteAddedTraceSink(std::string context, rpl::RplRoutingTableEntry entry)
{ 
  uint8_t buf[16];
  Ipv6Address dest;

  dest = entry.GetDest ();
  dest.GetBytes (buf);

  if (std::to_string(buf[15]) == context)
  {
    m_foundLastNode = true;
  }
}

// Add some help text to this case to describe what it is intended to test
RplLineTest::RplLineTest() : TestCase ("Rpl line test case")
{
}

// This destructor does nothing but we include it as a reminder that
// the test case should clean up after itself
RplLineTest::~RplLineTest()
{
}

//
// This method is the pure virtual method from class TestCase that every
// TestCase must implement
//
void RplLineTest::DoRun(void)
{


  //LogComponentEnable ("Rpl", LOG_LEVEL_DEBUG);
  //LogComponentEnable ("Ipv6L3Protocol", LOG_LEVEL_LOGIC);
  //LogComponentEnable ("Icmpv6L4Protocol", LOG_LEVEL_LOGIC);

  // parameters

  // distance of nodes
  int step = 80;
  Time simulationTime = Seconds (5);

  /// network
  /// nodes used in the example
  NodeContainer nodes;
  /// devices used in the example
  NetDeviceContainer devices;
  /// interfaces used in the example
  Ipv6InterfaceContainer interfaces;

  NS_LOG_UNCOND("rpl line test\n\n");

  // ---------------- Create Nodes -------------------------------

  // create nodes
  nodes.Create(m_numberOfNodes);
  MobilityHelper mobility;
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (step),
                                 "DeltaY", DoubleValue (0),
                                 "GridWidth", UintegerValue (m_numberOfNodes),
                                 "LayoutType", StringValue ("RowFirst"));
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (nodes);

  // ---------------- Create Devices -------------------------------

  LrWpanHelper lrWpanHelper;
  // Add and install the LrWpanNetDevice for each node
  NetDeviceContainer lrwpanDevices = lrWpanHelper.Install(nodes);

  // Fake PAN association and short address assignment.
  // This is needed because the lr-wpan module does not provide (yet)
  // a full PAN association procedure.
  lrWpanHelper.CreateAssociatedPan (lrwpanDevices , 1);

#ifdef USE_SIXLOWPAN
  SixLowPanHelper sixlowpan;
  devices = sixlowpan.Install (lrwpanDevices );
  sixlowpan.AddContext (devices, 0, Ipv6Prefix ("2001:2::"), Time (Hours (2)));
#else
  devices = lrwpanDevices;
#endif
  // ---------------- Install Internet Stack -------------------------------
  // install TCP/IP & RPL
  RplHelper rpl;
  // you can configure RPL attributes here using rpl.Set(name, value)

  InternetStackHelper stack;
  stack.SetIpv4StackInstall(false);
  stack.SetRoutingHelper (rpl);
  stack.Install (nodes);
  //rpl.AssignDisMop (NodeContainer (nodes.Get (1)) , rpl::DIS_MOP_SEND, Seconds (1), 5, RPL_DEFAULT_INSTANCE, rpl::MOP_STORING_NO_MULTICAST);
  rpl.AssignRoot (NodeContainer (nodes.Get (0)) );


  Ipv6AddressHelper ipv6;
  ipv6.SetBase (Ipv6Address ("2001:2::"), Ipv6Prefix (64));
  Ipv6InterfaceContainer deviceInterfaces;
  deviceInterfaces = ipv6.Assign (devices);

  for (int i = 0; i< m_numberOfNodes; ++i)
  {
    deviceInterfaces.SetForwarding (i, true);
  }
  
  GetRpl(nodes.Get(0))->TraceConnect("routeAdded", std::to_string(m_numberOfNodes), MakeCallback (&RplLineTest::RouteAddedTraceSink, this));

  // ---------------- Install Applications -------------------------------


  Simulator::Stop (simulationTime);
  
  lrWpanHelper.EnablePcapAll ("RPLEXAMPLEPCAP", true);
  
  Simulator::Run ();

  // check if last node is found in routing table of root node
  NS_TEST_ASSERT_MSG_EQ (m_foundLastNode, true, "did not find last node in RPL network");


  Simulator::Destroy ();

}


}
}