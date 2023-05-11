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
#include "rpl.h"


#include "ns3/log.h"

#include "ns3/trickle-timer.h"
#include "ns3/nstime.h"
#include "ns3/inet-socket-address.h"
#include "ns3/socket-factory.h"
#include "ns3/ipv6-raw-socket-factory.h"
#include "ns3/ipv6-packet-info-tag.h"
#include "ns3/boolean.h"
#include "ns3/uinteger.h"
#include<iostream>

namespace ns3 {
NS_LOG_COMPONENT_DEFINE("Rpl");
namespace rpl{

NS_OBJECT_ENSURE_REGISTERED(RoutingProtocol);


RoutingProtocol::RoutingProtocol (bool isRoot,
                                  /// Router parameters
                                  uint8_t instanceId,
                                  std::set<RplObjectiveCodePoint_e> ocps,
                                  std::set<uint8_t> routingMCs,
                                  uint32_t validLifetime,
                                  uint32_t preferredLifetime,
                                  bool onLink,
                                  bool autonomousAddressConfiguration,
                                  bool versionPredicate,
                                  bool instanceIdPredicate,
                                  bool dodagIdPredicate,
                                  bool k,
                                  rpl::RplMop_e mop,
                                  uint8_t routePreference,
                                  uint32_t routeLifetime,
                                  Ipv6Prefix prefix,
                                  /// Non-DODAG-Root Router
                                  Ipv6Prefix targetPrefix,
                                  bool poison,
                                  bool repair,
                                  /// DIS Mode of Operation
                                  rpl::RplDisMop_e disMop,
                                  Time disMessageTime,
                                  int numberOfDisMessages,
                                  /// DODAG Root
                                  uint8_t dioIntervalDoublings,
                                  uint8_t dioIntervalMin,
                                  uint8_t dioRedundancyConstant,
                                  uint8_t pathControlSize,
                                  uint16_t minHopRankIncrease,
                                  uint8_t dodagPreference,
                                  Ipv6Address dodagId,
                                  /// DAO based Parameters
                                  Time delayDao,
                                  int numberOfDaoRetries)
:
m_isRoot (isRoot),
/// Router parameters
m_instanceId (instanceId),
m_ocps (ocps),
m_routingMCs (routingMCs),
m_validLifetime (validLifetime),
m_preferredLifetime (preferredLifetime),
m_onLink (onLink),
m_autonomousAddressConfiguration (autonomousAddressConfiguration),
m_versionPredicate (versionPredicate),
m_instanceIdPredicate (instanceIdPredicate),
m_dodagIdPredicate (dodagIdPredicate),
m_k (k),
m_mop (mop),
m_routePreference (routePreference),
m_routeLifetime (routeLifetime),
m_prefix (prefix),
/// Non-DODAG-Root Router
m_targetPrefix (targetPrefix),
m_poison (poison),
m_repair (repair),
/// DIS Mode of Operation
m_disMop (disMop),
m_disMessageTime (disMessageTime),
m_numberOfDisMessages (numberOfDisMessages),
/// DODAG Root
m_dioIntervalDoublings (dioIntervalDoublings),
m_dioIntervalMin (dioIntervalMin),
m_dioRedundancyConstant (dioRedundancyConstant),
m_pathControlSize (pathControlSize),
m_minHopRankIncrease (minHopRankIncrease),
m_dodagPreference (dodagPreference),
m_dodagId (dodagId),
/// DAO based Parameters
m_delayDao (delayDao),
m_numberOfDaoRetries (numberOfDaoRetries)
{
  NS_LOG_DEBUG("--- RPL constructor ---");

  /*Ptr<Packet> packet = Create<Packet> ();
  DisHeader inDisHeader;
  RplIcmpv6Header inRplIcmpv6Header;
  RplIcmpv6Header outRplIcmpv6Header;
  DisHeader outDisHeader;
  inDisHeader.SetFlags (3);
  std::cout<<"---Input---"<< std::endl;
  inDisHeader.Print (std::cout);
  std::cout <<std::endl;
  std::cout<<"The Value of 'type' is "<< unsigned(inRplIcmpv6Header.GetType())<<std::endl;
  std::cout<<"The Value of 'code' is "<< unsigned(inRplIcmpv6Header.GetCode()) <<std::endl;
  std::cout<<"The Value of 'flags' is "<< unsigned(inDisHeader.GetFlags()) <<std::endl;

  //packet->AddHeader (inOption);
  packet->AddHeader (inDisHeader);
  packet->AddHeader (inRplIcmpv6Header);

  packet->RemoveHeader (outRplIcmpv6Header);
  packet->RemoveHeader (outDisHeader);
  //packet->RemoveHeader (outOption);

  std::cout <<std::endl;
  std::cout<<"---Output---"<< std::endl;
  outRplIcmpv6Header.Print (std::cout);
  std::cout <<std::endl;
  std::cout<<"The Value of 'type' is "<< unsigned(outRplIcmpv6Header.GetType())<<std::endl;
  std::cout<<"The Value of 'code' is "<< unsigned(outRplIcmpv6Header.GetCode()) <<std::endl;
  std::cout<<"The Value of 'flags' is "<< unsigned(outDisHeader.GetFlags()) <<std::endl;
  //outDisHeader = outRplHeader;

  //std::cout<<"The Value of 'flags' is "<< unsigned(outHeader.GetFlags()) <<std::endl;

  RplHeaderOption option;
  std::vector<uint8_t> testdata;
  uint8_t rawPrefix[16];
  for (int i = 0; i < 5; i++) {
    testdata.push_back(i);
  }
  std::fill(rawPrefix, rawPrefix+16, 0);
  rawPrefix[0] = 0xFF;
  rawPrefix[1] = 0xF0;

  Ipv6Prefix objectPrefix (rawPrefix);
  
  //option.SetDagMetricContainer(testdata);
  //option.SetPadN (2);
  option.SetRouteInformation (0b00011000, 0x01020304, objectPrefix);
  option.Print();*/

}


void RoutingProtocol::DoInitialize ()
{
  NS_LOG_FUNCTION(this);
  m_initialized = true;

  for (uint32_t i = 0 ; i < m_ipv6->GetNInterfaces (); i++)
  {
    RegisterSockets (i);
  }

  m_trickleTimer.SetFunction (&RoutingProtocol::ExpireTrickleTimer, this);

  if (m_disMop == DIS_MOP_SEND)
  {
    m_disMessageTimer.SetFunction (&RoutingProtocol::DisExpireTimer, this);
    m_disMessageTimer.SetDelay (m_disMessageTime);
    m_disMessageTimer.Schedule ();
  }

  /*if (!m_isRoot)
  {
    m_daoAckTimer.SetFunction (&RoutingProtocol::DaoAckExpireTimer, this);
    m_daoAckTimer.SetDelay (m_daoAckTimeout);
  }*/

  m_routingTable.SetRouteAddedTrace (m_routeAddedTrace);

  if (m_isRoot)
  {
    InitRoot ();
  }

  //std::cout<<"Is Root: "<< +m_isRoot <<std::endl;
  //std::cout<<"MOP: "<< +m_mop <<std::endl;
  //std::cout<<"Instance ID: "<< +m_instanceId <<std::endl;

  Ipv6RoutingProtocol::DoInitialize ();
}

void RoutingProtocol::InitRoot ()
{
  NS_LOG_FUNCTION(this);
  RplObjectiveCodePoint_e current_ocp;

  NS_ABORT_MSG_IF (m_mop > 3, "Mode of Operation is invalid");
  if (m_mop >= 2)
  {
    m_isStoring = true;
  }else{
    m_isStoring = false;
  }


  NS_ABORT_MSG_IF (m_ocps.empty (), "No OCPs given for root node");

  // take the first supported OCP found as OCP
  // TODO may change this to a better method
  for (const RplObjectiveCodePoint_e iter_ocp : m_ocps)
  {
    current_ocp = iter_ocp;
    break;
  }

  m_ocp = RplObjectiveFunction (current_ocp, m_minHopRankIncrease);

  m_rank = ROOT_RANK;
  m_dtsn = 0;

  if (m_dodagId.IsAny ())
  {
    // use first global address from node as DODAG ID
    bool foundGlobal = false;
    for (uint32_t interface = 0; interface < m_ipv6->GetNInterfaces (); interface++)
    {
      if (m_interfaceExclusions.find (interface) == m_interfaceExclusions.end ())
      {
        for (uint32_t addressIndex = 0; addressIndex < m_ipv6->GetNAddresses (interface); addressIndex++)
        {
          Ipv6InterfaceAddress address = m_ipv6->GetAddress (interface, addressIndex);

          if (address.GetScope() == Ipv6InterfaceAddress::GLOBAL)
          {
            m_dodagId = address.GetAddress ();  // TODO change this to add correct dodagId
            foundGlobal = true;
            break;
          }
        }
      }
      if (foundGlobal)
      {
        break;
      }
    }
    if (!foundGlobal)
    {
      NS_ABORT_MSG ("Root node does not have a global IP");
    }
  }
  m_isGrounded = true;
  // [RFC 6550, 8.3.1]
  m_trickleTimer.SetParameters (MilliSeconds(pow(2,m_dioIntervalMin)), m_dioIntervalDoublings, m_dioRedundancyConstant);
  m_trickleTimer.Enable ();
  m_trickleTimer.InconsistentEvent ();
}

TypeId RoutingProtocol::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::rpl::RoutingProtocol")
  .SetParent<Ipv6RoutingProtocol> ()
  .SetGroupName ("Rpl")
  .AddConstructor<RoutingProtocol> ()
  .AddAttribute ("MaxDaoParents",
                "Upper bound for number of DAO parents",
                UintegerValue (1),
                MakeUintegerAccessor (&RoutingProtocol::m_maxDaoParents),
                MakeUintegerChecker<uint8_t>())
  .AddAttribute ("DIOIntervalDoublings",
                "8-bit unsigned integer used to configure Imax of the DIO Trickle timer",
                UintegerValue (DEFAULT_DIO_INTERVAL_DOUBLINGS),
                MakeUintegerAccessor (&RoutingProtocol::m_dioIntervalDoublings),
                MakeUintegerChecker<uint8_t>())
  .AddAttribute ("DIOIntervalMin",
                "8-bit unsigned integer used to configure Imin of the DIO Trickle timer",
                UintegerValue (DEFAULT_DIO_INTERVAL_MIN),
                MakeUintegerAccessor (&RoutingProtocol::m_dioIntervalMin),
                MakeUintegerChecker<uint8_t>())
  .AddAttribute ("DIORedundancyConstant",
                "8-bit unsigned integer used to configure k of the DIO Trickle timer",
                UintegerValue (DEFAULT_DIO_REDUNDANCY_CONSTANT),
                MakeUintegerAccessor (&RoutingProtocol::m_dioRedundancyConstant),
                MakeUintegerChecker<uint8_t>())
  .AddTraceSource ("UpdatedPrefParent",
              "preferered parent updates to trace",
              MakeTraceSourceAccessor (&RoutingProtocol::m_updatedPrefParentTrace),
              "ns3::rpl::RoutingProtocol::updatedPrefParentCallback")
  .AddTraceSource ("DetachFromDodag",
              "trace when DODAG detaches",
              MakeTraceSourceAccessor (&RoutingProtocol::m_detachFromDodagTrace),
              "ns3::rpl::RoutingProtocol::detachFromDodagCallback")
  .AddTraceSource ("routeAdded",
              "every route that is added to the routing table to trace\n callback passed to routing table",
              MakeTraceSourceAccessor (&RoutingProtocol::m_routeAddedTrace),
              "ns3::rpl::routeAddedCallback")
  ;
  return tid;
}

bool RoutingProtocol::RouteInput (Ptr< const Packet > p,
  const Ipv6Header& header,
  Ptr< const NetDevice > idev,
  UnicastForwardCallback ucb,
  MulticastForwardCallback mcb,
  LocalDeliverCallback lcb,
  ErrorCallback ecb)
{
  NS_LOG_FUNCTION (this << p << header << header.GetSource () << header.GetDestination () << idev);
  NS_ASSERT (m_ipv6 != 0);
  // Check if input device supports IP
  NS_ASSERT (m_ipv6->GetInterfaceForDevice (idev) >= 0);
  uint32_t iif = m_ipv6->GetInterfaceForDevice (idev);
  Ipv6Address dst = header.GetDestination ();

  /*if (isMyAddress (header.GetSource (), iif))
  {
    return false;
  }*/


  // Multicast recognition; handle local delivery here
  if (dst.IsMulticast ())
  {
    if (header.GetDestination ().IsLinkLocalMulticast ())
    {
      NS_LOG_LOGIC ("Link-local Multicast is not going to be forwarded");
      return false;
    }
    NS_LOG_LOGIC ("Multicast destination");
    NS_LOG_DEBUG ("TODO RouteInput - multicast fowarding not implemented yet, take inspiration from ipv6-static-routing");
    return false;
  }

  if (header.GetDestination ().IsLinkLocal () ||
      header.GetSource ().IsLinkLocal ())
  {
    NS_LOG_LOGIC ("Dropping packet not for me and with src or dst LinkLocal");
    if (!ecb.IsNull ())
      {
        ecb (p, header, Socket::ERROR_NOROUTETOHOST);
      }
    return false;
  }

  // Check if input device supports IP forwarding
  if (m_ipv6->IsForwarding (iif) == false)
  {
    NS_LOG_LOGIC ("Forwarding disabled for this interface");
    if (!ecb.IsNull ())
    {
      ecb (p, header, Socket::ERROR_NOROUTETOHOST);
    }
    return true;
  }
  // Next, try to find a route
  NS_LOG_LOGIC ("Unicast destination");
  Ptr<Ipv6Route> rtentry = Lookup (header.GetDestination (), true);

  if (rtentry != 0)
  {
    NS_LOG_LOGIC ("Found unicast destination - calling unicast callback");
    ucb (idev, rtentry, p, header);  // unicast forwarding callback
    return true;
  }
  else
  {
    NS_LOG_LOGIC ("Did not find unicast destination - returning false");
    return false; // Let other routing protocols try to handle this
  }
}

Ptr<Ipv6Route> RoutingProtocol::RouteOutput (Ptr< Packet > p,
  const Ipv6Header& header,
  Ptr< NetDevice > oif,
  Socket::SocketErrno & sockerr)
{
  NS_LOG_FUNCTION (this << header << oif);
  Ptr<Ipv6Route> rtentry;
  rtentry = Create<Ipv6Route> ();

  Ipv6Address destination = header.GetDestination ();

  /*if (destination.IsMulticast ())
  {
    // Note:  Multicast routes for outbound packets are stored in the
    // normal unicast table.  An implication of this is that it is not
    // possible to source multicast datagrams on multiple interfaces.
    // This is a well-known property of sockets implementation on
    // many Unix variants.
    // So, we just log it and fall through to LookupStatic ()
    NS_LOG_LOGIC ("RouteOutput (): Multicast destination");
  }*/
  rtentry = Lookup (destination, true, oif);
  if (rtentry)
  {
    sockerr = Socket::ERROR_NOTERROR;
  }
  else
  {
    sockerr = Socket::ERROR_NOROUTETOHOST;
  }
  return rtentry;
}

void RoutingProtocol::SetIpv6 (Ptr<Ipv6> ipv6)
{
  NS_LOG_FUNCTION (this << ipv6);

  NS_ASSERT (m_ipv6 == 0 && ipv6 != 0);
  uint32_t i = 0;
  m_ipv6 = ipv6;

  for (i = 0; i < m_ipv6->GetNInterfaces (); i++)
    {
      if (m_ipv6->IsUp (i))
        {
          NotifyInterfaceUp (i);
        }
      else
        {
          NotifyInterfaceDown (i);
        }
    }

  Simulator::ScheduleNow (&RoutingProtocol::Start, this);
}
void RoutingProtocol::Start ()
{
}

bool RoutingProtocol::isRplAllAddress (Ipv6Address addr)
{
  if (addr == Ipv6Address (RPL_ALL_NODE))
  {
    return true;
  }
  return false;
}

bool RoutingProtocol::isMyAddress (Ipv6Address addr, Ptr<NetDevice> dev)
{
  // if interface is not given, search all interfaces for the address
  if (!dev)
  {
    return isMyAddress (addr);
  }
  else
  {
    uint32_t interface = m_ipv6->GetInterfaceForDevice (dev);
    return isMyAddress (addr, interface);
  }
}

bool RoutingProtocol::isMyAddress (Ipv6Address addr, uint32_t interface)
{

  for (uint32_t addressIndex = 0; addressIndex < m_ipv6->GetNAddresses (interface); addressIndex++)
  {
    if (addr == m_ipv6->GetAddress (interface, addressIndex).GetAddress ())
    {
      return true;
    }
  }
  return false;
}

bool RoutingProtocol::isMyAddress (Ipv6Address addr)
{
  bool forMe = false;

  for (uint32_t interface = 0; interface < m_ipv6->GetNInterfaces (); interface++)
  {
    if (m_interfaceExclusions.find (interface) == m_interfaceExclusions.end ())
    {
      forMe = isMyAddress (addr, interface);
      if (forMe)
      {
        return true;
      }
    }
  }

  return false;
}


bool RoutingProtocol::isDodagParent (Ipv6Address addr, uint32_t interface)
{
  for (RplNode dodagParent : m_dodagParents)
  {
    if (addr == dodagParent.address && interface == dodagParent.interface)
    {
      return true;
    }
  }
  return false;
}

void RoutingProtocol::Receive (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this << socket);

