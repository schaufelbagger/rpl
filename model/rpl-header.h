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


enum RplPacketCode_e : uint8_t {TYPE_DIS=0, TYPE_DIO=1, TYPE_DAO=2, TYPE_DAO_ACK=3, TYPE_CC=138};

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

class RplIcmpv6Header : public Icmpv6Header
{
public:
  /**
   * \brief RPL message types
   */

  RplIcmpv6Header();
  RplIcmpv6Header(RplPacketCode_e code);
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
class DisHeader : public Header
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
  TypeId GetInstanceTypeId () const;
  uint32_t GetSerializedSize () const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);
  void Print (std::ostream &os) const;

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
class DioHeader : public Header
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
   * \param dodagId 128-bit IPv6 address set by a DODAG root that uniquely identifies a DODAG
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
  Ipv6Address dodagId = Ipv6Address());
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId ();
  TypeId GetInstanceTypeId () const;
  uint32_t GetSerializedSize () const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);
  void Print (std::ostream &os) const;

  /**
   * \brief Set the Rpl Instance Id
   * \param rplInstanceId the Rpl Instance Id
   */
  void SetRplInstanceId (uint8_t rplInstanceId)
  {
    m_rplInstanceId = rplInstanceId;
  }
  /**
   * \brief Get the Rpl Instance Id
   * \return the Rpl Instance Id
   */
  uint8_t GetRplInstanceId ()
  {
    return m_rplInstanceId;
  }
  /**
   * \brief Set the Version Number
   * \param versionNumber the Version Number
   */
  void SetVersionNumber (uint8_t versionNumber)
  {
    m_versionNumber = versionNumber;
  }
  /**
   * \brief Get the Version Number
   * \return the Version Number
   */
  uint8_t GetVersionNumber ()
  {
    return m_versionNumber;
  }
  /**
   * \brief Set the Rank
   * \param rank the Rank
   */
  void SetRank (uint16_t rank)
  {
    m_rank = rank;
  }
  /**
   * \brief Get the Rank
   * \return the Rank
   */
  uint16_t GetRank ()
  {
    return m_rank;
  }
  /**
   * \brief Set the Grounded
   * \param grounded the Grounded
   */
  void SetGrounded (uint8_t grounded)
  {
    NS_ABORT_MSG_IF(grounded > 1, "Size of field is larger than allowed");
    m_grounded = grounded;
  }
  /**
   * \brief Get the Grounded
   * \return the Grounded
   */
  uint8_t GetGrounded ()
  {
    return m_grounded;
  }
  /**
   * \brief Set the Mop
   * \param mop the Mop
   */
  void SetMop (uint8_t mop)
  {
    NS_ABORT_MSG_IF(mop > 0b111, "Size of field is larger than allowed");
    m_mop = mop;
  }
  /**
   * \brief Get the Mop
   * \return the Mop
   */
  uint8_t GetMop ()
  {
    return m_mop;
  }
  /**
   * \brief Set the Prf
   * \param prf the Prf
   */
  void SetPrf (uint8_t prf)
  {
    NS_ABORT_MSG_IF(prf > 0b111, "Size of field is larger than allowed");
    m_prf = prf;
  }
  /**
   * \brief Get the Prf
   * \return the Prf
   */
  uint8_t GetPrf ()
  {
    return m_prf;
  }
  /**
   * \brief Set the Dtsn
   * \param dtsn the Dtsn
   */
  void SetDtsn (uint8_t dtsn)
  {
    m_dtsn = dtsn;
  }
  /**
   * \brief Get the Dtsn
   * \return the Dtsn
   */
  uint8_t GetDtsn ()
  {
    return m_dtsn;
  }
  /**
   * \brief Set the Flags
   * \param flags the Flags
   */
  void SetFlags (uint8_t flags)
  {
    m_flags = flags;
  }
  /**
   * \brief Get the Flags
   * \return the Flags
   */
  uint8_t GetFlags ()
  {
    return m_flags;
  }
  /**
   * \brief Set the Reserved
   * \param reserved the Reserved
   */
  void SetReserved (uint8_t reserved)
  {
    m_reserved = reserved;
  }
  /**
   * \brief Get the Reserved
   * \return the Reserved
   */
  uint8_t GetReserved ()
  {
    return m_reserved;
  }
  /**
   * \brief Set the Dodag Id
   * \param dodagId the Dodag Id
   */
  void SetDodagId (Ipv6Address dodagId)
  {
    m_dodagId = dodagId;
  }
  /**
   * \brief Get the Dodag Id
   * \return the Dodag Id
   */
  Ipv6Address GetDodagId ()
  {
    return m_dodagId;
  }
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
  Ipv6Address m_dodagId;
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
class DaoHeader : public Header
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
   * \param dodagId 128-bit IPv6 address set by a DODAG root that uniquely identifies a DODAG
   */
  DaoHeader (uint8_t rplInstanceId = 0, 
  uint8_t k = 0,
  uint8_t d = 0,
  uint8_t flags = 0,
  uint8_t reserved = 0,
  uint8_t daoSequence = 0,
  Ipv6Address dodagId = Ipv6Address());
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId ();
  TypeId GetInstanceTypeId () const;
  uint32_t GetSerializedSize () const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);
  void Print (std::ostream &os) const;
  /**
   * \brief Set the Rpl Instance Id
   * \param rplInstanceId the Rpl Instance Id
   */
  void SetRplInstanceId (uint8_t rplInstanceId)
  {
    m_rplInstanceId = rplInstanceId;
  }
  /**
   * \brief Get the Rpl Instance Id
   * \return the Rpl Instance Id
   */
  uint8_t GetRplInstanceId ()
  {
    return m_rplInstanceId;
  }
  /**
   * \brief Set the K
   * \param k the K
   */
  void SetK (uint8_t k)
  {
    NS_ABORT_MSG_IF(k > 1, "Size of field is larger than allowed");
    m_k = k;
  }
  /**
   * \brief Get the K
   * \return the K
   */
  uint8_t GetK ()
  {
    return m_k;
  }
  /**
   * \brief Set the D
   * \param d the D
   */
  void SetD (uint8_t d)
  {
    NS_ABORT_MSG_IF(d > 1, "Size of field is larger than allowed");
    m_d = d;
  }
  /**
   * \brief Get the D
   * \return the D
   */
  uint8_t GetD ()
  {
    return m_d;
  }
  /**
   * \brief Set the Flags
   * \param flags the Flags
   */
  void SetFlags (uint8_t flags)
  {
    NS_ABORT_MSG_IF(flags > 0b111111, "Size of field is larger than allowed");
    m_flags = flags;
  }
  /**
   * \brief Get the Flags
   * \return the Flags
   */
  uint8_t GetFlags ()
  {
    return m_flags;
  }
  /**
   * \brief Set the Reserved
   * \param reserved the Reserved
   */
  void SetReserved (uint8_t reserved)
  {
    m_reserved = reserved;
  }
  /**
   * \brief Get the Reserved
   * \return the Reserved
   */
  uint8_t GetReserved ()
  {
    return m_reserved;
  }
  /**
   * \brief Set the Dao Sequence
   * \param daoSequence the Dao Sequence
   */
  void SetDaoSequence (uint8_t daoSequence)
  {
    m_daoSequence = daoSequence;
  }
  /**
   * \brief Get the Dao Sequence
   * \return the Dao Sequence
   */
  uint8_t GetDaoSequence ()
  {
    return m_daoSequence;
  }
  /**
   * \brief Set the Dodag Id
   * \param dodagId the Dodag Id
   */
  void SetDodagId (Ipv6Address dodagId)
  {
    m_dodagId = dodagId;
  }
  /**
   * \brief Get the Dodag Id
   * \return the Dodag Id
   */
  Ipv6Address GetDodagId ()
  {
    return m_dodagId;
  }
