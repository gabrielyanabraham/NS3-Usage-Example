//===- src/main.cc -===//
//
// Part of the NS3-Usage-Example Project, under the GNU GPL License v3.0.
// See https://github.com/gabrielyanabraham/NS3-Usage-Example/blob/main/LICENSE for license information.
//
//===----------------------------------------------------------------------===//

// Headers from this project
#include "RawUdpSender.hpp"
#include "RawUdpReceiver.hpp"

// Headers from other projects
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/bridge-module.h"
#include "ns3/packet-socket-helper.h"
#include "ns3/packet-socket-address.h"
#include "ns3/packet-socket-factory.h"

int main() 
{
    using namespace ns3;

    NodeContainer nodes;
    nodes.Create(6); // n0 to n5

    CsmaHelper csma;
    csma.SetChannelAttribute("DataRate", StringValue("10Mbps"));
    csma.SetChannelAttribute("Delay", StringValue("2ms"));

    NetDeviceContainer link01 = csma.Install(NodeContainer(nodes.Get(0), nodes.Get(2))); // n0-n2
    NetDeviceContainer link12 = csma.Install(NodeContainer(nodes.Get(1), nodes.Get(2))); // n1-n2
    NetDeviceContainer link34 = csma.Install(NodeContainer(nodes.Get(3), nodes.Get(4))); // n3-n4
    NetDeviceContainer link35 = csma.Install(NodeContainer(nodes.Get(3), nodes.Get(5))); // n3-n5

    CsmaHelper csma23;
    csma23.SetChannelAttribute("DataRate", StringValue("1.5Mbps"));
    csma23.SetChannelAttribute("Delay", StringValue("20ms"));
    NetDeviceContainer link23 = csma23.Install(NodeContainer(nodes.Get(2), nodes.Get(3))); // n2-n3

    BridgeHelper bridge;
    NetDeviceContainer bridgeN2;
    bridgeN2.Add(link01.Get(1)); // n2 to n0
    bridgeN2.Add(link12.Get(1)); // n2 to n1
    bridgeN2.Add(link23.Get(0)); // n2 to n3
    bridge.Install(nodes.Get(2), bridgeN2);

    NetDeviceContainer bridgeN3;
    bridgeN3.Add(link23.Get(1)); // n3 to n2
    bridgeN3.Add(link34.Get(0)); // n3 to n4
    bridgeN3.Add(link35.Get(0)); // n3 to n5
    bridge.Install(nodes.Get(3), bridgeN3);

    PacketSocketHelper packetSocket;
    packetSocket.Install(nodes);

    InternetStackHelper stack;
    stack.Install(nodes);
    Ipv4AddressHelper ipv4;
    ipv4.SetBase("192.168.1.0", "255.255.255.0");
    Ipv4InterfaceContainer interfaces;
    interfaces.Add(ipv4.Assign(link01.Get(0))); // n0
    interfaces.Add(ipv4.Assign(link12.Get(0))); // n1
    interfaces.Add(ipv4.Assign(link34.Get(1))); // n4
    interfaces.Add(ipv4.Assign(link35.Get(1))); // n5

    Address macN0 = link01.Get(0)->GetAddress();
    Address macN1 = link12.Get(0)->GetAddress();
    Address macN4 = link34.Get(1)->GetAddress();
    Address macN5 = link35.Get(1)->GetAddress();

    Ptr<sim::RawUdpSender> senderN0 = CreateObject<sim::RawUdpSender>(
        nodes.Get(0), macN5, interfaces.GetAddress(0), interfaces.GetAddress(3), 1234, 5678);
    nodes.Get(0)->AddApplication(senderN0);
    senderN0->SetStartTime(Seconds(1.0));
    senderN0->SetStopTime(Seconds(10.0));

    Ptr<sim::RawUdpSender> senderN5 = CreateObject<sim::RawUdpSender>(
        nodes.Get(5), macN1, interfaces.GetAddress(3), interfaces.GetAddress(1), 1234, 5678);
    nodes.Get(5)->AddApplication(senderN5);
    senderN5->SetStartTime(Seconds(2.0));
    senderN5->SetStopTime(Seconds(10.0));

    Ptr<sim::RawUdpSender> senderN4toN1 = CreateObject<sim::RawUdpSender>(
        nodes.Get(4), macN1, interfaces.GetAddress(2), interfaces.GetAddress(1), 1234, 5678);
    nodes.Get(4)->AddApplication(senderN4toN1);
    senderN4toN1->SetStartTime(Seconds(3.0));
    senderN4toN1->SetStopTime(Seconds(10.0));

    Ptr<sim::RawUdpSender> senderN4toN0 = CreateObject<sim::RawUdpSender>(
        nodes.Get(4), macN0, interfaces.GetAddress(2), interfaces.GetAddress(0), 1234, 5678);
    nodes.Get(4)->AddApplication(senderN4toN0);
    senderN4toN0->SetStartTime(Seconds(4.0));
    senderN4toN0->SetStopTime(Seconds(10.0));

    for (const uint32_t i : {0, 1, 4, 5}) 
    {
        Ptr<sim::RawUdpReceiver> receiver = CreateObject<sim::RawUdpReceiver>(nodes.Get(i));
        nodes.Get(i)->AddApplication(receiver);
        receiver->SetStartTime(Seconds(0.0));
        receiver->SetStopTime(Seconds(10.0));
    }

    NetDeviceContainer pcapDevices;
    pcapDevices.Add(link01.Get(0)); // n0
    pcapDevices.Add(link12.Get(0)); // n1
    pcapDevices.Add(link34.Get(1)); // n4
    pcapDevices.Add(link35.Get(1)); // n5
    csma.EnablePcap("udp-raw-socket", pcapDevices, true);

    Simulator::Stop(Seconds(10.0));
    Simulator::Run();
    Simulator::Destroy();

    return EXIT_SUCCESS;
}
