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
#include "rpl-routing-table.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("RplRoutingTable");

namespace rpl {



  RplRoutingTableEntry::RplRoutingTableEntry ()
  {
  }

  RplRoutingTableEntry::RplRoutingTableEntry (Ipv6Address dest, Ipv6Address nextHop, uint32_t interface)
  : 
  Ipv6RoutingTableEntry ( Ipv6RoutingTableEntry::CreateHostRouteTo (dest, nextHop, interface, Ipv6Address ("::"))),
  m_metric (0)
  {
  }


  RplRoutingTableEntry::RplRoutingTableEntry (Ipv6Address dest, uint32_t interface)
  : 
  Ipv6RoutingTableEntry ( Ipv6RoutingTableEntry::CreateHostRouteTo (dest, interface)),
  m_metric (0)
  {
  }

  void RplRoutingTableEntry::Print (std::ostream &os) const
  {
    os << "( ";
    os << "Destination " << GetDest () << ", ";
    os << "Next Hop " << GetGateway () << ", ";
    os << "Interface " << GetInterface () << ", ";
    os << "Metric " << +m_metric << ", ";
    os << "DODAG ID " << +m_metric << ", ";
    os << "Instance ID " << +m_instanceId << ", ";
    os << "DTSN " << +m_dtsn << ", ";
    os << "Lifetime " << +m_lifetime << ", ";
    os << "Downward " << +m_downward << ")" << std::endl;
  }

  RplRoutingTable::RplRoutingTable ()
  {}

  bool RplRoutingTable::Empty () const
  {
    return m_routingTable.empty ();
  }

  void RplRoutingTable::RemoveRoutes (Ipv6Address dst, Ipv6Address gateway, uint32_t interface)
  {
    for(auto iter = m_routingTable.begin(); iter != m_routingTable.end();)
    {
      RplRoutingTableEntry currentRoute = *iter;
      if(!dst.IsAny() && currentRoute.GetDest () == dst && currentRoute.GetInterface () == interface)
      {
        iter = m_routingTable.erase(iter);
      }
      else if (!gateway.IsAny() && currentRoute.GetGateway () == gateway && currentRoute.GetInterface () == interface)
      {
        iter = m_routingTable.erase(iter);
      }
      else
      {
        ++iter;
      }
    }
  }

  void RplRoutingTable::RemoveDownwardRoutes (Ipv6Address dodagId, uint8_t instanceId)
  {
    for(auto iter = m_routingTable.begin(); iter != m_routingTable.end();)
    {
      RplRoutingTableEntry currentRoute = *iter;
      // remove routes where destination is global and next hop is link-local, as well the route belongs to the Sub-DODAG
      if(!currentRoute.GetDest ().IsLinkLocal () && currentRoute.GetGateway ().IsLinkLocal () && currentRoute.GetDownward () && currentRoute.GetDodagId () == dodagId && currentRoute.GetInstanceId () == instanceId)
      {
        iter = m_routingTable.erase(iter);
      }
      else
      {
        ++iter;
      }
    }
  }


  void RplRoutingTable::AddRoute (Ipv6Address dest, Ipv6Address nextHop, uint32_t interface, uint16_t metric, Ipv6Address dodagId, uint8_t instanceId, uint8_t dtsn, uint8_t lifetime, bool downward)
  {
    NS_LOG_FUNCTION (this << dest << nextHop << interface);
    if (!checkAndUpdateDuplicate (dest, nextHop, interface, metric, dodagId, instanceId, dtsn, lifetime, downward))
    {
      RplRoutingTableEntry route = RplRoutingTableEntry (dest, nextHop, interface);
      route.SetMetric (metric);
      route.SetDodagId (dodagId);
      route.SetInstanceId (instanceId);
      route.SetDtsn (dtsn);
      route.SetLifetime (lifetime);
      route.SetDownward (downward);
      m_routingTable.push_back (route);
    }
  }

