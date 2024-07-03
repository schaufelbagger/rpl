/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2024 Alexander Baranyai
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
 * Author: Alexander Baranyai <alexander.baranyai@gmx.net>
 */
#include "rpl-ipv6-header.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("RplIpv6Header");

namespace rpl {







RplHopByHopHeaderTag::RplHopByHopHeaderTag() :  Tag(), 
                                                m_o (0),
                                                m_r (0),
                                                m_f (0),
                                                m_rplInstanceId (0),
                                                m_senderRank (0)
{}




RplHopByHopHeaderTag::RplHopByHopHeaderTag(uint8_t o, 
uint8_t r, 
uint8_t f, 
uint8_t rplInstanceId, 
uint16_t senderRank)
: Tag ()
{
  NS_LOG_FUNCTION (this);

  NS_ABORT_MSG_IF(o > 1, "Size of field o is larger than allowed");
  NS_ABORT_MSG_IF(r > 1, "Size of field r is larger than allowed");
  NS_ABORT_MSG_IF(f > 1, "Size of field f is larger than allowed");


  m_o = o;
  m_r = r;
  m_f = f;
  m_rplInstanceId = rplInstanceId;
  m_senderRank = senderRank;
}


TypeId RplHopByHopHeaderTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::RplHopByHopHeaderTag")
    .SetParent<Tag> ()
    .SetGroupName ("Rpl")
    .AddConstructor<RplHopByHopHeaderTag> ()
  ;
  return tid;

}

TypeId RplHopByHopHeaderTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t RplHopByHopHeaderTag::GetSerializedSize (void) const
{
  return + sizeof (uint8_t) + sizeof (uint8_t) + sizeof (uint16_t);
}


void RplHopByHopHeaderTag::Serialize (TagBuffer i) const
{
  i.WriteU8 ((m_o << 7) | (m_r << 6) | (m_f << 5) | uint8_t(0));
  i.WriteU8 (m_rplInstanceId);
  i.WriteU16 (m_senderRank);
}

void RplHopByHopHeaderTag::Deserialize (TagBuffer i)
{

  uint8_t tmp;


  tmp = i.ReadU8 ();
  m_o = (tmp & 0b10000000) >> 7;
  m_r = (tmp & 0b01000000) >> 6;
  m_f = (tmp & 0b00100000) >> 5;
  m_rplInstanceId = i.ReadU8 ();
  m_senderRank = i.ReadU16 ();
}


void RplHopByHopHeaderTag::Print (std::ostream &os) const
{
  os << "Down(O): " << +m_o << std::endl;
  os << "Rank-Error(R): " << +m_r << std::endl;
  os << "Forwarding-Error(F): " << +m_f << std::endl;
  os << "RPL Instance ID: " << +m_rplInstanceId << std::endl;
  os << "Sender Rank: " << +m_senderRank << std::endl;
  //TODO add sub-TLVs
  return;
}



// NOT USED AS IT DOES NOT WORK - USING TAG INSTEAD

/*RplIpv6Header::RplIpv6Header () 
: Ipv6OptionHeader ()
{
  NS_LOG_FUNCTION (this);
  this->SetType (uint8_t (0x23));
  this->SetLength (uint8_t (4));
}

RplIpv6Header::RplIpv6Header(uint8_t o, 
uint8_t r, 
uint8_t f, 
uint8_t rplInstanceId, 
uint16_t senderRank)
: Ipv6OptionHeader ()
{
  NS_LOG_FUNCTION (this);

  NS_ABORT_MSG_IF(o > 1, "Size of field o is larger than allowed");
  NS_ABORT_MSG_IF(r > 1, "Size of field r is larger than allowed");
  NS_ABORT_MSG_IF(f > 1, "Size of field f is larger than allowed");

  this->SetType (uint8_t (0x23));
  this->SetLength (uint8_t (4));
  m_o = o;
  m_r = r;
  m_f = f;
  m_rplInstanceId = rplInstanceId;
  m_senderRank = senderRank;
}

TypeId RplIpv6Header::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::rpl::RplIpv6Header")
    .SetParent<Ipv6OptionHeader> ()
    .SetGroupName ("Rpl")
    .AddConstructor<RplIpv6Header> ()
  ;
  return tid;
}
TypeId RplIpv6Header::GetInstanceTypeId () const
{
  return GetTypeId ();
}

uint32_t RplIpv6Header::GetSerializedSize () const
{
  uint32_t size = 2;
  size = size + 4;
  //TODO add sub-TLV size
  return size;
}

void RplIpv6Header::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;

  i.WriteU8 (GetType ());
  i.WriteU8 (GetLength ());
  i.WriteU8 ((m_o << 7) | (m_r << 6) | (m_f << 5) | uint8_t(0));
  i.WriteU8 (m_rplInstanceId);
  i.WriteHtonU16 (m_senderRank);
  //TODO add sub-TLVs

}

uint32_t RplIpv6Header::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;

  uint8_t tmp;
  uint8_t optionType;
  uint8_t optionLength;

  optionType = i.ReadU8 ();
  optionLength = i.ReadU8 ();
  SetType (optionType);
  SetLength (optionLength);
  tmp = i.ReadU8 ();
  m_o = (tmp & 0b10000000) >> 7;
  m_r = (tmp & 0b01000000) >> 6;
  m_f = (tmp & 0b00100000) >> 5;
  m_rplInstanceId = i.ReadU8 ();
  m_senderRank = i.ReadNtohU16 ();
  //TODO add sub-TLVs

  uint32_t dist = i.GetDistanceFrom (start);
  NS_ASSERT (dist == GetSerializedSize ());
  return dist;
}

void RplIpv6Header::Print (std::ostream &os) const
{
  os << "Type: " << +GetType () << std::endl;
  os << "Length: " << +GetLength () << std::endl;
  os << "Down(O): " << +m_o << std::endl;
  os << "Rank-Error(R): " << +m_r << std::endl;
  os << "Forwarding-Error(F): " << +m_f << std::endl;
  os << "RPL Instance ID: " << +m_rplInstanceId << std::endl;
  os << "Sender Rank: " << +m_senderRank << std::endl;
  //TODO add sub-TLVs
  return;
}

Ipv6OptionHeader::Alignment RplIpv6Header::GetAlignment () const
{
  return (Alignment){2,0};
}
*/
}
}