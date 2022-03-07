/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef RPL_H
#define RPL_H

#include "ns3/ipv6-routing-protocol.h"
#include "ns3/ipv6-route.h"
#include "ns3/node.h"

#include "ns3/rpl-header.h"

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

  bool RouteInput(Ptr< const Packet > p,
  const Ipv6Header& header,
  Ptr< const NetDevice > idev,
  UnicastForwardCallback ucb,
  MulticastForwardCallback mcb,
  LocalDeliverCallback lcb,
  ErrorCallback ecb);

  Ptr<Ipv6Route> RouteOutput(Ptr< Packet > p,
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

private:
  /// IP protocol
  Ptr<Ipv6> m_ipv6;
};

}   // rpl
}   // ns3

#endif /* RPL_H */

