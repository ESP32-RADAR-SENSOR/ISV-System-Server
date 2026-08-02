#include "SensorMessage.h"

#include <boost/asio.hpp>

#include <iostream>
#include <string>

int main()
{
    try
    {
        using boost::asio::ip::tcp;

        SensorMessage message(
            "SIM-A",
            1,
            150.0
        );

        const nlohmann::json jsonMessage =
            message.toJson();

        const std::string serializedMessage =
            jsonMessage.dump() + '\n';

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

        boost::asio::write(
            socket,
            boost::asio::buffer(serializedMessage)
        );

        std::cout << "\n=== Sent Message ===\n";
        std::cout << serializedMessage;
    }
    catch (const std::exception& exception)
    {
        std::cerr << "Simulator error: "
            << exception.what() << '\n';

        return 1;
    }

    return 0;
}