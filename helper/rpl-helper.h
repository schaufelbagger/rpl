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
#ifndef RPL_HELPER_H
#define RPL_HELPER_H

#include <string>
#include "ns3/rpl.h"
#include "ns3/object-factory.h"
#include "ns3/node.h"
#include "ns3/node-container.h"
#include "ns3/ipv6-routing-helper.h"

namespace ns3 {

/**
 * \ingroup rpl
 *
 * \brief Helper class that adds RPL routing to nodes.
 *
 * This class is expected to be used in conjunction with
 * ns3::InternetStackHelper::SetRoutingHelper
 */
class RplHelper : public Ipv6RoutingHelper
{
public:
    
  /**
   * Create an RplHelper that makes life easier for people who want to install
   * RPL routing to nodes.
   */
  RplHelper (uint16_t instanceId = 0);

  
  ~RplHelper ();
  
  /**
   * \brief Construct an RplHelper from another previously initialized instance
   * (Copy Constructor).
   *
   * \param o object to copy
   */
  RplHelper (const RplHelper &o);


  /**
   * \returns pointer to clone of this RplHelper
   *
   * This method is mainly for internal use by the other helpers;
   * clients are expected to free the dynamic memory allocated by this method
   */
  RplHelper* Copy (void) const;

  /**
   * \param node the node on which the routing protocol will run
   * \returns a newly-created routing protocol
   *
   * This method will be called by ns3::InternetStackHelper::Install
   */
  Ptr<Ipv6RoutingProtocol> Create (Ptr<Node> node) const;

  /**
   * \param name the name of the attribute to set
   * \param value the value of the attribute to set.
   *
   * This method controls the attributes of ns3::rpl::RoutingProtocol
   */
  void Set (std::string name, const AttributeValue &value);

  /**
   * \brief sets the rpl instance ID
   * 
   * \param value the value of the rpl instance ID to set.
   */
  void SetInstanceId (uint16_t value);

private:
  uint16_t m_instanceId;
  RplHelper &operator = (const RplHelper &);
  ObjectFactory m_agentFactory; //!< Object factory
};

}

#endif /* RPL_HELPER_H */

