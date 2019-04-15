// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include "ib/mw/fwd_decl.hpp"

#include "ib/sim/io/IInPort.hpp"
#include "ib/sim/io/IIbToInPort.hpp"
#include "ib/cfg/Config.hpp"

#include <tuple>
#include <vector>

namespace ib {
namespace sim {
namespace io {

template<typename MsgT>
class InPort :
    public IInPort<MsgT>,
    public IIbToInPort<MsgT>
{
public:
    // ----------------------------------------
    // Public Data Types
    using typename IInPort<MsgT>::MessageType;
    using typename IInPort<MsgT>::ValueType;
    using typename IInPort<MsgT>::ConfigType;

    template<typename T>
    using CallbackT = typename IInPort<MsgT>::template CallbackT<T>;
    using typename IInPort<MsgT>::MessageHandler;
    using typename IInPort<MsgT>::ValueHandler;

public:
    // ----------------------------------------
    // Constructors and Destructor
    InPort() = delete;
    InPort(const InPort&) = default;
    InPort(InPort&&) = default;
    InPort(mw::IComAdapter* comAdapter);
    InPort(mw::IComAdapter* comAdapter, ConfigType config);

public:
    // ----------------------------------------
    // Operator Implementations
    InPort& operator=(InPort& other) = default;
    InPort& operator=(InPort&& other) = default;

public:
    // ----------------------------------------
    // Public interface methods
    //
    // IInPort
    auto Config() const -> const ConfigType& override;
    auto Read() const -> const ValueType& override;
    void RegisterHandler(MessageHandler handler) override;
    void RegisterHandler(ValueHandler handler) override;

     // IIbToInPort
    void ReceiveIbMessage(mw::EndpointAddress from, const MessageType& msg) override;

    //! \brief Setter and getter for the EndpointAddress associated with this controller
    void SetEndpointAddress(const mw::EndpointAddress& endpointAddress) override;
    auto EndpointAddress() const -> const mw::EndpointAddress& override;

public:
    // ----------------------------------------
    // Public interface methods

private:
    // ----------------------------------------
    // private data types
    template<typename T>
    using CallbackVector = std::vector<CallbackT<T>>;

private:
    // ----------------------------------------
    // private methods
    template<typename T>
    void RegisterHandlerImpl(CallbackT<T> handler);

    template<typename T>
    void CallHandlers(const T& msg);

private:
    // ----------------------------------------
    // private members
    ConfigType _config;
    mw::IComAdapter* _comAdapter{nullptr};
    mw::EndpointAddress _endpointAddr;
    MessageType _lastMessage;

    std::tuple<
        CallbackVector<MessageType>,
        CallbackVector<ValueType>
    > _callbacks;
};

// ================================================================================
//  Inline Implementations
// ================================================================================
template<typename MsgT>
InPort<MsgT>::InPort(mw::IComAdapter* comAdapter)
    : _comAdapter{comAdapter}
{
}

template<typename MsgT>
InPort<MsgT>::InPort(mw::IComAdapter* comAdapter, ConfigType config)
    : _comAdapter{comAdapter}
    , _config{config}
{
}

template<typename MsgT>
auto InPort<MsgT>::Config() const -> const ConfigType&
{
    return _config;
}

template<typename MsgT>
auto InPort<MsgT>::Read() const -> const ValueType&
{
    return _lastMessage.value;
}

template<typename MsgT>
void InPort<MsgT>::RegisterHandler(MessageHandler handler)
{
    RegisterHandlerImpl(std::move(handler));
}

template<typename MsgT>
void InPort<MsgT>::RegisterHandler(ValueHandler handler)
{
    RegisterHandlerImpl(std::move(handler));
}

template<typename MsgT>
void InPort<MsgT>::ReceiveIbMessage(mw::EndpointAddress from, const MessageType& msg)
{
    if (from == _endpointAddr)
        return;

    _lastMessage = msg;
    CallHandlers(msg);
    CallHandlers(msg.value);
}

template<typename MsgT>
void InPort<MsgT>::SetEndpointAddress(const mw::EndpointAddress& endpointAddress)
{
    _endpointAddr = endpointAddress;
}

template<typename MsgT>
auto InPort<MsgT>::EndpointAddress() const -> const mw::EndpointAddress&
{
    return _endpointAddr;
}


template<typename MsgT>
template<typename T>
void InPort<MsgT>::RegisterHandlerImpl(CallbackT<T> handler)
{
    auto&& handlers = std::get<CallbackVector<T>>(_callbacks);
    handlers.emplace_back(std::move(handler));
}

template<typename MsgT>
template<typename T>
void InPort<MsgT>::CallHandlers(const T& t)
{
    auto&& handlers = std::get<CallbackVector<T>>(_callbacks);
    for (auto&& handler : handlers)
    {
        handler(this, t);
    }
}

} // namespace io
} // namespace sim
} // namespace ib
