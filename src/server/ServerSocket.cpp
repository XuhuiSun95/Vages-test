#include "ServerSocket.hpp"

ServerSocket* ServerSocket::sInstance = nullptr;

ServerSocket* ServerSocket::Instance() {

    if(sInstance==nullptr)
        sInstance = new ServerSocket();

    return sInstance;
}

void ServerSocket::Release() {

    delete sInstance;
    sInstance = nullptr;
}

void ServerSocket::Init(const int& port) {

    if((mSockfd = socket(AF_INET, SOCK_STREAM, 0))==0) {
        std::cout << "Socket creation error" << std::endl;
        return;
    }
    
    //setup socket address structure
    memset(&mAddress, '0', sizeof(mAddress));
    mAddress.sin_family = AF_INET;
    mAddress.sin_port = htons(port);
    mAddress.sin_addr.s_addr = INADDR_ANY;
    
    // call bind to associate the socket with our local address and port
    if(bind(mSockfd, (struct sockaddr *)&mAddress, sizeof(mAddress))<0) {
        std::cout << "Bind Failed" << std::endl;
        return;
    }
    
    // convert the socket to listen for incoming connections
    if(listen(mSockfd, 3)<0) {
        std::cout << "Listen Failed" << std::endl;
        return;
    }

    mValid = true;
}

bool ServerSocket::Valid() {

    return mValid;
}

int ServerSocket::Accept() {

    int addrlen = sizeof(mAddress);
    int new_socket;

    if((new_socket = accept(mSockfd, (struct sockaddr *)&mAddress, (socklen_t*)&addrlen))<0) {
        std::cout << "Accept Failed" << std::endl;
        return -1;
    }

    return new_socket;
}

ServerSocket::ServerSocket() {

    mValid = false;
}

ServerSocket::~ServerSocket() {

    close(mSockfd);
}