  Address sender;
  Ptr<Packet> packet = socket->RecvFrom (sender);
  Inet6SocketAddress senderAddr = Inet6SocketAddress::ConvertFrom (sender);
  NS_LOG_INFO ("Received " << *packet << " from " << senderAddr);

  Ipv6Address senderAddress = senderAddr.GetIpv6 ();

  Ipv6PacketInfoTag interfaceInfo;
  if (!packet->RemovePacketTag (interfaceInfo))
  {
    NS_ABORT_MSG ("No incoming interface on RPL message, aborting.");
  }
  uint32_t incomingIf = interfaceInfo.GetRecvIf ();
  Ptr<Node> node = this->GetObject<Node> ();
  Ptr<NetDevice> dev = node->GetDevice (incomingIf);
  int32_t ipInterfaceIndex = m_ipv6->GetInterfaceForDevice (dev);
  if (ipInterfaceIndex < 0)
  {
    NS_ABORT_MSG ("received message on non existing interface");
  }

  /*SocketIpv6HopLimitTag hoplimitTag;
  if (!packet->RemovePacketTag (hoplimitTag))
  {
    NS_ABORT_MSG ("No incoming Hop Count on RPL message, aborting.");
  }
  uint8_t hopLimit = hoplimitTag.GetHopLimit ();*/

