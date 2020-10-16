// Copyright (c) Vector Informatik GmbH. All rights reserved.

#include "EthControllerReplay.hpp"

namespace ib {
namespace sim {
namespace eth {
void EthControllerReplay::Activate()
{
    _controller.Activate();
}

void EthControllerReplay::Deactivate()
{
    _controller.Deactivate();
}

auto EthControllerReplay::SendMessage(EthMessage msg) -> EthTxId
{
    // ignore the user's API calls if we're configured for replay
    if (tracing::IsReplayEnabledFor(_replayConfig, cfg::Replay::Direction::Send))
    {
        return 0;
    }

    return _controller.SendMessage(std::move(msg));
}

auto EthControllerReplay::SendFrame(EthFrame msg) -> EthTxId
{
    // ignore the user's API calls if we're configured for replay
    if (tracing::IsReplayEnabledFor(_replayConfig, cfg::Replay::Direction::Send))
    {
        return 0;
    }

    return _controller.SendFrame(std::move(msg));
}

auto EthControllerReplay::SendFrame(EthFrame msg, std::chrono::nanoseconds timestamp) -> EthTxId
{
    // ignore the user's API calls if we're configured for replay
    if (tracing::IsReplayEnabledFor(_replayConfig, cfg::Replay::Direction::Send))
    {
        return 0;
    }

    return _controller.SendFrame(std::move(msg), timestamp);
}

void EthControllerReplay::RegisterReceiveMessageHandler(ReceiveMessageHandler handler)
{
    _controller.RegisterReceiveMessageHandler(std::move(handler));
}

void EthControllerReplay::RegisterMessageAckHandler(MessageAckHandler handler)
{
    _controller.RegisterMessageAckHandler(std::move(handler));
}
void EthControllerReplay::RegisterStateChangedHandler(StateChangedHandler handler)
{
    _controller.RegisterStateChangedHandler(std::move(handler));
}

void EthControllerReplay::RegisterBitRateChangedHandler(BitRateChangedHandler handler)
{
    _controller.RegisterBitRateChangedHandler(std::move(handler));
}

// IIbToEthController
void EthControllerReplay::ReceiveIbMessage(ib::mw::EndpointAddress from, const EthMessage& msg)
{
    // ignore messages that do not originate from the replay scheduler 
    if (tracing::IsReplayEnabledFor(_replayConfig, cfg::Replay::Direction::Receive))
    {
        return;
    }

    _controller.ReceiveIbMessage(from, msg);
}

void EthControllerReplay::ReceiveIbMessage(ib::mw::EndpointAddress from, const EthTransmitAcknowledge& msg)
{
    // ignore messages that do not originate from the replay scheduler 
    if (tracing::IsReplayEnabledFor(_replayConfig, cfg::Replay::Direction::Receive))
    {
        return;
    }

    _controller.ReceiveIbMessage(from, msg);
}

void EthControllerReplay::SetEndpointAddress(const ib::mw::EndpointAddress& endpointAddress)
{
    _controller.SetEndpointAddress(endpointAddress);
}

auto EthControllerReplay::EndpointAddress() const -> const ib::mw::EndpointAddress&
{
    return _controller.EndpointAddress();
}

// ib::mw::sync::ITimeConsumer
void EthControllerReplay::SetTimeProvider(ib::mw::sync::ITimeProvider* timeProvider)
{
    _controller.SetTimeProvider(timeProvider);
}

// ITraceMessageSource
void EthControllerReplay::AddSink(extensions::ITraceMessageSink* sink)
{
    _controller.AddSink(sink);
}

// IReplayDataProvider

void EthControllerReplay::ReplayMessage(const extensions::IReplayMessage* replayMessage)
{
    using namespace ib::tracing;
    switch (replayMessage->GetDirection())
    {
    case extensions::Direction::Send:
        if (IsReplayEnabledFor(_replayConfig, cfg::Replay::Direction::Send))
        {
            ReplaySend(replayMessage);
        }
        break;
    case extensions::Direction::Receive:
        if (IsReplayEnabledFor(_replayConfig, cfg::Replay::Direction::Receive))
        {
            ReplayReceive(replayMessage);
        }
        break;
    default:
        throw std::runtime_error("EthReplayController: replay message has undefined Direction");
        break;
    }

}


void EthControllerReplay::ReplaySend(const extensions::IReplayMessage* replayMessage)
{
    // need to copy the message here.
    // will throw if invalid message type.
    sim::eth::EthFrame msg = dynamic_cast<const sim::eth::EthFrame&>(*replayMessage);
    _controller.SendFrame(std::move(msg));
}

void EthControllerReplay::ReplayReceive(const extensions::IReplayMessage* replayMessage)
{
    sim::eth::EthFrame frame = dynamic_cast<const sim::eth::EthFrame&>(*replayMessage);
    sim::eth::EthMessage msg{};
    msg.ethFrame = std::move(frame);
    msg.timestamp = replayMessage->Timestamp();
    // XXX there is no guarantee that the EndpointAddress from the trace is valid
    // it might even be the same as ours --> will be dropped.
    _controller.ReceiveIbMessage(replayMessage->EndpointAddress(), msg); //XXX will trigger an ACK here
}

} // namespace eth
} // namespace sim
} // namespace ib