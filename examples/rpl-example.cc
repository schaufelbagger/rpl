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

#include "ns3/energy-module.h"
/*#include "ns3/lr-wpan-radio-energy-model.h"
#include "ns3/lr-wpan-radio-energy-model-helper.h"*/
//#include "ns3/statistics-helper.h"

#include "ns3/rpl-node.h"
#include "rpl-example-helper.h"

//#define USE_WIFI
#define USE_SIXLOWPAN
#define USE_APPLICATION

using namespace ns3;

/*Ptr<rpl::RoutingProtocol> GetRpl(Ptr <Node> node){
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

void RemainingEnergy (Ptr<OutputStreamWrapper> stream, double oldValue, double remainingEnergy)
{
  *stream->GetStream () << Simulator::Now().GetSeconds();
  *stream->GetStream () << ", " << oldValue;
  *stream->GetStream () << ", " << remainingEnergy;
  *stream->GetStream () << std::endl;     
  //NS_LOG_UNCOND (Simulator::Now ().GetSeconds () << "s Current remaining energy = " << remainingEnergy << "J");
}
*/
void TotalEnergy (double oldValue, double totalEnergy)
{
  NS_LOG_UNCOND (Simulator::Now ().GetSeconds () << "s Total energy consumed by radio = " << totalEnergy << "J");
}

//int lrwpanPacketCounter = 0;

int packetCounter[50] = { 0 };

void tx6lowpanTraceContextSink (Ptr<OutputStreamWrapper> stream, std::string context, Ptr<const Packet> packet,  Ptr<SixLowPanNetDevice> netdev, uint32_t interface)
{
  TimestampTag timestamp;
  Time tx = Time ();
  // find only UDP packets
  if (packet->FindFirstMatchingByteTag (timestamp)) 
  {
    packetCounter[stoi(context)] ++;
    int packetCnt = packetCounter[stoi(context)];
    *stream->GetStream () << Simulator::Now().GetSeconds();
    *stream->GetStream () << ", " << std::to_string(packetCnt) ;
    *stream->GetStream () << std::endl;
  } 
}

void txPacketTraceContextSink (Ptr<OutputStreamWrapper> stream, std::string context, Ptr<const Packet> packet, uint8_t retries, uint8_t csmaBackoffs)
{
  TimestampTag timestamp;
  Time tx = Time ();
  // find only UDP packets
  if (packet->FindFirstMatchingByteTag (timestamp)) 
  {
    packetCounter[stoi(context)] ++;
    int packetCnt = packetCounter[stoi(context)];
    *stream->GetStream () << Simulator::Now().GetSeconds();
    *stream->GetStream () << ", " << std::to_string(packetCnt) ;
    *stream->GetStream () << std::endl;
  } 
}

void rxPacketTraceContextSink (Ptr<OutputStreamWrapper> stream, std::string context, Ptr<const Packet> packet, double sinr)
{
  TimestampTag timestamp;
  // find only UDP packets
  if (packet->FindFirstMatchingByteTag (timestamp)) 
  {
    packetCounter[stoi(context)] ++;
    int packetCnt = packetCounter[stoi(context)];
    *stream->GetStream () << Simulator::Now().GetSeconds();
    *stream->GetStream () << ", " << std::to_string(packetCnt);
    *stream->GetStream () << std::endl;
  }
}


