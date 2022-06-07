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

#include "rpl-header-option.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("RplHeaderOption");

namespace rpl {

RplHeaderOption::RplHeaderOption (uint8_t type, uint8_t optionLength, std::vector<uint8_t> data) 
:
m_type (type),
m_optionLength (optionLength)
{
}
TypeId RplHeaderOption::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::rpl::RplHeaderOption")
    .SetParent<Object> ()
    .SetGroupName ("Rpl")
    .AddConstructor<RplHeaderOption> ()
  ;
  return tid;
}

TypeId RplHeaderOption::GetInstanceTypeId () const
{
  return GetTypeId ();
}

uint8_t RplHeaderOption::GetType ()
{
  return m_type;
}
uint8_t RplHeaderOption::GetOptionLength ()
{
  return m_optionLength;
}


void RplHeaderOption::Print (std::ostream &os) const
{
  os << "RplHeaderOption:" << std::endl;

  switch (m_type)
  {
    case OPTION_TYPE_PAD1:
      os << "type: PAD1" << std::endl;
      break;
    case OPTION_TYPE_PADN:
      os << "type: PADN" << std::endl;
      break;
    case OPTION_TYPE_DAG_METRIC_CONTAINER:
      os << "type: DAG_METRIC_CONTAINER" << std::endl;
      break;
    case OPTION_TYPE_ROUTING_INFORMATION:
      os << "type: ROUTING_INFORMATION" << std::endl;
      break;
    case OPTION_TYPE_DODAG_CONFIGURATION:
      os << "type: DODAG_CONFIGURATION" << std::endl;
      break;
    case OPTION_TYPE_RPL_TARGET:
      os << "type: RPL_TARGET" << std::endl;
      break;
    case OPTION_TYPE_TRANSIT_INFORMATION:
      os << "type: TRANSIT_INFORMATION" << std::endl;
      break;
    case OPTION_TYPE_SOLICITED_INFORMATION:
      os << "type: SOLICITED_INFORMATION" << std::endl;
      break;
    case OPTION_TYPE_PREFIX_INFORMATION:
      os << "type: PREFIX_INFORMATION" << std::endl;
      break;
    case OPTION_TYPE_RPL_TARGET_DESCRIPTOR:
      os << "type: RPL_TARGET_DESCRIPTOR" << std::endl;
      break;
    default:
      NS_ABORT_MSG ("unrecognised option type");
  }

  os <<"Length: "<< +m_optionLength << std::endl;

  switch (m_type)
    {
    case OPTION_TYPE_PAD1:
      // print nothing
      break;
    case OPTION_TYPE_PADN:
      m_option.padN.Print (os);
      break;
    case OPTION_TYPE_DAG_METRIC_CONTAINER:
      m_option.dagMetricContainer.Print (os);
      break;
    case OPTION_TYPE_ROUTING_INFORMATION:
      m_option.routeInformation.Print (os);
      break;
    case OPTION_TYPE_DODAG_CONFIGURATION:
      m_option.dodagConfiguration.Print (os);
      break;
    case OPTION_TYPE_RPL_TARGET:
      m_option.rplTarget.Print (os);
      break;
    case OPTION_TYPE_TRANSIT_INFORMATION:
      m_option.transitInformation.Print (os);
      break;
    case OPTION_TYPE_SOLICITED_INFORMATION:
      m_option.solicitedInformation.Print (os);
      break;
    case OPTION_TYPE_PREFIX_INFORMATION:
      m_option.prefixInformation.Print (os);
      break;
    case OPTION_TYPE_RPL_TARGET_DESCRIPTOR:
      m_option.rplTargetDescriptor.Print (os);
      break;
    default:
      NS_ABORT_MSG ("unrecognised option type");
    }
}
void RplHeaderOption::Print ()
{
  Print (std::cout);
}

uint32_t RplHeaderOption::GetSerializedSize (void) const
{
  uint32_t size = 2 + m_optionLength;
  return size;
}

