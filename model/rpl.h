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
#include "ns3/timer.h"
#include "ns3/trickle-timer.h"
#include "ns3/traced-callback.h"
#include "ns3/lollipop-counter.h"
#include "ns3/ipv6-extension-header.h"
#include "ns3/ipv6-header.h"


#include "rpl-header.h"
#include "rpl-header-option.h"
#include "rpl-objective-function.h"
#include "rpl-state.h"
#include "rpl-routing-table.h"
#include "rpl-ipv6-header.h"

#include <memory>

namespace ns3 {
namespace rpl {

#define RPL_DEFAULT_INSTANCE 0

#define RPL_ALL_NODE "ff02::1a"

// Trickle timer parameter [RFC6550, 8.3.1]
#define DEFAULT_DIO_INTERVAL_MIN 3
#define DEFAULT_DIO_INTERVAL_DOUBLINGS 20
#define DEFAULT_DIO_REDUNDANCY_CONSTANT 10

#define DEFAULT_DIS_DELAY Seconds(1)
#define DEFAULT_DIS_MESSAGES 5
#define DEFAULT_DAO_DELAY Seconds(1)
#define DEFAULT_DAO_MESSAGES 5
#define DEFAULT_DODAG_PREFERENCE 0

#define DEFAULT_INIT_DODAG_VERSION 0

// use a day as lifetime
#define DEFAULT_LIFETIME_UNIT 60*60
#define DEFAULT_LIFETIME 24



enum RplMop_e : uint8_t {MOP_NO_DOWNWARD_ROUTES=0, MOP_NON_STORING=1, MOP_STORING_NO_MULTICAST=2, MOP_STORING_MULTICAST=3};
enum RplDisMop_e : uint8_t {DIS_MOP_WAIT=1, DIS_MOP_SEND=2};

///
/// \ingroup rpl
///
/// \brief RPL routing protocol for IPv6
///
class RoutingProtocol : public Ipv6RoutingProtocol
{
public:

  RoutingProtocol(bool m_isRoot = false,
                  /// Router parameters
                  uint8_t instanceId = RPL_DEFAULT_INSTANCE, 
                  std::set<RplObjectiveCodePoint_e> m_ocps = { OF0 },
                  std::set<uint8_t> m_routingMCs = { 0 },
                  uint32_t m_validLifetime = 0xFFFFFFFF,
                  uint32_t m_preferredLifetime = 0xFFFFFFFF,
                  bool m_onLink = true,
                  bool m_autonomousAddressConfiguration = true,
                  bool m_versionPredicate = false,
                  bool m_instanceIdPredicate = false,
                  bool m_dodagIdPredicate = false,
                  bool m_k = true,
                  rpl::RplMop_e m_mop = rpl::MOP_STORING_NO_MULTICAST,
                  uint8_t m_routePreference = 0b10,
                  uint32_t m_routeLifetime = 0xFFFFFFFF,
                  Ipv6Prefix m_prefix = Ipv6Prefix (),
                  /// Non-DODAG-Root Router
                  Ipv6Prefix m_targetPrefix = Ipv6Prefix (),
                  bool m_poison = true,
                  bool m_repair = true,
                  /// DIS Mode of Operation
                  rpl::RplDisMop_e m_disMop = rpl::DIS_MOP_WAIT,
                  Time m_disMessageTime = DEFAULT_DIS_DELAY,
                  int m_numberOfDisMessages = DEFAULT_DIS_MESSAGES,
                  /// DODAG Root
                  uint8_t m_dioIntervalDoublings = DEFAULT_DIO_INTERVAL_DOUBLINGS,
                  uint8_t m_dioIntervalMin = DEFAULT_DIO_INTERVAL_MIN,
                  uint8_t m_dioRedundancyConstant = DEFAULT_DIO_REDUNDANCY_CONSTANT,
                  uint8_t m_pathControlSize = DEFAULT_PATH_CONTROL_SIZE,
                  uint16_t m_minHopRankIncrease = DEFAULT_MIN_HOP_RANK_INCREASE,
                  uint8_t m_dodagPreference = DEFAULT_DODAG_PREFERENCE,
                  Ipv6Address m_dodagId = Ipv6Address ("::"),
                  /// DAO based Parameters
                  Time m_delayDao = DEFAULT_DAO_DELAY,
                  int m_numberOfDaoRetries = DEFAULT_DAO_MESSAGES);

  void DoInitialize ();
  /**
   * \brief Get the type ID.
   * \return The object TypeId.
   */
  static TypeId GetTypeId (void);

