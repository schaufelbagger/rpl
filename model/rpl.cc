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
  NS_LOG_DEBUG(this);
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

  if (m_isRoot)
  {
    InitRoot ();
  }

  std::cout<<"Is Root: "<< +m_isRoot <<std::endl;
  std::cout<<"MOP: "<< +m_mop <<std::endl;
  std::cout<<"Instance ID: "<< +m_instanceId <<std::endl;

  Ipv6RoutingProtocol::DoInitialize ();
}

void RoutingProtocol::InitRoot ()
{
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
    m_dodagId = m_ipv6->GetAddress (1,1).GetAddress ();  // TODO change this to add correct dodagId
  }
  m_isGrounded = true;
  // [RFC 6550, 8.3.1]
  m_trickleTimer.Enable ();
}

TypeId RoutingProtocol::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::rpl::RoutingProtocol")
  .SetParent<Ipv6RoutingProtocol> ()
  .SetGroupName ("Rpl")
  .AddConstructor<RoutingProtocol> ()
  //.AddAttribute() // TODO
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
  return true;
}

Ptr<Ipv6Route> RoutingProtocol::RouteOutput (Ptr< Packet > p,
  const Ipv6Header& header,
  Ptr< NetDevice > oif,
  Socket::SocketErrno & sockerr)
{

  //TODO 
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

  int32_t interfaceForAddress = m_ipv6->GetInterfaceForAddress (senderAddress);
  if (interfaceForAddress != -1)
  {
    NS_LOG_LOGIC ("Ignoring a packet sent by myself.");
    return;
  }

  Ipv6Header ipv6Header;
  packet->RemoveHeader (ipv6Header);
  NS_ABORT_MSG_UNLESS (ipv6Header.GetNextHeader () == Ipv6Header::IPV6_ICMPV6, "The received Packet is not an ICMPv6 packet");
  RplIcmpv6Header rplIcmpv6Header;

  packet->RemoveHeader (rplIcmpv6Header);
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
      ReceiveDao (packet, ipv6Header);
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

void RoutingProtocol::ReceiveDis (Ptr<Packet> packet, Ipv6Header ipv6Header)
{
  NS_LOG_FUNCTION (this << +packet << ipv6Header);
  NS_LOG_LOGIC ("Received DIS");
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
    NS_LOG_LOGIC ("RPL: Send DIO with DODAG Configuration to Node that sent the DIS message");
    Ptr<Packet> packet = Create<Packet> ();
    RplHeaderOption dodagConfig;
    DioHeader dioHeader (m_instanceId, m_dodagVersionNumber, m_rank, m_isGrounded, m_mop, m_dodagPreference, m_dtsn++, 0, 0, m_dodagId);
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
  NS_LOG_LOGIC ("Received DIO");

  packet->RemoveHeader (dioHeader);
  payloadLength = ipv6Header.GetPayloadLength () - 4 - dioHeader.GetSerializedSize ();

  if (!m_receivedDio)
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
            m_trickleTimer.Enable ();
          }else
          {
            NS_ABORT_MSG ("TODO add what to do when ocp is received which is not supported by this node");
          }
        }else{
          NS_ABORT_MSG ("TODO add what to do when receiving DIO with DODAG Config option and Node already joined DODAG");
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

  // no parents and no dodagConfiguration means the node hast to ask the sender for a DODAG Configuration Option
  if (m_dodagId.IsAny () && !receivedDodagConfiguration)
  {
    // send DIS
    NS_LOG_LOGIC ("RPL: Send DIS Unicast to request a DODAG Configuration Option");
    Ptr<Packet> packet = Create<Packet> ();
    DisHeader disHeader;
    RplIcmpv6Header rplIcmpv6Header (TYPE_DIS);
    packet->AddHeader (disHeader);
    packet->AddHeader (rplIcmpv6Header);

    SendOnAllInterfaces (packet, Inet6SocketAddress (ipv6Header.GetSource ()));
    return;
  }
  // check for route poisoning, then delete parent from upward routes and as preferred parent
  if (!m_dodagId.IsAny () && dioHeader.GetRank () == INFINITE_RANK && m_preferredParent.address == ipv6Header.GetSource ())
  {
    NS_LOG_LOGIC ("INFINITE_RANK is advertised from a preferred parent - delete preferred parent and remove it from candidate parents");
    DeletePreferredParent ();
    UpdatePreferredParent ();
  }

  // TODO change preferred parent to DAOparents
  // checks if dtsn is updated to include self route information for DAO messages
  if (m_preferredParent.address == ipv6Header.GetSource ())
  {
    if (dioHeader.GetDtsn () > m_preferredParent.dtsn)
    {
      m_dtsnChanged = true;
      m_preferredParent.dtsn = dioHeader.GetDtsn ();
      // for non-storing mode trigger self dtsn update
      if (!m_isStoring)
      {
        m_dtsn++;
      }
    }
  }

  // add sender as upward route if rank is lower then the nodes rank
  if (m_ocp.DagRank (dioHeader.GetRank ()) < m_ocp.DagRank (m_rank) && m_dodagId == dioHeader.GetDodagId () && m_instanceId == dioHeader.GetRplInstanceId ())
  {
    // add link local address
    AddRouteToRoutingTable (ipv6Header.GetSource (), incomingInterface, 1, dioHeader.GetDodagId (), dioHeader.GetRplInstanceId (), dioHeader.GetDtsn (), false);
    RplNode newNode = {dioHeader.GetRank (), ipv6Header.GetSource (), incomingInterface, dioHeader.GetDtsn ()};
    auto nodeIter = m_candidateParents.find (newNode);
    if (nodeIter != m_candidateParents.end () && nodeIter->dtsn != dioHeader.GetDtsn ()) 
    {
      m_candidateParents.erase (nodeIter);
    }
    m_candidateParents.insert (newNode);
    UpdatePreferredParent ();
    return;
  }


  if (m_ocp.DagRank (dioHeader.GetRank ()) >= m_ocp.DagRank (m_rank))
  {
    NS_LOG_LOGIC ("Dropping DIO message, higher or equal rank advertised");
    return;
  }

  /*if (dioHeader.GetVersionNumber () > m_dodagVersionNumber)
  {
    m_dodagVersionNumber = dioHeader.GetVersionNumber ();
  }*/

  NS_ABORT_MSG ("should never reach here");

}