void RplHeaderOption::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  i.WriteU8 (m_type);
  i.WriteU8 (m_optionLength);

  switch (m_type)
    {
    case OPTION_TYPE_PAD1:
      // do nothing
      break;
    case OPTION_TYPE_PADN:
      m_option.padN.Serialize (i);
      break;
    case OPTION_TYPE_DAG_METRIC_CONTAINER:
      m_option.dagMetricContainer.Serialize (i);
      break;
    case OPTION_TYPE_ROUTING_INFORMATION:
      m_option.routeInformation.Serialize (i);
      break;
    case OPTION_TYPE_DODAG_CONFIGURATION:
      m_option.dodagConfiguration.Serialize (i);
      break;
    case OPTION_TYPE_RPL_TARGET:
      m_option.rplTarget.Serialize (i);
      break;
    case OPTION_TYPE_TRANSIT_INFORMATION:
      m_option.transitInformation.Serialize (i);
      break;
    case OPTION_TYPE_SOLICITED_INFORMATION:
      m_option.solicitedInformation.Serialize (i);
      break;
    case OPTION_TYPE_PREFIX_INFORMATION:
      m_option.prefixInformation.Serialize (i);
      break;
    case OPTION_TYPE_RPL_TARGET_DESCRIPTOR:
      m_option.rplTargetDescriptor.Serialize (i);
      break;
    default:
      NS_ABORT_MSG ("unrecognised option type");
    }
}

uint32_t RplHeaderOption::Deserialize (Buffer::Iterator start)
{
  uint32_t size;
  Buffer::Iterator i = start;

  m_type  = i.ReadU8 ();
  m_optionLength  = i.ReadU8 ();
  size = 2 + m_optionLength;

  switch (m_type)
    {
    case OPTION_TYPE_PAD1:
      // do nothing
      break;
    case OPTION_TYPE_PADN:
      // do nothing
      break;
    case OPTION_TYPE_DAG_METRIC_CONTAINER:
      m_option.dagMetricContainer.Deserialize (i, m_optionLength);
      break;
    case OPTION_TYPE_ROUTING_INFORMATION:
      m_option.routeInformation.Deserialize (i, m_optionLength);
      break;
    case OPTION_TYPE_DODAG_CONFIGURATION:
      m_option.dodagConfiguration.Deserialize (i, m_optionLength);
      break;
    case OPTION_TYPE_RPL_TARGET:
      m_option.rplTarget.Deserialize (i, m_optionLength);
      break;
    case OPTION_TYPE_TRANSIT_INFORMATION:
      m_option.transitInformation.Deserialize (i, m_optionLength);
      break;
    case OPTION_TYPE_SOLICITED_INFORMATION:
      m_option.solicitedInformation.Deserialize (i, m_optionLength);
      break;
    case OPTION_TYPE_PREFIX_INFORMATION:
      m_option.prefixInformation.Deserialize (i, m_optionLength);
      break;
    case OPTION_TYPE_RPL_TARGET_DESCRIPTOR:
      m_option.rplTargetDescriptor.Deserialize (i, m_optionLength);
      break;
    default:
      NS_ABORT_MSG ("unrecognised option type");
    }

  return size;
}

// ---------------- RPL PAD1 Option -------------------------------
void RplHeaderOption::SetPad1 ()
{
  m_type = 0;
  m_optionLength = 0;
}

// ---------------- RPL PADN Option -------------------------------

void RplHeaderOption::SetPadN (uint8_t optionLength)
{
  m_type = 0x01;

  NS_ABORT_MSG_IF(optionLength > 5, "In RPL Header Option PadN the length was set to an invalid value.");

  m_optionLength = optionLength;
  m_option.padN.padding.resize (optionLength,0);
}

void RplHeaderOption::PadN::Print (std::ostream &os) const
{
  //os << "padding length: " << +padding.size () << std::endl;
  for (auto item: padding)
  {
    std::bitset<8> x(item);
    os << x << std::endl;
  }
}

void RplHeaderOption::PadN::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  //start.Write (padding.begin (), padding.end ());
  for (auto item: padding)
  {
    i.WriteU8 (item);
  }
}

uint32_t RplHeaderOption::PadN::Deserialize (Buffer::Iterator start, uint8_t optionLength)
{
  //must be ignored by receiver
  return 0;
}


