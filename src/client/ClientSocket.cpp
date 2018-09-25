#include "ClientSocket.hpp"

ClientSocket* ClientSocket::sInstance = nullptr;

ClientSocket* ClientSocket::Instance() {

    if(sInstance==nullptr)
        sInstance = new ClientSocket();

    return sInstance;
}

void ClientSocket::Release() {

    delete sInstance;
    sInstance = nullptr;
}

void ClientSocket::Init(const int& port) {

    if((mSockfd = socket(AF_INET, SOCK_STREAM, 0))==0) {
        std::cout << "Socket creation error" << std::endl;
        return;
    }

    //setup socket address structure
    memset(&mAddress, '0', sizeof(mAddress));
    mAddress.sin_family = AF_INET;
    mAddress.sin_port = htons(port);

    if(inet_pton(AF_INET, "127.0.0.1", &mAddress.sin_addr)<=0) {
        std::cout << "Invalid address/ Address not supported" << std::endl;
        return;
    }

    //socket connection
    if(connect(mSockfd, (struct sockaddr *)&mAddress, sizeof(mAddress))<0) {
        std::cout << "Connection Failed" << std::endl;
    }

    mValid = true;
}

bool ClientSocket::Valid() {

    return mValid;
}

void ClientSocket::Handshack() {

    mValread = read(mSockfd, mBuffer, 1024);
    std::string message = mBuffer;
    memset(mBuffer, 0, sizeof(mBuffer));
}

std::string ClientSocket::GetMessage() {

    int size;
    std::string message;
    std::string reply;

    mValread = read(mSockfd, mBuffer, 1024);
    std::string header = mBuffer;
    memset(mBuffer, 0, sizeof(mBuffer));
    if(header=="done")
        return header;
    if(isdigit(header[0])) {
        size = std::stoi(header);
    
        reply = "okay";
        send(mSockfd, reply.c_str(), reply.length(), 0);
    
        while(size) {
            mValread = read(mSockfd, mBuffer, 1024);
            message += mBuffer;
            size -= mValread;
            memset(mBuffer, 0, sizeof(mBuffer));
        }
        reply = "Success received";
        send(mSockfd, reply.c_str(), reply.length(), 0);

        return message;
    }
    return "-1";
}

ClientSocket::ClientSocket() {

    mValid = false;
}

ClientSocket::~ClientSocket() {

    close(mSockfd);
}
