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
#ifndef RPL_HEADER_H
#define RPL_HEADER_H

#include "ns3/header.h"
#include "ns3/object.h"
#include "ns3/icmpv6-header.h"

#include "rpl-header-option.h"

#include <bitset>


namespace ns3 {
namespace rpl {




//
// RPL packets
//


/**
 * \ingroup rpl
 * \brief   RPL ICMPv6 header with type set to 155
  \verbatim
   0                   1                   2                   3
   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |     Type      |     Code      |          Checksum             |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |                                                               |
  .                             Base                              .
  .                                                               .
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |                                                               |
  .                           Option(s)                           .
  .                                                               .
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  \endverbatim
 */

class RplHeader : public Icmpv6Header
{
public:
  /**
   * \brief RPL message types
   */
  enum RplPacketCode_e : uint8_t {TYPE_DIS=0, TYPE_DIO=1, TYPE_DAO=2, TYPE_DAO_ACK=3, TYPE_CONS_CHECK=138} RplPacketCode_t;
  RplHeader();
  static TypeId GetTypeId ();
};



/**
 * \ingroup rpl
 * \brief   RPL DODAG Information Solicitation (DIS)
  \verbatim
   0                   1                   2
   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |     Flags     |   Reserved    |   Option(s)...
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  \endverbatim
 */
class DisHeader : public RplHeader
{
public:
  /**
   * constructor
   *
   * \param flags the message flags (0)
   * \param reserved the reserved bits (0)
   * \param options the options bits (0)
   */
  DisHeader (uint8_t flags = 0, uint8_t reserved = 0, std::vector<RplHeaderOption> options = {});
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId ();
  uint8_t GetFlags();
  void SetFlags(uint8_t flags);
private:
  uint8_t m_flags;
  uint8_t m_reserved;
  std::vector<RplHeaderOption> m_options;
};

/**
 * \ingroup rpl
 * \brief   RPL DODAG Information Object (DIO)
  \verbatim
   0                   1                   2                   3
   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  | RPLInstanceID |Version Number |             Rank              |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |G|0| MOP | Prf |     DTSN      |     Flags     |   Reserved    |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |                                                               |
  +                                                               +
  |                                                               |
  +                            DODAGID                            +
  |                                                               |
  +                                                               +
  |                                                               |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |   Option(s)...
  +-+-+-+-+-+-+-+-+
  \endverbatim
 */
class DioHeader : public RplHeader
{
public:
  /**
   * constructor
   *
   * \param rplInstanceId the rpl instance ID(0)
   * \param versionNumber the rpl version number(0)
   * \param rank the node rank (0)
   * \param grounded Grounded flag (0)
   * \param mop Mode of Operation (0)
   * \param prf DODAGPreference (0)
   * \param dtsn Destination Advertisement Trigger Sequence Number (0)
   * \param flags the message flags (0)
   * \param reserved the reserved bits (0)
   * \param dodagid 128-bit IPv6 address set by a DODAG root that uniquely identifies a DODAG
   * \param options the options bits (0)
   */
  DioHeader (uint8_t rplInstanceId = 0, 
  uint8_t versionNumber = 0,
  uint16_t rank = 0,
  uint8_t grounded = 0,
  uint8_t mop = 0,
  uint8_t prf = 0,
  uint8_t dtsn = 0,
  uint8_t flags = 0,
  uint8_t reserved = 0,
  Ipv6Address dodagid = Ipv6Address(),
  std::vector<RplHeaderOption> options = {});
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId ();
private:
  uint8_t m_rplInstanceId;
  uint8_t m_versionNumber;
  uint16_t m_rank;
  uint8_t m_grounded : 1;
  uint8_t m_mop : 3;
  uint8_t m_prf : 3;
  uint8_t m_dtsn;
  uint8_t m_flags;
  uint8_t m_reserved;
  Ipv6Address m_dodagid;
  std::vector<RplHeaderOption> m_options;
};


/**
 * \ingroup rpl 
 * \brief RPL Destination Advertisement Object (DAO)
  \verbatim 
   0                   1                   2                   3
   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  | RPLInstanceID |K|D|   Flags   |   Reserved    | DAOSequence   |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |                                                               |
  +                                                               +
  |                                                               |
  +                            DODAGID*                           +
  |                                                               |
  +                                                               +
  |                                                               |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |   Option(s)...
  +-+-+-+-+-+-+-+-+
  \endverbatim
 */
class DaoHeader : public RplHeader
{
public:
  /**
   * constructor
   * \param rplInstanceId the rpl instance ID(0)
   * \param k
   * \param d
   * \param flags the message flags (0)
   * \param reserved the reserved bits (0)
   * \param daoSequence
   * \param dodagid 128-bit IPv6 address set by a DODAG root that uniquely identifies a DODAG
   * \param options the options bits (0)
   */
  DaoHeader (uint8_t rplInstanceId = 0, 
  uint8_t k = 0,
  uint8_t d = 0,
  uint8_t flags = 0,
  uint8_t reserved = 0,
  uint8_t daoSequence = 0,
  Ipv6Address dodagid = Ipv6Address(),
  std::vector<RplHeaderOption> options = {});
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId ();
private:
  uint8_t m_rplInstanceId;
  uint8_t m_k : 1;
  uint8_t m_d : 1;
  uint8_t m_flags : 6;
  uint8_t m_reserved;
  uint8_t m_daoSequence;
  Ipv6Address m_dodagid;
  std::vector<RplHeaderOption> m_options;
};

/**
 * \ingroup rpl
 * \brief Destination Advertisement Object Acknowledgement (DAO-ACK)
  \verbatim 
   0                   1                   2                   3
   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  | RPLInstanceID |D|  Reserved   |  DAOSequence  |    Status     |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |                                                               |
  +                                                               +
  |                                                               |
  +                            DODAGID*                           +
  |                                                               |
  +                                                               +
  |                                                               |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |   Option(s)...
  +-+-+-+-+-+-+-+-+
  \endverbatim
 * 
 */
class DaoAckHeader : public RplHeader
{
public:
  /**
   * constructor
   * \param rplInstanceId the rpl instance ID(0)
   * \param d
   * \param reserved the reserved bits (0)
   * \param daoSequence
   * \param status
   * \param dodagid 128-bit IPv6 address set by a DODAG root that uniquely identifies a DODAG
   * \param options the options bits (0)
   */
  DaoAckHeader (uint8_t rplInstanceId = 0, 
  uint8_t d = 0,
  uint8_t reserved = 0,
  uint8_t daoSequence = 0,
  uint8_t status = 0,
  Ipv6Address dodagid = Ipv6Address(),
  std::vector<RplHeaderOption> options = {});
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId ();
private:
  uint8_t m_rplInstanceId;
  uint8_t m_d : 1;
  uint8_t m_reserved : 7;
  uint8_t m_daoSequence;
  uint8_t m_status;
  Ipv6Address m_dodagid;
  std::vector<RplHeaderOption> m_options;
};

/**
 * \ingroup rpl 
 * \brief Consistency Check (CC)
  \verbatim 
   0                   1                   2                   3
   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  | RPLInstanceID |R|    Flags    |           CC Nonce            |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |                                                               |
  +                                                               +
  |                                                               |
  +                            DODAGID                            +
  |                                                               |
  +                                                               +
  |                                                               |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |                      Destination Counter                      |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |   Option(s)...
  +-+-+-+-+-+-+-+-+
  \endverbatim
 */
class CcHeader : public RplHeader
{
public:
  /**
   * constructor
   * \param rplInstanceId the rpl instance ID(0)
   * \param r The 'R' flag indicates whether the CC message is a response.
   * \param flags the reserved bits (0)
   * \param ccNonce 16-bit unsigned integer set by a CC request.  The
   *     corresponding CC response includes the same CC nonce value as
   *     the request.
   * \param dodagid 128-bit IPv6 address set by a DODAG root that uniquely identifies a DODAG
   * \param destinationCounter 32-bit unsigned integer value indicating the
   *     sender's estimate of the destination's current security counter
   *     value.  If the sender does not have an estimate, it SHOULD set
   *     the Destination Counter field to zero.
   * \param options the options bits (0)
   */
  CcHeader (uint8_t rplInstanceId = 0, 
  uint8_t r = 0,
  uint8_t flags = 0,
  uint16_t ccNonce = 0,
  Ipv6Address dodagid = Ipv6Address(),
  uint32_t destinationCounter = 0,
  std::vector<RplHeaderOption> options = {});
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId ();
private:
  uint8_t m_rplInstanceId;
  uint8_t m_r : 1;
  uint8_t m_flags : 7;
  uint16_t m_ccNonce;
  Ipv6Address m_dodagid;
  uint32_t m_destinationCounter;
  std::vector<RplHeaderOption> m_options;
};





}
}
#endif /* RPL_HEADER_H */