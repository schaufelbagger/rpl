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

#define INFINITE_RANK 0xFFFF


struct RplNode 
{
  mutable uint16_t rank = INFINITE_RANK;
  Ipv6Address address = Ipv6Address ("::");
  uint32_t interface = 0xFFFFFFFF;
  mutable uint8_t dtsn = 0;
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