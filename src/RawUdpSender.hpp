//===- src/RawUdpSender.hpp -===//
//
// Part of the NS3-Usage-Example Project, under the GNU GPL License v3.0.
// See https://github.com/gabrielyanabraham/NS3-Usage-Example/blob/main/LICENSE for license information.
//
//===----------------------------------------------------------------------===//

#pragma once

// Headers from other projects
#include "ns3/application.h"
#include "ns3/packet-socket.h"
#include "ns3/address.h"
#include "ns3/ipv4-address.h"

namespace sim 
{

class RawUdpSender final : public ns3::Application 
{
public:
    RawUdpSender(ns3::Ptr<ns3::Node> node, 
                 ns3::Address destMac, 
                 ns3::Ipv4Address srcIp, 
                 ns3::Ipv4Address destIp,
                 const uint16_t srcPort, 
                 const uint16_t destPort);

private:
    void StartApplication() override;
    void StopApplication() override;
    void SendPacket();

    ns3::Ptr<ns3::Node> m_node;
    ns3::Address m_destMac;
    ns3::Ipv4Address m_srcIp; 
    ns3::Ipv4Address m_destIp;
    uint16_t m_srcPort; 
    uint16_t m_destPort;
    ns3::Ptr<ns3::PacketSocket> m_socket;
};

} // namespace sim
