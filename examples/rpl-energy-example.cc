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
#include "ns3/flow-monitor-module.h"

#include "ns3/lr-wpan-module.h"
#include "ns3/yans-wifi-helper.h"

#include "ns3/rpl-node.h"
#include "ns3/energy-module.h"
/*#include "ns3/lr-wpan-radio-energy-model.h"
#include "ns3/lr-wpan-radio-energy-model-helper.h"
*/
#include "uint32-tag.h"
#include "rpl-example-helper.h"

#include <array>

//#define USE_WIFI
#define USE_SIXLOWPAN
#define USE_APPLICATION

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("energyExample");


int packetCounter[50] = { 0 };
std::map<std::array<uint8_t, 4>, std::set<uint32_t>> m_packetCounterMap;  // empty set

void tx6lowpanTraceContextSink (Ptr<OutputStreamWrapper> stream, std::string context, Ptr<const Packet> packet,  Ptr<SixLowPanNetDevice> netdev, uint32_t interface)
{

  //TimestampTag timestamp;
  //Time tx = Time ();
  Uint32Tag tagCopy;
  Ptr<Packet> pCopy = packet->Copy();
  packet->Print (std::cout);

  // find only UDP packets
  if (pCopy->PeekPacketTag (tagCopy)) 
  {

    // Headers must be removed in the order they're present.
    Ipv6Header ipHeader;
    pCopy->RemoveHeader(ipHeader);

    /*Ipv6Address from = ipHeader.GetSource ();
    uint32_t currentCounter = tagCopy.GetSimpleValue ();
    auto packetCounterSet = m_packetCounterMap[from];
    auto setPair = packetCounterSet.insert (currentCounter);
    m_packetCounterMap[from] = packetCounterSet;
    if (!setPair.second)
    {
      //NS_LOG_UNCOND ("insert failed!");
      return;
    }*/
    //packetCounter[stoi(context)] ++;
    //int packetCnt = packetCounter[stoi(context)];
    *stream->GetStream () << Simulator::Now().GetSeconds();
    //*stream->GetStream () << ", " << std::to_string(currentCounter) ;
    *stream->GetStream () << std::endl;
  } 
}

void txPacketTraceContextUdpSink (Ptr<OutputStreamWrapper> stream, std::string context, Ptr<const Packet> packet, uint8_t retries, uint8_t csmaBackoffs)
{

  Uint32Tag tagCopy;
  Ptr<Packet> pCopy = packet->Copy();
  //packet->Print (std::cout);

  int nodeNumber = stoi(context);

  // find only UDP packets
  if (pCopy->PeekPacketTag (tagCopy)) 
  {

    // Headers must be removed in the order they're present.
    LrWpanMacHeader lrWpanHeader;
    pCopy->RemoveHeader(lrWpanHeader);

    std::array<uint8_t, 4>  buffer;

    Mac16Address to = lrWpanHeader.GetShortDstAddr ();
    Mac16Address from = lrWpanHeader.GetShortSrcAddr ();
    //uint8_t seqNumber = lrWpanHeader.GetSeqNum ();

    to.CopyTo (&buffer[0]);
    from.CopyTo (&buffer[2]);

    // check if node is layer 2 sender or receiver - if not discard trace
    if (buffer[1]-1 != nodeNumber && buffer[3]-1 != nodeNumber)
    {
      return;
    }

    uint32_t currentCounter = tagCopy.GetSimpleValue ();

    auto packetCounterSet = m_packetCounterMap[buffer];
    auto setPair = packetCounterSet.insert (currentCounter);
    m_packetCounterMap[buffer] = packetCounterSet;
    if (!setPair.second)
    {
      //NS_LOG_UNCOND ("insert failed!");
      return;
    }
    //packetCounter[stoi(context)] ++;
    //int packetCnt = packetCounter[stoi(context)];
    *stream->GetStream () << Simulator::Now().GetSeconds();
    *stream->GetStream () << ", " << std::to_string(currentCounter) ;
    *stream->GetStream () << std::endl;
  } 
}

void txPacketTraceContextLrwpanSink (Ptr<OutputStreamWrapper> stream, std::string context, Ptr<const Packet> packet, uint8_t retries, uint8_t csmaBackoffs)
{
  //packetCounter[stoi(context)] ++;
  //int packetCnt = packetCounter[stoi(context)];
  *stream->GetStream () << Simulator::Now().GetSeconds();
  //*stream->GetStream () << ", " << std::to_string(packetCnt) ;
  *stream->GetStream () << std::endl;

}


void rxPacketTraceContextLrwpanSink (Ptr<OutputStreamWrapper> stream, std::string context, Ptr<const Packet> packet)
{

  //packetCounter[stoi(context)] ++;
  //int packetCnt = packetCounter[stoi(context)];
  *stream->GetStream () << Simulator::Now().GetSeconds();
  //*stream->GetStream () << ", " << std::to_string(packetCnt);
  *stream->GetStream () << std::endl;

}


