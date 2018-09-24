#include "ServerManager.hpp"

ServerManager* ServerManager::sInstance = nullptr;

ServerManager* ServerManager::Instance(const int& port, const std::string& file) {

    if(sInstance==nullptr)
        sInstance = new ServerManager(port, file);

    return sInstance;
}

void ServerManager::Release() {

    delete sInstance;
    sInstance = nullptr;
}

void ServerManager::Run() {

    mSocket->Init(mPort);
    std::thread t1(&ServerManager::Decode, this);
    t1.detach();

    std::thread t2(&ServerManager::UpdateCount, this);
    t2.detach();

    while(mSocket->Valid()) {

        int sockfd = mSocket->Accept();
        std::thread t(&ServerManager::NewConn, this, sockfd);
        t.detach();
    }
}

ServerManager::ServerManager(const int& port, const std::string& file) {

    mPort = port;
    mSavedImg = 0;

    mSocket = ServerSocket::Instance();
    mDecoder = Decoder::Instance(file);
}

ServerManager::~ServerManager() {

    ServerSocket::Release();
    mSocket = nullptr;
}

void ServerManager::Decode() {

    mDecoder->Run();
    mSavedImg = mDecoder->GetCount();

    Decoder::Release();
    mDecoder = nullptr;
}

void ServerManager::UpdateCount() {

    while(mDecoder!=nullptr) {
        mSavedImg = mDecoder->GetCount();
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

void ServerManager::NewConn(const int& sockfd) {

    SubSocket* conn = new SubSocket(sockfd);
    conn->SendMessage("hello\r\n");

    std::string msg;
    while(msg!="quit\r\n") {

        if(conn->SendCount()>=mSavedImg) {
            if(mDecoder==nullptr)
                break;
            else
                std::this_thread::sleep_for(std::chrono::seconds(2));
        } else {
            conn->SendMessage("Sending [" +
                              std::to_string(conn->SendCount()+1) +
                              "] frame(s).");
            conn->UpdateCount();
            msg = conn->GetMessage();
            if(msg=="error")
                break;
            std::cout << msg << std::endl;
        }
    }
    conn->Disconn();
    delete conn;
}
