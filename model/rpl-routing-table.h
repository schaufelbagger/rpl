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
#ifndef RPL_ROUTING_TABLE_H
#define RPL_ROUTING_TABLE_H

#include "ns3/nstime.h"
#include "ns3/output-stream-wrapper.h"
#include "ns3/node.h"
#include "ns3/ipv6-routing-protocol.h"
#include "ns3/trickle-timer.h"
#include "ns3/ipv6-routing-table-entry.h"

#include "rpl-state.h"
#include "rpl-header-option.h"


namespace ns3 {
namespace rpl {



class RplRoutingTableEntry : public Ipv6RoutingTableEntry
{
public:
  RplRoutingTableEntry ();

  RplRoutingTableEntry (Ipv6Address dest, Ipv6Address nextHop, uint32_t interface);

  RplRoutingTableEntry (Ipv6Address dest, uint32_t interface);

  void Print (std::ostream &os) const;

  /**
   * \brief Set the Metric
   * \param metric the Metric
   */
  void SetMetric (uint16_t metric)
  {
    m_metric = metric;
  }
  /**
   * \brief Get the Metric
   * \return the Metric
   */
  uint16_t GetMetric () const
  {
    return m_metric;
  }
  /**
   * \brief Set the DODAG ID
   * \param dodagId the DODAG ID
   */
  void SetDodagId (Ipv6Address dodagId)
  {
    m_dodagId = dodagId;
  }
  /**
   * \brief Get the DODAG ID
   * \return the DODAG ID
   */
  Ipv6Address GetDodagId () const
  {
    return m_dodagId;
  }
  /**
   * \brief Set the Instance ID
   * \param dodagId the Instance ID
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
   * \brief Set the DTSN
   * \param dtsn the DTSN
   */
  void SetDtsn (uint8_t dtsn)
  {
    m_dtsn = dtsn;
  }
  /**
   * \brief Get the DTSN
   * \return the DTSN
   */
  uint8_t GetDtsn () const
  {
    return m_dtsn;
  }
  /**
   * \brief Set route lifetime
   * \param lifetime the route lifetime
   */
  void SetLifetime (uint8_t lifetime)
  {
    m_lifetime = lifetime;
  }
  /**
   * \brief Get route lifetime
   * \return the route lifetime
   */
  uint8_t GetLifetime () const
  {
    return m_lifetime;
  }
  /**
   * \brief Set if route is a downward route
   * \param downward boolean if the route is downward
   */
  void SetDownward (bool downward)
  {
    m_downward = downward;
  }
  /**
   * \brief Get if route is a downward route
   * \return if the route is downward
   */
  bool GetDownward () const
  {
    return m_downward;
  }
private:
  uint16_t m_metric;
  Ipv6Address m_dodagId;
  uint8_t m_instanceId;
  uint8_t m_dtsn;
  uint8_t m_lifetime;
  bool m_downward;
};



/**
 * \ingroup rpl
 * \brief   RPL routing table
 */
class RplRoutingTable
{
public:
  /**
   * constructor
   */
  RplRoutingTable ();

  bool Empty () const;
  /**
   * \brief removes routes from the routing table where destination or gateway (or both) are found on the specified interface
   *        using the any-address will not remove the respective entry
   * 
   * \param dst destination of the route entry
   * \param gateway next hop of the route entry
   * \param interface interface of the route entry
   */
  void RemoveRoutes (Ipv6Address dst = Ipv6Address("::"), Ipv6Address gateway = Ipv6Address("::"), uint32_t interface = 0);
  void RemoveDownwardRoutes (Ipv6Address dodagId, uint8_t instanceId);
  void AddRoute (Ipv6Address dest, Ipv6Address nextHop, uint32_t interface, uint16_t metric, Ipv6Address dodagId, uint8_t instanceId, uint8_t dtsn, uint8_t lifetime, bool downward);
  void AddRoute (Ipv6Address dest, uint32_t interface, uint16_t metric, Ipv6Address dodagId, uint8_t instanceId, uint8_t dtsn, uint8_t lifetime, bool downward);
  void AddDownwardRoutes (std::list<RplHeaderOption::RplTarget> rplTargets, std::list<RplHeaderOption::TransitInformation> transitInformations, Ipv6Address nextHop, uint32_t interface, uint16_t metric, uint8_t daoSequence, Ipv6Address dodagId, uint8_t instanceId, bool isStoring);
  bool checkAndUpdateDuplicate (Ipv6Address dst, Ipv6Address nextHop, uint32_t interface, uint16_t metric, Ipv6Address dodagId, uint8_t instanceId, uint8_t dtsn, uint8_t lifetime, bool downward);
  /**
   * \brief searches the table entry by destination and interface
   *        returns the null pointer when not found
   * 
   * \param dst destination address
   * \param interface interface of the net device
   * \return the route
   */
  std::optional<RplRoutingTableEntry> SearchEntry (Ptr<Ipv6> ipv6, Ipv6Address dst, Ptr<NetDevice> interface) const;
  void Print (Ptr<Ipv6> ipv6, Ptr<OutputStreamWrapper> stream, Time::Unit unit = Time::S) const;
  void Print (Ptr<Ipv6> ipv6, std::ostream &os) const;
  void Print (std::ostream &os) const;

  /**
   * \brief Set the preferred parent route
   * \param metric the preferred parent route
   */
  void SetPreferredParentRoute (RplRoutingTableEntry preferredParentRoute)
  {
    m_preferredParentRoute = preferredParentRoute;
  }
  /**
   * \brief Get the preferred parent route
   * \return the preferred parent route
   */
  RplRoutingTableEntry GetPreferredParentRoute () const
  {
    return m_preferredParentRoute;
  }
  /**
   * \brief Set the routing table
   * \param metric the routing table
   */
  void SetRoutingTable (std::list<RplRoutingTableEntry> routingTable)
  {
    m_routingTable = routingTable;
  }
  /**
   * \brief Get the routing table
   * \return the routing table
   */
  std::list<RplRoutingTableEntry> GetRoutingTable () const
  {
    return m_routingTable;
  }
private:
  RplRoutingTableEntry m_preferredParentRoute;
  std::list<RplRoutingTableEntry> m_routingTable;
};



}
}

#endif // RPL_ROUTING_TABLE_H