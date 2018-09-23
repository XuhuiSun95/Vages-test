#ifndef SUBSOCKET_HPP
#define SUBSOCKET_HPP

#include <cstring>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

class SubSocket {

public:

    SubSocket(const int& sockfd);
    ~SubSocket();

    std::string GetMessage();
    void SendMessage(const std::string& msg);
    void Disconn();

private:

    int mSockfd, mValread;
    char mBuffer[1024];
};

#endif
