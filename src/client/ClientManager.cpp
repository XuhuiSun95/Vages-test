#include "ClientManager.hpp"

ClientManager* ClientManager::sInstance = nullptr;

ClientManager* ClientManager::Instance(const int& port) {

    if(sInstance==nullptr)
        sInstance = new ClientManager(port);

    return sInstance;
}

void ClientManager::Release() {

    delete sInstance;
    sInstance = nullptr;
}

void ClientManager::Run() {

    mSocket->Init(mPort);
    while(mSocket->Valid()) {

        std::string msg = mSocket->GetMessage();
        std::cout << "Message received: " << msg << std::endl;
    }
}

ClientManager::ClientManager(const int& port) {

    mPort = port;

    mSocket = ClientSocket::Instance();
}

ClientManager::~ClientManager() {

    ClientSocket::Release();
    mSocket = nullptr;
}
