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
#ifndef RPL_HEADER_OPTIONS_H
#define RPL_HEADER_OPTIONS_H

#include "ns3/object.h"
#include "ns3/icmpv6-header.h"

#include <bitset>

namespace ns3 {
namespace rpl {

#define DEFAULT_PATH_CONTROL_SIZE 0

enum OptionType_e : uint8_t 
{
  OPTION_TYPE_PAD1=0,
  OPTION_TYPE_PADN=1,
  OPTION_TYPE_DAG_METRIC_CONTAINER=2,
  OPTION_TYPE_ROUTING_INFORMATION=3,
  OPTION_TYPE_DODAG_CONFIGURATION=4,
  OPTION_TYPE_RPL_TARGET=5,
  OPTION_TYPE_TRANSIT_INFORMATION=6,
  OPTION_TYPE_SOLICITED_INFORMATION=7,
  OPTION_TYPE_PREFIX_INFORMATION=8,
  OPTION_TYPE_RPL_TARGET_DESCRIPTOR=9
};

/**
 * \ingroup rpl
 * \brief   RPL header options field
 */
class RplHeaderOption : public Header
{
public:
  /**
   * constructor
   *
   * \param type the option type (0)
   * \param length the length of the data (0)
   */
  RplHeaderOption (uint8_t type = 0, uint8_t optionLength = 0, std::vector<uint8_t> data = {});

  uint8_t GetType();
  uint8_t GetOptionLength();

  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  TypeId GetInstanceTypeId () const;
  void Print ();
  void Print (std::ostream &os) const;
  uint32_t GetSerializedSize (void) const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);

  

  void SetPad1 ();

  /**
   * PADN option
   \verbatim 
     0                   1                   2
     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- - - - - - - -
    |   Type = 0x01 | Option Length | 0x00 Padding...
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- - - - - - - -
   \endverbatim
   */
  struct PadN
  {
    std::vector<uint8_t> padding;
    /**
     * This method is used to print the content of a PadN option.
     * \param os output stream
     */
    void Print (std::ostream &os) const;
    /**
     * Returns the expected size of the header.
     * \returns the expected size of the header.
     */
    uint32_t GetSerializedSize (void) const;
    /**
     * This method is used by Packet::AddHeader to
     * store a header into the byte buffer of a packet.
     *
     * \param start an iterator which points to where the header should
     *        be written.
     */
    void Serialize (Buffer::Iterator start) const;
    /**
     * This method is used by Packet::RemoveHeader to
     * re-create a header from the byte buffer of a packet.
     *
     * \param start an iterator which points to where the header should
     *        read from.
     * \returns the number of bytes read.
     */
    uint32_t Deserialize (Buffer::Iterator start);
  };
  /**
   * \brief Set RPL Header Options to PadN
   * 
   * \param optionLength length of the padding, valid from 0 to 5
   */
  void SetPadN (uint8_t optionLength);

  /**
   * DAG Metric Container option
   *
   \verbatim 
     0                   1                   2
     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- - - - - - - -
    |   Type = 0x02 | Option Length | Metric Data
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- - - - - - - -
   \endverbatim
   */
  struct DagMetricContainer
  {
    std::vector<uint8_t> metricData;
    /**
     * This method is used to print the content of a DagMetricContainer option.
     * \param os output stream
     */
    void Print (std::ostream &os) const;
    /**
     * Returns the expected size of the header.
     * \returns the expected size of the header.
     */
    uint32_t GetSerializedSize (void) const;
    /**
     * This method is used by Packet::AddHeader to
     * store a header into the byte buffer of a packet.
     *
     * \param start an iterator which points to where the header should
     *        be written.
     */
    void Serialize (Buffer::Iterator start) const;
    /**
     * This method is used by Packet::RemoveHeader to
     * re-create a header from the byte buffer of a packet.
     *
     * \param start an iterator which points to where the header should
     *        read from.
     * \returns the number of bytes read.
     */
    uint32_t Deserialize (Buffer::Iterator start, uint8_t optionLength);
  };
  /**
   * \brief Set RPL Header Options to DAG Metric Container
   * 
   * \param optionLength the length of the padding
   * \param metricData the metric data
   */
  void SetDagMetricContainer(uint8_t optionLength, std::vector<uint8_t> metricData);
  void SetDagMetricContainer(std::vector<uint8_t> metricData);

