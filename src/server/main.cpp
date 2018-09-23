#include "ServerManager.hpp"

int main() {

    ServerManager* server = ServerManager::Instance(8124);

    server->Run();

    ServerManager::Release();
    server = nullptr;

    return 0;
}
