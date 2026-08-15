#include "SensorMessage.h"

#include <boost/asio.hpp>

#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>
#include <thread>

int main(int argc, char* argv[])
{
    try
    {
        using boost::asio::ip::tcp;

        std::string deviceId = "SENSOR-A";

        if (argc >= 2)
        {
            deviceId = argv[1];
        }

        std::cout << "Device ID argument: " << deviceId << '\n';

        boost::asio::io_context ioContext;

        tcp::socket socket(ioContext);

        tcp::endpoint serverEndpoint(
            boost::asio::ip::make_address("127.0.0.1"),
            9000
        );

        std::cout << "=== Sensor Simulator ===\n";
        std::cout << "Connecting to server...\n";

        socket.connect(serverEndpoint);

        std::cout << "Connected.\n";

        std::uint64_t sequence = 1;

        // ´ÜÀ§: meter
        double distance = 1.5;

        while (true)
        {
            SensorMessage message(
                "distance",
                deviceId,
                sequence,
                distance,
                "2026-08-15T12:30:00+09:00"
            );

            const std::string serializedMessage =
                message.toJson().dump() + '\n';

            std::cout << serializedMessage;

            boost::asio::write(
                socket,
                boost::asio::buffer(serializedMessage)
            );

            std::cout
                << "Sent: seq="
                << sequence
                << ", distance="
                << distance
                << " m\n";

            ++sequence;

            distance -= 0.02;

            std::this_thread::sleep_for(
                std::chrono::seconds(1)
            );
        }
    }
    catch (const std::exception& exception)
    {
        std::cerr
            << "Simulator error: "
            << exception.what()
            << '\n';

        return 1;
    }

    return 0;
}