#include "SensorMessage.h"

#include <iostream>
#include <utility>

SensorMessage::SensorMessage(
    std::string type,
    std::string deviceId,
    std::uint64_t sequence,
    double distance,
    std::string timestamp
)
    : type_(std::move(type)),
    deviceId_(std::move(deviceId)),
    sequence_(sequence),
    distance_(distance),
    timestamp_(std::move(timestamp))
{
}

void SensorMessage::print() const
{
    std::cout << "type: " << type_ << '\n';
    std::cout << "deviceId: " << deviceId_ << '\n';
    std::cout << "sequence: " << sequence_ << '\n';
    std::cout << "distance: " << distance_ << " m\n";
    std::cout << "timestamp: " << timestamp_ << '\n';
}

nlohmann::json SensorMessage::toJson() const
{
    return {
        {"type", type_},
        {"deviceId", deviceId_},
        {"sequence", sequence_},
        {"distance", distance_},
        {"timestamp", timestamp_}
    };
}