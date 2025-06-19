//===- src/RawUdpReceiver.cc -===//
//
// Part of the NS3-Usage-Example Project, under the GNU GPL License v3.0.
// See https://github.com/gabrielyanabraham/NS3-Usage-Example/blob/main/LICENSE for license information.
//
//===----------------------------------------------------------------------===//

// Headers from this project
#include "RawUdpReceiver.hpp"

// Headers from other projects
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "ns3/packet-socket-address.h"
#include "ns3/udp-socket-factory.h"

// Headers from standard libraries
#include <iostream>

namespace sim 
{

RawUdpReceiver::RawUdpReceiver(ns3::Ptr<ns3::Node> node) : m_node(node) {}

void RawUdpReceiver::StartApplication()
{
    using namespace ns3;

    TypeId tid = TypeId::LookupByName("ns3::PacketSocketFactory");
    Ptr<Socket> rawSocket = Socket::CreateSocket(m_node, tid);
    m_socket = DynamicCast<PacketSocket>(rawSocket);
    PacketSocketAddress addr;
    addr.SetSingleDevice(m_node->GetDevice(0)->GetIfIndex());
    addr.SetProtocol(0x0800); // IPv4
    m_socket->Bind(addr);
    m_socket->SetRecvCallback(MakeCallback(&RawUdpReceiver::HandleRead, this));

    Ptr<Socket> dummyUdp = Socket::CreateSocket(m_node, UdpSocketFactory::GetTypeId());
    InetSocketAddress dummyAddr = InetSocketAddress(Ipv4Address::GetAny(), 5678);
    dummyUdp->Bind(dummyAddr);
}


void RawUdpReceiver::StopApplication() 
{
    m_socket->Close();
}

void RawUdpReceiver::HandleRead(ns3::Ptr<ns3::Socket> socket) 
{
    ns3::Ptr<ns3::Packet> packet;
    while ((packet = socket->Recv())) 
    {
        std::cout << "Node " << m_node->GetId() << " received packet of size " << 
            packet->GetSize() << " bytes at time " << ns3::Simulator::Now().GetSeconds() << "s" << std::endl;

    }
}

} // namespace sim
