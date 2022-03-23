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

RplHeader::RplHeader () : Icmpv6Header()
{
  this->SetType (uint8_t (155));
  NS_LOG_FUNCTION (this);
}

TypeId RplHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::rpl::RplHeader")
    .SetParent<Icmpv6Header> ()
    .SetGroupName ("Rpl")
    .AddConstructor<RplHeader> ()
  ;
  return tid;
}



DisHeader::DisHeader (uint8_t flags, uint8_t reserved, std::vector<RplHeaderOption> options) : RplHeader(), 
  m_flags (flags),
  m_reserved (reserved),
  m_options (options)
{
  this->SetCode (uint8_t (RplHeader::TYPE_DIS));
}

TypeId DisHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::rpl::DisHeader")
    .SetParent<RplHeader> ()
    .SetGroupName ("Rpl")
    .AddConstructor<DisHeader> ()
  ;
  return tid;
}

uint8_t DisHeader::GetFlags ()
{
  return this->m_flags;
}

void DisHeader::SetFlags (uint8_t flags)
{
  this->m_flags = flags;
}


DioHeader::DioHeader (uint8_t rplInstanceId, 
  uint8_t versionNumber,
  uint16_t rank,
  uint8_t grounded,
  uint8_t mop,
  uint8_t prf,
  uint8_t dtsn,
  uint8_t flags,
  uint8_t reserved,
  Ipv6Address dodagid, 
  std::vector<RplHeaderOption> options)
  : 
  RplHeader(),
  m_rplInstanceId (rplInstanceId),
  m_versionNumber (versionNumber),
  m_rank (rank),
  m_grounded (grounded),
  m_mop (mop),
  m_prf (prf),
  m_dtsn (dtsn),
  m_flags (flags),
  m_reserved (reserved),
  m_dodagid (dodagid),
  m_options (options)
{
  this->SetCode (uint8_t (RplHeader::TYPE_DIO));
}

TypeId DioHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::rpl::DioHeader")
    .SetParent<RplHeader> ()
    .SetGroupName ("Rpl")
    .AddConstructor<DioHeader> ()
  ;
  return tid;
}

DaoHeader::DaoHeader (uint8_t rplInstanceId, 
  uint8_t k,
  uint8_t d,
  uint8_t flags,
  uint8_t reserved,
  uint8_t daoSequence,
  Ipv6Address dodagid,
  std::vector<RplHeaderOption> options)
  :
  RplHeader(),
  m_rplInstanceId (rplInstanceId),
  m_k (k),
  m_d (d),
  m_flags (flags),
  m_reserved (reserved),
  m_daoSequence (daoSequence),
  m_dodagid (dodagid),
  m_options (options)
{
  this->SetCode (uint8_t (RplHeader::TYPE_DAO));
}

TypeId DaoHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::rpl::DaoHeader")
    .SetParent<RplHeader> ()
    .SetGroupName ("Rpl")
    .AddConstructor<DaoHeader> ()
  ;
  return tid;
}

DaoAckHeader::DaoAckHeader (uint8_t rplInstanceId, 
  uint8_t d,
  uint8_t reserved,
  uint8_t daoSequence,
  uint8_t status,
  Ipv6Address dodagid,
  std::vector<RplHeaderOption> options)
  :
  RplHeader(),
  m_rplInstanceId (rplInstanceId),
  m_d (d),
  m_reserved (reserved),
  m_daoSequence (daoSequence),
  m_status (status),
  m_dodagid (dodagid),
  m_options (options)
{
  this->SetCode (uint8_t (RplHeader::TYPE_DAO_ACK));
}

TypeId DaoAckHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::rpl::DaoAckHeader")
    .SetParent<RplHeader> ()
    .SetGroupName ("Rpl")
    .AddConstructor<DaoAckHeader> ()
  ;
  return tid;
}

CcHeader::CcHeader (uint8_t rplInstanceId, 
  uint8_t r,
  uint8_t flags,
  uint16_t ccNonce,
  Ipv6Address dodagid,
  uint32_t destinationCounter,
  std::vector<RplHeaderOption> options)
  :
  RplHeader(),
  m_rplInstanceId (rplInstanceId),
  m_r (r),
  m_flags (flags),
  m_ccNonce (ccNonce),
  m_dodagid (dodagid),
  m_destinationCounter (destinationCounter),
  m_options (options)
{
  this->SetCode (uint8_t (RplHeader::TYPE_CONS_CHECK));
}

TypeId CcHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::rpl::CcHeader")
    .SetParent<RplHeader> ()
    .SetGroupName ("Rpl")
    .AddConstructor<CcHeader> ()
  ;
  return tid;
}


}
}