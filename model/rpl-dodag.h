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
#include "rpl-header.h"

namespace ns3 {
namespace rpl {


/**
 * \ingroup rpl
 * \brief   RPL header options field
 */
class RplDodag
{
public:
  /**
   * constructor
   *
   * \param dodagid 128-bit IPv6 address set by a DODAG root that uniquely identifies a DODAG
   * \param grounded Grounded flag (0)
   */
  RplDodag (uint8_t grounded = 0,
    Ipv6Address dodagid = {});
private:
  uint8_t m_grounded : 1;
  Ipv6Address m_dodagid = {};
};

}
}

#endif // RPL_DODAG_H