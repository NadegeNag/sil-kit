// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include <future>
#include <vector>
#include <map>

#include "ib/mw/fwd_decl.hpp"
#include "ib/mw/sync/ITimeConsumer.hpp"
#include "ib/sim/rpc/IRpcServer.hpp"
#include "ib/sim/rpc/IRpcCallHandle.hpp"

#include "IComAdapterInternal.hpp"
#include "IIbToRpcServerInternal.hpp"
#include "RpcCallHandle.hpp"

namespace ib {
namespace sim {
namespace rpc {

class RpcServerInternal : public IIbToRpcServerInternal,
                  public mw::sync::ITimeConsumer,
                  public mw::IIbServiceEndpoint
{
  public:
    RpcServerInternal(mw::IComAdapterInternal* comAdapter, cfg::RpcPort config, mw::sync::ITimeProvider* timeProvider,
        ib::sim::rpc::CallProcessor handler, IRpcServer* parent);

    void SetRpcHandler(CallProcessor handler);

    void SubmitResult(IRpcCallHandle* callHandlePtr, const std::vector<uint8_t>& resultData);

    //! \brief Accepts messages originating from IB communications.
    void ReceiveIbMessage(const mw::IIbServiceEndpoint* from, const FunctionCall& msg) override;
    void ReceiveMessage(const FunctionCall& msg);

    void SetEndpointAddress(const mw::EndpointAddress& endpointAddress) override;
    auto EndpointAddress() const -> const mw::EndpointAddress& override;

    // ib::mw::sync::ITimeConsumer
    void SetTimeProvider(mw::sync::ITimeProvider* provider) override;

    // IIbServiceEndpoint
    inline void SetServiceDescriptor(const mw::ServiceDescriptor& serviceDescriptor) override;
    inline auto GetServiceDescriptor() const -> const mw::ServiceDescriptor& override;

  private:

    // private Members
    cfg::RpcPort             _config{};
    mw::IComAdapterInternal* _comAdapter{nullptr};
    mw::ServiceDescriptor    _serviceDescriptor{};
    mw::sync::ITimeProvider* _timeProvider{nullptr};
    CallProcessor            _handler;
    IRpcServer*              _parent;
    std::map<std::string, std::unique_ptr<CallHandleImpl>> _receivedCallHandles;

};

// ================================================================================
//  Inline Implementations
// ================================================================================

void RpcServerInternal::SetServiceDescriptor(const mw::ServiceDescriptor& serviceDescriptor)
{
    _serviceDescriptor = serviceDescriptor;
}

auto RpcServerInternal::GetServiceDescriptor() const -> const mw::ServiceDescriptor&
{
    return _serviceDescriptor;
}

} // namespace rpc
} // namespace sim
} // namespace ib