  /**
   * Route Information option
   \verbatim 
     0                   1                   2                   3
     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |   Type = 0x03 | Option Length | Prefix Length |Resvd|Prf|Resvd|
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                        Route Lifetime                         |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                                                               |
    .                   Prefix (Variable Length)                    .
    .                                                               .
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   \endverbatim
   */
  struct RouteInformation
  {
    uint8_t prefixLength;
    uint8_t reserved1 : 3;
    uint8_t prf : 2;
    uint8_t reserved2 : 3;
    uint32_t routeLifetime;
    Ipv6Prefix prefix;
    /**
     * This method is used to print the content of a RouteInformation option.
     * \param os output stream
     */
    void Print (std::ostream &os) const;
    /**
     * Returns the expected size of the header.
     * \returns the expected size of the header.
     */
    uint32_t GetSerializedSize (void) const;
    /**
     * This method is used by Packet::AddHeader to
     * store a header into the byte buffer of a packet.
     *
     * \param start an iterator which points to where the header should
     *        be written.
     */
    void Serialize (Buffer::Iterator start) const;
    /**
     * This method is used by Packet::RemoveHeader to
     * re-create a header from the byte buffer of a packet.
     *
     * \param start an iterator which points to where the header should
     *        read from.
     * \returns the number of bytes read.
     */
    uint32_t Deserialize (Buffer::Iterator start, uint8_t optionLength);
  };
  /**
   * \brief Set RPL Header Options to Route Information
   * 
   * \param prefixLength length of the IPv6 prefix as the number of bits
   * \param prf the Route Preference flag
   * \param routeLifetime the metric data
   * \param prefix the IPv6 prefix
   */
  void SetRouteInformation(uint8_t prefixLength, uint8_t prf, uint32_t routeLifetime, Ipv6Prefix prefix);
  void SetRouteInformation(uint8_t prf, uint32_t routeLifetime, Ipv6Prefix prefix);
  

    /**
   * \ingroup rpl
   * DODAG Configuration Option
   * 
   \verbatim 
      0                   1                   2                   3
      0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |   Type = 0x04 |Opt Length = 14| Flags |A| PCS | DIOIntDoubl.  |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |  DIOIntMin.   |   DIORedun.   |        MaxRankIncrease        |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |      MinHopRankIncrease       |              OCP              |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |   Reserved    | Def. Lifetime |      Lifetime Unit            |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   \endverbatim
   */
  struct DodagConfiguration
  {
    uint8_t flags : 4;
    uint8_t a : 1;
    uint8_t pcs : 3;
    uint8_t dioIntervalDoublings;
    uint8_t dioIntervalMin;
    uint8_t dioRedundancyConstant;
    uint16_t maxRankIncrease;
    uint16_t minHopRankIncrease;
    uint16_t ocp;
    uint8_t reserved;
    uint8_t defaultLifetime;
    uint16_t lifetimeUnit;
    /**
     * This method is used to print the content of a DodagConfiguration option.
     * \param os output stream
     */
    void Print (std::ostream &os) const;
    /**
     * Returns the expected size of the header.
     * \returns the expected size of the header.
     */
    uint32_t GetSerializedSize (void) const;
    /**
     * This method is used by Packet::AddHeader to
     * store a header into the byte buffer of a packet.
     *
     * \param start an iterator which points to where the header should
     *        be written.
     */
    void Serialize (Buffer::Iterator start) const;
    /**
     * This method is used by Packet::RemoveHeader to
     * re-create a header from the byte buffer of a packet.
     *
     * \param start an iterator which points to where the header should
     *        read from.
     * \returns the number of bytes read.
     */
    uint32_t Deserialize (Buffer::Iterator start, uint8_t optionLength);
  };
  /**
   * \brief Set RPL Header Options to DODAG Configuration
   * 
   * \param a the Authentication Enabled flag
   * \param pcs the Path Control Size
   * \param dioIntervalDoublings the DIOIntervalDoublings
   * \param dioIntervalMin the DIOIntervalMin
   * \param dioRedundancyConstant the DIORedundancyConstant
   * \param maxRankIncrease the MaxRankIncrease
   * \param minHopRankIncrease the MinHopRankIncrease
   * \param ocp the Objective Code Point
   * \param defaultLifetime the Default Lifetime
   * \param lifetimeUnit the Lifetime Unit
   */
  void SetDodagConfiguration(uint8_t pcs, uint8_t dioIntervalDoublings, uint8_t dioIntervalMin, uint8_t dioRedundancyConstant, uint16_t maxRankIncrease, uint16_t minHopRankIncrease, uint16_t ocp, uint8_t reserved, uint8_t defaultLifetime, uint16_t lifetimeUnit);
  void SetDodagConfiguration(uint8_t pcs, uint8_t dioIntervalDoublings, uint8_t dioIntervalMin, uint8_t dioRedundancyConstant, uint16_t maxRankIncrease, uint16_t minHopRankIncrease, uint16_t ocp, uint8_t defaultLifetime, uint16_t lifetimeUnit);

