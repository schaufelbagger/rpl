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


RoutingProtocol::RoutingProtocol ()
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


  m_trickleTimer.SetFunction (&RoutingProtocol::ExpireTimer, this);

  if (m_isRoot)
  {
    NS_ABORT_MSG_IF (m_mop > 3, "Mode of Operation is invalid");
    if (m_mop >= 2)
    {
      m_isStoring = true;
    }else{
      m_isStoring = false;
    }
  }

  if (m_isRoot)
  {
    m_rank = ROOT_RANK;
    m_isGrounded = true;
    m_prf = 0;
    // [RFC 6550, 8.3.1]
    m_trickleTimer.Enable ();

    // TODO send DIS
  }

  std::cout<<"Is Root: "<< +m_isRoot <<std::endl;
  std::cout<<"MOP: "<< +m_mop <<std::endl;
  std::cout<<"Instance ID: "<< +m_instanceId <<std::endl;

  Ipv6RoutingProtocol::DoInitialize ();
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

  if (destination.IsMulticast ())
  {
    // Note:  Multicast routes for outbound packets are stored in the
    // normal unicast table.  An implication of this is that it is not
    // possible to source multicast datagrams on multiple interfaces.
    // This is a well-known property of sockets implementation on
    // many Unix variants.
    // So, we just log it and fall through to LookupStatic ()
    NS_LOG_LOGIC ("RouteOutput (): Multicast destination");
  }
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

  /*Ipv6PacketInfoTag interfaceInfo;
  if (!packet->RemovePacketTag (interfaceInfo))
  {
    NS_ABORT_MSG ("No incoming interface on RPL message, aborting.");
  }
  uint32_t incomingIf = interfaceInfo.GetRecvIf ();
  Ptr<Node> node = this->GetObject<Node> ();
  Ptr<NetDevice> dev = node->GetDevice (incomingIf);
  uint32_t ipInterfaceIndex = m_ipv6->GetInterfaceForDevice (dev);

  SocketIpv6HopLimitTag hoplimitTag;
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

  Ipv6Header ipv6;
  packet->RemoveHeader (ipv6);
  NS_ABORT_MSG_UNLESS (ipv6.GetNextHeader () == Ipv6Header::IPV6_ICMPV6, "The received Packet is not an ICMPv6 packet");
  RplIcmpv6Header rplIcmpv6Header;

  packet->RemoveHeader (rplIcmpv6Header);
  NS_ABORT_MSG_UNLESS (rplIcmpv6Header.GetType () == 155, "Received ICMPv6 Message with type other than 155");
switch (rplIcmpv6Header.GetCode ())
    {
    case TYPE_DIS:
      NS_ABORT_MSG ("TODO add what to do upon receiving base message with code "<< rplIcmpv6Header.GetCode ());
      break;
    case TYPE_DIO:
      NS_ABORT_MSG ("TODO add what to do upon receiving base message with code "<< rplIcmpv6Header.GetCode ());
      /*DioHeader dioHeader;
      packet->RemoveHeader (dioHeader);*/
      break;
    case TYPE_DAO:
      NS_ABORT_MSG ("TODO add what to do upon receiving base message with code "<< rplIcmpv6Header.GetCode ());
      break;
    case TYPE_DAO_ACK:
      NS_ABORT_MSG ("TODO add what to do upon receiving base message with code "<< rplIcmpv6Header.GetCode ());
      break;
    case TYPE_CC:
      NS_ABORT_MSG ("TODO add what to do upon receiving base message with code "<< rplIcmpv6Header.GetCode ());
      break;
    default:
      NS_ABORT_MSG ("Unrecognised base message code");
    }
}

void RoutingProtocol::NotifyInterfaceUp (uint32_t interface)
{
  // Initialize function may be not executed before the interface goes up

  NS_LOG_FUNCTION (this << interface);

  /*TypeId tid = TypeId::LookupByName ("ns3::Ipv6RawSocketFactory");
  Ptr<Socket> m_socket = Socket::CreateSocket (GetObject<Node> (), tid);

  NS_ASSERT (m_socket);

  m_socket->SetAttribute ("Protocol", UintegerValue(Ipv6Header::IPV6_ICMPV6));
  m_socket->SetRecvCallback (MakeCallback (&RoutingProtocol::Receive, this));
  */

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
{}
void RoutingProtocol::NotifyRemoveAddress (uint32_t interface, Ipv6InterfaceAddress address)
{}
void RoutingProtocol::NotifyAddRoute (Ipv6Address dst, Ipv6Prefix mask, Ipv6Address nextHop, uint32_t interface, Ipv6Address prefixToUse)
{}
void RoutingProtocol::NotifyRemoveRoute (Ipv6Address dst, Ipv6Prefix mask, Ipv6Address nextHop, uint32_t interface, Ipv6Address prefixToUse)
{}
void RoutingProtocol::PrintRoutingTable (Ptr<OutputStreamWrapper> stream, Time::Unit unit) const
{}

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
      //socket->SetRecvPktInfo (true);
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
    //m_multicastRecvSocket->SetRecvPktInfo (true);
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

  NS_ABORT_MSG ("RoutingProtocol::Lookup (...): TODO implement everything that is not a multicast!");
}

void RoutingProtocol::ExpireTimer (void)
{
  // send DIO
  NS_LOG_LOGIC ("RPL: Send DIO Broadcast due to Trickle Timer expiration");
  Ptr<Packet> packet = Create<Packet> ();
  DioHeader dioHeader (m_instanceId, m_dodagVersionNumber, m_rank, m_isGrounded, m_mop, m_prf, m_dtsn, 0, 0, m_dodagId);
  RplIcmpv6Header rplIcmpv6Header (TYPE_DIO);

  packet->AddHeader (dioHeader);
  packet->AddHeader (rplIcmpv6Header);




  for (SocketListI iter = m_unicastSocketList.begin (); iter != m_unicastSocketList.end (); iter++ )
  {
    uint32_t interface = iter->second;

    if (m_interfaceExclusions.find (interface) == m_interfaceExclusions.end ())
    {
      NS_LOG_DEBUG ("SendTo: " << +packet);
      iter->first->SendTo (packet, 0, Inet6SocketAddress (RPL_ALL_NODE));
    }
  }
  //m_multicastRecvSocket->SendTo (packet, 0, Inet6SocketAddress (RPL_ALL_NODE));


  //sendRplPacket(createDio(), DIO, Ipv6Address::ALL_NODES_1, uniform(0, 1));
  m_trickleTimer.Stop ();
}

}
}

