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
    // 값의 존재 검사
    if (!message.contains("type"))
    {
        errorMessage = "Missing field: type";
        return false;
    }
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

    if (!message.contains("distanceCm"))
    {
        errorMessage = "Missing field: distanceCm";
        return false;
    }

    if (!message.contains("timestamp"))
    {
        errorMessage = "Missing field: timestamp";
        return false;
    }

    // 타입 검사

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

    if (!message.at("distanceCm").is_number())
    {
        errorMessage = "distanceCm must be a number.";
        return false;
    }

    if (!message.at("timestamp").is_string())
    {
        errorMessage = "timestamp must be a string.";
        return false;
    }

    // 메시지 종류 겁사
    const std::string type =
        message.at("type").get<std::string>();

    if (type != "distance")
    {
        errorMessage = "Unsupported message type: " + type;
        return false;
    }

    // 거리 범위
    const double distanceCm =
        message.at("distanceCm").get<double>();

    if (distanceCm < 0.0 || distanceCm > 2000.0)
    {
        errorMessage =
            "distanceCm must be between 0 and 2000.";

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

        while (true)
        {
            boost::asio::read_until(
                socket,
                receiveBuffer,
                '\n'
            );

            std::istream inputStream(&receiveBuffer);

            std::string receivedMessage;
            std::getline(inputStream, receivedMessage);

            const nlohmann::json parsedMessage =
                nlohmann::json::parse(receivedMessage);

            std::string errorMessage;

            if (!validateSensorMessage(
                parsedMessage,
                errorMessage
            ))
            {
                std::cerr
                    << "Validation failed: "
                    << errorMessage
                    << '\n';

                continue;
            }

            const std::string deviceId =
                parsedMessage.at("deviceId")
                .get<std::string>();

            const std::uint64_t sequence =
                parsedMessage.at("sequence")
                .get<std::uint64_t>();

            const double distanceCm =
                parsedMessage.at("distanceCm")
                .get<double>();

            std::cout
                << "[" << deviceId << "] "
                << "seq=" << sequence
                << ", distance=" << distanceCm
                << " cm\n";
        }

        /*
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

        const std::string type =
            parsedMessage.at("type").get<std::string>();

        const std::string deviceId =
            parsedMessage.at("deviceId").get<std::string>();

        const std::uint64_t sequence =
            parsedMessage.at("sequence").get<std::uint64_t>();

        const double distanceCm =
            parsedMessage.at("distanceCm").get<double>();

        const std::string timestamp =
            parsedMessage.at("timestamp").get<std::string>();


        std::cout << "\n=== Parsed Sensor Data ===\n";
        std::cout << "type: " << type << '\n';
        std::cout << "deviceId: " << deviceId << '\n';
        std::cout << "sequence: " << sequence << '\n';
        std::cout << "distanceCm: " << distanceCm << " cm\n";
        std::cout << "timestamp: " << timestamp << '\n';
        */
    }
    catch (const std::exception& exception)
    {
        std::cerr << "Server error: "
            << exception.what() << '\n';

        return 1;
    }




    return 0;
}