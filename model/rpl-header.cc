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
#include "rpl-header.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("RplHeader");

namespace rpl {

// ---------------- RPL Header -------------------------------

RplIcmpv6Header::RplIcmpv6Header () : Icmpv6Header()
{
  NS_LOG_FUNCTION (this);
  this->SetType (uint8_t (155));
  m_calcChecksum = false;
}
RplIcmpv6Header::RplIcmpv6Header (RplPacketCode_e code) : Icmpv6Header()
{
  NS_LOG_FUNCTION (this);
  this->SetType (uint8_t (155));
  this->SetCode (code);
  m_calcChecksum = false;
}
TypeId RplIcmpv6Header::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::rpl::RplHeader")
    .SetParent<Icmpv6Header> ()
    .SetGroupName ("Rpl")
    .AddConstructor<RplIcmpv6Header> ()
  ;
  return tid;
}

// ---------------- RPL DIS Message -------------------------------

DisHeader::DisHeader (uint8_t flags, uint8_t reserved) 
:
m_flags (flags),
m_reserved (reserved)
{
}

TypeId DisHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::rpl::DisHeader")
    .SetParent<Header> ()
    .SetGroupName ("Rpl")
    .AddConstructor<DisHeader> ()
  ;
  return tid;
}
TypeId DisHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}

uint32_t DisHeader::GetSerializedSize () const
{
  uint32_t size = 2;
  for(auto option : m_options) 
    size += option.GetSerializedSize ();
  return size;
}
void DisHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  //RplHeader::Serialize (start);
  //i.Next (RplHeader::GetSerializedSize ());
  i.WriteU8 (m_flags);
  i.WriteU8 (m_reserved);
  //for(auto option : m_options) 
  //  option.Serialize (i);
}
uint32_t DisHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  //RplHeader::Deserialize (start);
  //i.Next (RplHeader::GetSerializedSize ());
  m_flags = i.ReadU8 ();
  m_reserved = i.ReadU8 ();

  uint32_t dist = i.GetDistanceFrom (start);
  NS_ASSERT (dist == GetSerializedSize ());
  return dist;
}
void DisHeader::Print (std::ostream &os) const
{
  os << "Flags: " << +m_flags << std::endl;
  //for(auto option : m_options) 
  //  option.Print (os);
}

uint8_t DisHeader::GetFlags () const
{
  return this->m_flags;
}

void DisHeader::SetFlags (uint8_t flags)
{
  this->m_flags = flags;
}

// ---------------- RPL DIO Message -------------------------------

DioHeader::DioHeader (uint8_t rplInstanceId, 
  uint8_t versionNumber,
  uint16_t rank,
  uint8_t grounded,
  uint8_t mop,
  uint8_t prf,
  uint8_t dtsn,
  uint8_t flags,
  uint8_t reserved,
  Ipv6Address dodagId)
  :
  m_rplInstanceId (rplInstanceId),
  m_versionNumber (versionNumber),
  m_rank (rank),
  m_grounded (grounded),
  m_mop (mop),
  m_prf (prf),
  m_dtsn (dtsn),
  m_flags (flags),
  m_reserved (reserved),
  m_dodagId (dodagId)
{
  NS_ABORT_MSG_IF(grounded > 1, "Size of field is larger than allowed");
  NS_ABORT_MSG_IF(prf > 0b111, "Size of field is larger than allowed");
  NS_ABORT_MSG_IF(mop > 0b111, "Size of field is larger than allowed");
}

TypeId DioHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::rpl::DioHeader")
    .SetParent<Header> ()
    .SetGroupName ("Rpl")
    .AddConstructor<DioHeader> ()
  ;
  return tid;
}

