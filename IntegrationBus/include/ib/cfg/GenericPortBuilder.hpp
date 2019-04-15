// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include "ib/IbMacros.hpp"

#include "Config.hpp"

#include "fwd_decl.hpp"
#include "ParentBuilder.hpp"

namespace ib {
namespace cfg {

class GenericPortBuilder : public ParentBuilder<ParticipantBuilder>
{
public:
    IntegrationBusAPI GenericPortBuilder(ParticipantBuilder *participant, std::string name, mw::EndpointId endpointId);

    IntegrationBusAPI auto WithLink(const std::string& linkname) -> GenericPortBuilder&;
    IntegrationBusAPI auto WithLinkId(int16_t linkId) -> GenericPortBuilder&;
    IntegrationBusAPI auto WithEndpointId(mw::EndpointId id) -> GenericPortBuilder&;

    IntegrationBusAPI auto WithProtocolType(GenericPort::ProtocolType protocolType) -> GenericPortBuilder&;
    IntegrationBusAPI auto WithDefinitionUri(std::string uri) -> GenericPortBuilder&;

    IntegrationBusAPI auto operator->() -> ParticipantBuilder*;

    IntegrationBusAPI auto Build() -> GenericPort;

private:
    GenericPort _port;
    std::string _link;
};

// ================================================================================
//  Inline Implementations
// ================================================================================

} // namespace cfg
} // namespace ib
