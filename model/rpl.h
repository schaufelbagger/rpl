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
#include "ns3/trickle-timer.h"

#include "rpl-header.h"
#include "rpl-header-option.h"
#include "rpl-objective-function.h"

#include <memory>

namespace ns3 {
namespace rpl {

#define RPL_DEFAULT_INSTANCE 0

#define RPL_ALL_NODE "ff02::1a"

// Trickle timer parameter [RFC6550, 8.3.1]
#define DEFAULT_DIO_INTERVAL_MIN 0x03
#define DEFAULT_DIO_INTERVAL_DOUBLINGS 0x03
#define DEFAULT_DIO_REDUNDANCY_CONSTANT 0x14

#define DEFAULT_DAO_DELAY 1
//#define DEFAULT_INIT_DODAG_VERSION 0



enum RplMop_e : uint8_t {MOP_NO_DOWNWARD_ROUTES=0, MOP_NON_STORING=1, MOP_STORING_NO_MULTICAST=2, MOP_STORING_MULTICAST=3};

///
/// \ingroup rpl
///
/// \brief RPL routing protocol for IPv6
///
class RoutingProtocol : public Ipv6RoutingProtocol
{
public:

  RoutingProtocol();

  void DoInitialize ();
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
  void NotifyInterfaceUp (uint32_t interface);
  void NotifyInterfaceDown (uint32_t interface);
  void NotifyAddAddress (uint32_t interface, Ipv6InterfaceAddress address);
  void NotifyRemoveAddress (uint32_t interface, Ipv6InterfaceAddress address);
  void NotifyAddRoute (Ipv6Address dst, Ipv6Prefix mask, Ipv6Address nextHop, uint32_t interface, Ipv6Address prefixToUse = Ipv6Address::GetZero ());
  void NotifyRemoveRoute (Ipv6Address dst, Ipv6Prefix mask, Ipv6Address nextHop, uint32_t interface, Ipv6Address prefixToUse = Ipv6Address::GetZero ());
  void PrintRoutingTable (Ptr<OutputStreamWrapper> stream, Time::Unit unit = Time::S) const;

  /**
   * \brief Set if node is Root
   * \param isRoot if node is Root
   */
  void SetRoot (bool isRoot)
  {
    m_isRoot = isRoot;
  }
  /**
   * \brief Get if node is Root
   * \return if node is Root
   */
  bool GetRoot ()
  {
    return m_isRoot;
  }
  /**
   * \brief Set the Mode of Operation
   * \param mop the Mode of Operation
   */
  void SetMop (RplMop_e mop)
  {
    m_mop = mop;
  }
  /**
   * \brief Get the Mode of Operation
   * \return the Mode of Operation
   */
  RplMop_e GetMop ()
  {
    return m_mop;
  }
  /**
   * \brief Set the Instance Id
   * \param instanceId the Instance Id
   */
  void SetInstanceId (uint8_t instanceId)
  {
    m_instanceId = instanceId;
  }
  /**
   * \brief Get the Instance Id
   * \return the Instance Id
   */
  uint8_t GetInstanceId ()
  {
    return m_instanceId;
  }
  /**
   * \brief Set the Interface Exclusions
   * \param instanceId the Interface Exclusions
   */
  void SetInterfaceExclusions (std::set<uint32_t> exceptions)
  {
    //NS_LOG_FUNCTION (this);
    m_interfaceExclusions = exceptions;
  }
  /**
   * \brief Get the Interface Exclusions
   * \return the Interface Exclusions
   */
  std::set<uint32_t> GetInterfaceExclusions () const
  {
    return m_interfaceExclusions;
  }





private:

  void Start ();
  /**
   * Receive and process control packet
   * \param socket input socket
   */
  void Receive (Ptr<Socket> socket);

  /**
   * \brief adds sending and receive sockets
   * 
   * \param interface the interface on which the link-local address will be added as sending address
   */
  void RegisterSockets (uint32_t interface);

  /**
   * \brief Lookup in the forwarding table for destination.
   * 
   * \param dst destination address of the packet
   * \param setSource set source address in the route
   * \param interface output interface if any (put 0 otherwise)
   * \return Ptr<Ipv6Route> Ipv6Route to route the packet to reach dest address
   */
  Ptr<Ipv6Route> Lookup (Ipv6Address dst, bool setSource, Ptr<NetDevice> interface = 0);

  /**
   * \brief Fires when trickle timer expires
   */
  void ExpireTimer (void);


  bool m_initialized = false;
  /// IP protocol
  Ptr<Ipv6> m_ipv6;
  std::set<uint32_t> m_interfaceExclusions; // Set of excluded interfaces
  /// Socket list type
  typedef std::map< Ptr<Socket>, uint32_t> SocketList;
  /// Socket list type iterator
  typedef std::map<Ptr<Socket>, uint32_t>::iterator SocketListI;
  /// Socket list type const iterator
  typedef std::map<Ptr<Socket>, uint32_t>::const_iterator SocketListCI;
  /// Sockets
  SocketList m_unicastSocketList; // list of sockets for unicast messages (socket, interface index)
  Ptr<Socket> m_multicastRecvSocket; // multicast receive socket
  /// RPL Identifiers
  uint8_t m_instanceId = RPL_DEFAULT_INSTANCE;
  Ipv6Address m_dodagId;
  uint8_t m_dodagVersionNumber;
  uint16_t m_rank;
  uint8_t m_dtsn;
  RplMop_e m_mop;
  uint8_t m_prf : 3;
  bool m_isStoring;
  bool m_isRoot = false;
  bool m_isLeaf = false;
  bool m_isGrounded = false;
  /// Timer
  TrickleTimer m_trickleTimer = TrickleTimer (Time(DEFAULT_DIO_INTERVAL_MIN), DEFAULT_DIO_INTERVAL_DOUBLINGS,DEFAULT_DIO_REDUNDANCY_CONSTANT);
};


}   // rpl
}   // ns3

#endif /* RPL_H */