  bool RouteInput(Ptr<const Packet> p,
                  const Ipv6Header& header,
                  Ptr<const NetDevice> idev,
                  const UnicastForwardCallback& ucb,
                  const MulticastForwardCallback& mcb,
                  const LocalDeliverCallback& lcb,
                  const ErrorCallback& ecb) override;
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
  void PrintRoutingTable  (std::ostream &os) const;
  void PrintRoutingTable  () const {PrintRoutingTable (std::cout);};

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
  bool GetRoot () const
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
  RplMop_e GetMop () const
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
  uint8_t GetInstanceId () const
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
  /**
   * \brief Set the DIS Mode of Operation
   * \param mop the DIS Mode of Operation
   */
  void SetDisMop (RplDisMop_e disMop)
  {
    m_disMop = disMop;
  }
  /**
   * \brief Get the DIS Mode of Operation
   * \return the DIS Mode of Operation
   */
  RplDisMop_e GetDisMop () const
  {
    return m_disMop;
  }
  /**
   * \brief Set the DIS Message Time
   * \param disMessageTime the DIS Message Time
   */
  void SetDisMessageTime (Time disMessageTime)
  {
    m_disMessageTime = disMessageTime;
  }
  /**
   * \brief Get the DIS Message Time
   * \return the DIS Message Time
   */
  Time GetDisMessageTime () const
  {
    return m_disMessageTime;
  }
  /**
   * \brief Set the Number Of DIS Messages
   * \param numberOfDisMessages the Number Of DIS Messages
   */
  void SetNumberOfDisMessages (int numberOfDisMessages)
  {
    m_numberOfDisMessages = numberOfDisMessages;
  }
  /**
   * \brief Get the Number Of DIS Messages
   * \return the Number Of DIS Messages
   */
  int GetNumberOfDisMessages () const
  {
    return m_numberOfDisMessages;
  }



  TracedCallback<RplNode> m_updatedPrefParentTrace;
  TracedCallback<RplNode> m_detachFromDodagTrace;
  TracedCallback<RplRoutingTableEntry> m_routeAddedTrace; // is used in the routing table

private:

  void Start ();
  void InitRoot ();
  bool isRplAllAddress (Ipv6Address addr);
  bool isMyAddress (Ipv6Address addr, Ptr<NetDevice> interface);
  bool isMyAddress (Ipv6Address addr, uint32_t interface);
  bool isMyAddress (Ipv6Address addr);
  bool isDodagParent (Ipv6Address addr, uint32_t interface);
  /**
   * Receive and process control packet
   * \param socket input socket
   */
  void Receive (Ptr<Socket> socket);

  void ReceiveDu (Ptr<Packet> packet, Ipv6Header, RplIcmpv6Header rplIcmpv6Header);
  void ReceiveDis (Ptr<Packet> packet, Ipv6Header ipv6Header);
  void ReceiveDio (Ptr<Packet> packet, Ipv6Header ipv6Header, uint32_t incomingInterface);
  void ReceiveDao (Ptr<Packet> packet, Ipv6Header ipv6Header, uint32_t incomingInterface);
  void ReceiveDaoAck (Ptr<Packet> packet, Ipv6Header ipv6Header);

  void UpdatePreferredParent ();
  void UpdateDaoParents ();
  void DeleteParent (Ipv6Address address, uint32_t interface);
  /**
   * \brief remove parents with higher rank
   */
  void RemoveObsoleteParents ();
  void DeletePreferredParent ();
  void ClearPreferredParentRoutes();
  void DetachFromDodag ();
  void ClearDownwardRoutes ();
  void PoisonChildren ();
  //void AddRouteToRoutingTable (Ipv6Address dest, Ipv6Address nextHop, uint32_t interface, uint16_t metric, Ipv6Address dodagId, uint8_t instanceId, uint8_t dtsn, bool downward);
  //void AddRouteToRoutingTable (Ipv6Address dest, uint32_t interface, uint16_t metric, Ipv6Address dodagId, uint8_t instanceId, uint8_t dtsn, bool downward);
  //void AddDownwardRoutesToRoutingTable (std::list<RplHeaderOption::RplTarget> rplTargets, std::list<RplHeaderOption::TransitInformation> transitInformations, Ipv6Address nextHop, uint32_t interface, uint16_t metric, uint8_t daoSequence);
  /**
   * \brief adds sending and receive sockets
   * 
   * \param interface the interface on which the link-local address will be added as sending address
   */
  void RegisterSockets (uint32_t interface);

  void SendOnAllInterfaces (Ptr<Packet> packet, const Address &toAddress);

  /**
   * \brief Lookup in the forwarding table for destination.
   * 
   * \param dst destination address of the packet
   * \param setSource set source address in the route
   * \param interface output interface if any (put 0 otherwise)
   * \return Ptr<Ipv6Route> Ipv6Route to route the packet to reach dest address
   */
  Ptr<Ipv6Route> Lookup (Ipv6Address dst, bool setSource, Ptr<NetDevice> interface = 0);

