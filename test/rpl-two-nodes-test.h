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
class RplTestTwoNodes : public TestCase
{
public:
  RplTestTwoNodes ();
  virtual ~RplTestTwoNodes ();

private:
  /// Total simulation time
  const Time m_time;
  virtual void DoRun (void);
  /// Create & configure test network
  void CreateNodes ();
};


}
}

#endif /* RPL_TWO_NODES_TEST_H */
