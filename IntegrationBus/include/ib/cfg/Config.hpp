// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include "ib/IbMacros.hpp"
#include "ib/mw/EndpointAddress.hpp"
#include "ib/sim/io/IoDatatypes.hpp"
#include "ib/sim/fr/FrDatatypes.hpp"
#include "ib/mw/logging/LoggingDatatypes.hpp"

namespace ib {
//! The config namespace
namespace cfg {

struct Version
{
    uint16_t major{0};
    uint16_t minor{0};
    uint16_t patchLevel{0};
};

// ================================================================================
//  SimulationSetup Config
// ================================================================================
struct Link
{
    enum class Type
    {
        Undefined,
        Invalid,
        CAN,
        LIN,
        Ethernet,
        FlexRay,
        DigitalIo,
        AnalogIo,
        PwmIo,
        PatternIo,
        GenericMessage
    };

    std::string name;
    int16_t id{-1};
    Type type = Type::Undefined;

    std::vector<std::string> endpoints;
};

struct Sink
{
    enum class Type
    {
        Remote,
        Stdout,
        File
    };

    Type type = Type::Remote;
    mw::logging::Level level = mw::logging::Level::Info;
    std::string logname;
};

struct Logger
{
    bool logFromRemotes{false};
    mw::logging::Level flush_level = mw::logging::Level::Off;
    std::vector<Sink> sinks;
};

struct CanController
{
    static constexpr Link::Type linkType = Link::Type::CAN;

    std::string name;
    mw::EndpointId endpointId{0};
    int16_t linkId{-1};
};

struct LinController
{
    static constexpr Link::Type linkType = Link::Type::LIN;

    std::string name;
    mw::EndpointId endpointId{0};
    int16_t linkId{-1};
};

struct EthernetController
{
    static constexpr Link::Type linkType = Link::Type::Ethernet;

    std::string name;
    mw::EndpointId endpointId{0};
    int16_t linkId{-1};
    std::array<uint8_t, 6> macAddress{};
};

struct FlexrayController
{
    static constexpr Link::Type linkType = Link::Type::FlexRay;

    std::string name;
    mw::EndpointId endpointId{0};
    int16_t linkId{-1};

    sim::fr::ClusterParameters clusterParameters;
    sim::fr::NodeParameters nodeParameters;
    std::vector<sim::fr::TxBufferConfig> txBufferConfigs;
};

enum class PortDirection : uint8_t
{
    In,
    Out,
    InOut
};

template<typename T>
constexpr auto get_io_link_type()
{
    return std::is_same<T, bool>::value ? Link::Type::DigitalIo
        : std::is_same<T, double>::value ? Link::Type::AnalogIo
        : std::is_same<T, sim::io::PwmValue>::value ? Link::Type::PwmIo
        : std::is_same<T, std::vector<uint8_t>>::value ? Link::Type::PatternIo
        : Link::Type::Invalid;
}

template <typename ValueT>
struct IoPort
{
    using ValueType = ValueT;
    static constexpr Link::Type linkType = get_io_link_type<ValueT>();

    std::string name;
    mw::EndpointId endpointId{0};
    int16_t linkId{-1};
    PortDirection direction{PortDirection::InOut};

    ValueT initvalue;
    std::string unit;
};

using AnalogIoPort  = IoPort<double>;
using DigitalIoPort = IoPort<bool>;
using PwmPort       = IoPort<sim::io::PwmValue>;
using PatternPort   = IoPort<std::vector<uint8_t>>;

struct GenericPort
{
    static constexpr Link::Type linkType = Link::Type::GenericMessage;

    enum class ProtocolType
    {
        Undefined,
        ROS,
        SOMEIP
    };

    std::string name;
    mw::EndpointId endpointId{0};
    int16_t linkId{-1};


    //! \brief The serialization protocol used to encode this message.
    ProtocolType protocolType{ProtocolType::Undefined};

