#include "ClientManager.hpp"

ClientManager* ClientManager::sInstance = nullptr;

ClientManager* ClientManager::Instance(const int& port, const std::string& id) {

    if(sInstance==nullptr)
        sInstance = new ClientManager(port, id);

    return sInstance;
}

void ClientManager::Release() {

    delete sInstance;
    sInstance = nullptr;
}

void ClientManager::Run() {

    mSocket->Init(mPort);
    mSocket->Handshack();
    while(mSocket->Valid()) {

        std::string msg = mSocket->GetMessage();
        if(msg=="-1") {
            break;
        }
        if(msg=="done") {
            std::cout << "Transmission done" << std::endl;
            break;
        }
        mSavedImg++;
        std::string filename = mId + "-" + std::to_string(mSavedImg) + ".pgm";
        std::ofstream f(filename);
        f << msg;
        f.close();
        std::cout << "Image received, saved to " << filename <<  std::endl;
    }
}

ClientManager::ClientManager(const int& port, const std::string& id) {

    mPort = port;
    mId = id;
    mSavedImg = 0;

    mSocket = ClientSocket::Instance();
}

ClientManager::~ClientManager() {

    ClientSocket::Release();
    mSocket = nullptr;
}