// ---------------- RPL DAG Metric Container Option -------------------------------

void RplHeaderOption::SetDagMetricContainer (uint8_t optionLength, std::vector<uint8_t> metricData)
{
  m_type = 0x02;
  m_optionLength = optionLength;
  m_option.dagMetricContainer.metricData = metricData;
}
void RplHeaderOption::SetDagMetricContainer (std::vector<uint8_t> metricData)
{
  SetDagMetricContainer (metricData.size (), metricData);
}

void RplHeaderOption::DagMetricContainer::Print (std::ostream &os) const
{
  //os << "padding length: " << +padding.size () << std::endl;
  for (auto item: metricData)
  {
    std::bitset<8> x(item);
    os << x << ", " << +item << std::endl;
  }
}

void RplHeaderOption::DagMetricContainer::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  //start.Write (padding.begin (), padding.end ());
  for (auto item: metricData)
  {
    i.WriteU8 (item);
  }
}

uint32_t RplHeaderOption::DagMetricContainer::Deserialize (Buffer::Iterator start, uint8_t optionLength)
{
  Buffer::Iterator i = start;
  metricData.clear ();
  for (uint8_t byteCounter = 0; byteCounter < optionLength; byteCounter++)
  {
    metricData.push_back (i.ReadU8 ());
  }
  uint32_t dist = i.GetDistanceFrom (start);
  NS_ASSERT (dist == optionLength);
  return dist;
}

// ---------------- RPL Route Information Option -------------------------------

void RplHeaderOption::SetRouteInformation(uint8_t prefixLength, uint8_t prf, uint32_t routeLifetime, Ipv6Prefix prefix)
{
  //NS_ABORT_MSG_UNLESS((flags|0b00011000) == 0b00011000, "In RPL Header Option Route Information the flags field is set to an invalid value.");
  NS_ABORT_MSG_IF(prf > 0b11, "Size of field is larger than allowed");
  uint8_t prefixLengthBits = prefixLength;
  NS_ABORT_MSG_IF(prefixLengthBits <=0, "In RPL Header Option Route Information the length of the Ipv6 Prefix was set to an invalid value.");

  // convert bits to next full byte in prefixLength
  uint8_t prefixLengthBytes = prefixLengthBits / 8;
  if (prefixLengthBits % 8 > 0)
  {
    prefixLengthBytes += 1;
  }

  m_type = 0x03;
  m_optionLength = prefixLengthBytes + 2*sizeof(uint8_t) + sizeof(uint32_t);

  m_option.routeInformation.prefixLength = prefixLengthBits;
  m_option.routeInformation.reserved1 = 0;
  m_option.routeInformation.prf = prf;
  m_option.routeInformation.reserved2 = 0;
  m_option.routeInformation.routeLifetime = routeLifetime;
  m_option.routeInformation.prefix = prefix;
  
  
  /*m_data.clear();
  m_data.reserve (2*sizeof(uint8_t) + sizeof(uint32_t));
  m_data.push_back (prefixLengthBits);
  m_data.push_back (flags);
  m_data.resize (2*sizeof(uint8_t) + sizeof(uint32_t));
  std::memcpy (&m_data[2], &routeLifetime, sizeof(uint32_t));

  prefix.GetBytes (buf);
  for (int i = 0; i < prefixLengthBytes; i++) {
    m_data.push_back (buf[i]);
  }*/
}
void RplHeaderOption::SetRouteInformation (uint8_t prf, uint32_t routeLifetime, Ipv6Prefix prefix)
{
  SetRouteInformation(prefix.GetPrefixLength (), prf, routeLifetime, prefix);
}

void RplHeaderOption::RouteInformation::Print (std::ostream &os) const
{
  os << "Prefix Length: " << +prefixLength << std::endl;
  os << "Flags: " << std::bitset<8>(prf) << ", " << +prf << std::endl;
  os << "Route Lifetime: 0x" << std::hex << +routeLifetime << std::dec << ", "  << +routeLifetime << std::endl;
  prefix.Print (os);
}

