#ifndef RPL_EXAMPLE_HELPER_H
#define RPL_EXAMPLE_HELPER_H

#include <ostream>

#include "ns3/core-module.h"
#include "ns3/rpl-helper.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/internet-apps-module.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/mobility-module.h"
#include "ns3/propagation-module.h"
#include "ns3/sixlowpan-module.h"
#include "ns3/applications-module.h"
#include "ns3/trace-helper.h"
#include "ns3/node.h"

using namespace ns3;

// Getter
Ptr<rpl::RoutingProtocol> GetRpl(Ptr <Node> node);
Ptr<RipNg> GetRipNg(Ptr <Node> node);
Ptr<MobilityModel> GetMobilityModel(Ptr <Node> node);

// Actions
void SilenceNode (Ptr<Node> node, int32_t interface);
void TurnOnNode (Ptr<Node> node, int32_t interface, Ipv6Address address);
int GetNextHighestSquareEdgeLength(int number);
std::string get_param_string (std::string routingProtocol, int run, int numberOfNodes, double trafficInterval, double applicationStart, double simulationTime, int networkSetup, int appSetup);

// Traces
void UpdatePrefParentTraceSink(Ptr<OutputStreamWrapper> stream, rpl::RplNode rplNode);
void RouteAddedTraceSink(Ptr<OutputStreamWrapper> stream, rpl::RplRoutingTableEntry entry);
void UdpTxTraceWithAddressesSink (Ptr<OutputStreamWrapper> stream, Ptr< const Packet > packet, const Address & from, const Address & localAddress);
void UdpRxTraceWithAddressesSink (Ptr<OutputStreamWrapper> stream, Ptr< const Packet > packet, const Address & from, const Address & localAddress);
void CourseChangeTraceSink (Ptr<OutputStreamWrapper> stream, Ptr<const MobilityModel> mobility);
void RemainingEnergy (Ptr<OutputStreamWrapper> stream, double oldValue, double remainingEnergy);
void RipNgRouteAddedTraceSink(Ptr<OutputStreamWrapper> stream, RipNgRoutingTableEntry entry);
#endif // RPL_EXAMPLE_HELPER_H