#include "rpl-example-helper.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("rplExampleHelper");

Ptr<rpl::RoutingProtocol> GetRpl(Ptr <Node> node)
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

Ptr<RipNg> GetRipNg(Ptr <Node> node)
{
  Ptr<Ipv6> ipv6 = node->GetObject<Ipv6> ();
  NS_ASSERT_MSG (ipv6, "Ipv6 not installed on node");
  Ptr<Ipv6RoutingProtocol> proto = ipv6->GetRoutingProtocol ();
  NS_ASSERT_MSG (proto, "Ipv6 routing not installed on node");
  Ptr<Ipv6ListRouting> listRouting = DynamicCast<Ipv6ListRouting> (proto);
  int16_t priority = -1;
  Ptr<RipNg> ripNg = DynamicCast<RipNg> (listRouting->GetRoutingProtocol (1, priority));
  std::cout << "priority: " << +priority << std::endl;

  if (ripNg)
  {
    return ripNg;
  }else
  {
    return nullptr;
  }
}

Ptr<MobilityModel> GetMobilityModel(Ptr <Node> node){

  Ptr<MobilityModel> mobilityModel = node->GetObject<MobilityModel> ();
  NS_ASSERT_MSG (mobilityModel, "mobilityModel not installed on node");
  //Ptr<rpl::RoutingProtocol> rpl = DynamicCast<rpl::RoutingProtocol> (proto);
  if (mobilityModel)
  {
    return mobilityModel;
  }else
  {
    return nullptr;
  }
}

std::string get_param_string (std::string routingProtocol, int run, int numberOfNodes, double trafficInterval, double applicationStart, double simulationTime, int networkSetup, int appSetup)
{
  //std::string paramString = "_routingProtocol_" + routingProtocol + "_run_" + std::to_string(run) + "_appSetup_" + std::to_string(appSetup) + "_trafficInterval_" + std::to_string(trafficInterval);
  std::ostringstream oss;
  oss << "_routingProtocol_" << routingProtocol 
      << "_run_" << run
      << "_numberOfNodes_" << numberOfNodes
      << "_trafficInterval_" << trafficInterval
      << "_networkSetup_" << networkSetup
      << "_appSetup_" << appSetup
      << "_applicationStart_" << applicationStart
      << "_simulationTime_" << simulationTime
  ;
  std::string paramString = oss.str();
  return paramString;
}

void SilenceNode (Ptr<Node> node, int32_t interface)
{
  NS_ASSERT (node);
  NS_LOG_FUNCTION ("silencing node " << node );
  Ptr<Ipv6> ipv6 = node->GetObject<Ipv6>();

  NS_ASSERT(ipv6);
  if (!ipv6->IsUp (interface))
  {
    NS_LOG_WARN ("ipv6 interface was not up! :" << ipv6->IsUp (interface));
  }
  NS_LOG_DEBUG ("was Up: " << ipv6->IsUp (interface));
  ipv6->SetDown (interface);
}

void TurnOnNode (Ptr<Node> node, int32_t interface, Ipv6Address address)
{
  NS_ASSERT (node);
  NS_LOG_FUNCTION ("turning on node " << node );
  Ptr<Ipv6> ipv6 = node->GetObject<Ipv6>();

  NS_ASSERT(ipv6);
  if (ipv6->IsUp (interface))
  {
    NS_LOG_WARN ("ipv6 interface was up! :" << ipv6->IsUp (interface));
  }
  NS_LOG_DEBUG ("was Down: " << ipv6->IsUp (interface));

  Ipv6InterfaceAddress ipv6Addr = Ipv6InterfaceAddress (address);

  ipv6->AddAddress (interface, ipv6Addr);
  ipv6->SetMetric (interface, 1);
  ipv6->SetUp (interface);
}

int GetNextHighestSquareEdgeLength(int number)
{
  // up to sqrt (INT_MAX)
  for (int i = 2; i <= 46340; i++)
  {
    if (i*i >= number )
    {
      return i;
    }
  }
  return 0;
}

void UpdatePrefParentTraceSink(Ptr<OutputStreamWrapper> stream, rpl::RplNode rplNode)
{ 
  *stream->GetStream () << Simulator::Now().GetSeconds();
  *stream->GetStream () << ", " << rplNode.address;
  *stream->GetStream () << ", " << rplNode.rank;
  *stream->GetStream () << std::endl;                                                                      
}

void RplNodeDetachedConsoleSink(rpl::RplNode rplNode)
{
  NS_LOG_UNCOND (Simulator::Now().GetSeconds() << ", " << rplNode.address << ", " << rplNode.rank );
}