  /**
   * \ingroup rpl
   * RPL Target option
   \verbatim
     0                   1                   2                   3
     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |   Type = 0x05 | Option Length |     Flags     | Prefix Length |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                                                               |
    +                                                               +
    |                Target Prefix (Variable Length)                |
    .                                                               .
    .                                                               .
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   \endverbatim
   */
  struct RplTarget
  {
    uint8_t flags;
    uint8_t prefixLength;
    Ipv6Prefix targetPrefix;

    /**
     * This method is used to print the content of a DodagConfiguration option.
     * \param os output stream
     */
    void Print (std::ostream &os) const;
    /**
     * Returns the expected size of the header.
     * \returns the expected size of the header.
     */
    uint32_t GetSerializedSize (void) const;
    /**
     * This method is used by Packet::AddHeader to
     * store a header into the byte buffer of a packet.
     *
     * \param start an iterator which points to where the header should
     *        be written.
     */
    void Serialize (Buffer::Iterator start) const;
    /**
     * This method is used by Packet::RemoveHeader to
     * re-create a header from the byte buffer of a packet.
     *
     * \param start an iterator which points to where the header should
     *        read from.
     * \returns the number of bytes read.
     */
    uint32_t Deserialize (Buffer::Iterator start, uint8_t optionLength);
  };
  /**
   * \brief Set RPL Header Options to RPL Target
   * 
   * \param flags the Route Preference flag
   * \param prefixLength the Prefix Length
   * \param targetPrefix the Target Prefix
   */
  void SetRplTarget (uint8_t flags, uint8_t prefixLength, Ipv6Prefix targetPrefix);
  void SetRplTarget (uint8_t prefixLength, Ipv6Prefix targetPrefix);
  void SetRplTarget (Ipv6Prefix targetPrefix);
  void SetRplTarget (Ipv6Address targetPrefix);

  /**
   * \ingroup rpl
   * Transit Information option
   \verbatim
     0                   1                   2                   3
     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |   Type = 0x06 | Option Length |E|    Flags    | Path Control  |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    | Path Sequence | Path Lifetime |                               |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                               +
    |                                                               |
    +                                                               +
    |                                                               |
    +                        Parent Address*                        +
    |                                                               |
    +                               +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                               |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   \endverbatim
   */
  struct TransitInformation
  {
    uint8_t e : 1;
    uint8_t flags : 7;
    uint8_t pathControl;
    uint8_t pathSequence;
    uint8_t pathLifetime;
    Ipv6Address parentAddress;
    /**
     * This method is used to print the content of a DodagConfiguration option.
     * \param os output stream
     */
    void Print (std::ostream &os) const;
    /**
     * Returns the expected size of the header.
     * \returns the expected size of the header.
     */
    uint32_t GetSerializedSize (void) const;
    /**
     * This method is used by Packet::AddHeader to
     * store a header into the byte buffer of a packet.
     *
     * \param start an iterator which points to where the header should
     *        be written.
     */
    void Serialize (Buffer::Iterator start) const;
    /**
     * This method is used by Packet::RemoveHeader to
     * re-create a header from the byte buffer of a packet.
     *
     * \param start an iterator which points to where the header should
     *        read from.
     * \returns the number of bytes read.
     */
    uint32_t Deserialize (Buffer::Iterator start, uint8_t optionLength);
  };
  /**
   * \brief Set RPL Header Options to Transit Information
   * 
   * \param e the External flag
   * \param flags the flags field
   * \param pathControl the Path Control
   * \param pathSequence the Path Sequence
   * \param pathLifetime the Path Lifetime
   * \param parentAddress the Paranet Address, not necessarily included
   */
  void SetTransitInformation (uint8_t e, uint8_t flags, uint8_t pathControl, uint8_t pathSequence, uint8_t pathLifetime, Ipv6Address parentAddress);
  void SetTransitInformation (uint8_t e, uint8_t pathControl, uint8_t pathSequence, uint8_t pathLifetime, Ipv6Address parentAddress);
  void SetTransitInformation (uint8_t e, uint8_t pathControl, uint8_t pathSequence, uint8_t pathLifetime);