int main (int argc, char *argv[])
{

  //LogComponentEnable ("Rpl", LOG_LEVEL_DEBUG);
  //LogComponentEnable ("Ipv6L3Protocol", LOG_LEVEL_LOGIC);
  //LogComponentEnable ("Icmpv6L4Protocol", LOG_LEVEL_LOGIC);

  // parameters
  bool verbose = true;
  int numberOfNodes;
  // distance of nodes
  double applicationStartSeconds = 100;
  double simulationTimeSeconds = 300;
#ifdef USE_APPLICATION
  double trafficInterval = 1.0;
  int maxPackets = 10000;
  uint32_t packetSize = 10;
  //uint32_t maxPacketCount = 5;
  Time interPacketInterval = Seconds (1.);
#endif
  //bool energyModelEnabled = true;
  //double txPower = 0;
  //double initialNodeEnergy = 10;
  //double txCurrentA = 0.0101; // apperently not used, does not change jack shit


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
  cmd.AddValue("trafficInterval", "the intervall between data messages are sent", trafficInterval);
  cmd.AddValue("appSetup", "how the trickle timer is set;0=normal, 1=dioIntervalMax=10s, 2=dioIntervalMax=20s", appSetup);
  cmd.AddValue("dioIntervalDoublings", "the RPL DIO Trickle timer Interval Doublings parameter", dioIntervalDoublings);
  cmd.AddValue("dioIntervalMin", "the RPL DIO Trickle timer Interval Min parameter", dioIntervalMin);
  //cmd.AddValue ("txPower", "Sending Power of the normal nodes.",txPower);
  //cmd.AddValue ("energyModelEnabled","Enables or disables the assignment of an energy model to the nodes", energyModelEnabled);
  //cmd.AddValue ("initialNodeEnergy","The available energy contained in the nodes battery", initialNodeEnergy);

  cmd.Parse (argc,argv);

  RngSeedManager::SetSeed (1);
  RngSeedManager::SetRun (run);

  

  //Config::SetDefault ("ns3::Icmpv6L4Protocol::DAD", BooleanValue (false));
  //Config::SetDefault ("ns3::Icmpv6L4Protocol::MaxUnicastSolicit", IntegerValue (1));
  //Config::SetDefault ("ns3::Icmpv6L4Protocol::MaxMulticastSolicit", IntegerValue (1));

  Time applicationStart = Seconds (applicationStartSeconds);
  Time simulationTime = Seconds (simulationTimeSeconds);

  NS_LOG_UNCOND("rpl energy example\n\n");

  // ---------------- Create Nodes -------------------------------

  // create nodes
  
  MobilityHelper mobility;
  //std::list<ns3::Vector2D> nodePositions;
  //nodePositions.push_back (Vector2D ());
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  // root node
  positionAlloc->Add(Vector3D (500,0,0)); // root node
  // second layer
  positionAlloc->Add(Vector3D (450,70,0));
  positionAlloc->Add(Vector3D (500,70,0));
  positionAlloc->Add(Vector3D (550,70,0));
  // third layer
  positionAlloc->Add(Vector3D (380,140,0)); // node 4
  positionAlloc->Add(Vector3D (620,140,0)); // node 5
  positionAlloc->Add(Vector3D (450,140,0));
  positionAlloc->Add(Vector3D (500,140,0));
  positionAlloc->Add(Vector3D (550,140,0));

  //fourth layer
  positionAlloc->Add(Vector3D (310,210,0)); // node 9
  positionAlloc->Add(Vector3D (430,210,0));
  positionAlloc->Add(Vector3D (450,210,0));
  positionAlloc->Add(Vector3D (500,210,0));
  positionAlloc->Add(Vector3D (550,210,0));
  positionAlloc->Add(Vector3D (570,210,0));
  positionAlloc->Add(Vector3D (690,210,0));
  // fith layer
  positionAlloc->Add(Vector3D (240,280,0)); // node 16
  positionAlloc->Add(Vector3D (310,280,0));
  positionAlloc->Add(Vector3D (430,280,0));
  positionAlloc->Add(Vector3D (450,280,0));
  positionAlloc->Add(Vector3D (500,280,0));
  positionAlloc->Add(Vector3D (550,280,0));
  positionAlloc->Add(Vector3D (570,280,0));
  positionAlloc->Add(Vector3D (690,280,0));
  positionAlloc->Add(Vector3D (760,280,0));
  // sixth layer
  positionAlloc->Add(Vector3D (240,350,0)); // node 25
  positionAlloc->Add(Vector3D (310,350,0));
  positionAlloc->Add(Vector3D (430,350,0));
  positionAlloc->Add(Vector3D (450,350,0));
  positionAlloc->Add(Vector3D (500,350,0));
  positionAlloc->Add(Vector3D (550,350,0));
  positionAlloc->Add(Vector3D (570,350,0));
  positionAlloc->Add(Vector3D (690,350,0));
  positionAlloc->Add(Vector3D (760,350,0));
  // seventh layer
  positionAlloc->Add(Vector3D (240,420,0)); // node 34
  positionAlloc->Add(Vector3D (310,420,0));
  positionAlloc->Add(Vector3D (430,420,0));
  positionAlloc->Add(Vector3D (450,420,0));
  positionAlloc->Add(Vector3D (500,420,0));
  positionAlloc->Add(Vector3D (550,420,0));
  positionAlloc->Add(Vector3D (570,420,0));
  positionAlloc->Add(Vector3D (690,420,0));
  positionAlloc->Add(Vector3D (760,420,0));
  
  //positionAlloc->Add("position_" + mode + ".csv");
  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  numberOfNodes = positionAlloc->GetSize();

  NS_ASSERT(numberOfNodes > 0);


  nodes.Create(numberOfNodes);
  mobility.Install (nodes);


  std::string paramString = get_param_string(routingProtocol, run, numberOfNodes, trafficInterval, applicationStartSeconds, simulationTimeSeconds, networkSetup, appSetup);
  paramString = "_DIDoublings_" + std::to_string(dioIntervalDoublings) + "_DIMin_" + std::to_string(dioIntervalMin) + paramString;
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
  rpl.Set ("DIOIntervalDoublings", UintegerValue (dioIntervalDoublings));
  rpl.Set ("DIOIntervalMin", UintegerValue (dioIntervalMin));
  //DIOIntervalMax = 10s
  /*if (appSetup == 1)
  {
    // DIO Intervall Max = 2^DIOIntervalMin * 2^DIOIntervalDoublings
    // DIO Intervall Max = 8.192s
    rpl.Set ("DIOIntervalDoublings", UintegerValue (10));
    rpl.Set ("DIOIntervalMin", UintegerValue (3));
  }else if (appSetup == 2)
  {
    // DIO Intervall Max = 16.384
    rpl.Set ("DIOIntervalDoublings", UintegerValue (11));
    rpl.Set ("DIOIntervalMin", UintegerValue (3));
  }
  else if (appSetup == 3)
  {
    // DIO Intervall Max = 16.384
    rpl.Set ("DIOIntervalDoublings", UintegerValue (13));
    rpl.Set ("DIOIntervalMin", UintegerValue (3));
  }*/
  

  InternetStackHelper stack;
  stack.SetIpv4StackInstall(false);
  stack.SetRoutingHelper (rpl);
  stack.Install (nodes);
  //rpl.AssignDisMop (NodeContainer (nodes.Get (1)) , rpl::DIS_MOP_SEND, Seconds (1), 5, RPL_DEFAULT_INSTANCE, rpl::MOP_STORING_NO_MULTICAST);
  rpl.AssignRoot (NodeContainer (nodes.Get (0)) );
  rpl.AssignLeaf(NodeContainer (nodes.Get (34), nodes.Get (35), nodes.Get (36), nodes.Get (37), nodes.Get (38),nodes.Get (39), nodes.Get (40), nodes.Get (41), nodes.Get (42)), true);


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

  for (int i : {4,5,6,7,8})
  {
    AsciiTraceHelper asciiTraceHelper;
    Ptr<OutputStreamWrapper> txUdpPackets = asciiTraceHelper.CreateFileStream (
      "RPLEXAMPLE_txUdpPackets_node_" + std::to_string(i) + paramString + ".txt");
    Ptr<OutputStreamWrapper> txLrwpanPackets = asciiTraceHelper.CreateFileStream (
      "RPLEXAMPLE_txLrwpanPackets_node_" + std::to_string(i) + paramString + ".txt");
    Ptr<OutputStreamWrapper> rxLrwpanPackets = asciiTraceHelper.CreateFileStream (
      "RPLEXAMPLE_rxLrwpanPackets_node_" + std::to_string(i) + paramString + ".txt");
    //Ptr<OutputStreamWrapper> rxPackets = asciiTraceHelper.CreateFileStream (
    //  "RPLEXAMPLE_rxPackets__node_" + std::to_string(i) + paramString + ".txt");

    Ptr<LrWpanNetDevice> lrwpandev = lrwpanDevices.Get (i)->GetObject<LrWpanNetDevice> ();
    lrwpandev->GetMac ()->TraceConnect("MacSentPkt", std::to_string(i), MakeBoundCallback (&txPacketTraceContextUdpSink, txUdpPackets));
    lrwpandev->GetMac ()->TraceConnect("MacSentPkt", std::to_string(i), MakeBoundCallback (&txPacketTraceContextLrwpanSink, txLrwpanPackets));
    lrwpandev->GetMac ()->TraceConnect("MacPromiscRx", std::to_string(i), MakeBoundCallback (&rxPacketTraceContextLrwpanSink, rxLrwpanPackets));
    //devices.Get (i)->GetObject<SixLowPanNetDevice > ()->TraceConnect("Tx", std::to_string(i), MakeBoundCallback (&tx6lowpanTraceContextSink, txPackets));

    //lrwpandev->GetPhy ()->TraceConnect("PhyRxEnd", std::to_string(i), MakeBoundCallback (&rxPacketTraceContextSink, rxPackets));
  }

  // ---------------- Install Applications -------------------------------

  // Add random application to generate (passive) traffic
  UdpEchoClientHelper udpClientHelper = UdpEchoClientHelper(deviceInterfaces.GetAddress(0,1), 6000);
  // Multicast disabled for now!
  //UdpEchoClientHelper udpClientHelper = UdpEchoClientHelper(Ipv6Address (IPV6_GROUP_ADDR), 6000);
  UdpEchoServerHelper udpServerHelper = UdpEchoServerHelper(6000);
  

  // Add send app to leaf nodes:
  int jitter = 0;
  for (int n : {34, 35, 36, 37, 38, 39, 40, 41, 42})
  //for (int n : {41, 42})
  {
    udpClientHelper.SetAttribute ("RemoteAddress", AddressValue (deviceInterfaces.GetAddress (0,1)));
    udpClientHelper.SetAttribute ("RemotePort", UintegerValue (6000));
    udpClientHelper.SetAttribute ("PacketSize", UintegerValue (packetSize));
    udpClientHelper.SetAttribute ("Interval", TimeValue (Seconds (trafficInterval) ));
    udpClientHelper.SetAttribute ("MaxPackets", UintegerValue (maxPackets));
    ApplicationContainer apps = udpClientHelper.Install(nodes.Get(n));

    apps.Start (applicationStart + MilliSeconds(jitter));
    apps.Stop (simulationTime);
    //jitter = jitter + 10;

    AsciiTraceHelper asciiTraceHelper;
    Ptr<OutputStreamWrapper> updClientRxWrapper = asciiTraceHelper.CreateFileStream ( "RPLEXAMPLE_udpClientReceive_node_" + std::to_string(n) + paramString + ".txt");
    apps.Get (0)->TraceConnectWithoutContext("RxWithAddresses", MakeBoundCallback (&UdpRxTraceWithAddressesSink, updClientRxWrapper));

    Ptr<OutputStreamWrapper> updClientTxWrapper = asciiTraceHelper.CreateFileStream ( "RPLEXAMPLE_udpClientSend_node_" + std::to_string(n) + paramString + ".txt");
    apps.Get (0)->TraceConnectWithoutContext("TxWithAddresses", MakeBoundCallback (&UdpTxTraceWithAddressesSink, updClientTxWrapper));
  }

  for (int n : {0})
  {
    udpServerHelper.SetAttribute ("Port", UintegerValue (6000));
    ApplicationContainer apps =  udpServerHelper.Install(nodes.Get(n));
    apps.Start (Seconds (1.0));
    apps.Stop (simulationTime + Seconds(2));

    AsciiTraceHelper asciiTraceHelper;
    Ptr<OutputStreamWrapper> updServerWrapper = asciiTraceHelper.CreateFileStream ( "RPLEXAMPLE_udpServerReceive_node_" + std::to_string(n) + paramString + ".txt");
    apps.Get (0)->TraceConnectWithoutContext("RxWithAddresses", MakeBoundCallback (&UdpRxTraceWithAddressesSink, updServerWrapper));
  }

  // install FlowMonitor to collect simulation statistics
  //FlowMonitorHelper flowHelper;
  //Ptr<FlowMonitor> flowMonitor = flowHelper.InstallAll();

  Simulator::Stop (simulationTime + Seconds(2));
  
  lrWpanHelper.EnablePcapAll ("RPLEXAMPLEPCAP", true);
  ns3::PacketMetadata::Enable ();

  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}





  /*if(energyModelEnabled)
  {
    BasicEnergySourceHelper basicSourceHelper;
    basicSourceHelper.Set ("BasicEnergySourceInitialEnergyJ", DoubleValue (initialNodeEnergy));
    basicSourceHelper.Set ("PeriodicEnergyUpdateInterval", TimeValue (Simulator::GetMaximumSimulationTime())); // do not reload the battery
    EnergySourceContainer sources = basicSourceHelper.Install(normalNodes);
    LrWpanRadioEnergyModelHelper radioEnergyHelper;
    DeviceEnergyModelContainer deviceModels = radioEnergyHelper.Install (panIDContainers[0], sources);
  }*/ 
    