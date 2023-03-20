/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2011-2015  Regents of the University of California.
 *
 * This file is part of ndnSIM. See AUTHORS for complete list of ndnSIM authors
 *and contributors.
 *
 * ndnSIM is free software: you can redistribute it and/or modify it under the
 *terms of the GNU General Public License as published by the Free Software
 *Foundation, either version 3 of the License, or (at your option) any later
 *version.
 *
 * ndnSIM is distributed in the hope that it will be useful, but WITHOUT ANY
 *WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 *A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ndnSIM, e.g., in COPYING.md file.  If not, see
 *<http://www.gnu.org/licenses/>.
 **/

// ndn-kademlia.cpp

#include "ns3/core-module.h"
#include "ns3/ndnSIM-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"

namespace ns3 {

/**
 * This scenario simulates a very kademlia network topology:
 *
 *
 *      +----------+     1Mbps      +--------+     1Mbps      +----------+
 *      | consumer | <------------> | router | <------------> | producer |
 *      +----------+         10ms   +--------+          10ms  +----------+
 *
 *
 * Consumer requests data from producer with frequency 10 interests per second
 * (interests contain constantly increasing sequence number).
 *
 * For every received interest, producer replies with a data packet, containing
 * 1024 bytes of virtual payload.
 *
 * To run scenario and see what is happening, use the following command:
 *
 *     NS_LOG=ndn.Consumer:ndn.Producer ./waf --run=ndn-kademlia
 */

using ns3::ndn::FibHelper;
using ns3::ndn::GlobalRoutingHelper;

vector<string>
split(string str, string separator)
{
  if (separator == "")
    return {str};
  vector<string> result;
  string tstr = str + separator;
  size_t l = tstr.length(), sl = separator.length();
  string::size_type pos = 0, prev = 0;

  for (; pos < l && (pos = tstr.find(separator, pos)) != string::npos; prev = (pos += sl)) {
    result.emplace_back(tstr, prev, pos - prev);
  }
  return result;
}

int
main(int argc, char* argv[])
{
  // setting default parameters for PointToPoint links and channels
  Config::SetDefault("ns3::PointToPointNetDevice::DataRate", StringValue("1Mbps"));
  Config::SetDefault("ns3::PointToPointChannel::Delay", StringValue("10ms"));
  Config::SetDefault("ns3::QueueBase::MaxSize", StringValue("20p"));

  std::string prefix = "/nakazato.lab/test";

  // Read optional command-line parameters (e.g., enable visualizer with ./waf
  // --run=<> --visualize
  CommandLine cmd;
  cmd.Parse(argc, argv);

  /**
  int len = array.size();
  uint32_t nodeCount = 3;
  for (uint32_t i = 0; i < nodeCount; i++)
    {
      const uuid id = random_generator () ();
      std::string contentHash = hash.encode (boost::lexical_cast<std::string> (id)).to_string ();
      array.push_back (contentHash);
    }
  */

  AnnotatedTopologyReader topologyReader("", 25);
  topologyReader.SetFileName("src/ndnSIM/examples/topologies/geant.txt");
  ns3::NodeContainer nodes = topologyReader.Read();
  topologyReader.SaveGraphviz("topo.dot");

  // Install NDN stack on all nodes
  ndn::StackHelper ndnHelper;
  // ndnHelper.setPolicy("nfd::cs::lru");

  char* csSizeStr = getenv("CS_SIZE");
  int csSize(csSizeStr ? atoi(csSizeStr) : 0);

  ndnHelper.setCsSize(csSize);
  ndnHelper.SetDefaultRoutes(true);
  ndnHelper.InstallAll();

  // Choosing forwarding strategy
  ndn::StrategyChoiceHelper::InstallAll("/", "/localhost/nfd/strategy/best-route");

  // Installing global routing interface on all nodes
  GlobalRoutingHelper ndnGlobalRoutingHelper;
  ndnGlobalRoutingHelper.InstallAll();

  // Installing applications

  char* tmp = getenv("ID_PRD");

  string env_prdId(tmp ? tmp : "");
  if (env_prdId.empty()) {
    cerr << "[ERROR] No such variable found! ID_PRD" << endl;
    exit(EXIT_FAILURE);
  }
  vector<string> vecProdId = split(env_prdId, ",");

  tmp = getenv("ID_CON_MJ");
  string env_conIdMajor(tmp ? tmp : "");
  if (env_conIdMajor.empty()) {
    cerr << "[ERROR] No such variable found! ID_CON_MJ" << endl;
    exit(EXIT_FAILURE);
  }
  vector<string> vecConMajorId = split(env_conIdMajor, ",");

  tmp = getenv("ID_CON_MN");
  string env_conIdMinor(tmp ? tmp : "");
  if (env_conIdMinor.empty()) {
    cerr << "[ERROR] No such variable found! ID_CON_MN" << endl;
    exit(EXIT_FAILURE);
  }
  vector<string> vecConMinorId = split(env_conIdMinor, ",");

  // Producer
  Ptr<Node> producerPrimary = Names::Find<Node>("rtr-" + vecProdId[0]);
  ndn::AppHelper producerPrimaryHelper("ns3::ndn::Producer");
  producerPrimaryHelper.SetAttribute("PayloadSize", StringValue("1024"));
  producerPrimaryHelper.SetPrefix(prefix + "/major");
  producerPrimaryHelper.Install(producerPrimary); // last node

  Ptr<Node> producerSecondary = Names::Find<Node>("rtr-" + vecProdId[1]);
  ndn::AppHelper producerSecondaryHelper("ns3::ndn::Producer");
  producerSecondaryHelper.SetAttribute("PayloadSize", StringValue("1024"));
  producerSecondaryHelper.SetPrefix(prefix + "/minor");
  producerSecondaryHelper.Install(producerSecondary); // last node

  if (vecProdId.size() == 3) {
    Ptr<Node> producerTertiary = Names::Find<Node>("rtr-" + vecProdId[2]);
    ndn::AppHelper producerTertiaryHelper("ns3::ndn::Producer");
    producerTertiaryHelper.SetAttribute("PayloadSize", StringValue("1024"));
    producerTertiaryHelper.SetPrefix(prefix + "/1");
    producerTertiaryHelper.Install(producerTertiary); // last node
  }

  // Consumer config - major
  NodeContainer majorConsumerNodes;

  for (string id : vecConMajorId) {
    majorConsumerNodes.Add(Names::Find<Node>("rtr-" + id));
  }

  ndn::AppHelper majorConsumerHelper("ns3::ndn::ConsumerZipfMandelbrot");
  majorConsumerHelper.SetPrefix(prefix + "/major");
  majorConsumerHelper.SetAttribute("Frequency", StringValue("5")); // 100 interests a second
  majorConsumerHelper.Install(majorConsumerNodes);

  // Consumer config - minor
  ndn::AppHelper minorConsumerHelper("ns3::ndn::ConsumerZipfMandelbrot");
  minorConsumerHelper.SetPrefix(prefix + "/minor");
  minorConsumerHelper.SetAttribute("Frequency", StringValue("5"));
  minorConsumerHelper.Install(majorConsumerNodes);

  if (vecProdId.size() == 3) {
    // Consumer config - 1
    ndn::AppHelper ConsumerHelper1("ns3::ndn::ConsumerZipfMandelbrot");
    ConsumerHelper1.SetPrefix(prefix + "/1");
    ConsumerHelper1.SetAttribute("Frequency", StringValue("5"));
    ConsumerHelper1.Install(majorConsumerNodes);
  }

  // ndnGlobalRoutingHelper.AddOrigins(prefix + "/major", producerPrimary);
  // ndnGlobalRoutingHelper.AddOrigins(prefix + "/minor", producerSecondary);
  
  #include "route.h"

  // Calculate and install FIBs
  GlobalRoutingHelper::CalculateRoutes();

  Simulator::Stop(Seconds(10.0));

  ndn::L3RateTracer::InstallAll("rate-trace.tsv", Seconds(0.5));
  L2RateTracer::InstallAll("drop-trace.tsv", Seconds(0.5));
  ndn::AppDelayTracer::InstallAll("app-delays-trace.tsv");

  Simulator::Run();
  Simulator::Destroy();

  return 0;
}
}
int
main(int argc, char* argv[])
{
  return ns3::main(argc, argv);
}
