// Copyright (c) Vector Informatik GmbH. All rights reserved.

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

#include "GetTestPid.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"


#include "silkit/SilKit.hpp"
#include "silkit/services/orchestration/all.hpp"
#include "silkit/vendor/CreateSilKitRegistry.hpp"
#include "silkit/services/all.hpp"

#include "ConfigurationTestUtils.hpp"

namespace {

using namespace std::chrono_literals;
using namespace SilKit;
using namespace SilKit::Services::Orchestration;
using namespace SilKit::Config;
using namespace SilKit::Services::PubSub;

const std::string systemMasterName{"SystemMaster"};
const std::string topic{"Topic"};
const std::string mediaType{ "A" };
static size_t numParticipants;
std::chrono::milliseconds communicationTimeout{2000ms};
std::chrono::milliseconds asyncDelayBetweenPublication{50ms};

class HopOnHopOffITest : public testing::Test
{

protected:
    HopOnHopOffITest()
    {
    }

    struct TestParticipant
    {
        TestParticipant(const std::string& newName)
        {
            name = newName;
            id = static_cast<uint8_t>(numParticipants++);
        }
        std::string                  name;
        uint8_t id;
        std::unique_ptr<IParticipant> participant;
        IDataPublisher* publisher;
        IDataSubscriber* subscriber;
        std::set<uint8_t> receivedIds;
        bool allReceived{ false };
        std::promise<void>           allReceivedPromise;

        bool simtimePassed{false};
        std::promise<void>           simtimePassedPromise;

        void ResetReception()
        {
            receivedIds.clear();
            allReceivedPromise = std::promise<void>{};
            allReceived = false;
        }

        void AwaitCommunication() 
        {
            auto futureStatus = allReceivedPromise.get_future().wait_for(communicationTimeout);
            EXPECT_EQ(futureStatus, std::future_status::ready)
                << "Test Failure: Awaiting test communication timed out";
        }
    };

    struct SystemMaster
    {
        std::unique_ptr<IParticipant> participant;
        ISystemController*           systemController;
        ISystemMonitor*              systemMonitor;
    };

    void ParticipantStatusHandler(const ParticipantStatus& newStatus)
    {
        switch (newStatus.state)
        {
        case ParticipantState::Error:
            //systemMaster.systemController->Shutdown();
            break;

        default:
            break;
        }
    }

    void SystemStateHandler(SystemState newState)
    {
        switch (newState)
        {
        case SystemState::ReadyToRun:
            systemMaster.systemController->Run();
            break;

        case SystemState::Stopped:
            for (auto&& name : syncParticipantNames)
            {
                systemMaster.systemController->Shutdown(name);
            }
            systemMaster.systemController->Shutdown(systemMasterName);
            break;

        case SystemState::Error:
            for (auto&& name : syncParticipantNames)
            {
                systemMaster.systemController->Shutdown(name);
            }
            systemMaster.systemController->Shutdown(systemMasterName);
            break;

        default:
            break;
        }
    }

    void ShutdownAndFailTest(const std::string& reason)
    {

        for (auto&& name : syncParticipantNames)
        {
            systemMaster.systemController->Shutdown(name);
        }
        FAIL() << reason;
    }

    void SyncParticipantThread(TestParticipant& participant, const std::string& registryUri)
    {
        try
        {
            participant.participant =
                SilKit::CreateParticipant(SilKit::Config::MakeEmptyParticipantConfiguration(), participant.name, registryUri);

            auto* lifecycleService = participant.participant->GetLifecycleService();
            auto* timeSyncService = lifecycleService->GetTimeSyncService();

            participant.publisher = participant.participant->CreateDataPublisher("TestPublisher", topic, mediaType, {}, 0);
            participant.subscriber = participant.participant->CreateDataSubscriber(
				"TestSubscriber", topic, mediaType, {},
                [&participant](IDataSubscriber* /*subscriber*/, const DataMessageEvent& dataMessageEvent) {
                    if (!participant.allReceived)
                    {
                        participant.receivedIds.insert(dataMessageEvent.data[0]);
                        // No self delivery: Expect numParticipants-1 receptions
                        if (participant.receivedIds.size() == numParticipants-1)
                        {
                            participant.allReceived = true;
                            participant.allReceivedPromise.set_value();
                        }
                    }
                },
                nullptr);

            timeSyncService->SetPeriod(1s);
            timeSyncService->SetSimulationTask(
                [&participant, this](std::chrono::nanoseconds now) {
                    participant.publisher->Publish(std::vector<uint8_t>{participant.id});
                    if (!participant.simtimePassed && now > simtimeToPass)
                    {
                        participant.simtimePassed = true;
                        participant.simtimePassedPromise.set_value();
                    }
                });
            auto finalStateFuture = lifecycleService->StartLifecycleWithSyncTime(timeSyncService, {true, true});
            finalStateFuture.get();
        }
        catch (const SilKit::ConfigurationError& error)
        {
            std::stringstream ss;
            ss << "Invalid configuration: " << error.what() << std::endl;
            ShutdownAndFailTest(ss.str());
        }
        catch (const std::exception& error)
        {
            std::stringstream ss;
            ss << "Something went wrong: " << error.what() << std::endl;
            ShutdownAndFailTest(ss.str());
        }

    }

