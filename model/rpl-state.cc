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
#include "rpl-state.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("RplState");

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


}
}