int main (int argc, char *argv[])
{

  //LogComponentEnable ("Rpl", LOG_LEVEL_DEBUG);
  //LogComponentEnable ("Ipv6L3Protocol", LOG_LEVEL_LOGIC);
  //LogComponentEnable ("Icmpv6L4Protocol", LOG_LEVEL_LOGIC);

  // parameters
  bool verbose = true;
  int numberOfNodes = 3;
  // distance of nodes
  int step = 100;
  double applicationStartSeconds = 100;
  double simulationTimeSeconds = 110;
#ifdef USE_APPLICATION
  double trafficInterval = 10;
  uint32_t packetSize = 10;
  //uint32_t maxPacketCount = 5;
  //Time interPacketInterval = Seconds (1.);
#endif
  //bool energyModelEnabled = false;
  //double txPower = 0;
  //double initialNodeEnergy = 10;

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

  CommandLine cmd (__FILE__);
  cmd.AddValue ("verbose", "Tell application to log if true", verbose);
  cmd.AddValue("routingProtocol", "the routing protocol used", routingProtocol);
  cmd.AddValue("rplConfigFilename", "filename of the RPL configuration", rplConfigFilename);
  cmd.AddValue("run", "the run number", run);
  cmd.AddValue("numberOfNodes", "number of nodes", numberOfNodes);
  cmd.AddValue("simulationTime", "the simulation time in seconds", simulationTimeSeconds);
  cmd.AddValue("applicationStart", "the application start time in seconds", applicationStartSeconds);
  cmd.AddValue("trafficInterval", "the intervall between data messages are sent", trafficInterval);
  //cmd.AddValue ("txPower", "Sending Power of the normal nodes.",txPower);
  //cmd.AddValue ("energyModelEnabled","Enables or disables the assignment of an energy model to the nodes", energyModelEnabled);
  //cmd.AddValue ("initialNodeEnergy","The available energy contained in the nodes battery", initialNodeEnergy);

  cmd.Parse (argc,argv);

  RngSeedManager::SetSeed (1);
  RngSeedManager::SetRun (run);

  Time applicationStart = Seconds (applicationStartSeconds);
  Time simulationTime = Seconds (simulationTimeSeconds);

  NS_LOG_UNCOND("rpl example\n\n");

  // ---------------- Create Nodes -------------------------------

  // create nodes
  nodes.Create(numberOfNodes);
  MobilityHelper mobility;
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (step),
                                 "DeltaY", DoubleValue (0),
                                 "GridWidth", UintegerValue (numberOfNodes),
                                 "LayoutType", StringValue ("RowFirst"));
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (nodes);

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

  //EnergySourceContainer sources;
  //StatisticsHelper statHelper;
  /*if(energyModelEnabled)
  {
    
    BasicEnergySourceHelper basicSourceHelper;
    basicSourceHelper.Set ("BasicEnergySourceInitialEnergyJ", DoubleValue (initialNodeEnergy));
    basicSourceHelper.Set ("PeriodicEnergyUpdateInterval", TimeValue (Simulator::GetMaximumSimulationTime())); // do not reload the battery
    sources = basicSourceHelper.Install(nodes);
    LrWpanRadioEnergyModelHelper radioEnergyHelper;
    DeviceEnergyModelContainer deviceModels = radioEnergyHelper.Install (lrwpanDevices, sources);
    //statHelper.Install(nodes.Get(it->first), DynamicCast<LrWpanNetDevice>(devContainer.Get(it->first)), energyModelEnabled, simEndTime);
  }*/
  

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
  rpl.AssignLeaf (NodeContainer (nodes.Get (2)) );


  Ipv6AddressHelper ipv6;
  ipv6.SetBase (Ipv6Address ("2001:2::"), Ipv6Prefix (64));
  Ipv6InterfaceContainer deviceInterfaces;
  deviceInterfaces = ipv6.Assign (devices);



  for (int i = 0; i< numberOfNodes; ++i)
  {
    deviceInterfaces.SetForwarding (i, true);
    AsciiTraceHelper asciiTraceHelper;
    Ptr<OutputStreamWrapper> updatedPrefParent = asciiTraceHelper.CreateFileStream (
          "RPLEXAMPLE_updatedPrefParent_routingProtocol_" + routingProtocol + "_node_" + std::to_string(i) + "_run_" + std::to_string(run) + "" + ".txt");
    GetRpl(nodes.Get(i))->TraceConnectWithoutContext("UpdatedPrefParent", MakeBoundCallback (&UpdatePrefParentTraceSink, updatedPrefParent));


    Ptr<OutputStreamWrapper> txPackets = asciiTraceHelper.CreateFileStream (
          "RPLEXAMPLE_txPackets_routingProtocol_" + routingProtocol + "_node_" + std::to_string(i) + "_run_" + std::to_string(run) + "" + ".txt");
    Ptr<OutputStreamWrapper> rxPackets = asciiTraceHelper.CreateFileStream (
      "RPLEXAMPLE_rxPackets_routingProtocol_" + routingProtocol + "_node_" + std::to_string(i) + "_run_" + std::to_string(run) + "" + ".txt");

    Ptr<LrWpanNetDevice> lrwpandev = lrwpanDevices.Get (i)->GetObject<LrWpanNetDevice> ();


    
    //lrwpandev->GetPhy ()->TraceConnect("PhyTxEnd", std::to_string(i), MakeBoundCallback (&txPacketTraceContextSink, txPackets));
    //lrwpandev->GetMac ()->TraceConnect("MacSentPkt", std::to_string(i), MakeBoundCallback (&txPacketTraceContextSink, txPackets));
    devices.Get (i)->GetObject<SixLowPanNetDevice > ()->TraceConnect("Tx", std::to_string(i), MakeBoundCallback (&tx6lowpanTraceContextSink, txPackets));
    
    lrwpandev->GetPhy ()->TraceConnect("PhyRxEnd", std::to_string(i), MakeBoundCallback (&rxPacketTraceContextSink, rxPackets));
    //lrwpandev->GetMac ()->TraceConnectWithoutContext("PhyTxEnd", MakeBoundCallback (&txPacketTraceSink, txPackets));


  }



  /*if(energyModelEnabled)
  {
    // all sources are connected to node 1
    // energy source
    Ptr<BasicEnergySource> basicSourcePtr = DynamicCast<BasicEnergySource> (sources.Get (1));
    AsciiTraceHelper asciiTraceHelper;
    Ptr<OutputStreamWrapper> remainingEnergy = asciiTraceHelper.CreateFileStream ("RPLEXAMPLE_remainingEnergy_node_" + std::to_string(1) + "_" + "" + ".txt");
    basicSourcePtr->TraceConnectWithoutContext ("RemainingEnergy", MakeBoundCallback (&RemainingEnergy, remainingEnergy));
    // device energy model
    //Ptr<DeviceEnergyModel> basicRadioModelPtr = basicSourcePtr->FindDeviceEnergyModels ("ns3::LrWpanRadioEnergyModel").Get (0);
    //NS_ASSERT (basicRadioModelPtr);
    //basicRadioModelPtr->TraceConnectWithoutContext ("TotalEnergyConsumption", MakeCallback (&TotalEnergy));
  }
  */

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
    udpClientHelper.SetAttribute ("Interval", TimeValue (Seconds (trafficInterval)));
    ApplicationContainer apps = udpClientHelper.Install(nodes.Get(n));

    apps.Start (applicationStart);
    apps.Stop (simulationTime);

    AsciiTraceHelper asciiTraceHelper;
    Ptr<OutputStreamWrapper> updServerRxWrapper = asciiTraceHelper.CreateFileStream ( "RPLEXAMPLE_udpClientReceive_node_" + std::to_string(n) + ".txt");
    apps.Get (0)->TraceConnectWithoutContext("RxWithAddresses", MakeBoundCallback (&UdpRxTraceWithAddressesSink, updServerRxWrapper));
    Ptr<OutputStreamWrapper> updClientTxWrapper = asciiTraceHelper.CreateFileStream ( "RPLEXAMPLE_udpClientSend_node_" + std::to_string(n) + ".txt");
    apps.Get (0)->TraceConnectWithoutContext("TxWithAddresses", MakeBoundCallback (&UdpTxTraceWithAddressesSink, updClientTxWrapper));
  }

  for (int n : {numberOfNodes-1})
  {
    udpServerHelper.SetAttribute ("Port", UintegerValue (6000));
    ApplicationContainer apps =  udpServerHelper.Install(nodes.Get(n));
    apps.Start (Seconds (1.0));
    apps.Stop (simulationTime);

    AsciiTraceHelper asciiTraceHelper;
    Ptr<OutputStreamWrapper> updServerWrapper = asciiTraceHelper.CreateFileStream ( "RPLEXAMPLE_udpServerReceive_node_" + std::to_string(n) + ".txt");
    apps.Get (0)->TraceConnectWithoutContext("RxWithAddresses", MakeBoundCallback (&UdpRxTraceWithAddressesSink, updServerWrapper));
  }



  Simulator::Stop (simulationTime + Seconds(2));
  
  lrWpanHelper.EnablePcapAll ("RPLEXAMPLEPCAP", true);
  
  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}


