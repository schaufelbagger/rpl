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

#include "rpl-example-helper.h"

//#define USE_WIFI
#define USE_SIXLOWPAN
#define USE_APPLICATION

using namespace ns3;


int main (int argc, char *argv[])
{

  LogComponentEnable ("Rpl", LOG_LEVEL_DEBUG);
  //LogComponentEnable ("Ipv6L3Protocol", LOG_LEVEL_LOGIC);
  //LogComponentEnable ("Icmpv6L4Protocol", LOG_LEVEL_LOGIC);

  // parameters
  bool verbose = true;
  bool showPositions = false;
  int numberOfNodes = 8;
  double applicationStartSeconds = 100;
  double simulationTimeSeconds = 115;
  // distance of nodes
  int xStep = 60;
  int yStep = 50;
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
  /// network
  /// nodes used in the example
  NodeContainer nodes;
  /// devices used in the example
  NetDeviceContainer devices;
  /// interfaces used in the example
  Ipv6InterfaceContainer interfaces;
  int appSetup = 1;
  int networkSetup = 0;
  
  CommandLine cmd (__FILE__);
  cmd.AddValue ("verbose", "Tell application to log if true", verbose);
  cmd.AddValue("routingProtocol", "the routing protocol used", routingProtocol);
  cmd.AddValue("rplConfigFilename", "filename of the RPL configuration", rplConfigFilename);
  cmd.AddValue("run", "the run number", run);
  cmd.AddValue("simulationTime", "the simulation time in seconds", simulationTimeSeconds);
  cmd.AddValue("applicationStart", "the application start time in seconds", applicationStartSeconds);
  cmd.AddValue("trafficInterval", "the intervall between data messages are sent", trafficInterval);
  cmd.AddValue("appSetup", "if traffix shall be sent fron one leaf to another one (5 to 7)", appSetup);

  cmd.Parse (argc,argv);

  RngSeedManager::SetSeed (1);
  RngSeedManager::SetRun (run);

  //std::string paramString = "_routingProtocol_" + routingProtocol + "_run_" + std::to_string(run) + "_appSetup_" + std::to_string(appSetup) + "_trafficInterval_" + std::to_string(trafficInterval);
  std::string paramString = get_param_string(routingProtocol, run, numberOfNodes, trafficInterval, applicationStartSeconds, simulationTimeSeconds, networkSetup, appSetup);


  Time applicationStart = Seconds (applicationStartSeconds);
  Time simulationTime = Seconds (simulationTimeSeconds);


  //Config::SetDefault ("ns3::Icmpv6L4Protocol::DAD", BooleanValue (false));
  //Config::SetDefault ("ns3::Icmpv6L4Protocol::MaxUnicastSolicit", IntegerValue (0));
  //Config::SetDefault ("ns3::Icmpv6L4Protocol::MaxMulticastSolicit", IntegerValue (0));
  //Config::SetDefault ("ns3::Icmpv6L4Protocol::RetransmissionTime", TimeValue (Seconds(60*60)));
  //Config::SetDefault ("ns3::Icmpv6L4Protocol::DelayFirstProbe", TimeValue (Seconds(60*60)));

  NS_LOG_UNCOND("rpl example\n\n");

  // ---------------- Create Nodes -------------------------------

  MobilityHelper mobility;
  //std::list<ns3::Vector2D> nodePositions;
  //nodePositions.push_back (Vector2D ());
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  int xStart = 1000;
  int yStart = 1000;
  positionAlloc->Add(Vector3D (xStart,yStart,0));
  positionAlloc->Add(Vector3D (xStart-xStep,yStart,0));
  positionAlloc->Add(Vector3D (xStart+xStep,yStart,0));
  positionAlloc->Add(Vector3D (xStart-2*xStep,yStart+yStep,0));
  positionAlloc->Add(Vector3D (xStart-2*xStep,yStart-yStep,0));
  positionAlloc->Add(Vector3D (xStart+2*xStep,yStart+yStep,0));
  positionAlloc->Add(Vector3D (xStart+2*xStep,yStart-yStep,0));
  positionAlloc->Add(Vector3D (xStart-3*xStep,yStart+2*yStep,0));
  //positionAlloc->Add("position_" + mode + ".csv");
  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  numberOfNodes = positionAlloc->GetSize();


  nodes.Create(numberOfNodes);
  mobility.Install (nodes);

  if (showPositions)
  {
    for (int i = 0; i< numberOfNodes; ++i)
    {
      std::cout << "node " << +i << ": " << GetMobilityModel (nodes.Get (i))->GetPosition () << std::endl;
    }
  }

  // create nodes
  /*nodes.Create(numberOfNodes);

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
  }*/
  

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

  for (int i = 0; i< numberOfNodes; ++i)
  {
    deviceInterfaces.SetForwarding (i, true);
    AsciiTraceHelper asciiTraceHelper;
    Ptr<OutputStreamWrapper> updatedPrefParent = asciiTraceHelper.CreateFileStream (
          "RPLEXAMPLE_updatedPrefParent_node_" + std::to_string(i) + paramString + ".txt");
    GetRpl(nodes.Get(i))->TraceConnectWithoutContext("UpdatedPrefParent", MakeBoundCallback (&UpdatePrefParentTraceSink, updatedPrefParent));

    Ptr<OutputStreamWrapper> routeAddedStream = asciiTraceHelper.CreateFileStream (
          "RPLEXAMPLE_routeAdded_node_" + std::to_string(i) + paramString + ".txt");
    GetRpl(nodes.Get(i))->TraceConnectWithoutContext("routeAdded", MakeBoundCallback (&RouteAddedTraceSink, routeAddedStream));
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
  UdpEchoClientHelper udpClientHelper = UdpEchoClientHelper(deviceInterfaces.GetAddress(0,1), 6000);
  // Multicast disabled for now!
  //UdpEchoClientHelper udpClientHelper = UdpEchoClientHelper(Ipv6Address (IPV6_GROUP_ADDR), 6000);
  UdpEchoServerHelper udpServerHelper = UdpEchoServerHelper(6000);
  
  // Add all leaf nodes
  std::vector<int> clients;
  std::vector<int> servers;
  if (appSetup == 1)
  {
    clients = {4, 5, 6, 7};
    servers = {0};
  }else if (appSetup == 2)
  {
    clients = {5};
    servers = {7};
  }else if (appSetup == 3)
  {
    clients = {7};
    servers = {0};
  }else
  {
    NS_ABORT_MSG ("Unkown appSetup number " + std::to_string(appSetup) + " given!");
  }

  for (int n : clients)
  {
    if (appSetup == 2)
    {
      udpClientHelper.SetAttribute ("RemoteAddress", AddressValue (deviceInterfaces.GetAddress (7,1)));
    }else
    {
      udpClientHelper.SetAttribute ("RemoteAddress", AddressValue (deviceInterfaces.GetAddress (0,1)));
    }
    //udpClientHelper.SetAttribute ("RemoteAddress", AddressValue (deviceInterfaces.GetAddress (0,1)));
    udpClientHelper.SetAttribute ("RemotePort", UintegerValue (6000));
    udpClientHelper.SetAttribute ("PacketSize", UintegerValue (packetSize));
    udpClientHelper.SetAttribute ("Interval", TimeValue (Seconds (trafficInterval)));
    udpClientHelper.SetAttribute ("MaxPackets", UintegerValue (maxPackets));
    ApplicationContainer apps = udpClientHelper.Install(nodes.Get(n));

    apps.Start (applicationStart);
    apps.Stop (simulationTime);

    AsciiTraceHelper asciiTraceHelper;
    Ptr<OutputStreamWrapper> updClientRxWrapper = asciiTraceHelper.CreateFileStream ( "RPLEXAMPLE_udpClientReceive_node_" + std::to_string(n) + paramString + ".txt");
    apps.Get (0)->TraceConnectWithoutContext("RxWithAddresses", MakeBoundCallback (&UdpRxTraceWithAddressesSink, updClientRxWrapper));

    Ptr<OutputStreamWrapper> updClientTxWrapper = asciiTraceHelper.CreateFileStream ( "RPLEXAMPLE_udpClientSend_node_" + std::to_string(n) + paramString + ".txt");
    apps.Get (0)->TraceConnectWithoutContext("TxWithAddresses", MakeBoundCallback (&UdpTxTraceWithAddressesSink, updClientTxWrapper));
  }

  for (int n : servers)
  {
    udpServerHelper.SetAttribute ("Port", UintegerValue (6000));
    ApplicationContainer apps =  udpServerHelper.Install(nodes.Get(n));
    apps.Start (Seconds (1.0));
    apps.Stop (simulationTime);

    AsciiTraceHelper asciiTraceHelper;
    Ptr<OutputStreamWrapper> updServerWrapper = asciiTraceHelper.CreateFileStream ( "RPLEXAMPLE_udpServerReceive_node_" + std::to_string(n) + paramString + ".txt");
    apps.Get (0)->TraceConnectWithoutContext("RxWithAddresses", MakeBoundCallback (&UdpRxTraceWithAddressesSink, updServerWrapper));
  }



  Simulator::Stop (simulationTime);
  
  lrWpanHelper.EnablePcapAll ("RPLEXAMPLEPCAP", true);
  
  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}