void RplHeaderOption::RouteInformation::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  uint8_t buf[16];
  uint8_t tmp =  (reserved1 << 5) | (prf << 3) | reserved2;

  uint8_t prefixLengthBytes = prefixLength / 8;
  if (prefixLength % 8 > 0)
  {
    prefixLengthBytes += 1;
  }

  i.WriteU8 (prefixLength);
  i.WriteU8 (tmp);
  i.WriteHtonU32 (routeLifetime);

  prefix.GetBytes (buf);
  i.Write (buf, prefixLengthBytes);
}

uint32_t RplHeaderOption::RouteInformation::Deserialize (Buffer::Iterator start, uint8_t optionLength)
{
  Buffer::Iterator i = start;
  uint8_t buf[16];
  uint8_t prefixLengthBytes;
  uint8_t tmp;

  std::fill(buf, buf+16, 0);

  prefixLength = i.ReadU8 ();
  tmp = i.ReadU8 ();
  reserved1 = (tmp & 0b11100000) >> 5;
  prf = (tmp & 0b00011000) >> 3;
  reserved2 = (tmp & 0b00000111);
  
  routeLifetime = i.ReadNtohU32 ();

  prefixLengthBytes = prefixLength / 8;
  if (prefixLength % 8 > 0)
  {
    prefixLengthBytes += 1;
  }

  i.Read (buf, prefixLengthBytes);
  prefix = Ipv6Prefix(buf);

  uint32_t dist = i.GetDistanceFrom (start);
  NS_ASSERT (dist == optionLength);
  return dist;
}

// ---------------- RPL Dodag Configuration Option -------------------------------

void RplHeaderOption::SetDodagConfiguration(uint8_t pcs, uint8_t dioIntervalDoublings, uint8_t dioIntervalMin, uint8_t dioRedundancyConstant, uint16_t maxRankIncrease, uint16_t minHopRankIncrease, uint16_t ocp, uint8_t reserved, uint8_t defaultLifetime, uint16_t lifetimeUnit)
{
  //NS_ABORT_MSG_UNLESS((flags|0b00001111) == 0b00001111, "In RPL Header Option DODAG Configuration flags field is set to an invalid value.");
  NS_ABORT_MSG_IF(pcs > 0b111, "Size of field is larger than allowed");
  NS_ABORT_MSG_UNLESS(reserved == 0, "In RPL Header Option DODAG Configuration reserved field is set to an invalid value.");
  m_type = 0x04;
  m_optionLength = 14;
  m_option.dodagConfiguration.flags = 0;
  m_option.dodagConfiguration.a = 0;  // no secure mode implemented so this is always zero
  m_option.dodagConfiguration.pcs = pcs;
  m_option.dodagConfiguration.dioIntervalDoublings = dioIntervalDoublings;
  m_option.dodagConfiguration.dioIntervalMin = dioIntervalMin;
  m_option.dodagConfiguration.dioRedundancyConstant = dioRedundancyConstant;
  m_option.dodagConfiguration.maxRankIncrease = maxRankIncrease;
  m_option.dodagConfiguration.minHopRankIncrease = minHopRankIncrease;
  m_option.dodagConfiguration.ocp = ocp;
  m_option.dodagConfiguration.reserved = reserved;
  m_option.dodagConfiguration.defaultLifetime = defaultLifetime;
  m_option.dodagConfiguration.lifetimeUnit = lifetimeUnit;
}
void RplHeaderOption::SetDodagConfiguration(uint8_t pcs, uint8_t dioIntervalDoublings, uint8_t dioIntervalMin, uint8_t dioRedundancyConstant, uint16_t maxRankIncrease, uint16_t minHopRankIncrease, uint16_t ocp, uint8_t defaultLifetime, uint16_t lifetimeUnit)
{
  SetDodagConfiguration(pcs, dioIntervalDoublings, dioIntervalMin, dioRedundancyConstant, maxRankIncrease, minHopRankIncrease, ocp, 0, defaultLifetime, lifetimeUnit);
}
void RplHeaderOption::DodagConfiguration::Print (std::ostream &os) const
{
  os << "PCS: " << std::bitset<8>(pcs) << ", " << +pcs << std::endl;
  os << "DIO Interval Doublings: " << +dioIntervalDoublings << std::endl;
  os << "DIO Interval Minimum: " << +dioIntervalMin << std::endl;
  os << "DIO Redundancy Constant: " << +dioRedundancyConstant << std::endl;
  os << "Max Rank Increase: " << +maxRankIncrease << std::endl;
  os << "Min Hop Rank Increase: " << +minHopRankIncrease << std::endl;
  os << "Objective Code Point: " << +ocp << std::endl;
  os << "Default Lifetime: " << +defaultLifetime << std::endl;
  os << "Lifetime Unit: " << +lifetimeUnit << std::endl;
}
void RplHeaderOption::DodagConfiguration::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  i.WriteU8 ((flags << 4) | (a << 3) | pcs);
  i.WriteU8 (dioIntervalDoublings);
  i.WriteU8 (dioIntervalMin);
  i.WriteU8 (dioRedundancyConstant);
  i.WriteHtonU16 (maxRankIncrease);
  i.WriteHtonU16 (minHopRankIncrease);
  i.WriteHtonU16 (ocp);
  i.WriteU8 (reserved);
  i.WriteU8 (defaultLifetime);
  i.WriteHtonU16 (lifetimeUnit);
}
uint32_t RplHeaderOption::DodagConfiguration::Deserialize (Buffer::Iterator start, uint8_t optionLength)
{
  Buffer::Iterator i = start;
  uint8_t tmp;
  tmp = i.ReadU8 ();
  flags = (tmp & 0b11110000) >> 4;
  a = (tmp & 0b00001000) >> 3;
  pcs = (tmp & 0b00000111);
  dioIntervalDoublings = i.ReadU8 ();
  dioIntervalMin = i.ReadU8 ();
  dioRedundancyConstant = i.ReadU8 ();
  maxRankIncrease = i.ReadNtohU16 ();
  minHopRankIncrease = i.ReadNtohU16 ();
  ocp = i.ReadNtohU16 ();
  reserved = i.ReadU8 ();
  defaultLifetime = i.ReadU8 ();
  lifetimeUnit = i.ReadNtohU16 ();

  uint32_t dist = i.GetDistanceFrom (start);
  NS_ASSERT (dist == optionLength);
  return dist;
}

