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
#include "rpl-helper.h"
#include "ns3/rpl.h"
#include "ns3/node-list.h"
#include "ns3/names.h"
#include "ns3/ptr.h"

namespace ns3 {


RplHelper::RplHelper(uint16_t instanceId, rpl::RplMop_e mop, bool isRoot)
:
m_instanceId (instanceId),
m_mop (mop),
m_isRoot (isRoot)
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

  //agent->SetInstanceId (m_instanceId);
  
  std::map<Ptr<Node>, std::set<uint32_t> >::const_iterator it = m_interfaceExclusions.find (node);
  if(it != m_interfaceExclusions.end ())
  {
    agent->SetInterfaceExclusions (it->second);
  }

  node->AggregateObject (agent);
  return agent;
}

void RplHelper::Set (std::string name, const AttributeValue &value)
{
  m_agentFactory.Set (name, value);
}

void RplHelper::SetInstanceId (uint8_t value)
{
  m_instanceId = value;
}
void RplHelper::SetRoot (bool value)
{
  m_isRoot = value;
}

void RplHelper::AssignRoot (NodeContainer c, uint8_t instanceId, rpl::RplMop_e mop, bool isRoot)
{
  Ptr<Node> node;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      node = (*i);
      Ptr<Ipv6> ipv6 = node->GetObject<Ipv6> ();
      NS_ASSERT_MSG (ipv6, "Ipv6 not installed on node");
      Ptr<Ipv6RoutingProtocol> proto = ipv6->GetRoutingProtocol ();
      NS_ASSERT_MSG (proto, "Ipv6 routing not installed on node");
      Ptr<rpl::RoutingProtocol> rpl = DynamicCast<rpl::RoutingProtocol> (proto);
      if (rpl)
      {
        rpl->SetRoot (isRoot);
        rpl->SetInstanceId (instanceId);
        rpl->SetMop (mop);
        continue;
      }
      // RPL may also be in a list
      Ptr<Ipv6ListRouting> list = DynamicCast<Ipv6ListRouting> (proto);
      if (list)
        {
          int16_t priority;
          Ptr<Ipv6RoutingProtocol> listProto;
          Ptr<rpl::RoutingProtocol> listRpl;
          for (uint32_t i = 0; i < list->GetNRoutingProtocols (); i++)
            {
              listProto = list->GetRoutingProtocol (i, priority);
              listRpl = DynamicCast<rpl::RoutingProtocol> (listProto);
              if (listRpl)
                {
                  listRpl->SetRoot (isRoot);
                  listRpl->SetInstanceId (instanceId);
                  listRpl->SetMop (mop);
                  break;
                }
            }
        }
    }
  return;
}
void RplHelper::AssignRoot (NodeContainer c)
{
  return AssignRoot (c, m_instanceId, m_mop, true);
}

void RplHelper::ExcludeInterface (Ptr<Node> node, uint32_t interface)
{
  std::map< Ptr<Node>, std::set<uint32_t> >::iterator it = m_interfaceExclusions.find (node);

  if (it == m_interfaceExclusions.end ())
    {
      std::set<uint32_t> interfaces;
      interfaces.insert (interface);

      m_interfaceExclusions.insert (std::make_pair (node, interfaces));
    }
  else
    {
      it->second.insert (interface);
    }
}

}
