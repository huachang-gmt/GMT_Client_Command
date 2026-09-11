#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include <string>
#include <vector>

#pragma comment(lib, "Ws2_32.lib")

int main()
{
    constexpr const char* SERVER_IP = "192.168.137.200";
    constexpr int SERVER_PORT = 9999;

    WSADATA wsa_data{};

    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
    {
        std::cerr << "[ERROR] WSAStartup() failed." << std::endl;
        return 1;
    }

    SOCKET client_socket = socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP);

    if (client_socket == INVALID_SOCKET)
    {
        std::cerr << "[ERROR] socket() failed." << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    if (inet_pton(
            AF_INET,
            SERVER_IP,
            &server_addr.sin_addr) != 1)
    {
        std::cerr << "[ERROR] Invalid server IP." << std::endl;
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    std::cout << "[CONNECT] "
              << SERVER_IP
              << ":"
              << SERVER_PORT
              << std::endl;

    if (connect(
            client_socket,
            reinterpret_cast<sockaddr*>(&server_addr),
            sizeof(server_addr)) == SOCKET_ERROR)
    {
        std::cerr << "[ERROR] connect() failed. Error="
                  << WSAGetLastError()
                  << std::endl;

        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    std::cout << "[OK] Connected to TCP Server." << std::endl;

    const std::vector<std::string> commands =
    {
        "INS 1",
        "STP",
        "SAH M01 M02 M03",
        "SHC M01 2 17 400000 40000 0 128",
        "SHC? M01",
        "SVO",
        "SVF",
        "CAL",
        "DSC",
        "VLS 0.15",
        "MOV R 2000 2000 1000 0 0.5 1",
        "MRV R 2000 2000 1000 0 0.5 1",
        "MSV M02 2000",
        "MSR M06 1",
        "MPV M01 M03 1200.0 -35.5",
        "MPR M01 M03 1200.0 -35.5",
        "MOV?",
        "POS?",
        "PMS?",
        "SPI R U 10 5 2",
        "SPI?",
        "FRS?",
        "DFRS ScanRoutine01",
        "FLM M10 0.2 V 0.01 TH 255",
        "BKN?"
    };

    std::cout << "========================================" << std::endl;
    std::cout << "GMT Client Command" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "0.  Quit" << std::endl;
    std::cout << "1.  INS 1" << std::endl;
    std::cout << "2.  STP" << std::endl;
    std::cout << "3.  SAH M01 M02 M03" << std::endl;
    std::cout << "4.  SHC M01 2 17 400000 40000 0 128" << std::endl;
    std::cout << "5.  SHC? M01" << std::endl;
    std::cout << "6.  SVO" << std::endl;
    std::cout << "7.  SVF" << std::endl;
    std::cout << "8.  CAL" << std::endl;
    std::cout << "9.  DSC" << std::endl;
    std::cout << "10. VLS 0.15" << std::endl;
    std::cout << "11. MOV R 2000 2000 1000 0 0.5 1" << std::endl;
    std::cout << "12. MRV R 2000 2000 1000 0 0.5 1" << std::endl;
    std::cout << "13. MSV M02 2000" << std::endl;
    std::cout << "14. MSR M06 1" << std::endl;
    std::cout << "15. MPV M01 M03 1200.0 -35.5" << std::endl;
    std::cout << "16. MPR M01 M03 1200.0 -35.5" << std::endl;
    std::cout << "17. MOV?" << std::endl;
    std::cout << "18. POS?" << std::endl;
    std::cout << "19. PMS?" << std::endl;
    std::cout << "20. SPI R U 10 5 2" << std::endl;
    std::cout << "21. SPI?" << std::endl;
    std::cout << "22. FRS?" << std::endl;
    std::cout << "23. DFRS ScanRoutine01" << std::endl;
    std::cout << "24. FLM M10 0.2 V 0.01 TH 255" << std::endl;
    std::cout << "25. BKN?" << std::endl;
    std::cout << "========================================" << std::endl;


    while (true)
    {
        std::cout << "GMT> ";

        std::string input;

        if (!std::getline(std::cin, input))
        {
            break;
        }

        if (input == "0" || input == "exit")
        {
            break;
        }

        std::string command;

        try
        {
            const std::size_t menu_index =
                std::stoul(input);

            if (menu_index >= 1 &&
                menu_index <= commands.size())
            {
                command = commands[menu_index - 1];
            }
            else
            {
                command = input;
            }
        }
        catch (...)
        {
            command = input;
        }

        command += "\r\n";

        const int sent = send(
            client_socket,
            command.c_str(),
            static_cast<int>(command.size()),
            0);

        if (sent == SOCKET_ERROR)
        {
            std::cerr << "[ERROR] send() failed. Error="
                    << WSAGetLastError()
                    << std::endl;
            break;
        }

        std::cout << "[TX] " << command;

        char buffer[1024]{};

        const int received = recv(
            client_socket,
            buffer,
            sizeof(buffer) - 1,
            0);

        if (received == SOCKET_ERROR)
        {
            std::cerr << "[ERROR] recv() failed. Error="
                    << WSAGetLastError()
                    << std::endl;
            break;
        }

        if (received == 0)
        {
            std::cout << "[INFO] Server disconnected." << std::endl;
            break;
        }

        buffer[received] = '\0';

        std::cout << "[RX] " << buffer;
    }

    closesocket(client_socket);
    WSACleanup();

    return 0;

}