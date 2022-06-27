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
#ifndef RPL_DODAG_H
#define RPL_DODAG_H

#include "ns3/node.h"
#include "ns3/ipv6-routing-table-entry.h"

#include "rpl-header.h"

#include <set>

namespace ns3 {
namespace rpl {


struct RplNode 
{
  uint16_t rank;
  Ipv6Address address;
  uint32_t interface;
  mutable uint8_t dtsn;
  bool operator<(const RplNode& rhs) const
  {
    return std::tie(rank, interface, address) < std::tie(rhs.rank, rhs.interface, rhs.address);
  }
  bool operator==(const RplNode& rhs) const
  {
    return std::tie(rank, interface, address) == std::tie(rhs.rank, rhs.interface, rhs.address);
  }
  bool operator!=(const RplNode& rhs) const
  {
    return std::tie(rank, interface, address) != std::tie(rhs.rank, rhs.interface, rhs.address);
  }
};

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
 * \brief   RPL header options field
 */
class RplState
{
public:
  /**
   * constructor
   *
   * \param dodagId 128-bit IPv6 address set by a DODAG root that uniquely identifies a DODAG
   * \param grounded Grounded flag (0)
   */
  RplState (uint8_t grounded = 0,
    Ipv6Address dodagId = {});
private:
  uint8_t m_grounded : 1;
  Ipv6Address m_dodagId = {};
  std::set<RplNode> m_parents;
  RplNode m_preferredParent;
};

}
}

#endif // RPL_DODAG_H