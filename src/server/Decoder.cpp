#include "Decoder.hpp"

Decoder* Decoder::sInstance = nullptr;

Decoder* Decoder::Instance() {

    if(sInstance==nullptr)
        sInstance = new Decoder();

    return sInstance;
}

void Decoder::Release() {

    delete sInstance;
    sInstance = nullptr;
}

std::string Decoder::Output() {

    return std::to_string(i);
}

Decoder::Decoder() {

    i = 1;
}

Decoder::~Decoder() {
}

void Decoder::Run() {

    while(i<20) {

        std::this_thread::sleep_for(std::chrono::seconds(5));
        i++;
    }
}