  /**
   * \ingroup rpl
   * Solicited Information option
   \verbatim
     0                   1                   2                   3
     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |   Type = 0x07 |Opt Length = 19| RPLInstanceID |V|I|D|  Flags  |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                                                               |
    +                                                               +
    |                                                               |
    +                            DODAGID                            +
    |                                                               |
    +                                                               +
    |                                                               |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |Version Number |
    +-+-+-+-+-+-+-+-+
   \endverbatim
   */
  struct SolicitedInformation
  {
    uint8_t rplInstanceId;
    uint8_t v : 1;
    uint8_t i : 1;
    uint8_t d : 1;
    uint8_t flags : 5;
    Ipv6Address dodagId;
    uint8_t versionNumber;

    /**
     * This method is used to print the content of a DodagConfiguration option.
     * \param os output stream
     */
    void Print (std::ostream &os) const;
    /**
     * Returns the expected size of the header.
     * \returns the expected size of the header.
     */
    uint32_t GetSerializedSize (void) const;
    /**
     * This method is used by Packet::AddHeader to
     * store a header into the byte buffer of a packet.
     *
     * \param start an iterator which points to where the header should
     *        be written.
     */
    void Serialize (Buffer::Iterator start) const;
    /**
     * This method is used by Packet::RemoveHeader to
     * re-create a header from the byte buffer of a packet.
     *
     * \param start an iterator which points to where the header should
     *        read from.
     * \returns the number of bytes read.
     */
    uint32_t Deserialize (Buffer::Iterator start, uint8_t optionLength);
  };
  /**
   * \brief Set RPL Header Options to Solicited Information
   * 
   * \param rplInstanceId the RPL Instance ID
   * \param v the Version predicate flag
   * \param i the InstanceID predicate flag
   * \param d the DODAGID predicate flag
   * \param flags remaining flag bits
   * \param dodagId the DODAG ID
   * \param versionNumber the Version Number
   */
  void SetSolicitedInformation (uint8_t rplInstanceId, uint8_t v, uint8_t i, uint8_t d, uint8_t flags, Ipv6Address dodagId, uint8_t versionNumber);
  void SetSolicitedInformation (uint8_t rplInstanceId, uint8_t v, uint8_t i, uint8_t d, Ipv6Address dodagId, uint8_t versionNumber);

  /**
   * \ingroup rpl
   * Prefix Information option
   \verbatim
     0                   1                   2                   3
     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |   Type = 0x08 |Opt Length = 30| Prefix Length |L|A|R|Reserved1|
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                         Valid Lifetime                        |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                       Preferred Lifetime                      |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                           Reserved2                           |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                                                               |
    +                                                               +
    |                                                               |
    +                            Prefix                             +
    |                                                               |
    +                                                               +
    |                                                               |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   \endverbatim
   */
  struct PrefixInformation
  {
    uint8_t prefixLength;
    uint8_t l : 1;
    uint8_t a : 1;
    uint8_t r : 1;
    uint8_t reserved1 : 5;
    uint32_t validLifetime;
    uint32_t preferredLifetime;
    uint32_t reserved2;
    Ipv6Prefix prefix;
    /**
     * This method is used to print the content of a DodagConfiguration option.
     * \param os output stream
     */
    void Print (std::ostream &os) const;
    /**
     * Returns the expected size of the header.
     * \returns the expected size of the header.
     */
    uint32_t GetSerializedSize (void) const;
    /**
     * This method is used by Packet::AddHeader to
     * store a header into the byte buffer of a packet.
     *
     * \param start an iterator which points to where the header should
     *        be written.
     */
    void Serialize (Buffer::Iterator start) const;
    /**
     * This method is used by Packet::RemoveHeader to
     * re-create a header from the byte buffer of a packet.
     *
     * \param start an iterator which points to where the header should
     *        read from.
     * \returns the number of bytes read.
     */
    uint32_t Deserialize (Buffer::Iterator start, uint8_t optionLength);
  };
  /**
   * \brief Set RPL Header Options to Prefix Information
   * 
   * \param prefixLength the Prefix Length
   * \param l the on-link flag
   * \param a the Autonomous address-configuration flag
   * \param r the Router address flag
   * \param validLifetime the Valid Lifetime
   * \param preferredLifetime the Preferred Lifetime
   * \param prefix the Prefix
   */
  void SetPrefixInformation (uint8_t prefixLength, uint8_t l, uint8_t a, uint8_t r, uint8_t reserved1, uint32_t validLifetime, uint32_t preferredLifetime, uint32_t reserved2, Ipv6Prefix prefix);
  void SetPrefixInformation (uint8_t prefixLength, uint8_t l, uint8_t a, uint8_t r, uint32_t validLifetime, uint32_t preferredLifetime, Ipv6Prefix prefix);

