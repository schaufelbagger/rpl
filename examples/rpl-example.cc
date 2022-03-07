/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/rpl-helper.h"
#include "ns3/internet-module.h"
#include "ns3/internet-stack-helper.h"

using namespace ns3;


int main (int argc, char *argv[])
{
    bool verbose = true;

    CommandLine cmd (__FILE__);
    cmd.AddValue ("verbose", "Tell application to log if true", verbose);

    cmd.Parse (argc,argv);

    NS_LOG_UNCOND("rpl example\n\n");
    // create 2 nodes
    NodeContainer nodes;
    nodes.Create(2);
    // install TCP/IP & RPL
    RplHelper rpl;
    InternetStackHelper internetv6;
    internetv6.SetRoutingHelper(rpl);
    internetv6.Install(nodes);


    Simulator::Run ();
    Simulator::Destroy ();
    return 0;
}


