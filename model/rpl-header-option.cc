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
m_optionLength (optionLength),
m_data (data)
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

  uint8_t RplHeaderOption::GetType ()
  {
    return m_type;
  }
  uint8_t RplHeaderOption::GetOptionLength ()
  {
    return m_optionLength;
  }
  std::vector<uint8_t> RplHeaderOption::GetData()
  {
    return m_data;
  }


void RplHeaderOption::Print (std::ostream &os)
{
  os << "RplHeaderOption:" << std::endl;
  os <<"type: "<< unsigned(m_type) << std::endl;
  os <<"length: "<< unsigned(m_optionLength) << std::endl;
  for (auto item: m_data)
  {
    std::bitset<8> x(item);
    os << x << ", " << unsigned(item) << std::endl;
  }
}
void RplHeaderOption::Print ()
{
  Print (std::cout);
}

void RplHeaderOption::SetPadN (uint8_t optionLength)
{
  m_type = 0x01;

  NS_ABORT_MSG_IF(optionLength > 5, "In RPL Header Option PadN the length was set to an invalid value.");

  m_optionLength = optionLength;
  m_data.resize (optionLength,0);
}

void RplHeaderOption::SetDagMetricContainer (uint8_t optionLength, std::vector<uint8_t> metricData)
{
  m_type = 0x02;
  m_optionLength = optionLength;
  m_data = metricData;
}
void RplHeaderOption::SetDagMetricContainer (std::vector<uint8_t> metricData)
{
  SetDagMetricContainer (metricData.size (), metricData);
}

void RplHeaderOption::SetRouteInformation (uint8_t flags, uint32_t routeLifetime, Ipv6Prefix prefix)
{
  // TODO find fix for memcopy
  uint8_t buf[16];
  //auto routeLifetimePointer = reinterpret_cast<unsigned char *> (routeLifetime);
  uint8_t prefixLengthBits = prefix.GetPrefixLength ();
  NS_ABORT_MSG_IF(prefixLengthBits <=0, "In RPL Header Option Route Information the length of the Ipv6 Prefix was set to an invalid value.");

  // convert bits to next full byte in preixLength
  uint8_t prefixLengthBytes = prefixLengthBits / 8;
  if (prefixLengthBits % 8 > 0)
  {
    prefixLengthBytes += 1;
  }

  m_type = 0x03;
  m_optionLength = prefixLengthBytes + 2*sizeof(uint8_t) + sizeof(uint32_t);
  
  m_data.clear();
  m_data.reserve (2*sizeof(uint8_t) + sizeof(uint32_t));
  m_data.push_back (prefixLengthBits);
  m_data.push_back (flags);
  m_data.resize (2*sizeof(uint8_t) + sizeof(uint32_t));
  std::memcpy (&m_data[2], &routeLifetime, sizeof(uint32_t));

  prefix.GetBytes (buf);
  for (int i = 0; i < prefixLengthBytes; i++) {
    m_data.push_back (buf[i]);
  }
}


}
}