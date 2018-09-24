#ifndef CLIENTMANAGER_HPP
#define CLIENTMANAGER_HPP

#include "ClientSocket.hpp"

class ClientManager {

public:

    static ClientManager* Instance(const int& port);
    static void Release();

    void Run();

private:

    static ClientManager* sInstance;

    // private variable
    int mPort;
    ClientSocket* mSocket; 

    ClientManager(const int& port);
    ~ClientManager();
};

#endif