void RoutingProtocol::ReceiveDao (Ptr<Packet> packet, Ipv6Header ipv6Header)
{
  NS_LOG_FUNCTION (this << +packet << ipv6Header);
  NS_LOG_LOGIC ("Received DAO");
  DaoHeader daoHeader;
  uint16_t payloadLength;

  packet->RemoveHeader (daoHeader);
  payloadLength = ipv6Header.GetPayloadLength () - 4 - daoHeader.GetSerializedSize ();

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
      NS_ABORT_MSG ("TODO add what to do when receiving DAO with RPL_TARGET option");
      break;
    case OPTION_TYPE_TRANSIT_INFORMATION:
      NS_ABORT_MSG ("TODO add what to do when receiving DAO with TRANSIT_INFORMATION option");
      break;
    case OPTION_TYPE_RPL_TARGET_DESCRIPTOR:
      NS_ABORT_MSG ("TODO add what to do when receiving DAO with RPL_TARGET_DESCRIPTOR option");
      break;
    default:
      NS_ABORT_MSG ("Receiving option with the invalid type " << +option.GetType ());
      break;
    }
  }


  NS_ABORT_MSG ("TODO add what to do when receiving DAO message!");
}

void RoutingProtocol::ReceiveDaoAck (Ptr<Packet> packet, Ipv6Header ipv6Header)
{
  NS_LOG_FUNCTION (this << +packet << ipv6Header);
  NS_LOG_LOGIC ("Received DAO-ACK");
  DaoAckHeader daoAckHeader;
  uint16_t payloadLength;

  packet->RemoveHeader (daoAckHeader);
  payloadLength = ipv6Header.GetPayloadLength () - 4 - daoAckHeader.GetSerializedSize ();

  NS_ABORT_MSG_IF (payloadLength > 0, "Receiving DAO-ACK message carrying options");

  // delete sent dao as the ack is received
  for(auto iter = m_sentDaos.begin(); iter != m_sentDaos.end();)
  {
    if (daoAckHeader.GetDaoSequence () == iter->daoSequence)
    {
      iter->event.Cancel ();
      iter = m_sentDaos.erase(iter);
    }
    else
    {
      ++iter;
    }
  }




  NS_ABORT_MSG ("TODO add what to do when receiving DAO-ACK message!");
}

