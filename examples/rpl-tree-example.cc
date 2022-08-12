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
// 
//        n0
//       / \
//      /   \
//    n1     n2
//   / \     / \
//  n3  n4  n5  n6
// /
//n7

//        n0
//      /  |
//    n1  n2
//   / |   | \
//  n3 n4 n5  n6
// /
//n7
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

#include "ns3/lr-wpan-module.h"
#include "ns3/yans-wifi-helper.h"

//#define USE_WIFI
#define USE_SIXLOWPAN
#define USE_APPLICATION

using namespace ns3;


Ptr<rpl::RoutingProtocol> GetRpl(Ptr <Node> node){
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

void UpdatePrefParentTraceSink(Ptr<OutputStreamWrapper> stream, rpl::RplNode rplNode)
{ 
  *stream->GetStream () << Simulator::Now().GetSeconds();
  *stream->GetStream () << ", " << rplNode.address;
  *stream->GetStream () << ", " << rplNode.rank;
  *stream->GetStream () << std::endl;                                                                      
}

int main (int argc, char *argv[])
{

  LogComponentEnable ("Rpl", LOG_LEVEL_DEBUG);
  //LogComponentEnable ("Ipv6L3Protocol", LOG_LEVEL_LOGIC);
  //LogComponentEnable ("Icmpv6L4Protocol", LOG_LEVEL_LOGIC);

  // parameters
  bool verbose = true;
  int numberOfNodes = 8;
  // distance of nodes
  int xStep = 90;
  int yStep = 50;
  Time applicationStart = Seconds (10);
  Time simulationTime = Seconds (15);
#ifdef USE_APPLICATION
  uint32_t packetSize = 10;
  //uint32_t maxPacketCount = 5;
  Time interPacketInterval = Seconds (1.);
#endif

  /// network
  /// nodes used in the example
  NodeContainer nodes;
  /// devices used in the example
  NetDeviceContainer devices;
  /// interfaces used in the example
  Ipv6InterfaceContainer interfaces;

  CommandLine cmd (__FILE__);
  cmd.AddValue ("verbose", "Tell application to log if true", verbose);

  cmd.Parse (argc,argv);

  Config::SetDefault ("ns3::Icmpv6L4Protocol::DAD", BooleanValue (false));
  Config::SetDefault ("ns3::Icmpv6L4Protocol::MaxUnicastSolicit", IntegerValue (0));
  Config::SetDefault ("ns3::Icmpv6L4Protocol::MaxMulticastSolicit", IntegerValue (0));
  Config::SetDefault ("ns3::Icmpv6L4Protocol::RetransmissionTime", TimeValue (Seconds(60*60)));
  Config::SetDefault ("ns3::Icmpv6L4Protocol::DelayFirstProbe", TimeValue (Seconds(60*60)));

  NS_LOG_UNCOND("rpl example\n\n");

  // ---------------- Create Nodes -------------------------------

  // create nodes
  nodes.Create(numberOfNodes);

  int treeLayers = std::ceil (std::log2 (numberOfNodes+1));
  int lastLayerNodes = pow(2, treeLayers);
  int rowLength = (lastLayerNodes * xStep);
  int xBegin = rowLength / 2;
  int iterNodeNum = 0;
  MobilityHelper mobility;
  for (int layer = 0; layer < treeLayers; ++layer)
  {
    NodeContainer layerNodes = NodeContainer ();
    int numLayerNodes = pow(2, layer);
    int numLastLayersNodes = iterNodeNum;
    mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                  "MinX", DoubleValue (xBegin),
                                  "MinY", DoubleValue (yStep * layer),
                                  "DeltaX", DoubleValue (xStep),
                                  "DeltaY", DoubleValue (0),
                                  "GridWidth", UintegerValue (numberOfNodes),
                                  "LayoutType", StringValue ("RowFirst"));
    mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
    xBegin = xBegin - xStep;

    for (; iterNodeNum < (numLastLayersNodes+numLayerNodes); ++iterNodeNum)
    {
      if (iterNodeNum >= numberOfNodes)
      {
        break;
      }
      layerNodes.Add (nodes.Get (iterNodeNum));
    }
    mobility.Install (layerNodes);
  }
  

  // ---------------- Create Devices -------------------------------
  /*
  WifiMacHelper wifiMac;
  wifiMac.SetType ("ns3::AdhocWifiMac");
  YansWifiPhyHelper wifiPhy;
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
  wifiPhy.SetChannel (wifiChannel.Create ());
  WifiHelper wifi;
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager", "DataMode", StringValue ("OfdmRate6Mbps"), "RtsCtsThreshold", UintegerValue (0));
  devices = wifi.Install (wifiPhy, wifiMac, nodes); 
  if (verbose)
  {
    wifiPhy.EnablePcapAll (std::string ("rpl"));
  }*/

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

  for (int i = 0; i< numberOfNodes; ++i)
  {
    deviceInterfaces.SetForwarding (i, true);
    AsciiTraceHelper asciiTraceHelper;
    Ptr<OutputStreamWrapper> updatedPrefParent = asciiTraceHelper.CreateFileStream (
          "stateChanges_node_" + std::to_string(i) + "_" + "updatedPrefParent" + ".txt");
    GetRpl(nodes.Get(i))->TraceConnectWithoutContext("UpdatedPrefParent", MakeBoundCallback (&UpdatePrefParentTraceSink, updatedPrefParent));
  }
  


  /*<Node> node;
  for (NodeContainer::Iterator i = nodes.Begin (); i != nodes.End (); ++i)
  {
    node = (*i);
    Ptr<Ipv6> ipv6 = node->GetObject<Ipv6> ();
    NS_ASSERT_MSG (ipv6, "Ipv6 not installed on node");
    Ptr<Ipv6RoutingProtocol> proto = ipv6->Get
  }*/


  // ---------------- Install Applications -------------------------------
/*#ifdef USE_APPLICATION
  Ping6Helper ping6;

#ifdef USE_SIXLOWPAN
  ping6.SetLocal (deviceInterfaces.GetAddress (0, 1));
  ping6.SetRemote (deviceInterfaces.GetAddress (numberOfNodes-1, 1));
#else
  ping6.SetLocal (deviceInterfaces.GetAddress (0, 0));
  ping6.SetRemote (deviceInterfaces.GetAddress (numberOfNodes-1, 0));
#endif
  ping6.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
  ping6.SetAttribute ("Interval", TimeValue (interPacketInterval));
  ping6.SetAttribute ("PacketSize", UintegerValue (packetSize));
  ApplicationContainer apps = ping6.Install (nodes.Get (0));

  apps.Start (Seconds (5.0));
  apps.Stop (simulationTime);
#endif*/

  // Add random application to generate (passive) traffic
  UdpEchoClientHelper udpClientHelper = UdpEchoClientHelper(deviceInterfaces.GetAddress(numberOfNodes-1,1), 6000);
  // Multicast disabled for now!
  //UdpEchoClientHelper udpClientHelper = UdpEchoClientHelper(Ipv6Address (IPV6_GROUP_ADDR), 6000);
  UdpEchoServerHelper udpServerHelper = UdpEchoServerHelper(6000);
  
  // Add random app to three nodes:
  for (int n : {0})
  {
    udpClientHelper.SetAttribute ("RemoteAddress", AddressValue (deviceInterfaces.GetAddress (numberOfNodes-1,1)));
    udpClientHelper.SetAttribute ("RemotePort", UintegerValue (6000));
    udpClientHelper.SetAttribute ("PacketSize", UintegerValue (packetSize));
    udpClientHelper.SetAttribute ("Interval", TimeValue (Seconds (10)));
    ApplicationContainer apps = udpClientHelper.Install(nodes.Get(n));

    apps.Start (applicationStart);
    apps.Stop (simulationTime);
  }

  for (int n : {numberOfNodes-1})
  {
    udpServerHelper.SetAttribute ("Port", UintegerValue (6000));
    ApplicationContainer apps =  udpServerHelper.Install(nodes.Get(n));
    apps.Start (Seconds (1.0));
    apps.Stop (simulationTime);
  }



  Simulator::Stop (simulationTime);
  
  lrWpanHelper.EnablePcapAll ("RPLEXAMPLEPCAP", true);
  
  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}


