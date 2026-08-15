#pragma once

#include <cstdint>
#include <string>
#include <nlohmann/json.hpp>

class SensorMessage
{
public:
    SensorMessage(
        std::string type,
        std::string deviceId,
        std::uint64_t sequence,
        double distance,
        std::string timestamp
    );

    void print() const;
    nlohmann::json toJson() const;

private:
    std::string type_;
    std::string deviceId_;
    std::uint64_t sequence_;
    double distance_;
    std::string timestamp_;
};