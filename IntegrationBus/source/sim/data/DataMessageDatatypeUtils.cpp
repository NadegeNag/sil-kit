// Copyright (c) Vector Informatik GmbH. All rights reserved.

#include "DataMessageDatatypeUtils.hpp"

namespace ib {
namespace sim {
namespace data {

bool operator==(const DataMessage& lhs, const DataMessage& rhs)
{
    return lhs.data == rhs.data;
}

bool Match(const DataExchangeFormat& subscriberDxf, const DataExchangeFormat& publisherDxf)
{
    return subscriberDxf.mediaType == "" || subscriberDxf.mediaType == publisherDxf.mediaType;
}

bool MatchLabels(const std::map<std::string, std::string>& subscriberLabels, const std::map<std::string, std::string>& publisherLabels)
{
    if (subscriberLabels.size() == 0)
        return true; // subscriberLabels empty -> match

    if (subscriberLabels.size() > publisherLabels.size())
        return false; // subscriberLabels more labels than outer set -> no match

    for (auto&& kv : subscriberLabels)
    {
        auto it = publisherLabels.find(kv.first);
        if (it == publisherLabels.end() || // Key not found -> no match
            (kv.second != "" && kv.second != (*it).second)) // Value does not match (and no wildcard given) -> no match
        {
            return false;
        }
    }
    return true; // All of subscriberLabels is there -> match
}

} // namespace data
} // namespace sim
} // namespace ib
