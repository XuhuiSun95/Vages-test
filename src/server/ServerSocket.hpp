#ifndef SERVERSOCKET_HPP
#define SERVERSOCKET_HPP

#include <cstring>
#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

class ServerSocket {

public:

    static ServerSocket* Instance();
    static void Release();

    void Init(const int& port);
    bool Valid();
    int Accept();

private:

    static ServerSocket* sInstance;

    bool mValid;
    int mSockfd;
    struct sockaddr_in mAddress;

    ServerSocket();
    ~ServerSocket();
};

#endif
