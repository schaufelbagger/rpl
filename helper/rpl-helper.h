/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef RPL_HELPER_H
#define RPL_HELPER_H

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
  RplHelper ();

  
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


private:
  RplHelper &operator = (const RplHelper &);
  ObjectFactory m_agentFactory; //!< Object factory
};

}

#endif /* RPL_HELPER_H */