  Ipv6Header ipv6Header;
  packet->RemoveHeader (ipv6Header);

  if (!isRplAllAddress (ipv6Header.GetDestination ()) && !isMyAddress (ipv6Header.GetDestination (), ipInterfaceIndex))
  {
    NS_LOG_LOGIC ("Packet Destination Address is neither RPL All Address or a IP address on this node - dropping packet");
    return;
  }

  NS_ABORT_MSG_UNLESS (ipv6Header.GetNextHeader () == Ipv6Header::IPV6_ICMPV6, "The received Packet is not an ICMPv6 packet");
  RplIcmpv6Header rplIcmpv6Header;
  packet->RemoveHeader (rplIcmpv6Header);


  // ignore packages sent by myself except ICMPv6 destination unreachable messages
  int32_t interfaceForAddress = m_ipv6->GetInterfaceForAddress (senderAddress);
  if (interfaceForAddress != -1)
  {
    if (rplIcmpv6Header.GetType () == 1)
    {
      ReceiveDu (packet, ipv6Header, rplIcmpv6Header);
      return;
    }else
    {
      NS_LOG_LOGIC ("Ignoring a packet sent by myself.");
      return;
    }

  }

  if (rplIcmpv6Header.GetType () != 155)
  {
    NS_LOG_INFO ("Received ICMPv6 Message with type other than 155 which is " << +rplIcmpv6Header.GetType ());
    return;
  }
  switch (rplIcmpv6Header.GetCode ())
  {
    case TYPE_DIS:
      ReceiveDis (packet, ipv6Header);
      break;
    case TYPE_DIO:
      ReceiveDio (packet, ipv6Header, ipInterfaceIndex);
      break;
    case TYPE_DAO:
      ReceiveDao (packet, ipv6Header, ipInterfaceIndex);
      break;
    case TYPE_DAO_ACK:
      ReceiveDaoAck (packet, ipv6Header);
      break;
    case TYPE_CC:
      NS_ABORT_MSG ("TODO add what to do upon receiving base message with code "<< +rplIcmpv6Header.GetCode ());
      break;
    default:
      NS_ABORT_MSG ("Receiving base message with the invalid code " << +rplIcmpv6Header.GetCode ());
  }
}

void RoutingProtocol::ReceiveDu (Ptr<Packet> packet, Ipv6Header ipv6Header, RplIcmpv6Header rplIcmpv6Header)
{
  NS_LOG_FUNCTION (this << +packet << ipv6Header);
  NS_LOG_LOGIC ("Received Destination Unreachable from " << ipv6Header.GetSource ());
  uint16_t payloadLength;
  uint8_t code = rplIcmpv6Header.GetCode ();
  // check weather it is reasonable to remove the destination from the routing table by the code of the destination unreachable 
  if (code == 4 || code == 7 || code == 8)
  {
    NS_LOG_INFO ("Unreasonable to remove destination from routing table");
    return;
  }

  payloadLength = ipv6Header.GetPayloadLength () - 4;

  NS_ASSERT_MSG (payloadLength >= 4, "Malformed Destination Unreachable packet");
  // remove unused bits from destination unreachable message
  // RFC 4443, 3.1
  packet->RemoveAtStart (4);
  payloadLength = payloadLength - 4;

  
  Ipv6Header invokingPacketIpv6Header;
  NS_ASSERT_MSG (payloadLength >= invokingPacketIpv6Header.GetSerializedSize (), "ipv6Header larger than payload left");
  packet->RemoveHeader (invokingPacketIpv6Header);

  Ptr<Ipv6Route> rtentry;
  Ptr<NetDevice> interface = 0;
  rtentry = Lookup (invokingPacketIpv6Header.GetDestination (), false, interface);

  if (!rtentry)
  {
    NS_LOG_LOGIC ("No route to this destination found");
    return;
  }

  // the unreachable address is the next hop address, if is given in the rtentry, otherwise the destination
  Ipv6Address unreachableAddress;
  uint32_t interfaceForUnreachableAddress = m_ipv6->GetInterfaceForDevice (rtentry->GetOutputDevice ());
  if (rtentry->GetGateway () == Ipv6Address::GetZero () || rtentry->GetGateway ().IsAny ())
  {
    unreachableAddress = rtentry->GetDestination ();
  }else
  {
    unreachableAddress = rtentry->GetGateway ();
  }

  // remove routes where destination or next Hop is the unreachable address
  m_routingTable.RemoveRoutesOfTarget (unreachableAddress, interfaceForUnreachableAddress);
  if (!m_isRoot)
  {
    DeleteParent (unreachableAddress, interfaceForUnreachableAddress);
  }

}

void RoutingProtocol::ReceiveDis (Ptr<Packet> packet, Ipv6Header ipv6Header)
{
  NS_LOG_FUNCTION (this << +packet << ipv6Header);
  NS_LOG_LOGIC ("Received DIS from " << ipv6Header.GetSource ());
  DisHeader disHeader;
  uint16_t payloadLength;

  packet->RemoveHeader (disHeader);
  payloadLength = ipv6Header.GetPayloadLength () - 4 - disHeader.GetSerializedSize ();

  while (payloadLength > 0)
  {
    RplHeaderOption option;
    packet->RemoveHeader (option);
    payloadLength -= option.GetSerializedSize ();
    switch (option.GetType ())
    {
    case OPTION_TYPE_PAD1:
      break;
    case OPTION_TYPE_PADN:
      break;
    case OPTION_TYPE_SOLICITED_INFORMATION:
      //TODO add stuff here
      NS_ABORT_MSG ("TODO add what to do when receiving DIS with SI option");
      break;
    default:
      NS_ABORT_MSG ("Receiving option with the invalid type " << +option.GetType ());
      break;
    }
  }
  
  if (ipv6Header.GetDestination ().IsMulticast ())
  {
    m_trickleTimer.InconsistentEvent ();
  }
  else if (ipv6Header.GetDestination ().IsLinkLocal ())
  {
    // send DIO
    NS_LOG_LOGIC ("RPL: Send DIO with DODAG Configuration to Node (" << ipv6Header.GetSource () << ")that sent the DIS message");
    Ptr<Packet> packet = Create<Packet> ();
    RplHeaderOption dodagConfig;
    DioHeader dioHeader (m_instanceId, m_dodagVersionNumber.GetValue (), m_rank, m_isGrounded, m_mop, m_dodagPreference, m_dtsn++, 0, 0, m_dodagId);
    RplIcmpv6Header rplIcmpv6Header (TYPE_DIO);
    dodagConfig.SetDodagConfiguration (m_pathControlSize, m_dioIntervalDoublings, m_dioIntervalMin, m_dioRedundancyConstant, 0, m_minHopRankIncrease, m_ocp.GetOcp (), m_defaultLifetime, m_defaultLifetimeUnit);

    packet->AddHeader (dodagConfig);
    packet->AddHeader (dioHeader);
    packet->AddHeader (rplIcmpv6Header);

    SendOnAllInterfaces (packet, Inet6SocketAddress (ipv6Header.GetSource ()));
  }else{
    NS_ABORT_MSG ("Receiving DIS message with invalid destination address (not multicast or link-local)");
  }
}