void RoutingProtocol::UpdatePreferredParent ()
{
  NS_LOG_FUNCTION (this);
  if (m_candidateParents.empty ())
  {
    DetachFromDodag ();
  }
  

  RplNode newPreferredParent = m_ocp.GetPreferredParent (m_candidateParents);

  if (newPreferredParent.rank == INFINITE_RANK) 
  {
    NS_ABORT_MSG ("Objective function couldn't select preferred parent");
  }

  if (newPreferredParent != m_preferredParent)
  {
    NS_LOG_LOGIC ("Updating new preferred parent");
    ClearPreferredParentRoutes ();
    m_sentDaos.clear();
    m_preferredParent = newPreferredParent;
    m_preferredParentRoute = RplRoutingTableEntry (newPreferredParent.address, newPreferredParent.interface);
    AddRouteToRoutingTable (newPreferredParent.address, newPreferredParent.interface, 1, m_dodagId, m_instanceId, m_dtsn, false);
    if (newPreferredParent.address != m_dodagId)
    {
      AddRouteToRoutingTable (m_dodagId, newPreferredParent.address, newPreferredParent.interface, 1, m_dodagId, m_instanceId, m_dtsn, false);
    }
    // trickle timer inconsistency due to parent change
    m_trickleTimer.InconsistentEvent ();

    if (m_mop != MOP_NO_DOWNWARD_ROUTES)
    {
      if (m_sendDaoEvent.GetUid () == m_sendDaoEvent.INVALID || m_sendDaoEvent.IsExpired ())
      {
        m_sendDaoEvent = Simulator::Schedule (m_delayDao, &RoutingProtocol::SendDao, this, m_daoSequence++, false);
      }
    }else{
      NS_LOG_LOGIC ("DAO disabled by mode of operation");
    }
  }

  //calculate new node rank from new parent
  uint16_t newRank = m_ocp.CalculateRank (newPreferredParent.rank);
  if (newRank != m_rank)
  {
    m_rank = newRank;
    RemoveObsoleteParents ();
  }
}