  /**
   * \ingroup rpl
   * RPL Target Descriptor option
   \verbatim
     0                   1                   2                   3
     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |   Type = 0x09 |Opt Length = 4 |           Descriptor
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
           Descriptor (cont.)       |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   \endverbatim
   */
  struct RplTargetDescriptor
  {
    uint32_t descriptor;
    /**
     * This method is used to print the content of a DodagConfiguration option.
     * \param os output stream
     */
    void Print (std::ostream &os) const;
    /**
     * Returns the expected size of the header.
     * \returns the expected size of the header.
     */
    uint32_t GetSerializedSize (void) const;
    /**
     * This method is used by Packet::AddHeader to
     * store a header into the byte buffer of a packet.
     *
     * \param start an iterator which points to where the header should
     *        be written.
     */
    void Serialize (Buffer::Iterator start) const;
    /**
     * This method is used by Packet::RemoveHeader to
     * re-create a header from the byte buffer of a packet.
     *
     * \param start an iterator which points to where the header should
     *        read from.
     * \returns the number of bytes read.
     */
    uint32_t Deserialize (Buffer::Iterator start, uint8_t optionLength);
  };
  /**
   * \brief Set RPL Header Options to RPL Target Descriptor
   * 
   * \param descriptor the Descriptor Opaque
   */
  void SetRplTargetDescriptor (uint32_t descriptor);

/**
 * Get the Dag Metric Container option.
 * \returns The Dag Metric Container message.
 */
const DagMetricContainer& GetDagMetricContainer () const
{
  NS_ASSERT (m_type == OPTION_TYPE_DAG_METRIC_CONTAINER);
  return m_option.dagMetricContainer;
}

/**
 * Get the Route Information option.
 * \returns The Route Information message.
 */
const RouteInformation& GetRouteInformation () const
{
  NS_ASSERT (m_type == OPTION_TYPE_ROUTING_INFORMATION);
  return m_option.routeInformation;
}

/**
 * Get the Dodag Configuration option.
 * \returns The Dodag Configuration message.
 */
const DodagConfiguration& GetDodagConfiguration () const
{
  NS_ASSERT (m_type == OPTION_TYPE_DODAG_CONFIGURATION);
  return m_option.dodagConfiguration;
}

/**
 * Get the Rpl Target option.
 * \returns The Rpl Target message.
 */
const RplTarget& GetRplTarget () const
{
  NS_ASSERT (m_type == OPTION_TYPE_RPL_TARGET);
  return m_option.rplTarget;
}

/**
 * Get the Transit Information option.
 * \returns The Transit Information message.
 */
const TransitInformation& GetTransitInformation () const
{
  NS_ASSERT (m_type == OPTION_TYPE_TRANSIT_INFORMATION);
  return m_option.transitInformation;
}

/**
 * Get the Solicited Information option.
 * \returns The Solicited Information message.
 */
const SolicitedInformation& GetSolicitedInformation () const
{
  NS_ASSERT (m_type == OPTION_TYPE_SOLICITED_INFORMATION);
  return m_option.solicitedInformation;
}

/**
 * Get the Prefix Information option.
 * \returns The Prefix Information message.
 */
const PrefixInformation& GetPrefixInformation () const
{
  NS_ASSERT (m_type == OPTION_TYPE_PREFIX_INFORMATION);
  return m_option.prefixInformation;
}

/**
 * Get the Rpl Target Descriptor option.
 * \returns The Rpl Target Descriptor message.
 */
const RplTargetDescriptor& GetRplTargetDescriptor () const
{
  NS_ASSERT (m_type == OPTION_TYPE_RPL_TARGET_DESCRIPTOR);
  return m_option.rplTargetDescriptor;
}

private:
  uint8_t m_type;
  uint8_t m_optionLength; // number of bytes of the option excluding the 2 type and optionLength bytes
  struct
  {
    PadN padN;
    DagMetricContainer dagMetricContainer;
    RouteInformation routeInformation;
    DodagConfiguration dodagConfiguration;
    RplTarget rplTarget;
    TransitInformation transitInformation;
    SolicitedInformation solicitedInformation;
    PrefixInformation prefixInformation;
    RplTargetDescriptor rplTargetDescriptor;
  } m_option; // The actual option being carried
};


}
}

#endif // RPL_HEADER_OPTIONS_H