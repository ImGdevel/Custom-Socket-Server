#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 8080
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

void errorExit(const std::string& message) {
    perror(message.c_str());
    exit(EXIT_FAILURE);
}

// 정확한 크기의 데이터를 송신
ssize_t sendAll(int socket, const char* buffer, size_t length) {
    size_t totalSent = 0;
    while (totalSent < length) {
        ssize_t sent = send(socket, buffer + totalSent, length - totalSent, 0);
        if (sent <= 0) {
            return sent; // 오류 발생
        }
        totalSent += sent;
    }
    return totalSent;
}

// 정확한 크기의 데이터를 수신
ssize_t recvAll(int socket, char* buffer, size_t length) {
    size_t totalReceived = 0;
    while (totalReceived < length) {
        ssize_t received = recv(socket, buffer + totalReceived, length - totalReceived, 0);
        if (received <= 0) {
            return received; // 오류 발생
        }
        totalReceived += received;
    }
    return totalReceived;
}

int main() {
    // 1. 소켓 생성
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        errorExit("Socket creation failed");
    }

    // 2. 서버 주소 설정
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0) {
        errorExit("Invalid address or address not supported");
    }

    // 3. 서버에 연결
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        errorExit("Connection to server failed");
    }
    std::cout << "Connected to server at " << SERVER_IP << ":" << SERVER_PORT << std::endl;

    // 4. 서버에 메시지 전송 (길이 헤더 포함)
    std::string message = "Hello, Server!";
    uint32_t messageLength = htonl(message.size()); // 네트워크 바이트 순서로 변환

    // 송신 데이터 준비: [길이 헤더(4바이트)] + [메시지]
    char sendBuffer[BUFFER_SIZE] = {0};
    memcpy(sendBuffer, &messageLength, sizeof(messageLength)); // 헤더 복사
    memcpy(sendBuffer + sizeof(messageLength), message.c_str(), message.size()); // 메시지 복사

    size_t totalMessageSize = sizeof(messageLength) + message.size();
    if (sendAll(clientSocket, sendBuffer, totalMessageSize) < 0) {
        errorExit("Failed to send message to server");
    }
    std::cout << "Message sent: " << message << std::endl;

    // 5. 서버로부터 응답 수신 (길이 헤더 포함)
    uint32_t receivedMessageLength = 0;
    if (recvAll(clientSocket, (char*)&receivedMessageLength, sizeof(receivedMessageLength)) <= 0) {
        errorExit("Failed to receive message length from server");
    }
    receivedMessageLength = ntohl(receivedMessageLength); // 호스트 바이트 순서로 변환
    if (receivedMessageLength > BUFFER_SIZE - 1) {
        errorExit("Received message is too large");
    }

    char receiveBuffer[BUFFER_SIZE] = {0};
    if (recvAll(clientSocket, receiveBuffer, receivedMessageLength) <= 0) {
        errorExit("Failed to receive message from server");
    }
    std::cout << "Message received from server: " << receiveBuffer << std::endl;

    // 6. 소켓 닫기
    close(clientSocket);
    std::cout << "Connection closed" << std::endl;

    return 0;
}
