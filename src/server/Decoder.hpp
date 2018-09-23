#ifndef DECODER_HPP
#define DECODER_HPP

#include <thread>
#include <chrono>
#include <string>

class Decoder {

public:

    static Decoder* Instance();
    static void Release();

    void Run();
    std::string Output();

private:

    static Decoder* sInstance;

    //TODO
    int i;
    
    Decoder();
    ~Decoder();
};

#endif
