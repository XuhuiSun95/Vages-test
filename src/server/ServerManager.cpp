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
        if(sockfd==-1)
            continue;
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
    conn->SendMessage("hello");

    std::string msg;
    while(msg!="quit") {

        if(conn->SendCount()>=mSavedImg) {
            if(mDecoder==nullptr) {
                conn->SendMessage("done");
                break;
            }
            else
                std::this_thread::sleep_for(std::chrono::seconds(2));
        } else {
            conn->UpdateCount();
            std::string filename = "img_processed/output-" +
                                   std::to_string(conn->SendCount()) +
                                   ".pgm";
            std::ifstream f(filename);
            if(!f)
                std::cerr << "cannot open file" << std::endl;
            std::string ctx((std::istreambuf_iterator<char>(f)),
                             std::istreambuf_iterator<char>());
            f.close();

            std::string size  = std::to_string(ctx.length());
            std::cout << "Sending " << filename << " with size of " << size << std::endl;
            conn->SendMessage(size);
            msg = conn->GetMessage();
            std::cout << msg << std::endl;
            conn->SendMessage(ctx);
            msg = conn->GetMessage();
            if(msg=="error")
                break;
            std::cout << msg << std::endl;
        }
    }
    conn->Disconn();
    delete conn;
}