void RoutingProtocol::ReceiveDio (Ptr<Packet> packet, Ipv6Header ipv6Header, uint32_t incomingInterface)
{
  NS_LOG_FUNCTION (this << +packet << ipv6Header);
  DioHeader dioHeader;
  uint16_t payloadLength;
  bool receivedDodagConfiguration = false;

  // drop received DIO if node is root
  if (m_isRoot)
  {
    NS_LOG_LOGIC ("Received DIO as root, discarding packet");
    return;
  }
  NS_LOG_LOGIC ("Received DIO from " << ipv6Header.GetSource ());

  packet->RemoveHeader (dioHeader);
  payloadLength = ipv6Header.GetPayloadLength () - 4 - dioHeader.GetSerializedSize ();

  if (!m_receivedDio && dioHeader.GetRank () != INFINITE_RANK)
  {
    m_receivedDio = true;
    if (m_disMessageTimer.IsRunning ())
    {
      m_disMessageTimer.Suspend ();
    }
  }

  while (payloadLength > 0)
  {
    RplHeaderOption option;
    packet->RemoveHeader (option);
    payloadLength -= option.GetSerializedSize ();
    switch (option.GetType ())
    {
    case OPTION_TYPE_PAD1:
      break;
    case OPTION_TYPE_PADN:
      break;
    case OPTION_TYPE_DAG_METRIC_CONTAINER:
      NS_ABORT_MSG ("TODO add what to do when receiving DIO with DAG_METRIC_CONTAINER option");
      break;
    case OPTION_TYPE_ROUTING_INFORMATION:
      NS_ABORT_MSG ("TODO add what to do when receiving DIO with ROUTING_INFORMATION option");
      break;
    case OPTION_TYPE_DODAG_CONFIGURATION:
      {// declaring a new scope to declare dodagConfiguration
        receivedDodagConfiguration = true;
        const RplHeaderOption::DodagConfiguration &dodagConfiguration = option.GetDodagConfiguration();
        if (m_dodagId.IsAny ())
        {
          RplObjectiveCodePoint_e ocp = static_cast<RplObjectiveCodePoint_e>(dodagConfiguration.ocp);
          if ( m_ocps.find (ocp) != m_ocps.end() )
          {
            m_ocp.ProcessingDio (ocp, dodagConfiguration.minHopRankIncrease, dodagConfiguration.maxRankIncrease);
            m_dioIntervalDoublings = dodagConfiguration.dioIntervalDoublings;
            m_dioIntervalMin = dodagConfiguration.dioIntervalMin;
            m_dioRedundancyConstant = dodagConfiguration.dioRedundancyConstant;
            m_pathControlSize = dodagConfiguration.pcs;
            m_minHopRankIncrease = dodagConfiguration.minHopRankIncrease;
            m_defaultLifetime = dodagConfiguration.defaultLifetime;
            m_defaultLifetimeUnit = dodagConfiguration.lifetimeUnit;
            m_instanceId = dioHeader.GetRplInstanceId ();
            m_dodagVersionNumber = dioHeader.GetVersionNumber ();
            m_rank = m_ocp.CalculateRank (dioHeader.GetRank ());
            NS_LOG_DEBUG ("Setting Rank to " << +m_ocp.DagRank (m_rank));
            m_isGrounded = dioHeader.GetGrounded ();
            m_mop = static_cast<RplMop_e>(dioHeader.GetMop ());
            m_dodagPreference = dioHeader.GetPrf ();
            m_dtsn = dioHeader.GetDtsn ();
            m_dodagId = dioHeader.GetDodagId ();
            m_dtsnChanged = true;

            if (m_mop >= 2)
            {
              m_isStoring = true;
            }else{
              m_isStoring = false;
            }
            m_trickleTimer.SetParameters (MilliSeconds(pow(2,m_dioIntervalMin)), m_dioIntervalDoublings, m_dioRedundancyConstant);
            m_trickleTimer.Enable ();
          }else
          {
            NS_ABORT_MSG ("TODO add what to do when ocp is received which is not supported by this node");
          }
          break;
          
        }else{
          NS_LOG_DEBUG ("TODO add what to do when receiving DIO with DODAG Config option and Node already joined DODAG");
        }
        break;
      }
    case OPTION_TYPE_PREFIX_INFORMATION:
      NS_ABORT_MSG ("TODO add what to do when receiving DIO with PI option");
      break;
    default:
      NS_ABORT_MSG ("Receiving option with the invalid type " << +option.GetType ());
      break;
    }
  }

  // if node joined no DODAG and no dodagConfiguration means the node hast to ask the sender for a DODAG Configuration Option
  if (m_dodagId.IsAny () && !receivedDodagConfiguration)
  {
    // send DIS
    NS_LOG_LOGIC ("RPL: Send DIS Unicast to request a DODAG Configuration Option from " << ipv6Header.GetSource ());
    Ptr<Packet> packet = Create<Packet> ();
    DisHeader disHeader;
    RplIcmpv6Header rplIcmpv6Header (TYPE_DIS);
    packet->AddHeader (disHeader);
    packet->AddHeader (rplIcmpv6Header);

    SendOnAllInterfaces (packet, Inet6SocketAddress (ipv6Header.GetSource ()));
    return;
  }
  
  // check for route poisoning, then delete parent from upward routes and as preferred parent
  if (!m_dodagId.IsAny () && dioHeader.GetRank () == INFINITE_RANK)
  {
    if (isDodagParent (ipv6Header.GetSource (), incomingInterface))
    {
      NS_LOG_LOGIC ("INFINITE_RANK is advertised from a parent - delete parent and remove it from DODAG parents");
      DeleteParent (ipv6Header.GetSource (), incomingInterface);
      return;
    }
    else
    {
      NS_LOG_LOGIC ("INFINITE_RANK is advertised from a non-parent - continuing");
      return;
    }

  }


  
  if (m_dodagId == dioHeader.GetDodagId () && m_instanceId == dioHeader.GetRplInstanceId () && m_dodagVersionNumber == dioHeader.GetVersionNumber ())
  {
    // add link local address when node is in the same dodag
    m_routingTable.AddRoute (ipv6Header.GetSource (), incomingInterface, 1, dioHeader.GetDodagId (), dioHeader.GetRplInstanceId (), dioHeader.GetDtsn (), 0xFF, false);

    // add sender as parent if rank is lower then the nodes rank
    if (m_ocp.DagRank (dioHeader.GetRank ()) < m_ocp.DagRank (m_rank))
    {
      RplNode newNode = {dioHeader.GetRank (), ipv6Header.GetSource (), incomingInterface, dioHeader.GetDtsn ()};
      bool nodeDtsnChanged = false;
      bool addedParentNode = false;
      bool foundParent = false;
      for (auto &dodagParent : m_dodagParents)
      {
        // if node is already in the dodag parent set - update it
        if (dodagParent.address == newNode.address && dodagParent.interface == newNode.interface)
        {
          foundParent = true;
          if (dodagParent.dtsn != dioHeader.GetDtsn ())
          {
            nodeDtsnChanged = true;
            //m_dodagParents.erase (foundNode);
            dodagParent.dtsn = dioHeader.GetDtsn ();
          }
          if (dodagParent.rank != dioHeader.GetRank ())
          {
            dodagParent.rank = dioHeader.GetRank ();
          }
          break;
        }
      }
     // m_dodagParents.find (newNode);

      if (!foundParent) 
      {
        addedParentNode = true;
        m_dodagParents.insert (newNode);
      }

      UpdatePreferredParent ();

      // checks if dtsn is updated to include self route information for DAO messages
      for (RplNode daoParent : m_daoParents)
      {
        if (daoParent.address == ipv6Header.GetSource () && daoParent.interface == incomingInterface)
        {
          if (nodeDtsnChanged || addedParentNode)
          {
            m_dtsnChanged = true;
            // for non-storing mode trigger self dtsn update
            if (!m_isStoring)
            {
              m_dtsn++;
            }
          }
        }
      }
      return;
    }

    
    if (m_ocp.DagRank (dioHeader.GetRank ()) >= m_ocp.DagRank (m_rank))
    {
      NS_LOG_LOGIC ("Dropping DIO message, higher or equal rank advertised");
      return;
    }
  }



  /*if (dioHeader.GetVersionNumber () > m_dodagVersionNumber)
  {
    m_dodagVersionNumber = dioHeader.GetVersionNumber ();
  }*/

  NS_ABORT_MSG ("should never reach here");

}

