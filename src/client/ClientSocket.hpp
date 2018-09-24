#ifndef CLIENTSOCKET_HPP
#define CLIENTSOCKET_HPP

#include <cstring>
#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

class ClientSocket {

public:

    static ClientSocket* Instance();
    static void Release();

    void Init(const int& port);
    bool Valid();
    std::string GetMessage();

private:
    
    static ClientSocket* sInstance;

    bool mValid;
    int mSockfd, mValread;
    struct sockaddr_in mAddress;
    char mBuffer[1024];

    ClientSocket();
    ~ClientSocket();
};

#endif
