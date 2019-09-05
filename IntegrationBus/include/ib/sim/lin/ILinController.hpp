// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include <functional>

#include "LinDatatypes.hpp"
#include "ib/mw/EndpointAddress.hpp"

namespace ib {
namespace sim {
namespace lin {

/*! 
 * The LIN controller can assume the role of a LIN master or a LIN
 * slave. It provides two kinds of interfaces to perform data
 * transfers and provide frame responses:
 *
 * AUTOSAR-like LIN master interface:
 *
 * - \ref SendFrame() transfers a frame from or to a LIN
 * master. Requires \ref ControllerMode::Master.
 *
 *
 * non-AUTOSAR interface:
 *
 * - \ref SetFrameResponse() configures
 * the response for a particular LIN identifier. Can be used with \ref
 * ControllerMode::Master and \ref ControllerMode::Slave.
 *
 * - \ref SendFrameHeader() initiates the transmission of a
 * LIN frame for a particular LIN identifier. For a successful
 * transmission, exactly one LIN slave or master must have previously
 * set a corresponding frame response for unconditional
 * transmission. Requires \ref ControllerMode::Master.
 * 
 */
class ILinController
{
public:
    /*! Callback type to indicate the end of a LIN Frame transmission.
     *  Cf., \ref RegisterFrameStatusHandler(FrameStatusHandler);
     */
    using FrameStatusHandler = std::function<void(ILinController*, const Frame&, FrameStatus, std::chrono::nanoseconds timestamp)>;
    
    /*! Callback type to indicate that a go-to-sleep frame has been received.
     *  Cf., \ref RegisterGoToSleepHandler(GoToSleepHandler);
     */
    using GoToSleepHandler = std::function<void(ILinController*)>;

    /*! Callback type to indicate that a wakeup pulse has been received.
     *  Cf., \ref RegisterWakeupHandler(WakeupHandler);
     */
    using WakeupHandler = std::function<void(ILinController*)>;
    
    using FrameResponseUpdateHandler = std::function<void(ILinController*, mw::EndpointAddress, const FrameResponse&)>;
    
public:
    virtual ~ILinController() = default;

    /*! \brief Initialize the LIN controller
     * 
     * \param config The Controller configuration contains:
     *  - controllerMode, either sets LIN master or LIN slave mode
     *  - baudRate, determine transmission speeds (only used for VIBE simulation)
     *  - frameResponses, an optional set of initial FrameResponses
     *
     * *AUTOSAR Name:* Lin_Init
     */
    virtual void Init(ControllerConfig config) = 0;

    //! \brief Get the current status of the LIN Controller, i.e., Operational or Sleep.
    virtual auto Status() const noexcept -> ControllerStatus = 0;

    /*! \brief AUTOSAR LIN master interface
     *
     * Perform a full LIN data transfer, i.e., frame header + frame
     * response. The responseType determines if frame.data is used for
     * the frame response or if a different node has to provide it:
     *
     * \li MasterResponse: \ref Frame is sent from this controller to
     *     all connected slaves.
     * \li SlaveResponse: the frame response must be provided by a
     *     connected slave and is received by this controller.
     * \li SlaveToSlave: the frame response must be provided by a
     *     connected slave and is ignored by this controller.
     *
     * *AUTOSAR Name:* Lin_SendFrame
     *
     * \param frame provides the LIN identifier, checksum model, and optional data
     * \param responseType determines if *frame.data* must is used for the frame response.
     */
    virtual void SendFrame(Frame frame, FrameResponseType responseType) = 0;

    //! Send Interface for a non-AUTOSAR LIN Master
    virtual void SendFrameHeader(LinIdT linId) = 0;
    /*! FrameResponse configuration for Slaves or non-AUTOSAR LIN
     *  Masters The corresponding LIN ID does not need to be
     *  previously configured. */
    virtual void SetFrameResponse(Frame frame, FrameResponseMode mode) = 0;
    /*! FrameResponse configuration for Slaves or non-AUTOSAR LIN Masters.
     * 
     * Configures multiple responses at once. Corresponding IDs do not
     * need to be previously configured.
     *
     * NB: only configures responses for the provided LIN IDs. I.e.,
     * an empty vector does not clear or reset the currently
     * configured FrameResponses.
     */
    virtual void SetFrameResponses(std::vector<FrameResponse> responses) = 0;

    /*! \brief Transmit a go-to-sleep-command and set ControllerState::Sleep and enable wake-up
     *
     * *AUTOSAR Name:* Lin_GoToSleep
     */
    virtual void GoToSleep() = 0;
    /*! \brief Set ControllerState::Sleep without sending a go-to-sleep command.
     *
     * *AUTOSAR Name:* Lin_GoToSleepInternal
     */
    virtual void GoToSleepInternal() = 0;
    /*! \brief Generate a wake up pulse and set ControllerState::Operational.
     *
     * *AUTOSAR Name:* Lin_Wakeup
     */
    virtual void Wakeup() = 0;
    /*! Set ControllerState::Operational without generating a wake up pulse.
     *
     * *AUTOSAR Name:* Lin_WakeupInternal
     */
    virtual void WakeupInternal() = 0;

    /*! \brief Report the \ref FrameStatus of a LIN \ref Frame
     * transmission and provides the transmitted frame.
     *
     * The FrameStatusHandler is called once per call to
     * SendFrame() or call to
     * SendFrameHeader(). The handler is called independently
     * of the transmission's success or failure.
     *
     * The FrameStatusHandler is called for all participating LIN
     * controllers. I.e., for LIN masters, it is always called, and
     * for LIN slaves, it is called if the corresponding \ref LinIdT is
     * configured SlaveFrameResponseMode::Rx or
     * SlaveFrameResponseMode::TxUnconditional.
     *
     * <em>Note: this is one of the major changes to the previous version.
     * Previously, frame transmission was indicated using different
     * means. For Masters, a TX was confirmed using the
     * TxCompleteHandler while an RX was handled using
     * ReceiveMessageHandler. For LIN slaves the confirmation varied
     * for simple simulation and VIBE simulation.</em>
     */
    virtual void RegisterFrameStatusHandler(FrameStatusHandler) = 0;

    /*! \brief The GoToSleepHandler is called whenever a go-to-sleep frame
     * was received.
     *
     * Note: The LIN controller does not automatically enter sleep
     * mode up reception of a go-to-sleep frame. I.e.,
     * GoToSleepInternal() must be called manually
     *
     * NB: This handler will always be called, independently of the
     * \ref FrameResponseMode configuration for LIN ID 0x3C. However,
     * regarding the FrameStatusHandler, the go-to-sleep frame is
     * treated like every other frame, i.e. the FrameStatusHandler is
     * only called for LIN ID 0x3C if configured as
     * FrameResponseMode::Rx.
     */
    virtual void RegisterGoToSleepHandler(GoToSleepHandler handler) = 0;

    /*! \brief The WakeupHandler is called whenever a wake up pulse is received
     *
     * Note: The LIN controller does not automatically enter
     * operational mode on wake up pulse detection. I.e.,
     * WakeInternal() must be called manually.
     */
    virtual void RegisterWakeupHandler(WakeupHandler handler) = 0;

    /*! \brief The FrameResponseUpdateHandler provides direct access
     * to the FrameResponse configuration of other LIN controllers.
     *
     * NB: This callback is mainly for diagnostic purposes and is NOT
     * needed for regular LIN controller operation.
     */
    virtual void RegisterFrameResponseUpdateHandler(FrameResponseUpdateHandler handler) = 0;
};

} // namespace lin
} // namespace sim
} // namespace ib