void RoutingProtocol::ReceiveDao (Ptr<Packet> packet, Ipv6Header ipv6Header, uint32_t incomingInterface)
{
  NS_LOG_FUNCTION (this << +packet << ipv6Header);
  if (m_mop == MOP_NO_DOWNWARD_ROUTES)
  {
    NS_LOG_LOGIC ("Received DAO, but no downward route are supported - dropping DAO");
    return;
  }else if (m_dodagId.IsAny())
  {
    NS_LOG_LOGIC ("Received DAO, but detached from DODAG- dropping DAO");
    return;
  }
  NS_LOG_LOGIC ("Received DAO from " << ipv6Header.GetSource ());

  if (!m_isRoot)
  {
    for (RplNode daoParent : m_daoParents)
    {
      if (daoParent.address == ipv6Header.GetSource () && daoParent.interface == incomingInterface)
      {
        NS_ABORT_MSG ("loop detected - received DAO from DAO parent");
      }
    }
  }
  DaoHeader daoHeader;
  uint16_t payloadLength;

  packet->RemoveHeader (daoHeader);
  payloadLength = ipv6Header.GetPayloadLength () - 4 - daoHeader.GetSerializedSize ();

  // if ACK is expected, send a DAO ACK back
  if (daoHeader.GetK ())
  {
    NS_LOG_LOGIC ("RPL: Send DAO ACK back to DAO sender to " << ipv6Header.GetSource ());
    Ptr<Packet> daoAckpacket = Create<Packet> ();
    uint8_t d = (m_instanceId & 0b10000000) >> 7; // set the d flag when the local RplInstanceID is used
    uint8_t reserved = 0;
    uint8_t status = 0;
    DaoAckHeader daoAckHeader (m_instanceId, d, reserved, daoHeader.GetDaoSequence (), status, m_dodagId);
    RplIcmpv6Header rplIcmpv6Header (TYPE_DAO_ACK);

    daoAckpacket->AddHeader (daoAckHeader);
    daoAckpacket->AddHeader (rplIcmpv6Header);

    SendOnAllInterfaces (daoAckpacket, Inet6SocketAddress (ipv6Header.GetSource ()));
  }

  std::list<RplHeaderOption::RplTarget> currentRplTargets;
  std::list<RplHeaderOption::TransitInformation> currentTransitInformations;

  NS_ABORT_MSG_IF (payloadLength == 0, "Received DAO with no options from " << ipv6Header.GetSource ());

  while (payloadLength > 0)
  {
    RplHeaderOption option;
    packet->RemoveHeader (option);
    payloadLength -= option.GetSerializedSize ();
    switch (option.GetType ())
    {
    case OPTION_TYPE_PAD1:
      break;
    case OPTION_TYPE_PADN:
      break;
    case OPTION_TYPE_RPL_TARGET:
      if (!currentTransitInformations.empty ())
      {
        // update routing table

        m_routingTable.AddDownwardRoutes (currentRplTargets, currentTransitInformations, ipv6Header.GetSource (), incomingInterface, 1, daoHeader.GetDaoSequence (), m_dodagId, m_instanceId, m_isStoring);
        currentRplTargets.clear ();
        currentTransitInformations.clear ();
      }
      currentRplTargets.push_back (option.GetRplTarget ());
      // save option temporarily to send upward
      m_childDaoOptions.push_back (option);
      break;
    case OPTION_TYPE_TRANSIT_INFORMATION:
      currentTransitInformations.push_back (option.GetTransitInformation ());
      // save option temporarily to send upward
      m_childDaoOptions.push_back (option);
      break;
    case OPTION_TYPE_RPL_TARGET_DESCRIPTOR:
      NS_ABORT_MSG ("TODO add what to do when receiving DAO with RPL_TARGET_DESCRIPTOR option");
      break;
    default:
      NS_ABORT_MSG ("Receiving option with the invalid type " << +option.GetType ());
      break;
    }
  }
  // update routing table
  m_routingTable.AddDownwardRoutes (currentRplTargets, currentTransitInformations, ipv6Header.GetSource (), incomingInterface, 1, daoHeader.GetDaoSequence (), m_dodagId, m_instanceId, m_isStoring);


  // pass received DAO upward to parents
  if (!m_isRoot && m_preferredParent.rank != INFINITE_RANK)
  {
    if (m_sendDaoEvent.GetUid () == m_sendDaoEvent.INVALID || m_sendDaoEvent.IsExpired ())
    {
      NS_LOG_DEBUG ("Scheduling new DAO");
      m_sendDaoEvent = Simulator::Schedule (m_delayDao, &RoutingProtocol::SendDao, this, false);
    }
  }

}

void RoutingProtocol::ReceiveDaoAck (Ptr<Packet> packet, Ipv6Header ipv6Header)
{
  NS_LOG_FUNCTION (this << +packet << ipv6Header);
  NS_LOG_LOGIC ("Received DAO-ACK from " << ipv6Header.GetSource ());
  DaoAckHeader daoAckHeader;
  uint16_t payloadLength;

  packet->RemoveHeader (daoAckHeader);
  payloadLength = ipv6Header.GetPayloadLength () - 4 - daoAckHeader.GetSerializedSize ();

  NS_ABORT_MSG_IF (payloadLength > 0, "Receiving DAO-ACK message carrying options");

  // delete sent dao as the ack is received
  for(auto iter = m_sentDaos.begin(); iter != m_sentDaos.end();)
  {
    if (daoAckHeader.GetDaoSequence () == iter->daoSequence && ipv6Header.GetSource () == iter->daoParent.address)
    {
      iter->event.Cancel ();
      iter = m_sentDaos.erase(iter);
    }
    else
    {
      ++iter;
    }
  }
}

