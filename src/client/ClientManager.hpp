#ifndef CLIENTMANAGER_HPP
#define CLIENTMANAGER_HPP

#include "ClientSocket.hpp"
#include <fstream>

class ClientManager {

public:

    static ClientManager* Instance(const int& port, const std::string& id);
    static void Release();

    void Run();

private:

    static ClientManager* sInstance;

    // private variable
    int mPort;
    std::string mId;
    int mSavedImg;
    ClientSocket* mSocket; 

    ClientManager(const int& port, const std::string& id);
    ~ClientManager();
};

#endif
