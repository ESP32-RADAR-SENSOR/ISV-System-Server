#include <boost/asio.hpp>
#include <nlohmann/json.hpp>

#include <cstdint>
#include <iostream>
#include <string>

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

    const double distance =
        message.at("distance").get<double>();

    if (distance < 0.0 || distance > 2000.0)
    {
        errorMessage =
            "distance must be between 0 and 2000 cm.";

        return false;
    }

    return true;
}

int main()
{
    try
    {
        using boost::asio::ip::tcp;
        // 네트워크 작업 관리 객체 ioContext 생성
        boost::asio::io_context ioContext;
        // ipv4 9000포트로 들어오는 acceptor 생성
        tcp::acceptor acceptor(
            ioContext,
            tcp::endpoint(tcp::v4(), 9000)
        );

        std::cout << "=== ISV TCP Server ===\n";
        std::cout << "Listening on 127.0.0.1:9000...\n";

        // 통신용 소켓 객체 생성
        tcp::socket socket(ioContext);
        // Client의 접속을 기다렸다가, 접속이 들어오면
        // acceptor가 연결된 소켓을 socket 에 담기
        acceptor.accept(socket);
        // socket은 특정 client와 연결되었고, read/write 가능해짐
        std::cout << "Client connected.\n";

        // 수신 버퍼 생성
        boost::asio::streambuf receiveBuffer;

        // 줄바꿈까지 수신
        boost::asio::read_until(
            socket,
            receiveBuffer,
            '\n'
        );

        // 버퍼를 읽는 스트림 생성 (inputStream이 buffer를 읽는다)
        std::istream inputStream(&receiveBuffer);

        std::string receivedMessage;
        std::getline(inputStream, receivedMessage);

        std::cout << "\n=== Received Message ===\n";
        std::cout << receivedMessage << '\n';

        const nlohmann::json parsedMessage =
            nlohmann::json::parse(receivedMessage);

        std::string errorMessage;

        if (!validateSensorMessage(
            parsedMessage,
            errorMessage
        ))
        {
            std::cerr << "Validation failed: "
                << errorMessage << '\n';

            return 1;
        }


        const std::string deviceId =
            parsedMessage.at("deviceId").get<std::string>();
        const std::uint64_t sequence =
            parsedMessage.at("sequence").get<std::uint64_t>();
        const double distance =
            parsedMessage.at("distance").get<double>();

        std::cout << "\n=== Parsed Sensor Data ===\n";
        std::cout << "deviceId: " << deviceId << '\n';
        std::cout << "sequence: " << sequence << '\n';
        std::cout << "distance: " << distance << " cm\n";

    }
    catch (const std::exception& exception)
    {
        std::cerr << "Server error: "
            << exception.what() << '\n';

        return 1;
    }


    const nlohamn::json paredMessage =
        nolohamn::json::parse(receivedMessage);


    return 0;
}