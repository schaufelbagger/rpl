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
#ifndef RPL_H
#define RPL_H

#include "ns3/ipv6-routing-protocol.h"
#include "ns3/ipv6-route.h"
#include "ns3/node.h"

#include "rpl-header.h"

namespace ns3 {
namespace rpl {



///
/// \ingroup rpl
///
/// \brief RPL routing protocol for IPv6
///
class RoutingProtocol : public Ipv6RoutingProtocol
{
public:

  RoutingProtocol();
  /**
   * \brief Get the type ID.
   * \return The object TypeId.
   */
  static TypeId GetTypeId (void);

  bool RouteInput (Ptr< const Packet > p,
  const Ipv6Header& header,
  Ptr< const NetDevice > idev,
  UnicastForwardCallback ucb,
  MulticastForwardCallback mcb,
  LocalDeliverCallback lcb,
  ErrorCallback ecb);

  Ptr<Ipv6Route> RouteOutput (Ptr< Packet > p,
  const Ipv6Header& header,
  Ptr< NetDevice > oif,
  Socket::SocketErrno & sockerr);



  void SetIpv6 (Ptr<Ipv6> ipv6);

  ///\name Receive control packets
  //\{
  /**
   * Receive and process control packet
   * \param socket input socket
   */
  void RecvRpl (Ptr<Socket> socket);

  void NotifyInterfaceUp (uint32_t interface);
  void NotifyInterfaceDown (uint32_t interface);
  void NotifyAddAddress (uint32_t interface, Ipv6InterfaceAddress address);
  void NotifyRemoveAddress (uint32_t interface, Ipv6InterfaceAddress address);
  void NotifyAddRoute (Ipv6Address dst, Ipv6Prefix mask, Ipv6Address nextHop, uint32_t interface, Ipv6Address prefixToUse = Ipv6Address::GetZero ());
  void NotifyRemoveRoute (Ipv6Address dst, Ipv6Prefix mask, Ipv6Address nextHop, uint32_t interface, Ipv6Address prefixToUse = Ipv6Address::GetZero ());
  void PrintRoutingTable (Ptr<OutputStreamWrapper> stream, Time::Unit unit = Time::S) const;

  void SetInstanceId(uint16_t instanceId);

private:
  /// IP protocol
  Ptr<Ipv6> m_ipv6;
  // RPL Identifiers
  uint8_t m_instanceId = 0;
  Ipv6Address m_dodagid;
  uint8_t m_dodagVersionNumber;
  uint16_t m_rank;
  bool m_isGrounded;

};

}   // rpl
}   // ns3

#endif /* RPL_H */