// ---------------- RPL Rpl Target Option -------------------------------

void RplHeaderOption::SetRplTarget (uint8_t flags, uint8_t prefixLength, Ipv6Prefix targetPrefix)
{
  NS_ABORT_MSG_UNLESS((flags|0b00000000) == 0b00000000, "In RPL Header Option the flags field is set to an invalid value.");
  uint8_t prefixLengthBits = prefixLength;
  NS_ABORT_MSG_IF(prefixLengthBits <=0, "In RPL Header Option RPL Target the length of the Ipv6 Prefix was set to an invalid value.");

  // convert bits to next full byte in prefixLength
  uint8_t prefixLengthBytes = prefixLengthBits / 8;
  if (prefixLengthBits % 8 > 0)
  {
    prefixLengthBytes += 1;
  }

  m_type = 0x05;
  m_optionLength = prefixLengthBytes + 2*sizeof(uint8_t);

  m_option.rplTarget.prefixLength = prefixLengthBits;
  m_option.rplTarget.flags = flags;
  m_option.rplTarget.targetPrefix = targetPrefix;
}
void RplHeaderOption::SetRplTarget (uint8_t prefixLength, Ipv6Prefix targetPrefix)
{
  SetRplTarget (0, prefixLength, targetPrefix);
}
void RplHeaderOption::SetRplTarget (Ipv6Prefix targetPrefix)
{
  SetRplTarget (0, targetPrefix.GetPrefixLength (), targetPrefix);
}
void RplHeaderOption::SetRplTarget (Ipv6Address targetPrefix)
{
  uint8_t rawPrefix[16];
  targetPrefix.GetBytes (rawPrefix);
  Ipv6Prefix selfAddress = Ipv6Prefix (rawPrefix);
  SetRplTarget (0, 128, selfAddress);
}

