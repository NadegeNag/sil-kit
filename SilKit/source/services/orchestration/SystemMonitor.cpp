/* Copyright (c) 2022 Vector Informatik GmbH

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */

#include <algorithm>
#include <ctime>
#include <iomanip> //std:put_time

#include "silkit/services/orchestration/string_utils.hpp"

#include "SystemMonitor.hpp"
#include "IServiceDiscovery.hpp"
#include "ILogger.hpp"
#include "LifecycleService.hpp"

namespace SilKit {
namespace Services {
namespace Orchestration {

SystemMonitor::SystemMonitor(Core::IParticipantInternal* participant)
    : _logger{participant->GetLogger()}
    , _participant{participant}
{
}

void SystemMonitor::ReceiveMsg(const IServiceEndpoint* /*from*/,
                                     const Orchestration::WorkflowConfiguration& workflowConfiguration)
{
    UpdateRequiredParticipantNames(workflowConfiguration.requiredParticipantNames);
    dynamic_cast<LifecycleService*>(_participant->GetLifecycleService())
        ->SetWorkflowConfiguration(workflowConfiguration);
    
}

void SystemMonitor::UpdateRequiredParticipantNames(const std::vector<std::string>& requiredParticipantNames)
{

    _requiredParticipantNames = requiredParticipantNames;

    bool allRequiredParticipantsKnown = true;
    // Init new participants in status map
    for (auto&& name : _requiredParticipantNames)
    {
        std::unique_lock<decltype(_participantStatusMx)> lock{_participantStatusMx};

        auto&& statusIter = _participantStatus.find(name);
        if (statusIter == _participantStatus.end())
        {
            allRequiredParticipantsKnown = false;
        }
    }

    // Update / propagate the system state in case status updated for all required participants have been received already
    if (allRequiredParticipantsKnown)
    {
        auto oldSystemState = _systemState;
        for (auto&& name : _requiredParticipantNames)
        {
            Orchestration::ParticipantStatus status{};
            {
                std::unique_lock<decltype(_participantStatusMx)> lock{_participantStatusMx};
                status = _participantStatus.at(name);
            }
            UpdateSystemState(status);
        }
        if (oldSystemState != _systemState)
        {
            _systemStateHandlers.InvokeAll(_systemState);
        }
    }
}
auto SystemMonitor::AddSystemStateHandler(SystemStateHandler handler) -> HandlerId
{
    if (_systemState != Orchestration::SystemState::Invalid)
    {
        handler(_systemState);
    }

    return _systemStateHandlers.Add(std::move(handler));
}

void SystemMonitor::RemoveSystemStateHandler(HandlerId handlerId)
{
    if (!_systemStateHandlers.Remove(handlerId))
    {
        _logger->Warn("RemoveSystemStateHandler failed: Unknown HandlerId.");
    }

}

auto SystemMonitor::AddParticipantStatusHandler(ParticipantStatusHandler handler) -> HandlerId
{
    {
        std::unique_lock<decltype(_participantStatusMx)> lock{_participantStatusMx};

        for (auto&& kv : _participantStatus)
        {
            auto&& participantStatus = kv.second;
            if (participantStatus.state == Orchestration::ParticipantState::Invalid)
                continue;

            handler(participantStatus);
        }
    }

    return _participantStatusHandlers.Add(std::move(handler));
}

void SystemMonitor::RemoveParticipantStatusHandler(HandlerId handlerId)
{
    if (!_participantStatusHandlers.Remove(handlerId))
    {
        _logger->Warn("RemoveParticipantStatusHandler failed: Unknown HandlerId.");
    }
}

auto SystemMonitor::SystemState() const -> Orchestration::SystemState
{
    return _systemState;
}

auto SystemMonitor::ParticipantStatus(const std::string& participantName) const -> const Orchestration::ParticipantStatus&
{
    std::unique_lock<decltype(_participantStatusMx)> lock{_participantStatusMx};

    auto&& statusIter = _participantStatus.find(participantName);
    if (statusIter == _participantStatus.end())
    {
        throw SilKitError{"Unknown participantName"};
    }

    return statusIter->second;
}


void SystemMonitor::ReceiveMsg(const IServiceEndpoint* /*from*/, const Orchestration::ParticipantStatus& newParticipantStatus)
{
    auto participantName = newParticipantStatus.participantName;

    // Explicitly initialize unknown participants
    auto&& statusIter = _participantStatus.find(participantName);
    if (statusIter == _participantStatus.end())
    {
        std::unique_lock<decltype(_participantStatusMx)> lock{_participantStatusMx};
        auto initialStatus = Orchestration::ParticipantStatus{};
        initialStatus.participantName = participantName;
        initialStatus.state = Orchestration::ParticipantState::Invalid;
        _participantStatus.emplace(participantName, initialStatus);
    }

    // Save former state
    ParticipantState oldParticipantState;
    {
        std::unique_lock<decltype(_participantStatusMx)> lock{_participantStatusMx};
        oldParticipantState = _participantStatus.at(participantName).state;
    }

    // Check if transition is valid
    ValidateParticipantStatusUpdate(newParticipantStatus, oldParticipantState);

    // Ignores transition if ParticipantState is Shutdown already
    if (oldParticipantState == Orchestration::ParticipantState::Shutdown)
    {
        Logging::Debug(_logger,
                        "Ignoring ParticipantState update from participant \'{}\' to ParticipantState::{} because "
                        "participant is already in terminal state ParticipantState::Shutdown.",
                        newParticipantStatus.participantName, newParticipantStatus.state);
        return;
    }


    // Update status map
    {
        std::unique_lock<decltype(_participantStatusMx)> lock{_participantStatusMx};
        _participantStatus.at(participantName) = newParticipantStatus;
    }

    // On new participant state
    if (oldParticipantState != newParticipantStatus.state)
    {
        // Fire status / state handlers
        _participantStatusHandlers.InvokeAll(newParticipantStatus);

        auto oldSystemState = _systemState;
        // Update the system state for required participants, others are ignored
        UpdateSystemState(newParticipantStatus);
        if (oldSystemState != _systemState)
        {
            _systemStateHandlers.InvokeAll(_systemState);
        }
    }
}

void SystemMonitor::SetParticipantConnectedHandler(ParticipantConnectedHandler handler)
{
    _participantConnectedHandler = std::move(handler);
}

void SystemMonitor::SetParticipantDisconnectedHandler(ParticipantDisconnectedHandler handler)
{
    _participantDisconnectedHandler = std::move(handler);
}

auto SystemMonitor::IsParticipantConnected(const std::string& participantName) const -> bool
{
    std::unique_lock<decltype(_connectedParticipantsMx)> lock{_connectedParticipantsMx};

    const auto it = _connectedParticipants.find(participantName);
    return it != _connectedParticipants.end();
}

void SystemMonitor::OnParticipantConnected(const ParticipantConnectionInformation& participantConnectionInformation)
{
    {
        std::unique_lock<decltype(_connectedParticipantsMx)> lock{_connectedParticipantsMx};

        // Add the participant name to the map of connected participant names/connections
        _connectedParticipants.emplace(participantConnectionInformation.participantName, participantConnectionInformation);
    }

    // Call the handler if set
    if (_participantConnectedHandler)
    {
        _participantConnectedHandler(participantConnectionInformation);
    }
}

void SystemMonitor::OnParticipantDisconnected(const ParticipantConnectionInformation& participantConnectionInformation)
{
    // Erase participant from connectedParticipant map
    {
        std::unique_lock<decltype(_connectedParticipantsMx)> lock{_connectedParticipantsMx};
        auto it = _connectedParticipants.find(participantConnectionInformation.participantName);
        if (it != _connectedParticipants.end())
        {
            _connectedParticipants.erase(it);
        }
    }

    // Erase participant from participantStatus map
    {
        std::unique_lock<decltype(_participantStatusMx)> lock{_participantStatusMx};
        auto it = _participantStatus.find(participantConnectionInformation.participantName);
        if (it != _participantStatus.end())
        {
            _participantStatus.erase(it);
        }
    }

    // Call the handler if set
    if (_participantDisconnectedHandler)
    {
        _participantDisconnectedHandler(participantConnectionInformation);
    }
}

bool SystemMonitor::AllRequiredParticipantsInState(std::initializer_list<Orchestration::ParticipantState> acceptedStates) const
{
    for (auto&& name : _requiredParticipantNames)
    {
        // Check if required participant hasn't been removed from _participantStatus map.
        // This also blocks any SystemState updates if a required participant has disconnected and thus removed from _participantStatus.
        auto&& it = _participantStatus.find(name);
        if (it == _participantStatus.end())
        {
            return false;
        }

        auto participantState = [this, &name] {
            std::unique_lock<decltype(_participantStatusMx)> lock{_participantStatusMx};
            return _participantStatus.at(name).state;
        }();

        bool isAcceptedState = std::any_of(begin(acceptedStates), end(acceptedStates), [participantState](auto acceptedState) {
            return participantState == acceptedState;
        });
        if (!isAcceptedState)
            return false;
    }
    return true;
}

void SystemMonitor::ValidateParticipantStatusUpdate(const Orchestration::ParticipantStatus& newStatus, Orchestration::ParticipantState oldState)
{
    auto is_any_of = [](Orchestration::ParticipantState state, std::initializer_list<Orchestration::ParticipantState> stateList)
    {
        return std::any_of(begin(stateList), end(stateList), [=](auto candidate) { return candidate == state; });
    };

    switch (newStatus.state)
    {
    case Orchestration::ParticipantState::ServicesCreated:
        if (is_any_of(oldState, {Orchestration::ParticipantState::Invalid}))
            return;

    case Orchestration::ParticipantState::CommunicationInitializing:
        if (is_any_of(oldState, {Orchestration::ParticipantState::ServicesCreated}))
            return;

    case Orchestration::ParticipantState::CommunicationInitialized:
        if (oldState == Orchestration::ParticipantState::CommunicationInitializing)
            return;

    case Orchestration::ParticipantState::ReadyToRun:
        if (oldState == Orchestration::ParticipantState::CommunicationInitialized)
            return;

    case Orchestration::ParticipantState::Running:
        if (is_any_of(oldState, {Orchestration::ParticipantState::ReadyToRun, Orchestration::ParticipantState::Paused}))
            return;

    case Orchestration::ParticipantState::Paused:
        if (oldState == Orchestration::ParticipantState::Running)
            return;

    case Orchestration::ParticipantState::Stopping:
        if (is_any_of(oldState, {Orchestration::ParticipantState::Running, Orchestration::ParticipantState::Paused}))
            return;

    case Orchestration::ParticipantState::Stopped:
        if (oldState == Orchestration::ParticipantState::Stopping)
            return;

    case Orchestration::ParticipantState::ShuttingDown:
        if (is_any_of(oldState, {Orchestration::ParticipantState::Error, Orchestration::ParticipantState::Stopped}))
            return;

    case Orchestration::ParticipantState::Shutdown:
        if (oldState == Orchestration::ParticipantState::ShuttingDown)
            return;

    case Orchestration::ParticipantState::Aborting: 
        return;

    case Orchestration::ParticipantState::Error:
        return;

    default:
        Logging::Error(_logger, "SystemMonitor::ValidateParticipantStatusUpdate() Unhandled ParticipantState::{}", newStatus.state);
    }

    std::time_t enterTime = std::chrono::system_clock::to_time_t(newStatus.enterTime);
    std::tm tmBuffer;
#if defined(_WIN32)
    localtime_s(&tmBuffer, &enterTime);
#else
    localtime_r(&enterTime, &tmBuffer);
#endif
    std::stringstream timeBuf;
    timeBuf <<  std::put_time(&tmBuffer, "%FT%T");

    Logging::Error(_logger,
        "SystemMonitor detected invalid ParticipantState transition from {} to {} EnterTime={}, EnterReason=\"{}\"",
        oldState,
        newStatus.state,
        timeBuf.str(),
        newStatus.enterReason);

    _invalidTransitionCount++;
}

void SystemMonitor::UpdateSystemState(const Orchestration::ParticipantStatus& newStatus)
{
    auto&& nameIter = std::find(_requiredParticipantNames.begin(), _requiredParticipantNames.end(), newStatus.participantName);
    if (nameIter == _requiredParticipantNames.end())
    {
        return;
    }

    switch (newStatus.state)
    {
    case Orchestration::ParticipantState::Invalid: 
        return; // ignore invalid participants until they do something
    case Orchestration::ParticipantState::ServicesCreated:
        if (AllRequiredParticipantsInState({Orchestration::ParticipantState::ServicesCreated,
                                            Orchestration::ParticipantState::CommunicationInitializing,
                                            Orchestration::ParticipantState::CommunicationInitialized,
                                            Orchestration::ParticipantState::ReadyToRun, 
                                            Orchestration::ParticipantState::Running}))
        {
            SetSystemState(Orchestration::SystemState::ServicesCreated);
        }
        return;

    case Orchestration::ParticipantState::CommunicationInitializing:
        if (AllRequiredParticipantsInState({Orchestration::ParticipantState::CommunicationInitializing,
                                            Orchestration::ParticipantState::CommunicationInitialized,
                                            Orchestration::ParticipantState::ReadyToRun, 
                                            Orchestration::ParticipantState::Running}))
        {
            SetSystemState(Orchestration::SystemState::CommunicationInitializing);
        }
        return;

    case Orchestration::ParticipantState::CommunicationInitialized:
        if (AllRequiredParticipantsInState({Orchestration::ParticipantState::CommunicationInitialized,
                                            Orchestration::ParticipantState::ReadyToRun, 
                                            Orchestration::ParticipantState::Running}))
        {
            SetSystemState(Orchestration::SystemState::CommunicationInitialized);
        }
        return;

    case Orchestration::ParticipantState::ReadyToRun:
        if (AllRequiredParticipantsInState({Orchestration::ParticipantState::ReadyToRun, 
                                            Orchestration::ParticipantState::Running}))
        {
            SetSystemState(Orchestration::SystemState::ReadyToRun);
        }
        return;

    case Orchestration::ParticipantState::Running:
        if (AllRequiredParticipantsInState({Orchestration::ParticipantState::Running}))
        {
            SetSystemState(Orchestration::SystemState::Running);
        }
        else if (AllRequiredParticipantsInState({
                     Orchestration::ParticipantState::Running,
                     Orchestration::ParticipantState::Stopping
                 }))
        {
            SetSystemState(Orchestration::SystemState::Stopping);
        }
        else if (AllRequiredParticipantsInState({
                     Orchestration::ParticipantState::Running,
                     Orchestration::ParticipantState::Stopping,
                     Orchestration::ParticipantState::Stopped
                 }))
        {
            SetSystemState(Orchestration::SystemState::Stopped);
        }
        else if (AllRequiredParticipantsInState({
                     Orchestration::ParticipantState::Running,
                     Orchestration::ParticipantState::Stopping,
                     Orchestration::ParticipantState::Stopped,
                     Orchestration::ParticipantState::ShuttingDown
                 }))
        {
            SetSystemState(Orchestration::SystemState::ShuttingDown);
        }
        else if (AllRequiredParticipantsInState({
                     Orchestration::ParticipantState::Running,
                     Orchestration::ParticipantState::Stopping,
                     Orchestration::ParticipantState::Stopped,
                     Orchestration::ParticipantState::ShuttingDown,
                     Orchestration::ParticipantState::Shutdown
                 }))
        {
            SetSystemState(Orchestration::SystemState::Shutdown);
        }
        return;

    case Orchestration::ParticipantState::Paused:
        if (AllRequiredParticipantsInState({
                     Orchestration::ParticipantState::Paused, 
                     Orchestration::ParticipantState::Running
            }))
        {
            SetSystemState(Orchestration::SystemState::Paused);
        }
        else if (AllRequiredParticipantsInState({
                     Orchestration::ParticipantState::Paused, 
                     Orchestration::ParticipantState::Running,
                     Orchestration::ParticipantState::Stopping
                 }))
        {
            SetSystemState(Orchestration::SystemState::Stopping);
        }
        else if (AllRequiredParticipantsInState({
                     Orchestration::ParticipantState::Paused, 
                     Orchestration::ParticipantState::Running,
                     Orchestration::ParticipantState::Stopping,
                     Orchestration::ParticipantState::Stopped
                 }))
        {
            SetSystemState(Orchestration::SystemState::Stopped);
        }
        else if (AllRequiredParticipantsInState({
                     Orchestration::ParticipantState::Paused, 
                     Orchestration::ParticipantState::Running,
                     Orchestration::ParticipantState::Stopping,
                     Orchestration::ParticipantState::Stopped,
                     Orchestration::ParticipantState::ShuttingDown
                 }))
        {
            SetSystemState(Orchestration::SystemState::ShuttingDown);
        }
        else if (AllRequiredParticipantsInState({
                     Orchestration::ParticipantState::Paused, 
                     Orchestration::ParticipantState::Running,
                     Orchestration::ParticipantState::Stopping,
                     Orchestration::ParticipantState::Stopped,
                     Orchestration::ParticipantState::ShuttingDown,
                     Orchestration::ParticipantState::Shutdown
                 }))
        {
            SetSystemState(Orchestration::SystemState::Shutdown);
        }
        return;

    case Orchestration::ParticipantState::Stopping:
        if (AllRequiredParticipantsInState({Orchestration::ParticipantState::Stopping, 
                                            Orchestration::ParticipantState::Stopped,
                                            Orchestration::ParticipantState::ShuttingDown,
                                            Orchestration::ParticipantState::Shutdown,
                                            Orchestration::ParticipantState::Paused, 
                                            Orchestration::ParticipantState::Running}))
            SetSystemState(Orchestration::SystemState::Stopping);
        return;

    case Orchestration::ParticipantState::Stopped:
        if (AllRequiredParticipantsInState({Orchestration::ParticipantState::Stopped,
                                            Orchestration::ParticipantState::ShuttingDown,
                                            Orchestration::ParticipantState::Shutdown}))
            SetSystemState(Orchestration::SystemState::Stopped);
        return;

    case Orchestration::ParticipantState::ShuttingDown:
        if (AllRequiredParticipantsInState({Orchestration::ParticipantState::ShuttingDown, 
                                            Orchestration::ParticipantState::Shutdown,
                                            Orchestration::ParticipantState::Stopped, 
                                            Orchestration::ParticipantState::Error,
                                            Orchestration::ParticipantState::ServicesCreated, 
                                            Orchestration::ParticipantState::ReadyToRun}))
            SetSystemState(Orchestration::SystemState::ShuttingDown);
        return;

    case Orchestration::ParticipantState::Shutdown:
        if (AllRequiredParticipantsInState({Orchestration::ParticipantState::Shutdown}))
            SetSystemState(Orchestration::SystemState::Shutdown);
        return;
    case Orchestration::ParticipantState::Aborting:
        SetSystemState(Orchestration::SystemState::Aborting);
        return;
    case Orchestration::ParticipantState::Error:
        SetSystemState(Orchestration::SystemState::Error);
        return;
    }
}

void SystemMonitor::SetSystemState(Orchestration::SystemState newState)
{
    _systemState = newState;
}

} // namespace Orchestration
} // namespace Services
} // namespace SilKit
