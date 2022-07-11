// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

namespace SilKit {
namespace Services {
namespace Can {
class IMsgForCanSimulator;
} // namespace Can
namespace Ethernet {
class IMsgForEthSimulator;
} // namespace Ethernet
namespace Flexray {
class IMsgForFlexrayBusSimulator;
} // namespace Flexray
namespace Lin {
class IMsgForLinSimulator;
} // namespace Lin
namespace PubSub {
class IMsgForDataPublisher;
class IMsgForDataSubscriber;
class IMsgForDataSubscriberInternal;
class DataSubscriberInternal;
} // namespace PubSub
namespace Rpc {
class IMsgForRpcClient;
class IMsgForRpcServer;
class IMsgForRpcServerInternal;
class RpcServerInternal;
class RpcDiscoverer;
} // namespace Rpc
namespace Orchestration {
class LifecycleService;
class TimeSyncService;
} // namespace Orchestration
} // namespace Services
namespace Core {
namespace Discovery {
class IServiceDiscovery;
} // namespace Discovery
} // namespace Core
} // namespace SilKit