void RplHeaderOption::RplTarget::Print (std::ostream &os) const
{
  os << "Prefix Length: " << +prefixLength << std::endl;
  os << "Flags: " << std::bitset<8>(flags) << ", " << +flags << std::endl;
  targetPrefix.Print (os);
}
void RplHeaderOption::RplTarget::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  uint8_t buf[16];

  uint8_t prefixLengthBytes = prefixLength / 8;
  if (prefixLength % 8 > 0)
  {
    prefixLengthBytes += 1;
  }

  i.WriteU8 (flags);
  i.WriteU8 (prefixLength);

  targetPrefix.GetBytes (buf);
  i.Write (buf, prefixLengthBytes);
}
uint32_t RplHeaderOption::RplTarget::Deserialize (Buffer::Iterator start, uint8_t optionLength)
{
  Buffer::Iterator i = start;
  uint8_t buf[16];
  uint8_t prefixLengthBytes;

  std::fill(buf, buf+16, 0);

  flags = i.ReadU8 ();
  prefixLength = i.ReadU8 ();

  prefixLengthBytes = prefixLength / 8;
  if (prefixLength % 8 > 0)
  {
    prefixLengthBytes += 1;
  }

  i.Read (buf, prefixLengthBytes);
  targetPrefix = Ipv6Prefix(buf);

  uint32_t dist = i.GetDistanceFrom (start);
  NS_ASSERT (dist == optionLength);
  return dist;
}

// ---------------- RPL Transit Information Option -------------------------------

void RplHeaderOption::SetTransitInformation (uint8_t e, uint8_t flags, uint8_t pathControl, uint8_t pathSequence, uint8_t pathLifetime, Ipv6Address parentAddress)
{
  NS_ABORT_MSG_IF(e > 0b1, "Size of field is larger than allowed");
  NS_ABORT_MSG_UNLESS(flags == 0, "In RPL Header Option the flags field is set to an invalid value.");

  m_type = 0x06;
  m_optionLength = 4*sizeof(uint8_t);
  if (parentAddress.IsInitialized())
  {
    m_optionLength += 16;
  }

  m_option.transitInformation.e = e;
  m_option.transitInformation.flags = flags;
  m_option.transitInformation.pathControl = pathControl;
  m_option.transitInformation.pathSequence = pathSequence;
  m_option.transitInformation.pathLifetime = pathLifetime;
  m_option.transitInformation.parentAddress = parentAddress;
}
void RplHeaderOption::SetTransitInformation (uint8_t e, uint8_t pathControl, uint8_t pathSequence, uint8_t pathLifetime, Ipv6Address parentAddress)
{
  SetTransitInformation (e, 0, pathControl, pathSequence, pathLifetime, parentAddress);
}
void RplHeaderOption::SetTransitInformation (uint8_t e, uint8_t pathControl, uint8_t pathSequence, uint8_t pathLifetime)
{
  SetTransitInformation (e, pathControl, pathSequence, pathLifetime, Ipv6Address());
}
void RplHeaderOption::TransitInformation::Print (std::ostream &os) const
{
  os << "External Flag: " << +e << std::endl;
  os << "Path Control: " << +pathControl << std::endl;
  os << "Path Sequence: " << +pathSequence << std::endl;
  os << "Path Lifetime: " << +pathLifetime << std::endl;
  parentAddress.Print (os);
}
void RplHeaderOption::TransitInformation::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  uint8_t buf[16];

  i.WriteU8 ( (e << 7) | flags);
  i.WriteU8 (pathControl);
  i.WriteU8 (pathSequence);
  i.WriteU8 (pathLifetime);

  if (parentAddress.IsInitialized())
  {
    parentAddress.Serialize (buf);
    i.Write (buf, 16);
  }
}
uint32_t RplHeaderOption::TransitInformation::Deserialize (Buffer::Iterator start, uint8_t optionLength)
{
  Buffer::Iterator i = start;
  uint8_t buf[16];
  uint8_t tmp;
  
  tmp = i.ReadU8 ();
  e = (tmp & 0b10000000) >> 7;
  flags = (tmp & 0b01111111);
  pathControl = i.ReadU8 ();
  pathSequence = i.ReadU8 ();
  pathLifetime = i.ReadU8 ();

  if (optionLength > 4)
  {
    i.Read (buf, 16);
    parentAddress = Ipv6Address(buf);
  }

  uint32_t dist = i.GetDistanceFrom (start);
  NS_ASSERT (dist == optionLength);
  return dist;
}

