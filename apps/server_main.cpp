#include <boost/asio.hpp>
#include <nlohmann/json.hpp>

#include <cstdint>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <algorithm>

using boost::asio::ip::tcp;

// 현재 연결된 WPF 클라이언트들
std::vector<std::shared_ptr<tcp::socket>> monitorClients;
std::mutex monitorMutex;


bool validateSensorMessage(
    const nlohmann::json& message,
    std::string& errorMessage
)
{
    if (!message.is_object())
    {
        errorMessage = "JSON root must be an object.";
        return false;
    }

    if (!message.contains("type"))
    {
        errorMessage = "Missing field: type";
        return false;
    }

    if (!message.contains("deviceId"))
    {
        errorMessage = "Missing field: deviceId";
        return false;
    }

    if (!message.contains("sequence"))
    {
        errorMessage = "Missing field: sequence";
        return false;
    }

    if (!message.contains("distance"))
    {
        errorMessage = "Missing field: distance";
        return false;
    }

    if (!message.contains("timestamp"))
    {
        errorMessage = "Missing field: timestamp";
        return false;
    }

    if (!message.at("type").is_string())
    {
        errorMessage = "type must be a string.";
        return false;
    }

    if (!message.at("deviceId").is_string())
    {
        errorMessage = "deviceId must be a string.";
        return false;
    }

    if (!message.at("sequence").is_number_unsigned())
    {
        errorMessage = "sequence must be an unsigned integer.";
        return false;
    }

    if (!message.at("distance").is_number())
    {
        errorMessage = "distance must be a number.";
        return false;
    }

    if (!message.at("timestamp").is_string())
    {
        errorMessage = "timestamp must be a string.";
        return false;
    }

    const std::string type =
        message.at("type").get<std::string>();

    if (type != "distance")
    {
        errorMessage =
            "Unsupported message type: " + type;
        return false;
    }

    const double distance =
        message.at("distance").get<double>();

    if (distance < 0.0 || distance > 20.0)
    {
        errorMessage =
            "distance must be between 0 and 20 meters.";
        return false;
    }

    return true;
}


// ===============================
// WPF로 데이터 전달
// ===============================
void broadcastToMonitors(const std::string& message)
{
    std::lock_guard<std::mutex> lock(monitorMutex);

    for (auto it = monitorClients.begin();
        it != monitorClients.end();)
    {
        try
        {
            boost::asio::write(
                **it,
                boost::asio::buffer(message)
            );

            ++it;
        }
        catch (const std::exception& e)
        {
            std::cout
                << "WPF disconnected: "
                << e.what()
                << '\n';

            it = monitorClients.erase(it);
        }
    }
}


// ===============================
// Sensor / Simulator 처리
// ===============================
void handleSensorClient(tcp::socket socket)
{
    try
    {
        boost::asio::streambuf receiveBuffer;

        while (true)
        {
            boost::asio::read_until(
                socket,
                receiveBuffer,
                '\n'
            );

            std::istream inputStream(&receiveBuffer);

            std::string receivedMessage;
            std::getline(
                inputStream,
                receivedMessage
            );

            const nlohmann::json parsedMessage =
                nlohmann::json::parse(
                    receivedMessage
                );

            std::string errorMessage;

            if (!validateSensorMessage(
                parsedMessage,
                errorMessage))
            {
                std::cerr
                    << "Validation failed: "
                    << errorMessage
                    << '\n';

                continue;
            }

            const std::string deviceId =
                parsedMessage
                .at("deviceId")
                .get<std::string>();

            const std::uint64_t sequence =
                parsedMessage
                .at("sequence")
                .get<std::uint64_t>();

            const double distance =
                parsedMessage
                .at("distance")
                .get<double>();

            std::cout
                << "[" << deviceId << "] "
                << "seq=" << sequence
                << ", distance="
                << distance
                << " m\n";

            // ★ 핵심
            // getline으로 \n이 제거되었으므로
            // 다시 붙여서 WPF로 전달
            broadcastToMonitors(
                receivedMessage + '\n'
            );
        }
    }
    catch (const std::exception& exception)
    {
        std::cout
            << "Sensor disconnected: "
            << exception.what()
            << '\n';
    }
}


// ===============================
// WPF 연결 받기
// ===============================
void acceptMonitorClients(
    boost::asio::io_context& ioContext,
    tcp::acceptor& monitorAcceptor
)
{
    while (true)
    {
        auto socket =
            std::make_shared<tcp::socket>(
                ioContext
            );

        monitorAcceptor.accept(*socket);

        {
            std::lock_guard<std::mutex>
                lock(monitorMutex);

            monitorClients.push_back(socket);
        }

        std::cout
            << "WPF Monitor connected.\n";
    }
}


int main()
{
    try
    {
        boost::asio::io_context ioContext;

        // Simulator / ESP32용
        tcp::acceptor sensorAcceptor(
            ioContext,
            tcp::endpoint(
                tcp::v4(),
                9000
            )
        );

        // WPF용
        tcp::acceptor monitorAcceptor(
            ioContext,
            tcp::endpoint(
                tcp::v4(),
                9001
            )
        );

        std::cout
            << "=== ISV TCP Server ===\n";

        std::cout
            << "Sensor port  : 9000\n";

        std::cout
            << "Monitor port : 9001\n";


        // WPF 연결 전용 thread
        std::thread monitorThread(
            acceptMonitorClients,
            std::ref(ioContext),
            std::ref(monitorAcceptor)
        );

        monitorThread.detach();


        // Sensor 연결
        while (true)
        {
            tcp::socket socket(ioContext);

            sensorAcceptor.accept(socket);

            std::cout
                << "Sensor connected.\n";

            std::thread sensorThread(
                handleSensorClient,
                std::move(socket)
            );

            sensorThread.detach();
        }
    }
    catch (const std::exception& exception)
    {
        std::cerr
            << "Server error: "
            << exception.what()
            << '\n';

        return 1;
    }

    return 0;
}