    /*! \brief URI where the \ref protocolType specific definition FILE can be found
     *
     * If \ref definitionUri is a relative path, the path should be
     * treated relative to the config file.
    */
    std::string definitionUri;
};

enum class SyncType
{
    DistributedTimeQuantum,  //!< TimeQuantum synchronization using a distributed algorithm. When using VAsio middleware, this SyncType provides inherent strict message delivery.
    DiscreteEvent,           //!< Classic discrete event simulation
    TimeQuantum,             //!< Dynamic length time quanta requested by the participants
    DiscreteTime,            //!< Simulation advances according to clock "Ticks" generated by a time master, Participant sends a "TickDone" on completion
    DiscreteTimePassive,     //!< Same as DiscreteTime, but the participant only listens to Ticks and does not send a "TickDone"
    Unsynchronized           //!< The participant does not participate in time synchronization and is not publish a participant state.
};

struct ParticipantController
{
    // ParticipantControllers are optional configuration items of a Participant
    // configuration. In C++17, this would be modeled with a
    // std::optional<ParticipantController>
    // participantController. Unfortunately, we are currently limited to
    // C++14. To keep things simple, we added a boolean member _is_configured to
    // ParticipantController, which is intended to mimic std::optional's
    // has_value(). I.e., if _is_configured == true, a ParticipantController was
    // configured.
    bool _is_configured = false;

    SyncType syncType = SyncType::Unsynchronized;

    std::chrono::milliseconds execTimeLimitSoft = std::chrono::milliseconds::max();
    std::chrono::milliseconds execTimeLimitHard = std::chrono::milliseconds::max();
};

struct Participant
{
    std::string name;
    std::string description;

    mw::ParticipantId id{0};

    Logger logger;
    ParticipantController participantController; //!< "optional" member.
    std::vector<CanController> canControllers;
    std::vector<LinController> linControllers;
    std::vector<EthernetController> ethernetControllers;
    std::vector<FlexrayController> flexrayControllers;
    std::vector<std::string> networkSimulators;

    std::vector<DigitalIoPort> digitalIoPorts;
    std::vector<AnalogIoPort> analogIoPorts;
    std::vector<PwmPort> pwmPorts;
    std::vector<PatternPort> patternPorts;

    std::vector<GenericPort> genericPublishers;
    std::vector<GenericPort> genericSubscribers;

    bool isSyncMaster{false};
};

struct Switch
{
    struct Port
    {
        static constexpr Link::Type linkType{Link::Type::Ethernet};

        std::string name;
        mw::EndpointId endpointId{0};
        std::vector<uint16_t> vlanIds;
    };
    std::string name;
    std::string description;
    std::vector<Port> ports;
};

struct NetworkSimulator
{
    std::string name;

    std::vector<std::string> simulatedLinks;
    std::vector<std::string> simulatedSwitches;
};

struct TimeSync
{
    //! FIXME: doxygen
    enum class SyncPolicy
    {
        Loose, //!< There is no guarantee that data has been received before the next simulation cycle
        Strict //!< Enforce that all sent data has been received before the next simulation cycle
    };
    SyncPolicy syncPolicy{SyncPolicy::Loose}; //!< FIXME: doxygen
    std::chrono::nanoseconds tickPeriod{0}; //!< FIXME: doxygen
};

struct SimulationSetup
{
    std::vector<Participant> participants;
    std::vector<Switch> switches;
    std::vector<Link> links;
    std::vector<NetworkSimulator> networkSimulators;

    TimeSync timeSync;
};

// ================================================================================
//  Middleware Config
// ================================================================================
namespace FastRtps {
enum class DiscoveryType
{
    Local,
    Multicast,
    Unicast,
    ConfigFile
};

struct Config
{
    DiscoveryType discoveryType{DiscoveryType::Local};
    std::map<std::string, std::string> unicastLocators;
    std::string configFileName;
    int sendSocketBufferSize{-1};
    int listenSocketBufferSize{-1};
};
} // namespace FastRTPS

namespace VAsio {
struct RegistryConfig
{
    std::string hostname{"localhost"};
    uint16_t port{8500};
};
struct Config
{
    RegistryConfig registry;
};
} // namespace VAsio

enum class Middleware
{
    NotConfigured = 0,
    FastRTPS = 1,
    VAsio = 2
};

struct MiddlewareConfig
{
    Middleware activeMiddleware = Middleware::NotConfigured;
    FastRtps::Config fastRtps;
    VAsio::Config vasio;
};

// ================================================================================
//  Main Config
// ================================================================================
struct Config
{
    Version version;
    std::string name;
    std::string description;
    std::string configPath;

    SimulationSetup simulationSetup;
    MiddlewareConfig middlewareConfig;

