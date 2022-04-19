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
  RplHelper ( uint16_t instanceId = 0, 
              rpl::RplMop_e mop = rpl::MOP_STORING_NO_MULTICAST, 
              bool isRoot = false, 
              rpl::RplDisMop_e disMop = rpl::DIS_MOP_WAIT, 
              Time disMessageTime = Seconds (1.0), 
              int numberOfDisMessages = 5
            );

  
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
  void SetInstanceId (uint8_t value);

  void SetRoot (bool isRoot);
  void AssignRoot (NodeContainer c, uint8_t instanceId, rpl::RplMop_e mop, bool isRoot = true);
  void AssignRoot (NodeContainer c);

  /**
   * \brief sets the initialization mode of the node if it should wait for DIO messages or actively send DIS messages and become a root if no DIO message was received
   * 
   * \param c 
   * \param disMop 
   * \param disMessageTime 
   * \param numberOfDisMessages 
   * \param instanceId 
   * \param mop 
   */
  void AssignDisMop (NodeContainer c, rpl::RplDisMop_e disMop, Time disMessageTime, int numberOfDisMessages, uint8_t instanceId, rpl::RplMop_e mop);
  void AssignDisMop (NodeContainer c);
  /**
   * \brief Exclude an interface from RPL protocol.
   *
   * You have to call this function \a before installing RIPng in the nodes.
   *
   * Note: the exclusion means that RPL will not be propagated on that interface.
   * The network prefix on that interface will be still considered in RPL.
   *
   * \param node the node
   * \param interface the network interface to be excluded
   */
  void ExcludeInterface (Ptr<Node> node, uint32_t interface);


private:
  uint8_t m_instanceId = RPL_DEFAULT_INSTANCE;
  rpl::RplMop_e m_mop;
  bool m_isRoot;
  /// DIS Mode of Operation
  rpl::RplDisMop_e m_disMop;
  Time m_disMessageTime;
  int m_numberOfDisMessages;

  std::map< Ptr<Node>, std::set<uint32_t> > m_interfaceExclusions; // Interface Exclusion set
  RplHelper &operator = (const RplHelper &);
  ObjectFactory m_agentFactory; //!< Object factory
};


}
#endif /* RPL_HELPER_H */