void RoutingProtocol::UpdatePreferredParent ()
{
  NS_LOG_FUNCTION (this);
  if (m_dodagParents.empty ())
  {
    DetachFromDodag ();
    return;
  }
  
  RplNode newPreferredParent = m_ocp.GetPreferredParent (m_dodagParents, m_preferredParent);

  if (newPreferredParent.rank == INFINITE_RANK) 
  {
    NS_ABORT_MSG ("Objective function couldn't select preferred parent");
  }

  if (newPreferredParent.address != m_preferredParent.address || m_preferredParent.interface != newPreferredParent.interface)
  {
    NS_LOG_LOGIC ("Updating new preferred parent");
    m_updatedPrefParentTrace (newPreferredParent);
    if (m_preferredParent.rank != INFINITE_RANK)
    {
      ClearPreferredParentRoutes ();
    }
    for(auto iter = m_sentDaos.begin(); iter != m_sentDaos.end();)
    {
      iter->event.Cancel ();
      iter = m_sentDaos.erase(iter);
    }
    m_preferredParent = newPreferredParent;
    m_routingTable.AddRoute (newPreferredParent.address, newPreferredParent.interface, 1, m_dodagId, m_instanceId, m_dtsn, 0xFF, false);
    if (newPreferredParent.address != m_dodagId)
    {
      m_routingTable.AddRoute (m_dodagId, newPreferredParent.address, newPreferredParent.interface, 1, m_dodagId, m_instanceId, m_dtsn, 0xFF, false);
    }
    // trickle timer inconsistency due to parent change
    m_trickleTimer.InconsistentEvent ();

    if (m_mop != MOP_NO_DOWNWARD_ROUTES)
    {
      m_dtsnChanged = true;
      if (m_sendDaoEvent.GetUid () == m_sendDaoEvent.INVALID || m_sendDaoEvent.IsExpired ())
      {
        m_sendDaoEvent = Simulator::Schedule (m_delayDao, &RoutingProtocol::SendDao, this, false);
      }
    }else{
      NS_LOG_LOGIC ("DAO disabled by mode of operation");
    }
  }
  else{
    //update DTSN of preferred parent
    m_preferredParent.dtsn = newPreferredParent.dtsn;
  }

  // when only the rank of the prefered parent changed, update it
  if (newPreferredParent.rank != m_preferredParent.rank)
  {
    m_updatedPrefParentTrace (newPreferredParent);
    m_preferredParent = newPreferredParent;
  }

  //calculate new node rank from new parent
  uint16_t newRank = m_ocp.CalculateRank (newPreferredParent.rank);
  if (newRank != m_rank)
  {
    m_rank = newRank;
    NS_LOG_DEBUG ("Setting Rank to " << +m_ocp.DagRank (m_rank));
    RemoveObsoleteParents ();
  }

  UpdateDaoParents ();
}

void RoutingProtocol::UpdateDaoParents ()
{
  std::set<RplNode> daoParents;
  daoParents.insert (m_preferredParent);
  // if more than 1 DAO parent shall be used, try to get additional ones
  if (m_maxDaoParents > 1)
  {
    int daoParentCounter = 1;
    for (RplNode parentNode : m_dodagParents)
    {
      // do not add preferred parent a second time
      if (parentNode == m_preferredParent)
      {
        continue;
      }
      daoParents.insert (parentNode);
      daoParentCounter++;
      if (daoParentCounter >= m_maxDaoParents)
      {
        break;
      }
    }
  }
  m_daoParents = daoParents;
}

void RoutingProtocol::RemoveObsoleteParents ()
{
  NS_LOG_FUNCTION (this);
  for(auto iter = m_dodagParents.begin(); iter != m_dodagParents.end();)
  {
    if(m_ocp.DagRank ((*iter).rank) >= m_ocp.DagRank (m_rank))
    {
      iter = m_dodagParents.erase(iter);
    }
    else
    {
      ++iter;
    }
  }
}

void RoutingProtocol::DeleteParent (Ipv6Address address, uint32_t interface)
{
  NS_LOG_FUNCTION (this << address << interface);
  if (m_dodagId.IsAny ()) 
  {
    NS_ABORT_MSG ("No preferred parent set");
  }
  for(auto iter = m_dodagParents.begin(); iter != m_dodagParents.end();)
  {
    if((*iter).address == address && (*iter).interface == interface)
    {
      iter = m_dodagParents.erase(iter);
    }
    else
    {
      ++iter;
    }
  }
  if (address == m_preferredParent.address && interface == m_preferredParent.interface)
  {
    DeletePreferredParent ();
  }
}


void RoutingProtocol::DeletePreferredParent ()
{
  NS_LOG_FUNCTION (this);
  if (m_preferredParent.rank == INFINITE_RANK) 
  {
    NS_ABORT_MSG ("No preferred parent set");
  }
  ClearPreferredParentRoutes();
  m_dodagParents.erase(m_preferredParent);
  m_preferredParent = {INFINITE_RANK, Ipv6Address ("::"), 0};
  UpdatePreferredParent ();
}

void RoutingProtocol::ClearPreferredParentRoutes()
{
  NS_LOG_FUNCTION (this);
  if (m_preferredParent.rank == INFINITE_RANK)
  {
    NS_ABORT_MSG ("No preferred parent set");
  }

  //NS_ABORT_MSG_IF (m_routingTable.Empty (), "Routes are empty");
  // delete routes where destination or next hop address is the preferred parent address
  m_routingTable.RemoveRoutesOfTarget (m_preferredParent.address, m_preferredParent.interface);
  
}

void RoutingProtocol::DetachFromDodag ()
{
  NS_LOG_FUNCTION (this);
  ClearDownwardRoutes ();
  for(auto iter = m_sentDaos.begin(); iter != m_sentDaos.end();)
  {
    iter->event.Cancel ();
    iter = m_sentDaos.erase(iter);
  }
  m_trickleTimer.Stop ();
  m_rank = INFINITE_RANK;
  PoisonChildren ();

  m_dodagId = Ipv6Address ("::");
  m_isGrounded = false;
  m_receivedDio = false;
  m_daoParents = std::set<RplNode> ();
  NS_LOG_LOGIC ("Detached from DODAG");
  m_detachFromDodagTrace (m_preferredParent);

  // send DIS messages to find a new root or become a new root itself
  if (m_disMop == DIS_MOP_SEND)
  {
    m_disMessageTimer.SetDelay (m_disMessageTime);
    m_disMessageTimer.Schedule ();
  }
}

void RoutingProtocol::ClearDownwardRoutes ()
{
  NS_LOG_FUNCTION (this);
  //NS_ABORT_MSG_IF (m_routingTable.Empty (), "Routes are empty");
  m_routingTable.RemoveDownwardRoutes (m_dodagId, m_instanceId);
}

void RoutingProtocol::PoisonChildren () 
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT_MSG(m_rank == INFINITE_RANK, "rank of node is not INFINITE");
  NS_LOG_LOGIC ("poisoning children");
  Ptr<Packet> packet = Create<Packet> ();
  DioHeader dioHeader (m_instanceId, m_dodagVersionNumber.GetValue (), INFINITE_RANK, m_isGrounded, m_mop, m_dodagPreference, m_dtsn, 0, 0, m_dodagId);
  RplIcmpv6Header rplIcmpv6Header (TYPE_DIO);

  packet->AddHeader (dioHeader);
  packet->AddHeader (rplIcmpv6Header);

  SendOnAllInterfaces (packet, Inet6SocketAddress (RPL_ALL_NODE));
}


void RoutingProtocol::NotifyInterfaceUp (uint32_t interface)
{
  // Initialize function may be not executed before the interface goes up

  NS_LOG_FUNCTION (this << interface);

  if (!m_initialized)
  {
    return;
  }

  RegisterSockets (interface);

}

void RoutingProtocol::NotifyInterfaceDown (uint32_t interface)
{
  NS_LOG_FUNCTION (this << interface);

  // TODO remove all routes that are assosiated with this interface

  for (SocketListI iter = m_unicastSocketList.begin (); iter != m_unicastSocketList.end (); iter++ )
  {
    NS_LOG_INFO ("Checking socket for interface " << interface);
    if (iter->second == interface)
    {
      NS_LOG_INFO ("Removed socket for interface " << interface);
      iter->first->Close ();
      m_unicastSocketList.erase (iter);
      break;
    }
  }

  // remove sent DAOs from this interface
  for(auto iter = m_sentDaos.begin(); iter != m_sentDaos.end();)
  {
    if (iter->daoParent.interface == interface)
    {
      iter->event.Cancel ();
      iter = m_sentDaos.erase(iter);
    }
    else
    {
      ++iter;
    }
  }

  // remove routing entries on this interface
  m_routingTable.RemoveRoutesOfInterface (interface);

  // remove parents on this interface
  bool deletedPreferredParent = false;
  for(auto iter = m_dodagParents.begin(); iter != m_dodagParents.end();)
  {
    if((*iter).interface == interface)
    {
      if ((*iter) == m_preferredParent)
      {
        deletedPreferredParent = true;
      }
      iter = m_dodagParents.erase(iter);
    }
    else
    {
      ++iter;
    }
  }
  if (deletedPreferredParent)
  {
    DeletePreferredParent ();
  }
  UpdateDaoParents ();

}
void RoutingProtocol::NotifyAddAddress (uint32_t interface, Ipv6InterfaceAddress address)
{
  NS_LOG_FUNCTION (this << interface << address);
}
void RoutingProtocol::NotifyRemoveAddress (uint32_t interface, Ipv6InterfaceAddress address)
{
  NS_LOG_FUNCTION (this << interface << address);
}
void RoutingProtocol::NotifyAddRoute (Ipv6Address dst, Ipv6Prefix mask, Ipv6Address nextHop, uint32_t interface, Ipv6Address prefixToUse)
{}
void RoutingProtocol::NotifyRemoveRoute (Ipv6Address dst, Ipv6Prefix mask, Ipv6Address nextHop, uint32_t interface, Ipv6Address prefixToUse)
{}
void RoutingProtocol::PrintRoutingTable (Ptr<OutputStreamWrapper> stream, Time::Unit unit) const
{
*stream->GetStream () << "Node: " << m_ipv6->GetObject<Node> ()->GetId ()
                        << "; Time: " << Now ().As (unit)
                        << ", Local time: " << m_ipv6->GetObject<Node> ()->GetLocalTime ().As (unit)
                        << ", RPL Routing table" << std::endl;

  m_routingTable.Print (*stream->GetStream ());

}

