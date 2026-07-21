#pragma once

#include <cstdint>
#include <string>

class SensorMessage
{
public:
	SensorMessage(
		std::string deviceId,
		std::uint64_t sequnece,
		double distance
	);

	void print() const;


private:
	std::string deviceId_;
	std::uint64_t sequence_;
	double distance_;

};
