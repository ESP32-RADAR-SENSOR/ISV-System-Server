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

    return 0;
}