private:
  uint8_t m_rplInstanceId;
  uint8_t m_k : 1;
  uint8_t m_d : 1;
  uint8_t m_flags : 6;
  uint8_t m_reserved;
  uint8_t m_daoSequence;
  Ipv6Address m_dodagId;
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
class DaoAckHeader : public Header
{
public:
  /**
   * constructor
   * \param rplInstanceId the rpl instance ID(0)
   * \param d
   * \param reserved the reserved bits (0)
   * \param daoSequence
   * \param status
   * \param dodagId 128-bit IPv6 address set by a DODAG root that uniquely identifies a DODAG
   */
  DaoAckHeader (uint8_t rplInstanceId = 0, 
  uint8_t d = 0,
  uint8_t reserved = 0,
  uint8_t daoSequence = 0,
  uint8_t status = 0,
  Ipv6Address dodagId = Ipv6Address());
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId ();
  TypeId GetInstanceTypeId () const;
  uint32_t GetSerializedSize () const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);
  void Print (std::ostream &os) const;

  /**
   * \brief Set the Rpl Instance Id
   * \param rplInstanceId the Rpl Instance Id
   */
  void SetRplInstanceId (uint8_t rplInstanceId)
  {
    m_rplInstanceId = rplInstanceId;
  }
  /**
   * \brief Get the Rpl Instance Id
   * \return the Rpl Instance Id
   */
  uint8_t GetRplInstanceId ()
  {
    return m_rplInstanceId;
  }
  /**
   * \brief Set the D
   * \param d the D
   */
  void SetD (uint8_t d)
  {
    NS_ABORT_MSG_IF(d > 1, "Size of field is larger than allowed");
    m_d = d;
  }
  /**
   * \brief Get the D
   * \return the D
   */
  uint8_t GetD ()
  {
    return m_d;
  }
  /**
   * \brief Set the Reserved
   * \param reserved the Reserved
   */
  void SetReserved (uint8_t reserved)
  {
    NS_ABORT_MSG_IF(reserved > 0b1111111, "Size of field is larger than allowed");
    m_reserved = reserved;
  }
  /**
   * \brief Get the Reserved
   * \return the Reserved
   */
  uint8_t GetReserved ()
  {
    return m_reserved;
  }
  /**
   * \brief Set the Dao Sequence
   * \param daoSequence the Dao Sequence
   */
  void SetDaoSequence (uint8_t daoSequence)
  {
    m_daoSequence = daoSequence;
  }
  /**
   * \brief Get the Dao Sequence
   * \return the Dao Sequence
   */
  uint8_t GetDaoSequence ()
  {
    return m_daoSequence;
  }
  /**
   * \brief Set the Status
   * \param status the Status
   */
  void SetStatus (uint8_t status)
  {
    m_status = status;
  }
  /**
   * \brief Get the Status
   * \return the Status
   */
  uint8_t GetStatus ()
  {
    return m_status;
  }
  /**
   * \brief Set the Dodag Id
   * \param dodagId the Dodag Id
   */
  void SetDodagId (Ipv6Address dodagId)
  {
    m_dodagId = dodagId;
  }
  /**
   * \brief Get the Dodag Id
   * \return the Dodag Id
   */
  Ipv6Address GetDodagId ()
  {
    return m_dodagId;
  }
