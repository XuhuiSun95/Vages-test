#include "ClientManager.hpp"

int main(int argc, char* argv[]) {

    if(argc!=2) {

        std::cerr << "Usage: " << argv[0] << " <client id>" << std::endl;
        return 1;
    }

    ClientManager* client = ClientManager::Instance(8124, argv[1]);

    client->Run();

    ClientManager::Release();
    client = nullptr;

    return 0;
}
