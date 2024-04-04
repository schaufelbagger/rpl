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
#ifndef RPL_HEADER_REGRESSION_TEST_H
#define RPL_HEADER_REGRESSION_TEST_H

#include "ns3/test.h"
#include "ns3/nstime.h"

namespace ns3 {
namespace rpl {
/**
 * \ingroup rpl-test
 * \ingroup tests
 *
 * \brief Trivial (still useful) test of RPL operation
 *
 * This test creates 2 stations with point-to-point link and runs OLSR without any extra traffic.
 */
class RplHeaderRegressionTest : public TestCase
{
public:
  RplHeaderRegressionTest ();
  ~RplHeaderRegressionTest ();
private:
  /// Total simulation time
  const Time m_time;
  /// Create & configure test network
  void DisTest ();
  void DioTest ();
  void DaoTest ();
  void DaoAckTest ();
  void CcTest ();
  //void Ipv6Test();
  void DoRun ();
};

}
}

#endif /* RPL_HEADER_REGRESSION_TEST_H */
