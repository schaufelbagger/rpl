/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Authors: Alexander Baranyai
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