void RplNodeDetachedSink(Ptr<OutputStreamWrapper> stream, rpl::RplNode rplNode)
{ 
  //NS_LOG_UNCOND (Simulator::Now().GetSeconds() << ", " << rplNode.address << ", " << rplNode.rank );
  *stream->GetStream () << "Detached from parent: ";
  *stream->GetStream () << Simulator::Now().GetSeconds();
  *stream->GetStream () << ", " << rplNode.address;
  *stream->GetStream () << ", " << rplNode.rank;
  *stream->GetStream () << std::endl;                                                                      
}

void RouteAddedTraceSink(Ptr<OutputStreamWrapper> stream, rpl::RplRoutingTableEntry entry)
{ 
  *stream->GetStream () << Simulator::Now().GetSeconds();
  *stream->GetStream () << ", " << entry.GetDest ();
  *stream->GetStream () << ", " << entry.GetGateway ();
  *stream->GetStream () << ", " << entry.GetInterface ();
  *stream->GetStream () << std::endl;
}

void UdpTxTraceWithAddressesSink (Ptr<OutputStreamWrapper> stream, Ptr< const Packet > packet, const Address & from, const Address & localAddress)
{
  uint8_t *buffer = new uint8_t[packet->GetSize ()];
  packet->CopyData (buffer, packet->GetSize ()); 
  std::string packetConetent = std::string(buffer, buffer+packet->GetSize());

  *stream->GetStream () << Simulator::Now ().GetSeconds()
      << ", " << Inet6SocketAddress::ConvertFrom (from).GetIpv6 ()
      << ", " << Inet6SocketAddress::ConvertFrom (localAddress).GetIpv6 ()
      << ", " << packetConetent << std::endl;
}


void UdpRxTraceWithAddressesSink (Ptr<OutputStreamWrapper> stream, Ptr< const Packet > packet, const Address & from, const Address & localAddress)
{

  TimestampTag timestamp;
  // Should never not be found since the sender is adding it, but
  // you never know.
  Time tx = Time ();
  Ptr<TimeMinMaxAvgTotalCalculator> delay;
  if (packet->FindFirstMatchingByteTag (timestamp)) 
  {
    tx = timestamp.GetTimestamp ();

     
    if (delay) 
    {
      delay->Update (Simulator::Now () - tx);
    }
  }else
  {
    NS_ABORT_MSG ("No Time Tag in packet!");
  }

  uint8_t *buffer = new uint8_t[packet->GetSize ()];
  packet->CopyData (buffer, packet->GetSize ()); 
  std::string packetConetent = std::string(buffer, buffer+packet->GetSize());

  *stream->GetStream () << Simulator::Now ().GetSeconds()
      << ", " << tx.GetSeconds ()
      << ", " << Inet6SocketAddress::ConvertFrom (from).GetIpv6 ()
      << ", " << Inet6SocketAddress::ConvertFrom (localAddress).GetIpv6 ()
      << ", " << packetConetent << std::endl;
}

// Prints actual position and velocity when a course change event occurs
void CourseChangeTraceSink (Ptr<OutputStreamWrapper> stream, Ptr<const MobilityModel> mobility)
{
  Vector pos = mobility->GetPosition (); // Get position
  Vector vel = mobility->GetVelocity (); // Get velocity
 
  // Prints position and velocities
  *stream->GetStream () << Simulator::Now ().GetSeconds() 
      << ", posX=" << pos.x << ", posY=" << pos.y
      << ", posZ=" << pos.z << "; velX=" << vel.x << ", velY=" << vel.y
      << ", velZ=" << vel.z << std::endl;
}

void RemainingEnergy (Ptr<OutputStreamWrapper> stream, double oldValue, double remainingEnergy)
{
  *stream->GetStream () << Simulator::Now().GetSeconds();
  *stream->GetStream () << ", " << oldValue;
  *stream->GetStream () << ", " << remainingEnergy;
  *stream->GetStream () << std::endl;     
  //NS_LOG_UNCOND (Simulator::Now ().GetSeconds () << "s Current remaining energy = " << remainingEnergy << "J");
}

void RipNgRouteAddedTraceSink(Ptr<OutputStreamWrapper> stream, RipNgRoutingTableEntry entry)
{ 
  *stream->GetStream () << Simulator::Now().GetSeconds();
  *stream->GetStream () << ", " << entry.GetDest ();
  *stream->GetStream () << ", " << entry.GetGateway ();
  *stream->GetStream () << ", " << entry.GetInterface ();
  *stream->GetStream () << std::endl;                                                                      
}