    void AsyncParticipantThread(TestParticipant& participant, const std::string& registryUri)
    {
        try
        {
            participant.participant =
                SilKit::CreateParticipant(SilKit::Config::MakeEmptyParticipantConfiguration(), participant.name, registryUri);
            participant.publisher = participant.participant->CreateDataPublisher("TestPublisher", topic, mediaType, {}, 0);
            participant.subscriber = participant.participant->CreateDataSubscriber(
                "TestSubscriber", topic, mediaType, {},
                [&participant](IDataSubscriber* /*subscriber*/, const DataMessageEvent& dataMessageEvent) {
                    if (!participant.allReceived)
                    {
                        participant.receivedIds.insert(dataMessageEvent.data[0]);
                        // No self delivery: Expect numParticipants-1 receptions
                        if (participant.receivedIds.size() == numParticipants - 1)
                        {
                            participant.allReceived = true;
                            participant.allReceivedPromise.set_value();
                        }
                    }
                },
                nullptr);

            while (runAsync)
            {
                participant.publisher->Publish(std::vector<uint8_t>{participant.id});
                std::this_thread::sleep_for(asyncDelayBetweenPublication);
            }

        }
        catch (const SilKit::ConfigurationError& error)
        {
            std::stringstream ss;
            ss << "Invalid configuration: " << error.what() << std::endl;
            ShutdownAndFailTest(ss.str());
        }
        catch (const std::exception& error)
        {
            std::stringstream ss;
            ss << "Something went wrong: " << error.what() << std::endl;
            ShutdownAndFailTest(ss.str());
        }

        // Explicitly delete the com adapter to end the async participant
        participant.participant.reset();
    }

    void RunRegistry(const std::string& registryUri)
    {
        try
        {
            registry = SilKit::Vendor::CreateSilKitRegistry(SilKit::Config::MakeEmptyParticipantConfiguration());
            registry->ProvideDomain(registryUri);
        }
        catch (const SilKit::ConfigurationError& error)
        {
            std::stringstream ss;
            ss << "Invalid configuration: " << error.what() << std::endl;
            ShutdownAndFailTest(ss.str());
        }
        catch (const std::exception& error)
        {
            std::stringstream ss;
            ss << "Something went wrong: " << error.what() << std::endl;
            ShutdownAndFailTest(ss.str());
        }
    }

    void RunSystemMaster(const std::string& registryUri)
    {
        try
        {
            systemMaster.participant =
                SilKit::CreateParticipant(SilKit::Config::MakeEmptyParticipantConfiguration(), systemMasterName, registryUri);

            systemMaster.systemController = systemMaster.participant->GetSystemController();
            systemMaster.systemMonitor = systemMaster.participant->GetSystemMonitor();

            systemMaster.systemController->SetWorkflowConfiguration({syncParticipantNames});

            systemMaster.systemMonitor->AddSystemStateHandler([this](SystemState newState) {
                SystemStateHandler(newState);
            });

            systemMaster.systemMonitor->AddParticipantStatusHandler([this](const ParticipantStatus& newStatus) {
                ParticipantStatusHandler(newStatus);
            });
        }
        catch (const SilKit::ConfigurationError& error)
        {
            std::stringstream ss;
            ss << "Invalid configuration: " << error.what() << std::endl;
            ShutdownAndFailTest(ss.str());
        }
        catch (const std::exception& error)
        {
            std::stringstream ss;
            ss << "Something went wrong: " << error.what() << std::endl;
            ShutdownAndFailTest(ss.str());
        }
    }

    void RunSyncParticipants(std::vector<TestParticipant>& participants, const std::string& registryUri)
    {
        try
        {
            for (auto& p : participants)
            {
                syncParticipantThreads.emplace_back(
                    [this, &p, registryUri] { SyncParticipantThread(p, registryUri); });
            }

        }
        catch (const SilKit::ConfigurationError& error)
        {
            std::stringstream ss;
            ss << "Invalid configuration: " << error.what() << std::endl;
            ShutdownAndFailTest(ss.str());
        }
        catch (const std::exception& error)
        {
            std::stringstream ss;
            ss << "Something went wrong: " << error.what() << std::endl;
            ShutdownAndFailTest(ss.str());
        }
    }

    void RunAsyncParticipants(std::vector<TestParticipant>& participants, const std::string& registryUri)
    {
        runAsync = true;

        try
        {
            for (auto& p : participants)
            {
                asyncParticipantThreads.emplace_back([this, &p, registryUri] { AsyncParticipantThread(p, registryUri); });
            }
        }
        catch (const SilKit::ConfigurationError& error)
        {
            std::stringstream ss;
            ss << "Invalid configuration: " << error.what() << std::endl;
            ShutdownAndFailTest(ss.str());
        }
        catch (const std::exception& error)
        {
            std::stringstream ss;
            ss << "Something went wrong: " << error.what() << std::endl;
            ShutdownAndFailTest(ss.str());
        }
    }