  void RplRoutingTable::AddRoute (Ipv6Address dest, uint32_t interface, uint16_t metric, Ipv6Address dodagId, uint8_t instanceId, uint8_t dtsn, uint8_t lifetime, bool downward)
  {
    NS_LOG_FUNCTION (this << dest << interface);
    if (!checkAndUpdateDuplicate (dest, Ipv6Address::GetZero (), interface, metric, dodagId, instanceId, dtsn, lifetime, downward))
    {
      RplRoutingTableEntry route = RplRoutingTableEntry (dest, interface);
      route.SetMetric (metric);
      route.SetDodagId (dodagId);
      route.SetInstanceId (instanceId);
      route.SetDtsn (dtsn);
      route.SetLifetime (lifetime);
      route.SetDownward (downward);
      m_routingTable.push_back (route);
    }

  }

  void RplRoutingTable::AddDownwardRoutes (std::list<RplHeaderOption::RplTarget> rplTargets, std::list<RplHeaderOption::TransitInformation> transitInformations, Ipv6Address nextHop, uint32_t interface, uint16_t metric, uint8_t daoSequence, Ipv6Address dodagId, uint8_t instanceId, bool isStoring)
  {
    if (!transitInformations.empty ())
    {
      if (isStoring && transitInformations.size () > 1)
      {
        NS_ABORT_MSG ("more than 1 transit Information Objects in storing mode");
      }
      for (auto currentRplTarget : rplTargets)
      {
        bool downward = true;
        uint8_t dtsn = daoSequence;
        if (isStoring)
        {
          uint8_t lifetime = transitInformations.front ().pathLifetime;
          AddRoute (currentRplTarget.targetPrefix.ConvertToIpv6Address (), nextHop, interface, metric, dodagId, instanceId, dtsn, lifetime, downward);
        }
        else
        {
          NS_ABORT_MSG ("TODO implement routing table entry in non-storing mode");
        }
      }
    }
  }

  bool RplRoutingTable::checkAndUpdateDuplicate (Ipv6Address dst, Ipv6Address nextHop, uint32_t interface, uint16_t metric, Ipv6Address dodagId, uint8_t instanceId, uint8_t dtsn, uint8_t lifetime, bool downward)
  {
    for (RplRoutingTableEntry & route : m_routingTable)
    {
      if (route.GetDest () == dst && route.GetGateway () == nextHop && route.GetInterface () == interface)
      {
        route.SetMetric (metric);
        route.SetDodagId (dodagId);
        route.SetInstanceId (instanceId);
        route.SetDtsn (dtsn);
        route.SetLifetime (lifetime);
        route.SetDownward (downward);
        return true;
      }
    }
    return false;
  }

  std::optional<RplRoutingTableEntry> RplRoutingTable::SearchEntry (Ptr<Ipv6> ipv6, Ipv6Address dst, Ptr<NetDevice> interface) const
  {
    for (RplRoutingTableEntry const& route : m_routingTable)
    {
      if (dst == route.GetDest () )
      {
        if (!interface || interface == ipv6->GetNetDevice (route.GetInterface ()))
        {
          return route;
        }
      }
    }
    return std::nullopt;
  }

  void RplRoutingTable::Print (Ptr<Ipv6> ipv6, Ptr<OutputStreamWrapper> stream, Time::Unit unit) const
  {
  *stream->GetStream () << "Node: " << ipv6->GetObject<Node> ()->GetId ()
                          << "; Time: " << Now ().As (unit)
                          << ", Local time: " << ipv6->GetObject<Node> ()->GetLocalTime ().As (unit)
                          << ", RPL Routing table" << std::endl;

    for (RplRoutingTableEntry const& route : m_routingTable)
    {
      route.Print (*stream->GetStream ());
    }
    *stream->GetStream () << std::endl;

  }

  void RplRoutingTable::Print (Ptr<Ipv6> ipv6, std::ostream &os) const
  {
    os << "Node: " << ipv6->GetObject<Node> ()->GetId ()
                          << ", RPL Routing table" << std::endl;
    Print (os);
  }

  void RplRoutingTable::Print (std::ostream &os) const
  {
    for (RplRoutingTableEntry const& route : m_routingTable)
    {
      route.Print (os);
    }
    os << std::endl;
  }

}
}