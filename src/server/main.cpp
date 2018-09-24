#include "ServerManager.hpp"
#include <signal.h>

int main(int argc, char* argv[]) {

    signal(SIGPIPE, SIG_IGN);

    if(argc!=2) {

        std::cerr << "Usage: " << argv[0] << "input_file" << std::endl;
        return 1;
    }

    ServerManager* server = ServerManager::Instance(8124, argv[1]);

    server->Run();

    ServerManager::Release();
    server = nullptr;

    return 0;
}
