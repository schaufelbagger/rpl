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

#define USE_SIXLOWPAN
#define USE_APPLICATION


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("movingNodeExample");


void TestTrace ( Ptr<ns3::MobilityModel const> newValue)
{
  std::cout << Simulator::Now().GetSeconds() 
  << ": Position " << newValue->GetPosition () 
  << ", Velocity " << newValue->GetVelocity ()
  << std::endl;
}



int main (int argc, char *argv[])
{

  //LogComponentEnable ("Rpl", LOG_LEVEL_DEBUG);
  //LogComponentEnable ("Ipv6L3Protocol", LOG_LEVEL_LOGIC);
  //LogComponentEnable ("Icmpv6L4Protocol", LOG_LEVEL_LOGIC);

  // parameters
  bool verbose = true;
  uint32_t numberOfGridNodes = 25;
  uint32_t numberOfMovingNodes = 1;
  // distance of nodes
  int xStep = 100;//70
  int yStep = 100;
  double applicationStartSeconds = 10;
  double simulationTimeSeconds = 1000;
#ifdef USE_APPLICATION
  double trafficInterval = 1.0;
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
  /// network
  int numberOfNodes = numberOfGridNodes + numberOfMovingNodes;
  /// nodes used in the example
  NodeContainer nodes;
  NodeContainer gridNodes;
  NodeContainer movingNodes;
  /// devices used in the example
  NetDeviceContainer devices;
  /// interfaces used in the example
  Ipv6InterfaceContainer interfaces;


  CommandLine cmd (__FILE__);
  cmd.AddValue ("verbose", "Tell application to log if true", verbose);
  cmd.AddValue("routingProtocol", "the routing protocol used", routingProtocol);
  cmd.AddValue("run", "the run number", run);
  cmd.AddValue("simulationTime", "the simulation time", simulationTimeSeconds);
  cmd.AddValue("applicationStart", "the application start time", applicationStartSeconds);
  cmd.AddValue("trafficInterval", "the intervall between data messages are sent", trafficInterval);
  cmd.AddValue("numberOfNodes", "number of nodes", numberOfNodes);
  cmd.AddValue("xStep", "the distance between nodes in x direction", xStep);
  cmd.AddValue("yStep", "the distance between nodes in y direction", yStep);
  cmd.Parse (argc,argv);

  RngSeedManager::SetSeed (1);
  RngSeedManager::SetRun (run);

  std::string paramString = get_param_string(routingProtocol, run, numberOfNodes, trafficInterval, applicationStartSeconds, simulationTimeSeconds, networkSetup, appSetup);

  Time applicationStart = Seconds (applicationStartSeconds);
  Time simulationTime = Seconds (simulationTimeSeconds);

  //Config::SetDefault ("ns3::Icmpv6L4Protocol::DAD", BooleanValue (false));
  //Config::SetDefault ("ns3::Icmpv6L4Protocol::MaxUnicastSolicit", IntegerValue (0));
  //Config::SetDefault ("ns3::Icmpv6L4Protocol::MaxMulticastSolicit", IntegerValue (0));
  //Config::SetDefault ("ns3::Icmpv6L4Protocol::RetransmissionTime", TimeValue (Seconds(60*60)));
  //Config::SetDefault ("ns3::Icmpv6L4Protocol::DelayFirstProbe", TimeValue (Seconds(60*60)));

  NS_LOG_UNCOND("rpl moving node example\n\n");

  // ---------------- Create Nodes -------------------------------

  // create nodes
  NS_ASSERT(numberOfGridNodes > 0);
  NS_ASSERT(numberOfMovingNodes > 0);
  nodes.Create(numberOfGridNodes+numberOfMovingNodes);
  for (uint32_t i = 0; i < numberOfGridNodes; i++)
  {
    gridNodes.Add (nodes.Get (i));
  }
  for (uint32_t i = numberOfGridNodes; i < numberOfGridNodes+numberOfMovingNodes; i++)
  {
    movingNodes.Add (nodes.Get (i));
  }
  
  MobilityHelper mobility;
  int gridWidth = GetNextHighestSquareEdgeLength (numberOfGridNodes);
  
  
  NS_ABORT_MSG_IF (gridWidth == 0,"invalid grid size");
  int middleNodeNumber;
  if (gridWidth % 2 == 0)
  {
    middleNodeNumber = static_cast<uint32_t>(static_cast<float>(gridWidth) / 2 + static_cast<float>(gridWidth) * static_cast<float>(gridWidth) / 2);
  }else
  {
    middleNodeNumber = static_cast<uint32_t>(static_cast<float>(gridWidth) * static_cast<float>(gridWidth) / 2);
  }
  

  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                "MinX", DoubleValue (0),
                                "MinY", DoubleValue (0),
                                "DeltaX", DoubleValue (xStep),
                                "DeltaY", DoubleValue (yStep),
                                "GridWidth", UintegerValue (gridWidth),
                                "LayoutType", StringValue ("RowFirst"));
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");

  // get center node as root node
  Ptr<Node> rootNode = gridNodes.Get (middleNodeNumber);
  mobility.Install (gridNodes);


  // configure the moving node
  mobility = MobilityHelper();
  //Ptr<Node> movingNode = nodes.Get (0);
  int zHeight = 0;
  // start in the middle of the grid
  double xPosition = static_cast<double>(xStep*gridWidth - xStep)/2 + 10;
  double yPosition = static_cast<double>(yStep*gridWidth - yStep)/2;

  Ptr<ListPositionAllocator> beginPositionAlloc = CreateObject<ListPositionAllocator> ();
  beginPositionAlloc->Add(Vector3D (xPosition,yPosition,zHeight));
  
  mobility.SetPositionAllocator (beginPositionAlloc);


  Ptr<ListPositionAllocator> waypointPositionAlloc = CreateObject<ListPositionAllocator> ();
  xPosition = gridWidth * xStep;
  waypointPositionAlloc->Add(Vector3D (xPosition,yPosition,zHeight));
  yPosition = 0;
  waypointPositionAlloc->Add(Vector3D (xPosition,yPosition,zHeight));
  // go to border of the grid
  xPosition = xPosition + 200;
  waypointPositionAlloc->Add(Vector3D (xPosition,yPosition,zHeight));
  xPosition = 0;
  waypointPositionAlloc->Add(Vector3D (xPosition,yPosition,zHeight));


  double pauseMean = 50;
  double pauseVariance = 1/3;
  Ptr<NormalRandomVariable> pause = CreateObject<NormalRandomVariable> ();
  pause->SetAttribute ("Mean", DoubleValue (pauseMean));
  pause->SetAttribute ("Variance", DoubleValue (pauseVariance));

  double speedMean = 2.0;
  double speedVariance = 0.1;
  Ptr<NormalRandomVariable> speed = CreateObject<NormalRandomVariable> ();
  speed->SetAttribute ("Mean", DoubleValue (speedMean));
  speed->SetAttribute ("Variance", DoubleValue (speedVariance));

  mobility.SetMobilityModel ("ns3::RandomWaypointMobilityModel",
                              "Pause", PointerValue(pause),
                              "Speed", PointerValue(speed),//StringValue ("ns3::ConstantRandomVariable[Constant=1.0]"),
                              "PositionAllocator", PointerValue(waypointPositionAlloc));

  mobility.Install (movingNodes);




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
  // install TCP/IP & RPL
  RplHelper rpl;
  // you can configure RPL attributes here using rpl.Set(name, value)

  InternetStackHelper stack;
  stack.SetIpv4StackInstall(false);
  stack.SetRoutingHelper (rpl);
  stack.Install (nodes);
  //rpl.AssignDisMop (NodeContainer (nodes.Get (1)) , rpl::DIS_MOP_SEND, Seconds (1), 5, RPL_DEFAULT_INSTANCE, rpl::MOP_STORING_NO_MULTICAST);
  rpl.AssignRoot (NodeContainer (rootNode) );


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

  //for (int i : {movingNode->GetId ()})
  for (uint32_t i = numberOfGridNodes; i < numberOfGridNodes + numberOfMovingNodes; ++i)
  {
    //mobility.AssignStreams (nodes.Get (i), 0);
    AsciiTraceHelper asciiTraceHelper;
    //MobilityHelper::EnableAsciiAll (asciiTraceHelper.CreateFileStream ("mobility-trace-example.mob"));
    //std::cout << i << std::endl;
    //MobilityHelper::EnableAscii (asciiTraceHelper.CreateFileStream ("stateChanges_node_" + std::to_string(i) + "_" + "mobility-trace-example.txt"), i);
    std::cout << "moving node id: " << std::to_string(i) << std::endl;
    Ptr<OutputStreamWrapper> courseChangeWrapper = asciiTraceHelper.CreateFileStream ( "RPLEXAMPLE_courseChange_node_" + std::to_string(i) + paramString + ".txt");
    GetMobilityModel (nodes.Get (i))->TraceConnectWithoutContext("CourseChange", MakeBoundCallback (&CourseChangeTraceSink, courseChangeWrapper));
    GetMobilityModel (nodes.Get (i))->TraceConnectWithoutContext ("CourseChange", MakeCallback(&TestTrace));
    //std::cout << GetMobilityModel (nodes.Get (i))->GetPosition () << std::endl;
  }
  std::cout << "root node: " << GetMobilityModel (nodes.Get (rootNode->GetId()))->GetPosition () << std::endl;

  //AsciiTraceHelper ascii;
  //MobilityHelper::EnableAsciiAll (ascii.CreateFileStream ("mobility-trace-example.mob"));

  // ---------------- Install Applications -------------------------------

  // Add random application to generate (passive) traffic
  UdpEchoClientHelper udpClientHelper = UdpEchoClientHelper(deviceInterfaces.GetAddress(rootNode->GetId (),1), 6000);
  // Multicast disabled for now!
  //UdpEchoClientHelper udpClientHelper = UdpEchoClientHelper(Ipv6Address (IPV6_GROUP_ADDR), 6000);
  UdpEchoServerHelper udpServerHelper = UdpEchoServerHelper(6000);
  
  // Add random app to root node and to first node:
  //for (uint32_t n : {movingNode->GetId ()})
  for (uint32_t n = numberOfGridNodes; n < numberOfGridNodes + numberOfMovingNodes; ++n)
  {
    NS_ABORT_MSG_IF (rootNode->GetId () == n, "udp client and server would be installed on the same node ( node " << rootNode->GetId () << "and " << n << ")");
    udpClientHelper.SetAttribute ("RemoteAddress", AddressValue (deviceInterfaces.GetAddress (rootNode->GetId (),1)));
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

  for (int n : {rootNode->GetId ()})
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


