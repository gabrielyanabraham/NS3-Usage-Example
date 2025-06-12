//===- src/RawUdpReceiver.hpp -===//
//
// Part of the NS3-Usage-Example Project, under the GNU GPL License v3.0.
// See https://github.com/gabrielyanabraham/NS3-Usage-Example/blob/main/LICENSE for license information.
//
//===----------------------------------------------------------------------===//

#pragma once

// Headers from other projects
#include "ns3/application.h"
#include "ns3/packet-socket.h"

namespace sim 
{

class RawUdpReceiver final : public ns3::Application 
{
public:
    RawUdpReceiver(ns3::Ptr<ns3::Node> node);

private:
    void StartApplication() override;
    void StopApplication() override;
    void HandleRead(ns3::Ptr<ns3::Socket> socket);

    ns3::Ptr<ns3::Node> m_node;
    ns3::Ptr<ns3::PacketSocket> m_socket;
};

} // namespace sim