private:
  uint8_t m_rplInstanceId;
  uint8_t m_d : 1;
  uint8_t m_reserved : 7;
  uint8_t m_daoSequence;
  uint8_t m_status;
  Ipv6Address m_dodagId;
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
class CcHeader : public Header
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
   * \param dodagId 128-bit IPv6 address set by a DODAG root that uniquely identifies a DODAG
   * \param destinationCounter 32-bit unsigned integer value indicating the
   *     sender's estimate of the destination's current security counter
   *     value.  If the sender does not have an estimate, it SHOULD set
   *     the Destination Counter field to zero.
   */
  CcHeader (uint8_t rplInstanceId = 0, 
  uint8_t r = 0,
  uint8_t flags = 0,
  uint16_t ccNonce = 0,
  Ipv6Address dodagId = Ipv6Address(),
  uint32_t destinationCounter = 0);
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId ();
  TypeId GetInstanceTypeId () const;
  uint32_t GetSerializedSize () const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);
  void Print (std::ostream &os) const;

  /**
   * \brief Set the Rpl Instance Id
   * \param rplInstanceId the Rpl Instance Id
   */
  void SetRplInstanceId (uint8_t rplInstanceId)
  {
    m_rplInstanceId = rplInstanceId;
  }
  /**
   * \brief Get the Rpl Instance Id
   * \return the Rpl Instance Id
   */
  uint8_t GetRplInstanceId ()
  {
    return m_rplInstanceId;
  }
  /**
   * \brief Set the R
   * \param r the R
   */
  void SetR (uint8_t r)
  {
    NS_ABORT_MSG_IF(r > 1, "Size of field is larger than allowed");
    m_r = r;
  }
  /**
   * \brief Get the R
   * \return the R
   */
  uint8_t GetR ()
  {
    return m_r;
  }
  /**
   * \brief Set the Flags
   * \param flags the Flags
   */
  void SetFlags (uint8_t flags)
  {
    NS_ABORT_MSG_IF(flags > 0b1111111, "Size of field is larger than allowed");
    m_flags = flags;
  }
  /**
   * \brief Get the Flags
   * \return the Flags
   */
  uint8_t GetFlags ()
  {
    return m_flags;
  }
  /**
   * \brief Set the Cc Nonce
   * \param ccNonce the Cc Nonce
   */
  void SetCcNonce (uint16_t ccNonce)
  {
    m_ccNonce = ccNonce;
  }
  /**
   * \brief Get the Cc Nonce
   * \return the Cc Nonce
   */
  uint16_t GetCcNonce ()
  {
    return m_ccNonce;
  }
  /**
   * \brief Set the Dodag Id
   * \param dodagId the Dodag Id
   */
  void SetDodagId (Ipv6Address dodagId)
  {
    m_dodagId = dodagId;
  }
  /**
   * \brief Get the Dodag Id
   * \return the Dodag Id
   */
  Ipv6Address GetDodagId ()
  {
    return m_dodagId;
  }
  /**
   * \brief Set the Destination Counter
   * \param destinationCounter the Destination Counter
   */
  void SetDestinationCounter (uint32_t destinationCounter)
  {
    m_destinationCounter = destinationCounter;
  }
  /**
   * \brief Get the Destination Counter
   * \return the Destination Counter
   */
  uint32_t GetDestinationCounter ()
  {
    return m_destinationCounter;
  }

private:
  uint8_t m_rplInstanceId;
  uint8_t m_r : 1;
  uint8_t m_flags : 7;
  uint16_t m_ccNonce;
  Ipv6Address m_dodagId;
  uint32_t m_destinationCounter;
};




}
}
#endif /* RPL_HEADER_H */