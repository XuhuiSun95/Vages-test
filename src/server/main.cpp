#include "ServerManager.hpp"
#include <signal.h>

int main() {

    signal(SIGPIPE, SIG_IGN);

    ServerManager* server = ServerManager::Instance(8124);

    server->Run();

    ServerManager::Release();
    server = nullptr;

    return 0;
}
