#include "SensorMessage.h"

#include <iostream>
#include <utility>

SensorMessage::SensorMessage(
    std::string deviceId,
    std::uint64_t sequence,
    double distance
)
    : deviceId_(std::move(deviceId)),
    sequence_(sequence),
    distance_(distance)
{
}

void SensorMessage::print() const
{
    std::cout << "deviceId: " << deviceId_ << '\n';
    std::cout << "sequence: " << sequence_ << '\n';
    std::cout << "distance: " << distance_ << " cm\n";
}