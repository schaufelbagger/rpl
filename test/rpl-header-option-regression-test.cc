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
#include "rpl-header-option-regression-test.h"
#include "ns3/simulator.h"
#include "ns3/rpl.h"
#include "ns3/rpl-helper.h"
#include "ns3/rpl-header.h"

namespace ns3 {
NS_LOG_COMPONENT_DEFINE("RPL header option regression test");
namespace rpl {

RplHeaderOptionRegressionTest::RplHeaderOptionRegressionTest () :
  TestCase ("Test RPL Header Option generation"),
  m_time (Seconds (10))
{
}

RplHeaderOptionRegressionTest::~RplHeaderOptionRegressionTest ()
{
}

void RplHeaderOptionRegressionTest::DoRun ()
{
  this->DagMetricContainerTest ();
  this->RouteInformationTest ();
  this->DodagConfigurationTest ();
  this->RplTargetTest ();
  this->TransitInformationTest ();
  this->SolicitedInformationTest ();
  this->PrefixInformationTest ();
  this->RplTargetDescriptorTest ();

  Simulator::Stop (m_time);
  Simulator::Run ();
  Simulator::Destroy ();
}

void RplHeaderOptionRegressionTest::DagMetricContainerTest ()
{
  RplHeaderOption inputOption;
  RplHeaderOption outputOption;
  Buffer buffer;
  std::vector<uint8_t> inputData;
  std::vector<uint8_t> outputData;
  uint8_t inputLength = 5;
  for (int i = 0; i < inputLength; i++) {
    inputData.push_back(i);
  }
  
  

  inputOption.SetDagMetricContainer(inputData);

  buffer.AddAtStart (inputOption.GetSerializedSize ());

  //dagMetricContainer.Print ();

  NS_TEST_ASSERT_MSG_EQ (+inputOption.GetType (), 0x02, "type does not match in the DAG Metric Container");
  NS_TEST_ASSERT_MSG_EQ (+inputOption.GetOptionLength (), +inputLength, "option length does not match in the DAG Metric Container");
  
  inputOption.Serialize (buffer.Begin ());
  outputOption.Deserialize (buffer.Begin ());
  const RplHeaderOption::DagMetricContainer &outputDagMetricContainer = outputOption.GetDagMetricContainer ();

  NS_TEST_ASSERT_MSG_EQ (+outputOption.GetType (), 0x02, "type does not match in the DAG Metric Container");
  NS_TEST_ASSERT_MSG_EQ (+outputOption.GetOptionLength (), +inputLength, "option length does not match in the DAG Metric Container");
  NS_TEST_ASSERT_MSG_EQ (+outputDagMetricContainer.metricData.size (), +inputData.size (), "in and output vector length do not match in the DAG Metric Container");
  for (uint8_t i = 0; i < inputLength; i++) {
    NS_TEST_ASSERT_MSG_EQ (+outputDagMetricContainer.metricData[i], +inputData[i], "data length does not match in the DAG Metric Container");
  }
}

void RplHeaderOptionRegressionTest::RouteInformationTest ()
{
  Buffer buffer;
  RplHeaderOption inputOption;
  RplHeaderOption outputOption;
  std::vector<uint8_t> inputData;
  std::vector<uint8_t> outputData;
  uint8_t rawPrefix[16];
  uint8_t flags = 0b00011000; 
  uint32_t routeLifetime = 0x01020304;

  unsigned long expectedDataSize;

  std::fill(rawPrefix, rawPrefix+16, 0);
  rawPrefix[0] = 0xFF;
  rawPrefix[1] = 0xF0;

  Ipv6Prefix objectPrefix (rawPrefix);

  uint8_t prefixLengthBits = objectPrefix.GetPrefixLength ();
  uint8_t prefixLengthBytes = prefixLengthBits / 8;
  if (prefixLengthBits % 8 > 0)
  {
    prefixLengthBytes += 1;
  }

  expectedDataSize =  2*sizeof(uint8_t) + sizeof(uint32_t)+prefixLengthBytes;

  
  //std::cout << "Prefix Length: " << unsigned(objectPrefix.GetPrefixLength ()) << std::endl;

  inputOption.SetRouteInformation (flags, routeLifetime, objectPrefix);

  buffer.AddAtStart (inputOption.GetSerializedSize ());

  //inputOption.Print ();

  inputOption.Serialize (buffer.Begin ());
  outputOption.Deserialize (buffer.Begin ());
  const RplHeaderOption::RouteInformation &outputRouteInformation = outputOption.GetRouteInformation ();

  NS_TEST_ASSERT_MSG_EQ (+outputOption.GetType (), 0x03, "Type does not match");
  NS_TEST_ASSERT_MSG_EQ (+outputOption.GetOptionLength (), +expectedDataSize, "Option length does not match");
  NS_TEST_ASSERT_MSG_EQ (+outputRouteInformation.flags, +flags, "Flag field differs from input");
  NS_TEST_ASSERT_MSG_EQ (+outputRouteInformation.prefixLength, +prefixLengthBits, "Prefix Length field differs from input");
  NS_TEST_ASSERT_MSG_EQ (+outputRouteInformation.routeLifetime, +routeLifetime, "Route Lifetime field differs from input");
  NS_TEST_ASSERT_MSG_EQ (outputRouteInformation.prefix, objectPrefix, "Prefix differs from input");
}

void RplHeaderOptionRegressionTest::DodagConfigurationTest ()
{
  RplHeaderOption inputOption;
  RplHeaderOption outputOption;
  Buffer buffer;
  std::vector<uint8_t> inputData;
  uint8_t flags = 0b00000000 | DEFAULT_PATH_CONTROL_SIZE;
  uint8_t dioIntervalDoublings = DEFAULT_DIO_INTERVAL_DOUBLINGS;
  uint8_t dioIntervalMin = DEFAULT_DIO_INTERVAL_MIN;
  uint8_t dioRedundancyConstant = DEFAULT_DIO_REDUNDANCY_CONSTANT;
  uint16_t maxRankIncrease = 1;
  uint16_t minHopRankIncrease = 1;
  uint16_t ocp = 1;
  uint8_t defaultLifetime = 1;
  uint16_t lifetimeUnit = 1;


  inputOption.SetDodagConfiguration(flags, dioIntervalDoublings, dioIntervalMin, dioRedundancyConstant, maxRankIncrease, minHopRankIncrease, ocp, defaultLifetime, lifetimeUnit);

  buffer.AddAtStart (inputOption.GetSerializedSize ());

  inputOption.Print ();

  NS_TEST_ASSERT_MSG_EQ (+inputOption.GetType (), 0x04, "type does not match");
  NS_TEST_ASSERT_MSG_EQ (+inputOption.GetOptionLength (), 14, "option length does not match");
  
  inputOption.Serialize (buffer.Begin ());
  outputOption.Deserialize (buffer.Begin ());
  /*const RplHeaderOption::DodagConfiguration &outputDodagConfiguration = outputOption.GetDodagConfiguration ();

  NS_TEST_ASSERT_MSG_EQ (+outputOption.GetType (), 0x04, "type does not match");
  NS_TEST_ASSERT_MSG_EQ (+outputOption.GetOptionLength (), 14, "option length does not match");
  NS_TEST_ASSERT_MSG_EQ (+outputDodagConfiguration.flags, +flags, "flags differs from input");
  NS_TEST_ASSERT_MSG_EQ (+outputDodagConfiguration.dioIntervalDoublings, +dioIntervalDoublings, "dioIntervalDoublings differs from input");
  NS_TEST_ASSERT_MSG_EQ (+outputDodagConfiguration.dioIntervalMin, +dioIntervalMin, "dioIntervalMin differs from input");
  NS_TEST_ASSERT_MSG_EQ (+outputDodagConfiguration.dioRedundancyConstant, +dioRedundancyConstant, "dioRedundancyConstant differs from input");
  NS_TEST_ASSERT_MSG_EQ (+outputDodagConfiguration.maxRankIncrease, +maxRankIncrease, "maxRankIncrease differs from input");
  NS_TEST_ASSERT_MSG_EQ (+outputDodagConfiguration.minHopRankIncrease, +minHopRankIncrease, "minHopRankIncrease differs from input");
  NS_TEST_ASSERT_MSG_EQ (+outputDodagConfiguration.ocp, +ocp, "ocp differs from input");
  NS_TEST_ASSERT_MSG_EQ (+outputDodagConfiguration.defaultLifetime, +defaultLifetime, "defaultLifetime differs from input");
  NS_TEST_ASSERT_MSG_EQ (+outputDodagConfiguration.lifetimeUnit, +lifetimeUnit, "lifetimeUnit differs from input");*/
}

void RplHeaderOptionRegressionTest::RplTargetTest ()
{
  Buffer buffer;
  RplHeaderOption inputOption;
  RplHeaderOption outputOption;
  std::vector<uint8_t> inputData;
  std::vector<uint8_t> outputData;
  uint8_t rawPrefix[16];
  uint8_t flags = 0; 

  unsigned long expectedDataSize;

  std::fill(rawPrefix, rawPrefix+16, 0);
  rawPrefix[0] = 0xFF;
  rawPrefix[1] = 0xF0;

  Ipv6Prefix objectPrefix (rawPrefix);

  uint8_t prefixLengthBits = objectPrefix.GetPrefixLength ();
  uint8_t prefixLengthBytes = prefixLengthBits / 8;
  if (prefixLengthBits % 8 > 0)
  {
    prefixLengthBytes += 1;
  }

  expectedDataSize =  2*sizeof(uint8_t) +prefixLengthBytes;

  
  //std::cout << "Prefix Length: " << unsigned(objectPrefix.GetPrefixLength ()) << std::endl;

  inputOption.SetRplTarget (objectPrefix);

  buffer.AddAtStart (inputOption.GetSerializedSize ());

  //inputOption.Print ();

  inputOption.Serialize (buffer.Begin ());
  outputOption.Deserialize (buffer.Begin ());
  const RplHeaderOption::RplTarget &outputRplTarget = outputOption.GetRplTarget ();

  NS_TEST_ASSERT_MSG_EQ (+outputOption.GetType (), 0x05, "Type does not match");
  NS_TEST_ASSERT_MSG_EQ (+outputOption.GetOptionLength (), +expectedDataSize, "Option length does not match");
  NS_TEST_ASSERT_MSG_EQ (+outputRplTarget.flags, +flags, "Flag field differs from input");
  NS_TEST_ASSERT_MSG_EQ (+outputRplTarget.prefixLength, +prefixLengthBits, "Prefix Length field differs from input");
  NS_TEST_ASSERT_MSG_EQ (outputRplTarget.targetPrefix, objectPrefix, "Target Prefix differs from input");
}



void RplHeaderOptionRegressionTest::TransitInformationTest ()
{
  Buffer buffer;
  RplHeaderOption inputOption;
  RplHeaderOption outputOption;
  uint8_t flags = 0b10000000;
  uint8_t pathControl = 1;
  uint8_t pathSequence = 1;
  uint8_t pathLifetime = 1;
  Ipv6Address parentAddress;

  inputOption.SetTransitInformation(flags, pathControl, pathSequence, pathLifetime, parentAddress);

  buffer.AddAtStart (inputOption.GetSerializedSize ());

  inputOption.Serialize (buffer.Begin ());
  outputOption.Deserialize (buffer.Begin ());
  const RplHeaderOption::TransitInformation &outputTransitInformation = outputOption.GetTransitInformation ();

  NS_TEST_ASSERT_MSG_EQ (+outputOption.GetType (), 0x06, "type does not match");
  NS_TEST_ASSERT_MSG_EQ (+outputOption.GetOptionLength (), 20, "option length does not match");
  NS_TEST_ASSERT_MSG_EQ (+outputTransitInformation.flags, +flags, "flags differs from input");
  NS_TEST_ASSERT_MSG_EQ (+outputTransitInformation.pathControl, +pathControl, "pathControl differs from input");
  NS_TEST_ASSERT_MSG_EQ (+outputTransitInformation.pathSequence, +pathSequence, "pathSequence differs from input");
  NS_TEST_ASSERT_MSG_EQ (+outputTransitInformation.pathLifetime, +pathLifetime, "pathLifetime differs from input");
  NS_TEST_ASSERT_MSG_EQ (outputTransitInformation.parentAddress, parentAddress, "parentAddress differs from input");
}

void RplHeaderOptionRegressionTest::SolicitedInformationTest ()
{
  Buffer buffer;
  RplHeaderOption inputOption;
  RplHeaderOption outputOption;
  uint8_t flags = 0b10100000;
  uint8_t rplInstanceId = 1;
  uint8_t versionNumber = 1;
  Ipv6Address dodagId = Ipv6Address ("2001:db8::1");

  inputOption.SetSolicitedInformation (rplInstanceId, flags, dodagId, versionNumber);

  buffer.AddAtStart (inputOption.GetSerializedSize ());

  inputOption.Serialize (buffer.Begin ());
  outputOption.Deserialize (buffer.Begin ());
  const RplHeaderOption::SolicitedInformation &outputSolicitedInformation = outputOption.GetSolicitedInformation ();

  NS_TEST_ASSERT_MSG_EQ (+outputOption.GetType (), 0x07, "type does not match");
  NS_TEST_ASSERT_MSG_EQ (+outputOption.GetOptionLength (), 19, "option length does not match");
  NS_TEST_ASSERT_MSG_EQ (+outputSolicitedInformation.rplInstanceId, +rplInstanceId, "RPL Instance ID differs from input");
  NS_TEST_ASSERT_MSG_EQ (+outputSolicitedInformation.flags, +flags, "Flags differs from input");
  NS_TEST_ASSERT_MSG_EQ (outputSolicitedInformation.dodagId, dodagId, "DODAG ID differs from input");
  NS_TEST_ASSERT_MSG_EQ (+outputSolicitedInformation.versionNumber, +versionNumber, "Version Number differs from input");
}

void RplHeaderOptionRegressionTest::PrefixInformationTest ()
{
  Buffer buffer;
  RplHeaderOption inputOption;
  RplHeaderOption outputOption;
  uint8_t flags = 0b10100000;
  uint8_t prefixLength = 1;
  uint32_t validLifetime = 0x01020304;
  uint32_t preferredLifetime = 0x01020304;
  Ipv6Prefix prefix = Ipv6Prefix ("2001:db8::");

  inputOption.SetPrefixInformation (prefixLength, flags, validLifetime, preferredLifetime, prefix);

  buffer.AddAtStart (inputOption.GetSerializedSize ());

  inputOption.Serialize (buffer.Begin ());
  outputOption.Deserialize (buffer.Begin ());
  const RplHeaderOption::PrefixInformation &outputPrefixInformation = outputOption.GetPrefixInformation ();

  NS_TEST_ASSERT_MSG_EQ (+outputOption.GetType (), 0x08, "Type does not match");
  NS_TEST_ASSERT_MSG_EQ (+outputOption.GetOptionLength (), 30, "Option length does not match");
  NS_TEST_ASSERT_MSG_EQ (+outputPrefixInformation.prefixLength, +prefixLength, "Prefix Length differs from input");
  NS_TEST_ASSERT_MSG_EQ (+outputPrefixInformation.flags, +flags, "Flags differs from input");
  NS_TEST_ASSERT_MSG_EQ (+outputPrefixInformation.validLifetime, +validLifetime, "Valid Lifetime differs from input");
  NS_TEST_ASSERT_MSG_EQ (+outputPrefixInformation.preferredLifetime, +preferredLifetime, "Preferred Lifetime differs from input");
  NS_TEST_ASSERT_MSG_EQ (outputPrefixInformation.prefix, prefix, "Version Number differs from input");
}
void RplHeaderOptionRegressionTest::RplTargetDescriptorTest ()
{
  Buffer buffer;
  RplHeaderOption inputOption;
  RplHeaderOption outputOption;
  uint32_t descriptor = 0x01020304;

  inputOption.SetRplTargetDescriptor (descriptor);

  buffer.AddAtStart (inputOption.GetSerializedSize ());

  inputOption.Serialize (buffer.Begin ());
  outputOption.Deserialize (buffer.Begin ());
  const RplHeaderOption::RplTargetDescriptor &outputRplTargetDescriptor = outputOption.GetRplTargetDescriptor ();

  NS_TEST_ASSERT_MSG_EQ (+outputOption.GetType (), 0x09, "Type does not match");
  NS_TEST_ASSERT_MSG_EQ (+outputOption.GetOptionLength (), 4, "Option length does not match");
  NS_TEST_ASSERT_MSG_EQ (+outputRplTargetDescriptor.descriptor, +descriptor, "Descriptor differs from input");
}

}   //rpl
}   //ns3