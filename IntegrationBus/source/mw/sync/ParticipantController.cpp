#include "ParticipantController.hpp"

#include <cassert>
#include <future>

#include "ib/cfg/string_utils.hpp"
#include "ib/mw/logging/spdlog.hpp"
#include "ib/mw/sync/string_utils.hpp"

using namespace std::chrono_literals;

namespace ib {
namespace mw {
namespace sync {

TaskRunner::TaskRunner(ParticipantController& controller)
    : _controller(controller)
{}

void TaskRunner::Run()
{
    RequestStep(_controller);
}

void TaskRunner::GrantReceived()
{
    _controller.ExecuteSimTask();
    FinishedStep(_controller);
    RequestStep(_controller);
}

class TimeQuantumTaskRunner
    : public TaskRunner
{
public:
    TimeQuantumTaskRunner(ParticipantController& controller)
        : TaskRunner::TaskRunner(controller)
    {}

    void RequestStep(ParticipantController& controller) override
    {
        controller.SendQuantumRequest();
    }
    void FinishedStep(ParticipantController& controller) override
    {
        controller.AdvanceQuantum();
    }
};

class DiscreteTimeTaskRunner
    : public TaskRunner
{
public:
    DiscreteTimeTaskRunner(ParticipantController& controller)
        : TaskRunner::TaskRunner(controller)
    {}

    void RequestStep(ParticipantController& /*controller*/) override {}
    void FinishedStep(ParticipantController& controller) override
    {
        controller.SendTickDone();
    }
};

class DiscreteTimePassiveTaskRunner
    : public TaskRunner
{
public:
    DiscreteTimePassiveTaskRunner(ParticipantController& controller)
        : TaskRunner::TaskRunner(controller)
    {}

    void RequestStep(ParticipantController& /*controller*/) override {}
    void FinishedStep(ParticipantController& /*controller*/) override {}
};


ParticipantController::ParticipantController(IComAdapter* comAdapter, cfg::Participant participantConfig, cfg::TimeSync timesyncConfig)
    : _comAdapter{comAdapter}
    , _participantConfig(std::move(participantConfig))
    , _timesyncConfig(std::move(timesyncConfig))
    , _logger{comAdapter->GetLogger()}
{
    _status.participantName = _participantConfig.name;
}

void ParticipantController::SetInitHandler(InitHandlerT handler)
{
    _initHandler = std::move(handler);
}

void ParticipantController::SetStopHandler(StopHandlerT handler)
{
    _stopHandler = std::move(handler);
}

void ParticipantController::SetShutdownHandler(ShutdownHandlerT handler)
{
    _shutdownHandler = std::move(handler);
}

void ParticipantController::SetSimulationTask(std::function<void(std::chrono::nanoseconds now, std::chrono::nanoseconds duration)> task)
{
    _simTask = std::move(task);
}

void ParticipantController::SetSimulationTask(std::function<void(std::chrono::nanoseconds now)> task)
{
    _simTask = [task = std::move(task)](auto now, auto /*duration*/){ task(now); };
}

void ParticipantController::EnableColdswap()
{
    _coldswapEnabled = true;
}

void ParticipantController::SetPeriod(std::chrono::nanoseconds period)
{
    if (_participantConfig.syncType != cfg::SyncType::TimeQuantum)
    {
        auto msPeriod = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(period);
        _logger->warn("ParticipantController::SetPeriod({}ms) is ignored", msPeriod.count());
        _logger->info("ParticipantController::SetPeriod() can only be used with SyncType::TimeQuantum (currently active: SyncType::{})", _participantConfig.syncType);
    }
    _period = period;
}

void ParticipantController::SetEarliestEventTime(std::chrono::nanoseconds /*eventTime*/)
{
    throw std::exception();
}

void ParticipantController::StartTaskRunner()
{
    if (!_simTask)
    {
        _logger->error("ParticipantController::Run() was called without having set a SimTask!");
        ReportError("ParticipantController::Run() was called without having set a SimTask!");
        throw std::exception();
    }

    switch (_participantConfig.syncType)
    {
    case cfg::SyncType::DiscreteEvent:
        _logger->critical("Unsupported SyncType {}", to_string(_participantConfig.syncType));
        throw std::runtime_error("Unsupported SyncType " + to_string(_participantConfig.syncType));
    case cfg::SyncType::TimeQuantum:
        _taskRunner = std::make_unique<TimeQuantumTaskRunner>(*this);
        break;
    case cfg::SyncType::DiscreteTime:
        _taskRunner = std::make_unique<DiscreteTimeTaskRunner>(*this);
        break;
    case cfg::SyncType::DiscreteTimePassive:
        _taskRunner = std::make_unique<DiscreteTimePassiveTaskRunner>(*this);
        break;
    default:
        _logger->critical("Invalid SyncType {}", to_string(_participantConfig.syncType));
        throw ib::cfg::Misconfiguration("Invalid SyncType " + to_string(_participantConfig.syncType));
    }

    ChangeState(ParticipantState::Idle, "ParticipantController::Run() was called");
}

auto ParticipantController::Run() -> ParticipantState
{
    return RunAsync().get();
}

auto ParticipantController::RunAsync() -> std::future<ParticipantState>
{
    StartTaskRunner();
    return _finalStatePromise.get_future();
}

void ParticipantController::ReportError(std::string errorMsg)
{
    ChangeState(ParticipantState::Error, std::move(errorMsg));
}

void ParticipantController::Pause(std::string reason)
{
    if (State() != ParticipantState::Running)
    {
        std::string errorMessage{"ParticipantController::Pause() was called in state ParticipantState::" + to_string(State())};
        _logger->error(errorMessage);
        ReportError(errorMessage);
        throw std::runtime_error(errorMessage);
    }
    ChangeState(ParticipantState::Paused, std::move(reason));
}

void ParticipantController::Continue()
{
    if (State() != ParticipantState::Paused)
    {
        std::string errorMessage{"ParticipantController::Continue() was called in state ParticipantState::" + to_string(State())};
        _logger->error(errorMessage);
        ReportError(errorMessage);
        throw std::runtime_error(errorMessage);
    }
    ChangeState(ParticipantState::Running, "Pause finished");
}

void ParticipantController::Initialize(const ParticipantCommand& command, std::string reason)
{
    ChangeState(ParticipantState::Initializing, reason);
    if (_initHandler)
    {
        try
        {
            _initHandler(command);
            reason += " and InitHandler completed without exception";
        }
        catch (const std::exception& e)
        {
            std::string errorMessage{"InitHandler did throw an exception: " + std::string{e.what()}};
            _logger->error(errorMessage);
            ReportError(errorMessage);
            return;
        }
    }
    else
    {
        reason += " and no InitHandler was registered";
    }

    _now = 0ns;
    ChangeState(ParticipantState::Initialized, std::move(reason));
}

void ParticipantController::Stop(std::string reason)
{
    ChangeState(ParticipantState::Stopping, reason);

    if (_stopHandler)
    {
        try
        {
            _stopHandler();
            // The handler can report an error, which overrules the default transition to ParticipantState::Stopped
            if (State() != ParticipantState::Error)
            {
                reason += " and StopHandler completed successfully";
                ChangeState(ParticipantState::Stopped, std::move(reason));
            }
        }
        catch (const std::exception& e)
        {
            reason += " and StopHandler threw exception: ";
            reason += e.what();
            ChangeState(ParticipantState::Stopped, std::move(reason));
        }
    }
    else
    {
        reason += " and no StopHandler registered";
        ChangeState(ParticipantState::Stopped, reason);
    }

}

void ParticipantController::Shutdown(std::string reason)
{
    ChangeState(ParticipantState::ShuttingDown, reason);

    if (_shutdownHandler)
    {
        try
        {
            _shutdownHandler();
            reason += " and ShutdownHandler completed";
            ChangeState(ParticipantState::Shutdown, std::move(reason));
        }
        catch (const std::exception& e)
        {
            reason += " and ShutdownHandler threw exception: ";
            reason += e.what();
            ChangeState(ParticipantState::Shutdown, std::move(reason));
        }
    }
    else
    {
        reason += " and no ShutdownHandler was registered";
        ChangeState(ParticipantState::Shutdown, std::move(reason));
    }

    _finalStatePromise.set_value(State());
}

void ParticipantController::PrepareColdswap()
{
    _logger->info("preparing coldswap...");
    ChangeState(ParticipantState::ColdswapPrepare, "Starting coldswap preparations");

    _comAdapter->OnAllMessagesDelivered([this]() {

        _comAdapter->FlushSendBuffers();
        ChangeState(ParticipantState::ColdswapReady, "Finished coldswap preparations.");
        _logger->info("ready for coldswap...");

    });
}

void ParticipantController::ShutdownForColdswap()
{
    _comAdapter->FlushSendBuffers();
    ChangeState(ParticipantState::ColdswapShutdown, "Coldswap was enabled for this participant.");

    _comAdapter->OnAllMessagesDelivered([this]() {

        _finalStatePromise.set_value(State());

    });
}

void ParticipantController::IgnoreColdswap()
{
    _comAdapter->FlushSendBuffers();
    ChangeState(ParticipantState::ColdswapIgnored, "Coldswap was not enabled for this participant.");
}

auto ParticipantController::State() const -> ParticipantState
{
    return _status.state;
}

auto ParticipantController::Status() const -> const ParticipantStatus&
{
    return _status;
}

void ParticipantController::RefreshStatus()
{
    _status.refreshTime = std::chrono::system_clock::now();
    SendIbMessage(_status);
}

auto ParticipantController::Now() const -> std::chrono::nanoseconds
{
    return _now;
}

void ParticipantController::LogCurrentPerformanceStats()
{
    using DoubleMSecs = std::chrono::duration<double, std::milli>;
    auto toMSecs = [](auto duration) { return std::chrono::duration_cast<DoubleMSecs>(duration).count(); };

    if (_execTimeMonitor.SampleCount() == 0u)
    {
        _logger->info("TotalTaskTime: -.--ms [-.--, -.--] \tWaitTime: -.--ms [-.--, -.--]  \tCpuTime: -.--ms, [-.--, -.--] \t(avg [min,max])");
    }
    else
    {
        _logger->info("TotalTaskTime: {:.2f}ms [{:.2f}, {:.2f}] \tWaitTime: {:.2f}ms [{:.2f}, {:.2f}]  \tCpuTime: {:.2f}, [{:.2f}, {:.2f}] \t(avg [min,max])",
            toMSecs(_execTimeMonitor.AvgDuration<DoubleMSecs>() + _waitTimeMonitor.AvgDuration<DoubleMSecs>()),
            toMSecs(_execTimeMonitor.MinDuration() + _waitTimeMonitor.MinDuration()),
            toMSecs(_execTimeMonitor.MaxDuration() + _waitTimeMonitor.MaxDuration()),

            toMSecs(_waitTimeMonitor.AvgDuration<DoubleMSecs>()),
            toMSecs(_waitTimeMonitor.MinDuration()),
            toMSecs(_waitTimeMonitor.MaxDuration()),

            toMSecs(_execTimeMonitor.AvgDuration<DoubleMSecs>()),
            toMSecs(_execTimeMonitor.MinDuration()),
            toMSecs(_execTimeMonitor.MaxDuration())
        );
    }
}

void ParticipantController::SetEndpointAddress(const mw::EndpointAddress& addr)
{
    _endpointAddress = addr;
}

auto ParticipantController::EndpointAddress() const -> const mw::EndpointAddress&
{
    return _endpointAddress;
}     

void ParticipantController::ReceiveIbMessage(ib::mw::EndpointAddress /*from*/, const ParticipantCommand& command)
{
    if (command.participant != _endpointAddress.participant)
        return;

    Initialize(command, std::string{"Received ParticipantCommand::"} + to_string(command.kind));
}

void ParticipantController::ReceiveIbMessage(ib::mw::EndpointAddress /*from*/, const SystemCommand& command)
{
    // We have to supress a SystemCommand::ExecuteColdswap during the restart
    // After a coldswap, this command is still present in the SystemControllers
    // history and thus retransmitted to the reconnecting participant. However,
    // we cannot flush this change from the SystemController's history because
    // this could happen before the command has been received by the participant,
    // thus missing the command.
    if (command.kind == SystemCommand::Kind::ExecuteColdswap && (State() == ParticipantState::Invalid || State() == ParticipantState::Idle))
    {
        return;
    }

    if (!_taskRunner)
    {
        ReportError("Received SystemCommand::" + to_string(command.kind) + " before ParticipantController::Run() or RunAsync() was called");
        return;
    }

    switch (command.kind)
    {
    case SystemCommand::Kind::Invalid:
        break;

    case SystemCommand::Kind::Run:
        if (State() == ParticipantState::Initialized)
        {
            ChangeState(ParticipantState::Running, "Received SystemCommand::Run");
            _waitTimeMonitor.StartMeasurement();
            _taskRunner->Run();
            return;
        }
        break;

    case SystemCommand::Kind::Stop:
        if (State() == ParticipantState::Stopped)
        {
            _logger->warn("Received SystemCommand::Stop, but ignored since already ParticipantState::Stopped");
            return;
        }
        else if (State() == ParticipantState::Running)
        {
            Stop("Received SystemCommand::Stop");
            return;
        }
        break;

    case SystemCommand::Kind::Shutdown:
        if (State() == ParticipantState::Error || State() == ParticipantState::Stopped)
        {
            Shutdown("Received SystemCommand::Shutdown");
            return;
        }
        break;

    case SystemCommand::Kind::PrepareColdswap:
        if (State() == ParticipantState::Error || State() == ParticipantState::Stopped)
        {
            PrepareColdswap();
            return;
        }
        break;

    case SystemCommand::Kind::ExecuteColdswap:
        if (State() == ParticipantState::ColdswapReady)
        {
            if (_coldswapEnabled)
            {
                ShutdownForColdswap();
            }
            else
            {
                IgnoreColdswap();
            }
            return;
        }
        break;
    }

    // We should not reach this point in normal operation.
    ReportError("Received SystemCommand::" + to_string(command.kind) + " while in ParticipantState::" + to_string(State()));
}

void ParticipantController::ReceiveIbMessage(mw::EndpointAddress /*from*/, const Tick& msg)
{
    switch (_participantConfig.syncType)
    {
    case cfg::SyncType::DiscreteTime:
    case cfg::SyncType::DiscreteTimePassive:
        break;
    default:
        return;
    }

    if (!_taskRunner)
    {
        ReportError("Received TICK before ParticipantController::Run() or RunAsync() was called");
        return;
    }

    switch (State())
    {
    case ParticipantState::Invalid:
        // [[fallthrough]]
    case ParticipantState::Idle:
        // [[fallthrough]]
    case ParticipantState::Initializing:
        // [[fallthrough]]
    case ParticipantState::Initialized:
        ReportError("Received TICK in state ParticipantState::" + to_string(State()));
        return;

    case ParticipantState::Paused:
        // We have to process TICKS received in Paused. This can happen due to
        // race conditions, and we can't undo a TICK. It should not occur more
        // than once though.
        // [[fallthrough]]
    case ParticipantState::Running:
        _now = msg.now;
        _duration = msg.duration;
        _taskRunner->GrantReceived();
        break;
        
    case ParticipantState::Stopping:
        return; // ignore TICK during stop/shutdown procedure
    case ParticipantState::Stopped:
        return; // ignore TICK during stop/shutdown procedure
    case ParticipantState::Error:
        return; // ignore TICK during stop/shutdown procedure
    case ParticipantState::ShuttingDown:
        return; // ignore TICK during stop/shutdown procedure
    case ParticipantState::Shutdown:
        return; // ignore TICK during stop/shutdown procedure
    default:
        ReportError("Received TICK in state ParticipantState::" + to_string(State()));
        return;
    }
}

void ParticipantController::ReceiveIbMessage(mw::EndpointAddress /*from*/, const QuantumGrant& msg)
{
    if (_participantConfig.syncType != cfg::SyncType::TimeQuantum)
        return;

    if (_endpointAddress != msg.grantee)
        return;

    if (!_taskRunner)
    {
        ReportError("Received QuantumGrant before ParticipantController::Run() or RunAsync() was called");
        return;
    }

    switch (State())
    {
    case ParticipantState::Invalid:
        // [[fallthrough]]
    case ParticipantState::Idle:
        // [[fallthrough]]
    case ParticipantState::Initializing:
        // [[fallthrough]]
    case ParticipantState::Initialized:
        ReportError("Received QuantumGrant in state ParticipantState::" + to_string(State()));
        return;

    case ParticipantState::Paused:
        // We have to process QuantumGrants received in Paused. This can happen
        // due to race conditions, and we can't undo a TICK. It should not occur
        // more than once though.
        // [[fallthrough]]
    case ParticipantState::Running:
        ProcessQuantumGrant(msg);
        return;

    case ParticipantState::Stopping:
        return; // ignore QuantumGrants during stop/shutdown procedure
    case ParticipantState::Stopped:
        return; // ignore QuantumGrants during stop/shutdown procedure
    case ParticipantState::Error:
        return; // ignore QuantumGrants during stop/shutdown procedure
    case ParticipantState::ShuttingDown:
        return; // ignore QuantumGrants during stop/shutdown procedure
    case ParticipantState::Shutdown:
        return; // ignore QuantumGrants during stop/shutdown procedure

    default:
        ReportError("Received QuantumGrant in state ParticipantState::" + to_string(State()));
        return;
    }
}

void ParticipantController::SendTickDone() const
{
    if (_timesyncConfig.syncPolicy == cfg::TimeSync::SyncPolicy::Strict)
    {
        _comAdapter->OnAllMessagesDelivered([this]() {

            SendIbMessage(TickDone{ Tick{_now, _duration} });

        });
    }
    else
    {
        SendIbMessage(TickDone{ Tick{ _now, _duration } });
    }
}

void ParticipantController::SendQuantumRequest() const
{
    if (_timesyncConfig.syncPolicy == cfg::TimeSync::SyncPolicy::Strict)
    {
        _comAdapter->OnAllMessagesDelivered([this]() {

            SendIbMessage(QuantumRequest{ _now, _period });

        });
    }
    else
    {
        SendIbMessage(QuantumRequest{ _now, _period });
    }
}

void ParticipantController::ProcessQuantumGrant(const QuantumGrant& msg)
{
    switch (msg.status)
    {
    case QuantumRequestStatus::Granted:
        if (msg.duration != _period)
        {
            ReportError("Granted quantum duration does not match request!");
        }
        else
        {
            _now = msg.now;
            _duration = msg.duration;
            _taskRunner->GrantReceived();
        }
        break;
    case QuantumRequestStatus::Rejected:
        _now = msg.now;
        _duration = 0ns;
        break;
    case QuantumRequestStatus::Invalid:
        ReportError("Received invalid QuantumGrant");
        break;
    default:
        ReportError("Received QuantumGrant with unknown Status");
    }
}

void ParticipantController::AdvanceQuantum()
{
    _now += _period;
}


void ParticipantController::ExecuteSimTask()
{
    assert(_simTask);
    _waitTimeMonitor.StopMeasurement();
    _execTimeMonitor.StartMeasurement();
    _simTask(_now, _duration);
    _execTimeMonitor.StopMeasurement();
    _waitTimeMonitor.StartMeasurement();
}

void ParticipantController::ChangeState(ParticipantState newState, std::string reason)
{
    _status.state = newState;
    _status.enterReason = reason;
    _status.enterTime = std::chrono::system_clock::now();
    _status.refreshTime = _status.enterTime;

    SendIbMessage(_status);
}
    
    
} // namespace sync
} // namespace mw
} // namespace ib
