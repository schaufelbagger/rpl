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
#ifndef RPL_HEADER_OPTIONS_H
#define RPL_HEADER_OPTIONS_H

#include "ns3/object.h"
#include "ns3/icmpv6-header.h"

#include <bitset>

namespace ns3 {
namespace rpl {

/**
 * \ingroup rpl
 * \brief   RPL header options field
 */
class RplHeaderOption : public Object
{
public:
   enum OptionType_e : uint8_t 
   {
      PAD1=0,
      PADN=1,
      DAG_METRIC_CONTAINER=2,
      ROUTING_INFORMATION=3,
      DODAG_CONFIGURATION=4,
      RPL_TARGET=5,
      TRANSIT_INFORMATION=6,
      SOLICITED_INFORMATION=7,
      PREFIX_INFORMATION=8,
      RPL_TARGET_DESCRIPTOR=9
      } OptionType_t;

  /**
   * constructor
   *
   * \param type the option type (0)
   * \param length the length of the data (0)
   * \param data the data bits (0)
   */
  RplHeaderOption (uint8_t type = 0, uint8_t optionLength = 0, std::vector<uint8_t> data = {});
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId ();
  uint8_t GetType();
  uint8_t GetOptionLength();
  std::vector<uint8_t> GetData();
  void Print();
  void Print(std::ostream &os);
  /**
   * \brief Set RPL Header Options to PadN
   * 
   * \param optionLength length of the padding, valid from 0 to 5
   * \verbatim 
      0                   1                   2
      0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- - - - - - - -
      |   Type = 0x01 | Option Length | 0x00 Padding...
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- - - - - - - -
   * \endverbatim
   * 
   */
  
  void SetPadN (uint8_t optionLength);
  /**
   * \brief Set RPL Header Options to DAG Metric Container
   * 
   * \param optionLength length of the padding
   * \param metricData the metric data
   * \verbatim 
      0                   1                   2
      0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- - - - - - - -
      |   Type = 0x02 | Option Length | Metric Data
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- - - - - - - -
   * \endverbatim
   * 
   */
  void SetDagMetricContainer(uint8_t optionLength, std::vector<uint8_t> metricData);
  void SetDagMetricContainer(std::vector<uint8_t> metricData);
  /**
   * \brief Set RPL Header Options to Route Information
   * 
   * \param optionLength length of the padding
   * \param prefixLength 
   * \param flags 
   * \param routeLifetime 
   * \param prefix 
   * \verbatim 
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
   * \endverbatim
   */
  void SetRouteInformation(uint8_t flags, uint32_t routeLifetime, Ipv6Prefix prefix);

private:
  uint8_t m_type;
  uint8_t m_optionLength;
  std::vector<uint8_t> m_data;
};


}
}

#endif // RPL_HEADER_OPTIONS_H