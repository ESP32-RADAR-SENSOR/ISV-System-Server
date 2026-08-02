#include "SensorMessage.h"

#include <iostream>
#include <utility>

SensorMessage::SensorMessage(
    std::string type,
    std::string deviceId,
    std::uint64_t sequence,
    double distanceCm,
    std::string timestamp

)
    : deviceId_(std::move(deviceId)),
      sequence_(sequence),
      distanceCm_(distanceCm),
      timestamp_(std::move(timestamp))

{

}

void SensorMessage::print() const
{   
    std::cout << "type: " << type_ << "\n";
    std::cout << "deviceId: " << deviceId_ << '\n';
    std::cout << "sequence: " << sequence_ << '\n';
    std::cout << "distanceCm: " << distanceCm_ << " cm\n";
    std::cout << "timestamp: " << timestamp_ << "\n";
}

nlohmann::json SensorMessage::toJson() const
{
    return {
        {"type",type_},
        {"deviceId", deviceId_},
        {"sequence", sequence_},
        {"distance", distanceCm_},
        {"timestamp",timestamp_}
    };
}