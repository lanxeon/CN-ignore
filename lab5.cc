/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 */

// Network topology
//
//       n0    n1   n2   n3
//       |     |    |    |
//     =====================
//
//  node n0,n1,n3 pings to node n2
//  node n0 generates protocol 2 (IGMP) to node n3

#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/applications-module.h"
#include "ns3/internet-apps-module.h"
#include "ns3/internet-module.h"
#include "ns3/flow-monitor-module.h"

using namespace ns3;


int
main (int argc, char *argv[])
{


  // Here, we will explicitly create four nodes.
  NodeContainer c;
  c.Create (6);

  // connect all our nodes to a shared channel.

  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", DataRateValue (DataRate (10000000)));
  csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));
  csma.SetDeviceAttribute ("EncapsulationMode", StringValue ("Llc"));
  NetDeviceContainer devs = csma.Install (c);

  // add an ip stack to all nodes.

  InternetStackHelper ipStack;
  ipStack.Install (c);

  // assign ip addresses

  Ipv4AddressHelper ip;
  ip.SetBase ("192.168.1.0", "255.255.255.0");
  Ipv4InterfaceContainer addresses = ip.Assign (devs);

  /*NS_LOG_INFO ("Create Source");
  Config::SetDefault ("ns3::Ipv4RawSocketImpl::Protocol", StringValue ("2"));
  InetSocketAddress dst = InetSocketAddress (addresses.GetAddress (3));
  OnOffHelper onoff = OnOffHelper ("ns3::Ipv4RawSocketFactory", dst);
  onoff.SetConstantRate (DataRate (15000));
  onoff.SetAttribute ("PacketSize", UintegerValue (1200));


  ApplicationContainer apps = onoff.Install (c.Get (0));
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (10.0));

  NS_LOG_INFO ("Create Sink.");
  PacketSinkHelper sink = PacketSinkHelper ("ns3::Ipv4RawSocketFactory", dst);
  apps = sink.Install (c.Get (3));
  apps.Start (Seconds (0.0));
  apps.Stop (Seconds (11.0));
*/
 

  InetSocketAddress dst = InetSocketAddress (addresses.GetAddress (3));
  OnOffHelper onoff = OnOffHelper ("ns3::UdpSocketFactory", dst);
  onoff.SetAttribute ("OnTime",  StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
  onoff.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
  onoff.SetAttribute ("PacketSize", UintegerValue (1100));
  onoff.SetAttribute ("DataRate", StringValue ("50Mbps")); //bit/s

  ApplicationContainer apps = onoff.Install (c.Get (0));
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (10.0));


  PacketSinkHelper sink = PacketSinkHelper ("ns3::UdpSocketFactory", dst);
  apps = sink.Install (c.Get (3));
  apps.Start (Seconds (0.0));
  apps.Stop (Seconds (11.0));



  V4PingHelper ping = V4PingHelper (addresses.GetAddress (2));
  NodeContainer pingers;
  pingers.Add (c.Get (2));
  pingers.Add (c.Get (1));
  pingers.Add (c.Get (3));
  pingers.Add (c.Get (4));
  pingers.Add (c.Get (5));
  apps = ping.Install (pingers);
  apps.Start (Seconds (2.0));
  apps.Stop (Seconds (10.0));

  FlowMonitorHelper flowmon;
  Ptr<FlowMonitor> monitor = flowmon.InstallAll();

  Simulator::Stop (Seconds (10));
  Simulator::Run ();

  monitor->CheckForLostPackets ();
  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier ());
  FlowMonitor::FlowStatsContainer stats = monitor->GetFlowStats ();
  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
    {
      Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
      
      std::cout << "Flow " << i->first  << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")\n";
      std::cout << "  Tx Packets: " << i->second.txPackets << "\n";
      std::cout << "  Rx Packets: " << i->second.rxPackets << "\n";
      std::cout << "  Lost Packets: " << i->second.lostPackets << "\n";
      std::cout << "  Throughput: " << i->second.rxBytes * 8.0 / 9.0 / 1000 / 1000  << " Mbps\n";
    }
  Simulator::Destroy ();
  return 0;
}
