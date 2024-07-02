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
#ifndef RPL_TWO_NODES_TEST_H
#define RPL_TWO_NODES_TEST_H

#include "ns3/test.h"
#include "ns3/nstime.h"
#include "ns3/node-container.h"

namespace ns3 {
namespace rpl {

/**
 * \ingroup rpl-test
 * \ingroup tests
 *
 * See \bugid{780}
 */
// This is an example TestCase.
class RplLineTest : public TestCase
{
public:
  RplLineTest ();
  virtual ~RplLineTest ();

private:
  /// Total simulation time
  bool m_foundLastNode = false;
  int m_numberOfNodes = 3;
  const Time m_time;
  /// Create & configure test network
  virtual void DoRun (void);
  void RouteAddedTraceSink(std::string context, rpl::RplRoutingTableEntry entry);
  Ptr<rpl::RoutingProtocol> GetRpl(Ptr <Node> node);

};


}
}

#endif /* RPL_TWO_NODES_TEST_H */
