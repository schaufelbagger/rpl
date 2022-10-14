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

/*
// turn of interfaces of n1 for some time and then reactivate them
//    n0
//   /  \
// n1    n2
//  \    |
//   \   n3
//    \ /
//     n4
*/

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
#include "ns3/node.h"

#include "ns3/lr-wpan-module.h"
#include "ns3/yans-wifi-helper.h"

#include "ns3/rpl-state.h"

#include "rpl-example-helper.h"
#include "timestamp-tag.h"

//#define USE_WIFI
#define USE_SIXLOWPAN
#define USE_APPLICATION

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("alternatePathExample");


int main (int argc, char *argv[])
{

  LogComponentEnable ("Rpl", LOG_LEVEL_DEBUG);
  //LogComponentEnable ("Ipv6L3Protocol", LOG_LEVEL_LOGIC);
  //LogComponentEnable ("Icmpv6L4Protocol", LOG_LEVEL_LOGIC);

  // parameters
  bool verbose = true;
  int numberOfNodes = 5;
  // distance of nodes
  double applicationStartSeconds = 100;
  double simulationTimeSeconds = 500;
  double silenceNodeTimeSeconds = 130;
  double turnOnNodeTimeSeconds = 300;

#ifdef USE_APPLICATION
  double trafficInterval = 10;
  int maxPackets = 2000;
  uint32_t packetSize = 10;
  //uint32_t maxPacketCount = 5;
  Time interPacketInterval = Seconds (1.);
#endif

  int run = 1;
  std::string routingProtocol ("rpl");
  std::string rplConfigFilename ("rplConfig.csv");
  int appSetup = 0;
  int networkSetup = 0;
  uint8_t dioIntervalDoublings = DEFAULT_DIO_INTERVAL_DOUBLINGS;
  uint8_t dioIntervalMin = DEFAULT_DIO_INTERVAL_MIN;
  /// network
  /// nodes used in the example
  NodeContainer nodes;
  /// devices used in the example
  NetDeviceContainer devices;
  /// interfaces used in the example
  Ipv6InterfaceContainer interfaces;


  CommandLine cmd (__FILE__);
  cmd.AddValue ("verbose", "Tell application to log if true", verbose);
  cmd.AddValue("routingProtocol", "the routing protocol used", routingProtocol);
  cmd.AddValue("rplConfigFilename", "filename of the RPL configuration", rplConfigFilename);
  cmd.AddValue("run", "the run number", run);
  cmd.AddValue("simulationTime", "the simulation time in seconds", simulationTimeSeconds);
  cmd.AddValue("applicationStart", "the application start time in seconds", applicationStartSeconds);
  cmd.AddValue("silenceNodeTime", "the time when the node 1 shall be silenced in seconds", silenceNodeTimeSeconds);
  cmd.AddValue("turnOnNodeTime", "the time when node 1 shall be turned on again in seconds", turnOnNodeTimeSeconds);
  cmd.AddValue("trafficInterval", "the intervall between data messages are sent", trafficInterval);
  cmd.AddValue("dioIntervalDoublings", "the RPL DIO Trickle timer Interval Doublings parameter", dioIntervalDoublings);
  cmd.AddValue("dioIntervalMin", "the RPL DIO Trickle timer Interval Min parameter", dioIntervalMin);

  
  //cmd.AddValue("numberOfNodes", "number of nodes", numberOfNodes);

  cmd.Parse (argc,argv);

  RngSeedManager::SetSeed (1);
  RngSeedManager::SetRun (run);

  std::string paramString = get_param_string(routingProtocol, run, numberOfNodes, trafficInterval, applicationStartSeconds, simulationTimeSeconds, networkSetup, appSetup);
  paramString = "_silenceNodeTime_" + std::to_string(silenceNodeTimeSeconds) + "_turnOnNodeTime_" + std::to_string(turnOnNodeTimeSeconds) + "_DIDoublings_" + std::to_string(dioIntervalDoublings) + "_DIMin_" + std::to_string(dioIntervalMin) + paramString;

  Time applicationStart = Seconds (applicationStartSeconds);
  Time simulationTime = Seconds (simulationTimeSeconds);
  Time silenceNodeTime = Seconds (silenceNodeTimeSeconds);
  Time turnOnNodeTime = Seconds (turnOnNodeTimeSeconds);
  //Config::SetDefault ("ns3::Icmpv6L4Protocol::DAD", BooleanValue (false));
  //Config::SetDefault ("ns3::Icmpv6L4Protocol::MaxUnicastSolicit", IntegerValue (0));
  //Config::SetDefault ("ns3::Icmpv6L4Protocol::MaxMulticastSolicit", IntegerValue (0));
  //Config::SetDefault ("ns3::Icmpv6L4Protocol::RetransmissionTime", TimeValue (Seconds(60*60)));
  //Config::SetDefault ("ns3::Icmpv6L4Protocol::DelayFirstProbe", TimeValue (Seconds(60*60)));
  
  //Config::SetDefault ("ns3::RipNg::SplitHorizon", EnumValue (RipNg::POISON_REVERSE));
  
  NS_LOG_UNCOND("rpl alt-path example\n\n");

  // ---------------- Create Nodes -------------------------------

  // create nodes
  
  MobilityHelper mobility;
  //std::list<ns3::Vector2D> nodePositions;
  //nodePositions.push_back (Vector2D ());
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add(Vector3D (100,0,0));
  positionAlloc->Add(Vector3D (50,70,0));
  positionAlloc->Add(Vector3D (160,60,0));
  positionAlloc->Add(Vector3D (170,120,0));
  positionAlloc->Add(Vector3D (70,150,0));
  //positionAlloc->Add("position_" + mode + ".csv");
  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  numberOfNodes = positionAlloc->GetSize();

  NS_ASSERT(numberOfNodes > 0);


  nodes.Create(numberOfNodes);
  mobility.Install (nodes);

  // ---------------- Create Devices -------------------------------


  LrWpanHelper lrWpanHelper;
  // Add and install the LrWpanNetDevice for each node
  NetDeviceContainer lrwpanDevices = lrWpanHelper.Install(nodes);

  // Fake PAN association and short address assignment.
  // This is needed because the lr-wpan module does not provide (yet)
  // a full PAN association procedure.
  lrWpanHelper.AssociateToPan (lrwpanDevices , 1);

#ifdef USE_SIXLOWPAN
  SixLowPanHelper sixlowpan;
  devices = sixlowpan.Install (lrwpanDevices );
  sixlowpan.AddContext (devices, 0, Ipv6Prefix ("2001:2::"), Time (Hours (2)));
#else
  devices = lrwpanDevices;
#endif
  // ---------------- Install Internet Stack -------------------------------
  // install TCP/IP & routing protocol

  InternetStackHelper stack;
  stack.SetIpv4StackInstall(false);
  Ipv6InterfaceContainer deviceInterfaces;
  
  if (routingProtocol == "rpl")
  {
    RplHelper rpl;
    // you can configure RPL attributes here using rpl.Set(name, value)
    // DIO Intervall Max = 2^DIOIntervalMin * 2^DIOIntervalDoublings
    // DIO Intervall Max = 8.192s
    rpl.Set ("DIOIntervalDoublings", UintegerValue (dioIntervalDoublings));
    rpl.Set ("DIOIntervalMin", UintegerValue (dioIntervalMin));

    stack.SetRoutingHelper (rpl);
    stack.Install (nodes);

    rpl.AssignRoot (NodeContainer (nodes.Get (0)) );
    
    

    Ipv6AddressHelper ipv6;
    ipv6.SetBase (Ipv6Address ("2001:2::"), Ipv6Prefix (64));
    
    deviceInterfaces = ipv6.Assign (devices);
  }
  else if (routingProtocol == "aodv")
  {
    NS_ABORT_MSG ( "TODO for IPv6 adapted AODV not implemented!");
  }
  else
  {
    NS_ABORT_MSG ( "no valid routing protocol given");
  }




  for (int i = 0; i< numberOfNodes; ++i)
  {
    deviceInterfaces.SetForwarding (i, true);
    if (routingProtocol == "rpl")
    {
      AsciiTraceHelper asciiTraceHelper;
      Ptr<OutputStreamWrapper> updatedPrefParent = asciiTraceHelper.CreateFileStream (
        "RPLEXAMPLE_updatedPrefParent_node_" + std::to_string(i) + paramString + ".txt");
      GetRpl(nodes.Get(i))->TraceConnectWithoutContext("UpdatedPrefParent", MakeBoundCallback (&UpdatePrefParentTraceSink, updatedPrefParent));

      Ptr<OutputStreamWrapper> routeAddedStream = asciiTraceHelper.CreateFileStream (
        "RPLEXAMPLE_routeAdded_node_" + std::to_string(i) + paramString + ".txt");
      GetRpl(nodes.Get(i))->TraceConnectWithoutContext("routeAdded", MakeBoundCallback (&RouteAddedTraceSink, routeAddedStream));
    }
    /*else if (routingProtocol == "ripng")
    {
      RipNgHelper routingHelper;
      Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper> (&std::cout);
      routingHelper.PrintRoutingTableAt (Seconds (310.0), nodes.Get(i), routingStream);

      AsciiTraceHelper asciiTraceHelper;
      Ptr<OutputStreamWrapper> routeAddedStream = asciiTraceHelper.CreateFileStream (
          "RPLEXAMPLE_routeAdded_routingProtocol_" + routingProtocol + "_node_" + std::to_string(i) + "_silenceNodeTime_" + std::to_string(silenceNodeTimeSeconds) + "_turnOnNodeTime_" + std::to_string(turnOnNodeTimeSeconds) + paramString + ".txt");
      GetRipNg(nodes.Get(i))->TraceConnectWithoutContext("routeAdded", MakeBoundCallback (&RipNgRouteAddedTraceSink, routeAddedStream));
    }*/
  }
  


  // ---------------- Install Applications -------------------------------

  // Add random application to generate (passive) traffic
  UdpEchoClientHelper udpClientHelper = UdpEchoClientHelper(deviceInterfaces.GetAddress(0,1), 6000);
  // Multicast disabled for now!
  //UdpEchoClientHelper udpClientHelper = UdpEchoClientHelper(Ipv6Address (IPV6_GROUP_ADDR), 6000);
  UdpEchoServerHelper udpServerHelper = UdpEchoServerHelper(6000);
  
  // Add random app to three nodes:
  for (int n : {numberOfNodes-1})
  {
    udpClientHelper.SetAttribute ("RemoteAddress", AddressValue (deviceInterfaces.GetAddress (0,1)));
    udpClientHelper.SetAttribute ("RemotePort", UintegerValue (6000));
    udpClientHelper.SetAttribute ("PacketSize", UintegerValue (packetSize));
    udpClientHelper.SetAttribute ("Interval", TimeValue (Seconds (trafficInterval)));
    udpClientHelper.SetAttribute ("MaxPackets", UintegerValue (maxPackets));
    ApplicationContainer apps = udpClientHelper.Install(nodes.Get(n));

    apps.Start (applicationStart);
    apps.Stop (simulationTime);

    AsciiTraceHelper asciiTraceHelper;
    Ptr<OutputStreamWrapper> updServerRxWrapper = asciiTraceHelper.CreateFileStream ( "RPLEXAMPLE_udpClientReceive_node_" + std::to_string(n) + paramString + ".txt");
    apps.Get (0)->TraceConnectWithoutContext("RxWithAddresses", MakeBoundCallback (&UdpRxTraceWithAddressesSink, updServerRxWrapper));
    Ptr<OutputStreamWrapper> updClientTxWrapper = asciiTraceHelper.CreateFileStream ( "RPLEXAMPLE_udpClientSend_node_" + std::to_string(n) + paramString + ".txt");
    apps.Get (0)->TraceConnectWithoutContext("TxWithAddresses", MakeBoundCallback (&UdpTxTraceWithAddressesSink, updClientTxWrapper));
  }

  for (int n : {0})
  {
    udpServerHelper.SetAttribute ("Port", UintegerValue (6000));
    ApplicationContainer apps =  udpServerHelper.Install(nodes.Get(n));
    apps.Start (Seconds (1.0));
    apps.Stop (simulationTime);

    AsciiTraceHelper asciiTraceHelper;
    Ptr<OutputStreamWrapper> updServerWrapper = asciiTraceHelper.CreateFileStream ( "RPLEXAMPLE_udpServerReceive_node_" + std::to_string(n) + paramString + ".txt");
    apps.Get (0)->TraceConnectWithoutContext("RxWithAddresses", MakeBoundCallback (&UdpRxTraceWithAddressesSink, updServerWrapper));
  }

  Simulator::Schedule(silenceNodeTime, &SilenceNode, nodes.Get (1), 1);
  Simulator::Schedule(turnOnNodeTime, &TurnOnNode, nodes.Get (1), 1, Ipv6Address("2001:2::ff:fe00:2"));

  Simulator::Stop (simulationTime);
  
  lrWpanHelper.EnablePcapAll ("RPLEXAMPLEPCAP", true);
  
  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}


