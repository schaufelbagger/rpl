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
#include <vector>
#include "rpl-header-regression-test.h"
#include "ns3/simulator.h"
#include "ns3/rpl.h"
#include "ns3/rpl-helper.h"
#include "ns3/rpl-header.h"
#include "ns3/rpl-ipv6-header.h"

namespace ns3 {
NS_LOG_COMPONENT_DEFINE("RPL header regression test");
namespace rpl {

RplHeaderRegressionTest::RplHeaderRegressionTest () :
  TestCase ("Test RPL Header messages generation"),
  m_time (Seconds (10))
{
}

RplHeaderRegressionTest::~RplHeaderRegressionTest ()
{
}

void RplHeaderRegressionTest::DoRun ()
{
  //test here
  this->DisTest ();
  this->DioTest ();
  this->DaoTest ();
  this->DaoAckTest ();
  this->CcTest ();

  Simulator::Stop (m_time);
  Simulator::Run ();
  Simulator::Destroy ();
}

void RplHeaderRegressionTest::DisTest ()
{
  Ptr<Packet> packet = Create<Packet> ();
  DisHeader inDisHeader;
  RplIcmpv6Header inRplIcmpv6Header;
  RplIcmpv6Header outRplIcmpv6Header;
  DisHeader outDisHeader;
  inDisHeader.SetFlags (3);

  packet->AddHeader (inDisHeader);
  packet->AddHeader (inRplIcmpv6Header);

  packet->RemoveHeader (outRplIcmpv6Header);
  NS_TEST_ASSERT_MSG_EQ (+outRplIcmpv6Header.GetType (), 155, "Type does not match");
  NS_TEST_ASSERT_MSG_EQ (+outRplIcmpv6Header.GetCode (), 0, "Code does not match");
  packet->RemoveHeader (outDisHeader);
  NS_TEST_ASSERT_MSG_EQ (+outDisHeader.GetFlags (), 3, "Code does not match");
}
void RplHeaderRegressionTest::DioTest ()
{
  Ptr<Packet> packet = Create<Packet> ();

  uint8_t rplInstanceId = 0xAA; 
  uint8_t versionNumber = 0b10101010;
  uint16_t rank = 0x0201;
  uint8_t grounded = 1;
  uint8_t mop = 0b101;
  uint8_t prf = 0b010;
  uint8_t dtsn = 0xAA;
  uint8_t flags = 0xAA;
  uint8_t reserved = 0;
  Ipv6Address dodagId = Ipv6Address("2001:db8::1");

  DioHeader inDioHeader (rplInstanceId, versionNumber, rank, grounded, mop, prf, dtsn, flags, reserved, dodagId);
  DioHeader outDioHeader;
  RplIcmpv6Header inRplIcmpv6Header (TYPE_DIO);
  RplIcmpv6Header outRplIcmpv6Header;

  

  packet->AddHeader (inDioHeader);
  packet->AddHeader (inRplIcmpv6Header);
  packet->RemoveHeader (outRplIcmpv6Header);
  NS_TEST_ASSERT_MSG_EQ (+outRplIcmpv6Header.GetType (), 155, "Type does not match");
  NS_TEST_ASSERT_MSG_EQ (+outRplIcmpv6Header.GetCode (), 1, "Code does not match");
  packet->RemoveHeader (outDioHeader);
  NS_TEST_ASSERT_MSG_EQ (+outDioHeader.GetRplInstanceId (), +rplInstanceId, "RPL Instance ID does not match");
  NS_TEST_ASSERT_MSG_EQ (+outDioHeader.GetVersionNumber (), +versionNumber, "Version Number does not match");
  NS_TEST_ASSERT_MSG_EQ (+outDioHeader.GetRank (), +rank, "Rank does not match");
  NS_TEST_ASSERT_MSG_EQ (+outDioHeader.GetGrounded (), +grounded, "Grounded Bit does not match");
  NS_TEST_ASSERT_MSG_EQ (+outDioHeader.GetMop (), +mop, "MOP does not match");
  NS_TEST_ASSERT_MSG_EQ (+outDioHeader.GetPrf (), +prf, "PRF does not match");
  NS_TEST_ASSERT_MSG_EQ (+outDioHeader.GetDtsn (), +dtsn, "DTSN does not match");
  NS_TEST_ASSERT_MSG_EQ (+outDioHeader.GetFlags (), +flags, "Flags does not match");
  NS_TEST_ASSERT_MSG_EQ (outDioHeader.GetDodagId (), dodagId, "DODAG ID does not match");
}
void RplHeaderRegressionTest::DaoTest ()
{
  Ptr<Packet> packet = Create<Packet> ();
  uint8_t rplInstanceId = 0xAA; 
  uint8_t d = 1;
  uint8_t k = 1;
  uint8_t flags = 0x2A;
  uint8_t reserved = 0;
  uint8_t daoSequence = 0xAA;
  Ipv6Address dodagId = Ipv6Address("2001:db8::1");

  DaoHeader inDaoHeader (rplInstanceId, k, d, flags, reserved, daoSequence, dodagId);
  DaoHeader outDaoHeader;
  RplIcmpv6Header inRplIcmpv6Header (TYPE_DAO);
  RplIcmpv6Header outRplIcmpv6Header;

  packet->AddHeader (inDaoHeader);
  packet->AddHeader (inRplIcmpv6Header);

  //packet->Print (std::cout);

  packet->RemoveHeader (outRplIcmpv6Header);
  NS_TEST_ASSERT_MSG_EQ (+outRplIcmpv6Header.GetType (), 155, "Type does not match");
  NS_TEST_ASSERT_MSG_EQ (+outRplIcmpv6Header.GetCode (), 2, "Code does not match");
  packet->RemoveHeader (outDaoHeader);
  //inDaoHeader.Print (std::cout);
  //outDaoHeader.Print (std::cout);
  NS_TEST_ASSERT_MSG_EQ (+outDaoHeader.GetRplInstanceId (), +rplInstanceId, "RPL Instance ID does not match");
  NS_TEST_ASSERT_MSG_EQ (+outDaoHeader.GetK (), +k, "K does not match");
  NS_TEST_ASSERT_MSG_EQ (+outDaoHeader.GetD (), +d, "D does not match");
  NS_TEST_ASSERT_MSG_EQ (+outDaoHeader.GetFlags (), +flags, "Flags does not match");
  NS_TEST_ASSERT_MSG_EQ (+outDaoHeader.GetReserved (), +reserved, "Reserved does not match");
  NS_TEST_ASSERT_MSG_EQ (+outDaoHeader.GetDaoSequence (), +daoSequence, "DAO Sequence does not match");
  NS_TEST_ASSERT_MSG_EQ (outDaoHeader.GetDodagId (), dodagId, "DODAG ID does not match");
}
void RplHeaderRegressionTest::DaoAckTest ()
{
  Ptr<Packet> packet = Create<Packet> ();
  uint8_t rplInstanceId = 0xAA; 
  uint8_t d = 1;
  uint8_t reserved = 0;
  uint8_t daoSequence = 0xAA;
  uint8_t status = 0xAA;
  Ipv6Address dodagId = Ipv6Address("2001:db8::1");

  DaoAckHeader inDaoAckHeader (rplInstanceId, d, reserved, daoSequence, status, dodagId);
  DaoAckHeader outDaoAckHeader;
  RplIcmpv6Header inRplIcmpv6Header (TYPE_DAO_ACK);
  RplIcmpv6Header outRplIcmpv6Header;

  packet->AddHeader (inDaoAckHeader);
  packet->AddHeader (inRplIcmpv6Header);
  packet->RemoveHeader (outRplIcmpv6Header);
  NS_TEST_ASSERT_MSG_EQ (+outRplIcmpv6Header.GetType (), 155, "Type does not match");
  NS_TEST_ASSERT_MSG_EQ (+outRplIcmpv6Header.GetCode (), 3, "Code does not match");
  packet->RemoveHeader (outDaoAckHeader);
  //inDaoAckHeader.Print (std::cout);
  //outDaoAckHeader.Print (std::cout);
  NS_TEST_ASSERT_MSG_EQ (+outDaoAckHeader.GetRplInstanceId (), +rplInstanceId, "RPL Instance ID does not match");
  NS_TEST_ASSERT_MSG_EQ (+outDaoAckHeader.GetD (), +d, "D does not match");
  NS_TEST_ASSERT_MSG_EQ (+outDaoAckHeader.GetReserved (), +reserved, "Reserved does not match");
  NS_TEST_ASSERT_MSG_EQ (+outDaoAckHeader.GetDaoSequence (), +daoSequence, "DAO Sequence does not match");
  NS_TEST_ASSERT_MSG_EQ (+outDaoAckHeader.GetStatus (), +status, "Status does not match");
  NS_TEST_ASSERT_MSG_EQ (outDaoAckHeader.GetDodagId (), dodagId, "DODAG ID does not match");
}
void RplHeaderRegressionTest::CcTest ()
{
  Ptr<Packet> packet = Create<Packet> ();
  uint8_t rplInstanceId = 0xAA; 
  uint8_t r = 1;
  uint8_t flags = 0;
  uint16_t ccNounce = 0xFBFA;
  Ipv6Address dodagId = Ipv6Address("2001:db8::1");
  uint32_t destinationCounter = 0x01020304;

  CcHeader inCcHeader (rplInstanceId, r, flags, ccNounce, dodagId, destinationCounter);
  CcHeader outCcHeader;
  RplIcmpv6Header inRplIcmpv6Header (TYPE_CC);
  RplIcmpv6Header outRplIcmpv6Header;

  packet->AddHeader (inCcHeader);
  packet->AddHeader (inRplIcmpv6Header);
  packet->RemoveHeader (outRplIcmpv6Header);
  NS_TEST_ASSERT_MSG_EQ (+outRplIcmpv6Header.GetType (), 155, "Type does not match");
  NS_TEST_ASSERT_MSG_EQ (+outRplIcmpv6Header.GetCode (), 0x8A, "Code does not match");
  packet->RemoveHeader (outCcHeader);
  NS_TEST_ASSERT_MSG_EQ (+outCcHeader.GetRplInstanceId (), +rplInstanceId, "RPL Instance ID does not match");
  NS_TEST_ASSERT_MSG_EQ (+outCcHeader.GetR (), +r, "R does not match");
  NS_TEST_ASSERT_MSG_EQ (+outCcHeader.GetFlags (), +flags, "Reserved does not match");
  NS_TEST_ASSERT_MSG_EQ (+outCcHeader.GetCcNonce (), +ccNounce, "CC Nonce does not match");
  NS_TEST_ASSERT_MSG_EQ (outCcHeader.GetDodagId (), dodagId, "DODAG ID does not match");
  NS_TEST_ASSERT_MSG_EQ (+outCcHeader.GetDestinationCounter (), +destinationCounter, "Destination Counter does not match");
}



}   //rpl
}   //ns3