void RoutingProtocol::RemoveObsoleteParents ()
{
  NS_LOG_FUNCTION (this);
  for(auto iter = m_candidateParents.begin(); iter != m_candidateParents.end();)
  {
    if(m_ocp.DagRank ((*iter).rank) > m_ocp.DagRank (m_rank))
    {
      iter = m_candidateParents.erase(iter);
    }
    else
    {
      ++iter;
    }
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
  m_candidateParents.erase(m_preferredParent);
  m_preferredParent = {INFINITE_RANK, Ipv6Address ("::"), 0};
}

void RoutingProtocol::ClearPreferredParentRoutes()
{
  NS_LOG_FUNCTION (this);
  if (m_preferredParent.rank == INFINITE_RANK)
  {
    NS_LOG_INFO ("No preferred parent set");
  }

  NS_ABORT_MSG_IF (m_routingTable.empty (), "Routes are empty");
  m_preferredParentRoute = RplRoutingTableEntry ();
  // delete routes where destination or next hop address is the preferred parent address
  for(auto iter = m_routingTable.begin(); iter != m_routingTable.end();)
  {
    RplRoutingTableEntry currentRoute = *iter;
    if(currentRoute.GetDest () == m_preferredParent.address && currentRoute.GetInterface () == m_preferredParent.interface)
    {
      iter = m_routingTable.erase(iter);
    }
    else if (currentRoute.GetGateway () == m_preferredParent.address && currentRoute.GetInterface () == m_preferredParent.interface)
    {
      iter = m_routingTable.erase(iter);
    }
    else
    {
      ++iter;
    }
  }
}

void RoutingProtocol::DetachFromDodag ()
{
  NS_LOG_FUNCTION (this);
  ClearDownwardRoutes ();
  m_sentDaos.clear ();
  m_trickleTimer.Stop ();
  m_rank = INFINITE_RANK;
  PoisonChildren ();

  m_dodagId = Ipv6Address ("::");
  m_isGrounded = false;
  NS_LOG_LOGIC ("Detached from DODAG");
  //NS_ABORT_MSG ( "No parent in set while trying to find the preferred parent");
}

void RoutingProtocol::ClearDownwardRoutes ()
{
  NS_LOG_FUNCTION (this);
  NS_ABORT_MSG_IF (m_routingTable.empty (), "Routes are empty");
  for(auto iter = m_routingTable.begin(); iter != m_routingTable.end();)
  {
    RplRoutingTableEntry currentRoute = *iter;
    // remove routes where destination is global and next hop is link-local, as well the route belongs to the Sub-DODAG
    if(!currentRoute.GetDest ().IsLinkLocal () && currentRoute.GetGateway ().IsLinkLocal () && currentRoute.GetDownward () && currentRoute.GetDodagId () == m_dodagId && currentRoute.GetInstanceId () == m_instanceId)
    {
      iter = m_routingTable.erase(iter);
    }
    else
    {
      ++iter;
    }
  }
}

void RoutingProtocol::PoisonChildren () 
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT_MSG(m_rank == INFINITE_RANK, "rank of node is not INFINITE");
  NS_LOG_LOGIC ("poisoning children");
  Ptr<Packet> packet = Create<Packet> ();
  DioHeader dioHeader (m_instanceId, m_dodagVersionNumber, m_rank, m_isGrounded, m_mop, m_dodagPreference, m_dtsn, 0, 0, m_dodagId);
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
}
void RoutingProtocol::NotifyAddAddress (uint32_t interface, Ipv6InterfaceAddress address)
{
  //AddRouteToRoutingTable (address.GetAddress (), interface);
}
void RoutingProtocol::NotifyRemoveAddress (uint32_t interface, Ipv6InterfaceAddress address)
{}
void RoutingProtocol::NotifyAddRoute (Ipv6Address dst, Ipv6Prefix mask, Ipv6Address nextHop, uint32_t interface, Ipv6Address prefixToUse)
{}
void RoutingProtocol::NotifyRemoveRoute (Ipv6Address dst, Ipv6Prefix mask, Ipv6Address nextHop, uint32_t interface, Ipv6Address prefixToUse)
{}
void RoutingProtocol::PrintRoutingTable (Ptr<OutputStreamWrapper> stream, Time::Unit unit) const
{}

void RoutingProtocol::AddRouteToRoutingTable (Ipv6Address dest, Ipv6Address nextHop, uint32_t interface, uint16_t metric, Ipv6Address dodagId, uint8_t instanceId, uint8_t dtsn, bool downward)
{
  NS_LOG_FUNCTION (this << dest << nextHop << interface);
  /*if (nextHop.IsLinkLocal())
  {
    NS_LOG_WARN ("RPL::AddNetworkRoute - Next hop should be link-local");
  }*/
  RplRoutingTableEntry route = RplRoutingTableEntry (dest, nextHop, interface);
  route.SetMetric (metric);
  route.SetDodagId (dodagId);
  route.SetInstanceId (instanceId);
  route.SetDtsn (dtsn);
  route.SetDownward (downward);
  m_routingTable.push_back (route);
}

void RoutingProtocol::AddRouteToRoutingTable (Ipv6Address dest, uint32_t interface, uint16_t metric, Ipv6Address dodagId, uint8_t instanceId, uint8_t dtsn, bool downward)
{
  NS_LOG_FUNCTION (this << dest << interface);
  RplRoutingTableEntry route = RplRoutingTableEntry (dest, interface);
  route.SetMetric (metric);
  route.SetDodagId (dodagId);
  route.SetInstanceId (instanceId);
  route.SetDtsn (dtsn);
  route.SetDownward (downward);
  m_routingTable.push_back (route);
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
      NS_LOG_LOGIC ("RPL: adding sending socket to " << address.GetAddress ());
      TypeId tid = TypeId::LookupByName ("ns3::Ipv6RawSocketFactory");
      Ptr<Socket> socket = Socket::CreateSocket (GetObject<Node> (), tid);
      NS_ASSERT (socket);
      socket->SetAttribute ("Protocol", UintegerValue(Ipv6Header::IPV6_ICMPV6));
      Inet6SocketAddress local = Inet6SocketAddress (address.GetAddress ());
      socket->BindToNetDevice (m_ipv6->GetNetDevice (interface));
      socket->Bind (local);  // this is the src Address if the socket is used to send packets, as well as the Address the Socket listens to when receiving packets
      socket->SetRecvCallback (MakeCallback (&RoutingProtocol::Receive, this));
      //socket->SetIpv6RecvHopLimit (true);
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
    m_multicastRecvSocket->SetRecvPktInfo (true);
  }
}