    /*! \brief Parse configuration from a JSON string.
    *
    * Create a configuration data object from settings described by a
    * JSON string.
    *
    * \param jsonString A string that adheres to our JSON schema.
    * \return The configuration data
    *
    * \throw ib::cfg::Misconfiguration The input string violates the
    * JSON format, schema or an integrity rule.
    */
    IntegrationBusAPI static auto FromJsonString(const std::string& jsonString) -> Config;
    /*! \brief Parse configuration from a JSON file.
    *
    * Create a configuration data object from settings described by a
    * JSON file.
    *
    * \param jsonFilename Path to the JSON file.
    * \return The configuration data
    *
    * \throw ib::cfg::Misconfiguration The file could not be read, or
    * the input string violates the JSON format, schema or an
    * integrity rule.
    */
    IntegrationBusAPI static auto FromJsonFile(const std::string& jsonFilename) -> Config;

    /*! \brief Convert this configuration into JSON.
    *
    * Create a JSON-formatted string representation from a
    * configuration data object.
    *
    * \return The JSON-formatted string
    */
    IntegrationBusAPI auto ToJsonString() -> std::string;
};

class Misconfiguration : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};

IntegrationBusAPI bool operator==(const Version& lhs, const Version& rhs);
IntegrationBusAPI bool operator==(const Participant& lhs, const Participant& rhs);
IntegrationBusAPI bool operator==(const Link& lhs, const Link& rhs);
IntegrationBusAPI bool operator==(const NetworkSimulator& lhs, const NetworkSimulator& rhs);
IntegrationBusAPI bool operator==(const Switch::Port& lhs, const Switch::Port& rhs);
IntegrationBusAPI bool operator==(const Switch& lhs, const Switch& rhs);
IntegrationBusAPI bool operator==(const TimeSync& lhs, const TimeSync& rhs);
IntegrationBusAPI bool operator==(const SimulationSetup& lhs, const SimulationSetup& rhs);
IntegrationBusAPI bool operator==(const FastRtps::Config& lhs, const FastRtps::Config& rhs);
IntegrationBusAPI bool operator==(const MiddlewareConfig& lhs, const MiddlewareConfig& rhs);
IntegrationBusAPI bool operator==(const Config& lhs, const Config& rhs);
IntegrationBusAPI bool operator==(const CanController& lhs, const CanController& rhs);
IntegrationBusAPI bool operator==(const LinController& lhs, const LinController& rhs);
IntegrationBusAPI bool operator==(const EthernetController& lhs, const EthernetController& rhs);
IntegrationBusAPI bool operator==(const FlexrayController& lhs, const FlexrayController& rhs);
IntegrationBusAPI bool operator==(const DigitalIoPort& lhs, const DigitalIoPort& rhs);
IntegrationBusAPI bool operator==(const AnalogIoPort& lhs, const AnalogIoPort& rhs);
IntegrationBusAPI bool operator==(const PwmPort& lhs, const PwmPort& rhs);
IntegrationBusAPI bool operator==(const PatternPort& lhs, const PatternPort& rhs);
IntegrationBusAPI bool operator==(const GenericPort& lhs, const GenericPort& rhs);

IntegrationBusAPI std::ostream& operator<<(std::ostream& out, const Version& version);
IntegrationBusAPI std::istream& operator>>(std::istream& in, Version& version);

// Yes, I know that this calls for operator<<(). However, I don't want to overload operator<< for std::array.
std::ostream& to_ostream(std::ostream& out, const std::array<uint8_t, 6>& macAddr);
std::istream& from_istream(std::istream& in, std::array<uint8_t, 6>& macAddr);

template<typename T>
inline auto find_by_name(T&& range, const std::string& name) -> decltype(range.begin());
template <typename T>
inline auto get_by_name(T&& range, const std::string& name) -> decltype(*range.begin());

// ================================================================================
//  Inline Implementations
// ================================================================================
template<typename T>
inline auto find_by_name(T&& range, const std::string& name) -> decltype(range.begin())
{
    return std::find_if(range.begin(), range.end(), [&name](auto&& elem) { return elem.name == name; });
}

template <typename T>
inline auto get_by_name(T&& range, const std::string& name) -> decltype(*range.begin())
{
    auto&& iter = find_by_name(std::forward<T>(range), name);
    if (iter == range.end())
        throw ib::cfg::Misconfiguration{"Missing ConfigItem with name '" + name + "'"};
    else
        return *iter;
}

} // namespace cfg
} // namespace ib
