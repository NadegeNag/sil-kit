// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include <string>
#include <memory>
#include <chrono>
#include <tuple>

#include "silkit/services/logging/fwd_decl.hpp"

#include "EndpointAddress.hpp"
#include "TraceMessage.hpp"

namespace SilKit {


//! \brief SinkType specifies the type of the output backend to use for permanent storage.
enum class SinkType
{
	PcapFile,
	PcapNamedPipe,
	Mdf4File
};

//! \brief Messages traces are written to a message sink.
//         It might be provided by an SIL Kit extension or built into SilKit

class ITraceMessageSink
{
public:
    virtual ~ITraceMessageSink() = default;


    virtual void Open(SinkType type, const std::string& outputPath) = 0;
    virtual void Close() = 0;
    virtual auto GetLogger() const -> Services::Logging::ILogger* = 0;
    virtual auto Name() const -> const std::string& = 0;

    virtual void Trace(
        SilKit::Services::TransmitDirection dir,
        const Core::EndpointAddress& address, //!< the address is used to identify the controller this message is from
        std::chrono::nanoseconds timestamp,
        const TraceMessage& message) = 0;
};

//! \brief Helper class to instantiate a trace message sink from a shared library module.
class ITraceMessageSinkFactory
{
public:
    virtual ~ITraceMessageSinkFactory() = default;
    // TODO
    virtual auto Create(/*Config::Config config, */
            SilKit::Services::Logging::ILogger* logger,
            std::string participantName,
            std::string sinkName
        )
       -> std::unique_ptr<ITraceMessageSink> = 0;
};


} //end namespace SilKit