void RoutingProtocol::PrintRoutingTable (std::ostream &os) const
{
  os << "Node: " << m_ipv6->GetObject<Node> ()->GetId ()
                  << "; Time: " << Now ().As (Time::S) << "s"
                  << ", RPL Routing table" << std::endl;
  m_routingTable.Print (os);
}




void RoutingProtocol::RegisterSockets (uint32_t interface)
{
  bool sendSocketFound = false;
  for (SocketListI iter = m_unicastSocketList.begin (); iter != m_unicastSocketList.end (); iter++ )
  {
    if (iter->second == interface)
    {
      sendSocketFound = true;
      break;
    }
  }

  bool activeInterface = false;
  if (m_interfaceExclusions.find (interface) == m_interfaceExclusions.end ())
  {
    activeInterface = true;
    m_ipv6->SetForwarding (interface, true);
  }

  for (uint32_t j = 0; j < m_ipv6->GetNAddresses (interface); j++)
  {
    Ipv6InterfaceAddress address = m_ipv6->GetAddress (interface, j);

    if (address.GetScope() == Ipv6InterfaceAddress::LINKLOCAL && sendSocketFound == false && activeInterface == true)
    {
      NS_LOG_LOGIC ("RPL: adding sending socket to " << address.GetAddress () << " from interface " << +interface);
      TypeId tid = TypeId::LookupByName ("ns3::Ipv6RawSocketFactory");
      Ptr<Socket> socket = Socket::CreateSocket (GetObject<Node> (), tid);
      NS_ASSERT (socket);
      socket->SetAttribute ("Protocol", UintegerValue(Ipv6Header::IPV6_ICMPV6));
      Inet6SocketAddress local = Inet6SocketAddress (address.GetAddress ());
      socket->BindToNetDevice (m_ipv6->GetNetDevice (interface));
      socket->Bind (local);  // this is the src Address if the socket is used to send packets, as well as the Address the Socket listens to when receiving packets
      socket->SetRecvCallback (MakeCallback (&RoutingProtocol::Receive, this));
      //socket->SetIpv6RecvHopLimit (true);
      socket->SetIpv6HopLimit(244);
      socket->SetRecvPktInfo (true);
      m_unicastSocketList[socket] = interface;
    }
    //else if (address.GetScope() == Ipv6InterfaceAddress::GLOBAL)
    //{
    //  SendTriggeredRouteUpdate ();
    //}
  }

  if (!m_multicastRecvSocket)
  {
    NS_LOG_LOGIC ("RPL: adding receiving socket");
    TypeId tid = TypeId::LookupByName ("ns3::Ipv6RawSocketFactory");
    m_multicastRecvSocket = Socket::CreateSocket (GetObject<Node> (), tid);
    NS_ASSERT (m_multicastRecvSocket);
    m_multicastRecvSocket->SetAttribute ("Protocol", UintegerValue(Ipv6Header::IPV6_ICMPV6));
    Inet6SocketAddress local = Inet6SocketAddress (RPL_ALL_NODE);
    m_multicastRecvSocket->Bind (local);  // this is the src Address if the socket is used to send packets, as well as the Address the Socket listens to when receiving packets
    m_multicastRecvSocket->SetRecvCallback (MakeCallback (&RoutingProtocol::Receive, this));
    //m_multicastRecvSocket->SetIpv6RecvHopLimit (true);
    m_multicastRecvSocket->SetIpv6HopLimit(244);
    m_multicastRecvSocket->SetRecvPktInfo (true);
  }
}

void RoutingProtocol::SendOnAllInterfaces (Ptr<Packet> packet, const Address &toAddress)
{
  NS_LOG_FUNCTION (this << packet << toAddress);
  for (SocketListI iter = m_unicastSocketList.begin (); iter != m_unicastSocketList.end (); iter++ )
  {
    uint32_t interface = iter->second;

    if (interface == 0)
    {
      continue;
    }

    if (m_interfaceExclusions.find (interface) == m_interfaceExclusions.end ())
    {
      NS_LOG_INFO ("SendTo: " << (*packet));
      iter->first->SendTo (packet, 0, toAddress);
    }
  }
}

Ptr<Ipv6Route> RoutingProtocol::Lookup (Ipv6Address dst, bool setSource, Ptr<NetDevice> interface)
{
  NS_LOG_FUNCTION (this << dst << setSource << interface);
  Ptr<Ipv6Route> rtentry = 0;

  // when sending on link-local multicast, there have to be interface specified
  if (dst.IsLinkLocalMulticast ())
  {
    NS_ASSERT_MSG (interface, "Try to send on link-local multicast address, and no interface index is given!");
    rtentry = Create<Ipv6Route> ();
    rtentry->SetSource (m_ipv6->SourceAddressSelection (m_ipv6->GetInterfaceForDevice (interface), dst));
    rtentry->SetDestination (dst);
    rtentry->SetGateway (Ipv6Address::GetZero ());
    rtentry->SetOutputDevice (interface);
    return rtentry;
  }
  
  if (isMyAddress (dst, interface))
  {
    rtentry = Create<Ipv6Route> ();
    rtentry->SetSource (dst);
    rtentry->SetDestination (dst);
    rtentry->SetGateway (Ipv6Address::GetZero ());
    if (interface)
    {
      rtentry->SetOutputDevice (interface);
    }else
    {
      rtentry->SetOutputDevice (m_ipv6->GetNetDevice (m_ipv6->GetInterfaceForAddress (dst)));
    }
    return rtentry;
  }

  if (dst.IsLinkLocal ())
  {
    if (!interface)
    {
      //NS_ASSERT_MSG (interface, "Try to send on link-local unicast address " << dst << ", and no interface " << interface << " index is given!");
      NS_LOG_DEBUG ("Try to send on link-local unicast address " << dst << ", and no interface is given! -> return no route");
      return rtentry;
    }else
    {
      rtentry = Create<Ipv6Route> ();
      rtentry->SetSource (m_ipv6->SourceAddressSelection (m_ipv6->GetInterfaceForDevice (interface), dst));
      rtentry->SetDestination (dst);
      rtentry->SetGateway (Ipv6Address::GetZero ());
      rtentry->SetOutputDevice (interface);
      return rtentry;
    }
  }

  // search routes first
  if (auto route = m_routingTable.SearchEntry (m_ipv6, dst, interface)) 
  {
    return CreateRouteFromTableEntry (route.value (), setSource, dst);
  }
  
  /*for (RplRoutingTableEntry const& route : m_routingTable)
  {
    if (dst == route.GetDest () )
    {
      if (!interface || interface == m_ipv6->GetNetDevice (route.GetInterface ()))
      {
        return CreateRouteFromTableEntry (route, setSource, dst);
      }
    }
  }*/

  // if no routes are found, send to preferred parent
  if (m_isRoot)
  {
    NS_ABORT_MSG ("RoutingProtocol::Lookup (...): TODO implement where root shall send packets that are not in the DODAG!");
    return rtentry;
  }
  else
  {
    if (m_preferredParent.rank != INFINITE_RANK)
    {
      RplRoutingTableEntry defaultRoute = RplRoutingTableEntry (dst, m_preferredParent.address, m_preferredParent.interface);
      return CreateRouteFromTableEntry (defaultRoute, setSource, dst);
    }
    else
    {
      NS_LOG_LOGIC ("No preferred parent set - returning empty route");
      return rtentry;
    }
  }
  
}

