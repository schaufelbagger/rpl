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
#include "ns3/inet-socket-address.h"
#include "ns3/socket-factory.h"
#include "ns3/ipv6-raw-socket-factory.h"
#include "ns3/boolean.h"
#include "ns3/uinteger.h"
#include<iostream>

namespace ns3 {
NS_LOG_COMPONENT_DEFINE("Rpl");
namespace rpl{

NS_OBJECT_ENSURE_REGISTERED(RoutingProtocol);


RoutingProtocol::RoutingProtocol ()
{
  NS_LOG_UNCOND("rpl constructor\n\n");

    
  //DisHeader disHeader;
  DisHeader head;
  head.SetFlags (2);
  std::cout<<"The Value of 'type' is "<< unsigned(head.GetType());
  std::cout<<std::endl;
  std::cout<<"The Value of 'code' is "<< unsigned(head.GetCode()) <<std::endl;
  std::cout<<"The Value of 'flags' is "<< unsigned(head.GetFlags()) <<std::endl;

  RplHeaderOption option;
  std::vector<uint8_t> testdata;
  for (int i = 0; i < 5; i++) {
    testdata.push_back(i);
  }
  
  option.SetDagMetricContainer(testdata);
  option.Print();

}

void RoutingProtocol::SetInstanceId(uint16_t instanceId)
{
  m_instanceId = instanceId;
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
  return rtentry;
}

void RoutingProtocol::SetIpv6 (Ptr<Ipv6> ipv6)
{
  NS_ASSERT (ipv6 != 0);
  NS_ASSERT (m_ipv6 == 0);
  NS_LOG_DEBUG ("Created rpl::RoutingProtocol");
  //TODO init code

  m_ipv6 = ipv6;
}

void RoutingProtocol::RecvRpl (Ptr<Socket> socket)
{

}

void RoutingProtocol::NotifyInterfaceUp (uint32_t interface)
{

  NS_LOG_UNCOND("rpl interface up\n\n");

  TypeId tid = TypeId::LookupByName ("ns3::Ipv6RawSocketFactory");
  Ptr<Socket> m_socket = Socket::CreateSocket (GetObject<Node> (), tid);

  NS_ASSERT (m_socket);

  m_socket->SetAttribute ("Protocol", UintegerValue(Ipv6Header::IPV6_ICMPV6));
  m_socket->SetRecvCallback (MakeCallback (&RoutingProtocol::RecvRpl, this));


  
}
void RoutingProtocol::NotifyInterfaceDown (uint32_t interface)
{}
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



}
}

