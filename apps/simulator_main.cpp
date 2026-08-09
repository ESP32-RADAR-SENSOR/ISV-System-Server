#include "SensorMessage.h"

#include <boost/asio.hpp>

#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>
#include <thread>

int main()
{

    try
    {
        using boost::asio::ip::tcp;

        //SensorMessage message(
        //    "distance",
        //    "SENSOR-A",
        //    1,
        //    150.0,
        //    "2026-08-02T13:54:00+09:00"
        //);

        //const nlohmann::json jsonMessage =
        //    message.toJson();

        //const std::string serializedMessage =
        //    jsonMessage.dump() + '\n';

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



        /*boost::asio::write(
            socket,
            boost::asio::buffer(serializedMessage)
        );

        std::cout << "\n=== Sent Message ===\n";
        std::cout << serializedMessage;*/

        std::uint64_t sequence = 1;
        double distanceCm = 150.0;

        while (true)
        {
            SensorMessage message(
                "distance",
                "SENSOR-A",
                sequence,
                distanceCm,
                "2026-08-09T12:34:00+09:00"
            );

            const std::string serializedMessage =
                message.toJson().dump() + '\n';

            std::cout << serializedMessage << "\n";

            boost::asio::write(
                socket,
                boost::asio::buffer(serializedMessage)
            );

            std::cout
                << "Sent: seq="
                << sequence
                << ", distance="
                << distanceCm
                << '\n';

            ++sequence;
            distanceCm -= 2.0;

            std::this_thread::sleep_for(
                std::chrono::seconds(1)
            );
        }
    }
    catch (const std::exception& exception)
    {
        std::cerr << "Simulator error: "
            << exception.what() << '\n';

        return 1;
    }

    return 0;
}