Ptr<Ipv6Route> RoutingProtocol::CreateRouteFromTableEntry (RplRoutingTableEntry const route, bool setSource, Ipv6Address dst)
{
  uint32_t interfaceIdx = route.GetInterface ();
  Ptr<Ipv6Route> rtentry = Create<Ipv6Route> ();

  if (setSource)
  {
    if (route.GetGateway ().IsAny ())
    {
      rtentry->SetSource (m_ipv6->SourceAddressSelection (interfaceIdx, route.GetDest ()));
    }
    else if (route.GetDest ().IsAny ()) /* default route */
    {
      rtentry->SetSource (m_ipv6->SourceAddressSelection (interfaceIdx, route.GetPrefixToUse ().IsAny () ? dst : route.GetPrefixToUse ()));
    }
    else
    {
      rtentry->SetSource (m_ipv6->SourceAddressSelection (interfaceIdx, route.GetDest ()));
    }
  }
  rtentry->SetDestination (route.GetDest ());
  rtentry->SetGateway (route.GetGateway ());
  rtentry->SetOutputDevice (m_ipv6->GetNetDevice (interfaceIdx));
  return rtentry;
}


void RoutingProtocol::ExpireTrickleTimer (void)
{
  NS_LOG_FUNCTION (this);
  // send DIO
  NS_LOG_LOGIC ("RPL: Send DIO Broadcast due to Trickle Timer expiration");
  Ptr<Packet> packet = Create<Packet> ();
  DioHeader dioHeader (m_instanceId, m_dodagVersionNumber.GetValue (), m_rank, m_isGrounded, m_mop, m_dodagPreference, m_dtsn++, 0, 0, m_dodagId);
  RplIcmpv6Header rplIcmpv6Header (TYPE_DIO);

  packet->AddHeader (dioHeader);
  packet->AddHeader (rplIcmpv6Header);

  SendOnAllInterfaces (packet, Inet6SocketAddress (RPL_ALL_NODE));

  // TODO remove trickle timer stop
  //m_trickleTimer.Stop ();
}

void RoutingProtocol::DisExpireTimer (void)
{
  NS_LOG_FUNCTION (this);
  if (!m_receivedDio)
  {
    if (m_disMessageCounter < m_numberOfDisMessages)
    {
      m_disMessageCounter++;

      // send DIS
      NS_LOG_LOGIC ("RPL: Send DIS Broadcast due to DIS Timer expiration");
      Ptr<Packet> packet = Create<Packet> ();
      DisHeader disHeader;
      RplIcmpv6Header rplIcmpv6Header (TYPE_DIS);
      /*RplHeaderOption option;
      option.SetSolicitedInformation (m_instanceId,0,0,0,m_dodagId, m_dodagVersionNumber);
      packet->AddHeader (option);*/
      packet->AddHeader (disHeader);
      packet->AddHeader (rplIcmpv6Header);

      SendOnAllInterfaces (packet, Inet6SocketAddress (RPL_ALL_NODE));

      m_disMessageTimer.Schedule ();

    }else{
      InitRoot ();
    }
  }
}

void RoutingProtocol::SendDao (bool isNoPath)
{
  NS_LOG_FUNCTION (this);

  if (m_mop == MOP_NO_DOWNWARD_ROUTES)
  {
    NS_LOG_LOGIC ("Downward routes disables by mode of operation - DAO will not be sent");
    return;
  }
  
  // send DAO
  Ptr<Packet> packet = Create<Packet> ();
  uint8_t d = (m_instanceId & 0b10000000) >> 7; // set the d flag when the local RplInstanceID is used

  if (m_isStoring)
  {
    // Add all global IP addresses As RPL Target when triggered from parents DIO (DTSN)
    if (m_dtsnChanged)
    {
      m_dtsnChanged = false;  // reset dtsn change
      // in storing mode add transitinformation to rpl self target(s)
      RplHeaderOption transitInformation;
      bool hasSelfTarget = false;
      // TODO RPL currently does not support learning nodes through different protocols
      uint8_t e = 0;
      // TODO add logic for path control field
      uint8_t pathControl = 0;
      uint8_t pathLifetime;
      if (isNoPath)
      {
        pathLifetime = 0;
      }else{
        pathLifetime = m_defaultLifetime;
      }
      m_pathSequence++;
      transitInformation.SetTransitInformation (e, pathControl, m_pathSequence.GetValue (), pathLifetime);

      packet->AddHeader (transitInformation);
      
      for (uint32_t interface = 0; interface < m_ipv6->GetNInterfaces (); interface++)
      {
        if (m_interfaceExclusions.find (interface) == m_interfaceExclusions.end ())
        {
          for (uint32_t addressIndex = 0; addressIndex < m_ipv6->GetNAddresses (interface); addressIndex++)
          {
            Ipv6InterfaceAddress address = m_ipv6->GetAddress (interface, addressIndex);

            if (address.GetScope() == Ipv6InterfaceAddress::GLOBAL)
            {
              RplHeaderOption selfTarget;
              hasSelfTarget = true;
              selfTarget.SetRplTarget (address.GetAddress ());
              packet->AddHeader (selfTarget);
            }
          }
        }
      }
      NS_ABORT_MSG_IF (!hasSelfTarget, "Node " << m_ipv6->GetObject<Node> ()->GetId () << "has no included interface with a global address - aborting");
    }
    // add rpl targets and transitinformations from child nodes in reverse order
    for (std::list<RplHeaderOption>::reverse_iterator iter=m_childDaoOptions.rbegin(); iter!=m_childDaoOptions.rend(); ++iter)
    {
      packet->AddHeader (*iter);
    }
  }
  else
  {
    NS_ABORT_MSG ("TODO Add what to do in Non-Storing mode");
  }


  m_daoSequence++;
  DaoHeader daoHeader (m_instanceId, m_k, d, 0, 0, m_daoSequence.GetValue (), m_dodagId);
  RplIcmpv6Header rplIcmpv6Header (TYPE_DAO);

  packet->AddHeader (daoHeader);
  packet->AddHeader (rplIcmpv6Header);

  // Send to all dao parents (currently only the preferred parent)
  for (RplNode daoParent : m_daoParents)
  {
    Ptr<Packet> daoPacket = packet->Copy ();
    NS_LOG_LOGIC ("RPL: Send DAO to DAO parent " << daoParent.address);
    SendOnAllInterfaces (packet, Inet6SocketAddress (daoParent.address));

    if (m_k && !isNoPath)
    {
      EventId event = Simulator::Schedule (m_daoAckTimeout, &RoutingProtocol::ResendDao, this, m_daoSequence.GetValue (), daoParent);
      SentDao newDao = {m_daoSequence.GetValue (), daoParent, daoPacket, 1, event};
      m_sentDaos.push_back (newDao);
      //m_daoAckTimer.Schedule ();
    }
  }


  m_childDaoOptions.clear ();

}

void RoutingProtocol::ResendDao (uint8_t daoSequence, RplNode daoParent)
{
  if (m_preferredParent.rank == INFINITE_RANK)
  {
    NS_LOG_LOGIC ("Dao retransmission failed - no preferred parent set");
    return;
  }
  for(auto iter = m_sentDaos.begin(); iter != m_sentDaos.end();)
  {
    if (daoSequence == iter->daoSequence && daoParent == iter->daoParent)
    {
      if (iter->daoMessageCounter <= m_numberOfDaoRetries)
      {
        NS_LOG_LOGIC ("Resending Dao");
        SendOnAllInterfaces (iter->daoPacket->Copy (), Inet6SocketAddress (iter->daoParent.address));
        EventId event = Simulator::Schedule (m_daoAckTimeout, &RoutingProtocol::ResendDao, this, daoSequence, daoParent);
        iter->event = event;
        iter->daoMessageCounter++;
        ++iter;
      }
      else
      {
        // RFC 18.2.6.
        NS_LOG_LOGIC ("Send No-Path Dao to parent, as he is not reachable");
        m_dtsnChanged = true; // to send self target in no-path
        SendDao (true);
        //m_sendDaoNoPathEvent = Simulator::Schedule (m_delayDao, &RoutingProtocol::SendDao, this, m_daoSequence++, true);
        DeleteParent (iter->daoParent.address, iter->daoParent.interface);
        if (m_sentDaos.empty ())
        {
          break;
        }
        iter = m_sentDaos.erase(iter);
      }
    }
    else
    {
      ++iter;
    }
  }

}



}
}

