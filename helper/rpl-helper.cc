/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "rpl-helper.h"
#include "ns3/rpl.h"
#include "ns3/node-list.h"
#include "ns3/names.h"
#include "ns3/ptr.h"

namespace ns3 {


RplHelper::RplHelper()
{
  m_agentFactory.SetTypeId ("ns3::rpl::RoutingProtocol");
}

RplHelper::RplHelper (const RplHelper &o) : m_agentFactory (o.m_agentFactory)
{
}

RplHelper::~RplHelper ()
{
}

RplHelper* RplHelper::Copy (void) const
{
  return new RplHelper(*this);
}

Ptr<Ipv6RoutingProtocol> RplHelper::Create (Ptr<Node> node) const
{
  Ptr<rpl::RoutingProtocol> agent = m_agentFactory.Create<rpl::RoutingProtocol> ();

  node->AggregateObject (agent);
  return agent;
}

void RplHelper::Set (std::string name, const AttributeValue &value)
{
  m_agentFactory.Set (name, value);
}

}