TypeId DioHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}
uint32_t DioHeader::GetSerializedSize () const
{
  return 24;
}
void DioHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  uint8_t buf[16];

  i.WriteU8 (m_rplInstanceId);
  i.WriteU8 (m_versionNumber);
  i.WriteHtonU16 (m_rank);
  i.WriteU8 ((m_grounded << 7) |(m_mop << 3) | m_prf);
  i.WriteU8 (m_dtsn);
  i.WriteU8 (m_flags);
  i.WriteU8 (m_reserved);
  m_dodagId.Serialize (buf);
  i.Write (buf, 16);
}
uint32_t DioHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  uint8_t buf[16];
  uint8_t tmp;
  m_rplInstanceId = i.ReadU8 ();
  m_versionNumber = i.ReadU8 ();
  m_rank = i.ReadNtohU16 ();

  tmp = i.ReadU8 ();
  m_grounded = (tmp & 0b10000000) >> 7;
  m_mop = (tmp & 0b00111000) >> 3;
  m_prf = (tmp & 0b00000111);
  m_dtsn = i.ReadU8 ();
  m_flags = i.ReadU8 ();
  m_reserved = i.ReadU8 ();
  i.Read (buf, 16);
  m_dodagId = Ipv6Address(buf);

  uint32_t dist = i.GetDistanceFrom (start);
  NS_ASSERT (dist == GetSerializedSize ());
  return dist;
}
void DioHeader::Print (std::ostream &os) const
{
  os << "RPL Instance ID: " << +m_rplInstanceId << std::endl;
  os << "Version Number: " << +m_versionNumber << std::endl;
  os << "Rank: " << +m_rank << std::endl;
  os << "Grounded: " << +m_grounded << std::endl;
  os << "MOP: " << +m_mop << std::endl;
  os << "PRF: " << +m_prf << std::endl;
  os << "DTSN: " << +m_dtsn << std::endl;
  os << "Flags: " << +m_flags << std::endl;
  m_dodagId.Print (os);
}



// ---------------- RPL DAO Message -------------------------------

DaoHeader::DaoHeader (uint8_t rplInstanceId, 
  uint8_t k,
  uint8_t d,
  uint8_t flags,
  uint8_t reserved,
  uint8_t daoSequence,
  Ipv6Address dodagId)
  :
  m_rplInstanceId (rplInstanceId),
  m_k (k),
  m_d (d),
  m_flags (flags),
  m_reserved (reserved),
  m_daoSequence (daoSequence),
  m_dodagId (dodagId)
{
  NS_ABORT_MSG_IF(k > 1, "Size of field is larger than allowed");
  NS_ABORT_MSG_IF(d > 1, "Size of field is larger than allowed");
  NS_ABORT_MSG_IF(flags > 0b111111, "Size of field is larger than allowed");
}

TypeId DaoHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::rpl::DaoHeader")
    .SetParent<Header> ()
    .SetGroupName ("Rpl")
    .AddConstructor<DaoHeader> ()
  ;
  return tid;
}
TypeId DaoHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}
uint32_t DaoHeader::GetSerializedSize () const
{
  if (m_d == 1)
  {
    return 20;
  }else{
    return 4;
  }
}
void DaoHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  i.WriteU8 (m_rplInstanceId);
  i.WriteU8 ( (m_k << 7) | (m_d << 6) | m_flags );
  i.WriteU8 (m_reserved);
  i.WriteU8 (m_daoSequence);
  if (m_d == 1)
  {
    uint8_t buf[16];
    m_dodagId.Serialize (buf);
    i.Write (buf, 16);
  }
  
}
uint32_t DaoHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  uint8_t tmp;
  m_rplInstanceId = i.ReadU8 ();
  tmp = i.ReadU8 ();
  m_k = (tmp & 0b10000000) >> 7;
  m_d = (tmp & 0b01000000) >> 6;
  m_flags = (tmp & 0b00111111);
  m_reserved = i.ReadU8 ();
  m_daoSequence = i.ReadU8 ();
  if (m_d == 1)
  {
    uint8_t buf[16];
    i.Read (buf, 16);
    m_dodagId = Ipv6Address(buf);
  }

  uint32_t dist = i.GetDistanceFrom (start);
  NS_ASSERT (dist == GetSerializedSize ());
  return dist;
}
void DaoHeader::Print (std::ostream &os) const
{
  os << "RPL Instance ID: " << +m_rplInstanceId << std::endl;
  os << "K: " << +m_k << std::endl;
  os << "D: " << +m_d << std::endl;
  os << "Flags: " << +m_flags << std::endl;
  os << "DAO Sequence: " << +m_daoSequence << std::endl;
  if (m_d == 1)
  {
    m_dodagId.Print (os);
  }
}

// ---------------- RPL DAO ACK Message -------------------------------

DaoAckHeader::DaoAckHeader (uint8_t rplInstanceId, 
  uint8_t d,
  uint8_t reserved,
  uint8_t daoSequence,
  uint8_t status,
  Ipv6Address dodagId)
  :
  m_rplInstanceId (rplInstanceId),
  m_d (d),
  m_reserved (reserved),
  m_daoSequence (daoSequence),
  m_status (status),
  m_dodagId (dodagId)
{
  NS_ABORT_MSG_IF(d > 1, "Size of field is larger than allowed");
  NS_ABORT_MSG_IF(reserved > 0b1111111, "Size of field is larger than allowed");
}

