#include "ServerManager.hpp"

ServerManager* ServerManager::sInstance = nullptr;

ServerManager* ServerManager::Instance(const int& port) {

    if(sInstance==nullptr)
        sInstance = new ServerManager(port);

    return sInstance;
}

void ServerManager::Release() {

    delete sInstance;
    sInstance = nullptr;
}

void ServerManager::Run() {

    mSocket->Init(mPort);
    std::thread t(&ServerManager::Update, this);
    t.detach();

    while(mSocket->Valid()) {

        int sockfd = mSocket->Accept();
        std::thread t(&ServerManager::NewConn, this, sockfd);
        t.detach();
    }
}

ServerManager::ServerManager(const int& port) {

    mPort = port;

    mSocket = ServerSocket::Instance();
    mDecoder = Decoder::Instance();
}

ServerManager::~ServerManager() {

    ServerSocket::Release();
    mSocket = nullptr;

    Decoder::Release();
    mDecoder = nullptr;
}

void ServerManager::Update() {

    mDecoder->Run();
}

void ServerManager::NewConn(const int& sockfd) {

    SubSocket* conn = new SubSocket(sockfd);
    conn->SendMessage("hello\r\n");

    std::string msg;
    while(msg!="quit\r\n") {
    //while(1) {
        //canvas->Update(msg);
        //conn->SendMessage(canvas->Output());
        std::this_thread::sleep_for(std::chrono::seconds(4));
        conn->SendMessage(mDecoder->Output());
        msg = conn->GetMessage();
        std::cout << msg << std::endl;
    }
    conn->Disconn();
    //delete canvas;
    delete conn;
}