  Ptr<Ipv6Route> CreateRouteFromTableEntry (RplRoutingTableEntry const route, bool setSource, Ipv6Address dst);

  /**
   * \brief Fires when trickle timer expires
   */
  void ExpireTrickleTimer (void);
  /**
   * \brief Fires when a DIS message shall be sent
   */
  void DisExpireTimer (void);
  /**
   * \brief Fires when a DAO message shall be sent
   */
  void SendDao (bool isNoPath);

  void ResendDao (uint8_t daoSequence, RplNode daoParent);
  //void DaoAckExpireTimer ();

  /// Internal management
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
  
  /// List of known routes
  //RplRoutingTableEntry m_preferredParentRoute;
  //std::list<RplRoutingTableEntry> m_routingTable;
  RplRoutingTable m_routingTable;

  /// General parameters
  bool m_isRoot;
  /// Router parameters
  uint8_t m_instanceId;
  std::set<RplObjectiveCodePoint_e> m_ocps;
  std::set<uint8_t> m_routingMCs;
  uint32_t m_validLifetime;
  uint32_t m_preferredLifetime;
  bool m_onLink;
  bool m_autonomousAddressConfiguration;
  bool m_versionPredicate;
  bool m_instanceIdPredicate;
  bool m_dodagIdPredicate;
  bool m_k;
  RplMop_e m_mop;
  uint8_t m_routePreference : 2;
  uint32_t m_routeLifetime;
  Ipv6Prefix m_prefix;
  /// Non-DODAG-Root Router
  Ipv6Prefix m_targetPrefix;
  bool m_poison;
  bool m_repair;
  /// DIS Mode of Operation
  RplDisMop_e m_disMop;
  Time m_disMessageTime;
  int m_numberOfDisMessages;
  /// DODAG Root
  uint8_t m_dioIntervalDoublings;
  uint8_t m_dioIntervalMin;
  uint8_t m_dioRedundancyConstant;
  uint8_t m_pathControlSize : 3;
  uint16_t m_minHopRankIncrease;
  uint8_t m_dodagPreference : 3;
  Ipv6Address m_dodagId;
  /// DAO based Parameters
  Time m_delayDao;
  int m_numberOfDaoRetries;
  // is used to construct the Dao and is cleared afterwards
  //std::map<RplHeaderOption,std::list<RplHeaderOption> > m_childRplTargets;
  std::list<RplHeaderOption> m_childDaoOptions;


  /// RPL Identifiers
  LollipopCounter<uint8_t> m_dodagVersionNumber = DEFAULT_INIT_DODAG_VERSION;
  uint16_t m_rank = INFINITE_RANK;
  uint8_t m_dtsn;
  bool m_isStoring;
  bool m_isLeaf = false;
  bool m_isGrounded = false;
  RplObjectiveFunction m_ocp;
  uint8_t m_defaultLifetime = DEFAULT_LIFETIME;
  uint16_t m_defaultLifetimeUnit = DEFAULT_LIFETIME_UNIT;
  LollipopCounter<uint8_t> m_daoSequence = 0;
  LollipopCounter<uint8_t> m_pathSequence = 0;
  uint8_t m_maxDaoParents;

  /// DODAG state
  //std::set<RplNode> m_candidateNeighbors; // not used as, neighbors are directly put into the routing table and DODAG Parents are directly added from DIOs
  std::set<RplNode> m_dodagParents;
  std::set<RplNode> m_daoParents;
  RplNode m_preferredParent = {INFINITE_RANK, Ipv6Address ("::"), 0,0};
  bool m_dtsnChanged;

  /// DIS Mode of Operation
  int m_disMessageCounter = 0;
  bool m_receivedDio = false;

  /// Timers
  TrickleTimer m_trickleTimer = TrickleTimer (MilliSeconds(pow(2,DEFAULT_DIO_INTERVAL_MIN)), DEFAULT_DIO_INTERVAL_DOUBLINGS,DEFAULT_DIO_REDUNDANCY_CONSTANT);
  Timer m_disMessageTimer = Timer (Timer::CANCEL_ON_DESTROY);
  //Timer m_daoAckTimer = Timer (Timer::CANCEL_ON_DESTROY);
  Time m_daoAckTimeout = Seconds(10);

  /// Events
  EventId m_sendDaoEvent = EventId ();
  EventId m_sendDaoNoPathEvent = EventId ();

  struct SentDao 
  {
    uint8_t daoSequence;
    RplNode daoParent;
    Ptr<Packet> daoPacket;
    int daoMessageCounter;
    EventId event;

  };
  /// Storage
  std::list<SentDao> m_sentDaos;


};


}   // rpl
}   // ns3

#endif /* RPL_H */

