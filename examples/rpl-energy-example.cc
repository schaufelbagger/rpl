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

#include "ns3/lr-wpan-module.h"
#include "ns3/yans-wifi-helper.h"

#include "ns3/rpl-state.h"
#include "ns3/energy-module.h"
#include "ns3/lr-wpan-radio-energy-model.h"
#include "ns3/lr-wpan-radio-energy-model-helper.h"

#include "rpl-example-helper.h"

//#define USE_WIFI
#define USE_SIXLOWPAN
#define USE_APPLICATION

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("energyExample");


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
  double simulationTimeSeconds = 500;
#ifdef USE_APPLICATION
  double trafficInterval = 1.0;
  int maxPackets = 10000;
  uint32_t packetSize = 10;
  //uint32_t maxPacketCount = 5;
  Time interPacketInterval = Seconds (1.);
#endif
  bool energyModelEnabled = true;
  double txPower = 0;
  double initialNodeEnergy = 10;
  double txCurrentA = 0.0101; // apperently not used, does not change jack shit


  int run = 1;
  std::string routingProtocol ("rpl");
  std::string rplConfigFilename ("rplConfig.csv");
  int appSetup = 0;
  int networkSetup = 0;
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
  cmd.AddValue("networkSetup", "how the trickle timer is set;0=normal, 1=dioIntervalMax=10s, 2=dioIntervalMax=20s", networkSetup);
  cmd.AddValue ("txPower", "Sending Power of the normal nodes.",txPower);
  cmd.AddValue ("energyModelEnabled","Enables or disables the assignment of an energy model to the nodes", energyModelEnabled);
  cmd.AddValue ("initialNodeEnergy","The available energy contained in the nodes battery", initialNodeEnergy);

  cmd.Parse (argc,argv);

  RngSeedManager::SetSeed (1);
  RngSeedManager::SetRun (run);

  

  Config::SetDefault ("ns3::Icmpv6L4Protocol::DAD", BooleanValue (false));
  Config::SetDefault ("ns3::Icmpv6L4Protocol::MaxUnicastSolicit", IntegerValue (1));
  Config::SetDefault ("ns3::Icmpv6L4Protocol::MaxMulticastSolicit", IntegerValue (1));

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
  
  // ---------------- Create Devices -------------------------------


  LrWpanHelper lrWpanHelper;
  // Add and install the LrWpanNetDevice for each node
  NetDeviceContainer lrwpanDevices = lrWpanHelper.Install(nodes);
  NetDeviceContainer energyDevices = NetDeviceContainer ();
  energyDevices.Add (lrwpanDevices.Get (4));
  energyDevices.Add (lrwpanDevices.Get (5));
  energyDevices.Add (lrwpanDevices.Get (6));
  energyDevices.Add (lrwpanDevices.Get (7));
  energyDevices.Add (lrwpanDevices.Get (8));
  NodeContainer energyNodes = NodeContainer (nodes.Get (4), nodes.Get (5), nodes.Get (6), nodes.Get (7), nodes.Get (8) );
  int numberOfEnergyNodes = 5;

  EnergySourceContainer sources;
  if(energyModelEnabled)
  {

    for (int i = 0; i< numberOfEnergyNodes; ++i)
    {
      Ptr<LrWpanPhy> phy = energyDevices.Get(i)->GetObject<LrWpanNetDevice>()->GetPhy();
      phy->SetAttribute("TxPower", DoubleValue(txPower));
    }
    BasicEnergySourceHelper basicSourceHelper;
    basicSourceHelper.Set ("BasicEnergySourceInitialEnergyJ", DoubleValue (initialNodeEnergy));
    basicSourceHelper.Set ("PeriodicEnergyUpdateInterval", TimeValue (Simulator::GetMaximumSimulationTime())); // do not reload the battery
    sources = basicSourceHelper.Install(energyNodes);
    LrWpanRadioEnergyModelHelper radioEnergyHelper;
    radioEnergyHelper.Set ("TxSendCurrentA", DoubleValue (txCurrentA));
    DeviceEnergyModelContainer deviceModels = radioEnergyHelper.Install (energyDevices, sources);
    //statHelper.Install(nodes.Get(it->first), DynamicCast<LrWpanNetDevice>(devContainer.Get(it->first)), energyModelEnabled, simEndTime);
  }

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
  //DIOIntervalMax = 10s
  if (networkSetup == 1)
  {
    // DIO Intervall Max = 2^DIOIntervalMin * 2^DIOIntervalDoublings
    // DIO Intervall Max = 8.192s
    rpl.Set ("DIOIntervalDoublings", UintegerValue (10));
    rpl.Set ("DIOIntervalMin", UintegerValue (3));
  }else if (networkSetup == 2)
  {
    // DIO Intervall Max = 16.384
    rpl.Set ("DIOIntervalDoublings", UintegerValue (11));
    rpl.Set ("DIOIntervalMin", UintegerValue (3));
  }
  

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

  if(energyModelEnabled)
  {
    // all sources are connected to node 1
    // energy source
    for (int n : {4,5,6,7,8})
    {
      Ptr<BasicEnergySource> basicSourcePtr = DynamicCast<BasicEnergySource> (sources.Get (n-4));
      AsciiTraceHelper asciiTraceHelper;
      Ptr<OutputStreamWrapper> remainingEnergy = asciiTraceHelper.CreateFileStream ("RPLEXAMPLE_remainingEnergy_node_" + std::to_string(n) + paramString + "" + ".txt");
      basicSourcePtr->TraceConnectWithoutContext ("RemainingEnergy", MakeBoundCallback (&RemainingEnergy, remainingEnergy));
    }
    // device energy model
    //Ptr<DeviceEnergyModel> basicRadioModelPtr = basicSourcePtr->FindDeviceEnergyModels ("ns3::LrWpanRadioEnergyModel").Get (0);
    //NS_ASSERT (basicRadioModelPtr);
    //basicRadioModelPtr->TraceConnectWithoutContext ("TotalEnergyConsumption", MakeCallback (&TotalEnergy));
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
    apps.Stop (simulationTime);
  }


  Simulator::Stop (simulationTime);
  
  lrWpanHelper.EnablePcapAll ("RPLEXAMPLEPCAP", true);
  
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
    