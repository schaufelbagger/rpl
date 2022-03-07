/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Authors: Alexander Baranyai
 */

#ifndef RPL_HELLO_REGRESSION_TEST_H
#define RPL_HELLO_REGRESSION_TEST_H

#include "ns3/test.h"
#include "ns3/nstime.h"
#include "ns3/socket.h"
#include "ns3/node-container.h"

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
class RplHelloRegressionTest : public TestCase
{
public:
  RplHelloRegressionTest ();
  ~RplHelloRegressionTest ();
private:
  /// Total simulation time
  const Time m_time;
  /// Create & configure test network
  void CreateNodes ();
  void DoRun ();
};

}
}

#endif /* RPL_HELLO_REGRESSION_TEST_H */
