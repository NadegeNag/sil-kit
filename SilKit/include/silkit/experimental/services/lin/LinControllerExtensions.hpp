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

#pragma once

#include "silkit/SilKitMacros.hpp"
#include "silkit/services/lin/ILinController.hpp"

namespace SilKit {
namespace Experimental {
namespace Services {
namespace Lin {

using namespace SilKit::Services::Lin;

/*! \brief A LIN frame response update event delivered in the \ref LinSlaveConfigurationHandler
*
* The event is received on a LIN Master when a LIN Slave is configured via LinController::Init().
* This event is mainly for diagnostic purposes and can be used to keep track of LIN Ids, where
* a response of a LIN Slave is to be expected by using \ref GetSlaveConfiguration() in the handler.
* 
*/
struct LinSlaveConfigurationEvent
{
    std::chrono::nanoseconds timestamp; //!< Time of the event.
};

/*! Callback type to indicate that a LIN Slave configuration has been received.
 *  
 * Triggered when a remote LIN Slave calls LinController::Init() or LinController::SetFrameResponse().
 *  Cf., \ref AddLinSlaveConfigurationHandler
 */
using LinSlaveConfigurationHandler = ILinController::CallbackT<LinSlaveConfigurationEvent>;

//! \brief The aggregated configuration of all LIN slaves in the network.
struct LinSlaveConfiguration
{
    std::vector<LinId>
        respondingLinIds; //!< A vector of LinIds on which any LIN Slave has configured LinFrameResponseMode::TxUnconditional
};

/*! \brief Add a LinSlaveConfigurationHandler on a given controller that triggers when a remote LIN slave is changes its configuration.
 *
 * This callback is mainly for diagnostic purposes and is NOT needed for regular LIN controller operation. 
 * It can be used to call \ref GetSlaveConfiguration to keep track of LIN Ids, where
 * a response of a LIN Slave is to be expected.
 * 
 * Requires \ref Services::Lin::LinControllerMode::Master.
 * 
 * \param linController The LIN controller to add the handler.
 * \param handler The callback that is triggered upon a configuration update.
 * 
 * \return Returns a \ref SilKit::Util::HandlerId that can be used to remove the callback.
 */
SilKitAPI auto AddLinSlaveConfigurationHandler(ILinController* linController, LinSlaveConfigurationHandler handler)
    -> SilKit::Util::HandlerId;

/*! \brief Remove a LinSlaveConfigurationHandler by HandlerId on a given controller.
 *
 * Requires \ref Services::Lin::LinControllerMode::Master.
 * 
 * \param linController The LIN controller to remove the handler.
 * \param handlerId Identifier of the callback to be removed. Obtained upon adding to respective handler.
 */
SilKitAPI void RemoveLinSlaveConfigurationHandler(ILinController* linController, SilKit::Util::HandlerId handlerId);

/*! \brief Get the aggregated response configuration of all LIN slaves in the network.
 *
 * Requires \ref Services::Lin::LinControllerMode::Master.
 * 
 * \param linController The LIN controller (master) to providing the view.
 * 
 * \return A struct containing all LinIds on which LIN Slaves have configured Services::Lin::LinFrameResponseMode::TxUnconditional.
 */
SilKitAPI auto GetSlaveConfiguration(ILinController* linController) -> LinSlaveConfiguration;

} // namespace Lin
} // namespace Services
} // namespace Experimental
} // namespace SilKit
