#include "SubSocket.hpp"

SubSocket::SubSocket(const int& sockfd) {

    memset(mBuffer, 0, sizeof(mBuffer));
    mSockfd = sockfd;
}

SubSocket::~SubSocket() {
}

std::string SubSocket::GetMessage() {

    mValread = read(mSockfd, mBuffer, 1024);
    std::string message = mBuffer;
    memset(mBuffer, 0, sizeof(mBuffer));

    return message;
}

void SubSocket::SendMessage(const std::string& msg) {

    send(mSockfd, msg.c_str(), msg.length(), 0);
}

void SubSocket::Disconn() {

    close(mSockfd);
}