    void StopSyncParticipants()
    {
        for (auto&& thread : syncParticipantThreads)
        {
            thread.join();
        }
        syncParticipantThreads.clear();
    }

    void StopAsyncParticipants()
    {
        runAsync = false;
        for (auto&& thread : asyncParticipantThreads)
        {
            thread.join();
        }
        asyncParticipantThreads.clear();
    }

    void SetupSystem(const std::string& registryUri, std::vector<TestParticipant>& syncParticipants)
    {
        for (auto&& p : syncParticipants)
        {
            syncParticipantNames.push_back(p.name);
        }

        RunRegistry(registryUri);
        RunSystemMaster(registryUri);
        
    }

    void ShutdownSystem()
    {
        asyncParticipantThreads.clear();
        syncParticipantThreads.clear();
        systemMaster.participant.reset();
        registry.reset();
    }

protected:
    std::vector<std::string> syncParticipantNames;
    std::unique_ptr<SilKit::Vendor::ISilKitRegistry> registry;
    SystemMaster systemMaster;
    std::vector<std::thread> syncParticipantThreads;
    std::vector<std::thread> asyncParticipantThreads;

    std::chrono::seconds simtimeToPass{ 3s };
    bool runAsync{ true };


};


TEST_F(HopOnHopOffITest, test_Async_HopOnHopOff_ToSynced)
{
    numParticipants = 0;
    auto registryUri = MakeTestRegistryUri();

    std::vector<TestParticipant> syncParticipants;
    syncParticipants.push_back({ "SyncParticipant1" });
    syncParticipants.push_back({ "SyncParticipant2" });

    std::vector<TestParticipant> asyncParticipants;
    asyncParticipants.push_back({ "AsyncParticipant1" });
    asyncParticipants.push_back({ "AsyncParticipant2" });

    SetupSystem(registryUri, syncParticipants);

    RunSyncParticipants(syncParticipants, registryUri);

    std::cout << "Await simtime progress" << std::endl;
    // Await simtime progress
    for (auto& p : syncParticipants)
    {
        auto futureStatus = p.simtimePassedPromise.get_future().wait_for(communicationTimeout);
        EXPECT_EQ(futureStatus, std::future_status::ready)
            << "Test Failure: Sync participants didn't achieve sim time barrier";
    }

    for (int i = 0; i < 3; i++)
    {
        std::cout << ">> Cycle " << i + 1 << "/3" << std::endl;
        std::cout << ">> Hop on async participants" << std::endl;

        // Hop on with async participant
        RunAsyncParticipants(asyncParticipants, registryUri);

        std::cout << ">> Await successful communication of async/sync participants" << std::endl;
        // Await successful communication of async/sync participants
        for (auto& p : syncParticipants)
            p.AwaitCommunication();
        for (auto& p : asyncParticipants)
            p.AwaitCommunication();

        std::cout << ">> Hop off async participants" << std::endl;
        // Hop off: Stop while-loop of async participants
        StopAsyncParticipants();

        // Reset communication and wait for reception once more for remaining sync participants
        numParticipants = syncParticipants.size();
        for (auto& p : syncParticipants)
            p.ResetReception();

        std::cout << ">> Await successful communication of remaining sync participants" << std::endl;
        for (auto& p : syncParticipants)
            p.AwaitCommunication();

        // Reset communication to repeat the cycle
        numParticipants = syncParticipants.size() + asyncParticipants.size();
        for (auto& p : syncParticipants)
            p.ResetReception();
        for (auto& p : asyncParticipants)
            p.ResetReception();
    }

    // Stop sync participants
    for (auto&& name : syncParticipantNames)
    {
        systemMaster.systemController->Shutdown(name);
    }
    systemMaster.systemController->Shutdown(systemMasterName);

    StopSyncParticipants();

    ShutdownSystem();
}


TEST_F(HopOnHopOffITest, test_Async_HopOnHopOff_ToEmpty)
{
    numParticipants = 0;
    auto registryUri = MakeTestRegistryUri();

    std::vector<TestParticipant> syncParticipants;
    std::vector<TestParticipant> asyncParticipants;
    asyncParticipants.push_back({ "AsyncParticipant1" });
    asyncParticipants.push_back({ "AsyncParticipant2" });

    SetupSystem(registryUri, syncParticipants);

    for (int i = 0; i < 3; i++)
    {
        // Hop on with async participant on empty sim
        RunAsyncParticipants(asyncParticipants, registryUri);

        // Await successful communication of async/sync participants
        for (auto& p : asyncParticipants)
            p.AwaitCommunication();

        // Hop off async participants
        StopAsyncParticipants();

        // Reset communication to repeat the cycle
        for (auto& p : asyncParticipants)
            p.ResetReception();
    }

    ShutdownSystem();
}

} // anonymous namespace
