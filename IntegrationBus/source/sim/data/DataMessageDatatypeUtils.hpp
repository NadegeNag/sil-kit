// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include <ostream>
#include <set>
#include <memory>

#include "ib/sim/data/DataMessageDatatypes.hpp"

namespace ib {
namespace sim {
namespace data {

class DataSubscriberInternal;

bool operator==(const DataMessage& lhs, const DataMessage& rhs);

bool               Match(const DataExchangeFormat& subscriberDxf, const DataExchangeFormat& publisherDxf);

bool MatchLabels(const std::map<std::string, std::string>& innerSet, const std::map<std::string, std::string>& outerSet);

struct SpecificDataHandler
{
    uint64_t id;
    DataExchangeFormat dataExchangeFormat;
    std::map<std::string, std::string> labels;
    DataHandlerT specificDataHandler;
    std::set<DataSubscriberInternal*> registeredInternalSubscribers;
};

} // namespace data
} // namespace sim
} // namespace ib
