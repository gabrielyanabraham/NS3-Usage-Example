//===- src/RawUdpSender.cc -===//
//
// Part of the NS3-Usage-Example Project, under the GNU GPL License v3.0.
// See https://github.com/gabrielyanabraham/NS3-Usage-Example/blob/main/LICENSE for license information.
//
//===----------------------------------------------------------------------===//

// Headers from this project
#include "RawUdpSender.hpp"

// Headers from other projects
#include "ns3/packet.h"
#include "ns3/udp-header.h"
#include "ns3/ipv4-header.h"
#include "ns3/simulator.h"
#include "ns3/packet-socket-address.h"
#include "ns3/mac48-address.h"
#include "ns3/log.h"

namespace sim 
{

RawUdpSender::RawUdpSender(ns3::Ptr<ns3::Node> node, 
                           ns3::Address destMac, 
                           ns3::Ipv4Address srcIp, 
                           ns3::Ipv4Address destIp,
                           const uint16_t srcPort, 
                           const uint16_t destPort) : m_node(node), m_destMac(destMac), 
                                                      m_srcIp(srcIp), m_destIp(destIp),
                                                      m_srcPort(srcPort), m_destPort(destPort) {}

void RawUdpSender::StartApplication() 
{
    NS_LOG_FUNCTION(this);

    if (!m_node || m_node->GetNDevices() == 0) 
    {
        NS_LOG_ERROR("Invalid node or no devices available");
        return;
    }

    ns3::TypeId tid = ns3::TypeId::LookupByName("ns3::PacketSocketFactory");
    ns3::Ptr<ns3::Socket> socket = ns3::Socket::CreateSocket(m_node, tid);
    m_socket = ns3::DynamicCast<ns3::PacketSocket>(socket);
    NS_ASSERT(m_socket != nullptr);

    ns3::PacketSocketAddress addr;
    addr.SetSingleDevice(m_node->GetDevice(0)->GetIfIndex());
    addr.SetPhysicalAddress(m_destMac);
    addr.SetProtocol(0x0800); // IPv4

    if (m_socket->Bind() != 0) 
    {
        NS_LOG_ERROR("Failed to bind socket");
        return;
    }
    if (m_socket->Connect(addr) != 0) 
    {
        NS_LOG_ERROR("Failed to connect socket");
        return;
    }

    ns3::Simulator::Schedule(ns3::Seconds(1.0), &RawUdpSender::SendPacket, this);
}

void RawUdpSender::StopApplication() 
{
    NS_LOG_FUNCTION(this);
    if (m_socket) 
    {
        m_socket->Close();
        m_socket = nullptr;
    }
}

void RawUdpSender::SendPacket() 
{
    NS_LOG_FUNCTION(this);

    if (!m_socket) 
    {
        NS_LOG_ERROR("Socket not initialized");
        return;
    }

    ns3::Ptr<ns3::Packet> payload = ns3::Create<ns3::Packet>(100);

    ns3::UdpHeader udpHeader;
    udpHeader.SetSourcePort(m_srcPort);
    udpHeader.SetDestinationPort(m_destPort);
    udpHeader.EnableChecksums();
    payload->AddHeader(udpHeader);

    ns3::Ipv4Header ipHeader;
    ipHeader.SetSource(m_srcIp);
    ipHeader.SetDestination(m_destIp);
    ipHeader.SetProtocol(17); // UDP
    ipHeader.SetTtl(64);
    ipHeader.SetPayloadSize(payload->GetSize()); // UDP header + payload
    ipHeader.SetDontFragment();
    ipHeader.SetIdentification(1234);
    ipHeader.EnableChecksum();
    payload->AddHeader(ipHeader);

    int bytesSent = m_socket->Send(payload, 0);
    if (bytesSent < 0) 
    {
        NS_LOG_ERROR("Failed to send packet");
    } 
    else 
    {
        NS_LOG_INFO("Sent packet of size " << payload->GetSize() << " bytes");
    }
}

} // namespace sim