// ---------------- RPL Solicited Information Option -------------------------------

void RplHeaderOption::SetSolicitedInformation (uint8_t rplInstanceId, uint8_t v, uint8_t i, uint8_t d, uint8_t flags, Ipv6Address dodagId, uint8_t versionNumber)
{
  NS_ABORT_MSG_IF(v > 0b1, "Size of field is larger than allowed");
  NS_ABORT_MSG_IF(i > 0b1, "Size of field is larger than allowed");
  NS_ABORT_MSG_IF(d > 0b1, "Size of field is larger than allowed");
  NS_ABORT_MSG_UNLESS(flags == 0, "In RPL Header Option the flags field is set to an invalid value.");
  m_type = 0x07;
  m_optionLength = 19;

  m_option.solicitedInformation.rplInstanceId = rplInstanceId;
  m_option.solicitedInformation.v = v;
  m_option.solicitedInformation.i = i;
  m_option.solicitedInformation.d = d;
  m_option.solicitedInformation.flags = flags;
  m_option.solicitedInformation.dodagId = dodagId;
  m_option.solicitedInformation.versionNumber = versionNumber;
}
void RplHeaderOption::SetSolicitedInformation (uint8_t rplInstanceId, uint8_t v, uint8_t i, uint8_t d, Ipv6Address dodagId, uint8_t versionNumber)
{
  SetSolicitedInformation (rplInstanceId, v, i, d, 0, dodagId, versionNumber);
}

void RplHeaderOption::SolicitedInformation::Print (std::ostream &os) const
{
  os << "RPL Instance ID: " << +rplInstanceId << std::endl;
  os << "V: " << +v << std::endl;
  os << "I: " << +i << std::endl;
  os << "D: " << +d << std::endl;
  dodagId.Print (os);
  os << "Version Number: " << +versionNumber << std::endl;
}
void RplHeaderOption::SolicitedInformation::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  uint8_t buf[16];

  i.WriteU8 (rplInstanceId);
  i.WriteU8 ( (v << 7) | (this->i << 6) | (d << 5) | flags);
  dodagId.Serialize (buf);
  i.Write (buf, 16);
  i.WriteU8 (versionNumber);
}
uint32_t RplHeaderOption::SolicitedInformation::Deserialize (Buffer::Iterator start, uint8_t optionLength)
{
  Buffer::Iterator i = start;
  uint8_t buf[16];
  uint8_t tmp;
  
  rplInstanceId = i.ReadU8 ();
  tmp = i.ReadU8 ();
  v = (tmp & 0b10000000) >> 7;
  this->i = (tmp & 0b01000000) >> 6;
  d = (tmp & 0b00100000) >> 5;
  flags = (tmp & 0b00011111);
  i.Read (buf, 16);
  dodagId = Ipv6Address(buf);
  versionNumber = i.ReadU8 ();

  uint32_t dist = i.GetDistanceFrom (start);
  NS_ASSERT (dist == optionLength);
  return dist;
}

// ---------------- RPL Prefix Information Option -------------------------------