TypeId DaoAckHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::rpl::DaoAckHeader")
    .SetParent<Header> ()
    .SetGroupName ("Rpl")
    .AddConstructor<DaoAckHeader> ()
  ;
  return tid;
}
TypeId DaoAckHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}
uint32_t DaoAckHeader::GetSerializedSize () const
{
  if (m_d == 1)
  {
    return 20;
  }else{
    return 4;
  }
  
}
void DaoAckHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  i.WriteU8 (m_rplInstanceId);
  i.WriteU8 ( (m_d << 7) | m_reserved);
  i.WriteU8 (m_daoSequence);
  i.WriteU8 (m_status);
  if (m_d == 1)
  {
    uint8_t buf[16];
    m_dodagId.Serialize (buf);
    i.Write (buf, 16);
  }
}
uint32_t DaoAckHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  uint8_t tmp;
  m_rplInstanceId = i.ReadU8 ();
  tmp = i.ReadU8 ();
  m_d = (tmp & 0b10000000) >> 7;
  m_reserved = (tmp & 0b01111111);
  m_daoSequence = i.ReadU8 ();
  m_status = i.ReadU8 ();
  if (m_d == 1)
  {
    uint8_t buf[16];
    i.Read (buf, 16);
    m_dodagId = Ipv6Address(buf);
  }

  uint32_t dist = i.GetDistanceFrom (start);
  NS_ASSERT (dist == GetSerializedSize ());
  return dist;
}
void DaoAckHeader::Print (std::ostream &os) const
{
  os << "RPL Instance ID: " << +m_rplInstanceId << std::endl;
  os << "D: " << +m_d << std::endl;
  os << "DAO Sequence: " << +m_daoSequence << std::endl;
  os << "Status: " << +m_status << std::endl;
  if (m_d == 1)
  {
    m_dodagId.Print (os);
  }
}
// ---------------- RPL CC Message -------------------------------

CcHeader::CcHeader (uint8_t rplInstanceId, 
  uint8_t r,
  uint8_t flags,
  uint16_t ccNonce,
  Ipv6Address dodagId,
  uint32_t destinationCounter)
  :
  m_rplInstanceId (rplInstanceId),
  m_r (r),
  m_flags (flags),
  m_ccNonce (ccNonce),
  m_dodagId (dodagId),
  m_destinationCounter (destinationCounter)
{
  NS_ABORT_MSG_IF(r > 1, "Size of field is larger than allowed");
  NS_ABORT_MSG_IF(flags > 0b1111111, "Size of field is larger than allowed");
}

TypeId CcHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::rpl::CcHeader")
    .SetParent<Header> ()
    .SetGroupName ("Rpl")
    .AddConstructor<CcHeader> ()
  ;
  return tid;
}
TypeId CcHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}
uint32_t CcHeader::GetSerializedSize () const
{
  return 24;
}
void CcHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  uint8_t buf[16];
  i.WriteU8 (m_rplInstanceId);
  i.WriteU8 ( (m_r << 7) | m_flags);
  i.WriteHtonU16 (m_ccNonce);
  m_dodagId.Serialize (buf);
  i.Write (buf, 16);
  i.WriteHtonU32 (m_destinationCounter);

}
uint32_t CcHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  uint8_t buf[16];
  uint8_t tmp;
  m_rplInstanceId = i.ReadU8 ();
  tmp = i.ReadU8 ();
  m_r = (tmp & 0b10000000) >> 7;
  m_flags = (tmp & 0b01111111);
  m_ccNonce = i.ReadNtohU16 ();

  i.Read (buf, 16);
  m_dodagId = Ipv6Address(buf);

  m_destinationCounter = i.ReadNtohU32 ();

  uint32_t dist = i.GetDistanceFrom (start);
  NS_ASSERT (dist == GetSerializedSize ());
  return dist;
}
void CcHeader::Print (std::ostream &os) const
{
  os << "RPL Instance ID: " << +m_rplInstanceId << std::endl;
  os << "R: " << +m_r << std::endl;
  os << "Flags: " << +m_flags << std::endl;
  os << "CC Nounce: " << +m_ccNonce << std::endl;
  m_dodagId.Print (os);
  os << "Destination Counter: " << +m_destinationCounter << std::endl;
}


}
}