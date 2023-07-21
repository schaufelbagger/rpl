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

#ifndef RPL_IPV6_HEADER_H
#define RPL_IPV6_HEADER_H

#include "ns3/header.h"
#include "ns3/object.h"
#include "ns3/ipv6-option-header.h"


namespace ns3 {
namespace rpl {


/**
 * \ingroup rpl
 * \brief   RPL ICMPv6 header with type set to 155
  \verbatim
  0                   1                   2                   3
  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
                                  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
                                  |  Option Type  |  Opt Data Len |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |O|R|F|0|0|0|0|0| RPLInstanceID |          SenderRank           |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |                         (sub-TLVs)                            |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  \endverbatim
 */
class RplIpv6Header : public Ipv6OptionHeader
{
public:
  RplIpv6Header();
  RplIpv6Header(uint8_t o, 
  uint8_t r, 
  uint8_t f, 
  uint8_t rplInstanceId, 
  uint16_t senderRank);
  static TypeId GetTypeId ();
  TypeId GetInstanceTypeId () const;
  uint32_t GetSerializedSize () const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);
  void Print (std::ostream &os) const;
  Ipv6OptionHeader::Alignment GetAlignment () const;

  void SetO (uint8_t o)
  {
    NS_ABORT_MSG_IF(o > 1, "Size of field is larger than allowed");
    m_o = o;
  }
  uint8_t GetO () const
  {
    return m_o;
  }

  void SetR (uint8_t r)
  {
    NS_ABORT_MSG_IF(r > 1, "Size of field is larger than allowed");
    m_r = r;
  }
  uint8_t GetR () const
  {
    return m_r;
  }

  void SetF (uint8_t f)
  {
    NS_ABORT_MSG_IF(f > 1, "Size of field is larger than allowed");
    m_f = f;
  }
  uint8_t GetF () const
  {
    return m_f;
  }

  void SetRplInstanceId (uint8_t id)
  {
    m_rplInstanceId = id;
  }
  uint8_t GetRplInstanceId () const
  {
    return m_rplInstanceId;
  }

  void SetSenderRank (uint16_t rank)
  {
    m_senderRank = rank;
  }
  uint16_t GetSenderRank () const
  {
    return m_senderRank;
  }

private:

  uint8_t m_o;
  uint8_t m_r;
  uint8_t m_f;
  uint8_t m_rplInstanceId;
  uint16_t m_senderRank;

};



}
}
#endif /* RPL_HEADER_H */