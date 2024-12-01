#include "ClientSession.h"
#include <unistd.h>
#include <iostream>

using namespace std;

ClientSession::ClientSession(int socket) : clientSocket(socket) {
    if(clientSocket < 0){
        throw runtime_error("Invalid clinet socket");
    }
}

ClientSession::~ClientSession(){
    closeSession();
}

int ClientSession::getSocket() const {
    return clientSocket;
}

void ClientSession::appendToBuffer(const char* data, size_t size) {
    receiveBuffer.append(data, size);
}

string& ClientSession::getBuffer() {
    return receiveBuffer;
}

bool ClientSession::isActive() const {
    return active;
}

void ClientSession::closeSession() {
    if (active) {
        close(clientSocket);
        active = false;
    }
}