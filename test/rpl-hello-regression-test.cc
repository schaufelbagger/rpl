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
#include "rpl-hello-regression-test.h"
#include "ns3/simulator.h"
#include "ns3/rpl.h"
#include "ns3/rpl-helper.h"

namespace ns3 {
NS_LOG_COMPONENT_DEFINE("RPL hello regression test");
namespace rpl {

RplHelloRegressionTest::RplHelloRegressionTest () :
  TestCase ("Test RPL Hello messages generation"),
  m_time (Seconds (5))
{
}

RplHelloRegressionTest::~RplHelloRegressionTest ()
{
}

void RplHelloRegressionTest::DoRun ()
{
  CreateNodes ();

  Simulator::Stop (m_time);
  Simulator::Run ();
  Simulator::Destroy ();
}

void RplHelloRegressionTest::CreateNodes ()
{

}


}   //rpl
}   //ns3