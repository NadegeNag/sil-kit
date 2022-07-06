// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include <chrono>
#include <string>
#include <functional>

#include "silkit/util/HandlerId.hpp"

namespace SilKit {
namespace Services {
namespace Orchestration {

/*!
* \brief Virtual time provider. Used for send timestamps.
* 
*/
class ITimeProvider
{
public:
    virtual ~ITimeProvider() {}
    //! \brief Get the current simulation time.
    virtual auto Now() const -> std::chrono::nanoseconds = 0;
    //! \brief Name of the time provider, for debugging purposes.
    virtual auto TimeProviderName() const -> const std::string& = 0;

    using NextSimStepHandlerT = std::function<void(std::chrono::nanoseconds now,
        std::chrono::nanoseconds duration)>;

    /*! \brief Register a handler that is executed when the next simulation step is started.
     *
     * \return Returns a \ref HandlerId that can be used to remove the callback.
     */
    virtual auto AddNextSimStepHandler(NextSimStepHandlerT handler) -> HandlerId = 0;

    /*! \brief Remove NextSimStepHandlerT by HandlerId on this time provider.
     *
     * \param handlerId Identifier of the callback to be removed. Obtained upon adding to respective handler.
     */
    virtual void RemoveNextSimStepHandler(HandlerId handlerId) = 0;

    virtual void SetTime(std::chrono::nanoseconds now, std::chrono::nanoseconds duration) = 0;
};


} // namespace Orchestration
} // namespace Services
} // namespace SilKit
