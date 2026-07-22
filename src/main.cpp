#include "SensorMessage.h"

#include <iostream>

int main()
{
    std::cout << "=== ISV Device Simulator ===\n";

    SensorMessage message(
        "SIM-A",
        1,
        150.0
    );

    message.print();

    nlohmann::json jsonMessage = message.toJson();
    std::cout << "\n=== JSON Message ===\n";
    std::cout << jsonMessage.dump(4) << "\n";
    return 0;
}