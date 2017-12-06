#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <signal.h>


// Для корректной работы freeaddrinfo в MinGW
// Подробнее: http://stackoverflow.com/a/20306451
#define _WIN32_WINNT 0x501

#include <WinSock2.h>
#include <WS2tcpip.h>

// Необходимо, чтобы линковка происходила с DLL-библиотекой
// Для работы с сокетам
#pragma comment(lib, "Ws2_32.lib")

using std::cerr;

class WsaInitializer
{
    WSADATA wsaData;

public:
    WsaInitializer()
    {
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

        // Если произошла ошибка подгрузки библиотеки
        if (result != 0) {
            std::string message = "WSAStartup failed: " + std::to_string(result);
            throw std::runtime_error(message);
        }
    }

    ~WsaInitializer()
    {
        WSACleanup();
    }
};


void send_response(SOCKET client_socket)
{
    const int max_client_buffer_size = 1024;
    char buf[max_client_buffer_size];

    int result = recv(client_socket, buf, max_client_buffer_size, 0);

    if (result == SOCKET_ERROR)
    {
        // ошибка получения данных
        cerr << "recv failed: " << result << "\n";
        closesocket(client_socket);
    }
    else if (result == 0)
    {
        // соединение закрыто клиентом
        cerr << "connection closed...\n";
    }
    else if (result > 0)
    {
        std::stringstream response; // сюда будет записываться ответ клиенту
        std::stringstream response_body; // тело ответа

        // Мы знаем фактический размер полученных данных, поэтому ставим метку конца строки
        // В буфере запроса.
        buf[result] = '\0';

        // Данные успешно получены
        // формируем тело ответа (HTML)
        response_body << "<title>Test C++ HTTP Server</title>\n"
            << "<h1>Test page</h1>\n"
            << "<p>This is body of the test page...</p>\n"
            << "<h2>Request headers</h2>\n"
            << "<pre>" << buf << "</pre>\n"
            << "<em><small>Test C++ Http Server</small></em>\n";

        // Формируем весь ответ вместе с заголовками
        response << "HTTP/1.1 200 OK\r\n"
            << "Version: HTTP/1.1\r\n"
            << "Content-Type: text/html; charset=utf-8\r\n"
            << "Content-Length: " << response_body.str().length()
            << "\r\n\r\n"
            << response_body.str();

        // Отправляем ответ клиенту с помощью функции send
        result = send(client_socket, response.str().c_str(), response.str().length(), 0);

        if (result == SOCKET_ERROR)
        {
            // произошла ошибка при отправле данных
            cerr << "send failed: " << WSAGetLastError() << "\n";
        }
        // Закрываем соединение к клиентом
        closesocket(client_socket);
    }
}

struct AddrInfoPtr
{
    addrinfo* addr = nullptr;

    AddrInfoPtr(const std::string& host, const std::string& port, addrinfo hints)
    {
        int result = getaddrinfo(host.c_str(), port.c_str(), &hints, &addr);

        if (result != 0)
        {
            std::string message = "getaddrinfo failed: " + std::to_string(result);
            throw std::runtime_error(message);
        }
    }

    addrinfo* operator->()
    {
        return addr;
    }

    ~AddrInfoPtr()
    {
        freeaddrinfo(addr);
    }
};

struct Socket
{
    SOCKET sckt;

    Socket(AddrInfoPtr& addr)
    {
        sckt = socket(
            addr->ai_family,
            addr->ai_socktype,
            addr->ai_protocol
        );

        if (sckt == INVALID_SOCKET)
        {
            std::string message = "Error at socket: " + std::to_string(WSAGetLastError());
            throw std::runtime_error(message);
        }
    }

    operator SOCKET()
    {
        return sckt;
    }

    ~Socket()
    {
        closesocket(sckt);
    }

    void Bind(AddrInfoPtr& addr)
    {
        int result = bind(sckt, addr->ai_addr, (int)addr->ai_addrlen);

        if (result == SOCKET_ERROR)
        {
            std::string message = "bind failed with error: " + std::to_string(WSAGetLastError());
            throw std::runtime_error(message);
        }
    }

    void Listen(int backlog)
    {
        if (listen(sckt, backlog) == SOCKET_ERROR)
        {
            std::string message = "listen failed with error: " + std::to_string(WSAGetLastError());
            throw std::runtime_error(message);
        }
    }

    SOCKET Accept(sockaddr * addr, int * addrlen)
    {
        SOCKET client_socket = accept(sckt, addr, addrlen);
        if (client_socket == INVALID_SOCKET)
        {
            std::string message = "accept failed: " + std::to_string(WSAGetLastError());
            throw std::runtime_error(message);
        }
    }
};

bool sigintGot = false;
int main()
{
    WsaInitializer __wsa;

    signal(SIGINT, [](int s) {
        std::cout << "SIGINT got";
        sigintGot = true;
    });

    addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    AddrInfoPtr addr("127.0.0.1", "8000", hints);
    
    Socket listen_socket(addr);
    listen_socket.Bind(addr);
    listen_socket.Listen(SOMAXCONN);

    while (!sigintGot)
    {
        SOCKET client_socket = listen_socket.Accept(nullptr, nullptr);
        std::thread(send_response, client_socket).detach();
    }

    return 0;
}