void RoutingProtocol::SendOnAllInterfaces (Ptr<Packet> packet, const Address &toAddress)
{
  for (SocketListI iter = m_unicastSocketList.begin (); iter != m_unicastSocketList.end (); iter++ )
  {
    uint32_t interface = iter->second;

    if (m_interfaceExclusions.find (interface) == m_interfaceExclusions.end ())
    {
      NS_LOG_DEBUG ("SendTo: " << (*packet));
      iter->first->SendTo (packet, 0, toAddress);
    }
  }
}

Ptr<Ipv6Route> RoutingProtocol::Lookup (Ipv6Address dst, bool setSource, Ptr<NetDevice> interface)
{
  Ptr<Ipv6Route> rtentry = Create<Ipv6Route> ();
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
  if (dst.IsLinkLocal ())
  {
    NS_ASSERT_MSG (interface, "Try to send on link-local unicast address, and no interface index is given!");
    rtentry = Create<Ipv6Route> ();
    rtentry->SetSource (m_ipv6->SourceAddressSelection (m_ipv6->GetInterfaceForDevice (interface), dst));
    rtentry->SetDestination (dst);
    rtentry->SetGateway (Ipv6Address::GetZero ());
    rtentry->SetOutputDevice (interface);
    return rtentry;
  }

  // search routes first
  for (RplRoutingTableEntry const& route : m_routingTable)
  {
    if (dst == route.GetDest () )
    {
      if (!interface || interface == m_ipv6->GetNetDevice (route.GetInterface ()))
      {
        return CreateRouteFromTableEntry (route, setSource, dst);
      }
    }
  }

  // if no routes are found, send to preferred parent
  if (m_isRoot)
  {
    NS_ABORT_MSG ("RoutingProtocol::Lookup (...): TODO implement where root shall send packets that are not in the DODAG!");
  }else{
    return CreateRouteFromTableEntry (m_preferredParentRoute, setSource, dst);
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
  DioHeader dioHeader (m_instanceId, m_dodagVersionNumber, m_rank, m_isGrounded, m_mop, m_dodagPreference, m_dtsn++, 0, 0, m_dodagId);
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

void RoutingProtocol::SendDao (uint8_t daoSequence, bool isNoPath)
{
  NS_LOG_FUNCTION (this);

  if (m_mop == MOP_NO_DOWNWARD_ROUTES)
  {
    NS_LOG_LOGIC ("Downward routes disables by mode of operation - DAO will not be sent");
    return;
  }
  
  // send DAO
  NS_LOG_LOGIC ("RPL: Send DAO Broadcast due to DAO Timer expiration");
  Ptr<Packet> packet = Create<Packet> ();
  uint8_t d = (m_instanceId & 0b10000000) >> 7; // set the d flag when the local RplInstanceID is used

  if (m_isStoring)
  {
    // in storing mode add multiple RPL Target Options and a singe Transit Information without parent address, since it is always this node
    RplHeaderOption transitInformation;
    
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
    transitInformation.SetTransitInformation (e, pathControl, m_pathSequence++, pathLifetime);

    packet->AddHeader (transitInformation);

    // Add all global IP addresses As RPL Target when triggered from parents DIO (DTSN)
    if (m_dtsnChanged)
    {
      m_dtsnChanged = false;  // reset dtsn change
      for (uint32_t interface = 0; interface < m_ipv6->GetNInterfaces (); interface++)
      {
        if (m_interfaceExclusions.find (interface) == m_interfaceExclusions.end ())
        {
          for (uint32_t addressIndex = 0; addressIndex < m_ipv6->GetNAddresses (interface); addressIndex++)
          {
            Ipv6InterfaceAddress address = m_ipv6->GetAddress (interface, addressIndex);

            if (address.GetScope() == Ipv6InterfaceAddress::GLOBAL)
            {
              uint8_t rawPrefix[16];
              address.GetAddress ().GetBytes (rawPrefix);
              Ipv6Prefix selfAddress = Ipv6Prefix (rawPrefix);
              RplHeaderOption selfTarget;
              selfTarget.SetRplTarget (128, selfAddress);
              packet->AddHeader (selfTarget);

            }
          }
        }
      }
    }
  }else
  {
    NS_ABORT_MSG ("TODO Add what to do in Non-Storing mode");
  }


  DaoHeader daoHeader (m_instanceId, m_k, d, 0, 0, daoSequence, m_dodagId);
  RplIcmpv6Header rplIcmpv6Header (TYPE_DAO);

  packet->AddHeader (daoHeader);
  packet->AddHeader (rplIcmpv6Header);

  SendOnAllInterfaces (packet, Inet6SocketAddress (m_preferredParent.address));

  if (m_k && !isNoPath)
  {
    EventId event = Simulator::Schedule (m_daoAckTimeout, &RoutingProtocol::ResendDao, this, daoSequence);
    SentDao newDao = {daoSequence, packet, 1, event};
    m_sentDaos.push_back (newDao);
    //m_daoAckTimer.Schedule ();
  }
  m_childRplTargets.clear ();

}

void RoutingProtocol::ResendDao (uint8_t daoSequence)
{
  if (m_preferredParent.rank == INFINITE_RANK)
  {
    NS_LOG_LOGIC ("Dao retransmission failed - no preferred parent set");
    return;
  }
  for(auto iter = m_sentDaos.begin(); iter != m_sentDaos.end();)
  {
    if (daoSequence == iter->daoSequence)
    {
      if (iter->daoMessageCounter <= m_numberOfDaoRetries)
      {
        NS_LOG_LOGIC ("Resending Dao");
        SendOnAllInterfaces (iter->daoPacket, Inet6SocketAddress (m_preferredParent.address));
        EventId event = Simulator::Schedule (m_daoAckTimeout, &RoutingProtocol::ResendDao, this, m_daoSequence);
        iter->event = event;
        iter->daoMessageCounter++;
        ++iter;
      }
      else
      {
        // RFC 18.2.6.
        NS_LOG_LOGIC ("Send No-Path Dao to parent, as he is not reachable");
        m_sendDaoNoPathEvent = Simulator::Schedule (m_delayDao, &RoutingProtocol::SendDao, this, m_daoSequence++, true);
        iter = m_sentDaos.erase(iter);
        DeletePreferredParent ();
        UpdatePreferredParent (); 
      }
    }
    else
    {
      ++iter;
    }
  }

}


/*void RoutingProtocol::DaoAckExpireTimer ()
{
  if (m_preferredParent.rank == INFINITE_RANK)
  {
    NS_LOG_LOGIC ("Dao retransmission failed - no preferred parent set");
    return;
  }
  if (m_daoMessageCounter <= m_numberOfDaoRetries)
  {
    
    if (m_sendDaoEvent.GetUid () == m_sendDaoEvent.INVALID || m_sendDaoEvent.IsExpired ())
    {
      NS_LOG_LOGIC ("Resending Dao");
      m_sendDaoEvent = Simulator::Schedule (m_delayDao, &RoutingProtocol::SendDao, this, m_daoSequence, false);
    }
  }
  else
  {
    // RFC 18.2.6.
    NS_LOG_LOGIC ("Send No-Path Dao to parent, as he is not reachable");
    m_sendDaoNoPathEvent = Simulator::Schedule (m_delayDao, &RoutingProtocol::SendDao, this, m_daoSequence++, true);
    DeletePreferredParent ();
    UpdatePreferredParent ();
  }
}
*/

}
}