void RplHeaderOption::SetPrefixInformation (uint8_t prefixLength, uint8_t l, uint8_t a, uint8_t r, uint8_t reserved1, uint32_t validLifetime, uint32_t preferredLifetime, uint32_t reserved2, Ipv6Prefix prefix)
{
  NS_ABORT_MSG_IF(l > 0b1, "Size of field is larger than allowed");
  NS_ABORT_MSG_IF(a > 0b1, "Size of field is larger than allowed");
  NS_ABORT_MSG_IF(r > 0b1, "Size of field is larger than allowed");
  NS_ABORT_MSG_IF(prefixLength <=0, "In RPL Header Option RPL Target the length of the Ipv6 Prefix was set to an invalid value.");
  NS_ABORT_MSG_UNLESS(reserved1 == 0, "In RPL Header Option RPL Prefix Information reserved1 field is set to an invalid value.");
  NS_ABORT_MSG_UNLESS(reserved2 == 0, "In RPL Header Option RPL Prefix Information reserved2 field is set to an invalid value.");
  
  m_type = 0x08;
  m_optionLength = 30;

  m_option.prefixInformation.prefixLength = prefixLength;
  m_option.prefixInformation.l = l;
  m_option.prefixInformation.a = a;
  m_option.prefixInformation.r = r;
  m_option.prefixInformation.reserved1 = reserved1;
  m_option.prefixInformation.validLifetime = validLifetime;
  m_option.prefixInformation.preferredLifetime = preferredLifetime;
  m_option.prefixInformation.reserved2 = reserved2;
  m_option.prefixInformation.prefix = prefix;
}
void RplHeaderOption::SetPrefixInformation (uint8_t prefixLength, uint8_t l, uint8_t a, uint8_t r, uint32_t validLifetime, uint32_t preferredLifetime, Ipv6Prefix prefix)
{
  SetPrefixInformation (prefixLength, l, a, r, 0, validLifetime, preferredLifetime, 0, prefix);
}
void RplHeaderOption::PrefixInformation::Print (std::ostream &os) const
{
  os << "Prefix Length: " << +prefixLength << std::endl;
  os << "On-link flag: " << +l << std::endl;
  os << "Autonomous address-configuration flag: " << +a << std::endl;
  os << "Router address flag: " << +r << std::endl;
  os << "Valid Lifetime: " << +validLifetime << std::endl;
  os << "Preferred Lifetime: " << +preferredLifetime << std::endl;
  prefix.Print (os);
}
void RplHeaderOption::PrefixInformation::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  uint8_t buf[16];

  std::fill(buf, buf+16, 0);

  i.WriteU8 (prefixLength);
  i.WriteU8 ( (l << 7) | (a << 6) | (r << 5) | reserved1);
  i.WriteHtonU32 (validLifetime);
  i.WriteHtonU32 (preferredLifetime);
  i.WriteHtonU32 (reserved2);
  prefix.GetBytes (buf);
  i.Write (buf, 16);
}
uint32_t RplHeaderOption::PrefixInformation::Deserialize (Buffer::Iterator start, uint8_t optionLength)
{
  Buffer::Iterator i = start;
  uint8_t buf[16];
  uint8_t tmp;
  
  prefixLength = i.ReadU8 ();
  tmp = i.ReadU8 ();
  l = (tmp & 0b10000000) >> 7;
  a = (tmp & 0b01000000) >> 6;
  r = (tmp & 0b00100000) >> 5;
  reserved1 = (tmp & 0b00011111);
  validLifetime = i.ReadNtohU32 ();
  preferredLifetime = i.ReadNtohU32 ();
  reserved2 = i.ReadNtohU32 ();
  i.Read (buf, 16);
  prefix = Ipv6Prefix(buf);

  uint32_t dist = i.GetDistanceFrom (start);
  NS_ASSERT (dist == optionLength);
  return dist;
}

// ---------------- RPL Rpl Target Descriptor Option -------------------------------

void RplHeaderOption::SetRplTargetDescriptor (uint32_t descriptor)
{
  m_type = 0x09;
  m_optionLength = 4;
  m_option.rplTargetDescriptor.descriptor = descriptor;
}
void RplHeaderOption::RplTargetDescriptor::Print (std::ostream &os) const
{
  os << "Descriptor: " << +descriptor << std::endl;
}
void RplHeaderOption::RplTargetDescriptor::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  i.WriteHtonU32 (descriptor);
}
uint32_t RplHeaderOption::RplTargetDescriptor::Deserialize (Buffer::Iterator start, uint8_t optionLength)
{
  Buffer::Iterator i = start;
  descriptor = i.ReadNtohU32 ();

  uint32_t dist = i.GetDistanceFrom (start);
  NS_ASSERT (dist == optionLength);
  return dist;
}



}
}