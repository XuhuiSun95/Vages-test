#include "ClientManager.hpp"

int main() {

    ClientManager* client = ClientManager::Instance(8124);

    client->Run();

    ClientManager::Release();
    client = nullptr;

    return 0;
}
